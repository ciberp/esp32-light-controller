
    static const char Btn[] PROGMEM =      
      "\n<script type=\"text/javascript\">\n"
      "function Btn(func,val){\n"
      "    var xmlhttp;\n"
      "    if (window.XMLHttpRequest){\n"
      "      xmlhttp = new XMLHttpRequest();\n"
      "    }\n"
      "    xmlhttp.onreadystatechange = function() {\n"
//      "    if (xmlhttp.readyState == XMLHttpRequest.DONE ) {\n"
//      "      if(xmlhttp.status == 200) {\n"
//      "        document.getElementById(\"div\").innerHTML = xmlhttp.responseText;\n"
//      "      }\n"
//      "    }\n"
      "  }\n"
      "  xmlhttp.open(\"GET\", \"btn?func=\" + func + \"&val=\" + val, true); xmlhttp.send(); };\n"
      "function Sld(id,val){\n"
      //"    console.log(id);\n"
      "    var btnstr = '';\n"
      "    if (id == 'rs99') { \n"
      "      btnstr = \"SPEED  \" + val + \"%\";"
      "      document.getElementById('rb' + id).innerHTML = btnstr;"
      "    } else if (id == 'bd99') { \n"
      "      btnstr = \"BEAT TRESHOLD \" + val + \"%\";"
      "      document.getElementById('bd' + id).innerHTML = btnstr;"
      "    } else if (id == 'by99') { \n"
      "      btnstr = \"BEAT TRANDLY \" + val + \"%\";"
      "      document.getElementById('by' + id).innerHTML = btnstr;"
      "    } else {"
      "      btnstr =\"RAND SPD \" + val + \"%\";"
      "      document.getElementById('rb' + id).innerHTML = btnstr;"
      "    }"
      //"    document.getElementById('rb' + id).innerHTML = btnstr;"
      "    var xmlhttp;\n"
      "    if (window.XMLHttpRequest){\n"
      "      xmlhttp = new XMLHttpRequest();\n"
      "    }\n"
      "    xmlhttp.onreadystatechange = function() {\n"
//      "    if (xmlhttp.readyState == XMLHttpRequest.DONE ) {\n"
//      "      if(xmlhttp.status == 200) {\n"
//      "        document.getElementById(\"div\").innerHTML = xmlhttp.responseText;\n"
//      "      }\n"
//      "    }\n"
      "  }\n"
      "  xmlhttp.open(\"GET\", \"sld?id=\" + id + \"&val=\" + val, true); xmlhttp.send(); };\n"
      "</script>\n";

    static const char bootstrap[] PROGMEM =   
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
      "<link rel=\"stylesheet\" href=\"https://pro.fontawesome.com/releases/v5.10.0/css/all.css\" integrity=\"sha384-AYmEC3Yw5cVb3ZcuHtOA93w35dYTsvhLPVnYs9eStHfGJvOvKxVfELGroGkvsg+p\" crossorigin=\"anonymous\"/>"
      "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-EVSTQN3/azprG1Anm3QDgpJLIm9Nao0Yz1ztcQTwFspd3yD65VohhpuuCOmLASjC\" crossorigin=\"anonymous\">"
      "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-MrcW6ZMFYlzcLA8Nl+NtUVF0sA7MsXsP1UyJoMp4YLEuNSfAP+JcXn/tWtIaxVXM\" crossorigin=\"anonymous\"></script>";
   
    
    
    void handle_root() {
      LogClientConnected();
      String html = "<!DOCTYPE html><html><head>";
      html += "<title>" + String(host) + "</title>";
      html +=  FPSTR(bootstrap);
      server.sendContent(html);
      yield();
      html = ""; // ne brisi   
      html += "<meta charset=\"UTF-8\">\n";
      if (SETTINGS.css_link[0] == '\0') { // preverim ali je css settings prazen, potem nalozim default css
        html += FPSTR(CSS);
      }
      else {
        html += "<link rel=\"stylesheet\" type=\"text/css\" href=\"" + String(SETTINGS.css_link) + "\">"; 
      }
      server.sendContent(html);
      yield();
      html = ""; // ne brisi
      html += "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">"; 
      html += "</head><body class=\"f9\" style=\"background-color:black;\">";
      html += "<script type=\"text/javascript\">function loadContent(){var xmlhttp;if (window.XMLHttpRequest){xmlhttp = new XMLHttpRequest();}xmlhttp.onreadystatechange = function(){\n";
      html += "if (xmlhttp.readyState == XMLHttpRequest.DONE ){if(xmlhttp.status == 200){document.getElementById(\"rtdiv\").innerHTML = xmlhttp.responseText;\n";
      server.sendContent(html);
      yield();
      html = ""; // ne brisi
      html += "setTimeout(loadContent, 50);}}}\n";
      html += "xmlhttp.open(\"GET\", \"/rtvalues\", true); xmlhttp.send(); } loadContent(); </script>\n";
      html += "<span id=\"rtdiv\">Checking ...</span>";
      html += "<br>";
      server.sendContent(html);
      yield();
      html = ""; // ne brisi  
      html += "\n";
      html += FPSTR(Btn);
      server.sendContent(html);
      yield();
      html = ""; // ne brisi
      html += "<div class=\"container\"><div class=\"row\">";
      for (int n = 0; n < max_pins; n++) {
        String Disabled = "";
        if (SETTINGS.renumbering[n] == 99) {
          Disabled = "disabled";
        }
        html += "<div class=\"btn-group btn-group-lg\" role=\"group\">";  // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND
        if (SETTINGS.enabled[n] == 1) {
          html += "<button type=\"button\" class=\"btn btn-dark\" disabled>" + String(n + 1) + "</button>";
          //html += "<button type=\"button\" class=\"btn btn-dark\" " + Disabled + " onclick=\"Btn(2," + String(n) + ")\">DISABLE</button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" " + Disabled + " onclick=\"Btn(1," + String(n) + ")\">ON</button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" " + Disabled + " onclick=\"Btn(0," + String(n) + ")\">OFF</button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" " + Disabled + " onclick=\"Btn(3," + String(n) + ")\">PROGRAM</button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" id=\"rbrs" + String(n) + "\" " + Disabled + " onclick=\"Btn(4," + String(n) + ")\">RAND SPD 0%</button>";
          html += "<button class=\"btn btn-dark\"><input id=\"rs" + String(n) + "\" type=\"range\" min=\"10\" max=\"90\" step=\"1\" value=\"10\" " + Disabled + " onchange=\"Sld(this.id,this.value);\"></button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" " + Disabled + " onclick=\"Btn(5," + String(n) + ")\">BEAT</button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" " + Disabled + " onclick=\"Btn(6," + String(n) + ")\">HTTP</button>";
          html += "<button type=\"button\" class=\"btn btn-dark\" disabled>" + String(SETTINGS.descr[n]) + "</button>";
        }
        html += "</div><br>";
      }
      html += "<div class=\"btn-group btn-group-lg\" role=\"group\">";
      html += "<button type=\"button\" class=\"btn btn-dark\" disabled> PROGRAMS: </button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,1)\">P1</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,2)\">P2</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,3)\">P3</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,4)\">P4</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,5)\">P5</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,6)\">P6</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,7)\">P7</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(88,8)\">P8</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" id=\"rbrs99\">SPEED 0%</button>";
      html += "<button class=\"btn btn-dark\"><input id=\"rs99\" type=\"range\" min=\"10\" max=\"90\" step=\"1\" value=\"10\" onchange=\"Sld(this.id,this.value);\"></button>";
      html += "</div>";
      server.sendContent(html);
      yield();
      html = ""; // ne brisi
      html += "<div class=\"btn-group btn-group-lg\" role=\"group\">"; // 0=timer, 1=http, 2=mic
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(99,2)\">BEAT</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" id=\"bdbd99\" disabled>BEAT TRESHOLD 10%</button>";
      html += "<button class=\"btn btn-dark\"><input id=\"bd99\" type=\"range\" min=\"10\" max=\"90\" step=\"1\" value=\"10\" onchange=\"Sld(this.id,this.value);\"></button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" id=\"byby99\" disabled>BEAT TRANDLY 10%</button>";
      html += "<button class=\"btn btn-dark\"><input id=\"by99\" type=\"range\" min=\"10\" max=\"90\" step=\"1\" value=\"10\" onchange=\"Sld(this.id,this.value);\"></button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(99,0)\">TIMER</button>";
      html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(99,1)\">HTTP</button>";
      server.sendContent(html);
      yield();
      html = ""; // ne brisi
      //html += "<button type=\"button\" class=\"btn btn-dark\" onclick=\"Btn(6,2)\">HTTP</button>";
      html += "</div></div></div><br>";
      html += "<br>";
      html += "<div class='set'><a href='/system'>&#128736;</a></div>";
      html += "</body></html>\n";
      server.sendContent(html); // poslje paket
      server.client().stop(); // sporocimo clientu, da smo poslali vse!
    }

    void http_root_refresh() {
      String html;
      html += "  <div class=\"container\"><div class=\"row\">";
      for (int n = 0; n < max_pins; n++) {
        String Bulb_Color = "";
        if (digitalRead(pin_num[n]) == 1) { // OFF
          //Bulb_Color = "style=\"color: red;\"";
          Bulb_Color = bulb_color_num_to_html(SETTINGS.bulb_color[n]);
        }
        String Bulb_Renumber = "";
//        if (SETTINGS.renumbering[n] == 99) {
//          Bulb_Renumber = String(n + 1) + ":NC";
//        } else {
//          Bulb_Renumber = String(n + 1) + ":" +  String(SETTINGS.renumbering[n] + 1);
//        }
        Bulb_Renumber = String(n + 1);
        if (SETTINGS.enabled[n] == 1) {
          switch (pin_func[n]) { // func -> // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND, 5 = TRIGGER BEAT, 6 = TRIGGER HTTP
            case 0:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\"></span></div><div class=\"row\"><h3 class=\"text-center\">OFF</h3></div></div>";
              break;
            case 1:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" " + Bulb_Color + "></span></div><div class=\"row\"><h3 class=\"text-center\">ON</h3></div></div>";
              break;
            case 2:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" style=\"color: grey;\"></span></div><div class=\"row\"><h3 class=\"text-center\">DIS</h3></div></div>";
              break;
            case 3:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" " + Bulb_Color + "></span></div><div class=\"row\"><h3 class=\"text-center\">PROG</h3></div></div>";
              break;
            case 4:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" " + Bulb_Color + "></span></div><div class=\"row\"><h3 class=\"text-center\">RAND</h3></div></div>";
              break;
            case 5:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" " + Bulb_Color + "></span></div><div class=\"row\"><h3 class=\"text-center\">BEAT</h3></div></div>";
              break;
            case 6:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" " + Bulb_Color + "></span></div><div class=\"row\"><h3 class=\"text-center\">HTTP</h3></div></div>";
              break;
            default:
              html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" style=\"color: grey;\"></span></div><div class=\"row\"><h3 class=\"text-center\">NC</h3></div></div>";
              break;
          }
        } else {
            html += "<div title=\"" + String(SETTINGS.descr[n]) + "\" class=\"col\"><div class=\"row\"><h3 class=\"text-center\">" + Bulb_Renumber + "</h3></div><div class=\"row\"><span class=\"far fa-lightbulb fa-5x text-center\" style=\"color: grey;\"></span></div><div class=\"row\"><h3 class=\"text-center\">NC</h3></div></div>";
        }  
      }
      html += "</div></div>";
      server.send(200, "text/html", html);
      yield();
    } 

        
