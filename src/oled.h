#include <Arduino.h>
#include <U8g2lib.h>
U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE); // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered

void setupOled()
{
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_blipfest_07_tr);
  char *_tmp = "Connecting";
  int i = (u8g2.getDisplayWidth() - u8g2.getStrWidth(_tmp)) / 2;
  u8g2.drawStr(i, (u8g2.getDisplayHeight() - 6) / 2, _tmp);
  u8g2.drawStr(i, (u8g2.getDisplayHeight() - 6) / 2 + 10, "to WiFi...");
  u8g2.drawStr(2, 46, "Powered by StarZ");

  u8g2.sendBuffer();
  // delay(1000);
}

void clearOled()
{
  u8g2.clearBuffer();
}

void sendOled()
{
  u8g2.sendBuffer();
}

void printOled(const uint8_t *font, u8g2_uint_t x, u8g2_uint_t y, const char *s)
{
  u8g2.setFont(font);
  u8g2.setCursor(x, y);
  u8g2.print(s);
}
