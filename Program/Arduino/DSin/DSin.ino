#define PI 3.1415926535897932384626433832795
float FSin = 1;      // Czestotliwosc Sinusa
float ASin = 1;       // Amplituda Trojkata, raczej stała
float FTr = FSin * 18;       // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
float ATr = 9;      // Amplituda Trojkata, im wieksza tym wieksza moc
float Fp = 300;    // Czestotliwosc probkowania, czestotliwosc przelaczania

int DelayValue;       // Wartoc pomiedzy probkowaniem
float StepGain;       // Wartosc w radianach co probkowanie
int AStepSin;          // aktualny krok fazy A
int BStepSin;         // aktualny krok fazy B
int CStepSin;          // aktualny krok fazy C
int StepTr;           // aktualny krok trojkata
double DSin, Tr;      // Wartosc 
float StepsPPSin, StepPPTr; // Ilosc probkowan na caly okres
bool Out;             // Wyjscie

bool Test_Sin = true;


void setup() {
  // put your setup code here, to run once:
  DelayValue = 1000000/Fp;
  Serial.begin(9600);
  //StepsPPSin = Fp/FSin;
  CStepSin = 0;
  StepTr = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
 if (Test_Sin) {
    StepPPTr = Fp/FTr;
    StepsPPSin = Fp/FSin;
    
    CStepSin += 1;                        // Inkrementacja kroku
    BStepSin = CStepSin + StepsPPSin/3;  // Przesuniecie o 1/3
    AStepSin = BStepSin + StepsPPSin/3;  // Przesuniecie o kolejna 1/3
    StepTr += 1;


    
    if (CStepSin >= StepsPPSin) {CStepSin = 0;StepTr = 0;}
    if (BStepSin >= StepsPPSin) BStepSin -= StepsPPSin;
    if (AStepSin >= StepsPPSin) AStepSin -= StepsPPSin;
    if (StepTr >= StepPPTr) StepTr = 0;
    
    StepGain = FSin/Fp * 2 * PI;
    
    // Wartosc Trojkata
    if(StepTr<=StepPPTr/4){Tr=ATr*(2*StepTr/StepPPTr+1/2.0);}else if(StepTr<=StepPPTr*3/4.0){Tr=ATr*(-2*StepTr/StepPPTr+3/2.0);}else{Tr=ATr*(2*StepTr/StepPPTr-3/2.0);}
  Serial.print("\t");
  Serial.print(Tr + 1);
  Serial.print(" ");
    // Wartosc DSinusa A
    if (AStepSin<=StepsPPSin/3){DSin=sin(AStepSin*StepGain);}else if(AStepSin<=StepsPPSin*2/3.0){DSin=sin((AStepSin-StepsPPSin/6)*StepGain);}else{DSin=0;}  
    if(Tr < DSin) Out = 1; else Out = 0;
  Serial.print(DSin + 1);
  Serial.print(" ");
  Serial.print(Out + 1);
  Serial.print(" ");
    // Wartosc DSinusa B
    if (BStepSin<=StepsPPSin/3){DSin=sin(BStepSin*StepGain);}else if(BStepSin<=StepsPPSin*2/3.0){DSin=sin((BStepSin-StepsPPSin/6)*StepGain);}else{DSin=0;}  
    if(Tr < DSin) Out = 1; else Out = 0;
  Serial.print(DSin + 2);
  Serial.print(" ");
  Serial.print(Out + 2);
  Serial.print(" ");
    // Wartosc DSinusa C
    if (CStepSin<=StepsPPSin/3){DSin=sin(CStepSin*StepGain);}else if(CStepSin<=StepsPPSin*2/3.0){DSin=sin((CStepSin-StepsPPSin/6)*StepGain);}else{DSin=0;}  
    if(Tr < DSin) Out = 1; else Out = 0;
  Serial.print(DSin + 3);
  Serial.print(" ");
  Serial.println(Out + 3);
  }



 
  delayMicroseconds(DelayValue);
}
