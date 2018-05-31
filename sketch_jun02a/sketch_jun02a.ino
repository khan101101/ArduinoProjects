




void setup() {
  // put your setup code here, to run once:
  pinMode(PA6,PWM);
  Timer3.setPeriod(8); // 118 -->118000khz = 118000khz * 1/118000 = 0,0000008*1000000 = 8microsencods
  pwmWrite(PA6,0);


}

void loop() {
  // put your main code here, to run repeatedly:
  
  pwmWrite(PA6, 55/100*65535);    

}
