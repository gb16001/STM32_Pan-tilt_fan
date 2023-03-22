
#include "shell.hpp"

void setup()
{
  ds.select(ds_address);
  this_mg.init(PB5); // 两个舵机引脚分别为B5,B9
  serialInit();
  myOled.init();
}

void loop()
{
  command_demo();
  delay(100);
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
  this_mg.digChange(0);
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
