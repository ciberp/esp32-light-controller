    void init_pins() {
      for (int n = 0; n < max_pins; n++) {
        pinMode(pin_num[n], OUTPUT);
        digitalWrite(pin_num[n], LOW);
        previousMillisRandom[n] = 0;
      }
      
    }
