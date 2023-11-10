// include the library code:
#include <LiquidCrystal.h>
#include <WiFi.h>
#include <time.h>
 
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(19, 23, 18, 17, 16, 15);

const char* ssid     = "MGTS_GPON_B987";
const char* password = "D3PLNHF5";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 10800;
 
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  // Initialize Serial Monitor
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  //printLocalTime();
}
 
void loop() {
  struct tm timeinfo; 

  char strDate[11];
  char strTime[9];

  lcd.setCursor(0, 0);

  if(!getLocalTime(&timeinfo)){
      lcd.print("Error"); 
  }   
  else{
    strftime(strDate, 11, "%d.%m.%Y", &timeinfo);
    strftime(strTime, 9, "%H:%M:%S", &timeinfo);
  
    Serial.print(strDate);
    Serial.print(" ");
    Serial.println(strTime);

    lcd.print(strDate); 
    lcd.setCursor(7, 1); 
    lcd.print(strTime); 
  }

  delay(1000);  
}
