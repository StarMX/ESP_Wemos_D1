

#include <Arduino.h>
#include "StatusBlink.h"

void StatusBlink::setupBlink(short pin)
{
    pinLedStatus = pin;
    pinMode(pinLedStatus, OUTPUT);
}

void StatusBlink::loopBlink()
{
  if (m_work==0) {
    digitalWrite(pinLedStatus, HIGH);
    return;
  }
  if (m_blinkState==HIGH){
    if (millis() > (m_blinkTime + m_offForTime))
      m_blinkState = LOW;
  }else{
    if (millis() > (m_blinkTime + m_onForTime))
      m_blinkState = HIGH;
  }
  if (digitalRead(pinLedStatus)!=m_blinkState)
    digitalWrite(pinLedStatus, m_blinkState);
}

void StatusBlink::OffBlink(){
   m_work = 0;
}

void StatusBlink::setBlink(int on_for, int off_for)
{
  m_onForTime = on_for;
  m_offForTime = off_for;
  m_work = 1;
}
