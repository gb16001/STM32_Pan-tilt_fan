#ifndef MY_18B20_H
#define MY_18B20_H
#include <DS18B20.h>

#define LOW_ALARM -10
#define HIGH_ALARM 50
// 这里选择ds18b20 DQ接入引脚
DS18B20 ds(PB8);
uint8_t ds_address[] = {40, 250, 31, 218, 4, 0, 0, 52};
// uint8_t selected;
/// @brief 初始化函数
/// @return 成功1不成功0
bool ds_init()
{
    return ds.select(ds_address);
}
// ds.select(ds_address)
// 选择ds18b20
// ds.getTempC()
// 函数返回摄氏度
#endif