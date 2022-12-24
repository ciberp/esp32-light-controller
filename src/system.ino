    String print_reset_reason(byte reason) {
      switch ( reason) {
        case 1 : return("POWERON_RESET");          /**<1, Vbat power on reset*/
        case 3 : return("SW_RESET");               /**<3, Software reset digital core*/
        case 4 : return("OWDT_RESET");             /**<4, Legacy watch dog reset digital core*/
        case 5 : return("DEEPSLEEP_RESET");        /**<5, Deep Sleep reset digital core*/
        case 6 : return("SDIO_RESET");             /**<6, Reset by SLC module, reset digital core*/
        case 7 : return("TG0WDT_SYS_RESET");       /**<7, Timer Group0 Watch dog reset digital core*/
        case 8 : return("TG1WDT_SYS_RESET");       /**<8, Timer Group1 Watch dog reset digital core*/
        case 9 : return("RTCWDT_SYS_RESET");       /**<9, RTC Watch dog Reset digital core*/
        case 10 : return("INTRUSION_RESET");       /**<10, Instrusion tested to reset CPU*/
        case 11 : return("TGWDT_CPU_RESET");       /**<11, Time Group reset CPU*/
        case 12 : return("SW_CPU_RESET");          /**<12, Software reset CPU*/
        case 13 : return("RTCWDT_CPU_RESET");      /**<13, RTC Watch dog Reset CPU*/
        case 14 : return("EXT_CPU_RESET");         /**<14, for APP CPU, reseted by PRO CPU*/
        case 15 : return("RTCWDT_BROWN_OUT_RESET"); /**<15, Reset when the vdd voltage is not stable*/
        case 16 : return("RTCWDT_RTC_RESET");      /**<16, RTC Watch dog reset digital core and rtc module*/
        default : return("NO_MEAN");
      }
    } 
    
    void handle_System() {
        if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
        }
        LogClientConnected();
        WifiReconnectCounter = 4;
        String html = "<!DOCTYPE html><html><head>";
        html += "<title>" + String(host) + "</title>";
        html += "<meta charset=\"UTF-8\">\n";
        if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
          //html += css_string();
          html += FPSTR(CSS);
        }
        else {
          html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
        }
        html += "</head><body class=\"f9\">";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi      
        html += "<form method='get' action='savesys' id=\"form\">";
        html += "<table>"; // ne brisi
        html += "<tr><th colspan=\"5\">Lights settings</th></tr>";
        html += "<tr><th>#</th><th>enable/disable</th><th>renumber to</th><th>bulb color</th><th>description</th></tr>";
        for (int n = 0; n < max_pins; n++) {
          html += "<tr><td>" + String(n+1) + "</td><td>";
          html += light_disabled_dropdown("dis" + String(n),SETTINGS.enabled[n]);  
          html += "</td><td>";
          html += light_renumber_dropdown("ren" + String(n),SETTINGS.renumbering[n]);
          html += "</td><td>";
          html += bulb_color_dropdown("col" + String(n),SETTINGS.bulb_color[n]);  
          html += "</td><td><input name='dsr" + String(n) + "' value='" + String(SETTINGS.descr[n]) + "' maxlength='100' type='text' size='40' />";
          html += "</td></tr>";
        }
        html += "</table>";
        //Serial.println(getSize(SETTINGS.renumbering));
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>"; // ne brisi
        html += "<tr><th colspan=\"4\">System settings</th></tr>";
        html += "<tr><th>ntp hostname</th><th>timezone GMT</th><th>CSS url</th><th>syslog ip</th></tr>";
        html += "<tr><td><input name='ntp' value='" + String(SETTINGS.NTP_hostname) + "' maxlength='150' type='text' size='15'/></td><td>";
        html += "<input name='gmt' value='" + String(SETTINGS.timezone_GMT) + "' maxlength='3' type='text' size='6' /></td>";
        html += "<td><input name='css' value='" + String(SETTINGS.css_link) + "' maxlength='150' type='text' size='25' /></td><td>";
        html += "<input name='syslog' value='" + String(SETTINGS.syslog) + "' maxlength='20' type='text' size='15'/></td></tr>";
        //html += "<tr><td colspan=\"4\">" + ESP.getFullVersion() + "</td></tr>";
        html += "</table>";
        server.sendContent(html);
        delay(10);
        yield();
        html = "<table>";         
        html += "<tr><th colspan=\"3\">IP settings</th></tr>";
        html += "<tr><th>STA IPv6</th><th>AP IPv6</th><th>Client IP</th></tr>";
        html += "<tr><td>" + WiFi.localIPv6().toString() + "</td><td>" + WiFi.softAPIPv6().toString() + "</td><td>" + server.client().remoteIP().toString() + "</td></tr>";
        html += "</table>";
        html += "<table>";         
        html += "<tr><th colspan=\"4\">CPU</th></tr>";
        html += "<tr><th>Core 0 rst reason</th><th>Core 1 rst reason</th><th>CPU freq</th><th>Core temperature</th></tr>";
        html += "<tr><td>" + String(print_reset_reason(rtc_get_reset_reason(0))) + "</td><td>" + String(print_reset_reason(rtc_get_reset_reason(1))) + "</td><td>" + String(ESP.getCpuFreqMHz()) + "MHz</td>";
        if (temprature_sens_read() != 128) {
          html += "<td>" + String(((temprature_sens_read() - 32) / 1.8)) + "°C</td></tr>";
        } else {
          html += "<td>NA</td></tr>";
        }
        html += "</table>";
        html += "<table>"; // ne brisi
        html += "<tr><th colspan=\"4\">System settings - login/wifi credentials - build " + Build + "</th></tr>";
        html += "<tr><th>SSID</th><th>PSK</th><th>AP SSID</th><th>AP PSK</th></tr>";
        html += "<tr><td><input name='ssid' value='" + String(SETTINGS.ssid) + "' maxlength='32' type='text' size='15' list='ssidlist'/>" + http_print_wifi_networks() + "</td><td>";
        html += "<input name='pass' value='" + String(SETTINGS.password) + "' maxlength='32' pattern='[0-9A-Za-z]{8,30}' type='password' size='15'/></td><td>";
        html += "<input name='ap_ssid' value='" + String(SETTINGS.ap_ssid) + "' maxlength='32' type='text' size='15' /></td><td>";
        html += "<input name='ap_pass' value='" + String(SETTINGS.ap_password) + "' maxlength='32' type='password' pattern='[0-9A-Za-z]{8,30}' size='15'/></td></tr>";
        html += "</table><table>";
        html += "<tr><th>wifi output power</th><th>ssid rssi channel encryption</th><th>wifi bssid status</th><th>username</th><th>password</th></tr>";
        html += "<tr><td><input name='w_pwr' value='" + String(SETTINGS.wifi_outpwr) + "' maxlength='3' type='text' size='6' />dBm</td><td>";
        html += print_wifi_networks() + "</td><td>";
        html += WiFi.BSSIDstr()+ "<br>" + String(wl_status_to_string(WiFi.status())) + "</td><td>";
        html += "<input name='w_user' value='" + String(SETTINGS.web_user) + "' maxlength='32' type='text' size='15'/></td><td>";
        html += "<input name='w_pass' value='" + String(SETTINGS.web_pass) + "' maxlength='32' type='password' size='15'/></td></tr>";
        html += "</table></form>";
        server.sendContent(html);
        delay(10);
        yield();
        html = ""; // ne brisi
        html += "<button class=\"g\" form=\"form\" type='submit'>SAVE</button>";
        html += "<a href=\"/\"><button class=\"g\">HOME</button></a>";
        html += "<a href=\"/settings\"><button class=\"g\">SETTINGS</button></a>";
        html += "<a href=\"/disap\"><button class=\"g\">ONLY STATION MODE</button></a>";
        html += "<a href=\"/enapsta\"><button class=\"g\">AP + STATION MODE</button></a>";
        html += "<a href=\"/recon\"><button class=\"g\">RECONNECT</button></a>";
        html += "<a href=\"/default\"><button class=\"g\">LOAD DEFAULTS</button></a>";
        //html += "<a href=\"/ntpupdate\"><button class=\"g\">NTP UPDATE</button></a>"; // ntp se update-a sam
        html += "<a href=\"/crashinfo\"><button class=\"g\">VIEW CRASH INFO</button></a>";
        html += "<a href=\"/clearcrash\"><button class=\"g\">CLEAR CRASH INFO</button></a>";
        html += "</body></html>\n";
        server.sendContent(html); // poslje paket
        delay(10);
        yield();
        server.client().stop(); // sporocimo clientu, da smo poslali vse!
        yield();
    }

    void LogClientConnected() {
      SendLog("INFO: Client connected:" + String(server.client().remoteIP().toString()));  
    }

     String light_renumber_dropdown(String Name, int selected) {
      String DropDown = "<select name=\"" + String(Name) + "\"><option value=\"99\">NOT USED</option>";
      for (int n = 0; n < max_pins; n++) {
        if (n == selected) {
          DropDown += "<option value=\"" + String(n) + "\" selected>RENUMBERED TO " + String(n+1) + "</option>";
        } else {
          DropDown += "<option value=\"" + String(n) + "\">RENUMBERED TO " + String(n+1) + "</option>";  
        }
      }
      DropDown += "</select>";
      return DropDown;   
    }

     String light_disabled_dropdown(String Name, int selected) {
      String DropDown = "<select name=\"" + String(Name) + "\">";
      if (1 == selected) {
        DropDown += "<option value=\"1\" selected>ENABLED</option><option value=\"0\" >DISABLED</option>";
      } else {
        DropDown += "<option value=\"0\" selected>DISABLED</option><option value=\"1\" >ENABLED</option>";
      }
      DropDown += "</select>";
      return DropDown;   
    }

    String bulb_color_dropdown(String Name, int selected) {
      String DropDown = "<select name=\"" + String(Name) + "\">";
      switch (selected) {
        case 0:
          DropDown += "<option value=\"0\" selected>red</option><option value=\"1\">green</option><option value=\"2\">blue</option><option value=\"3\">pink</option><option value=\"4\">purple</option><option value=\"5\">yellow</option><option value=\"6\">white</option>";
          break;
        case 1:
          DropDown += "<option value=\"0\">red</option><option value=\"1\" selected>green</option><option value=\"2\">blue</option><option value=\"3\">pink</option><option value=\"4\">purple</option><option value=\"5\">yellow</option><option value=\"6\">white</option>";
          break;
        case 2:
          DropDown += "<option value=\"0\">red</option><option value=\"1\">green</option><option value=\"2\" selected>blue</option><option value=\"3\">pink</option><option value=\"4\">purple</option><option value=\"5\">yellow</option><option value=\"6\">white</option>";
          break;
        case 3:
          DropDown += "<option value=\"0\">red</option><option value=\"1\">green</option><option value=\"2\">blue</option><option value=\"3\" selected>pink</option><option value=\"4\">purple</option><option value=\"5\">yellow</option><option value=\"6\">white</option>";
          break;
        case 4:
          DropDown += "<option value=\"0\">red</option><option value=\"1\">green</option><option value=\"2\">blue</option><option value=\"3\">pink</option><option value=\"4\" selected>purple</option><option value=\"5\">yellow</option><option value=\"6\">white</option>";
          break;
        case 5:
          DropDown += "<option value=\"0\">red</option><option value=\"1\">green</option><option value=\"2\">blue</option><option value=\"3\">pink</option><option value=\"4\">purple</option><option value=\"5\" selected>yellow</option><option value=\"6\">white</option>";
          break;
        case 6:
          DropDown += "<option value=\"0\">red</option><option value=\"1\">green</option><option value=\"2\">blue</option><option value=\"3\">pink</option><option value=\"4\">purple</option><option value=\"5\">yellow</option><option value=\"6\" selected>white</option>";
          break;
        default:
          DropDown += "<option value=\"0\">red</option><option value=\"1\">green</option><option value=\"2\">blue</option><option value=\"3\">pink</option><option value=\"4\">purple</option><option value=\"5\">yellow</option><option value=\"6\">white</option>";
          break;
      }
      return DropDown;   
    }

    String bulb_color_num_to_html(int id) {
      String html;
      switch (id) {
        case 0:
          html = "style=\"color: red;\"";
          break;
        case 1:
          html = "style=\"color: green;\"";
          break;
        case 2:
          html = "style=\"color: blue;\"";
          break;
        case 3:
          html = "style=\"color: pink;\"";
          break;
        case 4:
          html = "style=\"color: purple;\"";
          break;
        case 5:
          html = "style=\"color: yellow;\"";
          break;
        case 6:
          html = "style=\"color: white;\"";
          break;
        default:
          html = "style=\"color: red;\"";
          break;
      }
      return html;      
    }
