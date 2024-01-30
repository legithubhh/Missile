/**
 *******************************************************************************
 * @file      : infantry.cpp
 * @brief     :此文档实现控制电机的输出
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      2024-01-30      JasonLi        1. <note>
 *******************************************************************************
 * @attention :
 * ch0：控制Yaw轴
 * ch1：控制Pitch轴
 * ch2：控制Yaw轴，微调
 * ch3：控制Pitch轴，微调
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "infantry.h"

#include "bsp_dwt.h"
#include "gimbal.h"
#include "referee.h"
#include "remote.h"
#include "remote_keyboard.h"
#include "shoot.h"
#include "temp_measure.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void DartStateControl();
void ServoControl();
void RotationControl();
/**
 * @brief Initializes the system.
 */
void InfantrySystemInit()
{
    RemoteControlInit(&huart3);
    TemperatureMeasureInit(&huart1);
    shoot.MotorInit();
}

/**
 * @brief Initializes the gimbal.
 */
void GimbalInit()
{
    shoot.PidInit();
}

/**
 * @brief This function handles the Gimbal task.
 */
void GimbalTask()
{
    DartStateControl();

    shoot.Control();
    DjiMotorSend(&hcan1, 0x200, (int16_t)shoot.fric_output_[0], (int16_t)shoot.fric_output_[1],
                 (int16_t)shoot.fric_output_[2], (int16_t)shoot.fric_output_[3]);

    ServoControl();

    RotationControl();
}

void ServoControl()  // 丝杆步进电机
{
    time_real = DWT_GetTimeline_ms();
    if (remote.GetS2() != 3) {
        if (remote.GetS1() == 1) {
            PushDirSet(0);  // 前进
            DWT_Delay(1e-4);
            SetPushPWM(250);
        } else if (remote.GetS1() == 3) {
            PushDirSet(1);  // 回退
            DWT_Delay(1e-4);
            SetPushPWM(250);
        } else {
            PushDirSet(1);  // 停止
            SetPushPWM(0);
        }
    } else if (remote.GetS2() == 3 && fashe_flag == 1) {
        PushDirSet(0);  // 前进
        DWT_Delay(1e-4);
        SetPushPWM(400);
        // 加个计时函数
        if (time_real - time_this > 5200 && time_real - time_this < 7200) {
            SetPushPWM(0);
        }
        if (time_real - time_this > 7200 && time_real - time_this < 14900) {
            PushDirSet(0);
            DWT_Delay(1e-4);
            SetPushPWM(400);
        }
        if (time_real - time_this > 14900) {
            SetPushPWM(0);
        }
    } else {
        SetPushPWM(0);
    }
}

void RotationControl()
{
    if ((remote.GetCh1() != 0x00) || (remote.GetCh0() != 0x00)) {
        if (remote.GetCh1() > 656)  //  660> >656
        {
            PitchDirSet(0);  // 发射架P轴的步进电机
            DWT_Delay(1e-4);
            StartPitchPulse(1, 400);
        } else if (remote.GetCh1() < -656)  // -660< <-656
        {
            PitchDirSet(1);
            DWT_Delay(1e-4);
            StartPitchPulse(1, 400);
        }
        if (remote.GetCh0() > 656) {
            YawDirSet(0);  // 发射架Y轴的步进电机
            DWT_Delay(1e-4);
            StartYawPulse(1, 400);
        } else if (remote.GetCh0() < -656) {
            YawDirSet(1);
            DWT_Delay(1e-4);
            StartYawPulse(1, 400);
        }
    }
    // 微调
    if ((remote.GetCh3() != 0x00) || (remote.GetCh2() != 0x00)) {
        if (remote.GetCh3() > 656) {
            PitchDirSet(0);  // 发射架P轴的步进电机
            DWT_Delay(1e-4);
            StartPitchPulse(1, 100);
        } else if (remote.GetCh3() < -656) {
            PitchDirSet(1);
            DWT_Delay(1e-4);
            StartPitchPulse(1, 100);
        }
        if (remote.GetCh2() > 656) {
            YawDirSet(0);  // 发射架Y轴的步进电机
            DWT_Delay(1e-4);
            StartYawPulse(1, 100);
        } else if (remote.GetCh2() < -656) {
            YawDirSet(1);
            DWT_Delay(1e-4);
            StartYawPulse(1, 100);
        }
    }
}
