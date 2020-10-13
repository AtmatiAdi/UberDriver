void setup()
{  
 // Open serial communications and wait for port to open:
//  Serial.begin(1200);
//  Serial.begin(2400);
//  Serial.begin(4800);
  Serial3.begin(9600);
//  Serial.begin(14400);
//  Serial.begin(19200);
//  Serial.begin(38400);
//  Serial.begin(57600);
//  Serial.begin(115200); 
}
void loop()
{
  while(Serial3.available() > 0) {
    Serial3.write(Serial3.read());
  }
}
