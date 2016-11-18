
    #include <ESP8266WiFi.h>
    #include <WiFiClient.h>
    #include <ESP8266WebServer.h>
    #include <ESP8266mDNS.h>
    #include <EEPROM.h>
   
   
    boolean TurnOff60SoftAP;
    unsigned long wifi_connect_previous_millis = 0;
    
    ESP8266WebServer server(80);
    const char* host = "esp8266";
    const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
     
    
    // Tell it where to store your config data in EEPROM
    #define EEPROM_START 0    
    
    typedef struct {
      // wifi
      char ssid[32];
      char password[64];
      char ap_ssid[32];
      char ap_password[64];
    } ObjectSettings;
    
    ObjectSettings SETTINGS;
       
    String webString="";
    String webMetaRefreshShowdata="<meta http-equiv=\"refresh\" content=\"0;url=/\">";
    String webMetaRefreshSettings="<meta http-equiv=\"refresh\" content=\"0;url=/settings\">";
    
    // Generally, you should use "unsigned long" for variables that hold time
    unsigned long previousMillis = 0;        // will store last temp was read
    const long interval = 2000;              // interval at which to read sensor
    //  0 = D3 *
    //  1 = D10
    //  2 = D4 *
    //  3 = D9 *
    //  4 = D2 * DS18D20
    //  5 = D1 *
    //  6 = (SPI) - flash
    //  7 = (SPI) - flash
    //  8 = (SPI) - flash
    //  9 = S2    - flash
    // 10 = S3    - flash
    // 11 = (SPI) - flash
    // 12 = D6 *
    // 13 = D7 *
    // 14 = D5 *
    // 15 = D8 * SERVO
    // 16 = D0 *
    
    int pin1 = 16;   //D0
    int pin2 = 5;    //D1
    int pin3 = 4;    //D2
    int pin4 = 0;    //D3
    int pin5 = 2;    //D4
    int pin6 = 14;   //D5
    int pin7 = 12;   //D6
    int pin8 = 13;   //D7
    int pin9 = 15;   //D8
    int pin10  = 3;  //D9
    int pin11  = 1;  //D10
    int pin12  = 10; //SD3
    int pin13  = 9;  //SD2

    int randspeed = 15;
    int loop_delay = 1000;
    boolean pin1rand, pin2rand, pin3rand, pin4rand, pin5rand, pin6rand, pin7rand, pin8rand, pin9rand, pin10rand, pin11rand, pin12rand, pin13rand = false;
    
    void setup(void)
    {
      // You can open the Arduino IDE Serial Monitor window to see what the code is doing
      Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
      EEPROM.begin(512);
      loadConfig();
      ConnectToWifi();
      
      //MDNS.begin(host);
      
      server.on("/espupdate", HTTP_GET, [](){
        //server.sendHeader("Connection", "close");
        //server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/html", serverIndex);
      });
      server.on("/update", HTTP_POST, [](){
        server.sendHeader("Connection", "close");
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.send(200, "text/plain", (Update.hasError())?"FAIL":"OK");
        ESP.restart();
      },[](){
        HTTPUpload& upload = server.upload();
        if(upload.status == UPLOAD_FILE_START){
          Serial.setDebugOutput(true);
          WiFiUDP::stopAll();
          Serial.printf("Update: %s\n", upload.filename.c_str());
          uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
          if(!Update.begin(maxSketchSpace)){//start with max available size
            Update.printError(Serial);
          }
        } else if(upload.status == UPLOAD_FILE_WRITE){
          if(Update.write(upload.buf, upload.currentSize) != upload.currentSize){
            Update.printError(Serial);
          }
        } else if(upload.status == UPLOAD_FILE_END){
          if(Update.end(true)){ //true to set the size to the current progress
            Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
          } else {
            Update.printError(Serial);
          }
          Serial.setDebugOutput(false);
        }
        yield();
      });
      server.begin();
      MDNS.addService("http", "tcp", 80);
      Serial.printf("Ready! Open http://%s.local in your browser\n", host);

      
      server.on("/", handle_root);
      server.on("/info",esp_info);
      server.on("/json",handle_json);
      server.on("/default",handle_load_defaults);
      
      server.on("/pin1on",handle_pin1_on);
      server.on("/pin1off",handle_pin1_off);
      server.on("/pin1rand",handle_pin1_rand);
      
      server.on("/pin2on",handle_pin2_on);
      server.on("/pin2off",handle_pin2_off);
      server.on("/pin2rand",handle_pin2_rand);

      server.on("/pin3on",handle_pin3_on);
      server.on("/pin3off",handle_pin3_off);
      server.on("/pin3rand",handle_pin3_rand);

      server.on("/pin4on",handle_pin4_on);
      server.on("/pin4off",handle_pin4_off);
      server.on("/pin4rand",handle_pin4_rand);

      server.on("/pin5on",handle_pin5_on);
      server.on("/pin5off",handle_pin5_off);
      server.on("/pin5rand",handle_pin5_rand);

      server.on("/pin6on",handle_pin6_on);
      server.on("/pin6off",handle_pin6_off);
      server.on("/pin6rand",handle_pin6_rand);

      server.on("/pin7on",handle_pin7_on);
      server.on("/pin7off",handle_pin7_off);
      server.on("/pin7rand",handle_pin7_rand);

      server.on("/pin8on",handle_pin8_on);
      server.on("/pin8off",handle_pin8_off);
      server.on("/pin8rand",handle_pin8_rand);

      server.on("/pin9on",handle_pin9_on);
      server.on("/pin9off",handle_pin9_off);
      server.on("/pin9rand",handle_pin9_rand);

      server.on("/pin10on",handle_pin10_on);
      server.on("/pin10off",handle_pin10_off);
      server.on("/pin10rand",handle_pin10_rand);

      server.on("/pin11on",handle_pin11_on);
      server.on("/pin11off",handle_pin11_off);
      server.on("/pin11rand",handle_pin11_rand);

      server.on("/pin12on",handle_pin12_on);
      server.on("/pin12off",handle_pin12_off);
      server.on("/pin12rand",handle_pin12_rand);

      server.on("/pin13on",handle_pin13_on);
      server.on("/pin13off",handle_pin13_off);
      server.on("/pin13rand",handle_pin13_rand);

      server.on("/randup",handle_rand_speed_up);
      server.on("/randdown",handle_rand_speed_down);

      server.on("/settings",handle_Settings);
      
      server.on("/reboot",handle_reboot);
      //server.on("/savee",saveConfig);
      server.on("/save", []() {
        //ssid = server.arg("ssid");
        strcpy (SETTINGS.ssid, server.arg("ssid").c_str());        
        //password = server.arg("pwd");
        strcpy (SETTINGS.password, server.arg("pass").c_str());
        strcpy (SETTINGS.ap_ssid, server.arg("ap_ssid").c_str());        
        strcpy (SETTINGS.ap_password, server.arg("ap_pass").c_str());
        webString=webMetaRefreshSettings;
        server.send(200, "text/html", webString);
        saveConfig();
        Serial.print("ssid: ");
        Serial.println(SETTINGS.ssid);
        Serial.print("pass: ");
        Serial.println(SETTINGS.password);
        Serial.print("ap_ssid: ");
        Serial.println(SETTINGS.ap_ssid);
        Serial.print("ap_pass: ");
        Serial.println(SETTINGS.ap_password); 
        });
      server.on("/enapsta",handle_enable_ap_sta);
      server.on("/disap",handle_disable_ap);
      server.on("/recon",ConnectToWifi);    
      server.begin();
      //Serial.println("HTTP server started");
     
      pinMode(pin1, OUTPUT);
      pinMode(pin2, OUTPUT);
      pinMode(pin3, OUTPUT);
      pinMode(pin4, OUTPUT);
      pinMode(pin5, OUTPUT);
      pinMode(pin6, OUTPUT);
      pinMode(pin7, OUTPUT);
      pinMode(pin8, OUTPUT);
      pinMode(pin9, OUTPUT);
      pinMode(pin10, OUTPUT);
      pinMode(pin11, OUTPUT);
    }
     
    void loop(void) {
      // MAIN LOOP //
      server.handleClient();  
      if (millis() - previousMillis > loop_delay) {
        previousMillis = millis();
        rand_generator();
      }
    }

    void loadConfig() {
      EEPROM.get(EEPROM_START, SETTINGS);
      Serial.println("Settings loaded!");
    }
    
    void saveConfig() {
      EEPROM.put(EEPROM_START, SETTINGS);
      EEPROM.commit();
      webString=webMetaRefreshSettings;
      server.send(200, "text/html", webString);
      Serial.println("Settings saved!");
    }
      
    void handle_load_defaults() {
      // wifi
      strcpy (SETTINGS.ssid, "ssid-to-connect-to");
      strcpy (SETTINGS.password, "password");
      strcpy (SETTINGS.ap_ssid, "LIGHT-CONTROLLER");
      strcpy (SETTINGS.ap_password, "password");
      webString=webMetaRefreshSettings;
      server.send(200, "text/html", webString);
      Serial.println("Defaults loaded!");    
    }

    void handle_enable_ap_sta() {
      TurnOff60SoftAP = false;
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(SETTINGS.ap_ssid, SETTINGS.ap_password);
      Serial.print("Soft AP enabled! ");
      Serial.println(WiFi.softAPIP());
      webString=webMetaRefreshSettings;
      server.send(200, "text/html", webString);
    }
    
    void handle_disable_ap() {
      WiFi.mode(WIFI_STA);
      Serial.print("SoftAP disabled! ");
      Serial.println(WiFi.localIP());
      webString=webMetaRefreshSettings;
      server.send(200, "text/html", webString);
    }   
    
    void ConnectToWifi() {
      TurnOff60SoftAP = true;
      WiFi.mode(WIFI_AP_STA);
      /* You can remove the password parameter if you want the AP to be open. */
      WiFi.softAP(SETTINGS.ap_ssid, SETTINGS.ap_password);
      Serial.print("AP IP address: ");
      Serial.println(WiFi.softAPIP());
      // Connect to WiFi network
      WiFi.begin(SETTINGS.ssid, SETTINGS.password);
      Serial.print("\n\r \n\rConnecting to WiFi");
      // Wait for connection
      wifi_connect_previous_millis = millis();
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        if (millis() > wifi_connect_previous_millis + 30000) {
          break; //prekinem povezovanje in nadaljujem z AP_STA nacinom
        }
      }
      if (WiFi.status() == WL_CONNECTED) {
        WiFi.mode(WIFI_STA);
        Serial.println("");
        Serial.print("Connected to SSID: ");
        Serial.println(SETTINGS.ssid);
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
      }
    }

    void handle_json() {
       webString="{\"WiFi\": \"" + String(WiFi.RSSI()) + "\", \"Uptime\": \"" + String((unsigned long)millis()) + "\", \"FreeHeap\": \"" + String(ESP.getFreeHeap()) + "\" }";
       server.send(200, "text/plain", webString);
    }
 
    void handle_reboot() {
        ESP.restart();
        webString=webMetaRefreshSettings;
        server.send(200, "text/html", webString);
    }
   
    void esp_info() {
      webString = "ESP.getBootMode() " + String(ESP.getBootMode()) + "\nESP.getSdkVersion() " + ESP.getSdkVersion() + "\nESP.getBootVersion() " + ESP.getBootVersion() + "\nESP.getChipId() " +
      ESP.getChipId() + "\nESP.getFlashChipSize() " + ESP.getFlashChipSize() + "\nESP.getFlashChipRealSize() " + ESP.getFlashChipRealSize() + "\nESP.getFlashChipSizeByChipId() " +
      ESP.getFlashChipSizeByChipId() + "\nESP.getFlashChipId() " + ESP.getFlashChipId();
      server.send(200, "text/html", webString);
      Serial.println(webString);
    }
        
    void handle_root() {
        long days = 0;
        long hours = 0;
        long mins = 0;
        unsigned long secs = 0;
        secs = millis()/1000; //convert milliseconds to seconds
        mins=secs/60; //convert seconds to minutes
        hours=mins/60; //convert minutes to hours
        days=hours/24; //convert hours to days
        secs=secs-(mins*60); //subtract the coverted seconds to minutes in order to display 59 secs max
        mins=mins-(hours*60); //subtract the coverted minutes to hours in order to display 59 minutes max
        hours=hours-(days*24); //subtract the coverted hours to days in order to display 23 hours max
        webString="<!DOCTYPE html><HTML><meta charset=\"UTF-8\">\n"
        //"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />"
        "<title>   { Light }   </title>"
        "<style>\n"
        "table, th, td {\n"
        "font-family: DroidSans;\n"
        "font-size: 160%;\n"
        "text-align: center;\n"
        "}</style>\n"
        //"<script>setInterval(function(){location.reload();},5000);</script>\n"
        "<table style=\"width:100%\">\n"
        "<tr><td>#</td><td>TURN ON</td><td>TURN OFF</td><td>RAND</td><td>S</td><td>R</td></tr>"
        "<tr><td>1</td><td><a href=\"/pin1on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin1off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin1rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin1)) + "</td><td>" + String((boolean)pin1rand) + "</td></tr>"
        "<tr><td>2</td><td><a href=\"/pin2on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin2off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin2rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin2)) + "</td><td>" + String((boolean)pin2rand) + "</td></tr>"
        "<tr><td>3</td><td><a href=\"/pin3on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin3off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin3rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin3)) + "</td><td>" + String((boolean)pin3rand) + "</td></tr>"
        "<tr><td>4</td><td><a href=\"/pin4on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin4off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin4rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin4)) + "</td><td>" + String((boolean)pin4rand) + "</td></tr>"
        "<tr><td>5</td><td><a href=\"/pin5on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin5off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin5rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin5)) + "</td><td>" + String((boolean)pin5rand) + "</td></tr>"
        "<tr><td>6</td><td><a href=\"/pin6on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin6off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin6rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin6)) + "</td><td>" + String((boolean)pin6rand) + "</td></tr>"
        "<tr><td>7</td><td><a href=\"/pin7on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin8off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin7rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin7)) + "</td><td>" + String((boolean)pin7rand) + "</td></tr>"
        "<tr><td>8</td><td><a href=\"/pin8on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin8off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td><a href=\"/pin8rand\"><button style=\"width:100%\" class=\"g\"><h1>RAND</h1></button></a></td><td>" + String((boolean)digitalRead(pin8)) + "</td><td>" + String((boolean)pin8rand) + "</td></tr>"
        "<tr><td>9</td><td><a href=\"/pin9on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin9off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td>N/A</td><td>" + String((boolean)digitalRead(pin9)) + "</td><td>" + String((int)pin9rand) + "</td></tr>"
        "<tr><td>10</td><td><a href=\"/pin10on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin10off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td>N/A</td><td>" + String((boolean)digitalRead(pin10)) + "</td><td>" + String((int)pin10rand) + "</td></tr>"
        "<tr><td>11</td><td><a href=\"/pin11on\"><button style=\"width:100%\" class=\"g\"><h1>ON</h1></button></a></td><td><a href=\"/pin11off\"><button style=\"width:100%\" class=\"g\"><h1>OFF</h1></button></a></td><td>N/A</td><td>" + String((boolean)digitalRead(pin11)) + "</td><td>" + String((int)pin11rand) + "</td></tr>"
        "</table><br>\n"
        "<table style=\"width:100%\">\n"
        "<tr><td>RAND SPEED</td><td><a href=\"/randup\"><button style=\"width:250px\" class=\"g\"><h1>+</h1></button></a></td><td><a href=\"/randdown\"><button style=\"width:250px\" class=\"g\"><h1>-</h1></button></a></td><td>" + String((int)randspeed) + "</td>"
        "</table><br>\n"
        "<a href=\"/settings\"><button class=\"gb\" style=\"width:100%\"><h1>SETTINGS</h1></button></a>\n"
        "</div>\n";
        "</BODY></HTML>\n";
        server.send(200, "text/html", webString);
        delay(100);
    }

    void handle_Settings() {   
        webString="<!DOCTYPE html><HTML><meta charset=\"UTF-8\">\n"
        "<title>   { Nastavitve }   </title>\n"
        "<div>"
        "<style>table,td{font-size:160%;text-align:center;}\n"
        "input[type=text], input[type=password] {  display: block; width: 100%; height: 100%;  line-height: 160px; font-size: 80px;  border: 1px solid #bbb;}\n"
        "</style><table align=\"center\" border=\"0\">\n"
        "<style scoped>.g{width:100px;height:100%}</style>\n";
        server.sendContent(webString); // poslje paket
        webString = "<tr><td><form method='get' action='save'>SSID:</td><td><input name='ssid' value='" + String(SETTINGS.ssid) + "' maxlength='32' type='text'></td></tr>"
        "<tr><td>PSK:</td><td><input name='pass' value='" + String(SETTINGS.password) + "' maxlength='32' pattern='[0-9A-Za-z]{8,30}' type='password' ></td></tr>"
        "<tr><td><label>AP SSID:</label></td><td><input name='ap_ssid' value='" + String(SETTINGS.ap_ssid) + "' maxlength='32' type='text'></td></tr>"
        "<tr><td><label>AP PSK:</label></td><td><input name='ap_pass' value='" + String(SETTINGS.ap_password) + "' maxlength='32' type='text' pattern='[0-9A-Za-z]{8,30}'></td></tr>"
        "<tr><td colspan=\"2\"><button style=\"width:100%;height:150px\" type='submit' style='width:100%'><h1>SAVE</h1></button></form></td></tr>"
        "<tr><td colspan=\"2\"><a href=\"/default\"><button  style=\"width:100%;height:150px\"><h1>LOAD DEFAULTS</h1></button></a></td></tr>"
        "</table></td></tr></table>"
        "</div><div>" //potrebno zaradi gumbov!!!
        "<style scoped>.g{width:100%;height:300px}h1{font-size:400%;color:blue;}</style>"
        "<a href=\"/\"><button class=\"g\"><h1>BACK</h1></button></a>"
        "<a href=\"/enapsta\"><button class=\"g\" type='submit'><h1>AP+STA</h1></button></a>"
        "<a href=\"/disap\"><button class=\"g\"><h1>ONLY STA</h1></button></a>"
        "<a href=\"/recon\"><button class=\"g\"><h1>RECONNECT</h1></button></a>"
        //"<a href=\"/reboot\"><button class=\"g\"><h1>RESTART</h1></button></a>"
        "<a href=\"/espupdate\"><button class=\"g\"><h1>OTA UPDATE</h1></button></a>"
        "</div>"
        "</BODY></HTML>\n";
        server.send(200, "text/html", webString);
    }

 
    void handle_pin1_off() {
      pin1rand = false;
      digitalWrite(pin1, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    
    void handle_pin1_on() {
      pin1rand = false;
      digitalWrite(pin1, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    
    void handle_pin1_rand() {
      pin1rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }    

    
    void handle_pin2_off() {
      pin2rand = false;
      digitalWrite(pin2, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    
    void handle_pin2_on() {
      pin2rand = false;
      digitalWrite(pin2, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    
    void handle_pin2_rand() {
      pin2rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    
    void handle_pin3_off() {
      pin3rand = false;
      digitalWrite(pin3, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin3_on() {
      pin3rand = false;
      digitalWrite(pin3, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin3_rand() {
      pin3rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }
    
    void handle_pin4_off() {
      pin4rand = false;
      digitalWrite(pin4, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin4_on() {
      pin4rand = false;
      digitalWrite(pin4, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin4_rand() {
      pin4rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }
    
    void handle_pin5_off() {
      pin5rand = false;
      digitalWrite(pin5, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin5_on() {
      pin5rand = false;
      digitalWrite(pin5, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin5_rand() {
      pin5rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin6_off() {
      pin6rand = false;
      digitalWrite(pin6, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin6_on() {
      pin6rand = false;
      digitalWrite(pin6, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin6_rand() {
      pin6rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin7_off() {
      pin7rand = false;
      digitalWrite(pin7, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin7_on() {
      pin7rand = false;
      digitalWrite(pin7, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin7_rand() {
      pin7rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin8_off() {
      pin8rand = false;
      digitalWrite(pin8, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin8_on() {
      pin8rand = false;
      digitalWrite(pin8, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin8_rand() {
      pin8rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin9_off() {
      pin9rand = false;
      digitalWrite(pin9, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin9_on() {
      pin4rand = false;
      digitalWrite(pin9, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin9_rand() {
      pin9rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin10_off() {
      pin10rand = false;
      digitalWrite(pin10, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin10_on() {
      pin10rand = false;
      digitalWrite(pin10, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin10_rand() {
      pin10rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin11_off() {
      pin11rand = false;
      digitalWrite(pin11, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin11_on() {
      pin11rand = false;
      digitalWrite(pin11, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin11_rand() {
      pin11rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

     void handle_pin12_off() {
      pin12rand = false;
      digitalWrite(pin12, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin12_on() {
      pin12rand = false;
      digitalWrite(pin12, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin12_rand() {
      pin12rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_pin13_off() {
      pin13rand = false;
      digitalWrite(pin13, LOW);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }   
    void handle_pin13_on() {
      pin13rand = false;
      digitalWrite(pin13, HIGH);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_pin13_rand() {
      pin13rand = true;
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void handle_rand_speed_up() {
      if (randspeed < 25) {
        randspeed++;
        if (loop_delay <= 1000) {
          loop_delay = loop_delay - 100;
        }
        else if ((loop_delay > 1000) && (loop_delay <= 10000)){
          loop_delay = loop_delay - 500;
        }
        else{
          loop_delay = loop_delay - 2000;
        }
      }
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }  
    void handle_rand_speed_down() {
      if (randspeed > 0) {
        randspeed--;
        if (loop_delay <= 1000) {
          loop_delay = loop_delay + 100;
        }
        else if ((loop_delay > 1000) && (loop_delay <= 10000)){
          loop_delay = loop_delay + 500;
        }
        else{
          loop_delay = loop_delay + 2000;
        }
      }
      Serial.print("loop_delay:");
      Serial.println(loop_delay);
      webString=webMetaRefreshShowdata;
      server.send(200, "text/html", webString);  
    }

    void rand_generator() {
      int var = random(255);
      if (pin1rand) {
        digitalWrite(pin1,  HIGH && (var & B00000001));
      }
      if (pin2rand) {
        digitalWrite(pin2,  HIGH && (var & B00000010));
      }
      if (pin3rand) {
        digitalWrite(pin3,  HIGH && (var & B00000100));
      }
      if (pin4rand) {
        digitalWrite(pin4,  HIGH && (var & B00001000));
      }
      if (pin5rand) {
        digitalWrite(pin5,  HIGH && (var & B00010000));
      }
      if (pin6rand) {
        digitalWrite(pin6,  HIGH && (var & B00100000));
      }
      if (pin7rand) {
        digitalWrite(pin7,  HIGH && (var & B01000000));
      }
      if (pin8rand) {
        digitalWrite(pin8,  HIGH && (var & B10000000));
      }
    }

