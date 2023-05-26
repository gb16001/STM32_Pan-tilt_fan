/*
 * @Author: willing willing@123.com
 * @Date: 2023-02-25 22:58:24
 * @LastEditors: willing willing@123.com
 * @LastEditTime: 2023-02-25 23:17:59
 * @FilePath: \program\Client_ESP32\Client_ESP32.ino
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include "ds18b20.hpp"
const char* ssid = "LAPTOP_GB02 1295";
const char* password = "w20010912";

const char* host = "192.168.137.1";
const uint16_t port = 8888;
// const char* streamId   = "....................";
// const char* privateKey = "....................";
int value = 0;
WiFiClient client;
void setup() {
  Serial.begin(115200);
  
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial2.begin(115200);
  Serial2.setTimeout(10);
  Serial2.setRxTimeout(10);
}


void loop() {
  // Use WiFiClient class to create TCP connections
  // 建立连接
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);
  while (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(1000);
  }
  client.setNoDelay(true);
  client.setTimeout(1);
  // This will send a string to the server
  // 发送信息
  Serial.println("sending data to server");

  while (client.connected()) {
    // Serial.println("s2,read");
    // if(Serial2.available()){
    //   Serial.println("c,send s2");
    //   client.print(Serial2.readString());
    // }

// Serial.println("c,avai");
    if (client.available()) {
      Serial.println("c,recv");
      String recv=client.readStringUntil('\n');
      if(recv.startsWith("OK")){
        Serial.println(recv);
      }
      else{
        Serial.println(recv);
        Serial2.println(recv);
      }
    }
    // delay(5000);  // execute once every 5 s, don't flood remote service
  }
  // delay(500);
  // Serial.println("closing connection");
  // client.stop();
  
}
