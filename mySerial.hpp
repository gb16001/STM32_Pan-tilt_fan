#ifndef _mySerial_
#define _mySerial_
#include "HardwareSerial.h"
#define string_timeOut_ms 10 // 串口读取字符串等待时间（ms）
String serial_read_box;
/// @brief 初始化串口，board=115200
void serialInit()
{
    Serial.begin(115200);
    Serial2.begin(115200);
    Serial.setTimeout(string_timeOut_ms);
    Serial2.setTimeout(string_timeOut_ms);
}
/// @brief 读取串口1的输入（目前只支持串口1）
/// @param box 字符串存放处
/// @return 有输入1，无输入0
bool serial_readCommand(String &box)
{
    if (Serial.available())
    {
        
        // 方法一：读取string，等待setTimeout结束
        //  box = Serial.readString();

        // 方法二：读取until，读到\n结束
        box = Serial.readStringUntil('\n');

        
        // serial_read_box = Serial.readString();
        return true;
    }
    else
    {
        box = "\0";
        return false;
    }
}
/// @brief 清除串口1的读取缓冲
void serial_clearBuffer()
{
    while (Serial.available())
    {
        Serial.read();
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
