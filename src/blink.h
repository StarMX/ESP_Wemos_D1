

int m_onForTime = 0;
int m_offForTime = 0;
unsigned long m_blinkTime = 0;
short m_blinkState = 0;
short m_work = 0;


void setupBlink()
{
    pinMode(pinLedStatus, OUTPUT);
}

void loopBlink()
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


void OffBlink(){
   m_work = 0;
}

void setBlink(int on_for, int off_for)
{
  m_onForTime = on_for;
  m_offForTime = off_for;
  m_work = 1;
}
