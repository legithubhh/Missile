/**
 *******************************************************************************
 * @file      : gimbal.h
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
#ifndef __GIMBAL_H_
#define __GIMBAL_H_

#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/
void StartPitchPulse(uint8_t _r, uint16_t _step);
void PitchDirSet(uint8_t _dir);
void SetPushPWM(uint16_t _xpwm);
void PushDirSet(uint8_t _dir);
void StartYawPulse(uint8_t r, uint16_t _step);
void YawDirSet(uint8_t dir);
#endif

#endif /* __GIMBAL_H_ */
