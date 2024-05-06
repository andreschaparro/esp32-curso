//=====[#include guards - begin]===============================================

#ifndef _API_WIFI_H_
#define _API_WIFI_H_

//=====[Declarations (prototypes) of public functions]=========================

void wifi_init_softap_sta(wifi_mode_t mode, const char *ssid, const char *ssid_password, const char *ap_ssid, const char *ap_password);

//=====[#include guards - end]=================================================

#endif // _API_WIFI_H_