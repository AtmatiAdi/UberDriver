#include<Wire.h>

void event (int num){
  
}

void setup() {
  // put your setup code here, to run once:
  Wire.onReceive(event);
}

void loop() {
  // put your main code here, to run repeatedly:

}
