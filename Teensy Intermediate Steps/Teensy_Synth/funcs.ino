// this funcion reads the buttons and stores their states in the global 'buttons' variable
void getButtons(void) {
  buttons = 0;
  for (j = 0; j < 8; j++) {
    digitalWrite(MUX_SEL_A, j & 1);
    digitalWrite(MUX_SEL_B, (j >> 1) & 1);
    digitalWrite(MUX_SEL_C, (j >> 2) & 1);
    delayMicroseconds(50); 
    buttons |= digitalRead(MUX_OUT_2) << j;
  }
  buttons <<= 8;
  for (j = 0; j < 8; j++) {
    digitalWrite(MUX_SEL_A, j & 1);
    digitalWrite(MUX_SEL_B, (j >> 1) & 1);
    digitalWrite(MUX_SEL_C, (j >> 2) & 1);
    delayMicroseconds(50); 
    buttons |= digitalRead(MUX_OUT_1) << j;
  }
  buttons <<= 8;
  for (j = 0; j < 8; j++) {
    digitalWrite(MUX_SEL_A, j & 1);
    digitalWrite(MUX_SEL_B, (j >> 1) & 1);
    digitalWrite(MUX_SEL_C, (j >> 2) & 1);
    delayMicroseconds(50); 
    buttons |= digitalRead(MUX_OUT_0) << j;
  }
  buttons |= 0x1000000;  // for the 25th button
  //buttons &= ~0x1000000;
 
  old25 = pin25; 
  pin25 = digitalRead(KEY_25);
}

//shift octave variable 
void octShiftFunc(void) { 
  if (octaveShift == 0) { 
    octaveDisplay = -2; 
  } else if (octaveShift == 1) { 
    octaveDisplay = -1; 
  } else if (octaveShift == 2) { 
    octaveDisplay = 0; 
  } else if (octaveShift == 3) { 
    octaveDisplay = 1; 
  } else { 
    octaveDisplay = 2; 
  } 
} 

//update screen info
void screenUpdate(void) { 
  //Draw starting mode
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(1); 
  display.println(modeString[mode]);
  display.print(pitchString[pitchMode]);
  display.print("   "); 
  display.print("Oct: "); 
  display.println(octaveDisplay); 
  display.print(knob1String[mode]); 
  display.print("   "); 
  display.println(knob2String[mode]);
  display.println(waveString[waveForm]);  
  display.display();
}

//determine if function key (key 25) has been pressed
//if pressed complete function based on key combination 
void key25Func(void) { 
  //Find out if button 25 has been pressed
  if (pin25==1 && old25 == 0) {
    //Flash LED 
    digitalWrite(PP_LED, 1);
    delay(100);
    digitalWrite(PP_LED, 0);

    //determine if a secondary function key has been pressed for pitch or waveform

    if (secondFunc == 24){ //turn pitch tuning off
      pitchMode = (pitchMode+1) %2;  
    } else if (secondFunc == 22) { //change waveform
      waveForm = (waveForm+1) %8; 
    } else if (secondFunc == 1) { 
        octaveShift = octaveShift - 1; 
        if (octaveShift < 0) { 
          octaveShift = 0 ;
        }
        octShiftFunc();
    } else if (secondFunc == 3) {
        octaveShift = octaveShift + 1; 
        if (octaveShift > 4) { 
          octaveShift = 4 ;
        }
        octShiftFunc();       
    } else { //change mode if only button 25 is pressed
      //increase counter
      mode = (mode+1) %3; //counter only goes 0,1 now! 
    }
    screenUpdate();
  }
}

uint16_t waveFormFunc(uint8_t indd, int wf) { 
    uint16_t out; 
    if (wf == 1) { 
      out = sineTable[indd]; 
    } else if (wf == 2) {
      out = triTable[indd];
    } else if (wf == 3) { 
      out = squTable[indd];
    } else if (wf == 4) { 
      out = pulseTable[indd];
    } else if (wf == 5) { 
      out = fuzzTable[indd]; 
    } else if (wf == 6) { 
      out = digi1Table[indd];
    } else if (wf == 7) { 
      out = tanTable[indd]; 
    } else {
      out = sawTable[indd]; 
    }
    return out; 
}
