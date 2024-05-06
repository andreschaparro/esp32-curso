//=====[Libraries]=============================================================

#include <string.h>
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "cJSON.h"
#include "api_rest.h"
#include "api_dht11.h"

//=====[Declaration of private defines]========================================

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

//=====[Declaration of private data types]=====================================

typedef struct rest_server_context
{
    char base_path[ESP_VFS_PATH_MAX + 1];
    char scratch[SCRATCH_BUFSIZE];
} rest_server_context_t;

//=====[Declaration and initialization of private global constants]============

static const char *TAG = "API REST";

static const char *BASE_PATH = "/www";

//=====[Declarations (prototypes) of private functions]========================

static esp_err_t rest_common_get_handler(httpd_req_t *req);

static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath);

static esp_err_t rest_data_get_handler(httpd_req_t *req);

//=====[Implementations of public functions]===================================

/**
 * @brief Inicializa el SPIFFS.
 *
 * @return esp_err_t.
 */
esp_err_t init_fs(void)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");
    esp_vfs_spiffs_conf_t conf = {
        .base_path = BASE_PATH,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true,
    };
    esp_err_t err = esp_vfs_spiffs_register(&conf);
    if (err != ESP_OK)
    {
        if (err == ESP_FAIL)
        {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        }
        else if (err == ESP_ERR_NOT_FOUND)
        {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        }
        else
        {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(err));
        }
        return ESP_FAIL;
    }
    size_t total, used = 0;
    err = esp_spiffs_info(conf.partition_label, &total, &used);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(err));
        ESP_ERROR_CHECK(esp_spiffs_format(conf.partition_label));
        return ESP_FAIL;
    }
    else
    {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
    if (used > total)
    {
        ESP_LOGW(TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
        err = esp_spiffs_check(conf.partition_label);
        if (err != ESP_OK)
        {
            ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(err));
            return ESP_FAIL;
        }
        else
        {
            ESP_LOGI(TAG, "SPIFFS_check() successful");
        }
    }
    return ESP_OK;
}

/**
 * @brief Inicializa el servidor HTTP Restful.
 *
 * @return esp_err_t.
 */
esp_err_t start_rest_server(void)
{
    rest_server_context_t *rest_context = NULL;
    rest_context = calloc(1, sizeof(rest_server_context_t));
    if (rest_context == NULL)
    {
        ESP_LOGE(TAG, "No memory for rest context");
        return ESP_FAIL;
    }
    strlcpy(rest_context->base_path, BASE_PATH, sizeof(rest_context->base_path));
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    ESP_LOGI(TAG, "Starting HTTP Server");
    if (httpd_start(&server, &config) != ESP_OK)
    {
        ESP_LOGE(TAG, "Start server failed");
        free((void *)rest_context);
        return ESP_FAIL;
    }
    httpd_uri_t data_get_uri = {
        .uri = "/api/data",
        .method = HTTP_GET,
        .handler = rest_data_get_handler,
        .user_ctx = NULL};
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &data_get_uri));
    httpd_uri_t common_get_uri = {
        .uri = "/*",
        .method = HTTP_GET,
        .handler = rest_common_get_handler,
        .user_ctx = rest_context};
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &common_get_uri));
    return ESP_OK;
}

//=====[Implementations of private functions]==================================

/**
 * @brief Handler para atender los request de los clientes que soliciten archivos.
 *
 * @param req mensaje de request del cliente.
 * @return esp_err_t.
 */
static esp_err_t rest_common_get_handler(httpd_req_t *req)
{
    char filepath[FILE_PATH_MAX];
    rest_server_context_t *rest_context = (rest_server_context_t *)req->user_ctx;
    strlcpy(filepath, rest_context->base_path, sizeof(filepath));
    if (req->uri[strlen(req->uri) - 1] == '/')
    {
        strlcat(filepath, "/index.html", sizeof(filepath));
    }
    else
    {
        strlcat(filepath, req->uri, sizeof(filepath));
    }
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file : %s", filepath);
        ESP_ERROR_CHECK(httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to read existing file"));
        return ESP_FAIL;
    }
    if (set_content_type_from_file(req, filepath) != ESP_OK)
    {
        ESP_LOGE(TAG, "set_content_type_from_file failed");
        return ESP_FAIL;
    }
    char *chunk = rest_context->scratch;
    size_t read_bytes;
    do
    {
        read_bytes = fread(chunk, sizeof(char), SCRATCH_BUFSIZE, fp);
        if (ferror(fp))
        {
            fclose(fp);
            ESP_LOGE(TAG, "Failed to read file : %s", filepath);
            ESP_ERROR_CHECK(httpd_resp_sendstr_chunk(req, NULL));
            ESP_ERROR_CHECK(httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file"));
            return ESP_FAIL;
        }
        if (read_bytes > 0)
        {
            if (httpd_resp_send_chunk(req, chunk, read_bytes) != ESP_OK)
            {
                fclose(fp);
                ESP_LOGE(TAG, "File sending failed!");
                ESP_ERROR_CHECK(httpd_resp_sendstr_chunk(req, NULL));
                ESP_ERROR_CHECK(httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file"));
                return ESP_FAIL;
            }
        }
    } while (read_bytes > 0);
    fclose(fp);
    ESP_LOGI(TAG, "File sending complete");
    ESP_ERROR_CHECK(httpd_resp_send_chunk(req, NULL, 0));
    return ESP_OK;
}

/**
 * @brief Setea el HTTP response content type en funcion del tipo de archivo.
 *
 * @param req mensaje de request del cliente.
 * @param filepath nombre completo del archivo anteponiendo /.
 * @return esp_err_t.
 */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filepath)
{
    if (strcasecmp(&filepath[strlen(filepath) - strlen(".html")], ".html") == 0)
    {
        return httpd_resp_set_type(req, "text/html");
    }
    else if (strcasecmp(&filepath[strlen(filepath) - strlen(".js")], ".js") == 0)
    {
        return httpd_resp_set_type(req, "application/javascript");
    }
    else if (strcasecmp(&filepath[strlen(filepath) - strlen(".css")], ".css") == 0)
    {
        return httpd_resp_set_type(req, "text/css");
    }
    else if (strcasecmp(&filepath[strlen(filepath) - strlen(".png")], ".png") == 0)
    {
        return httpd_resp_set_type(req, "image/png");
    }
    else if (strcasecmp(&filepath[strlen(filepath) - strlen(".ico")], ".ico") == 0)
    {
        return httpd_resp_set_type(req, "image/x-icon");
    }
    else if (strcasecmp(&filepath[strlen(filepath) - strlen(".svg")], ".svg") == 0)
    {
        return httpd_resp_set_type(req, "text/xml");
    }
    return httpd_resp_set_type(req, "text/plain");
}

/**
 * @brief Handler para atender los request de los clientes que soliciten los valores de medicion.
 *
 * @param req mensaje de request del cliente.
 * @return esp_err_t.
 */
static esp_err_t rest_data_get_handler(httpd_req_t *req)
{
    float temp = 0.0;
    float hum = 0.0;
    read_dht11(&temp, &hum);
    if (httpd_resp_set_type(req, "application/json") != ESP_OK)
    {
        ESP_LOGE(TAG, "set_content_type_from_file failed");
        return ESP_FAIL;
    }
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temp", temp);
    cJSON_AddNumberToObject(root, "hum", hum);
    const char *sys_info = cJSON_Print(root);
    ESP_ERROR_CHECK(httpd_resp_sendstr(req, sys_info));
    free((void *)sys_info);
    cJSON_Delete(root);
    return ESP_OK;
}