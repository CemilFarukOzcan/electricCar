#include <Arduino.h>
#include <SoftwareSerial.h>
#include "NewPing.h"

SoftwareSerial btBaglantisi(3, 4); // tx ve rx doluluğunda pinleri haberleşme pinine çevirir.

#define max_speed 255
#define min_speed 0
#define default_speed 80

#define TRIGGER_PIN 5
#define ECHO_PIN 12
#define MAX_DISTANCE 400 

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float duration, mesafe;

// in1: ileri sağ
// in2: geri sağ
// in3: ileri sol
// in4: geri sol
// enA: sağ
// enB: sol

int ileri_sag = 7;
int geri_sag = 8;
int ileri_sol = 9;
int geri_sol = 10;
int soll = 11;
int sagg = 6;

float vites = default_speed;

void hareket_motor(int hiz_sag, int hiz_sol, bool yon_sag, bool yon_sol){
  analogWrite(sagg, hiz_sag);
  analogWrite(soll, hiz_sol);
  digitalWrite(ileri_sag, yon_sag);
  digitalWrite(geri_sag, !yon_sag);
  digitalWrite(ileri_sol, yon_sol);
  digitalWrite(geri_sol, !yon_sol);
}

void dur(){
  hareket_motor(0, 0, LOW, LOW);
}

void ileri(int hiz){
  hareket_motor(map(hiz, min_speed, max_speed, 0, max_speed), map(hiz, min_speed, max_speed, 0, max_speed), HIGH, HIGH);
}

void ileri_dur(int hiz){
  hareket_motor(map(hiz, min_speed, max_speed, 0, max_speed), map(hiz, min_speed, max_speed, 0, max_speed), LOW, LOW);
}

void geri(int hiz){
  hareket_motor(map(hiz, min_speed, max_speed, 0, max_speed), map(hiz, min_speed, max_speed, 0, max_speed), LOW, LOW);
}

void sol(int hiz){
  hareket_motor(map(hiz, min_speed, max_speed, 0, max_speed), map(hiz, min_speed, max_speed, 0, max_speed), LOW, HIGH);
}

void sag(int hiz){
  hareket_motor(map(hiz, min_speed, max_speed, 0, max_speed), map(hiz, min_speed, max_speed, 0, max_speed), HIGH, LOW);
}

void _delay(float seconds){
  delay(seconds * 1000);
}

void setup() {
  pinMode(ileri_sag, OUTPUT);
  pinMode(geri_sag, OUTPUT);
  pinMode(ileri_sol, OUTPUT);
  pinMode(geri_sol, OUTPUT);
  pinMode(soll, OUTPUT);
  pinMode(sagg, OUTPUT);
  btBaglantisi.begin(9600);
  btBaglantisi.flush();
  vites = default_speed;

  Serial.begin(9600);
}

void loop() {
  while(true) {
    mesafe = sonar.ping_cm();
  
    Serial.print("Mesafe = ");
  
    if (mesafe >= 400 || mesafe <= 2) 
    {
      Serial.println("Out of range");
    }
    else 
    {
      Serial.print(mesafe);
      Serial.println(" cm");
    }

    if(btBaglantisi.available()){
      char komut = btBaglantisi.read();
      switch(komut){

          case '1':
              vites = 120;
              break;
          case '2':
              vites = 140;
              break;
          case '3':
              vites = 160;
              break;
          case '4':
              vites = 180;
              break;
          case '5':
              vites = 200;
              break;
          case '6':
              vites = 240;
              break;
          case 'F':
              ileri(vites);
              break;
          case 'B':
              geri(vites);
              break;
          case 'L':
              sol(vites);
              break;
          case 'R':
              sag(vites);
              break;
          case 'S':
              dur();
              break;
          default:
              break;
      }
    }
    if (mesafe <= 20) {
      ileri(min_speed);
      if(btBaglantisi.available()){
      char komut = btBaglantisi.read();
      switch(komut){
          case '1':
              vites = 120;
              break;
          case '2':
              vites = 140;
              break;
          case '3':
              vites = 160;
              break;
          case '4':
              vites = 180;
              break;
          case '5':
              vites = 200;
              break;
          case '6':
              vites = 255;
              break;
          case 'B':
              geri(vites);
              break;
          case 'L':
              sol(vites);
              break;
          case 'R':
              sag(vites);
              break;
          case 'S':
              dur();
              break;
          default:
              break;
      }
    }
    }
  }
}
