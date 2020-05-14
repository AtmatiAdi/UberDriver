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
 *  Zmienne definiowane na potzreby sterowania DSinusem */
#define PI 3.1415926535897932384626433832795
float Fp = 100000;        // Czestotliwosc probkowania, czestotliwosc przelaczania
unsigned int DelayValue;           // Wartoc pomiedzy probkowaniem


  // Tryb pracy silnika:
  // 0 -> Wyłączony
  // 1 -> Włączony, tryb testowy
  int Function;           // Tryb działania
  
  float FSin;             // Czestotliwosc Sinusa
  float FTr;              // Czestotliwosc Trojkata

  float StepGain;         // Wartosc w radianach co probkowanie
  long AStepSin;          // aktualny krok fazy A
  long BStepSin;          // aktualny krok fazy B
  long CStepSin;          // aktualny krok fazy C
  long StepTr;            // aktualny krok trojkata
  double DSin, Tr;        // Wartosc 
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

  bool Test_Sin = false;
  int Motor_1 = 0;
  int Motor_2 = 0;
  int Motor_3 = 0;

/* ------------------------------------------------------------------------------ //
// Definicje zmiennych i obiektów ----------------------------------------------- */
/* Definicja Timera, HardwareTimer nazwa(numer od 0 do 3 iz sa 4 timery) */
// Ustawienie timerow jest poprawne, takie jest podlaczenie na plytce
HardwareTimer DVR_1_Tim(1);
HardwareTimer DVR_2_Tim(3);
HardwareTimer DVR_3_Tim(2);
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
/* Zmienne do sterowania własnosciami Sygnału sterujacego silnikami */
int PhaseDelay;
int PhaseWidth;

bool AdcTest = false;
bool prog_01;
bool prog_02;

void setup() {
  /* Pewne zeczy sie robia i definiuja same, piny ktore potem trzeba uzyc
  // sa zdefiniowane przez biblioteke obsługi UART czy usb, musimy ja wiec zatrzymac
  // Serial https://www.arduino.cc/reference/tr/language/functions/communication/serial/ */
  Serial.end();
  PinsInit();
  TimerInit(300, 15);
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

/* Funkcja ktora sluzy do przekazania informacji ktore zapewnie mostek wczesniej zlecil do uzyskania,
// Teraz rzada by je wysłać, jest to przerwanie wiec trzeba wiedziec ile czasu zajmie nam uzyskanie danych
// w pzreciwnym wypadku funkcja sie wywola zanim skonczylismy i dostaniemy jakies wczesniej definiowane wartosci */
void requestEvent()
{
  /* Dioda sie zmienia by sygnalizowac poprawnosc dzialania programu */
  ToggleLed(1); 
  Wire.write(byte(msg >> 8)); // send 8 left 8 bits of msg
  Wire.write(byte(msg)); // send 8 right bits of msg
  Wire.write(byte(data >> 8)); // send 8 left 8 bits of data
  Wire.write(byte(data)); // send 8 right bits of data
}

/* Funkcja ktora wywola sie gdy dostaniemy jakas informacje.
// Dokladnie gdy komunikacja sie rozpocznie i bajty beda przesylane jeden za drogim w jednym ciagu. Gdyby bajty wysylac osobno 
// za kazdym razem konczac komunikacje to co bajt by sie ta funkcja wywolywala */
void receiveEvent(int howMany){
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
        Test_Sin = true;
        Motor_1 = 1;
        FSin = 3;           // Czestotliwosc Sinusa
        FTr = FSin * 300;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
        Fp = 10000;        // Czestotliwosc probkowania, czestotliwosc przelaczania
        // DSinus
        DelayValue = 1000000/Fp;
        CStepSin = 0;
        StepTr = 0;
        // Obliczenia co kazda zmiane
        UpdateSin();
        // rozszezenie o PWM
        PhaseWidth = 1;
        Motor_Enable(1);
        ToggleLed(1);   
        break;
      }
      case '2': {
        Motor_1 = 0;
        Motor_Stop(1);
        Test_Sin = false;
        break;
      }
      case '3': {
        Test_Sin = true;
        Motor_2 = 1;
        FSin = 3;           // Czestotliwosc Sinusa
        FTr = FSin * 300;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
        Fp = 10000;        // Czestotliwosc probkowania, czestotliwosc przelaczania
        // DSinus
        DelayValue = 1000000/Fp;
        CStepSin = 0;
        StepTr = 0;
        // Obliczenia co kazda zmiane
        UpdateSin();
        // rozszezenie o PWM
        PhaseWidth = 1;
        Motor_Enable(2);
        ToggleLed(1);   
        break;
      }
      case '4': {
        Motor_2 = 0;
        Motor_Stop(2);
        Test_Sin = false;
        break;
      }
      case '5': {
        Test_Sin = true;
        Motor_3 = 1;
        FSin = 3;           // Czestotliwosc Sinusa
        FTr = FSin * 300;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
        Fp = 10000;        // Czestotliwosc probkowania, czestotliwosc przelaczania
        // DSinus
        DelayValue = 1000000/Fp;
        CStepSin = 0;
        StepTr = 0;
        // Obliczenia co kazda zmiane
        UpdateSin();
        // rozszezenie o PWM
        PhaseWidth = 1;
        Motor_Enable(3);
        ToggleLed(1);  
        break;
      }
      case '6': {   
        Motor_3 = 0;
        Motor_Stop(3);
        Test_Sin = false;
        break;
      }
      case '7': {   
        // Zwiekszenie obrotow
        FSin += 1;
        UpdateSin();
        break;
      }
      case '8': {     
        // Zmniejszenie obrotow    
        if(FSin > 1) FSin -= 1;
        UpdateSin();
        break;
      }
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
      case 'c': {
          PhaseDelay += 100;
          ToggleLed(2);
        break;
      }
      case 'd': {
          PhaseDelay -= 100;
          if (PhaseDelay < 1) PhaseDelay = 1;
          ToggleLed(2);
        break;
      }
      case 'e': {
          PhaseDelay += 10;
          ToggleLed(2);
        break;
      }
      case 'f': {
          PhaseDelay -= 10;
          if (PhaseWidth < 0) PhaseWidth = 0;
          ToggleLed(2);
        break;
      }
      case 'g': {
          PhaseDelay += 1;
          ToggleLed(2);
        break;
      }
      case 'h': {
          PhaseDelay -= 1;
          if (PhaseWidth < 0) PhaseWidth = 0;
          ToggleLed(2); 
        break;
      }
      ////////////////////////
      case 'i': {
          PhaseWidth += 1;
          ToggleLed(2);  
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

void UpdateSin() {
  FTr = FSin * 90;    // Czestotliwosc Trojkata, powinna byc 3*n razy wiekszy od sinusa, COUNTER JEST RESETOWANY WRAZ Z SINUSEM WIEC OKRESY PELNE MUSZA BYC W OKRESIE SINUSA
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

void loop() {
   
  // put your main code here, to run repeatedly:
  //delay(100);
  //digitalWrite(LED_BUILTIN, s);   
  //s = !s;
  /* Program do sterowania silnikiem, daje rade ale trzeba bedzie go rozwinac o odczyt napiecia na zwojach */
  if (prog_02 == true) {
    
        DVR_1_Tim.setCompare(TIMER_CH1, PhaseWidth);  // PA8 
        DVR_1_Tim.setCompare(TIMER_CH2, PhaseWidth);  // PA9
        DVR_1_Tim.setCompare(TIMER_CH3, 0);  // PA10
        delayMicroseconds(PhaseDelay);
        DVR_1_Tim.setCompare(TIMER_CH1, 0);  // PA8 
        DVR_1_Tim.setCompare(TIMER_CH2, PhaseWidth);  // PA9
        DVR_1_Tim.setCompare(TIMER_CH3, PhaseWidth);  // PA10
        delayMicroseconds(PhaseDelay);
        DVR_1_Tim.setCompare(TIMER_CH1, PhaseWidth);  // PA8 
        DVR_1_Tim.setCompare(TIMER_CH2, 0);  // PA9
        DVR_1_Tim.setCompare(TIMER_CH3, PhaseWidth);  // PA10
        delayMicroseconds(PhaseDelay);
        if (prog_01 == true) {
          if (PhaseDelay > 3000) PhaseDelay -= 100;
          if (PhaseDelay > 2000) PhaseDelay -= 10;
          //if (PhaseDelay > 1000) PhaseDelay -= 5;
          //if (PhaseDelay > 250) PhaseDelay -= 1;
        }
       
  }else if (Test_Sin) {
    //ToggleLed();
// DSINUS ///////////////////////////////////////////////////////////////////
    while(Test_Sin) {
      //StepPPTr = Fp/FTr;
      //StepsPPSin = Fp/FSin;
      
      CStepSin += 1;                       // Inkrementacja kroku
      BStepSin = CStepSin + StepsPPSin_3;  // Przesuniecie o 1/3
      AStepSin = BStepSin + StepsPPSin_3;  // Przesuniecie o kolejna 1/3
      StepTr += 1;
  
      if (CStepSin >= StepsPPSin) {CStepSin = 0;StepTr = 0;ToggleLed(1);  }
      if (BStepSin >= StepsPPSin) BStepSin -= StepsPPSin;
      if (AStepSin >= StepsPPSin) AStepSin -= StepsPPSin;
      if (StepTr >= StepPPTr) StepTr = 0;
      
      //StepGain = FSin/Fp * 2 * PI;
      
      // Wartosc Trojkata
      if(StepTr<=StepPPTr_4){Tr=(StepTr*StepPPTr_2+0.5);}else if(StepTr<=StepPPTr_34){Tr=(StepTr*StepPPTr__2+1.5);}else{Tr=(StepTr*StepPPTr_2-1.5);}
  
      // Wartosc DSinusa A
      if (AStepSin<=StepsPPSin_3){DSin=sin(AStepSin*StepGain);}else if(AStepSin<=StepsPPSin_23){DSin=sin((AStepSin-StepsPPSin_6)*StepGain);}else{DSin=0;}  
      //if(Tr < DSin) digitalWrite(DVR_1_A, HIGH); else digitalWrite(DVR_1_A, LOW);
      if(Tr < DSin) Motor_3_A(PhaseWidth); else Motor_3_A(0);
      
      // Wartosc DSinusa B
      if (BStepSin<=StepsPPSin_3){DSin=sin(BStepSin*StepGain);}else if(BStepSin<=StepsPPSin_23){DSin=sin((BStepSin-StepsPPSin_6)*StepGain);}else{DSin=0;}  
      //if(Tr < DSin) digitalWrite(DVR_1_B, HIGH); else digitalWrite(DVR_1_B, LOW);
      if(Tr < DSin) Motor_3_B(PhaseWidth); else Motor_3_B(0);
      
      // Wartosc DSinusa C
      if (CStepSin<=StepsPPSin_3){DSin=sin(CStepSin*StepGain);}else if(CStepSin<=StepsPPSin_23){DSin=sin((CStepSin-StepsPPSin_6)*StepGain);}else{DSin=0;}  
      //if(Tr < DSin) digitalWrite(DVR_1_C, HIGH); else digitalWrite(DVR_1_C, LOW);
      if(Tr < DSin) Motor_3_C(PhaseWidth); else Motor_3_C(0);
      //delayMicroseconds(DelayValue);
      //digitalWrite(LED_BUILTIN, HIGH);  
    }
  } else {
    Motor_Stop(0);
        
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
