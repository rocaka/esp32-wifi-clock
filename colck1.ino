#include <TimeLib.h>
#include "GDIcon.h"
//#include <ESP32HTTPClient.h>
#include <ArduinoJson.h>
//#include <ESP326WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "dht0.h"
dht0 dht11;
String cityName;
String weather="null";
String temperatures;
int icw;
static const char ntpServerName[] = "ntp1.aliyun.com"; //NTP服务器，阿里云
const int timeZone = 8; 
boolean isNTPConnected = false;
WiFiUDP udp;
unsigned int localPort = 8888;
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/14, /* data=*/27, /* reset=*/U8X8_PIN_NONE);
void dht11begain(){
  uint8_t temperature, humidity;
  if(!dht11.dht11Read(&temperature, &humidity))
    {Serial.printf("The temp:%d,The humi:%d\r\n", temperature, humidity);
    u8g2.setCursor(90,20);
    u8g2.print("t:");
    u8g2.print(temperature);
    u8g2.setCursor(90,40);
    u8g2.print("h:");
    u8g2.print(humidity);
    }
  else
    Serial.println("Error");
}
void u8g2Begin()
{
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFlipMode(1);
    u8g2.setColorIndex(1);
}
void startProcess(char *title, int num)
{  
  u8g2.setCursor(14, 30);
  u8g2.print(title);
  u8g2.drawRFrame(9, 17, 110, 30, 5);
  u8g2.drawFrame(14, 36, 100, 6);
  u8g2.drawBox(14, 36, num, 6);
}

void netStartUI(char *title, int num, bool isFirst)
{
  u8g2.setFont(u8g2_font_wqy12_t_gb2312);
  u8g2.firstPage();
  do
  {
    startProcess(title, num);
  } while (u8g2.nextPage());
}


const unsigned char xing[] U8X8_PROGMEM = {
  0x00, 0x00, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x08, 0x08, 0xF8, 0x0F, 0x80, 0x00, 0x88, 0x00,
  0xF8, 0x1F, 0x84, 0x00, 0x82, 0x00, 0xF8, 0x0F, 0x80, 0x00, 0x80, 0x00, 0xFE, 0x3F, 0x00, 0x00
};  /*星*/
const unsigned char liu[] U8X8_PROGMEM = { 
  0x40, 0x00, 0x80, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0xFF, 0x7F, 0x00, 0x00, 0x00, 0x00,
  0x20, 0x02, 0x20, 0x04, 0x10, 0x08, 0x10, 0x10, 0x08, 0x10, 0x04, 0x20, 0x02, 0x20, 0x00, 0x00
};  /*六*/

void GDWifiBegin(char *SSID, char *PASSWORD)
{
  netStartUI("WiFi 连接中...", 0, true);
  Serial.println(SSID);
  Serial.println(PASSWORD);
    WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  netStartUI("WiFi 连接中...", 6, true);
  while (WiFi.status() != WL_CONNECTED)
  {
    netStartUI("WiFi 连接中...", 10, true);
    delay(500);
    Serial.print(".");
  }
  netStartUI("WiFi 连接中...", 50, true);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  netStartUI("WiFi 连接成功！", 100, true);


}

void iconweather(){
if(weather=="Cloudy")
icw=103; 
if(weather=="Sunny")
icw=100;
if(weather=="Clear")
icw=100;
if(weather=="Rain")
icw=1;
}
void icon2(int icw)
{  
  int x=50;
  int y=6;
  int z=40;   
  int m=40;
 

    switch (icw)
    {
    case 1:
        u8g2.drawXBM(x,  y, 24, 24, umbrella);
        break;
    case 2:
        u8g2.drawXBM(x,  y, 10, 10, netIcon);
        break;
    case 3:
        u8g2.drawXBM(x,  y, 8, 10, refresh);
        break;
    case 100:
        u8g2.drawXBM(x,  y, 40, 40, suny);
        break;
    case 101:
    case 103:
        u8g2.drawXBM(x,  y, z, m, cloudy);
        break;
    case 102:
        u8g2.drawXBM(x,  y, 40, 40, partlyCloudy);
        break;
    case 104:
        u8g2.drawXBM(x,  y, 40, 40, overcast);
        break;
    case 300:
    case 301:
    case 302:
    case 303:
    case 304:
        u8g2.drawXBM(x,  y, 40, 40, thunderShowe);
        break;
    case 305:
    case 309:
        u8g2.drawXBM(x,  y, 40, 40, lightRain);
        break;
    case 306:
    case 313:
        u8g2.drawXBM(x,  y, 40, 40, moderateRain);
        break;
    case 307:
    case 308:
    case 310:
    case 311:
    case 312:
        u8g2.drawXBM(x, y, 40, 40, heavyRain);
        break;
    case 400:
        u8g2.drawXBM(x,  y, 40, 40, lightSnow);
        break;
    case 401:
    case 407:
        u8g2.drawXBM(x, y, 40, 40, moderateSnow);
        break;
    case 402:
    case 403:
        u8g2.drawXBM(x,  y, 40, 40, heavySnow);
        break;
    case 404:
    case 405:
    case 406:
        u8g2.drawXBM(x,  y, 40, 40, sleet);
        break;
    default:
        u8g2.drawXBM(x,  y, 40, 40, unknown);
        break;
    }
}
void icon(int icw)
{  
  int x=108;
  int y=-8;
  int z=40;   
  int m=40;
 

    switch (icw)
    {
    case 1:
        u8g2.drawXBM(x,  y, 24, 24, umbrella);
        break;
    case 2:
        u8g2.drawXBM(x,  y, 10, 10, netIcon);
        break;
    case 3:
        u8g2.drawXBM(x,  y, 8, 10, refresh);
        break;
    case 100:
        u8g2.drawXBM(x,  y, 40, 40, suny);
        break;
    case 101:
    case 103:
        u8g2.drawXBM(x,  y, z, m, cloudy);
        break;
    case 102:
        u8g2.drawXBM(x,  y, 40, 40, partlyCloudy);
        break;
    case 104:
        u8g2.drawXBM(x,  y, 40, 40, overcast);
        break;
    case 300:
    case 301:
    case 302:
    case 303:
    case 304:
        u8g2.drawXBM(x,  y, 40, 40, thunderShowe);
        break;
    case 305:
    case 309:
        u8g2.drawXBM(x,  y, 40, 40, lightRain);
        break;
    case 306:
    case 313:
        u8g2.drawXBM(x,  y, 40, 40, moderateRain);
        break;
    case 307:
    case 308:
    case 310:
    case 311:
    case 312:
        u8g2.drawXBM(x, y, 40, 40, heavyRain);
        break;
    case 400:
        u8g2.drawXBM(x,  y, 40, 40, lightSnow);
        break;
    case 401:
    case 407:
        u8g2.drawXBM(x, y, 40, 40, moderateSnow);
        break;
    case 402:
    case 403:
        u8g2.drawXBM(x,  y, 40, 40, heavySnow);
        break;
    case 404:
    case 405:
    case 406:
        u8g2.drawXBM(x,  y, 40, 40, sleet);
        break;
    default:
        u8g2.drawXBM(x,  y, 40, 40, unknown);
        break;
    }
}
void oledClockDisplay()
{
  int years, months, days, hours, minutes, seconds, weekdays;
  years = year();
  months = month();
  days = day();
  hours = hour();
  minutes = minute();
  seconds = second();
  weekdays = weekday();
  Serial.printf("%d/%d/%d %d:%d:%d Weekday:%d\n", years, months, days, hours, minutes, seconds, weekdays);
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_unifont_t_chinese2);
  u8g2.setCursor(0, 14);
  if (isNTPConnected)
    {if(weather=="null")
      u8g2.print("Get Weather...");
      else
      {
        u8g2.print(cityName);
        u8g2.print(":");
    u8g2.print(weather);
  
    iconweather();
    icon(icw);
    }
    }
  else
    u8g2.print("无网络!"); //如果上次对时失败，则会显示无网络
  String currentTime = "";
  if (hours < 10)
    currentTime += 0;
  currentTime += hours;
  currentTime += ":";
  if (minutes < 10)
    currentTime += 0;
  currentTime += minutes;
  currentTime += ":";
  if (seconds < 10)
    currentTime += 0;
  currentTime += seconds;
  String currentDay = "";
  currentDay += years;
  currentDay += "/";
  if (months < 10)
    currentDay += 0;
  currentDay += months;
  currentDay += "/";
  if (days < 10)
    currentDay += 0;
  currentDay += days;
  Serial.printf("%d/%d/%d %d:%d:%d Weekday:%d\n", years, months, days, hours, minutes, seconds, weekdays);
  u8g2.setFont(u8g2_font_logisoso24_tr);
  u8g2.setFontMode(0);
  u8g2.setCursor(0, 44);
  u8g2.print(currentTime);
  u8g2.setCursor(0, 61);
  u8g2.setFont(u8g2_font_unifont_t_chinese2);
  u8g2.print(currentDay);
  u8g2.drawXBM(80, 48, 16, 16, xing);
  u8g2.setCursor(95, 62);
  u8g2.print("期");
  if (weekdays == 1)
    u8g2.print("日");
  else if (weekdays == 2)
    u8g2.print("一");
  else if (weekdays == 3)
    u8g2.print("二");
  else if (weekdays == 4)
    u8g2.print("三");
  else if (weekdays == 5)
    u8g2.print("四");
  else if (weekdays == 6)
    u8g2.print("五");
  else if (weekdays == 7)
    u8g2.drawXBM(111, 49, 16, 16, liu);
  u8g2.sendBuffer();
}



 const int dsize=48;  // NTP时间在消息的前48个字节里
byte packetBuffer[dsize]; // 输入输出包的缓冲区


time_t getNtpTime()
{
  IPAddress ntpServerIP;          // NTP服务器的地址

  while (udp.parsePacket() > 0);  // 丢弃以前接收的任何数据包
  Serial.println("Transmit NTP Request");
  // 从池中获取随机服务器
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500)
  {
    int size = udp.parsePacket();
    if (size >= dsize)
    {
      Serial.println("Receive NTP Response");
      isNTPConnected = true;
      udp.read(packetBuffer, dsize); // 将数据包读取到缓冲区
      unsigned long secsSince1900;
      // 将从位置40开始的四个字节转换为长整型，只取前32位整数部分
      secsSince1900 = (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      Serial.println(secsSince1900);
      Serial.println(secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR);
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-("); //无NTP响应
  isNTPConnected = false;
  return 0; //如果未得到时间则返回0
}


// 向给定地址的时间服务器发送NTP请求
void sendNTPpacket(IPAddress& address)
{
  memset(packetBuffer, 0, 48);
  packetBuffer[0] = 0b11100011; // LI, Version, Mode
  packetBuffer[1] = 0;          // Stratum, or type of clock
  packetBuffer[2] = 6;          // Polling Interval
  packetBuffer[3] = 0xEC;       // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
udp.beginPacket(address, 123); //NTP需要使用的UDP端口号为123
 udp.write(packetBuffer, dsize);
  udp.endPacket();
}



//****************************************************************
//api.seniverse.com/v3/weather/now.json?key=SE04BQLUG9Ybz8_aF&location=Tianjin&language=ch&unit=c
void httpRequest();
void parseJson(WiFiClient client);
void OLEDDispaly(String cityName, String weather, String temperatures);


const int httpPort = 80; //端口号
const char* host = "api.seniverse.com"; //服务器地址

String reqUserKey = "SE04BQLUG9Ybz8_aF";//知心天气API私钥
String reqLocation = "Tianjin";//地址
String reqUnit = "c";//摄氏度
//-------------------http请求-----------------------------//
String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
                + "&location=" + reqLocation +
                "&language=en&unit=" + reqUnit;
String httprequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Connection: close\r\n\r\n";



void httpRequest() {
  WiFiClient client;
  //1 连接服务器
  if (client.connect(host, httpPort)) {
    Serial.println("连接成功，接下来发送请求");
    client.print(httprequest);//访问API接口
    String response_status = client.readStringUntil('\n');
    Serial.println(response_status);

    if (client.find("\r\n\r\n")) {
      Serial.println("响应报文体找到，开始解析");
    }
    parseJson(client);
  }
  else {
    Serial.println("连接服务器失败");
  }
}



  void OLEDDispaly(String cityName, String weather, String temperature) {
     u8g2.clearBuffer();
     iconweather();
    icon2(icw);
  u8g2.setCursor(0, 20);
  u8g2.print(weather);
  u8g2.setCursor(0, 40);
  u8g2.print("T: ");
  u8g2.print(temperatures);
  u8g2.print("C ");
  dht11begain();
  
  u8g2.sendBuffer();
}


void parseJson(WiFiClient client) {
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, client);

  JsonObject obj1 = doc["results"][0];
 cityName = obj1["location"]["name"].as<String>();
 weather = obj1["now"]["text"].as<String>();
 temperatures = obj1["now"]["temperature"].as<String>();
 
  {
  OLEDDispaly(cityName, weather, temperatures);
  }
  Serial.println(cityName);
  Serial.println(weather);
  Serial.println(temperatures);
}




void setup() {
    char ssid[] = "opll"; // wifi ssid
    char password[] = "123789456"; // wifi password
    
    Serial.begin(115200);
    u8g2Begin();
    GDWifiBegin(ssid, password);
     Serial.println("Starting UDP");
    udp.begin(localPort);
  Serial.print("Local port: ");

  Serial.println("waiting for sync");
  setSyncProvider(getNtpTime);
  setSyncInterval(300); //每300秒同步一次时间
  isNTPConnected = true;
    

    // theFirst();

}
  int a=0;
  int b=0;
void loop() {
  // put your main code here, to run repeatedly:

  if(a>25&&a<50){
    if(b==1)
      httpRequest();
      delay(100);
  }
  a=a+1;
  b=b+1;
 if(a<25){

oledClockDisplay();
 }
 if(a>50)
 {a=0;
 }
 if(b>25)
 {
  b=0;
 }
 }
 
