/**
 *******************************************************************************
 * @file      : user_lib.h
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      yyyy-mm-dd      <author>        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2023 Reborn Team, USTB.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USER_LIB_H_
#define __USER_LIB_H_

#ifdef __cplusplus

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>

#include "stdint.h"
/* Exported macro ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
namespace Math
{
float Sqrt(float _x);
float AbsLimit(float _num, float _max, float _min);
float Sign(float _value);
float FloatDeadband(float _value, float _min_value, float _max_value);
float FloatConstrain(float _value, float _min_value, float _max_value);
int16_t Int16Constrain(int16_t _value, int16_t _min_value, int16_t _max_value);
float LoopFloatConstrain(float _value, float _min_value, float _max_value);
float ThetaFormat(float _ang);
int FloatRounding(float _raw);

};  // namespace Math
/* Exported variables --------------------------------------------------------*/
/* Exported function prototypes ----------------------------------------------*/

#endif

#endif /* __USER_LIB_H_ */