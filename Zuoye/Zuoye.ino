//const int trigger1 = 3; //Trigger pin of 1st Sesnor
//
//const int echo1 = 2; //Echo pin of 1st Sesnor

const int trigger2 = 5; //Trigger pin of 2nd Sesnor

const int echo2 = 4;//Echo pin of 2nd Sesnor

const int buttonPin = 13;

#include <RTClib.h>
#include <stdio.h>
#include "U8glib.h"

#define OLED_MOSI  8
#define OLED_CLK   9
#define OLED_CS    11
#define OLED_DC    12
#define OLED_RESET 10
U8GLIB_SSD1306_128X64 u8g(OLED_MOSI, OLED_CLK, OLED_DC, OLED_CS, OLED_RESET); //设置OLED接口

#define BUZZER_PIN 6
unsigned char Alarm_Hour=20;          // 闹钟小时设置
unsigned char Alarm_Minute=20;
RTC_DS1307 RTC;

DateTime dateTime;

long time_taken;

int dist,distL,distR;

int KeyNum = 1; 
 


void setup() {

  Serial.begin(9600);
//  pinMode(trigger1, OUTPUT); 
//  pinMode(echo1, INPUT); 
  pinMode(trigger2, OUTPUT); 
  pinMode(echo2, INPUT); 
  pinMode(buttonPin,INPUT);
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);
 draw_intro();

}


/*###Function to calculate distance###*/
//计算距离
void calculate_distance(int trigger, int echo)

{

  digitalWrite(trigger, LOW);
  
  delayMicroseconds(2);
  
  digitalWrite(trigger, HIGH);
  
  delayMicroseconds(10);
  
  digitalWrite(trigger, LOW);
  
  
  time_taken = pulseIn(echo, HIGH);
  
  dist= time_taken*0.034/2;
  
  if (dist>50){
    dist = 50;
  }
}
//开机画面
void draw_intro()
{
  //  u8g.setFont(u8g_font_gdr10r); //设置字体
   u8g.firstPage();
   do{
    u8g.setFont(u8g_font_6x10);
    u8g.drawStr(0, 15, "experiment-4");
    u8g.drawStr(10, 26, "21020100050");
    u8g.drawStr(40, 35, "luojianman");
    u8g.drawStr(10, 45, "21020100049");
    u8g.drawStr(40, 55, "liuyanbin");
   }while (u8g.nextPage());
  delay(1000);
}
//屏幕显示内容
void draw()
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
  char buffer1[1];
  itoa(dateTime.year(), buffer1, 10);
  u8g.setFont(u8g_font_gdr12r);
  u8g.drawStr(30,15,"--Clock--");
  u8g.setFont(u8g_font_gdr9r); 
  u8g.drawStr(10, 30, "Date:"); 
  u8g.drawStr(50, 30, buffer1); 
  u8g.drawStr(80, 30, "/");  
  char butter2[2];
  itoa(dateTime.month(),butter2,10);
  u8g.drawStr(85, 30, butter2);
  u8g.drawStr(95, 30, "/");
  char butter3[3];
  itoa(dateTime.day(),butter3,10);
  u8g.drawStr(100, 30, butter3);
  char butter4[4];
  itoa(dateTime.hour(),butter4,10);
  char butter5[5];
  itoa(dateTime.minute(),butter5,10);
  char butter6[6];
  itoa(dateTime.second(),butter6,10);
  u8g.setFont(u8g_font_gdr9r);
  u8g.drawStr(10, 50, "Time:");
  u8g.drawStr(50, 50, butter4); 
  u8g.drawStr(65, 50, "/");  
  u8g.drawStr(75, 50, butter5);
  u8g.drawStr(90, 50, "/");
  u8g.drawStr(100, 50, butter6);
}





void loop() { //infinite loopy
 
  
  u8g.firstPage(); //  OLED首页
  do {
    dateTime = RTC.now(); // 获取现在的时间
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    draw();
    //闹钟功能，判断闹钟时间是否到达
    if (Alarm_Hour == now.hour()&& Alarm_Minute == now.minute()){                 
        //analogWrite(BUZZER_PIN, 100);
        long frequency = 300;
        tone(BUZZER_PIN,frequency);
        delay(300);
        }
  } while (u8g.nextPage());       //执行配置
  delay(100);

//  calculate_distance(trigger1,echo1);
//  distL =dist; //get distance of left sensor
  calculate_distance(trigger2,echo2);
  distR =dist; //get distance of right sensor
  
  
  //Uncomment for debudding
  
  Serial.print("R=");
  
  Serial.println(distR);
  
//  Serial.print("L=");
//  
//  Serial.println(distL);
//  
  
  
//  calculate_distance(trigger1,echo1);
//  
//  distL =dist;
  
  distL = 50;
  calculate_distance(trigger2,echo2);
  
  distR =dist;
  
  if (digitalRead(buttonPin) == LOW) {               // 如果检测到按键按下
    delay(500); KeyNum ++; delay(500);        // 执行模式 +1
    if (KeyNum>=3) {                          // 如果超过设定执行状态数量，那么就回到 1 号模式
      KeyNum = 1;
      }
    }
    //对应两种模式，视频和阅读
  switch(KeyNum){
    case 1:controlVideo(distL,distR);Serial.println("现在进入了控制视频模式");break;
    case 2:controlPaper(distL,distR);Serial.println("现在进入了控制阅读模式");break;
    default: Serial.println("程序异常");break;
  }
   
  delay(200);
}

//控制网页阅读
void controlPaper(int distL, int distR){
  if (distL>=10 && distL<=30)
  
  {
  
    delay(100); //Hand Hold Time
  
//    calculate_distance(trigger1,echo1);
//  
//    distL =dist;
  
    if (distL>=10 && distL<=30)
  
    {
  
      Serial.println("Left Locked");
  
      while(distL<=40 && distL>5)
  
      {
  
//        calculate_distance(trigger1,echo1);
//  
//        distL =dist;
  
        if (distL<30 && distL>10) //Hand pushed in 
  
        {
          Serial.println ("Pshort"); delay (300);
          }
      }
  
    }
  
  }


  
  if (distR>=10 && distR<=30)
  
  {
  
    delay(100); //Hand Hold Time
  
    calculate_distance(trigger2,echo2);
  
    distR =dist;
  
    if (distR>=10 && distR<=30)
  
    {
  
      Serial.println("Right Locked");
  
      while(distR<=40  && distR>5)
  
      {
  
        calculate_distance(trigger2,echo2);
  
        distR =dist;
  
        if (distR<15  && distR>5) //Right hand pushed in
  
        {
          Serial.println ("Pup"); delay (300);
          }
  
        if (distR>20) //Right hand pulled out
        {
          Serial.println ("Pdown"); delay (300);
          }
  
    }
  
  }
  }

}
  


//控制视频播放
void controlVideo(int distL, int distR){
    
     if ((distR <5)&&(distL < 5))
  {
    Serial.println("Play/Pause"); delay (500);
    }
  
  
//  calculate_distance(trigger1,echo1);
//  
//  distL =dist;
  
  
  calculate_distance(trigger2,echo2);
  
  distR =dist;
  
  
   
  
  
  //Control Modes
  
  //Lock Left - Control Mode
  
  if (distL>=10 && distL<=30)
  
  {
  
    delay(100); //Hand Hold Time
  
//    calculate_distance(trigger1,echo1);
//  
//    distL =dist;
  
    if (distL>=10 && distL<=30)
  
    {
  
      Serial.println("Left Locked");
  
      while(distL<=40 && distL>5)
  
      {
  
//        calculate_distance(trigger1,echo1);
//  
//        distL =dist;
  
        if (distL<15 && distL>5) //Hand pushed in 
  
        {
          Serial.println ("Vup"); delay (300);
          }
  
        if (distL>20) //Hand pulled out
  
        {
          Serial.println ("Vdown"); delay (300);
          }
  
      }
  
    }
  
  }
//  
  
  //Lock Right - Control Mode
  
  if (distR>=10 && distR<=30)
  
  {
  
    delay(100); //Hand Hold Time
  
    calculate_distance(trigger2,echo2);
  
    distR =dist;
  
    if (distR>=10 && distR<=30)
  
    {
  
      Serial.println("Right Locked");
  
      while(distR<=40  && distR>5)
  
      {
  
        calculate_distance(trigger2,echo2);
  
        distR =dist;
  
        if (distR<15  && distR>5) //Right hand pushed in
  
        {
          Serial.println ("Rewind"); delay (300);
          }
  
        if (distR>20) //Right hand pulled out
        {
          Serial.println ("Forward"); delay (300);
          }
  
    }
  
  }

  }
}


//测试是否能控制
void test(int distR){
  if (distR>=10 && distR<=30)
  
  {
  
    delay(100); //Hand Hold Time
  
    calculate_distance(trigger2,echo2);
  
    distR =dist;
  
    if (distR>=10 && distR<=30)
  
    {
  
      Serial.println("Right Locked");
  
      while(distR<=40  && distR>5)
  
      {
  
        calculate_distance(trigger2,echo2);
  
        distR =dist;
  
        if (distR<15  && distR>5) //Right hand pushed in
  
        {
          Serial.println ("Rewind"); delay (300);
          }
  
        if (distR>20) //Right hand pulled out
        {
          Serial.println ("Forward"); delay (300);
          }
  
    }
  
  }

  }
}
