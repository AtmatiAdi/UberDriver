unsigned long time;
unsigned long old_time;
#define LED_BUILTIN PB8
 /*  Zmienne definiowane na potzreby sterowania DSinusem */
#define PI 3.1415926535897932384626433832795
float FSin = 1;           // Czestotliwosc Sinusa
//float ASin = 1;           // Amplituda Trojkata, raczej stała
float FTr = FSin * 18;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
float ATr = 9;            // Amplituda Trojkata, im wieksza tym wieksza moc
float Fp = 100000;        // Czestotliwosc probkowania, czestotliwosc przelaczania

unsigned int DelayValue;           // Wartoc pomiedzy probkowaniem
float StepGain;           // Wartosc w radianach co probkowanie
long AStepSin;             // aktualny krok fazy A
long BStepSin;             // aktualny krok fazy B
long CStepSin;             // aktualny krok fazy C
long StepTr;               // aktualny krok trojkata
double DSin, Tr;          // Wartosc 
float StepsPPSin, StepPPTr; // Ilosc probkowan na caly okres
bool Out;             // Wyjscie
        // Obliczenia co kazda zmiane
long StepsPPSin_3;
long StepsPPSin_6;
long StepsPPSin_23;
long StepPPTr_4;
float StepPPTr_2;
long StepPPTr_34;
float StepPPTr__2;
bool Test_Sin = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  time = micros();
  
        Test_Sin = true;
        FSin = 1;           // Czestotliwosc Sinusa
        //ASin = 1;           // Amplituda Trojkata, raczej stała
        FTr = FSin * 90;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
        ATr = 1;            // Amplituda Trojkata, im wieksza tym mniejsza moc
        Fp = 10000;        // Czestotliwosc probkowania, czestotliwosc przelaczania
        // DSinus
        DelayValue = 1000000/Fp;
        CStepSin = 0;
        StepTr = 0;
        // Obliczenia co kazda zmiane
        StepPPTr = Fp/FTr;
        StepsPPSin = Fp/FSin;
        StepsPPSin_3 = StepsPPSin/3;
        StepsPPSin_6 = StepsPPSin/6;
        StepsPPSin_23 = StepsPPSin*2/3.0;
        StepGain = FSin/Fp * 2 * PI;
        StepPPTr_4 = StepPPTr/4;
        StepPPTr_2 = 2/StepPPTr;
        StepPPTr_34 = StepPPTr*3/4.0;
        StepPPTr__2 = -2/StepPPTr;
}
void ToggleLed() {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
  //digitalWrite(LED_2, !digitalRead(LED_2)); 
}

void loop() {
  // put your main code here, to run repeatedly:
    while(Test_Sin) {
      //StepPPTr = Fp/FTr;
      //StepsPPSin = Fp/FSin;
      
      CStepSin += 1;                       // Inkrementacja kroku
      BStepSin = CStepSin + StepsPPSin_3;  // Przesuniecie o 1/3
      AStepSin = BStepSin + StepsPPSin_3;  // Przesuniecie o kolejna 1/3
      StepTr += 1;
  
      if (CStepSin >= StepsPPSin) {CStepSin = 0;StepTr = 0;ToggleLed(); 
        time = micros();
        Serial.println((time-old_time)/Fp);
        time = micros();
        old_time = time;
      }
      if (BStepSin >= StepsPPSin) BStepSin -= StepsPPSin;
      if (AStepSin >= StepsPPSin) AStepSin -= StepsPPSin;
      if (StepTr >= StepPPTr) StepTr = 0;
      
      //StepGain = FSin/Fp * 2 * PI;
      
      // Wartosc Trojkata
      if(StepTr<=StepPPTr_4){Tr=ATr*(StepTr*StepPPTr_2+0.5);}else if(StepTr<=StepPPTr_34){Tr=ATr*(StepTr*StepPPTr__2+1.5);}else{Tr=ATr*(StepTr*StepPPTr_2-1.5);}
  
      // Wartosc DSinusa A
      if (AStepSin<=StepsPPSin_3){DSin=sin(AStepSin*StepGain);}else if(AStepSin<=StepsPPSin_23){DSin=sin((AStepSin-StepsPPSin_6)*StepGain);}else{DSin=0;}  
      if(Tr < DSin) digitalWrite(LED_BUILTIN, HIGH); else digitalWrite(LED_BUILTIN, LOW);
  
      // Wartosc DSinusa B
      if (BStepSin<=StepsPPSin_3){DSin=sin(BStepSin*StepGain);}else if(BStepSin<=StepsPPSin_23){DSin=sin((BStepSin-StepsPPSin_6)*StepGain);}else{DSin=0;}  
      if(Tr < DSin) digitalWrite(LED_BUILTIN, HIGH); else digitalWrite(LED_BUILTIN, LOW);
      // Wartosc DSinusa C
      if (CStepSin<=StepsPPSin_3){DSin=sin(CStepSin*StepGain);}else if(CStepSin<=StepsPPSin_23){DSin=sin((CStepSin-StepsPPSin_6)*StepGain);}else{DSin=0;}  
      if(Tr < DSin) digitalWrite(LED_BUILTIN, HIGH); else digitalWrite(LED_BUILTIN, LOW);
      //delayMicroseconds(DelayValue);
      //digitalWrite(LED_BUILTIN, HIGH);  
    }
}
