#include "wifi_provisioning.h"

wifi_provisioning::wifi_provisioning(const String &instance_name, const String& base_url /* = "/provisioning" */)
    : instance_name_(instance_name), base_url_(base_url)
{
    server_.on(base_url_, HTTP_GET, std::bind(&wifi_provisioning::handle_root_get, this));
    server_.on(base_url_, HTTP_POST, std::bind(&wifi_provisioning::handle_root_post, this));
    server_.onNotFound(std::bind(&wifi_provisioning::handle_unknown, this));
}

wl_status_t wifi_provisioning::connect(int attempts /* = 2 */)
{
    WiFi.mode(WIFI_STA);
    wl_status_t connect_result = WiFi.begin();
    while (connect_result != WL_CONNECTED && --attempts > 0)
    {
        Serial.printf("Connecting... Retries left: %d", attempts);
        connect_result = (wl_status_t)WiFi.waitForConnectResult();
    }

    Serial.printf("Connection result: %d", connect_result);
    return connect_result;
}

void wifi_provisioning::start_portal(const String &ap_password /*= "" */)
{
    Serial.println("Starting portal");
    WiFi.setAutoConnect(false);

 //   WiFi.mode(WIFI_AP);
    WiFi.softAP(instance_name_.c_str(), ap_password.length() ? ap_password.c_str() : nullptr);
    auto ip_address = WiFi.softAPIP();
    Serial.printf("AP IP address: %s", ip_address.toString().c_str());

    // Start DNS Server to respond and redirect to the address of the AP
    dns_server_.setErrorReplyCode(DNSReplyCode::NoError);
    dns_server_.start(53, "*", ip_address);

    server_.begin();

    // Scan available networks (async)
    WiFi.scanNetworks(true);
}

void wifi_provisioning::doLoop()
{
    dns_server_.processNextRequest();
    server_.handleClient();
}

void wifi_provisioning::handle_unknown()
{
    Serial.println("handle_unknown");
    auto root = "http://" + WiFi.softAPIP().toString() + base_url_;
    server_.sendHeader("Location", root);
    // See Other
    server_.send(302);
}

void wifi_provisioning::handle_root_get()
{
    Serial.println("handle_root_get");
    String ssid_options = "<option value=\"\">select wifi</option>";
    auto ssid_items = WiFi.scanComplete();
    Serial.printf("ssid Items: %d", ssid_items);
    for (auto index = 0; index < ssid_items; ++index)
    {
        auto ssid = WiFi.SSID(index);
        Serial.printf("Adding ssid: %s", ssid.c_str());
        ssid_options += "<option value=\"" + ssid + "\">" + ssid + "</option>";
    }

    String html(
        "<!DOCTYPE html>"
        "<html lang=\"en\">"
        "<head>"
        "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\" />"
        "<meta http-equiv=\"Pragma\" content=\"no-cache\">"
        "<title>" +
        instance_name_ +
        "</title>"
        "<style>"
        "label {width:80px;display:block;float:left;clear:left;}"
        "select,input,button {display:block;border-radius:5px;outline:none;}"
        "</style>"
        "</head>"
        "<body>"
        "<h1>" +
        instance_name_ +
        "</h1>"
        "<hr>"
        "<form method=\"POST\">"
        "<label for=\"ssid\">SSID:</label>"
        "<select name=\"ssid\">" +
        ssid_options +
        "</select>"
        "<br />"
        "<label for=\"password\">Password:</label>"
        "<input name=\"password\" type=\"password\">"
        "<br />"
        "<input type=\"submit\" value=\"Submit\">"
        "</form>"
        "</body>"
        "</html>");

    server_.send(200, "text/html", html);
}

void wifi_provisioning::handle_root_post()
{
    Serial.println("handle_root_post");
    String ssid, password;
    if (!server_.hasArg("ssid") || !server_.hasArg("password") || (ssid = server_.arg("ssid")) == nullptr || (password = server_.arg("password")) == nullptr)
    {
        server_.send(400, "text/plain", "400: Invalid Request");
        return;
    }

    Serial.printf("SSID: %s, password: %s", ssid.c_str(), password.c_str());

    WiFi.softAPdisconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    WiFi.setAutoConnect(true);

    auto connection_result = (wl_status_t)WiFi.waitForConnectResult();
    Serial.printf("Connection result: %d", connection_result);
    ESP.restart();
}