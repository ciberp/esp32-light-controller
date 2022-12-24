
    void ReconnectIfConnectionFailsLoop () {      
      if (WifiReconnectCounter <= 0) {
        if (WiFi.status() == WL_CONNECT_FAILED or WiFi.status() == WL_CONNECTION_LOST or WiFi.status() == WL_NO_SHIELD or WiFi.status() == WL_DISCONNECTED or WiFi.status() == WL_IDLE_STATUS) {
          ConnectToWifi();
        }
      }
      else {
        WifiReconnectCounter--;
      }
    }

    void handle_wifi_disconnect() {
      SendLog("INFO: WiFi disconnected by user.");
      server.send(200, "text/html", BackURLSystem);
      delay(100);
      WiFi.disconnect();
    }
    
    void ConnectToWifi() {
      SendLog("INFO: WiFi connecting ...");
      WiFi.hostname(host);
      //WiFi.hostname(host.c_str());
      TurnOff60SoftAP = true;
      WiFi.mode(WIFI_AP_STA); // AP & client mode
      /* You can remove the password parameter if you want the AP to be open. */
      WiFi.softAP(SETTINGS.ap_ssid, SETTINGS.ap_password);
      Serial.print("AP IP address: ");
      Serial.println(WiFi.softAPIP());
      // Connect to WiFi network
      //WiFi.setOutputPower(SETTINGS.wifi_outpwr); // this sets wifi to highest power 
      WiFi.begin(SETTINGS.ssid, SETTINGS.password);
      Serial.print("\n\r \n\rConnecting to WiFi");
      // Wait for connection
      wifi_connect_previous_millis = millis();
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (millis() > wifi_connect_previous_millis + 5000) {
          Serial.println("");
          Serial.println("--- AP+STA diagnostic ---");
          WiFi.printDiag(Serial);
          Serial.println("--- AP+STA diagnostic ---");
          Serial.println("Switching to only AP mode");
          WiFi.mode(WIFI_AP); // only AP mode
          Serial.println("--- AP settings ---");
          Serial.print("ssid: ");
          Serial.println(SETTINGS.ap_ssid);
          Serial.print("psk: ");
          Serial.println(SETTINGS.ap_password);
          Serial.println("--- AP settings ---");
          break; //prekinem povezovanje in nadaljujem z AP_STA nacinom
        }
      }
      if (WiFi.status() == WL_CONNECTED) {
        WiFi.hostname(host);
        WiFi.mode(WIFI_STA); // only client mode
        Serial.println("");
        Serial.print("Connected to SSID: ");
        Serial.println(SETTINGS.ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());       
      }
    }

    void handle_enable_ap_sta() {
      SendLog("INFO: Soft AP enabled.");
      TurnOff60SoftAP = false;
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(SETTINGS.ap_ssid, SETTINGS.ap_password);
      //Serial.print("Soft AP enabled! ");
      //Serial.println(WiFi.softAPIP());
      server.send(200, "text/html", BackURLSystem);
    }
    
    void handle_disable_ap() {
      SendLog("INFO: SoftAP disabled.");
      WiFi.mode(WIFI_STA);
      //Serial.print("SoftAP disabled! ");
      //Serial.println(WiFi.localIP());
      server.send(200, "text/html", BackURLSystem);
    }   
    
    void handle_reboot() {
//      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
//          return server.requestAuthentication();
//      }
      SendLog("INFO: Restart requested."); 
      server.send(200, "text/html", BackURLSystem);
      delay(300);
      ESP.restart();
    }

    void http_WiFi_Scan() {
      // WiFi.scanNetworks(async, show_hidden) 
      // asysnc - if set to true then scanning will start in background and function will exit without waiting for result. To check for result use separate function scanComplete that is described below.
      // show_hidden - set it to true to include in scan result networks with hidden SSID.
      WiFi.scanNetworks(true, true);
      server.send(200, "text/html", BackURLSystem);
    }

    // preskenira wifi in vrne String pull down networkov
    String http_print_wifi_networks() {
      String DropDown = "<datalist id='ssidlist'>";
      int n = WiFi.scanComplete();
      String MyWiFiNetworks;
      if (n == 0)
        SendLog("INFO: Wifi scan done, no networks found");
      else
      {
        for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          DropDown += "<option value='" + String(WiFi.SSID(i)) + "'>" + String(WiFi.SSID(i)) +" ("+ String(WiFi.RSSI(i)) +")[" + String(WiFi.channel(i)) + "]" + EncryptionTypeNumberToString(WiFi.encryptionType(i)) + "</option>"; 
          MyWiFiNetworks += String(WiFi.SSID(i)) + "("+ String(WiFi.RSSI(i)) + ")[" + String(WiFi.channel(i)) + "]" + EncryptionTypeNumberToString(WiFi.encryptionType(i)) + " ";
        }
        DropDown += "</datalist>";
      }
      delay(5);
      SendLog("INFO: Wifi scan done, found: " + MyWiFiNetworks);
      return DropDown;
    }

    // preskenira wifi in vrne String pull down networkov
    String print_wifi_networks() {
      String list = "";
      int n = WiFi.scanComplete();
      if (n == 0)
        SendLog("INFO: Wifi scan done, no networks found");
      else
      {
        for (int i = 0; i < n; ++i) {
          // Print SSID and RSSI for each network found
          if (i < n) {
            list += String(WiFi.SSID(i)) + " " + String(WiFi.RSSI(i)) + "dBm ch" + String(WiFi.channel(i)) + " " + EncryptionTypeNumberToString(WiFi.encryptionType(i)) + "<br>";
          } else {
            list += String(WiFi.SSID(i)) + " " + String(WiFi.RSSI(i)) + "dBm ch" + String(WiFi.channel(i)) + " " + EncryptionTypeNumberToString(WiFi.encryptionType(i));
          }
        }
      }
      return list;
    }

    String EncryptionTypeNumberToString (int EncryptionType) {
      switch (EncryptionType) {
        case (0):
          return "Open";
        case (1):
          return "WEP";
        case (2):
          return "WPA_PSK";
        case (3):
          return "WPA2_PSK";
        case (4):
          return "WPA_WPA2_PSK";
        case (5):
          return "WPA2_ENTERPRISE";
        default:
          return "UNKOWN";
      }
    }

    const char* wl_status_to_string(wl_status_t status) {
      switch (status) {
        case WL_NO_SHIELD: return "WL_NO_SHIELD";
        case WL_IDLE_STATUS: return "WL_IDLE_STATUS";
        case WL_NO_SSID_AVAIL: return "WL_NO_SSID_AVAIL";
        case WL_SCAN_COMPLETED: return "WL_SCAN_COMPLETED";
        case WL_CONNECTED: return "WL_CONNECTED";
        case WL_CONNECT_FAILED: return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST: return "WL_CONNECTION_LOST";
        case WL_DISCONNECTED: return "WL_DISCONNECTED";
      }
    }
