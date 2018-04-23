/*
 * ble_system.h
 *
 *  Created on: 22 Apr 2018
 *      Author: owner
 */

#ifndef MAIN_BLE_SYSTEM_H_
#define MAIN_BLE_SYSTEM_H_



#endif /* MAIN_BLE_SYSTEM_H_ */
#include "ble_config.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "driver/uart.h"
#include "string.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"



void init_bluetooth_controller(esp_bt_mode_t mode)

	{
		   /* Initialize NVS — it is used to store PHY calibration data */
		esp_err_t ret = nvs_flash_init();
		if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
			ESP_ERROR_CHECK(nvs_flash_erase());
			ret = nvs_flash_init();
			}
		ESP_ERROR_CHECK( ret );

		//ESP_LOGI(MY_DEMO_TAG, "BL controler enabling \n ");

		//setup initialise and enable controller with system defaults
		esp_bt_controller_config_t bl_cont_conf = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

		esp_bt_controller_init(&bl_cont_conf);

		esp_bt_controller_enable(mode);


		//ESP_LOGI(MY_DEMO_TAG, "Bluetooth controller running...\n");



	}



void init_bluetooth_stack()

	{

		esp_bluedroid_init();


		esp_bluedroid_enable();

	}




// return current status of ESP Bluetooth stack
esp_bluedroid_status_t ble_get_current_status(void)

	{

	esp_bluedroid_status_t status;

	status = esp_bluedroid_get_status();

	return status;
	}


esp_bluedroid_status_t ble_begin(){

	init_bluetooth_controller(ESP_BT_MODE_BLE);

	init_bluetooth_stack();

	return ble_get_current_status();







}
