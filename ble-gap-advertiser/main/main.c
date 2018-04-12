
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
#include "templates.h"


#define MY_DEMO_TAG "BT demo Gee"

#define TEST_DEVICE_NAME "TestDEV1"


static uint8_t adv_config_done = 0;
#define adv_config_flag      (1 << 0)
#define scan_rsp_config_flag (1 << 1)


// Initises Bluetooth controller 
void init_bl(esp_bt_mode_t mode)
	
	{	
		   /* Initialize NVS — it is used to store PHY calibration data */
		esp_err_t ret = nvs_flash_init();
		if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
			ESP_ERROR_CHECK(nvs_flash_erase());
			ret = nvs_flash_init();
			}
		
		
		ESP_ERROR_CHECK( ret );
		ESP_LOGI(MY_DEMO_TAG, "BL controler enabling \n ");
		
		esp_bt_controller_enable(mode);
		esp_bt_controller_config_t bl_cont_conf = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
	
		esp_bt_controller_init(&bl_cont_conf);
	
	
		esp_bt_controller_enable(ESP_BT_MODE_BLE);
	
		ESP_LOGI(MY_DEMO_TAG, "Bluetooth controller running...\n");



}

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
		ESP_LOGI(MY_DEMO_TAG, "Set con params recieved need to do\n");
	    break;
    default:
	    //ESP_LOGI(MY_DEMO_TAG," event: %x params: %x /n", event , param); 
	     break;
}

}


void app_main()
	{
	
	init_bl(ESP_BT_MODE_BLE);
	
	
	//Initiase BlueTooth stack
	
	 esp_bluedroid_init();
	 
	 
	 ESP_ERROR_CHECK(esp_bluedroid_enable());
	 
	 
	 esp_bluedroid_status_t status;
	 
	 //Check Stack is up and running 
	 status = esp_bluedroid_get_status();
	 if( status == ESP_BLUEDROID_STATUS_ENABLED)
	 {
	 
	 ESP_LOGI(MY_DEMO_TAG, "Bludroid enabled\n");
	 
	 }
	 else 
	 {
	  ESP_LOGE(MY_DEMO_TAG, "Bludroid FAILLLLLLL\n");
	 }
	 
	 
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