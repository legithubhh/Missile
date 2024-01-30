/**
 *******************************************************************************
 * @file      : user_lib.cpp
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
/* Includes ------------------------------------------------------------------*/
#include "user_lib.h"
/* Private macro -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

namespace Math
{
/**
 * @brief Calculates the square root of a given number.
 *
 * @param _x The number for which the square root is to be calculated.
 * @return The square root of the given number.
 */
float Sqrt(float _x)
{
    float y;
    float delta;
    float maxError;

    if (_x <= 0) {
        return 0;
    }

    // initial guess
    y = _x / 2;

    // refine
    maxError = _x * 0.001f;

    do {
        delta = (y * y) - _x;
        y -= delta / (2 * y);
    } while (delta > maxError || delta < -maxError);

    return y;
}

/**
 * @brief Calculates the absolute value of a number with a specified limit.
 *
 * This function calculates the absolute value of the given number '_num'. If the absolute value exceeds the specified limit '_limit',
 * the function returns the limit value with the same sign as the original number. Otherwise, it returns the absolute value of the number.
 *
 * @param _num The number for which the absolute value is to be calculated.
 * @param _limit The limit value beyond which the absolute value will be capped.
 * @return The absolute value of the number, capped at the specified limit.
 */
float AbsLimit(float _num, float _max, float _min)
{
    if (_num > _max) {
        _num = _max;
    } else if (_num < _min) {
        _num = _min;
    }
    return _num;
}

/**
 * @brief Returns the sign of a floating-point value.
 *
 * @param _value The value to determine the sign of.
 * @return The sign of the value: -1 if the value is negative, 0 if the value is zero, 1 if the value is positive.
 */
float Sign(float _value)
{
    if (_value >= 0.0f) {
        return 1.0f;
    } else {
        return -1.0f;
    }
}

/**
 * @brief Applies deadband to a floating-point value.
 *
 * This function takes a floating-point value and applies a deadband to it. The deadband is defined by a minimum value and a maximum value. If the input value is within the deadband range, it is clamped to zero. If the input value is outside the deadband range, it is returned unchanged.
 *
 * @param _value The input value to apply deadband to.
 * @param _min_value The minimum value of the deadband range.
 * @param _max_value The maximum value of the deadband range.
 * @return The input value with deadband applied.
 */
float FloatDeadband(float _value, float _min_value, float _max_value)
{
    if (_value < _max_value && _value > _min_value) {
        _value = 0.0f;
    }
    return _value;
}

/**
 * @brief Constrain a float value within a specified range.
 *
 * This function takes a float value and constrains it within the specified minimum and maximum values.
 * If the value is less than the minimum, it will be set to the minimum.
 * If the value is greater than the maximum, it will be set to the maximum.
 *
 * @param _value The float value to be constrained.
 * @param _min_value The minimum value to constrain to.
 * @param _max_value The maximum value to constrain to.
 * @return The constrained float value.
 */
float FloatConstrain(float _value, float _min_value, float _max_value)
{
    if (_value < _min_value)
        return _min_value;
    else if (_value > _max_value)
        return _max_value;
    else
        return _value;
}

/**
 * @brief Constrain the given value between the specified minimum and maximum values.
 *
 * @param _value The value to be constrained.
 * @param _min_value The minimum value.
 * @param _max_value The maximum value.
 * @return The constrained value.
 */
int16_t Int16Constrain(int16_t _value, int16_t _min_value, int16_t _max_value)
{
    if (_value < _min_value)
        return _min_value;
    else if (_value > _max_value)
        return _max_value;
    else
        return _value;
}

/**
 * @brief Constrains a floating-point value within a specified range.
 *
 * This function takes a floating-point value `_value` and constrains it within the range defined by `_min_value` and `_max_value`.
 * If `_value` is less than `_min_value`, it will be set to `_max_value`.
 * If `_value` is greater than `_max_value`, it will be set to `_min_value`.
 *
 * @param _value The value to be constrained.
 * @param _min_value The minimum value of the range.
 * @param _max_value The maximum value of the range.
 * @return The constrained value.
 */
float LoopFloatConstrain(float _value, float _min_value, float _max_value)
{
    if (_max_value < _min_value) {
        return _value;
    }

    if (_value > _max_value) {
        float len = _max_value - _min_value;
        while (_value > _max_value) {
            _value -= len;
        }
    } else if (_value < _min_value) {
        float len = _max_value - _min_value;
        while (_value < _min_value) {
            _value += len;
        }
    }
    return _value;
}

/**
 * @brief 将角度格式化为特定范围内的值。
 *
 * @param _ang 待格式化的角度值。
 * @return 格式化后的角度值。
 */
float ThetaFormat(float _ang)
{
    return LoopFloatConstrain(_ang, -180.0f, 180.0f);
}

/**
 * @brief Rounds a floating-point number to the nearest integer.
 *
 * @param _raw The floating-point number to be rounded.
 * @return The rounded integer value.
 */
int FloatRounding(float _raw)
{
    static int integer;
    static float decimal;
    integer = (int)_raw;
    decimal = _raw - integer;
    if (decimal > 0.5f)
        integer++;
    return integer;
}

}  // namespace Math
