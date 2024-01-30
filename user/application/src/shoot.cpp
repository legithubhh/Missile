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
void Shoot::PidInit()
{
    fric_speed_[0].Init(40.0f, 0.0f, 0.0f, 15000.0f, 0.0f);
    fric_speed_[1].Init(40.0f, 0.0f, 0.0f, 15000.0f, 0.0f);
    fric_speed_[2].Init(40.0f, 0.0f, 0.0f, 15000.0f, 0.0f);
    fric_speed_[3].Init(40.0f, 0.0f, 0.0f, 15000.0f, 0.0f);
    fric_speed_[0].Inprovement(PID_IMPROVE_NONE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fric_speed_[1].Inprovement(PID_IMPROVE_NONE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fric_speed_[2].Inprovement(PID_IMPROVE_NONE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    fric_speed_[3].Inprovement(PID_IMPROVE_NONE, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

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

void Shoot::SetFricLevel1Speed(float _speed)
{
    fric_speed_[1].SetRef(-_speed);
    fric_speed_[3].SetRef(_speed);
}

void Shoot::SetFricLevel2Speed(float _speed)
{
    fric_speed_[0].SetRef(-_speed);
    fric_speed_[2].SetRef(_speed);
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
