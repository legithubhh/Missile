/**
 *******************************************************************************
 * @file      : temp_measure.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TEMP_MEASURE_H_
#define __TEMP_MEASURE_H_

#ifdef __cplusplus
/* Includes ------------------------------------------------------------------*/
#include "bsp_uart.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
struct TempTransducerDatePack_t {
    int8_t site;
    int8_t functionid;
    int8_t size;
    int32_t temp1;
    // int32_t temp2;
    int16_t crc;
};

class Temperature_t
{
   public:
    UartInstance *premote_instance;
    void Update(uint8_t *_pdata);
    void GetDate();

    int
    GetTemp1()
    {
        return Pack_.temp1;
    }

    // int GetTemp2()
    // {
    //     return Pack_.temp2;
    // }

   private:
    uint8_t tx_buffer_[8] = {
        0x01, 0x03, 0x00, 0x00, 0x00, 0x01, 0x84, 0x0A};
    TempTransducerDatePack_t Pack_;
};
// 0x0A,0x84,0x01,0x00,0x00,0x00,0x03,0x01
// 0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0A

/* Exported variables --------------------------------------------------------*/
extern Temperature_t temperature;
/* Exported function prototypes ----------------------------------------------*/
void TemperatureMeasureInit(UART_HandleTypeDef *_phuart);
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif /*__TEMP_MEASURE_H_ */
