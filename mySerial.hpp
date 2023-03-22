#ifndef _mySerial_
#define _mySerial_
#include "HardwareSerial.h"
String serial_read_box;
/// @brief 初始化串口，board=115200
void serialInit()
{
    Serial.begin(115200);
    Serial2.begin(115200);
}
/// @brief 读取串口1的输入（目前只支持串口1）
/// @param box 字符串存放处
/// @return 有输入1，无输入0
bool serial_readCommand(String &box)
{
    if (Serial.available())
    {
        box = Serial.readString();
        // serial_read_box = Serial.readString();
        return true;
    }
    else
    {
        box = "\0";
        return false;
    }
}
/// @brief 打招呼，两个串口都进行
void serial_sayHI()
{
    Serial.println("hi would");
    Serial2.println("hi would");
}
/// @brief 两串口信息透传
void serial_exchange_message_in_2serial()
{
    if (Serial.available())
    { // If anything comes in Serial (USB),
        // serial_read_box=Serial.readString();
        Serial2.println(Serial.readString());
    }

    if (Serial2.available())
    { // If anything comes in Serial2 (pins 0 & 1)
        Serial.println(Serial2.readString());
    }
}
#endif
