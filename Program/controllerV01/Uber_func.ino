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
    delay(1); // Wait for voltage pomp (need 1ms)
    DVR_1_Tim.setCompare(TIMER_CH1, 0);  // PA8 // PWM High Pulse width is when reach second value
    DVR_1_Tim.setCompare(TIMER_CH2, 0);  // PA9
    DVR_1_Tim.setCompare(TIMER_CH3, 0);  // PA10
    DVR_1_Tim.refresh();
    DVR_1_Tim.resume(); 
  }
  if (Num == 2 || Num == 0) {
    digitalWrite(DVR_2_ENGATE, HIGH); // Enable gates
    delay(1); // Wait for voltage pomp (need 1ms)
    DVR_2_Tim.setCompare(TIMER_CH1, 0);  // PC6 // PWM High Pulse width is when reach second value
    DVR_2_Tim.setCompare(TIMER_CH2, 0);  // PC7
    DVR_2_Tim.setCompare(TIMER_CH3, 0);  // PC8
    DVR_2_Tim.refresh();
    DVR_2_Tim.resume();
  }
  if (Num == 3 || Num == 0) {
    digitalWrite(DVR_3_ENGATE, HIGH); // Enable gates
    delay(1); // Wait for voltage pomp (need 1ms)
    DVR_3_Tim.setCompare(TIMER_CH2, 0);  // PA1 // PWM High Pulse width is when reach second value
    DVR_3_Tim.setCompare(TIMER_CH3, 0);  // PA2
    DVR_3_Tim.setCompare(TIMER_CH4, 0);  // PA3
    DVR_3_Tim.refresh();
    DVR_3_Tim.resume();
  }
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
