/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/

#define LED_BUILTIN PE5
#define LED_BUILTIN2 PE6
#define Taster PC1  // PC1 to arduino introduced


int currentLED = LED_BUILTIN;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  /*Das wird nur einmal ausgeführt*/
  
  pinMode(Taster,INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_BUILTIN2, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);    // turn the LED off by making the voltage LOW
  digitalWrite(LED_BUILTIN2, HIGH);    // turn the LED off by making the voltage LOW
}

// the loop function runs over and over again forever
void loop() {
//low means that the button is Taster is pressed

 
  if(digitalRead(Taster)==LOW){ 
     if (currentLED == LED_BUILTIN) currentLED=LED_BUILTIN2;
      else  currentLED=LED_BUILTIN;
  }
  digitalWrite(currentLED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(currentLED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
 
  
}
