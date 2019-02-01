/* CAN Network Master Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/*
 * The following example demonstrates a master node in a CAN network. The master
 * node is responsible for initiating and stopping the transfer of data messages.
 * The example will execute multiple iterations, with each iteration the master
 * node will do the following:
 * 1) Start the CAN driver
 * 2) Repeatedly send ping messages until a ping response from slave is received
 * 3) Send start command to slave and receive data messages from slave
 * 4) Send stop command to slave and wait for stop response from slave
 * 5) Stop the CAN driver
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/can.h"
#include "canopen.h"
#include "Kinco_OD.h"

/* --------------------- Definitions and static variables ------------------ */
//Example Configuration
#define PING_PERIOD_MS          100
#define NO_OF_DATA_MSGS         50
#define NO_OF_ITERS             3
#define ITER_DELAY_MS           1000
#define RX_TASK_PRIO            8
#define TX_TASK_PRIO            9
#define PROTOCOL_TSK_PRIO       10
#define CTRL_TSK_PRIO       	11
#define TX_GPIO_NUM             21
#define RX_GPIO_NUM             22
#define EXAMPLE_TAG             "CAN Master"


static const can_timing_config_t t_config = CAN_TIMING_CONFIG_500KBITS();
static const can_filter_config_t f_config = CAN_FILTER_CONFIG_ACCEPT_ALL();
static const can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, CAN_MODE_NORMAL);

static QueueHandle_t tx_task_queue;
static QueueHandle_t rx_task_queue;
static QueueHandle_t motor_tx_task_queue;
static QueueHandle_t motor_rx_task_queue;
static SemaphoreHandle_t stop_ping_sem;
static SemaphoreHandle_t ctrl_task_sem;
static SemaphoreHandle_t done_sem;

/* --------------------------- Tasks and Functions -------------------------- */

static void can_receive_task(void *arg)
{
    while (1) 
    { 
        can_message_t rx_msg;
        rtx_task_action_t action;

        if(can_receive(&rx_msg, 1)==ESP_OK)
        {
	        if (rx_msg.identifier == ID_SLAVE_SDO_RESP) 
	        {
	            uint32_t data = 0;
	            for (int i = 4; i < rx_msg.data_length_code; i++) 
	            {
	                data |= (rx_msg.data[i] << (i * 8));
	            }
	            //ESP_LOGI(EXAMPLE_TAG, "Received data value %d", data);
	           
	        } 
	        else
	        {
	        	action.identifier = rx_msg.identifier;
	        	action.pdo.len = rx_msg.data_length_code;
	        	action.pdo.data_0 = rx_msg.data[0];
	        	action.pdo.data_1 = rx_msg.data[1];
	        	action.pdo.data_2 = rx_msg.data[2];
	        	action.pdo.data_3 = rx_msg.data[3];
	        	action.pdo.data_4 = rx_msg.data[4];
	        	action.pdo.data_5 = rx_msg.data[5];
	        	action.pdo.data_6 = rx_msg.data[6];
	        	action.pdo.data_7 = rx_msg.data[7];
	        }
   
        	xQueueSend(rx_task_queue, &action, 1);
        	//ESP_LOGI(EXAMPLE_TAG, "rx on");
        } 
        //vTaskDelay(pdMS_TO_TICKS(10));
        
    }
    vTaskDelete(NULL);
}

static void can_transmit_task(void *arg) 
{
	can_message_t tx_task_message;
	union can_int32_t tx_task_data;
	union can_int32_t tx_task_od;
    while (1) 
    {
        rtx_task_action_t action;
        xQueueReceive(tx_task_queue, &action, portMAX_DELAY);
		tx_task_od.all = action.od.all;
            
        if (action.cmd == SDO_READ_CMD) //read sdo cmd
        {
            tx_task_message.identifier = action.identifier;
            tx_task_message.data_length_code = 8;
            tx_task_message.flags = CAN_MSG_FLAG_NONE;
            tx_task_message.data[0] = SDO_READ_CMD;
            tx_task_message.data[1] = tx_task_od.byte.byte_2;
            tx_task_message.data[2] = tx_task_od.byte.byte_3;
            tx_task_message.data[3] = tx_task_od.byte.byte_1;
              
            can_transmit(&tx_task_message, portMAX_DELAY);
            //ESP_LOGI(EXAMPLE_TAG, "Transmitted sdo command");
            //vTaskDelay(pdMS_TO_TICKS(PING_PERIOD_MS));
        } 
        else if (action.cmd == SDO_WRITE_CMD)  // write sdo cmd
        {
        
        	switch(tx_task_od.byte.byte_0)
        	{
        		case 0x08:
        			tx_task_message.data[0] = SDO_WRITE_BYTE_CMD;
        			break;	
        		case 0x10:
        			tx_task_message.data[0] = SDO_WRITE_2BYTE_CMD;
        			break;	
        		case 0x20:
        			tx_task_message.data[0] = SDO_WRITE_4BYTE_CMD;
        			break;	
        		default:
        			tx_task_message.data[0] = SDO_WRITE_4BYTE_CMD;
        			break;	
        	}
            tx_task_data.all = action.data.all;
            tx_task_message.identifier = action.identifier;
            tx_task_message.data_length_code = 8;;
            tx_task_message.data_length_code = 8;
            tx_task_message.flags = CAN_MSG_FLAG_NONE;
            tx_task_message.data[1] = tx_task_od.byte.byte_2;
            tx_task_message.data[2] = tx_task_od.byte.byte_3;
            tx_task_message.data[3] = tx_task_od.byte.byte_1;
            tx_task_message.data[4] = tx_task_data.byte.byte_0;
            tx_task_message.data[5] = tx_task_data.byte.byte_1;
            tx_task_message.data[6] = tx_task_data.byte.byte_2;
            tx_task_message.data[7] = tx_task_data.byte.byte_3;
              
            can_transmit(&tx_task_message, portMAX_DELAY);
            //ESP_LOGI(EXAMPLE_TAG, "Transmitted sdo command");
            //vTaskDelay(pdMS_TO_TICKS(PING_PERIOD_MS));
        } 
        else if (action.cmd == TPDO_CMD)  // write pdo cmd
        {
        
            tx_task_data.all = action.data.all;
            tx_task_message.identifier = action.identifier;
            tx_task_message.data_length_code = action.pdo.len;
            tx_task_message.flags = CAN_MSG_FLAG_NONE;
            tx_task_message.data[0] = action.pdo.data_0;
            tx_task_message.data[1] = action.pdo.data_1;
            tx_task_message.data[2] = action.pdo.data_2;
            tx_task_message.data[3] = action.pdo.data_3;
            tx_task_message.data[4] = action.pdo.data_4;
            tx_task_message.data[5] = action.pdo.data_5;
            tx_task_message.data[6] = action.pdo.data_6;
            tx_task_message.data[7] = action.pdo.data_7;
              
            can_transmit(&tx_task_message, portMAX_DELAY);
            //ESP_LOGI(EXAMPLE_TAG, "Transmitted sdo command");
            //vTaskDelay(pdMS_TO_TICKS(PING_PERIOD_MS));
        } 
        //vTaskDelay(pdMS_TO_TICKS(2));
    }
    vTaskDelete(NULL);
}

void canopen_protocol_task(void *arg) 
{
    xSemaphoreTake(ctrl_task_sem, portMAX_DELAY);
    rtx_task_action_t tx_action;
    rtx_task_action_t rx_action;
	motor_tx_t motor_tx;
	motor_rx_t motor_rx;
    
    ESP_ERROR_CHECK(can_start());
    ESP_LOGI(EXAMPLE_TAG, "protocol started");

    while(1)
    {

		if(xQueueReceive(motor_tx_task_queue, &motor_tx, 1) == pdPASS)
		{
		    tx_action.identifier = TPDO1_ID;
		    tx_action.cmd = TPDO_CMD;
		    tx_action.pdo.len = TPDO1_LEN;
		    tx_action.pdo.data_0 = motor_tx.operation_mode;
		    tx_action.pdo.data_1 = motor_tx.control_word.byte.byte_0;
		    tx_action.pdo.data_2 = motor_tx.control_word.byte.byte_1;
		    
		    xQueueSend(tx_task_queue, &tx_action, portMAX_DELAY);
		    
		    tx_action.identifier = TPDO2_ID;
		    tx_action.cmd = TPDO_CMD;
		    tx_action.pdo.len = TPDO2_LEN;
		    tx_action.pdo.data_0 = motor_tx.target_speed.byte.byte_0;
		    tx_action.pdo.data_1 = motor_tx.target_speed.byte.byte_1;
		    tx_action.pdo.data_2 = motor_tx.target_speed.byte.byte_2;
		    tx_action.pdo.data_3 = motor_tx.target_speed.byte.byte_3;
		    tx_action.pdo.data_4 = motor_tx.target_position.byte.byte_0;
		    tx_action.pdo.data_5 = motor_tx.target_position.byte.byte_1;
		    tx_action.pdo.data_6 = motor_tx.target_position.byte.byte_2;
		    tx_action.pdo.data_7 = motor_tx.target_position.byte.byte_3;
		    
		    xQueueSend(tx_task_queue, &tx_action, portMAX_DELAY);
        }
        
        if(xQueueReceive(rx_task_queue, &rx_action, 1) == pdPASS)
        {
        	if(rx_action.identifier == RPDO1_ID)
        	{
        		if(rx_action.pdo.len == RPDO1_LEN)
        		{
        			motor_rx.operation_mode_buff = rx_action.pdo.data_0;
        			motor_rx.status_word.byte.byte_0 = rx_action.pdo.data_1;
        			motor_rx.status_word.byte.byte_1 = rx_action.pdo.data_2;
        			motor_rx.error_state.byte.byte_0 = rx_action.pdo.data_3;
        			motor_rx.error_state.byte.byte_1 = rx_action.pdo.data_4;
        			motor_rx.error_state2.byte.byte_0 = rx_action.pdo.data_5;
        			motor_rx.error_state2.byte.byte_1 = rx_action.pdo.data_6;
        			
        			xQueueSend(motor_rx_task_queue, &motor_rx, portMAX_DELAY);
        			
        		}
        	}
        	if(rx_action.identifier == RPDO2_ID)
        	{
        		if(rx_action.pdo.len == RPDO2_LEN)
        		{
        			motor_rx.speed_real.byte.byte_0 = rx_action.pdo.data_0;
        			motor_rx.speed_real.byte.byte_1 = rx_action.pdo.data_1;
        			motor_rx.speed_real.byte.byte_2 = rx_action.pdo.data_2;
        			motor_rx.speed_real.byte.byte_3 = rx_action.pdo.data_3;
        			motor_rx.pos_actual.byte.byte_0 = rx_action.pdo.data_4;
        			motor_rx.pos_actual.byte.byte_1 = rx_action.pdo.data_5;
        			motor_rx.pos_actual.byte.byte_2 = rx_action.pdo.data_6;
        			motor_rx.pos_actual.byte.byte_3 = rx_action.pdo.data_7;
        			
        			xQueueSend(motor_rx_task_queue, &motor_rx, portMAX_DELAY);
        			
        		}
        	}
        
        }
        //vTaskDelay(pdMS_TO_TICKS(10));
                
    }
    //Stop TX and RX tasks
    xQueueSend(tx_task_queue, &tx_action, portMAX_DELAY);
    xQueueSend(rx_task_queue, &rx_action, portMAX_DELAY);

    //Delete Control task
    xSemaphoreGive(done_sem);
    vTaskDelete(NULL);
}


void motor_control_task(void *arg) 
{
	motor_tx_t motor_tx;
	motor_rx_t motor_rx;
	int32_t my_speed=0;
	float i=0;
	
	ESP_LOGI(EXAMPLE_TAG, "motor started");
	
		motor_tx.operation_mode = -3;		
		motor_tx.control_word.all = 0x86;
		
		xQueueSend(motor_tx_task_queue, &motor_tx, portMAX_DELAY);
	
	while(1)
	{
		xQueueReceive(motor_rx_task_queue, &motor_rx, 1);
		if(motor_rx.status_word.bit.Ready_on==1)
		{
			motor_tx.control_word.all = 0xf;
		}
		if(motor_rx.status_word.bit.Fault==1)
		{
			ESP_LOGI(EXAMPLE_TAG, "motor error:%04x", motor_rx.error_state.all);
			motor_tx.control_word.all = 0x6;
		}
		if(motor_rx.status_word.bit.Operation_enable==1)
		{
			//ESP_LOGI(EXAMPLE_TAG, "motor ready %d",motor_rx.status_word.bit.Ready_on);	
			
				i=i+1;
				my_speed = sin(i/360)*2000000;
				//if(i>3600)
				//	i=0;
				motor_tx.target_speed.all = my_speed;	
			//ESP_LOGI(EXAMPLE_TAG, "motor real_speed:%d", motor_rx.speed_real.all);
			//xQueueSend(motor_tx_task_queue, &motor_tx, portMAX_DELAY);
			
		}
		
		xQueueSend(motor_tx_task_queue, &motor_tx, portMAX_DELAY);
		vTaskDelay(pdMS_TO_TICKS(8));
	
		
	}
	
}


void app_main()
{
    //Create tasks, queues, and semaphores
    rx_task_queue = xQueueCreate(1, sizeof(rtx_task_action_t));
    tx_task_queue = xQueueCreate(1, sizeof(rtx_task_action_t));
    motor_tx_task_queue = xQueueCreate(1, sizeof(motor_tx_t));
    motor_rx_task_queue = xQueueCreate(1, sizeof(motor_rx_t));
    
    ctrl_task_sem = xSemaphoreCreateBinary();
    stop_ping_sem = xSemaphoreCreateBinary();
    done_sem = xSemaphoreCreateBinary();
    xTaskCreatePinnedToCore(can_receive_task, "CAN_rx", 4096, NULL, RX_TASK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(can_transmit_task, "CAN_tx", 4096, NULL, TX_TASK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(canopen_protocol_task, "CAN_protocol", 4096, NULL, PROTOCOL_TSK_PRIO, NULL, tskNO_AFFINITY);
    xTaskCreatePinnedToCore(motor_control_task, "MOTOR_ctrl", 4096, NULL, CTRL_TSK_PRIO, NULL, tskNO_AFFINITY);

    //Install CAN driver
    ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
    ESP_LOGI(EXAMPLE_TAG, "Driver installed");

    xSemaphoreGive(ctrl_task_sem);              //Start control task
    xSemaphoreTake(done_sem, portMAX_DELAY);    //Wait for completion

    //Uninstall CAN driver
    ESP_ERROR_CHECK(can_driver_uninstall());
    ESP_LOGI(EXAMPLE_TAG, "Driver uninstalled");

    //Cleanup
    vQueueDelete(rx_task_queue);
    vQueueDelete(tx_task_queue);
    vSemaphoreDelete(ctrl_task_sem);
    vSemaphoreDelete(stop_ping_sem);
    vSemaphoreDelete(done_sem);
}
