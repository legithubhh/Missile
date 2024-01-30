#include "referee.h"

#include "math.h"
extGameRobotState_t robotState;
extRobotHurt_t robotHurt;
extPowerHeatData_t robotPowerHeat;
extShootData_t robotShootData;
ext_dart_client_cmd_t Dart_State;
ext_dart_remaining_time_t Dart_Time;
ringBuffer_t buffer;
ext_game_status_t gameState;

uint32_t ChassisPower_temp;
uint32_t ChassisPower_buffer;
float chassisPowerBuffer = 0;  // 功率缓冲
float Parameter_Transformation(int32_t data);
int Yaw_encoder = 0;
int Yaw_encoder_s = 0;
float test_power = 0;

/***
函数：void getRobotState(uint8_t *stateData)
功能：从裁判系统读取机器人状态(当前血量)
备注：ID：0x0001
      共17个数据，下标11，12为当前血量数据
***/
void getRobotState(uint8_t *stateData)
{
    int tempStateData[22], i, tempBlood = 0, Robot_numbel;
    for (i = 0; i < 22; i++) {
        tempStateData[i] = stateData[i];
    }
    /***读取当前机器人ID***/
    Robot_numbel = tempStateData[7];
    robotState.robot_id = Robot_numbel;
    /***读取当前血量值***/
    tempBlood = tempStateData[9] | tempStateData[10] << 8;
    robotState.remainHP = Transform_Hex_To_Oct(tempBlood, 16);
}

/*********************************************
函数：Parameter_Transformation
功能：四字节二进制数装换为浮点数
**********************************************/
float Parameter_Transformation(int32_t data)
{
    int temp1, temp4;
    long temp2;
    float temp3;
    // temp1是阶码
    // temp2是尾数
    // temp3是最后算好的数
    // temp4是电流尾数的每一位
    temp1 = ((data & 0X7F800000) >> 23) - 127;
    temp2 = data & 0X007FFFFF;
    for (int j = 0; j < 24; j++) {
        if (j == 0) {
            temp3 = (float)ldexp(1.0, temp1);
        } else {
            temp4 = (temp2 & (0x00400000 >> (j - 1))) >> (23 - j);
            temp3 = temp3 + temp4 * (float)ldexp(1.0, temp1 - j);
        }
    }
    return temp3;
}

/*************************************
功能：4字节表示的浮点数转为10进制小数
入口参数：GetFloatValueStrcut类型结构体
出口参数：10进制小数值
备注：无
**************************************/
float FloatValueSolveFunction(struct GetFloatValueStrcut *date)
{
    int i;
    date->Combination_Array = (date->Temp_Array[0] << 0) | (date->Temp_Array[1] << 8) | (date->Temp_Array[2] << 16) | (date->Temp_Array[3] << 24);  // 倒序组合二进制形式表示的浮点数
    date->JieMa = ((date->Combination_Array & 0X7F800000) >> 23) - 127;
    date->WeiShu = date->Combination_Array & 0X007FFFFF;
    for (i = 0; i < 24; i++) {
        if (i == 0) {
            date->Float_Value = 1 * date->ChengFang_Solution(2, date->JieMa);
        } else {
            date->WeiShu_EveryBit = (date->WeiShu & (0X00400000 >> (i - 1))) >> (23 - i);
            date->Float_Value = date->Float_Value + date->WeiShu_EveryBit * date->ChengFang_Solution(2, date->JieMa - i);
        }
    }
    return date->Float_Value;
}

/*******************
功能：乘方计算函数
入口参数：x--底数
          n--指数
返回值：乘方计算值
*********************/
float solve_chengfang(float x, int n)
{
    int i;
    int a;
    float s = 1.0;
    if (n >= 0)
        a = n;
    if (n < 0)
        a = -n;
    for (i = 1; i <= a; i++)
        s *= x;
    if (n >= 0)
        return s;
    if (n < 0)
        return 1 / s;
    return 0;
}

/***
函数：int Transform_Hex_To_Oct(int data,int len)
功能：16进制数转换成10进制数
备注：data:16进制格式  len：data中2进制位数
***/
int Transform_Hex_To_Oct(int data, int len)
{
    int a = 0x0001, temp = 0;
    int ans = 0;
    for (int i = 0; i < len; i++) {
        temp = (data & (a << i)) >> i;
        ans = ans + temp * solve_chengfang(2, i);
    }
    return ans;
}

/***
函数：void RingBuffer_Write(uint8_t data)
功能：将数据data写入环形队列buffer.ringBuf中
备注：无
***/
void RingBuffer_Write(uint8_t data)
{
    buffer.ringBuf[buffer.tailPosition] = data;      // 从尾部追加
    if (++buffer.tailPosition >= BUFFER_MAX)         // 尾节点偏移
        buffer.tailPosition = 0;                     // 大于数组最大长度，归零，形成环形队列
    if (buffer.tailPosition == buffer.headPosition)  // 如果尾部节点追到头部节点，则修改头结点偏移位置丢弃早期数据
        if (++buffer.headPosition >= BUFFER_MAX)
            buffer.headPosition = 0;
}

/***
函数：u8 RingBuffer_Read(uint8_t *pdata)
功能：从环形队列buffer.ringBuf中读取数据到地址pdata中
备注：无
***/
uint8_t RingBuffer_Read(uint8_t *pdata)
{
    if (buffer.headPosition == buffer.tailPosition)  // 如果头尾接触表示缓冲区为空
    {
        return 1;  // 返回1，环形缓冲区是空的
    } else {
        *pdata = buffer.ringBuf[buffer.headPosition];  // 如果缓冲区非空则取头节点并偏移头节点
        if (++buffer.headPosition >= BUFFER_MAX)
            buffer.headPosition = 0;
        return 0;  // 返回0，表示读取数据成功
    }
}

/***
函数：void getDartShootState(uint8_t *stateData)
功能：从裁判系统读取飞镖状态
备注：ID：0x020A
      共15个数据
***/
void getDartShootState(uint8_t *stateData)
{
    Dart_State.dart_launch_opening_status = stateData[7] + 1;  // 发射口状态
    Dart_State.dart_attack_target = stateData[8];              // 打击目标 0前哨站 1基地
    //	Dart_State.target_change_time = stateData[9] | (stateData[10]<<8);//切换打击目标时比赛剩余时间
    //	Dart_State.operate_launch_cmd_time = stateData[11] | (stateData[12]<<8);//操作手确定发射指令时比赛剩余时间
}
/***
函数：void getDart_time(uint8_t *stateData)
功能：从裁判系统读取飞镖剩余时间
备注：ID：0x0105
      共7个数据
***/
void getDart_time(uint8_t *stateData)
{
    Dart_Time.dart_remaining_time = stateData[7];  // 15秒倒计时
}

void getGameState(uint8_t *stateData)
{
    int tempGameData[12], i, tempTime = 0, Game_state;
    for (i = 0; i < 11; i++) {
        tempGameData[i] = stateData[i];
    }
    /***比赛状态***/
    Game_state = tempGameData[7];
    gameState.game_type = (Game_state & 0x0F);
    gameState.game_progress = (Game_state & 0xF0) >> 4;
    /***剩余时间***/
    tempTime = tempGameData[8] | tempGameData[9] << 8;
    gameState.stage_remain_time = Transform_Hex_To_Oct(tempTime, 16);
}