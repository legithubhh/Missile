/**
 *******************************************************************************
 * @file      : shoot.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SHOOT_H_
#define __SHOOT_H_

#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/
#include "dji_motor.h"
#include "pid.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
typedef enum {
    ANGLE_FLAG = 0,
    SPEED_FLAG = 1,
} Shoot_Flag;

/* Exported types ------------------------------------------------------------*/

/**
 * @brief Class representing the shooting mechanism.
 */
class Shoot
{
   public:
    Pid fric_speed_[4];      /**< Array of PID controllers for friction motors' speed control. */
    DjiMotor fric_motor_[4]; /**< Array of friction motors. */
    uint16_t shoot_flag_;    /**< Flag indicating the shooting state. */
    float fric_output_[4];   /**< Array of friction motors' output values. */

    /**
     * @brief Control function for the shooting mechanism.
     */
    void Control();

    /**
     * @brief Calculate the output for the friction motors.
     */
    void FricCalc();

    /**
     * @brief 第一级摩擦轮加速（速度应小于第二级）
     * @param _speed The desired speed.
     */
    void SetFricLevel1Speed(float _speed);

    /**
     * @brief 第二级摩擦轮加速
     * @param _speed The desired speed.
     */
    void SetFricLevel2Speed(float _speed);

    /**
     * @brief Initialize the PID controllers.
     */
    void PidInit();

    /**
     * @brief Initialize the motor objects.
     */
    void MotorInit();

   private:
};

/* Exported variables --------------------------------------------------------*/
extern Shoot shoot;
/* Exported function prototypes ----------------------------------------------*/

#endif

#endif /* __SHOOT_H_ */
