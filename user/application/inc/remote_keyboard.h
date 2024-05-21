/**
 *******************************************************************************
 * @file      : remote_keyboard.h
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
#ifndef __REMOTE_KEYBOARD_H_
#define __REMOTE_KEYBOARD_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint16_t fashe_flag;
extern float time_this;
extern float time_real;
/* Exported function prototypes ----------------------------------------------*/
void ModeTask();
void DoorInit();

#ifdef __cplusplus
}
#endif

#endif /* __REMOTE_KEYBOARD_H_ */
