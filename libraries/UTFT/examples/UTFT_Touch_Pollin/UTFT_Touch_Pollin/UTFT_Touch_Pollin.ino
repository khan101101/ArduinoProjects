// UTFT_Demo_320x240 + tOUCH 7843 
// Copyright (C)2015 Rinky-Dink Electronics, Henning Karlsen. All right reserved
// web: http://www.RinkyDinkElectronics.com/
// for Pollin Eval Board changes by mde Fra-UAS
//
// This program is a demo of how to use most of the functions
// of the library with a supported display modules.
//
// This demo was made for modules with a screen resolution 
// of 320x240 pixels.
//
// This program requires the UTFT library.
//

#include <UTFT.h>

#define DCLK     PA5
#define CS       PB7
#define DIN      PA7 
#define DOUT     PA6
#define IRQ      PB6 

unsigned int TP_X,TP_Y;
void spistart()                                     //SPI Start
{
  digitalWrite(CS,HIGH);
  digitalWrite(DCLK,HIGH);
  digitalWrite(DIN,HIGH);
  digitalWrite(DCLK,HIGH);
 
}
//**********************************************************
void WriteCharTo7843(unsigned char num)          //SPI Write Data
{
  unsigned char count=0;
  unsigned char temp;
  unsigned nop;
  temp=num;
  digitalWrite(DCLK,LOW);
  for(count=0;count<8;count++)
  {
    if(temp&0x80)
      digitalWrite(DIN,HIGH);
    else
      digitalWrite(DIN,LOW);
 
    temp=temp<<1; 
 
    digitalWrite(DCLK,LOW);                
    nop++;
    nop++;
    digitalWrite(DCLK,HIGH);
    nop++;
    nop++;
  }
}
 
//**********************************************************
unsigned int ReadFromCharFrom7843()             //SPI Read Data
{ 
  unsigned nop;
  unsigned char count=0;
  unsigned int Num=0;
  for(count=0;count<12;count++)
  {
    Num<<=1;
    digitalWrite(DCLK,HIGH);//DCLK=1; _nop_();_nop_();_nop_();                
    nop++;
    digitalWrite(DCLK,LOW);//DCLK=0; _nop_();_nop_();_nop_();
    nop++;
    if(digitalRead(DOUT)) Num++;
  }
  return(Num);
}
 
void AD7843(void)              
{
  digitalWrite(CS,LOW);                    
  WriteCharTo7843(0x90); 
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW); 
  TP_Y=ReadFromCharFrom7843();
  WriteCharTo7843(0xD0);   
  digitalWrite(DCLK,HIGH);
  digitalWrite(DCLK,LOW);
  TP_X=ReadFromCharFrom7843();
  digitalWrite(CS,HIGH);
}




// Declare which fonts we will be using
extern uint8_t SmallFont[];

// Remember to change the model parameter to suit your display module!
UTFT myGLCD(ILI9325D_16,PD11,PD5,PD7,PC1);


  int buf[318];
  int x, x2;
  int y, y2;
  int r;



void crosshair()
{
// Draw crosshairs
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);
  for (int i=9; i<310; i+=10)
    myGLCD.drawLine(i, 117, i, 121);
  for (int i=19; i<220; i+=10)
    myGLCD.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  myGLCD.setColor(0,255,255);
  myGLCD.print("Sin", 5, 15);
  for (int i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
  }
  
  myGLCD.setColor(255,0,0);
  myGLCD.print("Cos", 5, 27);
  for (int i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
  }

  myGLCD.setColor(255,255,0);
  myGLCD.print("Tan", 5, 39);
  for (int i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
  }

  delay(2000);
}

void sincurve()
{
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);

// Draw a moving sinewave
  x=1;
  for (int i=1; i<(318*20); i++) 
  {
    x++;
    if (x==319)
      x=1;
    if (i>319)
    {
      if ((x==159)||(buf[x-1]==119))
        myGLCD.setColor(0,0,255);
      else
        myGLCD.setColor(0,0,0);
      myGLCD.drawPixel(x,buf[x-1]);
    }
    myGLCD.setColor(0,255,255);
    y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    myGLCD.drawPixel(x,y);
    buf[x-1]=y;
  }
  delay(2000);
}

void filledrectangles()
{
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
  }

  delay(2000);
}

void filledrounded()
{
    myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled, rounded rectangles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
  }
  
  delay(2000);
}

void filledcircles()
{
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled circles
  for (int i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillCircle(100+(i*20),60+(i*20), 30);
  }
  
  delay(2000);
}

void linepatterns()
{
   myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some lines in a pattern
  myGLCD.setColor (255,0,0);
  for (int i=15; i<224; i+=5)
  {
    myGLCD.drawLine(1, i, (i*1.44)-10, 224);
  }
  myGLCD.setColor (255,0,0);
  for (int i=224; i>15; i-=5)
  {
    myGLCD.drawLine(318, i, (i*1.44)-11, 15);
  }
  myGLCD.setColor (0,255,255);
  for (int i=224; i>15; i-=5)
  {
    myGLCD.drawLine(1, i, 331-(i*1.44), 15);
  }
  myGLCD.setColor (0,255,255);
  for (int i=15; i<224; i+=5)
  {
    myGLCD.drawLine(318, i, 330-(i*1.44), 224);
  }
  
  delay(2000);
}

void randomcircles()
{
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some random circles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=32+random(256);
    y=45+random(146);
    r=random(30);
    myGLCD.drawCircle(x, y, r);
  }

  delay(2000);
}

void randomrectangles()
{
    myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some random rectangles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    myGLCD.drawRect(x, y, x2, y2);
  }

  delay(2000);
}

void randomfilledrectangles()
{
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some random rounded r0ectangles
  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(207);
    x2=2+random(316);
    y2=16+random(207);
    myGLCD.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
}

void randomlines()
{  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

  for (int i=0; i<100; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    x=2+random(316);
    y=16+random(209);
    x2=2+random(316);
    y2=16+random(209);
    myGLCD.drawLine(x, y, x2, y2);
  }

  delay(2000);
}

void randompoints()
{
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

  for (int i=0; i<10000; i++)
  {
    myGLCD.setColor(random(255), random(255), random(255));
    myGLCD.drawPixel(2+random(316), 16+random(209));
  }

  delay(2000);

}


void setup()
{
  randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.setFont(SmallFont);

//Setup Touch
  pinMode(DOUT,INPUT);
  pinMode(IRQ,INPUT);
  pinMode(DCLK,OUTPUT);
  pinMode(CS,OUTPUT);
  pinMode(DIN,OUTPUT);

  Serial1.begin(115200);
 
  spistart();
}

void loop()
{

// Clear the screen and draw the frame
  myGLCD.clrScr();

  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 319, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("* Universal Color TFT Display Library *", CENTER, 1);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.setColor(255,255,0);
  myGLCD.print("mde Fra-UAS  Pollin Demo-Board", CENTER, 227);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 319, 225);

  for (x=0; x < 3; x++)
    for (y=0; y <4; y++)
    {
      myGLCD.setColor(x*80+80, y*63+60, 90);
      myGLCD.fillRect(x*106, y*52+14, x*106+106, y*52+66);
    }

  while(digitalRead(IRQ)){}
  
  {
    if (digitalRead(IRQ)==0) {
      AD7843();
      Serial1.print("TP_X = ");
      Serial1.print(TP_X);
      Serial1.print(", TP_Y = ");
      Serial1.println(TP_Y);
      if (TP_Y < 1500) {
        if (TP_X < 1250)
           crosshair();
        else if (TP_X < 2100)
             filledrectangles();
        else if (TP_X < 2950)
              filledrounded();
        else 
              filledcircles();
      }
      else if (TP_Y < 2600)
      {
          if (TP_X < 1250)
             linepatterns();
          else if (TP_X < 2100)
               randomcircles();
          else if (TP_X < 2950)
                randomrectangles();
          else 
                randomfilledrectangles();
      }
      else
          if (TP_X < 1250)
             randomlines();
          else if (TP_X < 2100)
               randompoints();
          else if (TP_X < 2950)
                sincurve();
          else 
                crosshair();
    }
  }
      


/*
  crosshair();
  sincurve();
  filledrectangles();
  filledrounded();  
  filledcircles();
  linepatterns();
  randomcircles();
  randomrectangles();
  randomfilledrectangles();
  randomlines();
  randompoints();  
  
  if (digitalRead(IRQ)==0) {
    AD7843();
    myGLCD.print("* TP_X , TP_Y = *", LEFT, 1);
    myGLCD.printNumI(TP_X, 150, 1);
    myGLCD.printNumI(TP_Y, 200, 1);
  }

  myGLCD.fillScr(0, 0, 255);
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(80, 70, 239, 169);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("That's it!", CENTER, 93);
  myGLCD.print("Restarting in a", CENTER, 119);
  myGLCD.print("few seconds...", CENTER, 132);
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Runtime: (msecs)", CENTER, 210);
  myGLCD.printNumI(millis(), CENTER, 225);

  for (int i = 0; i < 20;i++) {
    if (digitalRead(IRQ)==0) {
      AD7843();
      myGLCD.print("* TP_X , TP_Y = *", LEFT, (i*10)+1);
      myGLCD.printNumI(TP_X, 150, (i*10)+1);
      myGLCD.printNumI(TP_Y, 200, (i*10)+1);
    }
    delay(500);
  }
  delay(5000);
*/
  
}

