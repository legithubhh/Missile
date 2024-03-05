/**
 *******************************************************************************
 * @file      : temp_control.cpp
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      2024-02-23      JasonLi        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, University of Science and Technology Beijing.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "temp_control.h"

#include "bsp_dwt.h"
#include "temp_measure.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief This function checks the temperature and controls the IO port based on the temperature conditions.
 *
 * It checks if the temperature is greater than 30 degrees Celsius. If the temperature is below 30 degrees Celsius for 1 second continuously,
 * it sets the IO port PI7 high. If the temperature is above 30 degrees Celsius, it sets the IO port PI7 low.
 *
 * @note This function relies on the temperature readings from the `temperature` object.（此段注释由Copilot自动生成，请根据实际情况修改。）
 */
void TempTask(void)
{
    // Code to check if the temperature is greater than 30
    if (temperature.GetTemp1() < 30.f || temperature.GetTemp2() < 30.f) {
        // Code to check if the state has been continuous for 1 second
        DWT_Delay(1.f);
        if (temperature.GetTemp1() < 30.f || temperature.GetTemp2() < 30.f) {
            // If true, set the IO port PI7 high
            HAL_GPIO_WritePin(TEMP_CONTROL_GPIO_Port, TEMP_CONTROL_Pin, GPIO_PIN_SET);
        }
    } else {
        DWT_Delay(1.f);
        // If false, set the IO port PI7 low
        if (temperature.GetTemp1() > 30.f || temperature.GetTemp2() > 30.f) {
            HAL_GPIO_WritePin(TEMP_CONTROL_GPIO_Port, TEMP_CONTROL_Pin, GPIO_PIN_RESET);
        }
    }
}
