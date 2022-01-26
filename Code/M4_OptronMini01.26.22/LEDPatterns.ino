#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void querySerial()
{
  // listen for serial:
  // set color arrays
  while (Serial.available() > 0) {

    char anal = Serial.read(); 
    switch(anal)
    {
      //************************************************************
      // Serial message is: BGi (R, G, B, LED_start, LED_length)
      case'B':  // string should start with B
      anal = Serial.read(); //Read second char and analyyze     
      if (anal == 'G')  // then next should be G
      { 
      if (Serial.read() == 'i')  // then next should be i
      { 
        // Enable Pattern in refresh:
        //patternEnArray[0] = true;  // BGi
        //patternEnArray[1] = false; // HSv
     // Enable Pattern in refresh: 
        patternEnArray[0] = Serial.parseInt();  // Enable BGi Array   
     // parse RGB
        BGiParams[0] = Serial.parseInt();     // then an ASCII number for red
        BGiParams[1] = Serial.parseInt();   // then an ASCII number for green
        BGiParams[2] = Serial.parseInt();    // then an ASCII number for blue
        BGiParams[3] = Serial.parseInt();   // then an ASCII number for start
        BGiParams[4] = Serial.parseInt();    // then an ASCII number for length

        clipLen(&BGiParams[3],&BGiParams[4]);
     
      }}

      if(anal == 'O') 
      { 
      if (Serial.read() == 'W')  // then next should be i
      { 
/*
        // Enable Pattern in refresh:
        patternEnArray[0] = false; // BGi
        patternEnArray[1] = false; // HSv
        patternEnArray[2] = true;  // Rainbow
        patternEnArray[3] = false; // Spit
*/
        // Enable Pattern in refresh: 
        patternEnArray[2] = Serial.parseInt();  // Rainbow 
        BOWParams[0] = Serial.parseInt();  // hue delta (length of hue transitions)
        BOWParams[1] = Serial.parseInt();   // Start LED
        BOWParams[2] = Serial.parseInt();   // Len
        BOWParams[3] = Serial.parseInt();   // Fade ammount, 0 strong to 255 weak

        clipLen(&BOWParams[1],&BOWParams[2]);
        
      }}

      //************************************************************
      // Bell Pad
      // Serial message is: BEL on/off h, v, start, len, dens
      // Notice: Missing saturation as it is assumed will have fixed value
    
      if(anal == 'E')  
      { 
      if (Serial.read() == 'L')  
      { 
        patternEnArray[10] = Serial.parseInt(); // then an ASCII number for 1/0 on off

        // fill overlay cells with dim color blobs at each ZX position
        for( byte i = 0; i < 8; i++)
        { 
         uint8_t index = ZXSensorPos[i];
         int colorindex = map(index, 0, NUM_LEDS, 0, 240); // map total number of LEDs to full gradient value
         
          // Have color volume of 10 no matter what so I can see where to hit
          bellOverlay[index] += ColorFromPalette(bellPalette, colorindex, 10);
          bellOverlay[index+1] += ColorFromPalette(bellPalette, colorindex+1, 10);
        }
        
      } break; }
      
// //************************************************************
      // Bell Pad sprite trigger
      // Serial message is: BIL Number Amplitude
      // Notice: Missing saturation as it is assumed confetti will have fixed value
      if(anal == 'I')  
      { 
      if (Serial.read() == 'L')  
      { 
     
     // parse kit sprites vars
        uint8_t bellSpriteNum = Serial.parseInt(); // then an ASCII number for sprite number
        uint8_t bellSpriteAmp = Serial.parseInt();
        uint8_t len = 4; // length of lit up sprite

        uint8_t index = ZXSensorPos[bellSpriteNum]; // calculate index at drumSprite

        int colorindex = map(index, 0, NUM_LEDS, 0, 240); // map total number of LEDs to full gradient value
        // Give sprite color boost
        //drumKitOverlay[index] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
        //drumKitOverlay[index+1] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
      // Set colors of pixels from outside toward center, increasing brightness in 32 steps    
      bellOverlay[index] = ColorFromPalette(bellPalette, colorindex, bellSpriteAmp);
      
      for(int j = 0; j < len; j++)
      {
        byte mypoint = (len-j);
        
          if((index-mypoint)>=0)  // ensure we're not writing to index off the strip
          {
            colorindex = map(index-mypoint, 0, NUM_LEDS, 0, 255); // map total number of LEDs to full gradient value
            // Give sprite color boost
            bellOverlay[index-mypoint] = ColorFromPalette(bellPalette, colorindex, bellSpriteAmp);
          } 
          if((index+mypoint)<(NUM_LEDS-1)) // ensure we're not writing to index off the strip
          {
            colorindex = map(index+mypoint, 0, NUM_LEDS, 0, 255); // map total number of LEDs to full gradient value
            // Give sprite color boost
            bellOverlay[index+mypoint] = ColorFromPalette(bellPalette, colorindex, bellSpriteAmp);
          }
          
        }
  
      }}

      break;

      //************************************************************
      // Serial message is: HSV (h, s, v, start, len)
      case'H':     
      if (Serial.read() == 'S')  
      { 
      if (Serial.read() == 'v')  
      { 
        // Enable Pattern in refresh:
//        patternEnArray[0] = false; // BGi
//        patternEnArray[1] = true;  // HSv
//        patternEnArray[2] = false; // Rainbow
//        patternEnArray[3] = false; // Spit
     
     // Enable Pattern in refresh: 
        patternEnArray[1] = Serial.parseInt();  // Enable BGi Array     
     // parse HSV
        HSVParams[0] = Serial.parseInt();     // then an ASCII number for Hue
        HSVParams[1] = Serial.parseInt();   // then an ASCII number for Saturation
        HSVParams[2] = Serial.parseInt();    // then an ASCII number for Brightness
        HSVParams[3] = Serial.parseInt();   // then an ASCII number for start
        HSVParams[4] = Serial.parseInt();    // then an ASCII number for len

        clipLen(&HSVParams[3],&HSVParams[4]);
        
      }}
      break;

      //************************************************************
      // Serial message is: PPT (h, s, v, start, len, fade_rate)
      // To spit colors was inspired by Willy Wonka, everlasting gobstopper that let kids spit colors
      case'P':     
      if (Serial.read() == 'P')  
      { 
      if (Serial.read() == 'T')  
      { 
        // Enable Pattern in refresh:
  //      patternEnArray[0] = false; // BGi
  //      patternEnArray[1] = false;  // HSv
  //      patternEnArray[2] = false; // Rainbow
        patternEnArray[3] = true; // Spit
        
     // parse HSV
        PPTParams[0] = Serial.parseInt();     // then an ASCII number for Hue
        PPTParams[1] = Serial.parseInt();   // then an ASCII number for Saturation
        PPTParams[2] = Serial.parseInt();    // then an ASCII number for Brightness
        PPTParams[3] = Serial.parseInt();   // then an ASCII number for start
        PPTParams[4] = Serial.parseInt();    // then an ASCII number for len
        PPTParams[5] = Serial.parseInt();    // then an ASCII number for fade_rate - [0 - 255] [none - instant] how fast spit disolves 

        clipLen(&PPTParams[3],&PPTParams[4]);

        //  Set new color segment in spit overlay layer immediately
        fill_solid( &(ppitOverlay[PPTParams[3]]), PPTParams[4], CHSV( PPTParams[0], PPTParams[1], PPTParams[2]) );
        
      }}
      break;
      
      //************************************************************
      // Serial message is: SPT (h, s, v, start, len, fade_rate)
      // To spit colors was inspired by Willy Wonka, everlasting gobstopper that let kids spit colors
      case'S':     
      if (Serial.read() == 'P')  
      { 
      if (Serial.read() == 'T')  
      { 
        // Enable Pattern in refresh:
//        patternEnArray[0] = false; // BGi
//        patternEnArray[1] = false;  // HSv
//        patternEnArray[2] = false; // Rainbow
        patternEnArray[4] = true; // Spit
        
     // parse HSV
        SPTParams[0] = Serial.parseInt();     // then an ASCII number for Hue
        SPTParams[1] = Serial.parseInt();   // then an ASCII number for Saturation
        SPTParams[2] = Serial.parseInt();    // then an ASCII number for Brightness
        SPTParams[3] = Serial.parseInt();   // then an ASCII number for start
        SPTParams[4] = Serial.parseInt();    // then an ASCII number for len
        SPTParams[5] = Serial.parseInt();    // then an ASCII number for fade_rate - [0 - 255] [none - instant] how fast spit disolves 

        clipLen(&SPTParams[3],&SPTParams[4]);

        //  Set new color segment in spit overlay layer immediately
        fill_solid( &(spitOverlay[SPTParams[3]]), SPTParams[4], CHSV( SPTParams[0], SPTParams[1], SPTParams[2]) );
        
      }}
      break;

      //************************************************************
      // Serial message is: TCK on/off (density, direction, hue, saturation, volume)
      case'T':  // string should start with B
      anal = Serial.read(); //Read second char and analyyze     
      if (anal == 'C')  // then next should be G
      { 
      if (Serial.read() == 'K')  // then next should be i
      {
        // Enable Pattern in refresh: 
        patternEnArray[5] = Serial.parseInt();  // On/Off TCK

      for(int i = 0; i < 5; i++) // There are 5 parameters passing in here
      {
        TCKParams[i] = Serial.parseInt();  
      }

      // Clear Tick array
      fill_solid( &(tickOverlay[0]), NUM_LEDS, CRGB( 0, 0, 0) );
      uint8_t numTicks = (int)(NUM_LEDS/TCKParams[0]); // calculate the modulo value to draw sprites based on density value
      
      // calculate number of tick steps to take in the following animation
      // based on tick sprite index
      TCKParams[5] = numTicks;
      
      for(int i = 0; i < NUM_LEDS; i++) // Fill tick array with sprites
      {
        if((i%numTicks) < 1) // Draw sprite spaced evenly based on density value
        {
          tickOverlay[i] = CHSV( TCKParams[2], TCKParams[3], TCKParams[4]);
          //tickOverlay[i] = CHSV( 0, 127, 127);

        }
      }
        
      }}
      break;

      //************************************************************
      // Serial message is: DKT (on/off)
      // Add glitter with 0 - 100 percent density
      case'D':     
      if (Serial.read() == 'K') 
      { 
      anal = Serial.read(); //Read second char and analyyze      
      if (anal == 'T') 
      { 
        
        patternEnArray[6] = Serial.parseInt(); // then an ASCII number for 1/0 on off

        // fill overlay cells with dim color blobs at each ZX position
        for( byte i = 0; i < 8; i++)
        { 
         uint8_t index = ZXSensorPos[i];
         int colorindex = map(index, 0, NUM_LEDS, 0, 240); // map total number of LEDs to full gradient value
         
          // Have color volume of 10 no matter what so I can see where to hit
          drumKitOverlay[index] += ColorFromPalette(drumPalette, colorindex, 10);
          drumKitOverlay[index+1] += ColorFromPalette(drumPalette, colorindex+1, 10);
        }
        
        break;
      }


      if (anal == 'N') 
      { 
     
     // parse kit sprites vars
        uint8_t drumSpriteNum = Serial.parseInt(); // then an ASCII number for sprite number
        uint8_t drumSpriteAmp = Serial.parseInt();
        uint8_t len = 4; // length of lit up sprite

        uint8_t index = ZXSensorPos[drumSpriteNum]; // calculate index at drumSprite

        int colorindex = map(index, 0, NUM_LEDS, 0, 255); // map total number of LEDs to full gradient value
        // Give sprite color boost
        //drumKitOverlay[index] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
        //drumKitOverlay[index+1] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
      // Set colors of pixels from outside toward center, increasing brightness in 32 steps    
      drumKitOverlay[index] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
      
      for(int j = 0; j < len; j++)
      {
        byte mypoint = (len-j);
        
          if((index-mypoint)>=0)  // ensure we're not writing to index off the strip
          {
            colorindex = map(index-mypoint, 0, NUM_LEDS, 0, 255); // map total number of LEDs to full gradient value
            // Give sprite color boost
            drumKitOverlay[index-mypoint] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
          } 
          if((index+mypoint)<(NUM_LEDS-1)) // ensure we're not writing to index off the strip
          {
            colorindex = map(index+mypoint, 0, NUM_LEDS, 0, 255); // map total number of LEDs to full gradient value
            // Give sprite color boost
            drumKitOverlay[index+mypoint] = ColorFromPalette(drumPalette, colorindex, drumSpriteAmp);
          }
          
        }
  
      }}
      break;
      
      //************************************************************
    // ARGB commands: ex, ARGB on/off [ CenterPoint wideness brightness, ... , ...]
    case 'A':  // string should start with A
      
    if (Serial.read() == 'R')  // then next should be R
    {
    if (Serial.read() == 'G')  // then next should be G
    { 
    if (Serial.read() == 'B')  // then next should be B
    {
      // Enable Pattern in refresh: 
      patternEnArray[7] = Serial.parseInt();  // On/Off ARGB
        
      for(int i = 0; i < 10; i++) // There are 9 parameters passing in here
      {
        ARGBParams[i] = Serial.parseInt();  
      }

      // Be sure we do not write over led length boundaries
      clipLen(&ARGBParams[0],&ARGBParams[1]);
      clipLen(&ARGBParams[3],&ARGBParams[4]);
      clipLen(&ARGBParams[6],&ARGBParams[7]);

      for(int i = 0; i < 3; i++)
      {
        int RC = ARGBParams[0+(i*3)];  // RmyIndex point start
        int RW = ARGBParams[1+(i*3)];  // R wideness
        int RB = ARGBParams[2+(i*3)];  // R brightness
        
  // draw solid pixels for each R, G, then B into RGB Overlay Layer
        for(int j = 0; j < RW; j++)
        {
          ARGBOverlay[RC+j] += CHSV( RGBHSV[i], 255, RB);
          // Clear LED Array
          //fill_solid( &(LED_BUFFER[RC]), RW, CHSV( RGBHSV[i], 255, RB) );
          //fill_solid( &(leds[HSVParams[3]]), HSVParams[4], CHSV( HSVParams[0], HSVParams[1], HSVParams[2]) );
        }
      }
    }
    }}
    break;

    //************************************************************
      // Serial message is: HSV (h, s, v, start, len)
      case'F':     
      anal = Serial.read(); //Read second char and analyyze     
      if (anal == 'R')  // then next should be G
      { 
      if (Serial.read() == 'E')  // then next should be i
      {  
        // Enable Pattern in refresh: 
        patternEnArray[8] = Serial.parseInt();  // On/Off FairyFire
        
     // parse Fairy Fire Params
        FIRParams[0] = Serial.parseInt();     // then an ASCII number for Hue
        FIRParams[1] = Serial.parseInt();   // then an ASCII number for sparking
        FIRParams[2] = Serial.parseInt();    // then an ASCII number for cooling
        FIRParams[3] = Serial.parseInt();   // then an ASCII number for start
        FIRParams[4] = Serial.parseInt();    // then an ASCII number for len
        FIRParams[5] = Serial.parseInt();    // then an ASCII number for len
        
        clipLen(&HSVParams[3],&HSVParams[4]);
        
      }break;}
      
      //************************************************************
      // Flicker LEDs
      // Serial message is: FLR (h, s, v, start, len)
      if(anal == 'L') 
      { 
      if (Serial.read() == 'R')  // then next should be i
      {
        
      }}
      break;

      //************************************************************
      // FunFetti
      // Serial message is: CFI (on/off) h, s, v, start, len, density
      // Notice: Missing saturation as it is assumed confetti will have fixed value
      case'C':     
      if (Serial.read() == 'F')  
      { 
      if (Serial.read() == 'I')  
      { 
        // parse glitter vars
        patternEnArray[9] = Serial.parseInt(); // then an ASCII number for 1/0 on off
        for(int i = 0; i < 6; i++) // There are 6 parameters passing in here
        {
         CFIParams[i] = Serial.parseInt();  
        }
        // constrain lengths to not write off max strip length
        clipLen(&CFIParams[3],&CFIParams[4]);
        
      }}
      break;

      //************************************************************
      // Mouth
      // Serial message is: MOU on/off h, s, v, index, len
      // Notice: Missing saturation as it is assumed confetti will have fixed value
      case'M':     
      if (Serial.read() == 'O')  
      { 
      if (Serial.read() == 'U')  
      { 
        // parse glitter vars
        patternEnArray[11] = Serial.parseInt(); // then an ASCII number for 1/0 on off

        for(int i = 0; i < 5; i++) // There are 5 parameters passing in here
        {
         MOUParams[i] = Serial.parseInt();  
        }
        
      }}
      break;
/*
      //************************************************************
      // Serial message is: WP (h, s, v, ms_interval, dir, animation counter)
      case'W':     
      if (Serial.read() == 'P')  
      { 
        // Enable Pattern in refresh: 
        patternEnArray[7] = 1;  // Turn On Automatically
        
        for(int i = 0; i < 5; i++) // There are 5 parameters passing in here
        {
         WPParams[i] = Serial.parseInt();  
        }
        WPParams[5] = NUM_LEDS; //reset animation counter

        // Clear wiper Array
        fill_solid( &(wiperOverlay[0]), NUM_LEDS, CRGB( 0, 0, 0) );

        if(WPParams[4]) // Set first LED position and color based on direction
        {
          wiperOverlay[NUM_LEDS-1] = CHSV( WPParams[0], WPParams[1], WPParams[2]);
        }
        else
        {
          wiperOverlay[0] = CHSV( WPParams[0], WPParams[1], WPParams[2]);
        }
        
      }
      break;
*/     
      //************************************************************
      // Serial message is: GLR (on/off, percent)
      // Add glitter with 0 - 100 percent density
      case'G':     
      if (Serial.read() == 'L') 
      { 
      if (Serial.read() == 'R') 
      { 
     
     // parse glitter vars
        patternEnArray[12] = Serial.parseInt(); // then an ASCII number for 1/0 on off
        
        // parse GlitterForce Params
        GLRParams[0] = Serial.parseInt();   // then an ASCII number for Hue
        GLRParams[1] = Serial.parseInt();   // then an ASCII number for Saturation
        GLRParams[2] = Serial.parseInt();   // then an ASCII number for Vol
        GLRParams[3] = Serial.parseInt();   // then an ASCII number for Random color spread
        GLRParams[4] = Serial.parseInt();   // then an ASCII number for GlitterForce (density)
        
        //glitterForce = Serial.parseInt();     // then an ASCII number for glitter density
        
        //glitterFlag = onOff;
      }}
      break;
      
      // Do nothing if no match
      default:
      break;
    }
  }
}

void callPatterns()
{
  // Clear LED Array
  fill_solid( &(leds[0]), NUM_LEDS, CRGB( 0, 0, 0) );
  
  for(int i=0; i < ARRAY_SIZE( gPatterns); i++)
  {
    if(patternEnArray[i]) // Check to see if each pattern in array is enabled
    {
      // Call the current pattern if enabled, updating the 'leds' array
      gPatterns[i]();
    }
  }

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do periodic updates here if needed
  //EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  //EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically


}

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

// This function takes pointers to array values to check if
// the start LED + LED length is less than total number of LEDs. If greater,
// set length to the correct ammount to just the end of the strip
// https://www.geeksforgeeks.org/pointers-in-c-and-c-set-1-introduction-arithmetic-and-array/
void clipLen(uint8_t *ledStart, uint8_t *ledLen)
{
   if(*ledStart > NUM_LEDS)
    *ledStart = *ledStart%NUM_LEDS; // Automatically warp back to begining if start is beyond number of LEDs
  
  if((*ledStart+*ledLen) > NUM_LEDS)
    *ledLen = NUM_LEDS - *ledStart;
}

void BGi()
{
  //  Set LEDs and save BG color values
     fill_solid( &(leds[BGiParams[3]]), BGiParams[4], CRGB( BGiParams[0], BGiParams[1], BGiParams[2]) );
}

void HSv()
{
  //  Set LEDs and save BG color values
     fill_solid( &(leds[HSVParams[3]]), HSVParams[4], CHSV( HSVParams[0], HSVParams[1], HSVParams[2]) );
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( &(leds[BOWParams[1]]), BOWParams[2], gHue, BOWParams[0]);
  fadeLightBy( leds, NUM_LEDS, BOWParams[3]);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

void spit()
{
  // Param array: h, v, start, len, dens
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));

  // copy Spit onto buffer
  for(int i = 0; i < NUM_LEDS; i++)
  {
    LED_BUFFER[i] += spitOverlay[i];
  }

  // Fade Spit layer for next time around
  fadeToBlackBy( spitOverlay, NUM_LEDS, SPTParams[5]); // Dim all LEDs in overlay layer by to 75% of what they were


  // Push LED buffer to LED array for writing
  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));

}

void ppit()
{
  // Param array: h, v, start, len, dens
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));

  // copy ppit onto buffer
  for(int i = 0; i < NUM_LEDS; i++)
  {
    LED_BUFFER[i] += ppitOverlay[i];
  }

  // Fade Spit layer for next time around
  fadeToBlackBy( ppitOverlay, NUM_LEDS, PPTParams[5]); // Dim all LEDs in overlay layer by to 75% of what they were


  // Push LED buffer to LED array for writing
  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));

}

void ticks()
{
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));
  uint8_t numTicks = TCKParams[5]; // pull num ticks 

  for(int i=0; i < NUM_LEDS; i++)
  {
    LED_BUFFER[i] += tickOverlay[i];
  }

// Fade out and shift over in direction till you hit original position of next led over
  EVERY_N_MILLISECONDS(1)
    {
      
      if(numTicks>0)
      {
        tickOverlay[NUM_LEDS-1] = CRGB( 0, 0, 0); // set top led to black
        
        for(int i=0; i < NUM_LEDS-1; i++)
        {
          tickOverlay[i] = tickOverlay[i+1];
        }
        
      TCKParams[5] = numTicks-1; // decrease tick count by 1
      
      }
      
      fadeLightBy( tickOverlay, NUM_LEDS, 40);
    }
  
  
  
  // Push LED buffer to LED array for writing
  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
  
}

void drumKit()
{
  
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));
  // Step 1:
  // First, create overlay of dim colored dots at the ZX sensor positions
  
  uint8_t len = 4; // Length 
  
  // fill overlay cells with dim color blobs at each ZX position
  for( byte i = 0; i < 8; i++)
  { 
    uint8_t index = ZXSensorPos[i];
                                                      // There are 169 LEDs, but 255 steps in the gradient
    int colorindex = map(index, 0, NUM_LEDS, 0, 240); // map total number of LEDs to full gradient value depending on ZX position

    // ADD minimum colors/brightness targets to buffer
    // Have color volume of 10 no matter what so I can see where to hit
      LED_BUFFER[index] += ColorFromPalette(drumPalette, colorindex, 10);
      LED_BUFFER[index+1] += ColorFromPalette(drumPalette, colorindex+1, 10);
    
    // If drumKitSprites at the current ZX sensor have color in them still, add to to LED_BUFFER
    if(drumKitOverlay[index])
    {
      LED_BUFFER[index] += drumKitOverlay[index];
      // Set colors of pixels from outside toward center, increasing brightness in 32 steps    
      for(int j = 0; j < len; j++)
      {
        byte mypoint = (len-j);
        
          if((index-mypoint)>=0)  // ensure we're not writing to index off the strip
          {
            // Map to main leds array immediately
            LED_BUFFER[index-mypoint] = drumKitOverlay[index-mypoint];
          }
          if((index+mypoint)<(NUM_LEDS-1)) // ensure we're not writing to index off the strip
          {
            LED_BUFFER[index+mypoint] = drumKitOverlay[index+mypoint];
          }

        }
    }

    // Fade all Kit Sprites down a little
    // last number is fade speed
    EVERY_N_MILLISECONDS(9)
    {
      fadeToBlackBy( drumKitOverlay, NUM_LEDS, 20);
    }
    // Push LED buffer to LED array for writing
    memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
  
  }

}

void ARGB()
{
  
  
  // We're realy using HSV, not RGB, so Hue values will be
  // 0 = RED, 96 = GREEN, 160 = BLUE, saved in array on an page
  
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));

  // Update ARGB Overlay
  // copy ARGBOverlay onto buffer, Our Fade Layer with pure RGB colors
  for(int i = 0; i < NUM_LEDS; i++)
  {
    LED_BUFFER[i] += ARGBOverlay[i];
  }

  // Fade ARGB layer for next time around by Fade Rate
  fadeToBlackBy( ARGBOverlay, NUM_LEDS, ARGBParams[9]); // Dim all LEDs in overlay layer by to 75% of what they were

// Clear LED Buffer at ARGB Locations to add back in foreground ARGB sprites
// This needs to be done before and seperately before adding over top the Actual fuzzy RGB sprites
  for(int i = 0; i < 3; i++)
      {
        int RC = ARGBParams[0+(i*3)];  // RmyIndex point start
        int RW = ARGBParams[1+(i*3)];  // R wideness
        int RB = ARGBParams[2+(i*3)];  // R brightness
        
  // draw pixels for each R, G, then B into RGB Overlay Layer
        for(int j = 0; j < RW; j++)
        {
          LED_BUFFER[RC+j] = CHSV(0, 0, 0);
        }
      }
  
  // Now - Write Actual "fuzzy" ARGB sprites over top of Everyting 
  uint8_t randomVar = 40; // ammount of random fuzz in hue

      for(int i = 0; i < 3; i++)
      {
        int RC = ARGBParams[0+(i*3)];  // RmyIndex point start
        int RW = ARGBParams[1+(i*3)];  // R wideness
        int RB = ARGBParams[2+(i*3)];  // R brightness
        
  // draw pixels for each R, G, then B into RGB Overlay Layer
        for(int j = 0; j < RW; j++)
        {
          LED_BUFFER[RC+j] += CHSV( RGBHSV[i] + (random8(randomVar)-(randomVar>>2)), 255, RB);
        }
      }

  // Push LED buffer to LED array for writing
  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
}



void funfetti() 
{
  // Param array: h, s, v, start, len, dens
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( funfettiOverlay, NUM_LEDS, 64);

  // Determine which LEDs will light up stochastically
  for(int i = CFIParams[3]; i < (CFIParams[3]+CFIParams[4]); i++)
  {
    int prob = random16(1024); // roll a 100-sided dice
    //funfettiOverlay[i] += CHSV( CFIParams[0] + random8(64), 200, prob);
    
    if(prob < CFIParams[5])
    {
      //glitterPos = i; // report funfetti pixel to serial for potential sonification
      funfettiOverlay[i] += CHSV( CFIParams[0] + random8(64), CFIParams[1], CFIParams[2]);
    }
    
  }

  // copy funfetti onto buffer
  for(int i = 0; i < NUM_LEDS; i++)
  {
    LED_BUFFER[i] += funfettiOverlay[i];
  }

  // Push LED buffer to LED array for writing
  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
}

// Bell Cloud colors
void bell()
{
  
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));
  // Step 1:
  // First, create overlay of dim colored dots at the ZX sensor positions
  
  uint8_t len = 4; // Length 
  
  // fill overlay cells with dim color blobs at each ZX position
  for( byte i = 0; i < 8; i++)
  { 
    uint8_t index = ZXSensorPos[i];
                                                      // There are 169 LEDs, but 255 steps in the gradient
    int colorindex = map(index, 0, NUM_LEDS, 0, 240); // map total number of LEDs to full gradient value depending on ZX position

    // ADD minimum colors/brightness targets to buffer
    // Have color volume of 10 no matter what so I can see where to hit
      LED_BUFFER[index] += ColorFromPalette(bellPalette, colorindex, 10);
      LED_BUFFER[index+1] += ColorFromPalette(bellPalette, colorindex+1, 10);
    
    // If drumKitSprites at the current ZX sensor have color in them still, add to to LED_BUFFER
    if(bellOverlay[index])
    {
      LED_BUFFER[index] += bellOverlay[index];
      // Set colors of pixels from outside toward center, increasing brightness in 32 steps    
      for(int j = 0; j < len; j++)
      {
        byte mypoint = (len-j);
        
          if((index-mypoint)>=0)  // ensure we're not writing to index off the strip
          {
            // Map to main leds array immediately
            LED_BUFFER[index-mypoint] = bellOverlay[index-mypoint];
          }
          if((index+mypoint)<(NUM_LEDS-1)) // ensure we're not writing to index off the strip
          {
            LED_BUFFER[index+mypoint] = bellOverlay[index+mypoint];
          }

        }
    }

    // Fade all Kit Sprites down a little
    // last number is fade speed
    EVERY_N_MILLISECONDS(9)
    {
      fadeToBlackBy( bellOverlay, NUM_LEDS, 20);
    }
    // Push LED buffer to LED array for writing
    memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
  
  }

}

// Mouth effect in foreground, overwrite LEDs at this effect
// random variation in hue for fuzz effect because... why not?
void mouth()
{
  uint8_t randomVar = 40; // ammount of random fuzz in hue
  uint8_t theIndex = MOUParams[3];
  uint8_t theLen = MOUParams[4];
  // Param array: [hue, blend, v, start, len] where blend is ammount of middle leds to mix in, cool when faded completely out
  // Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));
  
  // Write mouth effect directly into LED_Buffer
  for(int i = 0; i < theLen; i++)
  {
    uint8_t myPoint = theLen-i; // Calculate write point
    //float myVol = MOUParams[2] * (myPoint/theLen); // calculate brightness as ratio away from center point
    
    LED_BUFFER[theIndex] = CHSV( MOUParams[0] + random8(randomVar), MOUParams[1], MOUParams[2]); // write center point pixel
    
    // write to the start position minus calculated write point
    if((theIndex-myPoint)>=0)
    {
      if(!i) // end points always exact hsv as center point, 
        LED_BUFFER[theIndex-myPoint] = CHSV( MOUParams[0] + random8(randomVar), 255, MOUParams[2]);
      else // middle stuff takes into account blend
        LED_BUFFER[theIndex-myPoint] = CHSV( MOUParams[0] + random8(randomVar), 255, MOUParams[1]);

    }
    if((theIndex+myPoint)<(NUM_LEDS-1))
    {
      if(!i) // end points always exact hsv as center point
        LED_BUFFER[theIndex+myPoint] = CHSV( MOUParams[0] + random8(randomVar), 255, MOUParams[2]);
      else
        LED_BUFFER[theIndex+myPoint] = CHSV( MOUParams[0] + random8(randomVar), 255, MOUParams[1]);
    }
  }

  // Push LED buffer to LED array for writing
  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
  
}

// glitterForce var is percent change or density of glitter effect
void addGlitter() 
{
  if( random8() < GLRParams[4]) { // Roll 255 sided dice, if within "density" probability, then
    uint16_t randLED = random16(NUM_LEDS); // Choose a random LED location to light up
    
//    leds[ randLED ] += CRGB::White;
    leds[ randLED ] = CHSV( GLRParams[0] + random8(GLRParams[3]), GLRParams[1], GLRParams[2]); // Create fuzzy hue, sat, vol at LED
    glitterPos = randLED; // Report chosen glitter pixel position out serial for sonification
  }
  
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void setRGB()
{
  for(int i = 0; i < NUM_LEDS/2; i++) {   
    // fade everything out
    //leds.fadeToBlackBy(40);
    fadeToBlackBy( leds, NUM_LEDS, 40);
    // let's set an led value
    leds[i] = CHSV(gHue++,255,255);

    // now, let's first 20 leds to the top 20 leds, 
    leds[NUM_LEDS/2,NUM_LEDS-1] = leds[NUM_LEDS/2 - 1 ,0];
    FastLED.delay(33);
  }
}

// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120
// June 12, 2019 Chet adds Serial control of parameters. Looks awesome.
void fairyFire()
{
  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random8());

  CRGB darkcolor  = CHSV(FIRParams[0],255,192); // pure hue, three-quarters brightness
  CRGB lightcolor = CHSV(FIRParams[0],128,255); // half 'whitened', full brightness
  gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White); // set gradient color palette
// User error check to be sure fire len is never more than NUM_LEDS, which will crash the program
  int fireLen = FIRParams[4];
  if(fireLen > NUM_LEDS)
    fireLen = NUM_LEDS;
  
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS]; // cool all leds down regardless of current length for natural cooling effect

// Step 0: Copy contents of LEDs to LED_Buffer
  memmove8(&LED_BUFFER, &leds, NUM_LEDS*sizeof(CRGB));
// If active, fade background by 50%
  fadeLightBy( LED_BUFFER, NUM_LEDS, 20);
  
  // Step 1.  Cool down every cell a little, qsub subtracts random ammount from each cell
    for( int i = 0; i < NUM_LEDS; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((FIRParams[2] * 10) / NUM_LEDS) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    // Do this only for desired length of flame
    for( int k= fireLen - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < FIRParams[1] ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED Buffer colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat[j], 240);
      CRGB color = ColorFromPalette( gPal, colorindex);
      // Dim by parameter ammount
      color %= FIRParams[5];  
      int pixelnumber;
      // If Reverse Direction, flip, else don't
      if( FIRParams[3] )
      {
        pixelnumber = (NUM_LEDS-1) - j;
      } else {
        pixelnumber = j;
      }
      LED_BUFFER[pixelnumber] = color;
    }

    // ADD LED buffer to LED array for layering
    for(int i=0; i < NUM_LEDS; i++)
    {
      leds[i] += LED_BUFFER[i];
    }
    
}

// Old stuff:
/*
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}



 */

 /*
void wiper()
{
  uint8_t numTicks = WPParams[5]; // pull num ticks 
  
  for(int i=0; i < NUM_LEDS; i++)
  {
    LED_BUFFER[i] += wiperOverlay[i];
  }
  
  // Fade out and shift over in direction till you hit original position of next led over
  EVERY_N_MILLISECONDS(1)
    {
      if(numTicks>=0)
      {
        if(WPParams[4]) // Set first LED position and color based on direction
        {
          //wiperOverlay[NUM_LEDS-1] = CHSV( WPParams[0], WPParams[1], WPParams[2]);
          for(int i=0; i < NUM_LEDS-1; i++)
          {
            wiperOverlay[i] = wiperOverlay[i+1];
          }
        }
        else
        {
          //wiperOverlay[0] = CHSV( WPParams[0], WPParams[1], WPParams[2]);
          for(int i=NUM_LEDS-1; i > 0; i--)
          {
            wiperOverlay[i] = wiperOverlay[i-1];
          }
        }
        
      WPParams[5] = numTicks-1; // decrease tick count by 1
      
      }

      else
      {
        // Turn yourself off, animation over 
        patternEnArray[7] = 0;  // Turn off Automatically
      }
      
      //fadeLightBy( wiperOverlay, NUM_LEDS, 10);
    }

  memmove8(&leds, &LED_BUFFER, NUM_LEDS*sizeof(CRGB));
}

*/
