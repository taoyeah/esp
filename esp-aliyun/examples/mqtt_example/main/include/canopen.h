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
#include <stdio.h>
#include <stdlib.h>

#define ID_MASTER_SDO_CMD      0x601
#define ID_SLAVE_SDO_RESP      0x581

#define SDO_READ_CMD           0x40
#define SDO_WRITE_CMD          0x20
#define SDO_WRITE_BYTE_CMD     0x2F
#define SDO_WRITE_2BYTE_CMD    0x2B
#define SDO_WRITE_4BYTE_CMD    0x23

#define TPDO_CMD           	0x10
#define RPDO_CMD          	0x30

union can_int32_t
{
	int32_t all;
	struct
	{
		int8_t byte_0;
		int8_t byte_1;
		int8_t byte_2;
		int8_t byte_3;
	}byte;
	struct
	{
		int16_t word_0;
		int16_t word_1;
	}word;		
};
union can_int16_t
{
	int16_t all;
	
	struct
	{
		int8_t byte_0;
		int8_t byte_1;
	}byte;		
};

typedef struct {
	uint8_t cmd;
	uint16_t identifier;
    union can_int32_t od;
    union can_int32_t data;
	struct 
	{
		int8_t len;
		int8_t data_0;
		int8_t data_1;
		int8_t data_2;
		int8_t data_3;
		int8_t data_4;
		int8_t data_5;
		int8_t data_6;
		int8_t data_7;
	}pdo;
} rtx_task_action_t;












