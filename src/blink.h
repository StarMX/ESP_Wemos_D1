#include <TimedBlink.h>

TimedBlink status(pinLedStatus);

void setupBlink()
{
  pinMode(pinLedStatus, OUTPUT);
}

void loopBlink()
{
  status.blink();
}
