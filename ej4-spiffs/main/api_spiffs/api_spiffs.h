//=====[#include guards - begin]===============================================

#ifndef _API_SPIFFS_H_
#define _API_SPIFFS_H_

//=====[Declarations (prototypes) of public functions]=========================

esp_err_t init_fs(void);

esp_err_t read_file(const char *filename);

//=====[#include guards - end]=================================================

#endif // _API_SPIFFS_H_