
    // 20 - 200hz Single Pole Bandpass IIR Filter
    float bassFilter(float sample) {
        static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
        xv[0] = xv[1]; xv[1] = xv[2]; 
        xv[2] = (sample) / 3.f; // change here to values close to 2, to adapt for stronger or weeker sources of line level audio  
        yv[0] = yv[1]; yv[1] = yv[2]; 
        yv[2] = (xv[2] - xv[0])+ (-0.7960060012f * yv[0]) + (1.7903124146f * yv[1]);
        return yv[2];
    }

    // 10hz Single Pole Lowpass IIR Filter
    float envelopeFilter(float sample) { //10hz low pass
        static float xv[2] = {0,0}, yv[2] = {0,0};
        xv[0] = xv[1]; 
        xv[1] = sample / 50.f;
        yv[0] = yv[1]; 
        yv[1] = (xv[0] + xv[1]) + (0.9875119299f * yv[0]);
        return yv[1];
    }

    // 1.7 - 3.0hz Single Pole Bandpass IIR Filter
    float beatFilter(float sample) {
        static float xv[3] = {0,0,0}, yv[3] = {0,0,0};
        xv[0] = xv[1]; xv[1] = xv[2]; 
        xv[2] = sample / 2.7f;
        yv[0] = yv[1]; yv[1] = yv[2]; 
        yv[2] = (xv[2] - xv[0]) + (-0.7169861741f * yv[0]) + (1.4453653501f * yv[1]);
        return yv[2];
    }

    void Loop_Mic_Beat_Detection() {
        //unsigned long time = micros(); // Used to track rate

        TriggerOffTimerCounter++; // trigger counter
        if (TriggerOffTimerCounter >= TriggerOffTimer) {
            if (BEAT_DETECT) {
              Trigger_BEAT();
              BEAT_DETECT = false; 
            }
        }
      
        //for(i = 0;;++i){
        i++;
        // Read ADC and center so +-512
        analogReadResolution(11); // output do 1024
        //sample = (float)analogRead(A0) - 503.f;
        sample = (float)analogRead(A0) - 900.f;

        
        // Filter only bass component
        value = bassFilter(sample);
       
        // Take signal amplitude and filter
        if(value < 0)value=-value;
        envelope = envelopeFilter(value);
       
        // Every 200 samples (25hz) filter the envelope 
        if (i == 200) { 
          // Filter out repeating bass sounds 100 - 180bpm
          beat = beatFilter(envelope);
    
          // Threshold it based on potentiometer on AN1
          //thresh = 0.02f * (float)analogRead(1);
          int slider_thresh = map(BeatDetectThreshold, 10, 90, 50, 50000);
          thresh = 0.02f* slider_thresh;
      
          // If we are above threshold, light up LED
          //if(beat > thresh) digitalWrite(BUILTIN_LED, HIGH);
          if(beat > thresh) {
            if (!BEAT_DETECT) {
              Trigger_BEAT();
              BEAT_DETECT = true;
              TriggerOffTimerCounter = 0;
            }

            Blink_Beat_Per_Light_ON();
            //digitalWrite(BUILTIN_LED, HIGH);
          }
          //else digitalWrite(BUILTIN_LED, LOW);
          else {
//            if (BEAT_DETECT) {
//              Trigger_BEAT();
//              BEAT_DETECT = false; 
//            }
            Blink_Beat_Per_Light_OFF();
            //digitalWrite(BUILTIN_LED, LOW);
          }
          //Reset sample counter
          i = 0;
        }
        // Consume excess clock cycles, to keep at 5000 hz
        //for(unsigned long up = time+SAMPLEPERIODUS; time > 20 && time < up; time = micros());
        //}  
    }
