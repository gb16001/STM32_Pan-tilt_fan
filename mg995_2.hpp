#ifndef MG995_2_H
#define MG995_2_H
#include <Arduino.h>
#include <HardwareTimer.h>
// typedef uint8_t uint8;
uint16 pwm_10us = 0;     // 用于handler使用得知高电平时间
uint8 handlerPinBox = 0; // 用于handler使用得知使用引脚
void handler1(void);     // 中断函数1，引脚高
void handler2(void);     // 中断函数2，引脚低

class MG995_hdTimer // 中断控制电机
{
private:
public:
    uint8 pwmPin = 0;
    uint8 pwmUptime_10us = 0;
    int timer_channel = 1;
    HardwareTimer *timer;
    MG995_hdTimer(/* args */){};
    ~MG995_hdTimer(){};
    /// @brief 初始化
    /// @param pin 选择电机pwm连接引脚
    /// @param channel 定时器通道（目前不支持修改，只能用timer3）
    /// @param time up时间（10us）默认500us（0度）
    void init(uint8 pin, HardwareTimer timer = Timer3, int channel = TIMER_CH1, uint16 time = 50)
    {

        this->pwmPin = pin;
        handlerPinBox = pin;
        pwm_10us = this->pwmUptime_10us = time + 1;
        this->timer_channel = channel;
        this->timer = &timer;
        pinMode(this->pwmPin, OUTPUT);
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
};
void handler1(void)
{
    digitalWrite(handlerPinBox, HIGH);
    Timer3.setCompare(TIMER_CH1, pwm_10us);
    Timer3.attachInterrupt(TIMER_CH1, handler2);
}
void handler2(void)
{
    digitalWrite(handlerPinBox, LOW);
    Timer3.setCompare(TIMER_CH1, 1);
    Timer3.attachInterrupt(TIMER_CH1, handler1);
}
MG995_hdTimer hd_Mg02;
#endif


