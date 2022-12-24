    void saveConfig() {
      SETTINGS.defaults = LoadDefaultNumber;
      EEPROM.begin(EEPROM_SIZE);
      delay(10);
      EEPROM.put(EEPROM_START, SETTINGS);
      yield();
      EEPROM.commit();
      //server.send(200, "text/html", BackURLSystem);
      SendLog("INFO: Settings saved.");
    }
    
    void loadConfig() {
      EEPROM.begin(EEPROM_SIZE);
      delay(10);
      EEPROM.get(EEPROM_START, SETTINGS);
      yield();
      if (SETTINGS.defaults == LoadDefaultNumber) {
        Set_Syslog_From_Settings();
        SendLog("INFO: Settings loaded.");
        // Check for invalid settings  
      }
      else {
        handle_load_defaults();
      }
    }

    void http_handle_load_defaults() {
//      if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
//          return server.requestAuthentication();
//      }
      handle_load_defaults();
      server.send(200, "text/html", BackURLSystem);
    }    
      
    void handle_load_defaults() {
      // wifi
      strcpy (SETTINGS.ssid, "ssid");
      strcpy (SETTINGS.password, "password");
      strcpy (SETTINGS.ap_ssid, "light");
      strcpy (SETTINGS.ap_password, "password");
      strcpy (SETTINGS.web_user, "admin");
      strcpy (SETTINGS.web_pass, "password");
      SETTINGS.wifi_outpwr = 5;
      strcpy (SETTINGS.css_link, "");
      strcpy (SETTINGS.syslog, "0.0.0.0");
      strcpy (SETTINGS.NTP_hostname, "ntp1.arnes.si");
      SETTINGS.timezone_GMT = 1;
      //description
      for (int n = 0; n < max_pins; n++) {
        strcpy (SETTINGS.descr[n], "");
        SETTINGS.bulb_color[n] = 0;
      }
      SendLog("INFO: Defaults loaded."); 
    }
