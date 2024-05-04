/**
 *******************************************************************************
 * @file      : gimbal.cpp
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      yyyy-mm-dd      <author>        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "gimbal.h"

#include "bsp_dwt.h"
#include "tim.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/**
 * @brief      控制Pitch轴的脉冲输出，2ms一个脉冲，2ms>2.5us(最小脉宽)
 * @param       _step:脉冲输出个数
 *   @arg       None
 * @retval      None
 * @note        None
 */
void PitchOneStepSET(uint16_t _step)
{
    int cnt;
    for (cnt = _step; cnt != 0; cnt--) {
        HAL_GPIO_WritePin(PITCH_PUL_GPIO_Port, PITCH_PUL_Pin, GPIO_PIN_SET);  // PE9
        DWT_Delay(1e-3);
        HAL_GPIO_WritePin(PITCH_PUL_GPIO_Port, PITCH_PUL_Pin, GPIO_PIN_RESET);
        DWT_Delay(1e-3);
    }
}

/**
 * @brief      开始Pitch轴脉冲输出
 * @param       _r:步数
 * @param       _step:每步脉冲数
 *   @arg       None
 * @retval      None
 * @note        None
 */
void StartPitchPulse(uint8_t _r, uint16_t _step)
{
    int cnt = 0;
    for (cnt = _r; cnt != 0; cnt--) {
        PitchOneStepSET(_step);
    }
}

/**
 * @brief       控制Pitch轴运动方向
 * @param       _dir:标志电机运动方向
 *   @arg       None
 * @retval      None
 * @note        None
 */
void PitchDirSet(uint8_t _dir)
{
    if (_dir == 1) {
        HAL_GPIO_WritePin(PITCH_DIR_GPIO_Port, PITCH_DIR_Pin, GPIO_PIN_SET);  // PE11
    } else if (_dir == 0) {
        HAL_GPIO_WritePin(PITCH_DIR_GPIO_Port, PITCH_DIR_Pin, GPIO_PIN_RESET);
    } else {
    }
}

/**
 * @brief       控制飞镖推进方向
 * @param       _dir:标志电机运动方向
 *   @arg       None
 * @retval      None
 * @note        None
 */
void PushDirSet(uint8_t _dir)
{
    if (_dir == 1) {
        HAL_GPIO_WritePin(PUSH_DIR_GPIO_Port, PUSH_DIR_Pin, GPIO_PIN_SET);  // PI6
    } else if (_dir == 0) {
        HAL_GPIO_WritePin(PUSH_DIR_GPIO_Port, PUSH_DIR_Pin, GPIO_PIN_RESET);
    } else {
    }
}

/**
 * @brief      PWM输出控制推杆脉冲
 * @param       _xpwm:飞镖脉冲占空比_xpwm/50（不影响速度，保证高低电平脉宽>2.5us即可）
 *   @arg       None
 * @retval      None
 * @note        None
 */
void SetPushPWM(uint16_t _xpwm)
{
    __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, _xpwm);
}

/**
 * @brief      控制Yaw轴的脉冲输出与Pitch轴同理
 */
void YawOneStepSET(uint16_t _step)
{
    int cnt;
    for (cnt = _step; cnt != 0; cnt--) {
        HAL_GPIO_WritePin(YAW_PUL_GPIO_Port, YAW_PUL_Pin, GPIO_PIN_SET);  // PE14
        DWT_Delay(1e-3);
        HAL_GPIO_WritePin(YAW_PUL_GPIO_Port, YAW_PUL_Pin, GPIO_PIN_RESET);
        DWT_Delay(1e-3);
    }
}

/**
 * @brief      开启Yaw轴的脉冲输出与Pitch轴同理
 */
void StartYawPulse(uint8_t r, uint16_t _step)
{
    int cnt = 0;
    for (cnt = r; cnt != 0; cnt--) {
        YawOneStepSET(_step);
    }
}

/**
 * @brief      控制Yaw轴的运动方向与Pitch轴同理
 */
void YawDirSet(uint8_t dir)
{
    if (dir == 1) {
        HAL_GPIO_WritePin(YAW_DIR_GPIO_Port, YAW_DIR_Pin, GPIO_PIN_SET);  // PC6
    } else if (dir == 0) {
        HAL_GPIO_WritePin(YAW_DIR_GPIO_Port, YAW_DIR_Pin, GPIO_PIN_RESET);
    } else {
    }
}