
#include "mg995.hpp"
MG995 mg_01, mg_02;
//PB5,PB9
void mg_init()
{
    mg_01.init(PB5);
    mg_02.init(PB9);
}
void mg_01_02_demo() // not timer interrupt
{
  static unsigned long pwm_value = 0;
  pwm_value+=10;
  pwm_value %= 2000;
  mg_01.sendPWM(pwm_value+500);
  mg_02.sendPWM(pwm_value+500);
}