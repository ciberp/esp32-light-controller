#include <rom/rtc.h>
    #include <WiFi.h>
    #include "EEPROM.h"
    #include <ESP32WebServer.h>
    #include <Update.h>
    #include <ntp.h>
    #include <Time.h>
    // webhook
    #include <HTTPClient.h>
    #include <rom/rtc.h>
    #include "esp_wifi.h"

    // read internal ESP32 core temperature
     #ifdef __cplusplus
      extern "C" {
     #endif
    
      uint8_t temprature_sens_read();
    
    #ifdef __cplusplus
    }
    #endif
    uint8_t temprature_sens_read();


static const char CSS[] PROGMEM =
  "<style>"
  "@import url(http://fonts.googleapis.com/css?family=Roboto:400,500,700,300,100);"
  "html {"
  "font-family: \"Roboto\", helvetica, arial, sans-serif;"
  "text-rendering: optimizeLegibility;"
  "font-weight: 400 !important;"
  "font-size: 100% !important;"
  "}"
  ".f9{"
  "max-width: 95%;"
  "background: #FAFAFA;"
  "padding: 10px;"
  "margin: 10px auto;"
  "box-shadow: 1px 1px 25px rgba(0, 0, 0, 0.35);"
  "border-radius: 10px;"
  "border: 3px solid black;"
  "}"
  ".set a:link, a:visited {position: absolute;right: 2%;top: 1.5%; text-decoration: none; font-size: 44px;}"
  ".set1 a:link, a:visited {position: absolute;right: 1.8%;top: 7.2%;text-decoration: none; font-size: 50px;}"
  ".set2 a:link, a:visited {position: absolute;right: 1.8%;top: 13.5%;text-decoration: none; font-size: 56px;}"
  ".set a:link, a:visited {"
  "position: absolute;"
  "left: 96%;"
  "top: 2%;"
  "font-size: 150%;"
  "text-decoration: none;"
  "color: #305A72;"
  "}"
  ".f9 ul{"
  "padding:0;"
  "margin:0;"
  "list-style:none;"
  "}"
  ".f9 ul li{"
  "display: block;"
  "margin-bottom: 10px;"
  "min-height: 35px;"
  "}"
  ".f9 ul li  .field-style{"
  "box-sizing: border-box;"
  "-webkit-box-sizing: border-box;"
  "-moz-box-sizing: border-box;"
  "padding: 8px;"
  "outline: none;"
  "border: 1px solid #B0CFE0;"
  "}.f9 ul li  .field-style:focus{"
  "box-shadow: 0 0 5px #B0CFE0;"
  "border:1px solid #B0CFE0;"
  "}"
  ".f9 ul li .field-full{"
  "width: 100%;"
  "}"
  ".f9 ul li .field-split{"
  "font-size: 100%;"
  "}"
  ".f9 ul li input.align-left{"
  "float:left;"
  "}"
  ".f9 ul li input.align-right{"
  "float:right;"
  "}"
  ".f9 ul li textarea{"
  "width: 100%;"
  "height: 100px;"
  "}"
  ".g {"
  "-moz-box-shadow: inset 0px 1px 0px 0px #3985B1;"
  "-webkit-box-shadow: inset 0px 1px 0px 0px #3985B1;"
  "box-shadow: inset 0px 1px 0px 0px #3985B1;"
  "background-color: #216288;"
  "border: 1px solid #17445E;"
  "display: inline-block;"
  "cursor: pointer;"
  "color: #FFFFFF;"
  "padding: 8px 18px;"
  "text-decoration: none;"
  "font: 12px Arial, Helvetica, sans-serif;"
  "width: 33%;"
  "height: 8%;"
  "margin: 2px;"
  "}"
  "#rtdiv {"
  "width: 100%;"
  "}"
  ".cloud {"
  "background: #F1F1F1;"
  "border-radius: 0.4em;"
  "-moz-border-radius: 0.4em;"
  "-webkit-border-radius: 0.4em;"
  "color: #333;"
  "display: inline-block;"
  "margin-right: 5px;"
  "text-align: center;"
  "margin-bottom: 10px;"
  "border:1px solid #B0CFE0;"
  "padding: 3px 3px;"
  "min-width: 10.1%; "
  "}"
  ".f9 ul li input[type=\"submit\"], {"
  "-moz-box-shadow: inset 0px 1px 0px 0px #3985B1;"
  "-webkit-box-shadow: inset 0px 1px 0px 0px #3985B1;"
  "box-shadow: inset 0px 1px 0px 0px #3985B1;"
  "background-color: #216288;"
  "border: 1px solid #17445E;"
  "display: inline-block;"
  "cursor: pointer;"
  "color: #FFFFFF;"
  "padding: 8px 18px;"
  "text-decoration: none;"
  "font: 12px Arial, Helvetica, sans-serif;"
  "font-size: 250% !important;"
  "}"
  ".f9 ul li input[type=\"button\"]:hover,"
  ".f9 ul li input[type=\"submit\"]:hover {"
  "background: linear-gradient(to bottom, #2D77A2 5%, #337DA8 100%);"
  "background-color: #28739E;"
  "}"
  "input[type=\"text\"], select, .d {"
  "font-size: 100% !important;"
  "}"
  "table {"
  "border:1px solid #B0CFE0;"
  "color: #333;"
  "width: 100%;"
  "margin-bottom: 10px;"
  "font-size: 100% !important;"
  "}"
  "td, th {"
  "border: 1px solid transparent;"
  "height: 8%; "
  "}"
  "th {"
  "font-weight: bold;"
  "}"
  "td {"
  "background: #FAFAFA;"
  "text-align: center;"
  "}"
  "th {"
  "background: #DFDFDF;"
  "font-weight: bold;"
  "}"
  "td {  "
  "background: #FAFAFA;"
  "text-align: center;"
  "}"
  "@media screen and (max-width: 999px) {"
  "html {"
  "font-size: 250% !important;"
  "}"
  ".f9 ul li .field-split{"
  "font-size: 250% !important;"
  "}"
  ".g {"
  "font-size: 66% !important;"
  "width: 32.5% !important;"
  "height: 150px !important;"
  "}"
  ".cloud {"
  "min-width: 31.8% !important; "
  "}"
  "table {"
  "font-size: 40% !important;"
  "}"
  "}"
  "</style>";
  
//        REASON_DEFAULT_RST      = 0,   /* normal startup by power on */
//        REASON_WDT_RST         = 1,   /* hardware watch dog reset */
//        REASON_EXCEPTION_RST   = 2,   /* exception reset, GPIO status won’t change */
//        REASON_SOFT_WDT_RST      = 3,   /* software watch dog reset, GPIO status won’t change */
//        REASON_SOFT_RESTART    = 4,   /* software restart ,system_restart , GPIO status won’t change */
//        REASON_DEEP_SLEEP_AWAKE   = 5,   /* wake up from deep-sleep */HEAD
//        REASON_EXT_SYS_RST      = 6      /* external system reset */ - See more at: http://www.esp8266.com/viewtopic.php?f=32&t=13388#sthash.Pi59f7lq.dpuf 
    

    IPAddress syslogServer(0, 0, 0, 0);
    int LastSyslogMsgID[2][25]; // Pazi,da ne presezes 25, sicer potrebno povecati, crkne syslog!!!
    String Build = "20221222";
        
    //ADC_MODE(ADC_VCC);
    boolean TurnOff60SoftAP;
    unsigned long wifi_connect_previous_millis = 0;

    ESP32WebServer server(80);
    String host = "LightControl";
    int http_client_timeout = 1000;
    //const char* serverIndex = "<form method='POST' action='/update' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>";
    const char* serverIndex = "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
    "<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
        "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
    "<div id='prg'>progress: 0%</div>"
    "<script>"
    "$('form').submit(function(e){"
        "e.preventDefault();"
          "var form = $('#upload_form')[0];"
          "var data = new FormData(form);"
          " $.ajax({"
                "url: '/update',"
                "type: 'POST',"               
                "data: data,"
                "contentType: false,"                  
                "processData:false,"  
                "xhr: function() {"
                    "var xhr = new window.XMLHttpRequest();"
                    "xhr.upload.addEventListener('progress', function(evt) {"
                        "if (evt.lengthComputable) {"
                            "var per = evt.loaded / evt.total;"
                            "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
                        "}"
                   "}, false);"
                   "return xhr;"
                "},"                                
                "success:function(d, s) {"    
                    "console.log('success!')"
               "},"
                "error: function (a, b, c) {"
                "}"
              "});"
    "});"
    "</script>";

    // Tell it where to store your config data in EEPROM
    #define EEPROM_START 0    

    // Tell it where to store your config data in EEPROM
    #define EEPROM_START 0     // začetek  
    int EEPROM_SIZE;
    #define LoadDefaultNumber 2021

    // Sizes
    #define StringLenghtNormal 25
    #define StringLenghtURL 100
    #define StringLenghtIP 15
    
    typedef struct {
      // wifi
      char ssid[32];
      char password[64];
      char ap_ssid[32];
      char ap_password[64];
      char web_user[32];
      char web_pass[32];
      int wifi_outpwr;
      int defaults;
      char css_link[150];
      char syslog[16];
      char NTP_hostname[16];
      int timezone_GMT;
      int renumbering[12];
      int enabled[12];
      char descr[12][100];
      int bulb_color[12];
    } ObjectSettings;
    
    ObjectSettings SETTINGS;

    // HTML back url for root and settings
    String BackURLSystem="<meta http-equiv=\"refresh\" content=\"0;url=/system\">";
    String BackURLSettings="<meta http-equiv=\"refresh\" content=\"0;url=/settings\">";
    String BackURLRoot = "<meta http-equiv=\"refresh\" content=\"0;url=/\">";
    String BackURLDebug = "<meta http-equiv=\"refresh\" content=\"0;url=/debug\">";
    
    // time
    long days = 0;
    long hours = 0;
    long mins = 0;
    long infinite_mins = 0;
    unsigned long secs = 0;
    unsigned long previousMillisUptime = 0;
    unsigned long uptime = 0;
    unsigned long previousMillisWiFiReconect = 0; // WiFi reconnect problems
    int WifiReconnectCounter = 0;

    //#define max_pins 11
    //static const int    pin_num[]  = {16 , 5 , 4 , 0 , 2 ,14 ,12 ,13 ,15 ,3   ,1};
    //static const String pin_name[] = {"1","2","3","4","5","6","7","8","9","10","11"};

    #define max_pins 12
    static const int    pin_num[]  = {16, 17, 18, 19, 21, 22, 23, 25, 26,  27,  32,  33};
    static const String pin_name[] = {"1","2","3","4","5","6","7","8","9","10","11","12"}; 
    int                 pin_func[] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,  0 ,  0,  0  };  // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG
    int number_of_enabled = 0;
    int step_counter = 0;
    int step_counter1 = 0;
    int step_counter2 = 0;
    unsigned long previousMillisProgram = 0;

    unsigned long previousMillisRandom[max_pins];
    int RandomSpeed[max_pins];
    int ProgramSpeed;
    int ProgramFunction;
    int BeatDetectThreshold;
    int ProgramTrigger = 0;  // 0=timer, 1=http, 2=mic

    // beat detector measures    
    unsigned long previousMillisADCFreq = 0; // ADC sample freq
    unsigned long ADCSampleCounter = 0; // ADC sample counter
    int ADCSampleFreq = 0;
    int TriggerOffTimerCounter = 0; // trigger set off after x msec
    int TriggerOffTimer = 1350; // optimum default delay
    //  0 = D3 * IO 
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
    int pin_select_s0 = 14;   //D5          // pin za izbiro AD0
    int pin_select_s1 = 12;   //D6          // pin za izbiro AD0
    int pin_select_s2 = 13;   //D7          // pin za izbiro AD0
    //int pin_servo = 15;       //D8
    // D0(16) naredi reset in se non-stop resetira!!!


    // A0 microphone
    // Our Global Sample Rate, 5000hz
    #define SAMPLEPERIODUS 200
    float sample, value, envelope, beat, thresh;
    int i;
    unsigned long previousMicrosBeat;
    boolean BEAT_DETECT= false;
    boolean BEAT_DETECT_BEFORE = false;

    //#define DEBUG


    // UDP data posiljamo na syslog IP naslov
    WiFiUDP udp;


    void setup(void)
    {
      // You can open the Arduino IDE Serial Monitor window to see what the code is doing
      Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable
      EEPROM_SIZE = sizeof(ObjectSettings);
      if (!EEPROM.begin(EEPROM_SIZE))
      {
        Serial.println("failed to initialise EEPROM");
        delay(1000);
      }
      EEPROM.begin(EEPROM_SIZE);
      loadConfig();
      ConnectToWifi();
      
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
      server.on("/", handle_root);
      server.on("/trigger", Trigger_HTTP);
      server.on("/rtvalues", http_root_refresh);
      server.on("/settings", handle_Settings);
      server.on("/system", handle_System);
      //server.on("/json", http_json);
      //server.on("/cfg", http_dump_settings_json);
      server.on("/default",http_handle_load_defaults);
      server.on("/scan", http_WiFi_Scan);
      server.on("/enapsta",handle_enable_ap_sta);
      server.on("/disap",handle_disable_ap);
      server.on("/recon",ConnectToWifi); 
      server.on("/reboot",handle_reboot);
      server.on("/disconnect",handle_wifi_disconnect);
      server.on("/savee",saveConfig);
      server.on("/btn", []() {
        // func -> 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND, 5 = TRIGGER BEAT, 6 = TRIGGER HTTP
        // var  -> pin number
        // func -> 5=PROGRAM
        // var  -> program numer
        #ifdef DEBUG
          Serial.print("func=");
          Serial.print(server.arg("func").toInt());
          Serial.print(" val= ");
          Serial.println(server.arg("val").toInt());
        #endif
        if (server.arg("func").toInt() == 88) {
          ProgramFunction = server.arg("val").toInt(); // func -> // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND,  var  -> pin number
          #ifdef DEBUG
            Serial.print("ProgramFunction = ");
            Serial.println(ProgramFunction);
          #endif
        }
        if (server.arg("func").toInt() == 99) {
          ProgramTrigger = server.arg("val").toInt();  // 0=timer, 1=browser, 2=mic
          #ifdef DEBUG
          Serial.print("ProgramTrigger = ");
          Serial.println(ProgramTrigger);
          #endif
        } 
        if (server.arg("func").toInt() < 10) {
          ButtonFunction(server.arg("func").toInt(), server.arg("val").toInt());
          #ifdef DEBUG
          Serial.print("ButtonFunction = ");
          Serial.print(server.arg("func").toInt());
          Serial.print(" / ");
          Serial.println(server.arg("val").toInt());
          #endif
        }
        server.send(200, "text/html", BackURLRoot);
      });
      // slider
      server.on("/sld", []() {
        for (int n = 0; n < max_pins; n++) {
          if (String(server.arg("id")) == "rs" + String(n)) {
            RandomSpeed[n] = server.arg("val").toInt();
          }         
        }
        if (String(server.arg("id")) == "rs99") {
          ProgramSpeed = server.arg("val").toInt();
        } 
        if (String(server.arg("id")) == "bd99") {
          BeatDetectThreshold = server.arg("val").toInt();
        }
        if (String(server.arg("id")) == "by99") {
          TriggerOffTimer = map(server.arg("val").toInt(), 10, 90, 500, 5000); // 500 * 100usec = 50msec, 5000 * 100usec = 500msec
          #ifdef DEBUG
          Serial.print("TriggerOffTimer = ");
          Serial.println(TriggerOffTimer);
          #endif
        }
        server.send(200, "text/html", BackURLRoot);
      });
      server.on("/savesettings", []() {
        if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
        }
        //Shranim spremembe
        saveConfig();
        server.send(200, "text/html", BackURLSettings);
        Set_Syslog_From_Settings();
        yield();
        });
      server.on("/savesys", []() {
         #ifdef DEBUG
          Serial.println("savesys aka save cfg");
         #endif
        if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
        }
        //renumbering
        for (int n = 0; n < max_pins; n++) {
          if (server.arg("ren" + String(n)).toInt() <= 99 or server.arg("ren" + String(n)).toInt() >= 0) {
            SETTINGS.renumbering[n] = server.arg("ren" + String(n)).toInt();
          }
        }
        //enabled/disabled
         #ifdef DEBUG
          Serial.println("enable/disable");
         #endif
        for (int n = 0; n < max_pins; n++) {
          if (server.arg("dis" + String(n)).toInt() <= 1 or server.arg("dis" + String(n)).toInt() >= 0) {
            SETTINGS.enabled[n] = server.arg("dis" + String(n)).toInt();
            #ifdef DEBUG
              Serial.print("id=");
              Serial.print(n);
              Serial.print(" value=");
              Serial.println(server.arg("dis" + String(n)).toInt());
            #endif
          }
        }
        //description
        for (int n = 0; n < max_pins; n++) {
          strcpy (SETTINGS.descr[n], server.arg("dsr" + String(n)).c_str());
          SETTINGS.bulb_color[n] = server.arg("col" + String(n)).toInt();
        }
        strcpy (SETTINGS.ssid, server.arg("ssid").c_str());        
        strcpy (SETTINGS.password, server.arg("pass").c_str());
        strcpy (SETTINGS.ap_ssid, server.arg("ap_ssid").c_str());        
        strcpy (SETTINGS.ap_password, server.arg("ap_pass").c_str());
        strcpy (SETTINGS.web_user, server.arg("w_user").c_str());        
        strcpy (SETTINGS.web_pass, server.arg("w_pass").c_str());
        if (server.arg("w_pwr").toInt() < 0 or server.arg("w_pwr").toInt() > 20) {
          SETTINGS.wifi_outpwr = 5;
        } else {
          SETTINGS.wifi_outpwr = server.arg("w_pwr").toInt();
        }
        strcpy (SETTINGS.css_link, server.arg("css").c_str());
        strcpy (SETTINGS.syslog, server.arg("syslog").c_str());
        // system
        strcpy (SETTINGS.NTP_hostname, server.arg("ntp").c_str());
        SETTINGS.timezone_GMT = server.arg("gmt").toInt();
        //Shranim spremembe
        saveConfig();
        server.send(200, "text/html", BackURLSystem);
        Set_Syslog_From_Settings();
        yield();
        });
      server.begin();
      http_WiFi_Scan();
      init_pins();
    }
     
    void loop(void) {
      server.handleClient();
//      if (millis() - previousMillis > 1000) {
//        previousMillis = millis();
//        // tole se izvaja na 1sek
//      }
      // check WiFi status, if IDLE disconnect & reconnect
      if (millis() - previousMillisWiFiReconect > 120000) { // reconnect interval 2min
        previousMillisWiFiReconect = millis();
        ReconnectIfConnectionFailsLoop();
        yield();
      }
      Loop_Blink_Random_Per_Light();
      yield();
      if (ProgramTrigger == 0) {
        Loop_Light_Programs_Timer();
      }
      if (millis() - previousMillisADCFreq > 2000) { // 2sec
        previousMillisADCFreq = millis();
        ADCSampleFreq = ADCSampleCounter / 2;
        ADCSampleCounter = 0;
        yield();
      }
      
      yield();
      //if (micros() - previousMicrosBeat >= 200) {
      if (micros() - previousMicrosBeat >= 100) {
        previousMicrosBeat = micros();
        ADCSampleCounter++;
        Loop_Mic_Beat_Detection();
      }
      yield();
    }
