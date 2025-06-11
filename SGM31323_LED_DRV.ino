/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
#include <Wire.h>

int LED_BUILTIN = 2;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("\nI2C Scanner");
}

void scan_i2c_addr(){
 byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000); 
}

void sgm31323_write(uint8_t* data)
{
    Wire.beginTransmission(0x30);

    for(uint8_t i = 0; i < 9; ++i){
      Wire.write(i); // reg0 ~ reg8
      Wire.write(data[i]); // data0 ~ data8
      delay(1);
    }

    byte error = Wire.endTransmission();
    if(error == 0 ){
      Serial.println("I2C device close ok\n");
    }    
    else if (error==4) {
      Serial.println("Unknow error at address 0x");
    }
}

// 3v3 -> R-LED -> D1
// 3v3 -> G-LED -> D2
// 3v3 -> B-LED -> D3


// the loop function runs over and over again forever
void loop() {
  // scan_i2c_addr(); //0x30 is sgm31323 address in Wire()

  uint8_t data[9] = {
    0x1c, // always on
    0x0e, // approximately 2s
    0xfd, // PWM1 timer
    0xfd, // PWM2 timer
    0x0E, // 01 = LED1 Always ON
    0x33, // PWM RISE 3 0x03, PWM FALL 0x03
    0x4f, // led1 current
    0x4f, // led2 current
    0x4f  // led3 current 
  };

  sgm31323_write(data);

  delay(1000);                      // wait for a second

  while(1){

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);   
  }                   // wait for a second
}
