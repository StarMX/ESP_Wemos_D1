
#include <wifi_provisioning.h>
#include <StatusBlink.h>

StatusBlink blink;
void setupWifi()
{
  auto mac = WiFi.macAddress();
	mac.replace(":", "");
	mac.toLowerCase();
  auto instance_name = String(app_name) + "-" + mac;
  blink.setupBlink(pinLedStatus);
  
  wifi_provisioning provisioning(instance_name);
	auto const portal_timeout_milliseconds = (uint)3 * 60 * 1000;
	if (provisioning.connect() != WL_CONNECTED)
	{
		Serial.println("Provisioning...");
		provisioning.start_portal(ap_password);
		auto start = millis();
		while (WiFi.softAPgetStationNum() > 0 || millis() - start < portal_timeout_milliseconds)
			provisioning.doLoop();

		Serial.println("Provisioning timeout. Restarting...");
		ESP.restart();
	}

  blink.OffBlink();

  Serial.println();
  Serial.println(F("Status\t\tWiFi connected!"));
  Serial.print(F("IP address:\t"));
  Serial.println(WiFi.localIP());
  Serial.print("GW address:\t");
  Serial.println(WiFi.gatewayIP());
  Serial.print("MAC address:\t");
  Serial.println(WiFi.macAddress());
  Serial.print( "WiFi Subnetmask:\t" );
  Serial.println( WiFi.subnetMask() );
  Serial.print( "WiFi DNS Server\t" );
  Serial.println( WiFi.dnsIP() );

}

void loopWifi()
{
  switch (WiFi.status())
  {
  case WL_NO_SSID_AVAIL:
    Serial.println("SSID not available");
    blink.setBlink(150, 150);
    break;
  case WL_CONNECT_FAILED:
    Serial.println("Connection failed");
    blink.setBlink(200, 800);
    break;
  case WL_CONNECTION_LOST:
    Serial.println("Connection lost");
    blink.setBlink(200, 800);
    break;
  case WL_DISCONNECTED:
    Serial.println("WiFi disconnected");
    blink.setBlink(300, 300);
    break;
  case WL_CONNECTED:
    // Serial.println("WiFi connected");
    blink.OffBlink();
    break;
  }
  blink.loopBlink();
}

void disconnectWifi()
{
  WiFi.disconnect();
}
