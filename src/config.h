
#define pinLedStatus BUILTIN_LED
#define wifiSsid "MX2017"
#define wifiPassword "1234567"
const uint32_t deviceId = ESP.getChipId();

#define NTP
#ifdef NTP
const int timeOffsetHours = +8; //ntp timezone
#endif

#define WEATHER

#define BILIBILI
#ifdef BILIBILI
const uint32_t vmid = 102396463;
#endif

#define BATTERY

//#define DEEPSLEEP
#ifdef DEEPSLEEP
const int loopDelaySeconds = 5 * 60;
#endif
