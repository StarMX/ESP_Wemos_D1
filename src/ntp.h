
#ifdef NTP

#include <NTPClient.h>
#include <WiFiUdp.h>

// NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "ntp1.aliyun.com");

struct NTPValues
{
    String Year;
    String Month;
    String Day;
    String Hours;
    String Minutes;
    String Seconds;
    String Week;
};

NTPValues ntpValues;

void setupNTP()
{
    // Init NTP Client.
    timeClient.begin();
    // Set the timezone.
    timeClient.setTimeOffset(timeOffsetHours * 3600);
    // Set time update interval to 6 hours.
    //timeClient.setUpdateInterval(6 * 3600);
}

void loopNTP()
{
    timeClient.update();
}

NTPValues getNTPTimeValues()
{
    time_t rawtime = timeClient.getEpochTime();
    struct tm *ti;
    ti = localtime(&rawtime);

    uint16_t year = ti->tm_year + 1900;
    ntpValues.Year = String(year);

    uint8_t month = ti->tm_mon + 1;
    ntpValues.Month = month < 10 ? "0" + String(month) : String(month);

    uint8_t day = ti->tm_mday;
    ntpValues.Day = day < 10 ? "0" + String(day) : String(day);

    uint8_t hours = ti->tm_hour;
    ntpValues.Hours = hours < 10 ? "0" + String(hours) : String(hours);

    uint8_t minutes = ti->tm_min;
    ntpValues.Minutes = minutes < 10 ? "0" + String(minutes) : String(minutes);

    uint8_t seconds = ti->tm_sec;
    ntpValues.Seconds = seconds < 10 ? "0" + String(seconds) : String(seconds);

    int w = ((rawtime / 86400L) + 4) % 7;
    String days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    ntpValues.Week = days[w];

    return ntpValues;
}
#endif
