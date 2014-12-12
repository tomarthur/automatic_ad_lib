/*

 Automatic Ad Lib
 Time Feature Version

 Tom Arthur
 ITP Automata 2014 Fall

 Includes public domain code by Tom Igoe:
 Time Check

 Gets the time from the Linux processor via Bridge
 then parses out hours, minutes and seconds for the Arduino
 using an Arduino Yun.

 created  27 May 2013
 modified 21 June 2013
 By Tom Igoe

 */


#include <Process.h>
#include <AccelStepper.h>
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

Process date;                 // process used to get the date
int hours, minutes, seconds;  // for the results
String hourString, minString;
int lastSecond = -1;          // need an impossible value for comparison
int lastMinute = -1;          // last minute for use with cards
int lastDisplayedTime = -2;   // last time displayed on cards

int motorSpeed = 5000;        //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 3000;        //steps/second/second to accelerate

const int ledPin =  13;       // the number of the LED pin

int serialCard0, serialCard1, serialCard2, serialCard3, serialCard4;

// universal for motor control
const int motorDirPin = 4;    // all motors travel in the same direction
int fullRotation = 2000;      // define full rotation to calculate new card
int state = 0;                // current display mode

//set up the accelStepper intances and associated variables
const int motorStepPin0 = 3;
const int calibratePin0 = 2;
const int homingValue0 = 470;
int calibrateState0 = 0;
int nextCard0 = 0;
int currentCard0 = 0;
int stepperCard0 = 0;
AccelStepper stepper0(1, motorStepPin0, motorDirPin);    // flip1

const int motorStepPin1 = 5;
const int calibratePin1 = 11;
const int homingValue1 = 110;
int calibrateState1 = 0;
int nextCard1 = 0;
int currentCard1 = 0;
int stepperCard1 = 0;
AccelStepper stepper1(1, motorStepPin1, motorDirPin);    // flip2

const int motorStepPin2 = 6;
const int calibratePin2 = 7;
const int homingValue2 = 515;
int calibrateState2 = 0;
int nextCard2 = 0;
int currentCard2 = 0;
int stepperCard2 = 0;
AccelStepper stepper2(1, motorStepPin2, motorDirPin);    // flip3

const int motorStepPin3 = 9;
const int calibratePin3 = 8;
const int homingValue3 = 92;
int calibrateState3 = 0;
int nextCard3 = 0;
int currentCard3 = 0;
int stepperCard3 = 0;
AccelStepper stepper3(1, motorStepPin3, motorDirPin);    // flip4

const int motorStepPin4 = 10;
const int calibratePin4 = 12;
const int homingValue4 = 94;
int calibrateState4 = 0;
int nextCard4 = 0;
int currentCard4 = 0;
int stepperCard4 = 0;
AccelStepper stepper4(1, motorStepPin4, motorDirPin);    // flip5

int randoSerialCard[] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44};
int randoCenterCard[] = {0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44};

void setup() {
  Bridge.begin();        // initialize Bridge
  Serial.begin(9600);    // initialize serial
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

//  while (!Serial);              // wait for Serial Monitor to open
  Serial.println("Automatic Automata");  // Title of sketch

  // run an initial date process. Should return:
  // hh:mm:ss :
  if (!date.running())  {
    date.begin("date");
    date.addParameter("+%T");
    date.run();

  }

  pinMode(ledPin, OUTPUT);
  randomSeed(analogRead(0));

  stepper0.setPinsInverted(1, 0, 0);
  stepper0.setMaxSpeed(motorSpeed);
  stepper0.setAcceleration(motorAccel);
  pinMode(calibratePin0, INPUT);

  stepper1.setPinsInverted(1, 0, 0);
  stepper1.setMaxSpeed(motorSpeed);
  stepper1.setAcceleration(motorAccel);
  pinMode(calibratePin1, INPUT);

  stepper2.setPinsInverted(1, 0, 0);
  stepper2.setMaxSpeed(motorSpeed);
  stepper2.setAcceleration(motorAccel);
  pinMode(calibratePin2, INPUT);

  stepper3.setPinsInverted(1, 0, 0);
  stepper3.setMaxSpeed(motorSpeed);
  stepper3.setAcceleration(motorAccel);
  pinMode(calibratePin3, INPUT);

  stepper4.setPinsInverted(1, 0, 0);
  stepper4.setMaxSpeed(motorSpeed);
  stepper4.setAcceleration(motorAccel);
  pinMode(calibratePin4, INPUT);

  calibrate(stepper0, 0);
  calibrate(stepper1, 1);
  calibrate(stepper2, 2);
  calibrate(stepper3, 3);
  calibrate(stepper4, 4);
}

void loop() {
  
//    // Get clients coming from server
//  YunClient client = server.accept();
//
//  // There is a new client?
//  if (client) {
//    // Process request
//    process(client);
//
//    // Close connection and free resources.
//    client.stop();
//  }


  // select random cards not including numbers, and choose an operator for middle card (serialCard2)
  serialCard0 = randoSerialCard[random(0, 45)];
  serialCard1 = randoSerialCard[random(0, 45)];
  serialCard2 = randoCenterCard[random(0, 
  serialCard3 = randoSerialCard[random(0, 45)];
  serialCard4 = randoSerialCard[random(0, 45)];


  if (state == 0) {                   // delay
    getTime();                        // get new time during wait period, this will add an interminate amout of delay
    delay(9000);
    Serial.println("time to display random cards");
    if (lastDisplayedTime != lastMinute) {
      state = 2;
    } else {
      state = 1;
    }

  }
  else if (state == 1) {              // set next turn for blank display
    Serial.println("time to display 0s");
    delay(9000);

    serialCard0 = 0;
    serialCard1 = 0;
    serialCard2 = 0;
    serialCard3 = 0;
    serialCard4 = 0;

    state = 0;

  }
  else if (state == 2) {             // display time
    delay(9000);
    Serial.println("time to display time");

    String tempHours0, tempHours1, tempMin0, tempMin1;

    tempHours0 = hourString.substring(0, 1);
    tempHours1 = hourString.substring(1);

    tempMin0 = minString.substring(0, 1);
    tempMin1 = minString.substring(1);

    Serial.println(hourString);
    Serial.println(minString);
    Serial.println("tempHours: ");
    Serial.println(tempHours0);
    Serial.println(tempHours1);
    Serial.println(tempMin0);
    Serial.println(tempMin1);


    if (tempHours0 == 0 && tempHours1 == 0 && tempMin0 == 0 && tempMin1 == 0) {
      serialCard0 = random(11, 44);
      serialCard1 = random(11, 44);
      serialCard2 = random(2, 13);
      serialCard3 = random(11, 44);
      serialCard4 = random(11, 44);
      Serial.println("0 time so random cards");
    } else {
      serialCard0 = tempHours0.toInt() + 1;
      serialCard1 = tempHours1.toInt() + 1;
      serialCard2 = 1;
      serialCard3 = tempMin0.toInt() + 2;
      serialCard4 = tempMin1.toInt() + 1;
    }

    state = 1;
    lastDisplayedTime = minutes;

  }

  serialCard0 = constrain(serialCard0, 0, 40);
  serialCard1 = constrain(serialCard1, 0, 44);
  serialCard2 = constrain(serialCard2, 0, 40);
  serialCard3 = constrain(serialCard3, 0, 44);
  serialCard4 = constrain(serialCard4, 0, 44);

  stepperCard0 = serialCard0 * 45;
  stepperCard1 = serialCard1 * 45;
  stepperCard2 = serialCard2 * 50;
  stepperCard3 = serialCard3 * 45;
  stepperCard4 = serialCard4 * 45;


  int newPosition0 = stepperCard0;
  int newPosition1 = stepperCard1;
  int newPosition2 = stepperCard2;
  int newPosition3 = stepperCard3;
  int newPosition4 = stepperCard4;

  boolean fixLocation0 = false;
  boolean fixLocation1 = false;
  boolean fixLocation2 = false;
  boolean fixLocation3 = false;
  boolean fixLocation4 = false;

  if (stepperCard0 < currentCard0) {
    int tempPost0 = fullRotation - currentCard0;
    newPosition0 = currentCard0 + tempPost0 + newPosition0;
    //Serial.print("board 0 fixed position: ");
    //Serial.println(newPosition0);
    fixLocation0 = true;
  }


  if (stepperCard1 < currentCard1) {
    int tempPost1 = fullRotation - currentCard1;
    //Serial.print("board 1 fixed position: ");
    newPosition1 = currentCard1 + tempPost1 + newPosition1;
    fixLocation1 = true;
  }

  if (stepperCard2 < currentCard2) {
    int tempPost2 = fullRotation - currentCard2;
    //Serial.print("board 1 fixed position: ");
    newPosition2 = currentCard2 + tempPost2 + newPosition2;
    fixLocation2 = true;
  }

  if (stepperCard3 < currentCard3) {
    int tempPost3 = fullRotation - currentCard3;
    //Serial.print("board 1 fixed position: ");
    newPosition3 = currentCard3 + tempPost3 + newPosition3;
    fixLocation3 = true;
  }
  if (stepperCard4 < currentCard4) {
    int tempPost4 = fullRotation - currentCard4;
    //Serial.print("board 1 fixed position: ");
    newPosition4 = currentCard4 + tempPost4 + newPosition4;
    fixLocation4 = true;
  }

  stepper0.moveTo(newPosition0);
  stepper1.moveTo(newPosition1);
  stepper2.moveTo(newPosition2);
  stepper3.moveTo(newPosition3);
  stepper4.moveTo(newPosition4);

  while (stepper0.distanceToGo() != 0 || stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0 || stepper3.distanceToGo() != 0 || stepper4.distanceToGo() != 0) {
    stepper0.run();
    stepper1.run();
    stepper2.run();
    stepper3.run();
    stepper4.run();

    if (stepper0.distanceToGo() == 0) {
      if (fixLocation0 == true) {
        //Serial.print("location 0 it thinks is: ");
        //Serial.println(stepper0.currentPosition());
        stepper0.setCurrentPosition(stepperCard0);
        //Serial.print("fixed location for 0 ");
        //Serial.println(stepperCard0);
        fixLocation0 = false;
      }

      currentCard0 = stepper0.currentPosition();
      stepper0.stop();
    }

    if (stepper1.distanceToGo() == 0) {
      if (fixLocation1 == true) {
        //Serial.print("location 1 it thinks is: ");
        //Serial.println(stepper1.currentPosition());
        stepper1.setCurrentPosition(stepperCard1);
        //Serial.print("fixed location for 1 ");
        //Serial.println(stepperCard1);
        fixLocation1 = false;
      }

      currentCard1 = stepper1.currentPosition();
      stepper1.stop();
    }

    if (stepper2.distanceToGo() == 0) {
      if (fixLocation2 == true) {
        //Serial.print("location 0 it thinks is: ");
        //Serial.println(stepper0.currentPosition());
        stepper2.setCurrentPosition(stepperCard2);
        //Serial.print("fixed location for 0 ");
        //Serial.println(stepperCard0);
        fixLocation2 = false;
      }

      currentCard2 = stepper2.currentPosition();
      stepper2.stop();
    }

    if (stepper3.distanceToGo() == 0) {
      if (fixLocation3 == true) {
        //Serial.print("location 0 it thinks is: ");
        //Serial.println(stepper0.currentPosition());
        stepper3.setCurrentPosition(stepperCard3);
        //Serial.print("fixed location for 0 ");
        //Serial.println(stepperCard0);
        fixLocation3 = false;
      }

      currentCard3 = stepper3.currentPosition();
      stepper3.stop();
    }

    if (stepper4.distanceToGo() == 0) {
      if (fixLocation4 == true) {
        //Serial.print("location 0 it thinks is: ");
        //Serial.println(stepper0.currentPosition());
        stepper4.setCurrentPosition(stepperCard4);
        //Serial.print("fixed location for 0 ");
        //Serial.println(stepperCard0);
        fixLocation4 = false;
      }

      currentCard4 = stepper4.currentPosition();
      stepper4.stop();
    }
  }
}

void displayTime() {


}

void calibrate(AccelStepper currentStepper, int currentFlip) {
  boolean step1 = false;        // part one of calibration complete
  boolean calibrated = false;   // calibration complete
  int startTime = millis();
  // long millisWait = 0;
  int currentCalibrate = -1;
  int currentCalibratePin = -1;
  int currentHomingValue = -1;

  currentStepper.setMaxSpeed(100);
  currentStepper.setAcceleration(80000.0);
  Serial.println(currentFlip);
  // set pins and homing locations for each board
  if (currentFlip == 0) {
    currentCalibrate = calibrateState0;
    currentCalibratePin = calibratePin0;
    currentHomingValue = homingValue0;
  }
  else if (currentFlip == 1) {
    currentCalibrate = calibrateState1;
    currentCalibratePin = calibratePin1;
    currentHomingValue = homingValue1;

  }
  else if (currentFlip == 2) {
    currentCalibrate = calibrateState2;
    currentCalibratePin = calibratePin2;
    currentHomingValue = homingValue2;

  }
  else if (currentFlip == 3) {
    currentCalibrate = calibrateState3;
    currentCalibratePin = calibratePin3;
    currentHomingValue = homingValue3;

  }
  else if (currentFlip == 4) {
    currentCalibrate = calibrateState4;
    currentCalibratePin = calibratePin4;
    currentHomingValue = homingValue4;

  }
  else {

    //    Serial.println("ERROR THIS ISN'T A STEPPER I UNDERSTAND");
  }

  // set an artifically long movement so we it all the way though the hole
  currentStepper.moveTo(10000);

  // find the magnet that indicates the index position
  while (step1 == false) {
    currentStepper.run();

    currentCalibrate = digitalRead(currentCalibratePin);
    //    Serial.println(currentCalibrate);
    //    if (millis() > startTime + 1000){
    // check if the home position is found
    // if it is, the calibration is complete:
    if (currentCalibrate == LOW) {
      if (step1 == false) {
        // turn LED on:
        digitalWrite(ledPin, HIGH);
        currentStepper.stop();
        currentStepper.setCurrentPosition(0);

        delay(500);

        step1 = true;
        currentStepper.moveTo(currentHomingValue);

      }
    }
    else {
      digitalWrite(ledPin, LOW);
    }
    //    }
  }

  // move to the start card
  while (step1 == true && calibrated == false) {

    if (currentStepper.distanceToGo() == 0) {
      //go the other way the same amount of steps
      //so if current position is 400 steps out, go position -400
      currentStepper.setCurrentPosition(0);
      calibrated = true;
    }

    currentStepper.run();
  }

  currentStepper.setCurrentPosition(0);

  //  Serial.print("calibration done for: ");
  //  Serial.println(currentFlip);
  //  delay(500);
}



void getTime() {
  Serial.println("Time!");
  if (lastSecond != seconds) { // if a second has passed
    // print the time:
    if (hours <= 9) Serial.print("0");    // adjust for 0-9
    Serial.print(hours);
    Serial.print(":");
    if (minutes <= 9) Serial.print("0");  // adjust for 0-9
    Serial.print(minutes);
    Serial.print(":");
    if (seconds <= 9) Serial.print("0");  // adjust for 0-9
    Serial.println(seconds);

    // restart the date process:
    if (!date.running())  {
      date.begin("date");
      date.addParameter("+%T");
      date.run();
    }
  }

  //if there's a result from the date process, parse it:
  while (date.available() > 0) {
    // get the result of the date process (should be hh:mm:ss):
    String timeString = date.readString();

    // find the colons:
    int firstColon = timeString.indexOf(":");
    int secondColon = timeString.lastIndexOf(":");

    // get the substrings for hour, minute second:
    hourString = timeString.substring(0, firstColon);
    minString = timeString.substring(firstColon + 1, secondColon);
    String secString = timeString.substring(secondColon + 1);

    // convert to ints,saving the previous second:
    hours = hourString.toInt();
    minutes = minString.toInt();
    lastSecond = seconds;          // save to do a time comparison
    seconds = secString.toInt();
    lastMinute = minutes;
  }

}

//void process(YunClient client) {
//  // read the command
//  String command = client.readStringUntil('/');
//
//  if (command == "calibrate") {
//    calibrateRequest(client);
//  }
//
//  if (command == "setmmanually") {
//    setmManually(client);
//  }
//  
//  if (command == "auto") {
//    changeMode(client);
//  }
//  
//}
//
//void calibrateRequest (YunClient client) {
//  int board;
//
//  // Read pin number
//  board = client.parseInt();
//  
//  // Send feedback to client
//  client.print(F("Display "));
//  client.print(board);
//  client.println(F(" set to calibrate. Please wait"));
//  
//  if (board < 5){
//    stopAll();
//    calibrate(board, board);
//  }
//}
//
//void setmManually(YunClient client) {
//  int displayNum;
//
//  // Read pin number
//  displayNum = client.parseInt();
//
//  // If the next character is not a '/' we have a malformed URL
//  if (client.read() != '/') {
//    client.println(F("error"));
//    return;
//  }
//
//  String card = client.readStringUntil('\r');
//
//  switch (displayNum) {
//    case 0:
//      serialCard0 = card.toInt();
//      break;
//    case 1:
//      serialCard1 = card.toInt();
//      break;
//    case 2:
//      serialCard2 = card.toInt();
//      break;
//    case 3:
//      serialCard3 = card.toInt();
//      break;
//    case 4:
//      serialCard4 = card.toInt();
//      break;
//  }
//    
//    // Send feedback to client
//    client.print(F("Display "));
//    client.print(displayNum);
//    client.print(F(" Set to card:"));
//    client.println(card);
//    return;
//  }
//
//  client.print(F("error: invalid display or card"));
//  client.print(mode);
//}
//
//void calibrateRequest (YunClient client) {
//  int board;
//
//  // Read pin number
//  board = client.parseInt();
//  
//  if (board < 5){
//    stopAll();
//    calibrate(board, board);
//
//  // Send feedback to client
//  client.print(F("Display "));
//  client.print(board);
//  client.println(F(" set to calibrate. Please wait"));
//
////  // Update datastore key with the current pin value
////  String key = "D";
////  key += pin;
////  Bridge.put(key, String(value));
//}
//
//void changeMode (YunClient client) {
//
//  
//  // Send feedback to client
//  client.print(F("Not implemented "));
//}
//
//
//void stopAll(){
//  
//}
