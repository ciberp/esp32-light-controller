    void handle_Settings() {
        if (!server.authenticate(SETTINGS.web_user, SETTINGS.web_pass)) {
          return server.requestAuthentication();
        }
        LogClientConnected();
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
        delay(5);
        yield();
        html = ""; // ne brisi      
        html += "<form method='get' action='savesettings' id=\"form\">";
        server.sendContent(html);
        html = ""; // ne brisi
        html += "</form>";
        html += "<button class=\"g\" form=\"form\" type='submit'>SAVE</button>";
        html += "<a href=\"/\"><button class=\"g\">HOME</button></a>";
        html += "<a href=\"/system\"><button class=\"g\">SYSTEM</button></a>";
        html += "</body></html>";
        server.sendContent(html); // poslje paket
        delay(5);
        yield();
        server.client().stop(); // sporocimo clientu, da smo poslali vse!
        yield();
    }
