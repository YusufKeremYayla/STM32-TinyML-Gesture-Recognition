/*
 * lis3dsh.c
 *
 *  Created on: 24 Eyl 2026
 *      Author: yusuf
 */

#include "lis3dsh.h"

/*
 * 1. CS (Chip Select) Kontrol Fonksiyonları (İçleri dolu şekilde)
 */
static void LIS3DSH_CS_Enable(LIS3DSH_t *lis3dsh)
{
    HAL_GPIO_WritePin(lis3dsh->cs_port, lis3dsh->cs_pin, GPIO_PIN_RESET);
}

static void LIS3DSH_CS_Disable(LIS3DSH_t *lis3dsh)
{
    HAL_GPIO_WritePin(lis3dsh->cs_port, lis3dsh->cs_pin, GPIO_PIN_SET);
}

/*
 * 6. Başlangıç (Initialization) Ayarları
 */

bool LIS3DSH_Initialization(LIS3DSH_t *lis3dsh, SPI_HandleTypeDef *hspi, GPIO_TypeDef *csPort, uint16_t csPin)
{
    lis3dsh->hspi    = hspi;
    lis3dsh->cs_port = csPort;
    lis3dsh->cs_pin  = csPin;

    lis3dsh->x_raw   = 0;
    lis3dsh->y_raw   = 0;
    lis3dsh->z_raw   = 0;

    if(LIS3DSH_Who_Am_I(lis3dsh) == LIS3DSH_WHO_AM_I_VALUE)
    {
        lis3dsh->found = true;

        /*
         * CTRL_REG4 : X Y Z aktif ve ODR 100 Hz
         */
        if(!LIS3DSH_Write_Register(lis3dsh, LIS3DSH_CONTROL_REG_4, 0x57))
        	return false;
        /*
         * CTRL_REG5 : +- 2g full scale ,anti alias filter 800Hz
         */
        if(!LIS3DSH_Write_Register(lis3dsh, LIS3DSH_CONTROL_REG_5, 0x00))
               	return false;

        /*
         * CTRL_REG3 : Veri Hazır (DRDY) kesmesini INT1 pininden aktif et (Active HIGH)
         * Binary: 1100 1000 -> Hex: 0xC8
        */
        if(!LIS3DSH_Write_Register(lis3dsh, LIS3DSH_CONTROL_REG_3, 0xC8))
                return false;
    }
    else
    {
        lis3dsh->found = false;
    }

    return true;
}

uint8_t LIS3DSH_Who_Am_I	   (LIS3DSH_t *lis3dsh)
{
	return LIS3DSH_Read_Register(lis3dsh, LIS3DSH_WHO_AM_I_ADDR);
}

bool LIS3DSH_Read_XYZ(LIS3DSH_t *lis3dsh)
{
		uint8_t xl, xh, yl, yh, zl, zh;

	    xl = LIS3DSH_Read_Register(lis3dsh, LIS3DSH_OUT_X_L);
	    xh = LIS3DSH_Read_Register(lis3dsh, LIS3DSH_OUT_X_H);
	    lis3dsh->x_raw = (int16_t)((xh << 8) | xl);

	    yl = LIS3DSH_Read_Register(lis3dsh, LIS3DSH_OUT_Y_L);
	    yh = LIS3DSH_Read_Register(lis3dsh, LIS3DSH_OUT_Y_H);
	    lis3dsh->y_raw = (int16_t)((yh << 8) | yl);

	    zl = LIS3DSH_Read_Register(lis3dsh, LIS3DSH_OUT_Z_L);
	    zh = LIS3DSH_Read_Register(lis3dsh, LIS3DSH_OUT_Z_H);
	    lis3dsh->z_raw = (int16_t)((zh << 8) | zl);

	    return true;
	}

uint8_t LIS3DSH_Read_Register(LIS3DSH_t *lis3dsh, uint8_t regAddr)
{
    // 1. Okuma Maskesi: Adresin en sol bitini (RW) 1 yapıyoruz
    uint8_t txData = regAddr | 0x80;
    uint8_t rxData = 0;

    LIS3DSH_CS_Enable(lis3dsh);

    if (HAL_SPI_Transmit(lis3dsh->hspi, &txData, 1, 100) == HAL_OK)
    {
        if (HAL_SPI_Receive(lis3dsh->hspi, &rxData, 1, 100) == HAL_OK)
        {
        	//diagnostic yada başka işlemler
        }
    }

    LIS3DSH_CS_Disable(lis3dsh);

    return rxData;
}

bool LIS3DSH_Write_Register(LIS3DSH_t *lis3dsh, uint8_t regAddr, uint8_t data)
{
    uint8_t txData[2] = {0};

    txData[0] = regAddr & 0x7F;

    txData[1] = data;

    LIS3DSH_CS_Enable(lis3dsh);

    HAL_StatusTypeDef status = HAL_SPI_Transmit(lis3dsh->hspi, txData, 2, 100);

    LIS3DSH_CS_Disable(lis3dsh);

    return (status == HAL_OK);
}

void	LIS3DSH_Calculate_Angles	(LIS3DSH_t *lis3dsh)
{
	lis3dsh->roll  = atan2f( lis3dsh->y_raw  , sqrtf(lis3dsh->x_raw * lis3dsh->x_raw + lis3dsh->z_raw * lis3dsh->z_raw)) * RAD_TO_DEG;
	lis3dsh->pitch = atan2f(-lis3dsh->x_raw  , sqrtf(lis3dsh->y_raw * lis3dsh->y_raw + lis3dsh->z_raw * lis3dsh->z_raw)) * RAD_TO_DEG;

	lis3dsh->roll_filtered  = (0.1f * lis3dsh->roll)  + (0.9f * lis3dsh->roll_filtered);
	lis3dsh->pitch_filtered = (0.1f * lis3dsh->pitch) + (0.9f * lis3dsh->pitch_filtered);
}
