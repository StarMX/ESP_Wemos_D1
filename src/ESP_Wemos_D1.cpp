#include <ArduinoJson.h>

#include "config.h"
#include "Oled.h"
#include "wifi.h"
#include "ntp.h"


#include "openapi.h"

#ifdef BATTERY
#include "battery.h"
#endif



void oled(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_blipfest_07_tr );
#ifdef NTP
  String _day = ntpValues.Year + "-" + ntpValues.Month + "-" + ntpValues.Day;
  u8g2.drawStr(u8g2.getDisplayWidth() - u8g2.getStrWidth(_day.c_str()) - 2, 6,_day.c_str());
  u8g2.drawStr(2, 6,ntpValues.Week.c_str());
#endif
  
  String _ip = WiFi.localIP().toString();
  u8g2.drawStr(u8g2.getDisplayWidth() - u8g2.getStrWidth(_ip.c_str()) - 2 ,u8g2.getDisplayHeight(),_ip.c_str());
  
#ifdef BILIBILI
  u8g2.drawStr(2, 14,("Bili fans " + bilibiliValues.Follower).c_str());
#endif

#ifdef WEATHER
  //weathe
  u8g2.drawStr(u8g2.getDisplayWidth() - u8g2.getStrWidth(weatheValues.Temperature.c_str()) - 2, u8g2.getDisplayHeight()- 8,weatheValues.Temperature.c_str());
  u8g2.drawStr(2, u8g2.getDisplayHeight() - 8,weatheValues.Text.c_str());
  u8g2.drawStr(2, u8g2.getDisplayHeight(),weatheValues.City.c_str());
#endif 

#ifdef NTP
  u8g2.setFont(u8g2_font_tenthinguys_tf);
  String _time = ntpValues.Hours + ":" + ntpValues.Minutes + ":" + ntpValues.Seconds;
  u8g2.setCursor((u8g2.getDisplayWidth() - u8g2.getStrWidth(_time.c_str()))/2,(u8g2.getDisplayHeight() +9)/2);
  u8g2.print(_time.c_str());
#endif

  u8g2.sendBuffer();
}




void setup() {
  Serial.begin(115200);
  setupOled();
  setupWifi();
  setupNTP();

#ifdef DEEPSLEEP
  Serial.println("Deep_sleep");
  disconnectWifi();
  ESP.deepSleep(loopDelaySeconds * 1e3);
#endif

}

void loop() {
  loopWifi();
#ifdef NTP
  loopNTP();
  getNTPTimeValues();
#endif
  clearOled();
  
#ifdef WEATHER
  weatherAPI();
#endif

#ifdef BILIBILI
  bilibili();
#endif
  
  oled();
#ifdef BATTERY
  BatteryLevel batteryLevel=measureBatteryLevel();
  // Serial.println(batteryLevel.level);
  // Serial.println(batteryLevel.isCharging);
  // Serial.println(batteryLevel.realVoltage);
#endif
  sendOled();
  //delay(loopDelaySeconds);
}

