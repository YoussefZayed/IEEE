
#include "MeMCore.h"
byte leftSensor;
byte rightSensor;
boolean starting = true;
boolean moved = true;
MeBuzzer buzzer;


// x and y are flipped x is vertical y is horizontal
byte board [7][4] {
  {1, 1, 1, 1},
  {1, 1, 1, 1},
  {1, 1, 1, 1},
  {1, 1, 1, 1},
  {1, 1, 1, 1},
  {1, 1, 1, 1},
  {1, 1, 1, 1}
};
byte path [10][2] {
  {6, 1},
  {6, 2},
  {6, 3},
  {5, 3},
  {4, 3},
  {3, 3},
  {2, 3},
  {1, 3},
  {0, 3},
  { -1, -1}
};
byte path_step = 0;
byte pathx = 0;
byte pathy = 0;
byte curr_x = 0;
byte curr_y = 0;
byte curr_d = 1;
byte d_needed = 1;
byte h = 0;
byte curr_px = 0;
byte curr_py = 0;

MeDCMotor motor1(PORT_1);
MeDCMotor motor3(M1);

MeDCMotor motor2(PORT_2);
MeDCMotor motor4(M2);
byte objectD = 0;
uint8_t motorSpeed = 200;

MeUltrasonicSensor ultraSensor(4);

boolean tr = false;
boolean tl = false;
boolean at_stop = false;
byte tring = 0;
void setup() {

  Serial.begin(9600);

  pinMode(12, INPUT);
  pinMode(9, INPUT);
  delay(2000);

}

void loop() {

  leftSensor = digitalRead(12);
  rightSensor = digitalRead(9);
  objectD = ultraSensor.distanceCm();
  //  Serial.print(leftSensor);
  //  Serial.println(rightSensor);
  //
  //  Serial.print(ultraSensor.distanceCm());
  //  Serial.println(" cm");
  if (curr_d > 4) {
    curr_d = 1;
  }
  if (tr == true) {
    turnR();
  }
  else if (tl == true) {
    turnL();
  }
  else if (leftSensor == 1 && rightSensor == 0) {
    motor1.run(-motorSpeed);
    motor3.run(-motorSpeed);
    motor2.run(motorSpeed - 100);
    motor4.run(motorSpeed - 100);

  }
  else if (leftSensor == 0 && rightSensor == 1) {
    motor1.run(-motorSpeed + 100);
    motor3.run(-motorSpeed + 100);
    motor2.run(motorSpeed);
    motor4.run(motorSpeed);

  }

  else if (leftSensor == 0 && rightSensor == 0) {
    buzzer.tone(262, 0.25 * 1000);
    brake();
    delay(20);
    Serial.print(pathx);
    Serial.print("_node_");
    Serial.println(pathy);
    moved = true;
    checkNode();

  }
  else {
    drive();
  }


}

void drive() {

  motor1.run(-motorSpeed);
  motor2.run(motorSpeed);
  motor3.run(-motorSpeed);
  motor4.run(motorSpeed);

}
void turnR() {
  objectD = ultraSensor.distanceCm();
  if (tring == 0 && leftSensor == 0 && rightSensor == 0 && at_stop == true) {
    drive();
    delay(100);


  }
  else if (tring == 0  ) {
    if (leftSensor == 1 && rightSensor == 0 && at_stop == true) {
      motor1.run(100 - motorSpeed);
      motor2.run(100 - motorSpeed);
      motor3.run(100 - motorSpeed);
      motor4.run(100 - motorSpeed);
    }
    else if (leftSensor == 0 && rightSensor == 1 && at_stop == false) {
      motor1.run(motorSpeed - 100);
      motor2.run(motorSpeed - 100);
      motor3.run(motorSpeed - 100);
      motor4.run(motorSpeed - 100);
    }
    else {
      tring++;
    }
  }
  else if (tring == 1) {
    if (rightSensor == 1) {
      motor1.run(100 - motorSpeed);
      motor2.run(100 - motorSpeed);
      motor3.run(100 - motorSpeed);
      motor4.run(100 - motorSpeed);
    } else {
      tring ++;
    }
  }
  else if (leftSensor == 0) {
    motor1.run(motorSpeed - 100);
    motor2.run(motorSpeed - 100);
    motor3.run(motorSpeed - 100);
    motor4.run(motorSpeed - 100);
  }
  else {
    brake();
    curr_d++;
    delay(100);
    moved = false;
    checkNode();

  }
}
void turnL() {
  objectD = ultraSensor.distanceCm();
  if (tring == 0 && leftSensor == 0 && rightSensor == 0 && at_stop == true) {
    drive();
    delay(100);


  }
  else if (tring == 0  ) {
    if (leftSensor == 1 && rightSensor == 0 && at_stop == true) {
      motor1.run(100 - motorSpeed);
      motor2.run(100 - motorSpeed);
      motor3.run(100 - motorSpeed);
      motor4.run(100 - motorSpeed);
    }
    else if (leftSensor == 0 && rightSensor == 1 && at_stop == false) {
      motor1.run(motorSpeed - 100);
      motor2.run(motorSpeed - 100);
      motor3.run(motorSpeed - 100);
      motor4.run(motorSpeed - 100);
    }
    else {
      tring++;
    }
  }
  else if (tring == 1) {
    if (leftSensor == 1) {
      motor1.run(motorSpeed - 100);
      motor2.run(motorSpeed - 100);
      motor3.run(motorSpeed - 100);
      motor4.run(motorSpeed - 100);

    } else {
      tring ++;
    }
  }
  else if (rightSensor == 0) {
    motor1.run(100 - motorSpeed);
    motor2.run(100 - motorSpeed);
    motor3.run(100 - motorSpeed);
    motor4.run(100 - motorSpeed);
  }
  else {
    brake();
    curr_d--;
    delay(100);
    moved = false;
    checkNode();
  }
}
void brake() {
  motor1.stop();
  motor2.stop();
  motor3.stop();
  motor4.stop();
}


void checkNode() {
  if (starting == true) {
    curr_y = 0;
    curr_x = 6;
    starting = false;
    path_step--;
  }
  if (moved == true) {
    path_step++;
    pathx = path[path_step][0];
    pathy = path[path_step][1];
    Serial.print(path_step);
    Serial.print("_path_step_");
    Serial.print(pathx);
    Serial.print("_____");
    Serial.println(pathy);
    switch (curr_d) {
      //north
      case 1 : curr_x--; break;
      //east
      case 2: curr_y++; break;
      //south
      case 3: curr_x++; break;
      //west
      case 4: curr_y--; break;
      default: break;

    }
  }
  Serial.print(pathx);
  Serial.print("_____");
  Serial.println(pathy);
  if (pathx == -1 && pathy == -1) {
    buzzer.tone(262, 0.25 * 1000);
    brake();
    delay(20000);
  }
  else if (pathx < curr_x) {
    d_needed = 1;
  }
  else if (pathx > curr_x) {
    d_needed = 3;
  }
  else if (pathy > curr_y) {
    d_needed = 2;
  }
  else if (pathy < curr_y) {
    d_needed = 4;
  }

  if (curr_d == d_needed) {
    tl = false;
    tr = false;
    at_stop == false;
    tring = 0;
  }
  else if (curr_d - d_needed == 1 || curr_d - d_needed == -3) {
    tl = true;
    at_stop == false;
    tring = 0;
    turnL();
  }
  else {
    tr = true;
    at_stop == false;
    tring = 0;
    turnR();
  }
}

void aStar () {
  path = {
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1},
    { -1, -1}
  };




}
