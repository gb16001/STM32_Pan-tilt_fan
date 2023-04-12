#ifndef _the_shell_
#define _the_shell_
#include <Arduino.h>
#include "my_18b20.hpp"
#include "mg995.hpp"
#include "mySerial.hpp"
#include "oledDisplay.h"
#include "HardwareSerial.h"
#include "myFan.hpp"
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
    case 'm': // 舵机命令
        command.remove(0, 2);
        this_mg.digChange(command.toInt());
        break;
    case 'n':
        command.remove(0, 2);
        // 保留用作电机2的命令
        break;
    case 'o': // OLED显示屏命令
        command.remove(0, 2);
        if (command=="clear\n")
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