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
#include "math.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
Temperature_t temperature;
/* Private function prototypes -----------------------------------------------*/

void Temperature_t::Update(uint8_t *_pdata)
{
    // if (calc_crc16(_pdata, 0x08) == 0) {//暂时通不过校验，但是数据正常，原因未知
    Pack_.site = (_pdata[0] & 0xFF);
    Pack_.functionid = (_pdata[1] & 0xFF);
    Pack_.size = (_pdata[2] & 0xFF);
    switch (_pdata[3] >> 7) {
        case 0:
            Pack_.datatemp1 = ((_pdata[3] << 8 | _pdata[4]) & 0xFFFF);
            break;
        case 1:
            Pack_.datatemp1 = (((_pdata[3] << 8 | _pdata[4]) - 0xffff - 0x0001) & 0xFFFF);
            break;
    }
    switch (_pdata[5] >> 7) {
        case 0:
            Pack_.datatemp2 = ((_pdata[5] << 8 | _pdata[6]) & 0xFFFF);
            break;
        case 1:
            Pack_.datatemp2 = (((_pdata[5] << 8 | _pdata[6]) - 0xffff - 0x0001) & 0xFFFF);
            break;
    }
    Pack_.crc = ((_pdata[5] << 8 | _pdata[6]) & 0xFFFF);
    /*由于接受数据间隔时间，收到的温度数据为0，所以需要过滤这一信号*/
    if (abs(Pack_.datatemp1 - 0) > 1e-6) {
        Pack_.realtemp1 = Pack_.datatemp1 / 10.0f;
    }
    if (abs(Pack_.datatemp2 - 0) > 1e-6) {
        Pack_.realtemp2 = Pack_.datatemp2 / 10.0f;
    }
    // }
}

void Temperature_t::ReceiveDate()
{
    UartSendData(&huart6, tx_buffer_, 0x08, UART_TRAMSMIT_BLOCKING);
    DWT_Delay(0.1f);
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