#ifndef __STATUSBLINK_H__
#define __STATUSBLINK_H__

// #include <Arduino.h>

class StatusBlink {
  private:
    int m_onForTime = 0;
    int m_offForTime = 0;
    unsigned long m_blinkTime = 0;
    short m_blinkState = 0;
    short m_work = 0;
    short pinLedStatus = 0;

  public:
    void setupBlink(short pin);
    void loopBlink();
    void OffBlink();
    void setBlink(int on_for, int off_for);

};
#endif