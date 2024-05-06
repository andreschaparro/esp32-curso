//=====[Libraries]=============================================================

#include <string.h>
#include "esp_log.h"
#include "esp_vfs.h"
#include "esp_spiffs.h"
#include "api_spiffs.h"

//=====[Declaration of private defines]========================================

#define FILE_PATH_MAX (ESP_VFS_PATH_MAX + 128)
#define SCRATCH_BUFSIZE (10240)

//=====[Declaration and initialization of private global constants]============

static const char *TAG = "API SPIFFS";

static const char *BASE_PATH = "/www";

static char scratch[SCRATCH_BUFSIZE];

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
 * @brief Lee el contenido de un archivo y lo muestra en el ESP-IDF Monitor device.
 *
 * @param filename nombre completo del archivo anteponiendo /.
 * @return esp_err_t.
 */
esp_err_t read_file(const char *filename)
{
    ESP_LOGI(TAG, "Reading file");
    char filepath[FILE_PATH_MAX];
    strlcpy(filepath, BASE_PATH, sizeof(filepath));
    strlcat(filepath, filename, sizeof(filepath));
    FILE *fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    memset(scratch, '\0', sizeof(scratch));
    size_t read_bytes;
    read_bytes = fread(scratch, sizeof(char), SCRATCH_BUFSIZE, fp);
    if (ferror(fp))
    {
        fclose(fp);
        ESP_LOGE(TAG, "Failed to read file : %s", filepath);
        return ESP_FAIL;
    }
    else if (feof(fp))
    {
        if (read_bytes > 0)
        {
            ESP_LOGI(TAG, "%s", scratch);
        }
    }
    fclose(fp);
    return ESP_OK;
}