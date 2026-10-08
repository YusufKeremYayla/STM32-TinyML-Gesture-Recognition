/*
 * lis3dsh.h
 *
 *  Created on: 24 Eyl 2026
 *      Author: yusuf
 */

#ifndef INC_LIS3DSH_H_
#define INC_LIS3DSH_H_

#include "main.h"

#include <stdbool.h>
#include "math.h"

/*
 * REGISTERS
 */

#define LIS3DSH_WHO_AM_I_ADDR 	0x0F
#define LIS3DSH_STAT_REG		0x18

#define LIS3DSH_CONTROL_REG_4	0x20
#define LIS3DSH_CONTROL_REG_1	0x21
#define LIS3DSH_CONTROL_REG_2	0x22
#define LIS3DSH_CONTROL_REG_3	0x23
#define LIS3DSH_CONTROL_REG_5	0x24
#define LIS3DSH_CONTROL_REG_6	0x25

#define LIS3DSH_STATUS_REG		0x27

#define LIS3DSH_OUT_X_L			0x28
#define LIS3DSH_OUT_X_H			0x29

#define LIS3DSH_OUT_Y_L			0x2A
#define LIS3DSH_OUT_Y_H			0x2B

#define LIS3DSH_OUT_Z_L			0x2C
#define LIS3DSH_OUT_Z_H			0x2D

#define LIS3DSH_FIFO_CTRL		0x2E
#define LIS3DSH_FIFO_SRC		0x2F

#define RAD_TO_DEG (180/M_PI)

/*
 *VALUES
*/

#define LIS3DSH_WHO_AM_I_VALUE 	0x3F

typedef struct
{
    SPI_HandleTypeDef   	*hspi;
    GPIO_TypeDef         	*cs_port;
    uint16_t              	cs_pin;

    int16_t               	x_raw;
    int16_t              	y_raw;
    int16_t              	z_raw;

    float 					roll;
    float 					pitch;

    float 					roll_filtered;
    float 					pitch_filtered;

    bool                    found;

} LIS3DSH_t;

bool 	LIS3DSH_Initialization 		(LIS3DSH_t *lis3dsh, SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin);
uint8_t LIS3DSH_Who_Am_I	   		(LIS3DSH_t *lis3dsh);

bool 	LIS3DSH_Read_XYZ	   		(LIS3DSH_t *lis3dsh);

uint8_t LIS3DSH_Read_Register  		(LIS3DSH_t *lis3dsh, uint8_t regAddr);
bool 	LIS3DSH_Write_Register 		(LIS3DSH_t *lis3dsh, uint8_t regAddr, uint8_t data);
void	LIS3DSH_Calculate_Angles	(LIS3DSH_t *lis3dsh);

#endif /* INC_LIS3DSH_H_ */
