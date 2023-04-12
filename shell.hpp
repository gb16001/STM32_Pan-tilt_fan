#ifndef _the_shell_
#define _the_shell_
#include <Arduino.h>
#include "my_18b20.hpp" //温度传感器

#include "mySerial.hpp"     //串口初始化
#include "oledDisplay.h"    //显示屏驱动
#include "HardwareSerial.h" //硬件串口
#include "myFan.hpp"        //风扇控制

#include "mg995.hpp"//定时中断控制舵机
// #include "mg_delay.hpp"     //延时函数控制舵机
/*硬件资源类抽象*/

oledDisplay myOled(-1, -1, false, false, 700000);
/*
命令格式：｛字母｝ ｛数字参数｝
一个字母开头选择功能，后面的参数一个空格后直接写上
*/
/// @brief 理解并执行命令
/// @param command 要执行的命令
void commandUnderstand(String command)
{
    switch (command[0])
    {
    case 't': // 测试命令
        command.remove(0, 2);
        // int argdata= command.toInt();
        Serial.print("get a num:");
        Serial.println(command.toInt());
        break;
#ifdef MG995_H
    case 'm': // 舵机命令
        command.remove(0, 2);
        hd_Mg01.digChange(command.toInt());
        break;
    case 'n':
        command.remove(0, 2);
        hd_Mg02.digChange(command.toInt());
        break;
#endif
#ifndef MG995_H
#ifdef MG_delay_H
    case 'm': // 舵机命令
        command.remove(0, 2);
        mg_01.digChange(command.toInt());
#ifdef DEBUG_PRINT
        Serial.print("mg1 dig=");
        Serial.println(command.toInt());
#endif
        break;
    case 'n': // 舵机2命令
        command.remove(0, 2);
        mg_02.digChange(command.toInt());
#ifdef DEBUG_PRINT
        Serial.print("mg2 dig=");
        Serial.println(command.toInt());
#endif
        break;
#endif
#endif
    case 'o': // OLED显示屏命令
        command.remove(0, 2);
        if (command == "clear\n")
        {
            myOled.clear();
        }
        else
        {
            myOled < command;
        }

        break;
    case 'f': // 风扇命令
        command.remove(0, 2);
        // 这里写风扇控制函数
        break;
    case 'T': // 温度传感器命令
        Serial.print("temp is");
        Serial.println(ds.getTempC());
        break;
    default:
        break;
    }
}
#endif