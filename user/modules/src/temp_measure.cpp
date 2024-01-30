/**
 *******************************************************************************
 * @file      : temp_measure.cpp
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      2024-01-30      JasonLi        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, University of Science and Technology Beijing.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "temp_measure.h"

#include "bsp_dwt.h"
#include "crc.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
Temperature_t temperature;
uint8_t ab = 0;
uint8_t ac = 0;
/* Private function prototypes -----------------------------------------------*/

void Temperature_t::Update(uint8_t *_pdata)
{
    ac = 1;
    if (calc_crc16(_pdata, 0x07) == 0) {
        Pack_.site = (_pdata[0] & 0xFF);
        Pack_.functionid = (_pdata[1] & 0xFF);
        Pack_.size = (_pdata[2] & 0xFF);
        switch (_pdata[3] >> 7) {
            case 0:
                Pack_.temp1 = ((_pdata[3] << 8 | _pdata[4]) & 0xFFFF);
                break;
            case 1:
                Pack_.temp1 = (((_pdata[3] << 8 | _pdata[4]) - 0xffff - 0x0001) & 0xFFFF);
                break;
        }
        // Pack_.temp2 =
        Pack_.crc = ((_pdata[5] << 8 | _pdata[6]) & 0xFFFF);
        Pack_.temp1 /= 10.f;
        // Pack_.temp2 /= 10.f;
    }
}

void Temperature_t::GetDate()
{
    ab = 1;
    UartSendData(&huart1, tx_buffer_, 0x08, UART_TRAMSMIT_DMA);
    DWT_Delay(5e-3);
}

static void TemperatureMeasureCallBack()
{
    temperature.Update(temperature.premote_instance->rx_buffer);
}

void TemperatureMeasureInit(UART_HandleTypeDef *_phuart)
{
    UartInitConfig conf;
    conf.huart = _phuart;
    conf.rx_buffer_size = 7;
    conf.callback_function = TemperatureMeasureCallBack;
    temperature.premote_instance = pUartRegister(&conf);
    return;
}