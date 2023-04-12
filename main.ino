
#include "shell.hpp"
//
MG995 mg_01, mg_02;

void setup()
{
  ds.select(ds_address);
  // this_mg.init(PB5); // 两个舵机引脚分别为B5,B9
  // mg995_2.init(PB9,Timer2,TIMER_CH1,50,handler3);

  serialInit();
  myOled.init();

  mg_01.init(PB5);
  mg_02.init(PB9);
  
}

void loop()
{
  command_demo();
  delay(100);
  // this_mg.digChange(0);
  mg_01_02_demo();
  Serial.println("init complet");
}
void mg_01_02_demo() // not timer interrupt
{
  static unsigned long pwm_value = 0;
  pwm_value+=10;
  pwm_value %= 2000;
  mg_01.sendPWM(pwm_value+500);
  mg_02.sendPWM(pwm_value+500);
}
void command_demo()
{
  if (serial_readCommand(serial_read_box))
  {
    Serial.print(serial_read_box);
    commandUnderstand(serial_read_box);
  }
}
void serial_demo()
{
  serial_sayHI();
  serial_exchange_message_in_2serial();
}
void MG995_demo()
{
  static unsigned long pwm_value = 0;
  pwm_value++;
  pwm_value %= 180;
  this_mg.digChange(pwm_value);
}
void ds18b20_demo()
{
  Serial.print("temp is");
  Serial.println(ds.getTempC());
}
void oled_show_demo()
{
  myOled.setPrintPosition(0, 0);
  // OLED_Set_Pos(0,0);
  myOled < "temperature:";
  myOled < ds.getTempC();
  myOled < endl;
  myOled << "fuck you!";
  delay(5000);
  myOled < endl;
  myOled < "hellow would";
  delay(5000);
  myOled.clear();
  delay(5000);
}
