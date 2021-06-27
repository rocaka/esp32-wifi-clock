

#ifndef _dht0_H_

#define _dht0_H_
#include <Arduino.h>
#define SUCC        0   //成功编码
#define INITTOUT    1   //初始化超时错误编码
#define CHECKSUM    2   //校验和错误编码

#define MAXCOUNT    100 //溢出次数，超过此值则超时
#define DHT_PIN     0   //控制DHT11的IO编号，此处为：GPIO0
#define IO_OUTMODE  pinMode(DHT_PIN, OUTPUT);//初始化GPIO0为输出
#define IO_INMODE   pinMode(DHT_PIN, INPUT);  //设置IO位输入
#define IO_SET      digitalWrite(DHT_PIN, HIGH);//IO输出高
#define IO_CLR      digitalWrite(DHT_PIN,  LOW);//IO输出低
#define READ_IO     digitalRead(DHT_PIN)        //读取IO输入

#define DELAY_MS    delay              //ms级延时函数
#define DELAY_US    delayMicroseconds  //us级延时函数

/* Exported class declaration ------------------------------------- */
class dht0{
  public:
  uint8_t dht11Read(uint8_t *temp, uint8_t *humi);
  
  uint8_t humidity_integer;
  uint8_t temperature_integer;
  uint8_t humidity_decimal;
  uint8_t temperature_decimal;
  uint8_t checksum;
};

#endif

