
#include "shell.hpp"
//


void setup()
{
  serialInit();
  Serial.println("serial init complet");
  ds.select(ds_address);
  Serial.println("ds18b20 init complet");
  // this_mg.init(PB5); // 两个舵机引脚分别为B5,B9
  // mg995_2.init(PB9,Timer2,TIMER_CH1,50,handler3);

  // myOled.init();
  // Serial.println("oled init complet");
  mg_init();
  Serial.println("mg995 init complet");
  Serial.println("system init complet");
}

void loop()
{
  command_demo();
  delay(100);
  // this_mg.digChange(0);

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
#ifdef MG995_H
void MG995_demo()
{
  static unsigned long pwm_value = 0;
  pwm_value++;
  pwm_value %= 180;
  this_mg.digChange(pwm_value);
}
#endif
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
