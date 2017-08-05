#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

// MIDI setup
byte note = 0; //The MIDI note value to be played
byte resetMIDI = 4; //Tied to VS1053 Reset line
//byte ledPin = 13; //MIDI traffic inidicator
int instrument =12;

// play song
int menu = 0;//6 113 14 116 23
int inst[18] = {1, 2, 3, 4, 5, 9, 10, 11, 12, 14, 23, 47, 55, 100, 113, 118, 123, 124};
int melody[4][52] = {{55,60,60,62,60,59,57,57,57,62,62,64,62,60,59,55,55,64,64,65,64,62,60,57,55,55,57,62,59,60,55,60,60,60,59,59,60,59,57,55,62,64,62,60,67,55,55,55,57,62,59,60},
                    {64,64,64,64,64,64,64,67,60,62,64,65,65,65,65,64,64,64,62,62,64,62,67,64,64,64,64,64,64,64,67,60,62,64,65,65,65,65,65,64,64,64,67,67,65,62,60},
                    {67,69,67,64,67,69,67,64,74,74,71,72,72,67,69,69,72,71,69,67,69,67,64,69,69,72,71,69,67,69,67,64,74,74,77,74,71,72,76,72,67,64,67,65,62,60},
                    {74,74,74,72,67,74,74,76,72,0,69,72,74,74,76,72,0,72,71,72,71,69,0,76,74,69,76,77,76,74,0,72,71,71,71,72,71,67}};

float rest[4][52] = {{6,6,12,12,12,12,6,6,6,6,12,12,12,12,6,6,6,6,12,12,12,12,6,6,12,12,6,6,6,3,6,6,6,6,3,6,6,6,6,3,6,6,6,6,6,6,12,12,6,6,6,3},
                      {8,8,4,8,8,4,8,8,8,8,2,8,8,4,8,8,4,8,8,8,8,4,4,8,8,4,8,8,4,8,8,8,8,2,8,8,8,8,8,8,8,8,8,8,8,8,2},
                      {3,8,4,1.5,3,8,4,1.5,2,4,1.5,2,4,1.5,2,4,3,8,4,3,8,4,1.5,2,4,3,8,4,3,8,4,1.5,2,4,3,8,4,1.5,1.5,3,8,4,3,8,4,1},
                      {4,8,4,4,8,8,8,8,8,4,8,8,8,8,4,4,8,8,8,8,8,4,4,3,3,4,8,8,8,8,3,8,4,8,4,4,4,3}};

// ledPin setup
const int ledPin = 13;
const int ledPin2 = 13;
const int ledPin3 = 13;
const int ledPin4 = 13;
const int ledPin5 = 13;
const int ledPin6 = 13;
const int ledPin7 = 13;
const int ledPin8 = 13;

// buttonPin setup
const int buttonPin = 13;
const int buttonPin2 = 12;
const int buttonPin3 = 11;
const int buttonPin4 = 10;
const int buttonPin5 = 9;
const int buttonPin6 = 8;
const int buttonPin7 = 7;
const int buttonPin8 = 2;
const int buttonPin9 = 5;
const int buttonPin10 = 6;

// buttonState setup
int buttonState = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int buttonState5 = 0;
int buttonState6 = 0;
int buttonState7 = 0;
int buttonState8 = 0;
int buttonState9 = 0;
int buttonState10 = 0;

void setup() {
  Serial.begin(57600);
  
  //Setup soft serial for MIDI control
  mySerial.begin(31250);

  //Reset the VS1053
  pinMode(resetMIDI, OUTPUT);
  digitalWrite(resetMIDI, LOW);
  delay(100);
  digitalWrite(resetMIDI, HIGH);
  delay(100);
  talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)

  //initialize the pushbutton pins as an input
  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  pinMode(buttonPin4, INPUT);
  pinMode(buttonPin5, INPUT);
  pinMode(buttonPin6, INPUT);
  pinMode(buttonPin7, INPUT);
  pinMode(buttonPin8, INPUT);
  pinMode(buttonPin9, INPUT);
  pinMode(buttonPin10, INPUT);

  //random seeds for instrument 
  randomSeed(analogRead(0));
}

void loop() {
talkMIDI(0xB0, 0, 0x00); //Default bank GM1
talkMIDI(0xC0, instrument, 0);

buttonState = digitalRead(buttonPin);
buttonState2 = digitalRead(buttonPin2);
buttonState3 = digitalRead(buttonPin3);
buttonState4 = digitalRead(buttonPin4);
buttonState5 = digitalRead(buttonPin5);
buttonState6 = digitalRead(buttonPin6);
buttonState7 = digitalRead(buttonPin7);
buttonState8 = digitalRead(buttonPin8);
buttonState9 = digitalRead(buttonPin9);
buttonState10 = digitalRead(buttonPin10);

/* change instrument */
if (buttonState8 == HIGH) 
{ 
  long randNum;
  //int i = 0;
  randNum = random(18);
  //randNum = (i+1)%24;
  //Serial.println(randNum);

  //instrument = randNum;
  instrument = inst[randNum];
  Serial.println(randNum);
} else{}

/* change songs */
if (buttonState10 == HIGH) 
{ 
  if(menu == 3){
    menu = 0;
  }else{
    menu++;
  }// end of else
} else{ }

/* play the song */
if (buttonState9 == HIGH) 
{ 
  int i = menu;
  int j = 0;
  for(; j<52; j++){
    int d = 1500 / rest[i][j];
    lightOn(melody[i][j]);
    noteOn(0,melody[i][j],127);
    int pause = d*1.3;
    delay(pause);
    noteOff(0,melody[i][j],127);
    lightOff(melody[i][j]);
    delay(10);
  }// end of for
} else{ }

/* piano key */
if (buttonState == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(60, DEC); 
noteOn(0, 60, 127);
delay(1);
noteOff(0, 60, 60);
delay(1);
} else{ }

if (buttonState2 == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(62, DEC);
noteOn(0, 62, 127);
delay(1);
noteOff(0, 62, 60);
delay(1); 
} else{}

if (buttonState3 == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(64, DEC);
noteOn(0, 64, 127);
delay(1);
noteOff(0, 64, 60);
delay(1); 
} else{}

if (buttonState4 == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(65, DEC);
noteOn(0, 65, 127);
delay(1);
noteOff(0, 65, 60);
delay(1); 
} else{}

if (buttonState5 == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(67, DEC); 
noteOn(0, 67, 127);
delay(1);
noteOff(0, 67, 60);
delay(1);
} else{ }

if (buttonState6 == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(69, DEC); 
noteOn(0, 69, 127);
delay(1);
noteOff(0, 69, 60);
delay(1);
} else{ }

if (buttonState7 == HIGH) 
{ 
talkMIDI(0xC0, instrument, 0);
Serial.print("N:");
Serial.println(71, DEC); 
noteOn(0, 71, 127);
delay(1);
noteOff(0, 71, 60);
delay(1);
} else{ }

}


//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(ledPin, HIGH);
  mySerial.write(cmd);
  mySerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0 )
    mySerial.write(data2);

  digitalWrite(ledPin, LOW);
}

