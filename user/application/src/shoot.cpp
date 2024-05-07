/**
 *******************************************************************************
 * @file      : shoot.cpp
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
#include "shoot.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
Shoot shoot;
/* Private function prototypes -----------------------------------------------*/
static void Left1FricMotorCallback();
static void Left2FricMotorCallback();
static void Right1FricMotorCallback();
static void Right2FricMotorCallback();

/**
 * @brief       设置摩擦轮电机级各级PID的基本参数
 *   @arg       None
 * @retval      None
 * @note        None
 */
void Shoot::PidInit()
{
    fric_speed_[0].Init(2.f, 1.f, 0.02f, 5000.0f, 0.0f);
    fric_speed_[1].Init(1.5f, 1.f, 0.01f, 5000.0f, 0.0f);
    fric_speed_[2].Init(2.f, 1.f, 0.02f, 5000.0f, 0.0f);
    fric_speed_[3].Init(1.5f, 1.f, 0.01f, 5000.0f, 0.0f);  // 目标速度越大，静态速度波动越小，推测比例环影响较大，如果减小比例对飞镖射出瞬间速度无影响，可考虑根据击打目标转换PID
    fric_speed_[0].Inprovement(PID_TRAPEZOID_INTEGRAL | PID_INTEGRAL_LIMIT, 60.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fric_speed_[1].Inprovement(PID_TRAPEZOID_INTEGRAL | PID_INTEGRAL_LIMIT, 60.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fric_speed_[2].Inprovement(PID_TRAPEZOID_INTEGRAL | PID_INTEGRAL_LIMIT, 60.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fric_speed_[3].Inprovement(PID_TRAPEZOID_INTEGRAL | PID_INTEGRAL_LIMIT, 60.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

/**
 * @brief       分配电机的ID，以及CAN通道和更新函数
 *   @arg       None
 * @retval      None
 * @note        None
 */
void Shoot::MotorInit()
{
    fric_motor_[0].Init(0x201, &hcan1, ABSOLUTE_FLAG);
    fric_motor_[1].Init(0x202, &hcan1, ABSOLUTE_FLAG);
    fric_motor_[2].Init(0x203, &hcan1, ABSOLUTE_FLAG);
    fric_motor_[3].Init(0x204, &hcan1, ABSOLUTE_FLAG);
    fric_motor_[0].pdji_motor_instance->pCanCallBack = Left1FricMotorCallback;
    fric_motor_[1].pdji_motor_instance->pCanCallBack = Left2FricMotorCallback;
    fric_motor_[2].pdji_motor_instance->pCanCallBack = Right1FricMotorCallback;
    fric_motor_[3].pdji_motor_instance->pCanCallBack = Right2FricMotorCallback;
}

/**
 * @brief       计算输出（目标值设定位于remote_keyboard.cpp文件，集中在ModeTask()函数）
 *   @arg       None
 * @retval      None
 * @note        None
 */
void Shoot::Control()
{
    FricCalc();
}

void Shoot::FricCalc()
{
    for (int i = 0; i < 4; i++) {
        fric_speed_[i].SetMeasure(fric_motor_[i].GetSpd());
        fric_output_[i] = fric_speed_[i].Calculate();
    }
}

void Shoot::SetFricLevel2Speed(float _speed)
{
    fric_speed_[0].SetRef(_speed);
    fric_speed_[2].SetRef(-_speed);
}

void Shoot::SetFricLevel1Speed(float _speed)
{
    fric_speed_[1].SetRef(_speed);
    fric_speed_[3].SetRef(-_speed);
}

static void Left1FricMotorCallback()
{
    shoot.fric_motor_[0].Update();
}

static void Left2FricMotorCallback()
{
    shoot.fric_motor_[1].Update();
}

static void Right1FricMotorCallback()
{
    shoot.fric_motor_[2].Update();
}

static void Right2FricMotorCallback()
{
    shoot.fric_motor_[3].Update();
}
