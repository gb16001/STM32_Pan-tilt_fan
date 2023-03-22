#ifndef OLED_H
#define OLED_H
#include <Arduino.h>
#include "oledfont.h"
#include "Wire.h"
#include <pins_arduino.h>
#include "HardwareSerial.h"
#define res PA3 // RES

#define OLED_RES_Clr() digitalWrite(res, LOW) // RES
#define OLED_RES_Set() digitalWrite(res, HIGH)

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

void OLED_ColorTurn(u8 i = 0);
void OLED_DisplayTurn(u8 i = 0);
void OLED_WR_Byte(u8 dat, u8 mode);
void OLED_Set_Pos(char x = -1, char y = -1);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear();
void OLED_ShowChar(int x = -1, int y = -1, const u8 chr = ' ', u8 sizey = 8);
void OLED_ShowString(char x, char y, const char *chr, u8 sizey = 8);
u32 oled_pow(u8 m, u8 n);
void OLED_ShowNum(char x, char y, int num, u8 len, u8 sizey = 8);
void OLED_ShowChinese(char x, char y, const u8 no, u8 sizey);
void OLED_DrawBMP(char x, char y, u8 sizex, u8 sizey, const u8 BMP[]);
void OLED_Init(int sda = -1, int scl = -1, uint32_t freq = 0);
class oledDisplay
{
private:
    int scl = -1, sda = -1;

public:
    bool turnColor = false, turnDisplay = false;
    int char_position_y = 0, char_position_x = 0;
    u8 size_x = 128, size_y = 4;
    #ifdef ARDUINO_ARCH_ESP32
uint32_t frequency=700000;
    #elif defined(ARDUINO_ARCH_ESP8266)
uint32_t frequency=400000;
    #else
uint32_t frequency=0;
    #endif
    
    oledDisplay(int sda, int scl, bool t_color = false, bool t_display = false, uint32_t frequency = 0);
    oledDisplay();
    ~oledDisplay();
    void init();
    void print(char text[]);
    void print(char character);
    void println(char text[]);
    void println(char character);
    void operator<<(const char text[]);
    void operator<(const char character);
    void operator<(const char text[]);
    void operator<(const int num);
    void operator<(const float real);
    void operator<(const String text_sting);
    void setSize(u8 x, u8 y);
    void setPrintPosition(int x, int y);
    #define endl '\n'
    void clear();
};


#endif