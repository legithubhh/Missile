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
#include "missile.h"

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
 * @brief 串口，CAN对应分配，使用中断回调函数无需占用线程
 */
void InfantrySystemInit()
{
    RemoteControlInit(&huart3);
    TemperatureMeasureInit(&huart1);
    referee.Init(&huart6);
    shoot.MotorInit();
}

/**
 * @brief 电机PID初始化参数
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
    temperature.ReceiveDate();

    DartStateControl();

    shoot.Control();
    DjiMotorSend(&hcan1, 0x200, (int16_t)shoot.fric_output_[0], (int16_t)shoot.fric_output_[1],
                 (int16_t)shoot.fric_output_[2], (int16_t)shoot.fric_output_[3]);  //(int16_t)shoot.fric_output_[2]

    ServoControl();

    RotationControl();
}

void ServoControl()  // 丝杆步进电机
{
    // 当不为裁判系统模式时，使用遥控器S1控制推弹机构进行前进、后退、停止
    if (remote.GetS2() != 3) {
        if (remote.GetS1() == 1) {
            PushDirSet(0);  // 前进
            DWT_Delay(1e-3);
            SetPushPWM(150);
        } else if (remote.GetS1() == 3) {
            PushDirSet(1);  // 后退
            DWT_Delay(1e-3);
            SetPushPWM(150);
        } else {
            PushDirSet(1);  // 停止
            SetPushPWM(0);
        }
    } else if (remote.GetS2() == 3 && fashe_flag == 1) {
        PushDirSet(0);  // 前进
        DWT_Delay(1e-3);
        SetPushPWM(25);
        // 加个计时函数
        time_real = DWT_GetTimeline_ms();
        // 约3.5秒一发，暂停约5s，让摩擦轮恢复状态，留出装甲板判断击打的空窗期，尽量吃满增益。
        if (time_real - time_this > 3200 && time_real - time_this < 7500) {
            SetPushPWM(0);
        }
        if (time_real - time_this > 7500 && time_real - time_this < 10700) {
            PushDirSet(0);  // 前进
            DWT_Delay(1e-3);
            SetPushPWM(150);
        }
        if (time_real - time_this > 10700) {
            SetPushPWM(0);
        }
    } else {
        SetPushPWM(0);
    }
}

void RotationControl()
{

        if (remote.GetCh1() > 656 && remote.GetCh3() > 656) {
            PitchDirSet(0);  // 发射架P轴的步进电机
            StartPitchPulse(1, 100);
        } else if (remote.GetCh1() < -656 && remote.GetCh3() < -656) {
            PitchDirSet(1);
            StartPitchPulse(1, 100);
        }
        if (remote.GetCh0() > 656 && remote.GetCh2() > 656) {
            YawDirSet(0);  // 发射架Y轴的步进电机
            DWT_Delay(1e-3);
            StartYawPulse(1, 100);
        } else if (remote.GetCh0() < -656 && remote.GetCh2() < -656) {
            YawDirSet(1);
            DWT_Delay(1e-3);
            StartYawPulse(1, 100);
        }
}
