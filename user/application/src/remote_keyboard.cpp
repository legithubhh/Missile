/**
 *******************************************************************************
 * @file      : remote_keyboard.cpp
 * @brief     :此文档实现通过读取遥控器与键鼠状态，实现模式切换。并给予各电机目标值。
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      2024-01-30      JasonLi        1. <note>
 *******************************************************************************
 * @attention :
 * s1:1       s2:1      测试模式：打开摩擦轮，默认目标为前哨站。推杆连续前移。
 * s1:3       s2:1      测试模式：打开摩擦轮，默认目标为前哨站。推杆连续后退。
 * s1:2       s2:1      测试模式：打开摩擦轮，默认目标为前哨站。推杆停止。
 * s1:1       s2:2      裁判系统模式：打开摩擦轮，默认目标为前哨站。推杆间断以一定距离前移两次。
 * s1:3       s2:2      裁判系统模式：打开摩擦轮，默认目标为前哨站。推杆停止。
 * s1:2       s2:2      裁判系统模式：关闭摩擦轮，默认目标为前哨站。推杆停止。
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "remote_keyboard.h"

#include "bsp_dwt.h"
#include "gimbal.h"
#include "referee.h"
#include "remote.h"
#include "shoot.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
uint16_t fashe_flag = 0;
uint16_t fri_flag = 0;
float time_this;
float time_real;
/* Private function prototypes -----------------------------------------------*/
void DartStateControl();
void RemoteShootTarget0Ctrl();
void RemoteShootTarget1Ctrl();
void StopFricCtrl();

void ModeTask()
{
    DartStateControl();

    if (remote.GetS2() == 1)  // 测试模式，打开摩擦轮
    {
        RemoteShootTarget0Ctrl();
    }

    if (remote.GetS2() == 2)  // 关闭摩擦轮
    {
        StopFricCtrl();

    } else if (remote.GetS2() == 3 && fri_flag == 1)  // 裁判系统，打开摩擦轮
    {
        if (Dart_State.dart_attack_target == 0) {
            RemoteShootTarget0Ctrl();
        } else if (Dart_State.dart_attack_target == 1) {
            RemoteShootTarget1Ctrl();
        }

    } else if (remote.GetS2() == 3 && fri_flag == 0)  // 裁判系统，关闭摩擦轮
    {
        StopFricCtrl();
    }
}

void RemoteShootTarget0Ctrl()  // 目标为前哨站
{
    shoot.SetFricLevel2Speed(20.f * 60.f);  //(n*60)即n转每秒
    shoot.SetFricLevel1Speed(10.f * 60.f);
}

void RemoteShootTarget1Ctrl()  // 目标为基地
{
    shoot.SetFricLevel2Speed(30.f * 60.f);
    shoot.SetFricLevel1Speed(20.f * 60.f);
}

void StopFricCtrl()
{
    shoot.SetFricLevel1Speed(0.f);
    shoot.SetFricLevel2Speed(0.f);
}

/**
 * @brief      比赛模式控制飞镖发射
 *   @arg       None
 * @retval      None
 * @note
 * 当前飞镖发射口的状态：
 *2：关闭；
 *3：正在开启或者关闭中；
 *1：已经开启。
 * @note
 *飞镖的打击目标，默认为前哨站：
 *0：前哨站；
 *1：基地。
 */
void DartStateControl()
{
    if (remote.GetS2() == 3) {
        switch (remote.GetS1()) {
            case 1: {
                Dart_State.dart_launch_opening_status = 1;
                break;
            }
            case 3: {
                Dart_State.dart_launch_opening_status = 3;
                break;
            }
            case 2: {
                Dart_State.dart_launch_opening_status = 2;
                break;
            }
        }
        if (Dart_State.dart_attack_target == 0 && Dart_State.last_dart_attack_target == 1)  // 基地切换前哨站
        {
            YawDirSet(0);  // 发射架Y轴的步进电机
            StartYawPulse(1, 500);
        } else if (Dart_State.dart_attack_target == 1 && Dart_State.last_dart_attack_target == 0)  // 前哨站切换基地
        {
            YawDirSet(1);  // 发射架Y轴的步进电机
            StartYawPulse(1, 500);
        }
        Dart_State.last_dart_attack_target = Dart_State.dart_attack_target;
    }

    // s2 是状态
    if (remote.GetS2() == 3)  // 裁判系统
    {
        if (Dart_State.dart_launch_opening_status == 2)  // 关闭
        {
            fashe_flag = 0;
            fri_flag = 0;
        } else if (Dart_State.dart_launch_opening_status == 3)  // 正在开启或者关闭中
        {
            fashe_flag = 0;
            fri_flag = 1;
        } else if (Dart_State.dart_launch_opening_status == 1 && fashe_flag == 0)  // 已经开启
        {
            fashe_flag = 1;
            fri_flag = 1;
            time_this = DWT_GetTimeline_ms();
        }
    }
}