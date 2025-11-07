#pragma once
#include "services/eth_service.h"
#include "services/device_service.h"

void app_ble_init();
void app_ble_sleep();
void app_disconnect();
void app_delete_bonds();
void app_advertising_start(bool erase_bonds);
void app_advertising_stop();

extern eth_service_t eth_service;
extern device_service_t device_service;
