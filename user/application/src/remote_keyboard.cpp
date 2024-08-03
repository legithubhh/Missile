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
 * s1:1       s2:3      裁判系统模式：打开摩擦轮，默认目标为前哨站。推杆间断以一定距离前移两次。一定切换目标为基地固定目标！！！
 * s1:3       s2:3      裁判系统模式：打开摩擦轮蓄能，默认目标为前哨站。推杆停止。
 * s1:2       s2:3      裁判系统模式：关闭摩擦轮，默认目标为前哨站。推杆停止。
 * s1:1       s2:2      测试模式：关闭摩擦轮，默认目标为前哨站。推杆连续前移。
 * s1:3       s2:2      测试模式：关闭摩擦轮，默认目标为前哨站。推杆连续后退。
 * s1:2       s2:2      测试模式：关闭摩擦轮，默认目标为前哨站。推杆停止。
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Includes ------------------------------------------------------------------*/
#include "remote_keyboard.h"

#include "bsp_dwt.h"
#include "gimbal.h"
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
uint8_t attack_target;
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
        RemoteShootTarget1Ctrl();
    }

    if (remote.GetS2() == 2)  // 关闭摩擦轮
    {
        StopFricCtrl();

    } else if (remote.GetS2() == 3 && fri_flag == 1)  // 裁判系统，打开摩擦轮
    {
        attack_target == 0;  // 测试用
        if (attack_target == 0) {
            RemoteShootTarget1Ctrl();
        } else if (attack_target == 1) {
            RemoteShootTarget0Ctrl();
        }

    } else if (remote.GetS2() == 3 && fri_flag == 0)  // 裁判系统，关闭摩擦轮
    {
        StopFricCtrl();
    }
}

void RemoteShootTarget0Ctrl()  // 目标为前哨站13.2m/s
{
    shoot.SetFricLevel2Speed(100.f * 60.f);  //(n*60)即n转每秒
    shoot.SetFricLevel1Speed(70.f * 60.f);
    // shoot.SetFricLevel2Speed(75.f * 60.f);  //(n*60)即n转每秒
    // shoot.SetFricLevel1Speed(42.f * 60.f);
}

void RemoteShootTarget1Ctrl()  // 目标为基地固定目标
{
    shoot.SetFricLevel2Speed(105.f * 60.f);
    shoot.SetFricLevel1Speed(84.f * 60.f);
}

void StopFricCtrl()
{
    shoot.SetFricLevel1Speed(0.f);
    shoot.SetFricLevel2Speed(0.f);
}

/**
 * @brief      遥控器模拟裁判系统控制飞镖发射
 *   @arg       None
 * @retval      None
 * @note
 * 当前飞镖发射口的状态：referee.dart_client_cmd_.dart_launch_opening_status
 *1：关闭；
 *2：正在开启或者关闭中；
 *0：已经开启。
 * @note
 *飞镖的打击目标，默认为前哨站：referee.dart_info_info_.dart_info_attack_target
 *0：前哨站；
 *1：基地固定目标。
 *2：基地随机目标。
 */
void DartStateControl()
{
    if (remote.GetS2() == 3) {
        switch (remote.GetS1()) {
            /*以下代码只供模拟裁判系统发送发射口状态，正式比赛请注释*/
            case 1: {
                referee.dart_client_cmd_.dart_launch_opening_status = 0;
                break;
            }
            case 3: {
                referee.dart_client_cmd_.dart_launch_opening_status = 2;
                break;
            }
            case 2: {
                referee.dart_client_cmd_.dart_launch_opening_status = 1;
                break;
            }
        }
        /*如果只打基地，可以注销以下代码，无需切换，瞄准基地固定目标打就行*/
        if (referee.dart_info_info_.dart_info_attack_target != referee.dart_info_info_.dart_info_last_attack_target) {
            if (referee.dart_info_info_.dart_info_attack_target == 0) {  // 基地固定目标切换前哨站
                YawDirSet(0);                                            // 发射架Y轴的步进电机
                StartYawPulse(1, 500);
            }
            if (referee.dart_info_info_.dart_info_attack_target == 1) {  // 前哨站切换基地固定目标
                YawDirSet(1);                                            // 发射架Y轴的步进电机
                StartYawPulse(1, 500);
            }
            referee.dart_info_info_.dart_info_last_attack_target = referee.dart_info_info_.dart_info_attack_target;
        }
    }

    /*判断发射口状态，对摩擦轮以及推弹电机的控制变量进行使能*/
    if (remote.GetS2() == 3)  // 裁判系统
    {
        if (referee.dart_client_cmd_.dart_launch_opening_status == 1)  // 关闭
        {
            fashe_flag = 0;
            fri_flag = 0;
        } else if (referee.dart_client_cmd_.dart_launch_opening_status == 2)  // 正在开启或者关闭中
        {
            fashe_flag = 0;
            fri_flag = 1;
        } else if (referee.dart_client_cmd_.dart_launch_opening_status == 0 && fashe_flag == 0)  // 已经开启
        {
            fashe_flag = 1;
            fri_flag = 1;
            time_this = DWT_GetTimeline_ms();
        }
    }
}

// 初始化飞镖状态为关闭；
void DoorInit()
{
    referee.dart_client_cmd_.dart_launch_opening_status = 1;
}