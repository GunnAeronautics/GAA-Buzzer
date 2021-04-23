#include "Arduino.h"
#include "Buzzer.h"

//version 0.3

boolean buzzOn = true; // is the buzzer on?
elapsedMillis lastChange = 0;
uint16_t millis_last = 0; // how long has it been since last change
uint16_t beepsLeft = 0; // for double beeping

byte pin; //what pin
uint16_t beepNum = 1; // how many times do we beep
uint16_t pwm_duty = 128; // shouldn't really be changed
uint16_t longFreq = 1000; // how much time between beeps
uint16_t shortFreq = 100; // how much time between repeated beeps
uint16_t tohn = 220; // the tone. currently does not support songwriting


Buzzer::Buzzer(int inpin) {
  pin = inpin;
  pwm_duty = 128;
  tohn = 220;
  pinMode(pin, OUTPUT);
  analogWriteFrequency(pin, tohn);
  buzzOn = true;
  longFreq = 1000;
  shortFreq = 100;
}

void Buzzer::updat(){
  
  if(beepsLeft == 0){
    buzzOn = false;
    if(lastChange >= 1000) {
      buzzOn = true;
      lastChange = 0;
      beepsLeft = beepNum;
    }
  }else if(lastChange >= 100){
    buzzOn = !buzzOn;
    lastChange = 0;
    if(buzzOn){
      beepsLeft--;
    }
  }

  /*
  Serial.print(millis());
  Serial.print('\t');
  Serial.print(lastChange);
  Serial.print('\t');
  Serial.println(beepsLeft);
  */

  analogWrite(pin, buzzOn * pwm_duty);
  
  //analogWrite(pin, 128);
}

void Buzzer::beep(int intohn){
  analogWriteFrequency(pin, intohn);
  analogWrite(pin, pwm_duty);
  delay(shortFreq);
  analogWrite(pin, 0);
  analogWriteFrequency(pin, tohn);
}

void Buzzer::off(){
  analogWrite(pin, 0);
}

void Buzzer::morse(byte inbyte){
  byte reps = (morses[inbyte] << 5);
  reps = reps >> 5;
  byte oreps = reps;
  //Serial.println(reps);
  while(reps > 0){
    byte temp = morses[inbyte] << (oreps - reps);
    temp = temp >> 7;
    if(temp == 0){
      tone(pin, tohn, shortFreq);
      //Serial.print('.');
      delay(shortFreq);
    }else{
      tone(pin, tohn, 3 * shortFreq);
      //Serial.print('-');
      delay(3 * shortFreq);
    }
    //Serial.print('\n');
    delay(shortFreq);
    reps--;
  }
  delay(3 * shortFreq);
}

void Buzzer::morse(char inchar){
  if(inchar == 0x20){
    delay(9 * shortFreq);
    inchar = 0x00;
  }else{
    String chars = "!&()+,-./0123456789=abcdefghijklmnopqrstuvwxyz";
    inchar = char(String(inchar).toLowerCase()[0]);
    inchar = 1 + chars.indexOf(inchar);
  }
  morse((byte)inchar); 
}

void Buzzer::morse(String instring){
  for(unsigned int i = 0; i < instring.length(); i++){
    morse(instring[i]);
  }
}