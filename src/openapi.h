

#ifdef WEATHER
#include <ESP8266HTTPClient.h>

#define TimeBetweenReadingWeather 10 * 1000 // thinkpage限制20次/分钟
unsigned long timeWeather = 0;
struct WeatheValues
{
  String Temperature;
  String Code;
  String Text;
  String City;
};
WeatheValues weatheValues;
void weatherAPI()
{

  //https://api.ip.sb/geoip

  if (weatheValues.City == "")
  {
    HTTPClient http;
    http.begin("http://ip-api.com/json");
    http.setUserAgent("esp8266");
    //http.addHeader("U-ApiKey",UKey, true);
    int httpCode = http.GET(); //http.POST("{\"value\":" + data + "}");
    // Serial.print("code:");
    // Serial.println(httpCode);
    if (httpCode == 200)
    {
      DynamicJsonDocument doc(500);
      DeserializationError error = deserializeJson(doc, http.getString());
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      JsonObject root = doc.as<JsonObject>();
      if (doc.isNull() == false)
      {
        // Serial.println(root);
        weatheValues.City = root["city"].as<String>();
      }
    }
    http.end();
  }

  if ((millis() > (timeWeather + TimeBetweenReadingWeather) || timeWeather == 0) && weatheValues.City != "")
  {
    timeWeather = millis();
    String GetUrl = "http://api.thinkpage.cn/v3/weather/now.json?key=";
    GetUrl += "gso5ckc10it60dy8"; // gso5ckc10it60dy8
    GetUrl += "&location=";
    GetUrl += weatheValues.City;
    GetUrl += "&language=en";
    HTTPClient http;
    http.begin(GetUrl);
    int httpCode = http.GET();
    if (httpCode == 200)
    { // 访问成功，取得返回参数
      DynamicJsonDocument doc(500);
      DeserializationError error = deserializeJson(doc, http.getString());
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      JsonObject root = doc.as<JsonObject>();
      if (doc.isNull() == false)
      {
        weatheValues.Temperature = root["results"][0]["now"]["temperature"].as<String>() + "°C";
        weatheValues.Code = root["results"][0]["now"]["code"].as<String>();
        weatheValues.Text = root["results"][0]["now"]["text"].as<String>();
        // Serial.println(weatheValues.Temperature);
        // Serial.println(weatheValues.Text);
        // Serial.println(weatheValues.Code);
      }
    }
    else
    { // 访问不成功，打印原因
      String payload = http.getString();
      Serial.print("context:");
      Serial.println(payload);
    }
    http.end();
  }
}
#endif

#ifdef BILIBILI
#define TimeBetweenReadingBilibili 1000
unsigned long timeBilibili = 0;
struct BilibiliValues
{
  String Following;
  String Whisper;
  String Black;
  String Follower;
};
BilibiliValues bilibiliValues;

void bilibili()
{
  if (millis() > (timeBilibili + TimeBetweenReadingBilibili) || timeBilibili == 0)
  {
    WiFiClient bilibiliApi;
    timeBilibili = millis();
    HTTPClient http;
    http.begin("http://api.bilibili.com/x/relation/stat?vmid=" + String(vmid));
    http.setUserAgent("esp8266");
    int httpCode = http.GET();
    // Serial.print("code:");
    // Serial.println(httpCode);
    if (httpCode == 200)
    {
      DynamicJsonDocument doc(500);
      DeserializationError error = deserializeJson(doc, http.getString());
      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
      JsonObject root = doc.as<JsonObject>();
      if (doc.isNull() == false)
      {
        // Serial.println(root);
        bilibiliValues.Following = root["data"]["following"].as<String>();
        bilibiliValues.Whisper = root["data"]["whisper"].as<String>();
        bilibiliValues.Black = root["data"]["black"].as<String>();
        bilibiliValues.Follower = root["data"]["follower"].as<String>();
      }
    }
    http.end();
  }
}
#endif
