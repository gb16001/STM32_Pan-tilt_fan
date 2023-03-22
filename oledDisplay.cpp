#include "oledDisplay.h"

/// @brief 反色显示
/// @param i 0正常，1反色
void OLED_ColorTurn(u8 i)
{
    if (!i)
        OLED_WR_Byte(0xA6, OLED_CMD); // 正常显示
    else
        OLED_WR_Byte(0xA7, OLED_CMD); // 反色显示
    return;
}

/// @brief 屏幕旋转180度
/// @param i 0正常/1旋转
void OLED_DisplayTurn(u8 i)
{
    if (i == 0)
    {
        OLED_WR_Byte(0xC8, OLED_CMD); // 正常显示
        OLED_WR_Byte(0xA1, OLED_CMD);
    }
    if (i == 1)
    {
        OLED_WR_Byte(0xC0, OLED_CMD); // 反转显示
        OLED_WR_Byte(0xA0, OLED_CMD);
    }
}

// 发送一个字节
// 向SSD1306写入一个字节。
// mode:数据/命令标志 0,表示命令;1,表示数据;

/// @brief 向SSD1306写入一个字节
/// @param dat 数据
/// @param mode 数据/命令标志 0,表示命令;1,表示数据
void OLED_WR_Byte(u8 dat, u8 mode)
{
    Wire.beginTransmission(0x3c);
    if (mode)
    {
        Wire.write(0x40);
    }
    else
    {
        Wire.write(0x00);
    }
    Wire.write(dat);        // sends one byte
    Wire.endTransmission(); // stop transmitting
}

/// @brief 坐标设置 x，y<0,则位置不变
/// @param x 横像素坐标
/// @param y 纵行坐标，一行8像素
void OLED_Set_Pos(char x, char y)
{
    if (x >= 0 && y >= 0)
    {
        OLED_WR_Byte(0xb0 + y, OLED_CMD);
        OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
        OLED_WR_Byte((x & 0x0f), OLED_CMD);
    }
    else
        return;
}
// 开启OLED显示

/// @brief 开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD); // DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD); // DISPLAY ON
}

/// @brief 关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD); // SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD); // DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD); // DISPLAY OFF
}
/// @brief 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear()
{
    u8 i, n;
    for (i = 0; i < 4; i++)
    {
        OLED_WR_Byte(0xb0 + i, OLED_CMD); // 设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);     // 设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);     // 设置显示位置—列高地址
        for (n = 0; n < 128; n++)
            OLED_WR_Byte(0, OLED_DATA);
    } // 更新显示
}

/// @brief 在指定位置显示一个字符
/// @param x 横向像素位置0~127，x=y=-1在旧位置继续打印
/// @param y 纵向行位置（8像素一行），0~63
/// @param chr （打印的字符的ascii）
/// @param sizey 选择大小8/16
void OLED_ShowChar(int x, int y, const u8 chr, u8 sizey)
{
    u8 c = 0, sizex = sizey / 2, temp;
    u16 i = 0, size1;
    if (sizey == 8)
        size1 = 6;
    else
        size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * (sizey / 2);
    c = chr - ' '; // 得到偏移后的值
    if (x >= 0 && y >= 0)
        OLED_Set_Pos(x, y);
    for (i = 0; i < size1; i++)
    {
        if (i % sizex == 0 && sizey != 8)
            OLED_Set_Pos(x, y++);
        if (sizey == 8)
        {
            temp = asc2_0806[c][i];
            OLED_WR_Byte(temp, OLED_DATA); // 6X8字号
        }
#ifndef EASY_FONT
        else if (sizey == 16)
        {
            temp = pgm_read_byte(&asc2_1608[c][i]);
            OLED_WR_Byte(temp, OLED_DATA); // 8x16字号
        }
#endif
        else
            return;
    }
}
/// @brief 显示一个字符串
/// @param x 横向像素位置坐标0~127
/// @param y 纵向行位置坐标0~63
/// @param chr 字符串地址（英文）
/// @param sizey 每个字符的大小 8/16
void OLED_ShowString(char x, char y, const char *chr, u8 sizey)
{
    u8 j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j++], sizey);
        if (sizey == 8)
            x += 6;
        else
            x += 8;
    }
}
/// @brief 计算幂函数m^n
/// @param m 底数
/// @param n 指数
/// @return m^n
u32 oled_pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)
        result *= m;
    return result;
}

/// @brief 显示数字
/// @param x 横向像素位置坐标
/// @param y 纵向行位置坐标
/// @param num 要显示的数字
/// @param len 数字的位数
/// @param sizey 字体大小
void OLED_ShowNum(char x, char y, int num, u8 len, u8 sizey)
{
    u8 t, temp, m = 0;
    u8 enshow = 0;
    if (sizey == 8)
        m = 2;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (sizey / 2 + m) * t, y, ' ', sizey);
                continue;
            }
            else
                enshow = 1;
        }
        OLED_ShowChar(x + (sizey / 2 + m) * t, y, temp + '0', sizey);
    }
}
#ifndef EASY_FONT
/// @brief 显示汉字
/// @param x 坐标
/// @param y 坐标
/// @param no 汉字位号
/// @param sizey 字体大小
void OLED_ShowChinese(char x, char y, const u8 no, u8 sizey)
{
    u16 i, size1 = (sizey / 8 + ((sizey % 8) ? 1 : 0)) * sizey;
    u8 temp;
    for (i = 0; i < size1; i++)
    {
        if (i % sizey == 0)
            OLED_Set_Pos(x, y++);
        if (sizey == 16)
        {
            temp = pgm_read_byte(&Hzk[no][i]);
            OLED_WR_Byte(temp, OLED_DATA); // 16x16字号
        }
        //    else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);//用户添加字号
        else
            return;
    }
}
#endif
/// @brief 显示图片
/// @param x 显示坐标
/// @param y 显示坐标
/// @param sizex 图片长宽
/// @param sizey 图片长宽
/// @param BMP 要显示的图片
void OLED_DrawBMP(char x, char y, u8 sizex, u8 sizey, const u8 BMP[])
{
    u16 j = 0;
    u8 i, m, temp;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (i = 0; i < sizey; i++)
    {
        OLED_Set_Pos(x, i + y);
        for (m = 0; m < sizex; m++)
        {
            OLED_WR_Byte(BMP[j++], OLED_DATA);
        }
    }
}

/// @brief OLED初始化
/// @param sda iic数据线，-1默认（D21）
/// @param scl iic地址线，-1默认（D22）
void OLED_Init(int sda, int scl, uint32_t freq)
{
    pinMode(res, OUTPUT); // RES
    if (sda == -1 && scl == -1 && freq == 0)
    {
        Wire.begin(); // 使用默认iic（scl22，sda21）
    }
    else
    {
        Wire.begin(); // 可以用来修改iic通信引脚，但是依然推荐默认
    }
    OLED_RES_Clr();
    delay(200);
    OLED_RES_Set();

    OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
    OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set display start line*/
    OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
    OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
    OLED_WR_Byte(0xff, OLED_CMD); /*128*/
    OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
    OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
    OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
    OLED_WR_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
    OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
    OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
    OLED_WR_Byte(0x80, OLED_CMD);
    OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
    OLED_WR_Byte(0x1f, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD); /*display ON*/
}

/// @brief 创建初始化oled显示
/// @param sda iic DA引脚
/// @param scl iic CLK引脚
/// @param t_color 是否反色显示
/// @param t_display 是否旋转屏幕
///@param frequency for Frequency by default Let's use ~700khz if ESP8266 is in 160Mhz mode, this will be limited to ~400khz if the ESP8266 in 80Mhz mode
oledDisplay::oledDisplay(int sda, int scl, bool t_color, bool t_display, uint32_t frequency)
{
    this->scl = scl;
    this->sda = sda;
    this->turnColor = t_color;
    this->turnDisplay = t_display;
    // OLED_Init(sda, scl, frequency); // 可以用来修改iic通信引脚，但是依然推荐默认
}
oledDisplay::oledDisplay()
{
    // OLED_Init();         // 使用默认iic（scl22，sda21）
}
oledDisplay::~oledDisplay()
{
    OLED_Clear();
    OLED_Display_Off();
}
/// @brief 打印字符，字母和数字和换行
/// @param character 字符
void oledDisplay::operator<(const char character)
{
    // OLED_ShowChar(-1, -1, character, 8);

    // u8 c = character - ' ';
    switch (character)
    {
    case '\n':
        this->setPrintPosition(0, this->char_position_y + 1);
        break;
    default:
        if (this->char_position_x < this->size_x - 6)
        {
            for (int i = 0; i < 6; i++)
            {
                OLED_WR_Byte(asc2_0806[character - ' '][i], OLED_DATA);
            }
            this->char_position_x += 6;
            break;
        }

        else
        {
            *this < endl;
            for (int i = 0; i < 6; i++)
            {
                OLED_WR_Byte(asc2_0806[character - ' '][i], OLED_DATA);
            }
            this->char_position_x += 6;
            break;
        }
    }
}
/// @brief 打印字符串
/// @param text 字符串
void oledDisplay::operator<(const char text[])
{
    // OLED_ShowString(-1, -1, text, 8);
    int i = 0;
    while (text[i] != '\0')
    {
        *this < text[i++];
    }
}
/// @brief 打印字符串,并换行
/// @param text 字符串
void oledDisplay::operator<<(const char text[])
{
    *this < text;
    *this < endl;
}
/// @brief 清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void oledDisplay::clear()
{
    OLED_Clear();
}
/// @brief 设置光标位置
/// @param x 横向像素位置
/// @param y 纵向行位置（8pixel）
void oledDisplay::setPrintPosition(int x, int y)
{
    x %= this->size_x;
    y %= this->size_y;
    this->char_position_x = x;
    this->char_position_y = y;
    // OLED_Set_Pos(x, y);
    if (x >= 0 && y >= 0)
    {
        OLED_WR_Byte(0xb0 + y, OLED_CMD);
        OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
        OLED_WR_Byte((x & 0x0f), OLED_CMD);
    }
    else
        return;
}
/// @brief 设置屏幕大小
/// @param x 横向分辨率
/// @param y 纵向行数(8像素/行)
void oledDisplay::setSize(u8 x, u8 y)
{
    this->size_x = x;
    this->size_y = y;
}
/// @brief 初始化，开启oled
void oledDisplay::init()
{
    pinMode(res, OUTPUT); // RES
    if (this->sda == -1 && this->scl == -1 && this->frequency == 0)
    {
        Wire.begin(); // 使用默认iic（scl22，sda21）
    }

    OLED_RES_Clr();
    delay(200);
    OLED_RES_Set();

    OLED_WR_Byte(0xAE, OLED_CMD); /*display off*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set lower column address*/
    OLED_WR_Byte(0x10, OLED_CMD); /*set higher column address*/
    OLED_WR_Byte(0x00, OLED_CMD); /*set display start line*/
    OLED_WR_Byte(0xB0, OLED_CMD); /*set page address*/
    OLED_WR_Byte(0x81, OLED_CMD); /*contract control*/
    OLED_WR_Byte(0xff, OLED_CMD); /*128*/
    OLED_WR_Byte(0xA1, OLED_CMD); /*set segment remap*/
    OLED_WR_Byte(0xA6, OLED_CMD); /*normal / reverse*/
    OLED_WR_Byte(0xA8, OLED_CMD); /*multiplex ratio*/
    OLED_WR_Byte(0x1F, OLED_CMD); /*duty = 1/32*/
    OLED_WR_Byte(0xC8, OLED_CMD); /*Com scan direction*/
    OLED_WR_Byte(0xD3, OLED_CMD); /*set display offset*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xD5, OLED_CMD); /*set osc division*/
    OLED_WR_Byte(0x80, OLED_CMD);
    OLED_WR_Byte(0xD9, OLED_CMD); /*set pre-charge period*/
    OLED_WR_Byte(0x1f, OLED_CMD);
    OLED_WR_Byte(0xDA, OLED_CMD); /*set COM pins*/
    OLED_WR_Byte(0x00, OLED_CMD);
    OLED_WR_Byte(0xdb, OLED_CMD); /*set vcomh*/
    OLED_WR_Byte(0x40, OLED_CMD);
    OLED_WR_Byte(0x8d, OLED_CMD); /*set charge pump enable*/
    OLED_WR_Byte(0x14, OLED_CMD);
    OLED_Clear();
    OLED_WR_Byte(0xAF, OLED_CMD);        /*display ON*/
    OLED_ColorTurn(this->turnColor);     // 0正常显示 1反色显示
    OLED_DisplayTurn(this->turnDisplay); // 0正常显示 1翻转180度显示
}
/// @brief 打印整数
/// @param num
void oledDisplay::operator<(const int num)
{
    this->operator<(String(num));
}
/// @brief 打印字符串类
/// @param text_sting
void oledDisplay::operator<(const String text_sting)
{
    for (uint8 i = 0; i < text_sting.length(); i++)
    {
        *this < text_sting[i];
    }
}
/// @brief 打印浮点数
/// @param real 
void oledDisplay::operator<(const float real)
{
    *this < (String(real));
}
