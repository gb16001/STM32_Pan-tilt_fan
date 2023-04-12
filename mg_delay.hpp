
#ifndef MG_delay_H
#define MG_delay_H
#include <Arduino.h>
class MG995 // 顺序控制电机
{
private:
    /* data */
public:
    uint8_t pin = 0;
    MG995(){};
    ~MG995(){};
    void init(uint8_t pin)
    {
        this->pin = pin;
        pinMode(pin, OUTPUT);
    }
    /// @brief 发送PWM以改变角度
    /// @param PWM up time(ms),500-2500->0-180 digree
    void sendPWM(unsigned long PWM)
    {
        PWM = (PWM - 500) % 2000 + 500;
        digitalWrite(this->pin, HIGH);
        delayMicroseconds(PWM);
        digitalWrite(this->pin, LOW);
        delay(20);
    }
    /// @brief 改变角度
    /// @param digree 0-180 digree
    void digChange(uint16 digree)
    {
        unsigned long up_time_us = (digree * 200 / 18.0) + 500;
        digitalWrite(this->pin, HIGH);
        delayMicroseconds(up_time_us);
        digitalWrite(this->pin, LOW);
        delay(20);
    }
};

MG995 mg_01, mg_02;
// PB5,PB9
void mg_init() // 初始化
{
    mg_01.init(PB5);
    mg_02.init(PB9);
}
void mg_01_02_demo() // not timer interrupt
{
    static unsigned long pwm_value = 0;
    pwm_value += 1;
    pwm_value %= 180;
    mg_01.digChange(pwm_value);
    mg_02.digChange(pwm_value);
#ifdef DEBUG_PRINT
    Serial.print("pwm_value = ");
    Serial.println(pwm_value);
#endif
}
#endif