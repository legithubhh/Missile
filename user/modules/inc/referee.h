/**
 *******************************************************************************
 * @file      : referee.h
 * @brief     :
 * @history   :
 *  Version     Date            Author          Note
 *  V0.9.0      yyyy-mm-dd      <author>        1. <note>
 *******************************************************************************
 * @attention :
 *******************************************************************************
 *  Copyright (c) 2024 Reborn Team, University of Science and Technology Beijing.
 *  All Rights Reserved.
 *******************************************************************************
 */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __REFEREE_H_
#define __REFEREE_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported macro ------------------------------------------------------------*/
#define SolveFloatValueStr_Init       \
    {                                 \
        {0, 0, 0, 0},                 \
            0,                        \
            0,                        \
            0,                        \
            0,                        \
            0.0,                      \
            &FloatValueSolveFunction, \
            &solve_chengfang,         \
    }
#define BUFFER_MAX 100

/* Exported constants --------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef struct
{
    uint8_t sof;           // 数据帧头，固定0xA5
    uint16_t data_length;  // 每帧内数据data的长度
    uint8_t seq;           // 包序号
    uint8_t crc8;          // 帧头的crc校验结果
} frame_header_t;

typedef struct
{
    uint8_t robot_id;
    uint16_t stageRemainTime;  // 当前阶段剩余时间
    uint8_t gameProgress;      // 当前比赛处于哪个阶段
    uint8_t robotLevel;        // 机器人当前等级
    uint16_t remainHP;         // 机器人当前血量
    uint16_t maxHP;            // 机器人满血量
} extGameRobotState_t;         // 比赛机器人状态

typedef struct
{
    uint8_t armorType;  // 因装甲被击打掉血时，标示受到伤害的装甲ID
    uint8_t hurtType;   // 血量变化类型
} extRobotHurt_t;       // 伤害数据

typedef struct
{
    uint8_t bulletType;  // 弹丸类型  1：17mm弹丸   2：42mm弹丸
    uint8_t bulletFreq;  // 弹丸射频
    float bulletSpeed;   // 弹丸射速
} extShootData_t;        // 实时射击信息

typedef struct
{
    float ChassisVolt;         // 底盘输出电压
    float ChassisCurrent;      // 底盘输出电流
    float ChassisPower;        // 底盘输出功率
    float ChassisPowerBuffer;  // 底盘功率缓冲
    uint16_t shooter_17_Heat;  // 17mm枪口热量
    uint16_t shooter_42_Heat;  // 42mm枪口热量
} extPowerHeatData_t;          // 实时功率热量数据

typedef struct
{
    uint8_t cardType;  // 卡类型
    uint8_t cardIdx;   // 卡索引号
} extRfidDetect_t;     // 场地交互数据

typedef struct
{
    uint8_t winner;
} extGameResult_t;  // 比赛胜负数据

typedef struct
{
    uint8_t buffType;      // buff类型
    uint8_t buffAddition;  // buff加成比
} extGetBuff_t;            // Buff获取数据

typedef struct
{
    float x;
    float y;
    float z;
    float yaw;        // 枪口朝向角度值
} extGameRobotPos_t;  // 机器人位置角度信息

typedef struct
{
    float data1;
    float data2;
    float data3;
    uint8_t mask;
} extShowData_t;  // 参赛队自定义数据

typedef struct GetFloatValueStrcut {
    int Temp_Array[4];
    long int Combination_Array;
    int JieMa;
    long int WeiShu;
    int WeiShu_EveryBit;
    float Float_Value;
    float (*FloatValueSolve)(struct GetFloatValueStrcut *date);
    float (*ChengFang_Solution)(float X, int N);
} GetFloatValueStrcut;

typedef struct
{
    uint8_t game_type : 4;
    uint8_t game_progress : 4;
    uint16_t stage_remain_time;
    uint64_t SyncTimeStamp;
} ext_game_status_t;

typedef struct
{
    int headPosition;
    int tailPosition;
    uint8_t ringBuf[BUFFER_MAX];
} ringBuffer_t;

typedef struct  // 飞镖状态
{
    uint8_t dart_launch_opening_status;
    uint8_t dart_attack_target;
    uint8_t last_dart_attack_target;
    uint16_t target_change_time;
    uint16_t operate_launch_cmd_time;
} ext_dart_client_cmd_t;

typedef struct  // 飞镖发射时间
{
    uint8_t dart_remaining_time;
} ext_dart_remaining_time_t;
/* Exported variables --------------------------------------------------------*/
extern extGameRobotState_t robotState;
extern extRobotHurt_t robotHurt;
extern extPowerHeatData_t robotPowerHeat;
extern extShootData_t robotShootData;
extern ringBuffer_t buffer;
extern int Yaw_encoder;
extern int Yaw_encoder_s;
extern ext_dart_client_cmd_t Dart_State;     //飞镖状态
extern ext_dart_remaining_time_t Dart_Time;  //飞镖发射时间
/* Exported function prototypes ----------------------------------------------*/
float FloatValueSolveFunction(struct GetFloatValueStrcut *date);
float solve_chengfang(float x, int n);
int Transform_Hex_To_Oct(int data, int len);

void getRobotState(uint8_t *stateData);
void getRobotHurt(uint8_t *hurtData);
void getRobotPowerHeat(uint8_t *powerHeartData);
void getRobotShootData(uint8_t *shootData);
void getDartShootState(uint8_t *stateData);
void getDart_time(uint8_t *stateData);
void RingBuffer_Write(uint8_t data);
void getGameState(uint8_t *stateData);
uint8_t RingBuffer_Read(uint8_t *pdata);
#endif /* __REFEREE_H_ */