// Copyright 2015-2018 Kinco LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#define Pos_Actual             0x60630020
#define Target_Position        0x607A0020
#define Speed_Real             0x606c0020
#define Target_Speed           0x60ff0020
#define Control_Word           0x60400010
#define Status_Word            0x60410010
#define I_q                    0x60780010
#define Operation_Mode         0x60600008
#define Operation_Mode_Buff    0x60610008
#define Profile_Speed          0x60810020
#define Profile_Acce           0x60830020
#define Profile_Dece           0x60840020
#define Error_State            0x26010010
#define Error_State2           0x26020010
#define Temp_Device            0x60f70b10
#define Driver_IIt_Real        0x60f61010
#define Motor_IIt_Real         0x60f61210

#define TPDO1_ID		0x201
#define TPDO1_LEN		3

#define TPDO2_ID		0x301
#define TPDO2_LEN		8

#define RPDO1_ID		0x181
#define RPDO1_LEN		7

#define RPDO2_ID		0x281
#define RPDO2_LEN		8


union control_word_t

{
	uint16_t all;
	struct
	{
		uint8_t Switch_on:1;//.........0
		uint8_t Enable_voltage:1;//....1
		uint8_t Quick_stop:1;//........2
		uint8_t Enable_operation:1;//..3
		uint8_t Set_Point:1;//...........4
		uint8_t Immed_Change:1;//........5
		uint8_t Related_Abs:1;//.........6
		uint8_t Fault_reset:1;//.........7
		uint8_t Halt:1;//..............8
		uint8_t Reserved0:1;//.........9
		uint8_t Reserved1:1;//.........10
		uint8_t Manufacture0:1;//......11......no 2nd command buffer if one is running
		uint8_t Manufacture1:1;//........12....updata command immediatily in 1 mode
		uint8_t Manufacture2:1;//........13....no buffer if software limit for turkey customer
		uint8_t Manufacture3:1;//........14
		uint8_t Manufacture4:1;//........15
	}bit;
	struct
	{
		uint8_t byte_0;
		uint8_t byte_1;
	}byte;	
};
union status_word_t
{
	uint16_t all;
	struct
	{
		uint8_t Ready_on:1;//.............0
		uint8_t Switched_on:1;//..........1
		uint8_t Operation_enable:1;//.....2
		uint8_t Fault:1;//................3
		uint8_t Voltage_enable:1;//.........4
		uint8_t Quick_stop:1;//.............5
		uint8_t Switchon_disabled:1;//......6
		uint8_t Warning:1;//................7
		uint8_t Maunufacture0:1;//........8
		uint8_t Remote:1;//...............9
		uint8_t Target_reached:1;//.......10
		uint8_t Intlim_active:1;//........11
		uint8_t Setpoint_Ack:1;//...........12
		uint8_t Fllowing_Error:1;//.........13
		uint8_t Commutation_Found:1;//......14
		uint8_t Reference_Found:1;//........15
	}bit;
	struct
	{
		uint8_t byte_0;
		uint8_t byte_1;
	}byte;	
};

union error_t
{
	uint16_t all;
	struct
	{
		uint8_t Internal:1;//.............0
		uint8_t ABZ:1;//..................1
		uint8_t UVW:1;//..................2
		uint8_t ENC_Counting:1;//.........3
		uint8_t Temp:1;//...................4
		uint8_t OV:1;//.....................5
		uint8_t UV:1;//.....................6
		uint8_t Over_Current:1;//...........7
		uint8_t Chop_Resistor:1;//........8
		uint8_t Following:1;//............9
		uint8_t Logic_Voltage:1;//........10
		uint8_t IIt:1;//..................11
		uint8_t Over_Frequency:1;//.........12
		uint8_t Motor_Temp:1;//.............13
		uint8_t Commutation:1;//............14
		uint8_t EEPROM:1;//.................15
	}bit;
	struct
	{
		uint8_t byte_0;
		uint8_t byte_1;
	}byte;	
};
union error2_t
{
	uint16_t all;
	struct
	{
		uint8_t Current_Sensor:1;//........0
		uint8_t Watch_Dog:1;//.............1
		uint8_t Wrong_Interrupt:1;//.......2
		uint8_t MCU:1;//...................3
		uint8_t Motor:1;//...............4
		uint8_t Output:1;//..............5
		uint8_t STO1:1;//................6
		uint8_t STO2:1;//................7
		uint8_t Extenal_Enable:1;//........8
		uint8_t Pos_Limit:1;//.............9
		uint8_t Neg_Limit:1;//.............10
		uint8_t SPI:1;//...................11
		uint8_t BUS:1;//................12
		uint8_t Closed_Loop:1;//........13
		uint8_t Master_ABZ:1;//.........14
		uint8_t Master_Counting:1;//....15
	}bit;
	struct
	{
		uint8_t byte_0;
		uint8_t byte_1;
	}byte;	
};

typedef struct {
	int8_t operation_mode;
	union control_word_t control_word;
	union can_int32_t target_speed;
	union can_int32_t target_position;
} motor_tx_t;

typedef struct {
	int8_t operation_mode;
	int8_t operation_mode_buff;
	union status_word_t status_word;
	union error_t error_state;
	union error2_t error_state2;
	union can_int32_t speed_real;
	union can_int32_t pos_actual;
	union can_int16_t temp_device;
	union can_int16_t i_q_b;
	union control_word_t control_word;
	union can_int32_t target_speed;
	union can_int32_t target_position;
	union can_int32_t profile_speed;
	union can_int32_t profile_acce;
	union can_int32_t profile_dece;
	union can_int16_t driver_iit_real;
	union can_int16_t motor_iit_real;

} motor_rx_t;






