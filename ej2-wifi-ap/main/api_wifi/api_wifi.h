//=====[#include guards - begin]===============================================

#ifndef _API_WIFI_H_
#define _API_WIFI_H_

//=====[Declarations (prototypes) of public functions]=========================

void wifi_init_sta(const char *ssid, const char *password);

void wifi_init_softap(const char *ssid, const char *password);

//=====[#include guards - end]=================================================

#endif // _API_WIFI_H_