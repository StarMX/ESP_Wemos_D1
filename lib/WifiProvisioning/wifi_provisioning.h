#pragma once

#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266WiFi.h>

class wifi_provisioning
{
private:
	const String &instance_name_;
	const String base_url_;

	ESP8266WebServer server_;
	DNSServer dns_server_;

	void handle_unknown();
	void handle_root_get();
	void handle_root_post();

public:
	wifi_provisioning(const String &instance_name, const String& base_url = "/provisioning");
	wl_status_t connect(int attempts = 2);

	void start_portal(const String &ap_password = "");
	void doLoop();
};