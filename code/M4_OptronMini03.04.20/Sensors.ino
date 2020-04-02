void sendVals()
{
  #ifdef OUTPUT_READABLE

      Serial.println("MPU6050");
//      Serial.println((float)yaw);
//      Serial.println((float)pitch);
//      Serial.println((float)roll);
      Serial.println((float)ax/16000);
      Serial.println((float)ay/16000);
      Serial.println((float)az/16000);
      Serial.println((float)gx/16000);
      Serial.println((float)gy/16000);
      Serial.println((float)gz/16000);

/* 
// print all ZX values
    for (uint8_t i=0; i<8; i++) {
      if ( z_pos[i] != ZX_ERROR ) 
      {
        Serial.print("Z"); Serial.print(i); Serial.print(": ");
        Serial.print(z_pos[i]);
      }
      if ( x_pos[i] != ZX_ERROR ) 
      {
        Serial.print("  X"); Serial.print(i); Serial.print(": ");
        Serial.println(x_pos[i]);
      }
    }
*/        
      Serial.print(" FSR: ");
      Serial.println(fsrVal);

      Serial.print(" LinPot: ");
      Serial.println(linVal);
      
    #endif
    
    #ifdef OUTPUT_BINARY
      // Sensor number (1=analog, 2=MPU, 3=SpkZX, ), num packets
      // sensor1 H, Sensor1 L
      // sensor2 H, Sensor1 2
      // etc
      // packet data
      binaryFloat hi;
// Note, the first two bytes transmitted must be taken into account in the software serial client
// byte 1 = device type, byte 2 = how many transactions/transmits
// Type 1 = analog sensor transmitting 2-byte (H/L) ints, Type 2 = serial, 4-byte floating point
      
     // 2 = device type serial, 4-byte floating point, then transmit 9 transactions of this type
     //Serial.write((uint8_t)2); Serial.write((uint8_t)9); // Use this for transmitting all 9 things (YPR)
     Serial.write((uint8_t)2); Serial.write((uint8_t)6); // Without YPR transmission
      // ex: 2, 9, a a a a, b b b b, c c c c, etc . . .
//      hi.floatingPoint = yaw; Serial.write(hi.binary,4);
//      hi.floatingPoint = pitch; Serial.write(hi.binary,4);
//      hi.floatingPoint = roll; Serial.write(hi.binary,4);
      hi.floatingPoint = (float)ax/16000; Serial.write(hi.binary,4);
      hi.floatingPoint = (float)ay/16000; Serial.write(hi.binary,4);
      hi.floatingPoint = (float)az/16000; Serial.write(hi.binary,4);
      hi.floatingPoint = (float)gx/16000; Serial.write(hi.binary,4);
      hi.floatingPoint = (float)gy/16000; Serial.write(hi.binary,4);
      hi.floatingPoint = (float)gz/16000; Serial.write(hi.binary,4);

      // 1 = type Analog, 2-byte int (H/L), transmit 4 transactions of this type
      Serial.write(1); Serial.write(3); // 2 analog
      // ex: 1, 4, a a, b b, c c, d d  (till finished)

      // Optron Light Event Out high byte, low byte
      Serial.write((uint8_t)(0x00)); Serial.write((uint8_t)(glitterPos));
      
      // FSR high byte, low byte
      Serial.write((uint8_t)(fsrVal >> 8)); Serial.write((uint8_t)(fsrVal & 0xFF));
      
      // Linear sensor high byte, low byte
      Serial.write((uint8_t)(linVal >> 8)); Serial.write((uint8_t)(linVal & 0xFF));

      // ePicUp high byte, low byte
//      Serial.write((uint8_t)(ePicUpVal >> 8)); Serial.write((uint8_t)(ePicUpVal & 0xFF));

      // LinPot high byte, low byte
//      Serial.write((uint8_t)(ePicDownVal >> 8)); Serial.write((uint8_t)(ePicDownVal & 0xFF));

/*
      // 1 = type Analog, 2-byte int (H/L), transmit 4 transactions of this type
     Serial.write(3); Serial.write(16); // 8x2 ZX readings
      // ex: 1, 4, a a, b b, c c, d d  (till finished)

      // print all ZX values
    for (uint8_t i=0; i<8; i++) {
        Serial.write(z_pos[i]);
        Serial.write(x_pos[i]);
    }
*/ 
  // ********** Get All Readings right!     ********  
    #endif
    
}
/*
// SPARKFUN XZ Sensor Read Function:
void readXZ()
{
// TCA loop to read all 8 sensor ports
  for (uint8_t t=0; t<8; t++) {
    tcaseselect(t);
    // If there is position data available, read and print it
    if ( zx_sensor.positionAvailable() ) {
      x_pos[t] = zx_sensor.readX();
      z_pos[t] = zx_sensor.readZ();
    }
    
  } // end TCA loop
}
*/

// Read Analog sensor values
void readAnalog()
{
  fsrVal = analogRead(FSRPin);
  linVal = analogRead(LinPin);
//  ePicUpVal = analogRead(ePicUpPin);
//  ePicDownVal = analogRead(ePicDownPin);
}

// Generic subroutine for reading MPU6050 i2c Data
void measure_mpu6050(void)
{
  // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);


} // of measure_mpu6050() 
