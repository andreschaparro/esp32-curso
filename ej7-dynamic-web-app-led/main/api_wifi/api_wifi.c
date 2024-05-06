//=====[Libraries]=============================================================

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_mac.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "api_wifi.h"

//=====[Declaration and initialization of private global constants]============

static const uint8_t WIFI_CHANNEL = 1;
static const uint8_t MAX_STA_CONN = 4;

static const char *TAG = "API Wi-Fi";

static const char *H2E_IDENTIFIER = "";

static const EventBits_t WIFI_CONNECTED_BIT = BIT0;

//=====[Declaration and initialization of private global variables]============

static EventGroupHandle_t s_wifi_event_group;

//=====[Declarations (prototypes) of private functions]========================

static esp_netif_t *wifi_init_sta(const char *ssid, const char *password);

static esp_netif_t *wifi_init_softap(const char *ssid, const char *password);

static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data);

//=====[Implementations of public functions]===================================

/**
 * @brief Inicializa la interfaz Wi-Fi en modo sta, ap, o ap/sta.
 *
 * @param mode modo de inicializar la interfaz: WIFI_MODE_STA, WIFI_MODE_AP, o WIFI_MODE_APSTA.
 * @param ssid ssid de la red en modo WIFI_MODE_STA o WIFI_MODE_APSTA. Caso contrario, pasarle NULL.
 * @param ssid_password password de la red en modo WIFI_MODE_STA o WIFI_MODE_APSTA. Caso contrario, pasarle NULL.
 * @param ap_ssid ssid del ESP32 en modo WIFI_MODE_AP o WIFI_MODE_APSTA. Caso contrario, pasarle NULL.
 * @param ap_password password del ESP32 en modo WIFI_MODE_AP o WIFI_MODE_APSTA. Caso contrario, pasarle NULL.
 */
void wifi_init_softap_sta(wifi_mode_t mode, const char *ssid, const char *ssid_password, const char *ap_ssid, const char *ap_password)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    s_wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &event_handler,
        NULL,
        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT,
        IP_EVENT_STA_GOT_IP,
        &event_handler,
        NULL,
        NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));
    if (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA)
    {
        assert(ap_ssid != NULL);
        assert(ap_password != NULL);
        ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
        esp_netif_t *ap_netif = wifi_init_softap(ap_ssid, ap_password);
        assert(ap_netif != NULL);
    }
    if (mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA)
    {
        assert(ssid != NULL);
        assert(ssid_password != NULL);
        ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
        esp_netif_t *sta_netif = wifi_init_sta(ssid, ssid_password);
        assert(sta_netif != NULL);
    }
    ESP_ERROR_CHECK(esp_wifi_start());
    EventBits_t bits = xEventGroupWaitBits(
        s_wifi_event_group,
        WIFI_CONNECTED_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s", ssid, ssid_password);
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

//=====[Implementations of private functions]==================================

/**
 * @brief Inicializa la interfaz Wi-Fi como sta.
 *
 * @param ssid ssid de la red.
 * @param password password de la red.
 * @return esp_netif_t*.
 */
static esp_netif_t *wifi_init_sta(const char *ssid, const char *password)
{
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    wifi_config_t sta_config = {
        .sta = {
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .sae_pwe_h2e = WPA3_SAE_PWE_BOTH,
        },
    };
    strlcpy((char *)sta_config.sta.ssid, ssid, sizeof(sta_config.sta.ssid));
    strlcpy((char *)sta_config.sta.password, password, sizeof(sta_config.sta.password));
    strlcpy((char *)sta_config.sta.sae_h2e_identifier, H2E_IDENTIFIER, sizeof(sta_config.sta.sae_h2e_identifier));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
    ESP_LOGI(TAG, "wifi_init_sta finished");
    return sta_netif;
}

/**
 * @brief Inicializa la interfaz Wi-Fi como ap.
 *
 * @param ssid ssid de la red.
 * @param password password de la red.
 * @return esp_netif_t*.
 */
static esp_netif_t *wifi_init_softap(const char *ssid, const char *password)
{
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
    wifi_config_t ap_config = {
        .ap = {
            .channel = WIFI_CHANNEL,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .required = true,
            },
        },
    };
    if (strlen(password) == 0)
    {
        ap_config.ap.pmf_cfg.required = false;
        ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    strlcpy((char *)ap_config.ap.ssid, ssid, sizeof(ap_config.ap.ssid));
    ap_config.ap.ssid_len = strlen(ssid);
    strlcpy((char *)ap_config.ap.password, password, sizeof(ap_config.ap.password));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%" PRIu8, ssid, password, WIFI_CHANNEL);
    return ap_netif;
}

/**
 * @brief Maneja los eventos de la conexion Wi-Fi.
 *
 * @param arg no se utiliza.
 * @param event_base nos dice el tipo de evento: Wi-Fi o IP.
 * @param event_id nos dice el nombre del evento.
 * @param event_data nos entrega informacion del evento.
 */
static void event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        ESP_ERROR_CHECK(esp_wifi_connect());
        ESP_LOGI(TAG, "Station started");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        ESP_ERROR_CHECK(esp_wifi_connect());
        ESP_LOGI(TAG, "retry to connect to the AP");
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "Station " MACSTR " join, AID=%" PRIu8, MAC2STR(event->mac), event->aid);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "Station " MACSTR " left, AID=%" PRIu8, MAC2STR(event->mac), event->aid);
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}