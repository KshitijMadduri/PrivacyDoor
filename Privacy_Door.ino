


/* Privacy Door: This code is for Arduino Proccessors and makes use of Arduino Mega, SD Card Module, KY-037 Sound Sensor and HC-SR04 Proximity Sensor.
 *  The KY-037 Sensor gives analog readings which are mapped accordingly to illuminate an Led Strip in realtime.


This Code makes use of multipile libraries.
The SD Library is used to intialise a micro-SD Card reader and allows Data Transfer.
For using The micro SD Card module
    
    SD card attached to SPI bus as follows:
 ** MOSI - pin 11 on Arduino Uno/Duemilanove/Diecimila
 ** MISO - pin 12 on Arduino Uno/Duemilanove/Diecimila
 ** CLK - pin 13 on Arduino Uno/Duemilanove/Diecimila
 ** CS - depends on your SD card shield or module. (Pin 53 on Mega, 10 on UNO/Nano)

The TMRpcm Library allows Playback of .wav files using PWM Output of Arudunios. 
The audio files have to be 8-bit, 16000 Hz and Mono .wav files
     
     
   */




#include "TMRpcm.h"
#include "SPI.h"
#include "SD.h"

#define SD_ChipSelectPin 53 //For Mega
#define echoPin 40 // attach pin 40 Arduino to pin Echo of HC-SR04
#define trigPin 41//attach pin 41 Arduino to pin Trig of HC-SR04

long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


TMRpcm tmrpcm;

void setup() {
 Serial.begin(9600);

  pinMode(A1, INPUT); //Configuring A1 for analog input of KY037
  pinMode(37, INPUT); //Configuring 37 as Music toggle input
  pinMode(34, OUTPUT);//Configuring 34 as music toggle indicator
  pinMode(35, OUTPUT);//Configuring 35 as Proximity indicator

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT


//Configuring LED Strip as output 
for(int z = 22; z < 36; z++){
pinMode(z, OUTPUT);} 

tmrpcm.speakerPin = 12;
if (!SD.begin(SD_ChipSelectPin)) {
Serial.println("SD fail");
return;

}
}

void loop() {
  
 /* ============ Sound Module =============================*/
 
//here we are storing the volume value
int volume = analogRead(A1);
//max value for analog read is 1023 but it must be very very loud to reach this value
//so I lower it down in map function to 700
//mapping volume value to make it easier to turn LEDs on

volume = map(volume, 500, 510, 0, 10);
//volume = map(volume, 5, 10, 0, 10);
//Serial.println(volume); //use this to see analog values to map them better.

//for loop to turn on or off all LEDs
//we are going through all pins where we have LEDs and checking if the volume is
//bigger then pin number (that's why we are maping the volume)
for(int a = 22; a < 32; a++){ 
 if(volume >= a){
 //if it is bigger we can turn on the LED
 digitalWrite(a, HIGH);
 }else{
 //if it is smaller we can turn the LED off
 digitalWrite(a, LOW);
 }
}

/* ================ Music Playback =================== */
 
 //Switch Logic to play Music
 
 if(digitalRead(37)==HIGH)
 {tmrpcm.play("inter.wav");} //audio file is named as "inter"

 if(tmrpcm.isPlaying()==1)
 digitalWrite(34,HIGH);
 else
 digitalWrite(34,LOW);
 


 /* ============== Ultra Sonic Proximity Logic ========*/

// Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
 
  
 if(distance<100)  //Proximity Trigger that will light up LED accordingly
 {digitalWrite(35,HIGH);}
 else
 {digitalWrite(35,LOW);}

 
}
