/* ------------------------------------------------------------------------------ //
// Biblioteki ------------------------------------------------------------------- */
/* Biblioteka do obsługi I2C https://www.arduino.cc/en/reference/wire 
// Wire i Wire_slawe to to samo, rozne procesorki maja rozne biblioteki o tych samych nazwach
// biblioteka Wire jest niekompletna o obsluge bycia slavem. */
#include<Wire_slave.h>  
/* Biblioteka do obslogi SPI https://www.arduino.cc/en/reference/SPI */
#include<SPI.h>
// Biblioteka włąsnych funkcji
/* ------------------------------------------------------------------------------ //
// Definicje pinow ---------------------------------------------------------------*/
/* Piny zdefioniowane dla urzytkownika do sygnalizowanie diodami
// Procek posiada kilka portów: PA, PB, PC, PD, PE po 16 pinow, nie wszystkie sa wyprowadzone
// niektore frazy sa popularne i kompilator je podswietla, nic to nie znaczy generalnie */
#define LED_BUILTIN PB8
#define LED_2 PB9

/* Definicja pinow dla kazdego sterownika, pomieszanie ze wzgledu ze kierowalem sie 
// te gdzie timer pasuje */
/* Pin selekcji SPI */
#define DVR_1_SPI PB12   
#define DVR_2_SPI PA7 
#define DVR_3_SPI PC3 
/* Pin do wybudzania sterownika ? zobaczyc w datasheet */
#define DVR_1_WAKE PA12
#define DVR_2_WAKE PC9
#define DVR_3_WAKE PC14
/* Pin uruchomienia bramek i wew. systemów np pompy napiecia */
#define DVR_1_ENGATE PA11
#define DVR_2_ENGATE PB2
#define DVR_3_ENGATE PC15
/* Pin do sygnalizowania bledu */
#define DVR_1_NFAULT PA15
#define DVR_2_NFAULT PC4
#define DVR_3_NFAULT PA0
/* Piny PWM do sterowania silnikami */
// TIM 1
#define DVR_1_A PA10  // CH_3
#define DVR_1_B PA9   // CH 2
#define DVR_1_C PA8   // CH 1
// TIM 3 
#define DVR_2_A PC6   // CH 1
#define DVR_2_B PC7   // CH 2
#define DVR_2_C PC8   // CH 3
// TIM 2
#define DVR_3_A PA3   // CH 4
#define DVR_3_B PA2   // CH 3
#define DVR_3_C PA1   // CH 2

/*Create an instance of the SPI Class called SPI_2 that uses the 2nd SPI Port
  Using the second SPI port (SPI_2)
    SS    <-->  PB12 <-->  BOARD_SPI2_NSS_PIN
    SCK   <-->  PB13 <-->  BOARD_SPI2_SCK_PIN
    MISO  <-->  PB14 <-->  BOARD_SPI2_MISO_PIN
    MOSI  <-->  PB15 <-->  BOARD_SPI2_MOSI_PIN */
SPIClass SPI_2(2);
/* Zmienne do zwracania wartosci pzrez I2C */
word msg;
word data;

byte Func;
word Data;
float FData;
/* Zmienne do sterowania własnosciami Sygnału sterujacego silnikami */
//int PhaseDelay;
//int PhaseWidth;

bool AdcTest = false;
bool prog_01;
bool prog_02;
/* ------------------------------------------------------------------------------ //
// Definicje zmiennych i obiektów ----------------------------------------------- */
/* Definicja Timera, HardwareTimer nazwa(numer od 0 do 3 iz sa 4 timery) */
// Ustawienie timerow jest poprawne, takie jest podlaczenie na plytce
HardwareTimer DVR_1_Tim(1);
HardwareTimer DVR_2_Tim(3);
HardwareTimer DVR_3_Tim(2);


void TimerInit(int Prescaller, int Overflow) {
/* Pause the timer while we're configuring it */
  DVR_1_Tim.pause();
  DVR_2_Tim.pause();
  DVR_3_Tim.pause();
  /* Setup Timer channels modes
  // Konfiguracja Kanalow timera, kazdy timer ma 4 kanaly */
  DVR_1_Tim.setMode(TIMER_CH1, TIMER_PWM);
  DVR_1_Tim.setMode(TIMER_CH2, TIMER_PWM);
  DVR_1_Tim.setMode(TIMER_CH3, TIMER_PWM);
  
  DVR_2_Tim.setMode(TIMER_CH1, TIMER_PWM);
  DVR_2_Tim.setMode(TIMER_CH2, TIMER_PWM);
  DVR_2_Tim.setMode(TIMER_CH3, TIMER_PWM);
  
  DVR_3_Tim.setMode(TIMER_CH2, TIMER_PWM);
  DVR_3_Tim.setMode(TIMER_CH3, TIMER_PWM);
  DVR_3_Tim.setMode(TIMER_CH4, TIMER_PWM);
  // Pozostalosci po testowaniu
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
  /* Ustawienie aktualnie naliczonej wartosci */
  DVR_1_Tim.setCount(0);
  DVR_2_Tim.setCount(0);
  DVR_3_Tim.setCount(0);
  /* Zegar systemowy ma czestotliwosc 72Mhz, z taka czestotliwoscia 
  // tyka do Timera sygnał, trzeba go podzielic by robił to żadziej
  // 72 000 000 Hz / 720 = 100 000 Hz (czestotliwosc maksymalna sterownika / 2) 
  // 72MHz / 720 = 100 000Hz -> 10 microseconds
  // 72MHz / 360 = 200 000Hz -> 5 microseconds*/
  // 128Mhz / 
  DVR_1_Tim.setPrescaleFactor(Prescaller);
  DVR_2_Tim.setPrescaleFactor(Prescaller);
  DVR_3_Tim.setPrescaleFactor(Prescaller);
  /* Ustawienie maksymalnej ilosci */
  DVR_1_Tim.setOverflow(Overflow);
  DVR_2_Tim.setOverflow(Overflow);
  DVR_3_Tim.setOverflow(Overflow);
  // chyba jednak liczy sie to inaczej, czestotliwosc pwm jest zalezna od ilosci przelaczen a nie od czasu pomiedzy tickami ktore sa zliczane -> 
  // zalezy od czasu kiedy napelnienia Overflow do pelna i timer sie zresetuje chyba liczy sie wlasnie ta wartosc, jaka rozdzielczosc bedzie mial
  // sam nasz pw to nie wazne , sterownik sobie ze swoja rozdzielczoscia to jzu zinterpretuje.
  // Tak wiec wartosci trzeba dobrac tak ze przy przepelnieniu licznika i jego resecie to od startu mija 5us(200kHz) lub 10us(100kHz) 
  /* Ustawienie wstepnie wartosci do ktorej jak doliczy timer to przelaczy z stanu wysokiego na stan niski */
  DVR_1_Tim.setCompare(TIMER_CH1, 0);  // PA8 // PWM High Pulse width is when reach second value
  DVR_1_Tim.setCompare(TIMER_CH2, 0);  // PA9
  DVR_1_Tim.setCompare(TIMER_CH3, 0);  // PA10

  DVR_2_Tim.setCompare(TIMER_CH1, 0);  // PC6 // PWM High Pulse width is when reach second value
  DVR_2_Tim.setCompare(TIMER_CH2, 0);  // PC7
  DVR_2_Tim.setCompare(TIMER_CH3, 0);  // PC8

  DVR_3_Tim.setCompare(TIMER_CH2, 0);  // PA1 // PWM High Pulse width is when reach second value
  DVR_3_Tim.setCompare(TIMER_CH3, 0);  // PA2
  DVR_3_Tim.setCompare(TIMER_CH4, 0);  // PA3
  /* Aby tez zmiany zostaly wprowadzone trzeba zresetowac timer, sam by to zrobil po naliczeniu do wczesniej ustawionej wartosci */
  DVR_1_Tim.refresh();
  DVR_2_Tim.refresh();
  DVR_3_Tim.refresh();
}

void PinsInit() {
/* Konfiguracja pinow sterownikow */
  pinMode(DVR_1_WAKE, OUTPUT);
  pinMode(DVR_2_WAKE, OUTPUT);
  pinMode(DVR_3_WAKE, OUTPUT);
  
  pinMode(DVR_1_NFAULT, INPUT);
  pinMode(DVR_2_NFAULT, INPUT);
  pinMode(DVR_3_NFAULT, INPUT);
  
  pinMode(DVR_1_ENGATE, OUTPUT);
  pinMode(DVR_2_ENGATE, OUTPUT);
  pinMode(DVR_3_ENGATE, OUTPUT);
  
  pinMode(DVR_1_A, PWM);
  pinMode(DVR_1_B, PWM);
  pinMode(DVR_1_C, PWM);

  pinMode(DVR_2_A, PWM);
  pinMode(DVR_2_B, PWM);
  pinMode(DVR_2_C, PWM);

  pinMode(DVR_3_A, PWM);
  pinMode(DVR_3_B, PWM);
  pinMode(DVR_3_C, PWM);
  // Ledy
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_2, OUTPUT);

  /* Ustawienie pinow selekcji SPI do poszczegolnych procesorow */
  pinMode(DVR_1_SPI, OUTPUT);
  pinMode(DVR_2_SPI, OUTPUT);
  pinMode(DVR_3_SPI, OUTPUT);
  /* Ustawienie na HIGH sprawia ze */
  digitalWrite(DVR_1_SPI, HIGH);
  digitalWrite(DVR_2_SPI, HIGH);
  digitalWrite(DVR_3_SPI, HIGH);
}

void ToggleLed(int Num) {
  if (Num == 1) digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
  else if (Num == 2) digitalWrite(LED_2, !digitalRead(LED_2));
  //digitalWrite(LED_2, !digitalRead(LED_2)); 
}

word DVR_SPI_Send(int Num, word msg) {
  if      (Num == 1) digitalWrite(DVR_1_SPI, LOW); // manually take CSN low for SPI_2 transmission
  else if (Num == 2) digitalWrite(DVR_2_SPI, LOW);
  else if (Num == 3) digitalWrite(DVR_3_SPI, LOW);
  
  data = SPI_2.transfer16(msg); //Send the HEX data 0x55 over SPI-2 port and store the received byte to the <data> variable.
  
  digitalWrite(DVR_1_SPI, HIGH); // manually take CSN high between spi transmissions
  digitalWrite(DVR_2_SPI, HIGH);
  digitalWrite(DVR_3_SPI, HIGH);

  return data;
}

void Motor_Stop(int Num) {
  if (Num == 1 || Num == 0) {
    digitalWrite(DVR_1_ENGATE, LOW); // Disable gates
    digitalWrite(DVR_1_A, LOW);
    digitalWrite(DVR_1_B, LOW); 
    digitalWrite(DVR_1_C, LOW); 
    DVR_1_Tim.setCompare(TIMER_CH1, 0);  // PA8 // PWM High Pulse width is when reach second value
    DVR_1_Tim.setCompare(TIMER_CH2, 0);  // PA9
    DVR_1_Tim.setCompare(TIMER_CH3, 0);  // PA10
    DVR_1_Tim.pause();
  }
  if (Num == 2 || Num == 0) {
    digitalWrite(DVR_2_ENGATE, LOW); // Disable gates
    digitalWrite(DVR_2_A, LOW);
    digitalWrite(DVR_2_A, LOW); 
    digitalWrite(DVR_2_A, LOW);
    DVR_2_Tim.setCompare(TIMER_CH1, 0);  // PC6 // PWM High Pulse width is when reach second value
    DVR_2_Tim.setCompare(TIMER_CH2, 0);  // PC7
    DVR_2_Tim.setCompare(TIMER_CH3, 0);  // PC8
    DVR_2_Tim.pause();
  }
  if (Num == 3 || Num == 0) {
    digitalWrite(DVR_3_ENGATE, LOW); // Disable gates
    digitalWrite(DVR_3_A, LOW);
    digitalWrite(DVR_3_A, LOW); 
    digitalWrite(DVR_3_A, LOW);
    DVR_3_Tim.setCompare(TIMER_CH2, 0);  // PA1 // PWM High Pulse width is when reach second value
    DVR_3_Tim.setCompare(TIMER_CH3, 0);  // PA2
    DVR_3_Tim.setCompare(TIMER_CH4, 0);  // PA3
    DVR_3_Tim.pause(); 
  }
}

void Motor_Enable(int Num) {
  if (Num == 1 || Num == 0) {
    digitalWrite(DVR_1_ENGATE, HIGH); // Enable gates
    DVR_1_Tim.setCompare(TIMER_CH1, 0);  // PA8 // PWM High Pulse width is when reach second value
    DVR_1_Tim.setCompare(TIMER_CH2, 0);  // PA9
    DVR_1_Tim.setCompare(TIMER_CH3, 0);  // PA10
    DVR_1_Tim.refresh();
    DVR_1_Tim.resume(); 
  }
  if (Num == 2 || Num == 0) {
    digitalWrite(DVR_2_ENGATE, HIGH); // Enable gates
    DVR_2_Tim.setCompare(TIMER_CH1, 0);  // PC6 // PWM High Pulse width is when reach second value
    DVR_2_Tim.setCompare(TIMER_CH2, 0);  // PC7
    DVR_2_Tim.setCompare(TIMER_CH3, 0);  // PC8
    DVR_2_Tim.refresh();
    DVR_2_Tim.resume();
  }
  if (Num == 3 || Num == 0) {
    digitalWrite(DVR_3_ENGATE, HIGH); // Enable gates
    DVR_3_Tim.setCompare(TIMER_CH2, 0);  // PA1 // PWM High Pulse width is when reach second value
    DVR_3_Tim.setCompare(TIMER_CH3, 0);  // PA2
    DVR_3_Tim.setCompare(TIMER_CH4, 0);  // PA3
    DVR_3_Tim.refresh();
    DVR_3_Tim.resume();
  }
  delay(1); // Wait for voltage pomp (need 1ms)
}

void Motor_1_A(int PhaseWidth) { DVR_1_Tim.setCompare(TIMER_CH3, PhaseWidth); }
void Motor_1_B(int PhaseWidth) { DVR_1_Tim.setCompare(TIMER_CH2, PhaseWidth); }
void Motor_1_C(int PhaseWidth) { DVR_1_Tim.setCompare(TIMER_CH1, PhaseWidth); }

void Motor_2_A(int PhaseWidth) { DVR_2_Tim.setCompare(TIMER_CH1, PhaseWidth); }
void Motor_2_B(int PhaseWidth) { DVR_2_Tim.setCompare(TIMER_CH2, PhaseWidth); }
void Motor_2_C(int PhaseWidth) { DVR_2_Tim.setCompare(TIMER_CH3, PhaseWidth); }

void Motor_3_A(int PhaseWidth) { DVR_3_Tim.setCompare(TIMER_CH4, PhaseWidth); }
void Motor_3_B(int PhaseWidth) { DVR_3_Tim.setCompare(TIMER_CH3, PhaseWidth); }
void Motor_3_C(int PhaseWidth) { DVR_3_Tim.setCompare(TIMER_CH2, PhaseWidth); }

/* ------------------------------------------------------------------------------ //
 *  Zmienne definiowane na potzreby sterowania DSinusem */
#define PI 3.1415926535897932384626433832795
float Fp = 10000;        // Czestotliwosc probkowania, czestotliwosc przelaczania
unsigned int DelayValue;           // Wartoc pomiedzy probkowaniem

struct Motor {
  // Tryb pracy silnika:
  // 0 -> Wyłączony
  // 1 -> Włączony, tryb testowy
  int Function;           // Tryb działania
  bool IsRunning;
  int Num;                 // Numer silnika
  
  float FSin;             // Czestotliwosc Sinusa
  float FTr;              // Czestotliwosc Trojkata

  float StepGain;         // Wartosc w radianach co probkowanie
  long AStepSin;          // aktualny krok fazy A
  long BStepSin;          // aktualny krok fazy B
  long CStepSin;          // aktualny krok fazy C
  long StepTr;            // aktualny krok trojkata
  double ASin, BSin, CSin, Tr;        // Wartosc 
  float StepsPPSin, StepPPTr; // Ilosc probkowan na caly okres
  bool Out;             // Wyjscie
  // Obliczenia co kazda zmiane
  float StepsPPSin_3;
  float StepsPPSin_6;
  float StepsPPSin_23;
  float StepPPTr_4;
  float StepPPTr_2;
  float StepPPTr_34;
  float StepPPTr__2;

  int PhaseWidth;
  bool Forward = true;

  Motor(int num) {
    Num = num;
  }
  
  void Enable(int Func) {
    Function = Func;       // Zmiana trybu na włączony
    IsRunning = false;     // Potrzebne dla FSin = 0
    Motor_Enable(Num);
    // DSinus
    CStepSin = 0;
    StepTr = 0;
    PhaseWidth = 0;
  }

  void Disable() {
    Motor_Stop(Num);
    IsRunning = false;
    Function = 0;
  }

  void UpdateSin() {
    // Zabezpieczenie na wypadek dzielenia przez 0, lub zwykle zatrzymanie silnika
    if (FSin == 0) {
      IsRunning = false;         // Zatrzymanie silnika 
      return;
    }
    // Standardowa czestotliosc trojkata
    if (FTr == 0) FTr = FSin * 90;
    // Obliczenia co kazda zmiane
    DelayValue = 1000000/Fp;
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
    if (IsRunning == false && Function != 0) IsRunning = true;    // Uruchomienie silnika w przypadku wczesniejszego zatrzymania
  }
  
  void SetSpeed(float Sin, float Tr, int Power) {
    FSin = Sin;           // Czestotliwosc Sinusa
    FTr = Tr;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
    // Obliczenia co kazda zmiane
    UpdateSin();
    // rozszezenie o PWM
    PhaseWidth = Power;
  }

  void SetSpeed(float Sin, float Tr) {
    FSin = Sin;           // Czestotliwosc Sinusa
    FTr = Tr;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
    // Obliczenia co kazda zmiane
    UpdateSin();
  }

  void SetSpeed(float Sin) {
    FSin = Sin;           // Czestotliwosc Sinusa
    FTr = Tr;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
    // Obliczenia co kazda zmiane
    UpdateSin();
  }

  void SetPower(int power) {
    PhaseWidth = power;
  }

  void SetDirection(bool dir) {
    Forward = dir;
  }

  bool Update() {
    // Funkcja pierwsza
    if (Function == 1) {   
      if (IsRunning == true) {    
        CStepSin += 1;                       // Inkrementacja kroku
        BStepSin = CStepSin + StepsPPSin_3;  // Przesuniecie o 1/3
        AStepSin = BStepSin + StepsPPSin_3;  // Przesuniecie o kolejna 1/3
        StepTr += 1;
    
        if (CStepSin >= StepsPPSin) {CStepSin = 0;StepTr = 0;ToggleLed(1);  }
        if (BStepSin >= StepsPPSin) BStepSin -= StepsPPSin;
        if (AStepSin >= StepsPPSin) AStepSin -= StepsPPSin;
        if (StepTr >= StepPPTr) StepTr = 0;
        
        // Wartosc Trojkata
        if(StepTr<=StepPPTr_4){Tr=(StepTr*StepPPTr_2+0.5);}else if(StepTr<=StepPPTr_34){Tr=(StepTr*StepPPTr__2+1.5);}else{Tr=(StepTr*StepPPTr_2-1.5);}
    
        // Wartosc DSinusa A
        if (AStepSin<=StepsPPSin_3){ASin=sin(AStepSin*StepGain);}else if(AStepSin<=StepsPPSin_23){ASin=sin((AStepSin-StepsPPSin_6)*StepGain);}else{ASin=0;}          
        // Wartosc DSinusa B
        if (BStepSin<=StepsPPSin_3){BSin=sin(BStepSin*StepGain);}else if(BStepSin<=StepsPPSin_23){BSin=sin((BStepSin-StepsPPSin_6)*StepGain);}else{BSin=0;}  
        // Wartosc DSinusa C
        if (CStepSin<=StepsPPSin_3){CSin=sin(CStepSin*StepGain);}else if(CStepSin<=StepsPPSin_23){CSin=sin((CStepSin-StepsPPSin_6)*StepGain);}else{CSin=0;}  

        switch (Num) {
          case 1: {
            if (Forward) {if(Tr < ASin) Motor_1_A(PhaseWidth); else Motor_1_A(0);} else {if(Tr < ASin) Motor_1_C(PhaseWidth); else Motor_1_C(0);}
            if(Tr < BSin) Motor_1_B(PhaseWidth); else Motor_1_B(0);
            if (Forward) {if(Tr < CSin) Motor_1_C(PhaseWidth); else Motor_1_C(0);} else {if(Tr < CSin) Motor_1_A(PhaseWidth); else Motor_1_A(0);}
            break;
          }
          case 2: {
            if (Forward) {if(Tr < ASin) Motor_2_A(PhaseWidth); else Motor_2_A(0);} else {if(Tr < ASin) Motor_2_C(PhaseWidth); else Motor_2_C(0);}
            if(Tr < BSin) Motor_2_B(PhaseWidth); else Motor_2_B(0);
            if (Forward) {if(Tr < CSin) Motor_2_C(PhaseWidth); else Motor_2_C(0);} else {if(Tr < CSin) Motor_2_A(PhaseWidth); else Motor_2_A(0);}
            break;
          }
          case 3: {
            if (Forward) {if(Tr < ASin) Motor_3_A(PhaseWidth); else Motor_3_A(0);} else {if(Tr < ASin) Motor_3_C(PhaseWidth); else Motor_3_C(0);}
            if(Tr < BSin) Motor_3_B(PhaseWidth); else Motor_3_B(0);
            if (Forward) {if(Tr < CSin) Motor_3_C(PhaseWidth); else Motor_3_C(0);} else {if(Tr < CSin) Motor_3_A(PhaseWidth); else Motor_3_A(0);}
            break;
          }
        }
        //delayMicroseconds(DelayValue);
        //digitalWrite(LED_BUILTIN, HIGH);  
      } else {
        switch (Num) {
          case 1: {
            Motor_1_A(0);
            Motor_1_B(0);
            Motor_1_C(0);
            break;
          }
          case 2: {
            Motor_2_A(0);
            Motor_2_B(0);
            Motor_2_C(0);
            break;
          }
          case 3: {
            Motor_3_C(0);
            Motor_3_B(0);
            Motor_3_A(0);
            break;
          }
        }
        // W przypadku zatrzymania silnika  

      }
      return true;
    } else return false;    
  }
};

Motor Motor_1(1);
Motor Motor_2(2);
Motor Motor_3(3);

void setup() {
  /* Pewne zeczy sie robia i definiuja same, piny ktore potem trzeba uzyc
  // sa zdefiniowane przez biblioteke obsługi UART czy usb, musimy ja wiec zatrzymac
  // Serial https://www.arduino.cc/reference/tr/language/functions/communication/serial/ */
  Serial.end();
  PinsInit();
  TimerInit(512, 31);
  /* Konfiguracja I2C, Wire.begin(Slave addres), jak poda sie adres slave
  // wtedy jestes slave o tym adresie */
  Wire.begin(4); 
  /* Jako slave trzeba przypisac funkcje ktore wywalaja sie w przypadku
  // przerwania rzadania odpowiedzi lub gdy dostaniemy jakies dane 
  // Set on receive and reqest events */
  Wire.onReceive(receiveEvent); // register event  
  Wire.onRequest(requestEvent); // register event 
  /* Konfiguracja drogiej magistrali I2C do odczytu z ADC, funkcja 
  // Wire1.setClock(400000); zmieniac mozna czestotliwosc zegara */
  Wire1.begin();    
  /* Konfiguracja SPI, na poczatku inicjalizacja */
  SPI_2.begin(); //Initialize the SPI_2 port.
  SPI_2.setBitOrder(MSBFIRST); // Set the SPI_2 bit order
  SPI_2.setDataMode(SPI_MODE1); //Set the  SPI_2 data mode 1
  SPI_2.setClockDivider(SPI_CLOCK_DIV16);  // Use a different speed to SPI 1

  // Inicjalizacja 3PWM
  msg = 0b011101010010110;
  while (DVR_SPI_Send(1, msg) == 0) {};
  while (DVR_SPI_Send(2, msg) == 0) {};
  while (DVR_SPI_Send(3, msg) == 0) {};
  
  prog_01 = false;
  prog_02 = false;
  /* Jak ktos cos potrzebuje czas na uporzadkowanie po konfiguracji */
  
  delay(1);
  
}
#define SET_MOTOR_1_SPEED_FORWAD    64   // 0
#define SET_MOTOR_1_SPEED_BACK      65   // 1
#define SET_MOTOR_2_SPEED_FORWAD    66   // 2
#define SET_MOTOR_2_SPEED_BACK      67   // 3
#define SET_MOTOR_3_SPEED_FORWAD    68   // 4
#define SET_MOTOR_3_SPEED_BACK      69   // 5

#define SET_MOTOR_1_POWER           70   // 6
#define SET_MOTOR_2_POWER           71   // 7
#define SET_MOTOR_3_POWER           72   // 8

#define MOTOR_1_ENABLE              73   // 9
#define MOTOR_2_ENABLE              74   // 10
#define MOTOR_3_ENABLE              75   // 11

#define MOTOR_1_DISABLE             76   // 12
#define MOTOR_2_DISABLE             77   // 13
#define MOTOR_3_DISABLE             78   // 14

/* Funkcja ktora sluzy do przekazania informacji ktore zapewnie mostek wczesniej zlecil do uzyskania,
// Teraz rzada by je wysłać, jest to przerwanie wiec trzeba wiedziec ile czasu zajmie nam uzyskanie danych
// w pzreciwnym wypadku funkcja sie wywola zanim skonczylismy i dostaniemy jakies wczesniej definiowane wartosci */
void requestEvent()
{
  
  /* Dioda sie zmienia by sygnalizowac poprawnosc dzialania programu 
  ToggleLed(1); 
  Wire.write(byte(msg >> 8)); // send 8 left 8 bits of msg
  Wire.write(byte(msg)); // send 8 right bits of msg
  Wire.write(byte(data >> 8)); // send 8 left 8 bits of data
  Wire.write(byte(data)); // send 8 right bits of data*/
  //Wire.write(Data);
  Wire.write(Func + 1);
}
void receiveEvent(int howMany){
  
  if(Wire.available()){
    Func = (byte)Wire.read(); // receive byte as a character
    if(Wire.available()){
      
      Data = ((word)Wire.read()) << 8 ; // receive byte as a character
      Data += ((word)Wire.read());
      // TURBO ISTOTNE, WARTIOSCI PREDKOSCI SA DZIELONE PRZEZ 10 A MOC JUZ NIE !!!
      FData = (float)Data/10;
      // Pierwszy bajt to funkcja a drogi to dane 
      switch (Func){
        case SET_MOTOR_1_SPEED_FORWAD: {
          Motor_1.SetDirection(true);
          Motor_1.SetSpeed(FData, FData * 90);
          break;
        }
        case SET_MOTOR_1_SPEED_BACK: {
          Motor_1.SetDirection(false);
          Motor_1.SetSpeed(FData, FData * 90);
          break;
        }
        case SET_MOTOR_2_SPEED_FORWAD: {
          Motor_2.SetDirection(true);
          Motor_2.SetSpeed(FData, FData * 90);
          break;
        }
        case SET_MOTOR_2_SPEED_BACK: {
          Motor_2.SetDirection(false);
          Motor_2.SetSpeed(FData, FData * 90);
          break;
        }
        case SET_MOTOR_3_SPEED_FORWAD: {
          Motor_3.SetDirection(true);
          Motor_3.SetSpeed(FData, FData * 90);
          break;
        }
        case SET_MOTOR_3_SPEED_BACK: {
          Motor_3.SetDirection(false);
          Motor_3.SetSpeed(FData, FData * 90);
          break;
        }
        case SET_MOTOR_1_POWER: {
          Motor_1.SetPower(Data);
          break;
        }
        case SET_MOTOR_2_POWER: {
          Motor_2.SetPower(Data);
          break;
        }
        case SET_MOTOR_3_POWER: {
          Motor_3.SetPower(Data);
          break;
        }
        case MOTOR_1_ENABLE: {
          Motor_1.Enable(1);
          break;
        }
        case MOTOR_2_ENABLE: {
          Motor_2.Enable(1);
          break;
        }
        case MOTOR_3_ENABLE: {
          Motor_3.Enable(1);
          break;
        }     
        case MOTOR_1_DISABLE: {
          ToggleLed(1); 
          Motor_1.Disable();
          break;
        }
        case MOTOR_2_DISABLE: {
          ToggleLed(1); 
          Motor_2.Disable();
          break;
        }
        case MOTOR_3_DISABLE: {
          ToggleLed(1); 
          Motor_3.Disable();
          break;
        }     
        default: {
          if ((Func == 80) and (Data == 65535)) ToggleLed(2);
          break;         
        }
      }
    }
  }
  
}
/* Funkcja ktora wywola sie gdy dostaniemy jakas informacje.
// Dokladnie gdy komunikacja sie rozpocznie i bajty beda przesylane jeden za drogim w jednym ciagu. Gdyby bajty wysylac osobno 
// za kazdym razem konczac komunikacje to co bajt by sie ta funkcja wywolywala */
void OldreceiveEvent(int howMany){
  char one, two;
  /* Sprawdzamy czy jakis bajty dostalismy, dziwne ale tak sie robi bo to dobra jest tradycja */
  if(Wire.available())one = Wire.read(); // receive byte as a character
  /* Sprawdzamy czy dostalismy jeszcze drogi bajt, generalnie musze zrobic lepszy sposob na ropoznawanie co sterownik od nas chce
  // Zamiast tego switcha pozniej, komunaikacja musi byc jak najszybsza i najbardziej upakowana */
  if(Wire.available()){
    /* Zorbione ejst tak ze jezeli dstajemy 2 Bajty to wiemy ze mostek chce bysmy wysłali oba bajty do sterownika dalej przez SPI
    // Ze przez spi mozna wyslac 2 bajty razem jako word a nie 2 chary osobno to pakujemy je razem
    // i wysyłamy, wykonamy w ten sposob jakis zapis lub odczyt w rejestrze, zalezy co nam mostek zapoda.
    // JEST TO TYLKO DO TESTOWANIA, MOSTEK BEDZIE KAZAL NP. SIE ZINICJALIZOWAC, ZMIENIC COS A TEN STEROWNIK MA SAM WIEDZIEC
    // JAKIE WYSLAC BAJTY DO JAKICH REJESTROW */
    ToggleLed(2);
    /* Jezeli jest drogi bajt to znaczy ze to komenda dalej a nie dala samego sterownika */
    two = Wire.read(); // receive byte as a character
    msg = word(one << 8) + two;
    DVR_SPI_Send(3, msg);
  /* Jezeli ejden Bajt to pewnie jakas komenda  */  
  } else {
    // Komenda sterownika
    switch (one) {
      case '1': {
        ToggleLed(1);  
        Motor_1.Enable(1);
        Motor_1.SetSpeed(1, 90, 1);
        break;
      }
      case '2': {
        Motor_1.Disable();
        break;
      }
      case '3': {
        ToggleLed(1);  
        Motor_2.Enable(1); 
        Motor_2.SetSpeed(2, 90, 1);
        break;
      }
      case '4': {
        Motor_2.Disable();
        break;
      }
      case '5': {
        ToggleLed(1);  
        Motor_3.Enable(1); 
        Motor_3.SetSpeed(3, 90, 1);
        break;
      }
      case '6': {   
        Motor_3.Disable();
        break;
      }
      case '7': {   
        // Zwiekszenie obrotow

        break;
      }
      case '8': {     
        // Zmniejszenie obrotow    
        break;
      }
      /*
      case 'A': {
        // Teraz to sluzy do kontroli trybu w programie 2 a nie jest oddzielnym programem
        prog_01 = true;
        digitalWrite(PA11, HIGH); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        ToggleLed(1); 
        break;
      }
      case 'B': {
        prog_01 = false;
        //digitalWrite(PA11, LOW); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        ToggleLed(1);
        break;
      }
      case 'C': {
        prog_02 = true;
        PhaseDelay = 10000;
        PhaseWidth = 16;
        digitalWrite(PA11, HIGH); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  

        DVR_1_Tim.refresh();
        DVR_1_Tim.resume();
        break;
      }
      case 'D': {
        prog_02 = false;
        DVR_1_Tim.setCompare(TIMER_CH1, 0);  // PA8 // PWM High Pulse width is when reach second value
        DVR_1_Tim.setCompare(TIMER_CH2, 0);  // PA9
        DVR_1_Tim.setCompare(TIMER_CH3, 0);  // PA10
        //delay(100);
        digitalWrite(PA11, LOW); // Enable gates
        delay(1); // Wait for voltage pomp (need 1ms)
        //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));  
        DVR_1_Tim.pause();
        break;
      }
      ////////////////////////
      case 'i': {
          PhaseWidth += 1;
          ToggleLed(2);  
      break;
      }*/
      case 'x': {
        AdcTest = true;
        break;
      }
    }
  }
}

void loop() {
  Motor_1.Update();
  Motor_2.Update();
  Motor_3.Update();

  if (false) {
   for (word a = 0; a <= 100000; a++) {
      delayMicroseconds(10);
    }
    ToggleLed(1);
  }
  /* Program do odczytu czegos z rejestru zewnetrznego pzretwornika ADC, poblem jest taki ze nie mozna go wykonac w funkcji OnRecive bo ona sie wykonuje podczas obslugi I2C_1,
  // a do adc komunikujemy sie po I2C_2, procek doznaje jakieos bledu wtedy, wiec przez I2C_1 dostajemy info ze cos chcemu z ADC, potem w normalnej petli programu obslugujemy I2C_2,
  // i po jakims czasie mostek sobie wywola Request i odczyta bity zwrotne z adc. Zazwyczaj tak jest ze przy zapisie do rejestru jak i odczycie dostajemy wiadomosc zwrotna tego co tam jest,
  // przez spi tez tak to dziala,
  // PROGRAM TEZ DO TESTOW, SAMI PODCZAS STEROWANIA SILNIKAMI BEDZIEMY SPRAWDZAC DANE ZE SILNIKOW, 
  // ACZ GDY FAZA JEST PODLACZONA DO NAPIECIA WTEDY MOZEMY SPRAWDZAC NAPIECIE I TO NIEMAL BEZPOSREDNIE NAPIECEI NA BATERJI,
  // NAPIECIE NA BATERJI EJST JUZ ISTOTNE DLA MOSTKA*/
  if (AdcTest == true) {
        // 1001 011x -> SCL
        // 1001 0111 -> Odczyt -> 97
        // 1001 0111 -> Zapis -> 96
        // To co powyzej nie aktualne dla nas poniewrz podajemy i tak 7 bitowy adres a funkcje jakie wywolamy same ten 8 daja jaki trzeba
          data = 1; // Adres rejestru
          Wire1.beginTransmission(0b1001011); //7-bit deviceAddress
          Wire1.write(byte(1));       //registerAddress
          data = Wire1.endTransmission();
          ToggleLed(2);
          msg = 1;
          //delay(1);
          Wire1.requestFrom(0b1001011, 2);
          //delay(1);
          if (2 <= Wire1.available()) { // if two bytes were received
            msg = Wire1.read();  // receive high byte (overwrites previous reading)
            msg = msg << 8;    // shift high byte to be high 8 bits
            msg |= Wire1.read(); // receive low byte as lower 8 bits
          } else ToggleLed(1);  
          AdcTest = false;
  ///////////////////////////////////////////////////////////////////////    
  }
}
