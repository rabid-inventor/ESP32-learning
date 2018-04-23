


#include "ble_system.h"
//#include "ble_config.h"
#include "esp_gap_ble_api.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
//#include "esp_bt.h"
#include "driver/uart.h"
#include "string.h"
#include "esp_bt.h"
#include "esp_bt_main.h"


#include "esp_gap_ble_api.h"



static uint8_t adv_config_done = 0;
#define adv_config_flag      (1 << 0)
#define scan_rsp_config_flag (1 << 1)


// Initises Bluetooth controller 


//Handler for advertising 
void gap_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param){
 ESP_LOGI(MY_DEMO_TAG, "BLE Event detected");
switch (event) {
		
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
         adv_config_done &= (~adv_config_flag);
         if (adv_config_done == 0){
             esp_ble_gap_start_advertising(&default_test_adv_params);
         }
         
		 break;
		 
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
         adv_config_done &= (~scan_rsp_config_flag);
         if (adv_config_done == 0){
             esp_ble_gap_start_advertising(&default_test_adv_params);
         }
         
		 break;
    
	case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
    //advertising start complete event to indicate advertising start successfully or failed
         if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS) {
             ESP_LOGE(MY_DEMO_TAG, "Advertising start failed\n");
         }
         
		 break;
	case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:

	       ESP_LOGI(MY_DEMO_TAG, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
	                  param->update_conn_params.status,
	                  param->update_conn_params.min_int,
	                  param->update_conn_params.max_int,
	                  param->update_conn_params.conn_int,
	                  param->update_conn_params.latency,
	                  param->update_conn_params.timeout);
	       //esp_ble_gap_update_conn_params(&update_conn_params);
		ESP_LOGI(MY_DEMO_TAG, "Set con params recieved need to do\n");
	    //ESP_LOGI(MY_DEMO_TAG," event: %x params: %x /n", event , param);
	    break;
    default:
	    //ESP_LOGI(MY_DEMO_TAG," event: %x params: %x /n", event , param); 
	     break;
}

}




void app_main()
	{
	
	ble_begin();
	
	//Initiase BlueTooth stack
	
	 
	esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(TEST_DEVICE_NAME);
	
	if (set_dev_name_ret){
		ESP_LOGE(MY_DEMO_TAG, "set device name failed, error code = %x", set_dev_name_ret);
	}
	
	
	
	esp_err_t ret = esp_ble_gap_config_adv_data(&default_adv_data);
	
	if (ret){
		ESP_LOGE(MY_DEMO_TAG, "config adv data failed, error code = %x", ret);
	}
	adv_config_done |= adv_config_flag;
	
	//config scan response data
	ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
	
	if (ret){
		ESP_LOGE(MY_DEMO_TAG, "config scan response data failed, error code = %x", ret);
	}
	adv_config_done |= scan_rsp_config_flag;

    ret = esp_ble_gap_register_callback(gap_handler);
	
    if (ret){
        ESP_LOGE(MY_DEMO_TAG, "gap register error, error code = %x", ret);
        return;
    }
	 ESP_LOGI(MY_DEMO_TAG, "GAP CB Up");
	 
	     if (adv_config_done == 0){
             esp_ble_gap_start_advertising(&default_test_adv_params);
			 ESP_LOGI(MY_DEMO_TAG, "Advert start");
         }
		 
		 ESP_ERROR_CHECK(esp_ble_gap_start_advertising(&default_test_adv_params));
		 
         
}
