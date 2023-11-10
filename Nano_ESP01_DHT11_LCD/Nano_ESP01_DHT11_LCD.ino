#include <SoftwareSerial.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial espSerial(2, 8);    // RX, TX

String strData = "";
boolean recievedFlag;

DHT dht(12, DHT11); 
long pauseDht = 20;
long curTimerDht = -pauseDht * 1000;
int curH = 0;
int curT = 0;

LiquidCrystal_I2C lcd(0x3F,16,2);  // Устанавливаем дисплей
long pauseLCD = 1; //Периодичность вывода на LCD
long curTimerLCD = - pauseLCD * 1000; // Чтобы сразу запустилось

#define test true  

//dht11,t=55.5&h=66.66
//status,status=0
 
void setup()
{
  Serial.begin(9600);
  while (!Serial) {
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  }
  Serial.print("Serial init OK\r\n");

  delay(100);
  espSerial.begin(9600);
  while (!espSerial) {
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  }
  Serial.print("ESP01 serial init OK\r\n"); 
  
  delay(100);
  dht.begin();  

  delay(100);
  lcd.init(); 
  lcd.backlight();// Включаем подсветку дисплея
  
  delay(1000);
}
 
void loop()
{
  String myStr;
  
  if (espSerial.available()) {
    myStr = espSerial.readString();
    
    Serial.print(myStr);  
  }

  // если приходят данные из компьютера - отправим их в Wi-Fi модуль
  if (Serial.available()) {
     myStr = Serial.readString();
    
    if (test){
      myStr = "millis=";
      myStr += millis();
      myStr += ", curTimerDht=";
      myStr += curTimerDht;
      myStr += ", delta=";

      long delta = millis() - curTimerDht;
      myStr += delta;
            
      Serial.println(myStr);
    }
  }

  //Отправка данных датчика
  if (millis() - curTimerDht > pauseDht * 1000) {
    curT = dht.readTemperature();
    curH = dht.readHumidity();

    if (isnan(curH) || isnan(curT)) { 
      Serial.println("Ошибка считывания счетчиков?!");
    }
    else {
      String message="dht11,t=";
  
      message +=curT;
      message +="&h=";
      message +=curH; 
      message +="%";
      
      Serial.println(message);
      
      espSerial.print(message);
      delay(500);
    }
    curTimerDht = millis();    
  }

  //LCD
  if (millis() - curTimerLCD > pauseLCD * 1000) {
    lcd.setCursor(0, 0);
    String s = "Humidity = ";
    s+=curH;
    s+="%";    
    lcd.print(s);
    
    lcd.setCursor(0, 1);
    s = "Temperature = ";
    s+=curT;   
    lcd.print(s);       
    
    curTimerLCD = millis();     
  }    
}
