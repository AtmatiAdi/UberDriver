#include<Wire_slave.h>  
//#include<Wire.h>
#include <SPI.h>

//use IIC2
//TwoWire WIRE2 (2,I2C_FAST_MODE);
//TwoWire Wire2 (2, I2C_FAST_MODE);
#define LED_BUILTIN PB8
#define LED_2 PB9

#define DVR_1_SPI PB12   //SPI_2 Chip Select pin is PB12. You can change it to the STM32 pin you want.
#define DVR_1_WAKE PA12
#define DVR_1_ENGATE PA11
#define DVR_1_C PA10
#define DVR_1_B PA9
#define DVR_1_A PA8

#define DVR_2_SPI PA7 
#define DVR_2_WAKE PC9
#define DVR_2_ENGATE PB2
#define DVR_2_C PC6
#define DVR_2_B PC7
#define DVR_2_A PA8

/*
 *     Using the second SPI port (SPI_2)
    SS    <-->  PB12 <-->  BOARD_SPI2_NSS_PIN
    SCK   <-->  PB13 <-->  BOARD_SPI2_SCK_PIN
    MISO  <-->  PB14 <-->  BOARD_SPI2_MISO_PIN
    MOSI  <-->  PB15 <-->  BOARD_SPI2_MOSI_PIN
 */
 HardwareTimer timer(1);
SPIClass SPI_2(2); //Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port
word msg;
word data;

bool AdcTest = false;
bool prog_01;
bool prog_02;
int PhaseDelay;
int PhaseWidth;

void setup() {
  Serial.end();
  // Setup pins for DVR_1 pins
  pinMode(DVR_1_WAKE, OUTPUT);
  pinMode(PA11, OUTPUT);
  pinMode(DVR_1_A, PWM);
  pinMode(DVR_1_B, PWM);
  pinMode(DVR_1_C, PWM);
  // Pause the timer while we're configuring it
  timer.pause();
  // Setup Timer channels modes
  timer.setMode(TIMER_CH1, TIMER_PWM);
  timer.setMode(TIMER_CH2, TIMER_PWM);
  timer.setMode(TIMER_CH3, TIMER_PWM);
  //timer.setMode(TIMER_CH4, TIMER_OUTPUT_COMPARE);
  //timer.attachInterrupt(TIMER_CH4, ToggleLed);
  //timer.attachCompare4Interrupt(ToggleLed);
  //timer.setInputCaptureMode(TIMER_CH1, TIMER_IC_INPUT_DEFAULT); // co to robi ??
   /*
    * TIMER_OUTPUT_COMPARE -
    * In this mode, the timer counts from 0 to its reload value repeatedly; every time the counter value reaches one of the channel compare values, the corresponding interrupt is fired.
    * TIMER_PWM -
    * PWM output mode.
    * This is the default mode for pins after initialization.
    * TIMER_DISABLED -
    * In this mode, the timer stops counting, channel interrupts are detached, and no state changes are output.
    */
  timer.setCount(0);
  // 72Mhz / Prescaller value
  timer.setPrescaleFactor(360);
  // Period witch 0 to value
  timer.setOverflow(7);
  // 72MHz / 720 = 100 000Hz -> 10 microseconds
  // 72MHz / 360 = 200 000Hz -> 10 microseconds
  timer.setCompare(TIMER_CH1, 0);  // PA8 // PWM High Pulse width is when reach second value
  timer.setCompare(TIMER_CH2, 0);  // PA9
  timer.setCompare(TIMER_CH3, 0);  // PA10
  //timer.setCompare(TIMER_CH4, 0); 
  timer.refresh();
  //timer.resume();

  
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_2, OUTPUT);
  // Star communication with slave addres 4
  Wire.begin(4); 
  // I2c for adc
  //Wire1.setClock(400000);
  Wire1.begin();    
  // Set on receive and reqest events
  Wire.onReceive(receiveEvent); // register event  
  Wire.onRequest(requestEvent); // register event 

  // Setup SPI 2
  SPI_2.begin(); //Initialize the SPI_2 port.
  SPI_2.setBitOrder(MSBFIRST); // Set the SPI_2 bit order
  SPI_2.setDataMode(SPI_MODE1); //Set the  SPI_2 data mode 1
  SPI_2.setClockDivider(SPI_CLOCK_DIV16);  // Use a different speed to SPI 1
  pinMode(DVR_1_SPI, OUTPUT);
  pinMode(DVR_2_SPI, OUTPUT);
  digitalWrite(DVR_1_SPI, HIGH);
  digitalWrite(DVR_2_SPI, HIGH);


  delay(100);

  prog_01 = false;
  prog_02 = false;
}

void requestEvent()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
  Wire.write(byte(msg >> 8)); // send 8 left 8 bits of msg
  Wire.write(byte(msg)); // send 8 right bits of msg
  Wire.write(byte(data >> 8)); // send 8 left 8 bits of data
  Wire.write(byte(data)); // send 8 right bits of data
}

void receiveEvent(int howMany){
  char one, two;
  if(Wire.available())one = Wire.read(); // receive byte as a character
  if(Wire.available()){
    digitalWrite(LED_2, !digitalRead(LED_2)); 
    // Jezeli jest drogi bajt to znaczy ze to komenda dalej a nie dala samego sterownika
    two = Wire.read(); // receive byte as a character
    msg = word(one << 8) + two;
    digitalWrite(DVR_2_SPI, LOW); // manually take CSN low for SPI_1 transmission
    data = SPI_2.transfer16(msg); //Send the HEX data 0x55 over SPI-1 port and store the received byte to the <data> variable.
    digitalWrite(DVR_2_SPI, HIGH); // manually take CSN high between spi transmissions
    
  } else {
    // Komenda sterownika
    switch (one) {
      case '1': {
        digitalWrite(LED_BUILTIN, LOW);
        break;
      }
      case '2': {
        digitalWrite(LED_BUILTIN, HIGH);
        break;
      }
      case '3': {
        digitalWrite(PA11, HIGH); // Enable gates
        //delay(100); // Wait for voltage pomp (need 1ms)
        digitalWrite(DVR_1_A, HIGH); 
        break;
      }
      case '4': {
        digitalWrite(PA11, HIGH); // Enable gates
        //delay(100); // Wait for voltage pomp (need 1ms)
        digitalWrite(DVR_1_A, LOW);
        break;
      }
      case '5': {
        digitalWrite(PA11, HIGH); // Enable gates
        delay(100); // Wait for voltage pomp (need 1ms)
        digitalWrite(DVR_1_B, HIGH); 
        break;
      }
      case '6': {       
        digitalWrite(PA11, HIGH); // Enable gates
        delay(100); // Wait for voltage pomp (need 1ms)
        digitalWrite(DVR_1_B, LOW); 
        break;
      }
      case '7': {        
        digitalWrite(PA11, HIGH); // Enable gates
        delay(100); // Wait for voltage pomp (need 1ms)
        digitalWrite(DVR_1_C, HIGH); 
        break;
      }
      case '8': {        
        digitalWrite(PA11, HIGH); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        digitalWrite(DVR_1_C, LOW); 
        break;
      }
      case 'A': {
        prog_01 = true;
        prog_02 = false;
        digitalWrite(PA11, HIGH); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
        break;
      }
      case 'B': {
        prog_01 = false;
        digitalWrite(PA11, LOW); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
        break;
      }
      case 'C': {
        prog_02 = true;
        prog_01 = false;
        PhaseDelay = 10000;
        PhaseWidth = 7;
        digitalWrite(PA11, HIGH); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
        timer.setCompare(TIMER_CH4, 49); 

        timer.refresh();
        timer.resume();
        break;
      }
      case 'D': {
        prog_02 = false;
        digitalWrite(PA11, LOW); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
        timer.pause();
        break;
      }
      case 'c': {
          PhaseDelay += 100;
          digitalWrite(LED_2, !digitalRead(LED_BUILTIN));  
        break;
      }
      case 'd': {
          PhaseDelay -= 100;
          if (PhaseDelay < 1) PhaseDelay = 1;
          digitalWrite(LED_2, !digitalRead(LED_BUILTIN));  
        break;
      }
      case 'e': {
          PhaseWidth += 1;
          digitalWrite(LED_2, !digitalRead(LED_BUILTIN));  
        break;
      }
      case 'f': {
          PhaseWidth -= 1;
          if (PhaseWidth < 0) PhaseWidth = 0;
          digitalWrite(LED_2, !digitalRead(LED_BUILTIN));  
        break;
      }
      case 'x': {
        AdcTest = true;
        break;
      }
    }
  }
}

void program_01(int Width, int Freq){
  int FDelay;
  int WDelay;
  
}

void ToggleLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
  //digitalWrite(LED_2, !digitalRead(LED_2)); 
}

void loop() {
  // put your main code here, to run repeatedly:
  //delay(100);
  //digitalWrite(LED_BUILTIN, s);   
  //s = !s;
  if (prog_02 == true) {
    
        timer.setCompare(TIMER_CH1, PhaseWidth);  // PA8 
        timer.setCompare(TIMER_CH2, PhaseWidth);  // PA9
        timer.setCompare(TIMER_CH3, 0);  // PA10
        delayMicroseconds(PhaseDelay);
        timer.setCompare(TIMER_CH1, 0);  // PA8 
        timer.setCompare(TIMER_CH2, PhaseWidth);  // PA9
        timer.setCompare(TIMER_CH3, PhaseWidth);  // PA10
        delayMicroseconds(PhaseDelay);
        timer.setCompare(TIMER_CH1, PhaseWidth);  // PA8 
        timer.setCompare(TIMER_CH2, 0);  // PA9
        timer.setCompare(TIMER_CH3, PhaseWidth);  // PA10
        delayMicroseconds(PhaseDelay);
        if (PhaseDelay > 3000) PhaseDelay -= 100;
        if (PhaseDelay > 2000) PhaseDelay -= 10;
        if (PhaseDelay > 1000) PhaseDelay -= 5;
        if (PhaseDelay > 250) PhaseDelay -= 1;
       
  }
  if (prog_01 == true) {
            
        
         digitalWrite(DVR_1_A, HIGH);
         delayMicroseconds(500);
         digitalWrite(DVR_1_A, LOW); 
        digitalWrite(DVR_1_B, LOW); 
        digitalWrite(DVR_1_C, LOW);
         
        delay(100);
        
        digitalWrite(DVR_1_A, LOW); 
         digitalWrite(DVR_1_B, HIGH); 
         delayMicroseconds(500);
         digitalWrite(DVR_1_B, LOW);
        digitalWrite(DVR_1_C, LOW); 
        
        delay(100);
        
        digitalWrite(DVR_1_A, LOW);
        digitalWrite(DVR_1_B, LOW); 
         digitalWrite(DVR_1_C, HIGH);
         delayMicroseconds(500); 
         digitalWrite(DVR_1_C, LOW); 
         
        delay(100);
  } else {
        digitalWrite(DVR_1_A, LOW);
        digitalWrite(DVR_1_B, LOW); 
        digitalWrite(DVR_1_C, LOW); 
  }
  if (AdcTest == true) {
        // 1001 011x -> SCL
        // 1001 0111 -> Odczyt -> 97
        // 1001 0111 -> Zapis -> 96
          data = 1;
          Wire1.beginTransmission(0b1001011); //7-bit deviceAddress
          Wire1.write(byte(1));       //registerAddress
          data = Wire1.endTransmission();
          digitalWrite(LED_2, !digitalRead(LED_2));  
          msg = 1;
          //delay(1);
          Wire1.requestFrom(0b1001011, 2);
          //delay(1);
          if (2 <= Wire1.available()) { // if two bytes were received
            msg = Wire1.read();  // receive high byte (overwrites previous reading)
            msg = msg << 8;    // shift high byte to be high 8 bits
            msg |= Wire1.read(); // receive low byte as lower 8 bits
          } else digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
          AdcTest = false;
  ///////////////////////////////////////////////////////////////////////    
  }
}
