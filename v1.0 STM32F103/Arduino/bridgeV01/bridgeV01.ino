#include<Wire_slave.h>
char r;

void setup() {
  /* Inicjalizacja komunikacji po serialu, jest to przez interfejs USB */
  Serial.begin(9600);
  /* I2C communication at pin B6, B7 as Master*/
  Wire.begin(); 
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    r = Serial.read();
    if (r != '\r') {
      if ((r == 'C') or (r == 'c')){
        // Jezeli jest to komenda
        if(Serial.available()) {
          r = Serial.read();
          if (r == '1') {
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("Led off");
          }
          if (r == '2') {
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("Led on");
          }  
          Wire.beginTransmission(4);  
          Wire.write(r);                        
          Wire.endTransmission(); 
          if (r == 'y') {
            delay(1);
            Wire.requestFrom(4, 4); // addres, ilosc bitow
            word msg = 0;
            if (Wire.available()) msg = word(Wire.read() << 8) + word(Wire.read()); 
            Serial.println(msg);
            if (Wire.available()) msg = word(Wire.read() << 8) + word(Wire.read()); 
            Serial.println(msg);
          }
        } else {
          Serial.println("Empty command");
        }
      }
      if ((r == 'B') or (r == 'b')) {
        // Jezeli jest to slowo do sterownika
        int i;
        bool ok = true;
        word msg = 0;
        for (i=15;i>=0;i--) {
          if(Serial.available()){
            r = Serial.read();
            if (r == '1') bitWrite(msg, i, 1);
            else if (r == '0') bitWrite(msg, i, 0);
            else {
              Serial.print("Bad bits");
              ok = false;
              break;             
            }
          } else {
            Serial.print("Not enought bits");
            ok = false;
            break;
          }
        }
        if(Serial.available()){
          r = Serial.read();
          if ((r != '\r') and (ok)) {
            ok = false;
            Serial.print("To many bits");
          }
        }
        if (ok) {
          Serial.println("Ok ");
          Serial.println(msg);
          Wire.beginTransmission(4);  
          byte right = byte(msg);
          byte left = byte(msg >> 8);                          
          Wire.write(left);   
          Wire.write(right);                        
          Wire.endTransmission(); 
          // Odczyt danych uzyskanych od drivera
          delay(1);
          Wire.requestFrom(4, 4); // addres, ilosc bitow
          msg = 0;
          if (Wire.available()) msg = word(Wire.read() << 8) + word(Wire.read()); 
          Serial.println(msg);
          if (Wire.available()) msg = word(Wire.read() << 8) + word(Wire.read()); 
          Serial.println(msg);
          
        }
      }
      
      Serial.print(r);
      // Begin transmittion to device with addres 8
      /*Wire.requestFrom(8, 1);    // request 1 bytes from slave device #8
      //delay(1);
       while(Wire.available())    // slave may send less than requested
  {
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }*/
    
    }
    
  }
}
