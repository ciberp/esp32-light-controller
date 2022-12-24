
    void ButtonFunction(int func, int n) {
      // func -> // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND, 5 = TRIGGER BEAT, 6 = TRIGGER HTTP
      // var  -> pin number
      switch (func) {
        case 0:
          digitalWrite(pin_num[n], LOW);
          pin_func[n] = 0;
          break;
        case 1:
          digitalWrite(pin_num[n], HIGH);
          pin_func[n] = 1;
          break;
        case 2:
          digitalWrite(pin_num[n], LOW);
          pin_func[n] = 2;
          break;
        case 3:
          pin_func[n] = 3;
          break;
        case 4:
          pin_func[n] = 4;
          break;
        case 5:
          pin_func[n] = 5;
          break;
        case 6:
          pin_func[n] = 6;
          break;
        default:
          digitalWrite(pin_num[n], LOW);
          pin_func[n] = 2;
          break;
        }
    }

    void Trigger_HTTP() { // 0=timer, 1=http, 2=mic
      server.send(200, "text/html", "OK");
      Blink_HTTP_Per_Light();
      if (ProgramTrigger == 1) {
        Light_Programs_By_Pulse();
      }
    }

    void Trigger_BEAT() { // 0=timer, 1=http, 2=mic
      //if (BEAT_DETECT and !BEAT_DETECT_BEFORE) {
        if (ProgramTrigger == 2) {
          Light_Programs_By_Pulse();
        }
      //}
      //if (!BEAT_DETECT and BEAT_DETECT_BEFORE) {
      //  if (ProgramTrigger == 2) {
      //    Light_Programs_By_Pulse();
      // }
      //}
    }

    void Loop_Blink_Random_Per_Light() {
      for (int n = 0; n < max_pins; n++) {
        if (pin_func[n] == 4) { // preverim ce je vklopljen RAND
          int delay_msec = map(RandomSpeed[n], 10, 90, 10000, 50);
          if (millis() - previousMillisRandom[n] >= delay_msec) {
            previousMillisRandom[n] = millis();
            if (random(0, 2) == 0) {
              digitalWrite(pin_num[n], LOW);
            } else {
              digitalWrite(pin_num[n], HIGH);
            }
          }
        }
      }
    }

    void Blink_HTTP_Per_Light() {
      for (int n = 0; n < max_pins; n++) {
        if (pin_func[n] == 6) { // preverim ce je vklopljen HTTP
          if (digitalRead(pin_num[n]) == LOW) {
            digitalWrite(pin_num[n], HIGH);
          } else {
            digitalWrite(pin_num[n], LOW);
          }
        }
      }
    }

    void Blink_Beat_Per_Light_ON() {
      for (int n = 0; n < max_pins; n++) {
        if (pin_func[n] == 5) { // preverim ce je vklopljen BEAT
          digitalWrite(pin_num[n], HIGH);
        }
      }
    }

    void Blink_Beat_Per_Light_OFF() {
      for (int n = 0; n < max_pins; n++) {
        if (pin_func[n] == 5) { // preverim ce je vklopljen BEAT
          digitalWrite(pin_num[n], LOW);
        }
      }
    }

    void Light_Programs_By_Pulse() {
      number_of_enabled = Number_Of_Enabled();
      //int delay_msec = map(ProgramSpeed, 10, 90, 10000, 50);
      if (step_counter >= number_of_enabled) {
        step_counter = 0;
        step_counter1++;
      }
      if (step_counter1 == 2) {
        step_counter1 = 0;
      }
      switch (ProgramFunction) {
        case 1:
          Program_1();
          break;
        case 2:
          Program_2();
          break;
        case 3:
          Program_3();
          break;
        case 4:
          Program_4();
          break;
        case 5:
          Program_5();
          break;
        case 6:
          Program_6();
          break;
        case 7:
          Program_7();
          break;
        case 8:
          Program_8();
          break;
        default:
          Program_1();
          break;
      }
      step_counter++;
    }

    void Loop_Light_Programs_Timer() {
      number_of_enabled = Number_Of_Enabled();
      int delay_msec = map(ProgramSpeed, 10, 90, 10000, 50);
      if (millis() - previousMillisProgram >= delay_msec) {
        previousMillisProgram = millis();
        if (step_counter >= number_of_enabled) {
          step_counter = 0;
          step_counter1++;
        }
        if (step_counter1 == 2) {
          step_counter1 = 0;
        }
        //Program_2();
        switch (ProgramFunction) {
          case 1:
            Program_1();
            break;
          case 2:
            Program_2();
            break;
          case 3:
            Program_3();
            break;
          case 4:
            Program_4();
            break;
          case 5:
            Program_5();
            break;
          case 6:
            Program_6();
            break;
          case 7:
            Program_7();
            break;
          case 8:
            Program_8();
            break;
          default:
            Program_1();
            break;
        }
        step_counter++;
      }
    }

    void Program_8() {
      // utripanje: 101000 -> 010100 -> 001010 -> 000101 -> 000010 ...
      for (int x = 0; x < number_of_enabled; x++) {
        if (step_counter1 == 0) {
          if (x != step_counter and x != step_counter + 2) {
            Write_Low_To_Pin(x);
          } else {
            Write_High_To_Pin(x);
          }          
        } else {
          if (x != number_of_enabled - 2 - step_counter and x != number_of_enabled - 4 - step_counter) {
            Write_Low_To_Pin(x);
          } else {
            Write_High_To_Pin(x);
          }          
        }
      }
    }

    void Program_7() {
      // utripanje: 1100 -> 0110 -> 0011 -> 0001 -> 0011 -> 0110 ...
      for (int x = 0; x < number_of_enabled; x++) {
        if (step_counter1 == 0) {
          if (x != step_counter and x != step_counter + 1) {
            Write_Low_To_Pin(x);
          } else {
            Write_High_To_Pin(x);
          }          
        } else {
          if (x != number_of_enabled - 1 - step_counter and x != number_of_enabled - 2 - step_counter) {
            Write_Low_To_Pin(x);
          } else {
            Write_High_To_Pin(x);
          }          
        }
      }
    }

    void Program_6() {
      // utripanje: 100001 -> 010010 -> 001100 -> 010010 -> 100001
      for (int x = 0; x < number_of_enabled; x++) {
        if (step_counter1 == 0) {
          if (x != step_counter) {
            Write_Low_To_Pin(x);
          } else {
            Write_High_To_Pin(x);
          }          
        } else {
          if (x != number_of_enabled - 1 - step_counter) {
            Write_Low_To_Pin(x);
          } else {
            Write_High_To_Pin(x);
          }          
        }
      }
    }

    void Program_5() {
      // utripanje: 110000 -> 011000 -> 001100 -> 000110 -> 000011
      for (int x = 0; x < number_of_enabled; x++) {
        if (x != step_counter and x != step_counter + 1) {
          Write_Low_To_Pin(x);
        } else {
          Write_High_To_Pin(x);
        }
      }
    }

    void Program_4() {
      // utripanje: 100001 -> 010010 -> 001100 -> 010010 -> 100001
      for (int x = 0; x < number_of_enabled; x++) {
        if (x != step_counter and x != number_of_enabled - step_counter) {
          Write_Low_To_Pin(x);
        } else {
          Write_High_To_Pin(x);
        }
      }
    }

    void Program_3() {
      // utripanje: 0101 -> 1010 -> 0101 -> 1010
      for (int x = 0; x < number_of_enabled; x++) {
        if (step_counter % 2) {      
          if (x % 2) {
            Write_High_To_Pin(x); 
          } else {
            Write_Low_To_Pin(x);
          }
        } else {
           if (x % 2) {
            Write_Low_To_Pin(x); 
          } else {
            Write_High_To_Pin(x);
          }         
        }
      }
    }

    void Program_2() {
      // utripanje vseh luci enako 0 -> 1, 1->0
      for (int x = 0; x < number_of_enabled; x++) {            
        if (step_counter % 2) {
          Write_High_To_Pin(x);
        } else {
          Write_Low_To_Pin(x);
        }
      }
    }

    void Program_1() {
      // potovanje luci 1000 -> 0100 -> 0010 -> 0001
      for (int x = 0; x < number_of_enabled; x++) {            
        if (x == step_counter) {
          Write_High_To_Pin(x);
        } else {
          Write_Low_To_Pin(x);
        }
      }
    }

    int Number_Of_Enabled() {
      int count = 0;
      int uniq[max_pins];
      for (int n = 0; n < max_pins; n++) {
        if (SETTINGS.renumbering[n] != 99) {
          uniq[SETTINGS.renumbering[n]] = 1;
        }
      }
      for (int n = 0; n < max_pins; n++) {
        if (uniq[n] == 1) {
          count++;
        }
      }
      return count++;
    }

    void Write_High_To_Pin(int pin) {
      // write high to all renumbered pin x
      // func -> // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND
      for (int n = 0; n < max_pins; n++) {
        if (SETTINGS.renumbering[n] == pin) {
          if (pin_func[n] == 3) {
            digitalWrite(pin_num[n], HIGH);
          }
        }
      }
    }

    void Write_Low_To_Pin(int pin) {
      // write low to all renumbered pin x
      // func -> // 0 = OFF, 1 = ON, 2 = DIS, 3 = PROG, 4 = RAND
      for (int n = 0; n < max_pins; n++) {
        if (SETTINGS.renumbering[n] == pin) {
          if (pin_func[n] == 3) {
            digitalWrite(pin_num[n], LOW);
          }
        }
      }
    }



    
