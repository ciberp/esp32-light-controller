
    void SendLog(String msgtosend) {
      //Serial.print("log: ");
      //Serial.println(msgtosend);
      if (WiFi.status() == WL_CONNECTED) {
        //SendUDPSyslog(msgtosend);
        //mqtt_send_log(String(host) + msgtosend);
        yield();
      }
    }
    
    void SendUDP(String msgtosend) {
      if (WiFi.status() == WL_CONNECTED) {
        if (SETTINGS.syslog[0] != '\0') { // preverim ali syslog IP nastavljen
          //Send a packet
          udp.beginPacket(syslogServer, 1500);
          udp.print(msgtosend + "\n");
          udp.endPacket();
        }
      }
    }

    void Set_Syslog_From_Settings() {
      if (syslogServer.fromString(SETTINGS.syslog)) {
        //SendLog("INFO: Valid syslog IP Address");
        int Parts[4] = {0,0,0,0};
        int Part = 0;
        for ( int i=0; i<sizeof(SETTINGS.syslog); i++ )
        {
          char c = SETTINGS.syslog[i];
          if ( c == '.' )
          {
            Part++;
            continue;
          }
          Parts[Part] *= 10;
          Parts[Part] += c - '0';
        }
        IPAddress syslogServer( Parts[0], Parts[1], Parts[2], Parts[3] );
      }    
    }
