#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <blink.h>

int connectInteration = 0;
int connectInterationMaximum = 12;

void setupWifi(const String deviceId)
{
  setupBlink();

  Serial.println(F("\n▶ WiFi"));
  Serial.print(F("Connecting to \""));
  Serial.print(wifiSsid);
  Serial.println(F("\""));
  Serial.print("\t\t");

  // WiFi.mode(WIFI_STA);
  WiFi.hostname(deviceId);
  WiFi.begin(wifiSsid, wifiPassword);
  setBlink(200, 800); //等待快连LED 闪烁
  while (WiFi.status() != WL_CONNECTED)
  {
    connectInteration++;
    connectInterationMaximum++;
    loopBlink();
    if (connectInteration == 12)
    {
      Serial.print(".\n\t\t");
      connectInteration = 0;
    }
    else
    {
      Serial.print(F("."));
    }
    if (connectInterationMaximum == 12)
    {
      byte numSsid = WiFi.scanNetworks();
      Serial.print("SSID List:\t\t");
      Serial.println(numSsid);
      connectInteration = 12;
      delay(15000);
      ESP.restart();
    }
    delay(300);
  }
  OffBlink();

  Serial.println();
  Serial.println(F("Status\t\tWiFi connected!"));
  Serial.print(F("IP address:\t"));
  Serial.println(WiFi.localIP());
  Serial.print("GW address:\t");
  Serial.println(WiFi.gatewayIP());
  Serial.print("MAC address:\t");
  Serial.println(WiFi.macAddress());
}

void loopWifi()
{
  wl_status_t wifiStatus = WiFi.status();
  switch (wifiStatus)
  {
  case WL_NO_SSID_AVAIL:
    Serial.println("SSID not available");
    setBlink(150, 150);
    break;
  case WL_CONNECT_FAILED:
    Serial.println("Connection failed");
    setBlink(200, 800);
    break;
  case WL_CONNECTION_LOST:
    Serial.println("Connection lost");
    setBlink(200, 800);
    break;
  case WL_DISCONNECTED:
    Serial.println("WiFi disconnected");
    setBlink(300, 300);
    break;
  case WL_CONNECTED:
    // Serial.println("WiFi connected");
    OffBlink();
    break;
  }
  loopBlink();
}

void disconnectWifi()
{
  WiFi.disconnect();
}
