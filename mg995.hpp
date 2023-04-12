#ifndef MG995_H
#define MG995_H
#include <Arduino.h>
#include <HardwareTimer.h>
// typedef uint8_t uint8;
uint16 pwm_10us = 0, pwm_10us_2 = 0;          // 用于handler使用得知高电平时间
uint8 handlerPinBox = 0, handlerPinBox_2 = 0; // 用于handler使用得知使用引脚
void handler1(void);                          // 中断函数1，引脚高
void handler2(void);                          // 中断函数2，引脚低
void handler3(void); 
class MG995_hdTimer // 中断控制电机
{
private:
public:
    uint8 pwmPin = 0, pwmPin2 = 0;
    uint8 pwmUptime_10us = 0, pwmUptime_10us_2 = 0;
    int timer_channel = 1;
    HardwareTimer *timer;
    MG995_hdTimer(/* args */){};
    ~MG995_hdTimer(){};
    /// @brief 初始化
    /// @param pin 选择电机pwm连接引脚
    /// @param channel 定时器通道（目前不支持修改，只能用timer3）
    /// @param time up时间（10us）默认500us（0度）
    void init(uint8 pin, uint8 pin2 = PB9, HardwareTimer timer = Timer3, int channel = TIMER_CH1, uint16 time = 50, uint16 time2 = 50)
    {

        this->pwmPin = pin;
        this->pwmPin2 = pin2;
        handlerPinBox = pin;
        handlerPinBox_2 = pin2;
        pwm_10us = this->pwmUptime_10us = time + 1;
        pwm_10us_2 = this->pwmUptime_10us_2 = time2 + 1;
        this->timer_channel = channel;
        this->timer = &timer;
        pinMode(this->pwmPin, OUTPUT);
        pinMode(this->pwmPin2, OUTPUT);
        // 对定时中断的初始化没写
        timer.setMode(this->timer_channel, TIMER_OUTPUTCOMPARE);
        timer.pause();
        timer.setCount(0);
        timer.setPrescaleFactor(720); // 72Mhz/720=10khz=0.01ms
        timer.setOverflow(2000);      // 10k/2000=50hz=20ms
        timer.refresh();
        timer.setCompare(this->timer_channel, 1);             // 0.01ms
        timer.attachInterrupt(this->timer_channel, handler1); // pin up,change compare to
        timer.resume();
    }

    /// @brief 改变角度
    /// @param time_10us up time(10us),50-250->0-180 digree
    void sendDigChange(uint16 time_10us)
    {
        pwm_10us = this->pwmUptime_10us = time_10us + 1;
    }

    /// @brief 改变角度
    /// @param digree 0-180 digree
    void digChange(uint16 digree)
    {
        pwm_10us = this->pwmUptime_10us = (digree * 20 / 18.0) + 51;
    }
    /// @brief 改变角度,舵机2
    /// @param digree 0-180 digree
    void digChange_2(uint16 digree)
    {
        pwm_10us_2 = this->pwmUptime_10us_2 = (digree * 20 / 18.0) + 51;
    }
};
bool mg_timer_flage = false; // 两个舵机谁先到期
void handler1(void)
{
    
    digitalWrite(handlerPinBox, HIGH);
    digitalWrite(handlerPinBox_2, HIGH);
    if (pwm_10us < pwm_10us_2)
    {
        mg_timer_flage = false;
        Timer3.setCompare(TIMER_CH1, pwm_10us);
    }
    else
    {
        mg_timer_flage = true;
        Timer3.setCompare(TIMER_CH1, pwm_10us_2);
    }
    Timer3.attachInterrupt(TIMER_CH1, handler2);
}
void handler2(void)
{
    if (!mg_timer_flage)
    {
        digitalWrite(handlerPinBox, LOW);
        Timer3.setCompare(TIMER_CH1, pwm_10us_2);
    }
    else
    {
        digitalWrite(handlerPinBox_2, LOW);
        Timer3.setCompare(TIMER_CH1, pwm_10us);
    }

    Timer3.attachInterrupt(TIMER_CH1, handler3);
}
void handler3()
{
    digitalWrite(handlerPinBox, LOW);
    digitalWrite(handlerPinBox_2, LOW);
    Timer3.setCompare(TIMER_CH1, 1);
    Timer3.attachInterrupt(TIMER_CH1, handler1);
}
MG995_hdTimer hd_Mg01;
#endif

#define MG995_02_H
#ifndef MG995_02_H
#define MG995_02_H
uint16 pwm_10us_2 = 0;     // 用于handler使用得知高电平时间
uint8 handlerPinBox_2 = 0; // 用于handler使用得知使用引脚
void handler1_2(void);     // 中断函数1，引脚高
void handler2_2(void);     // 中断函数2，引脚低
class mg995_hdTimer_2 : public MG995_hdTimer
{
private:
    /* data */
public:
    /// @brief 初始化
    /// @param pin 选择电机pwm连接引脚
    /// @param channel 定时器通道（目前不支持修改，只能用timer3）
    /// @param time up时间（10us）默认500us（0度）
    void init(uint8 pin, HardwareTimer timer = Timer4, int channel = TIMER_CH2, uint16 time = 50)
    {
        this->pwmPin = pin;
        handlerPinBox_2 = pin;
        pwm_10us_2 = this->pwmUptime_10us = time + 1;
        this->timer_channel = channel;
        this->timer = &timer;
        pinMode(this->pwmPin, OUTPUT);
        Serial.println("set mg2 interrupt");
        // 对定时中断的初始化没写
        timer.setMode(this->timer_channel, TIMER_OUTPUTCOMPARE);
        timer.pause();
        timer.setCount(0);
        timer.setPrescaleFactor(720); // 72Mhz/720=10khz=0.01ms
        timer.setOverflow(2000);      // 10k/2000=50hz=20ms
        timer.refresh();
        timer.setCompare(this->timer_channel, 1);               // 0.01ms
        timer.attachInterrupt(this->timer_channel, handler1_2); // pin up,change compare to
        Serial.println("set mg2 interrupt finish");
        timer.resume();
    }
    /// @brief 改变角度
    /// @param digree 0-180 digree
    void digChange(uint16 digree)
    {
        pwm_10us_2 = this->pwmUptime_10us = (digree * 20 / 18.0) + 51;
    }
};
mg995_hdTimer_2 hd_Mg02;
void handler1_2(void)
{
    digitalWrite(handlerPinBox_2, HIGH);
    Timer4.setCompare(TIMER_CH2, pwm_10us_2);
    Timer4.attachInterrupt(TIMER_CH2, handler2_2);
    Serial.println("mg2 interrupt1");
}
void handler2_2(void)
{
    digitalWrite(handlerPinBox_2, LOW);
    Timer4.setCompare(TIMER_CH2, 1);
    Timer4.attachInterrupt(TIMER_CH2, handler1_2);
}

#endif
