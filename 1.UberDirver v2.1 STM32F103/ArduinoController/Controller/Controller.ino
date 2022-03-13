/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

#define SET_MOTOR_1_SPEED_FORWAD        64    // 0
#define SET_MOTOR_1_SPEED_BACK          65    // 1
#define SET_MOTOR_1_SPEED_FORWAD_SEQ_1  66    // 2
#define SET_MOTOR_1_SPEED_BACK_SEQ_1    67    // 3
#define SET_MOTOR_1_SPEED_FORWAD_SEQ_2  68    // 4
#define SET_MOTOR_1_SPEED_BACK_SEQ_2    69    // 5
#define SET_MOTOR_1_SPEED_FORWAD_SEQ_3  70    // 6
#define SET_MOTOR_1_SPEED_BACK_SEQ_3    71    // 7

#define SCAN_ONE_VARIABLE               80    // 16

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  pinMode(3,INPUT);
}

uint8_t power = 0;
uint8_t com_delay = 0;
uint8_t old_power = 0;
int delay_ms = 50;
int scan_data_size = 4095;
int scan_pack_size = 4;
bool display_RPM = true;
// the loop function runs over and over again forever
void loop() {
  
  uint16_t pot1 = analogRead(A0);
  uint16_t pot2 = analogRead(A1);
  bool bt1 = digitalRead(3);
  if(pot1 > 1023) pot1 = 1023;
  if(pot2 > 1023) pot2 = 1023;
  
  power = pot1/4;
  com_delay = pot2/4;
  uint8_t buf[3] = {0,0,0};
  int scan_cnt = 0;

  if (bt1){
    display_RPM = false;
    // Button funcion
    buf[0] = SCAN_ONE_VARIABLE;
    buf[1] = 1;
    buf[2] = 0;
    Serial1.write(buf, 3);
    while(scan_cnt < scan_data_size){
      if(Serial1.available() >= scan_pack_size){
        for (int i = 0; i <= scan_pack_size - 2; i ++ ){
          Serial.print(Serial1.read());
          Serial.print("\t");
        }
        Serial.println(Serial1.read());
        scan_cnt += scan_pack_size;
      }
    }
  }

  buf[0] = SET_MOTOR_1_SPEED_FORWAD;
  buf[1] = power;
  buf[2] = com_delay;
  Serial1.write(buf, 3);
  delay(delay_ms);
      
  if (display_RPM){
    if(Serial1.available()){
        uint8_t Rotations = Serial1.read();
        float RPM = ((float)Rotations / 4) * (1000/delay_ms) * 60;
        for (int i = 0; i <= scan_pack_size - 2; i ++ ){
          Serial.print(RPM);
          Serial.print("\t");
        }
        Serial.println(255);
    }
  }
}
