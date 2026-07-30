#include "TeensyStep.h"

Stepper right_motor(2, 3);       // STEP pin: 2, DIR pin: 3
Stepper left_motor(6, 7);       // STEP pin: 6, DIR pin: 7

#define right_EN  4
#define left_EN  8

#define right_TRIGGER 10
#define left_TRIGGER 12

char user_input;
int rightTriggerState;
int leftTriggerState;
bool stimulusFired;

int delay_length =500;

// ear stim
int right_motor_direction = -1;  // 1=CCW, -1=CW
int right_motor_num_steps = 100;
int right_motor_max_speed = 7500;
int right_motor_acceleration = 500000;
int right_motor_pullInSpeed = 1500;

// whisker stim
int left_motor_direction = -1;  // 1=CCW, -1=CW
int left_motor_num_steps = 200;
int left_motor_max_speed = 60000;
int left_motor_acceleration = 500000;
int left_motor_pullInSpeed = 1500;

int previousTime;
int currentTime;

StepControl controller;    // Use default settings 

void setup()
{
  
    right_motor.setMaxSpeed(right_motor_max_speed);
    right_motor.setAcceleration(right_motor_acceleration);
    right_motor.setPullInSpeed(right_motor_pullInSpeed);

    left_motor.setMaxSpeed(left_motor_max_speed);
    left_motor.setAcceleration(left_motor_acceleration);
    left_motor.setPullInSpeed(left_motor_pullInSpeed);

    pinMode(right_EN, OUTPUT);
    pinMode(left_EN, OUTPUT);

    pinMode(right_TRIGGER, INPUT);
    pinMode(left_TRIGGER, INPUT);

    digitalWrite(right_EN, HIGH); //Pull enable pin high to disable motor control
    digitalWrite(left_EN, HIGH); //Pull enable pin high to disable motor control
}


void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      
      if (user_input =='r')
      {
         right_Stimulus();
      }
      if (user_input =='l')
      {
         left_Stimulus();
      }
      if (user_input =='b')
      {
         Both_Stimulus();
      }

      if (user_input =='q')
      {
         right_Nudge_CW();
      }
      if (user_input =='a')
      {
         right_Nudge_CCW();
      }
      if (user_input =='w')
      {
         left_Nudge_CW();
      }
      if (user_input =='s')
      {
         left_Nudge_CCW();
      }
      
  }

  rightTriggerState = digitalRead(right_TRIGGER);
  leftTriggerState = digitalRead(left_TRIGGER);

  if (rightTriggerState==HIGH || leftTriggerState==HIGH) {
    delay(1); // give the other trigger a chance to occur

    rightTriggerState = digitalRead(right_TRIGGER);
    leftTriggerState = digitalRead(left_TRIGGER);
  
    if (rightTriggerState==HIGH & leftTriggerState==HIGH) {
        Serial.println("Both stimulus trigger detected");
        Both_Stimulus();
        while (rightTriggerState == HIGH || leftTriggerState==HIGH) {
          rightTriggerState = digitalRead(right_TRIGGER);
          leftTriggerState = digitalRead(left_TRIGGER);
          delay(1);
        }
        return;
    }
    
    if (rightTriggerState == HIGH & leftTriggerState == LOW) {
        Serial.println("right stimulus trigger detected");
        right_Stimulus();
        while (rightTriggerState == HIGH) {
          rightTriggerState = digitalRead(right_TRIGGER);
          delay(1);
        }
        return;
    }
  
    if (rightTriggerState == LOW & leftTriggerState == HIGH) {
        Serial.println("left stimulus trigger detected");
        left_Stimulus();
        while (leftTriggerState == HIGH) {
          leftTriggerState = digitalRead(left_TRIGGER);
          delay(1);
        }
        return;
    }
  }
}

void right_Stimulus()
{
  
  Serial.println("right Stimulus");
  digitalWrite(right_EN, LOW); //Pull enable pin low to allow motor control

  previousTime = millis();
  
  right_motor.setTargetRel(right_motor_num_steps*right_motor_direction);
  controller.move(right_motor);    // Do the move

  delay(delay_length);

  right_motor.setTargetRel(-right_motor_num_steps*right_motor_direction);
  controller.move(right_motor);    // Do the move

  currentTime = millis();
  Serial.println(currentTime - previousTime);

  delay(50);
  digitalWrite(right_EN, HIGH); //Pull enable pin high to disable motor control

}

void left_Stimulus()
{
  
  Serial.println("left Stimulus");
  digitalWrite(left_EN, LOW); //Pull enable pin low to allow motor control
  
  left_motor.setTargetRel(left_motor_num_steps*left_motor_direction);
  controller.move(left_motor);    // Do the move

  delay(delay_length);

  left_motor.setTargetRel(-left_motor_num_steps*left_motor_direction);
  controller.move(left_motor);    // Do the move

  
  delay(50);
  digitalWrite(left_EN, HIGH); //Pull enable pin high to disable motor control

}


void Both_Stimulus()
{
  
  Serial.println("Both Stimulus");
  digitalWrite(right_EN, LOW); //Pull enable pin low to allow motor control
  digitalWrite(left_EN, LOW); //Pull enable pin low to allow motor control
  
  right_motor.setTargetRel(right_motor_num_steps*right_motor_direction);
  left_motor.setTargetRel(left_motor_num_steps*left_motor_direction);
  controller.move(right_motor, left_motor);    // Do the move

  delay(delay_length);

  right_motor.setTargetRel(-right_motor_num_steps*right_motor_direction);
  left_motor.setTargetRel(-left_motor_num_steps*left_motor_direction);
  controller.move(right_motor, left_motor);    // Do the move

  delay(50);
  digitalWrite(right_EN, HIGH); //Pull enable pin high to disable motor control
  digitalWrite(left_EN, HIGH); //Pull enable pin high to disable motor control

}


void left_Nudge_CW()
{

  digitalWrite(left_EN, LOW); //Pull enable pin low to allow motor control

  left_motor.setTargetRel(10);
  controller.move(left_motor);    // Do the move

  delay(50);
  digitalWrite(left_EN, HIGH); //Pull enable pin high to disable motor control

}

void left_Nudge_CCW()
{

  digitalWrite(left_EN, LOW); //Pull enable pin low to allow motor control

  left_motor.setTargetRel(-10);
  controller.move(left_motor);    // Do the move

  delay(50);
  digitalWrite(left_EN, HIGH); //Pull enable pin high to disable motor control

}

void right_Nudge_CW()
{

  digitalWrite(right_EN, LOW); //Pull enable pin low to allow motor control

  right_motor.setTargetRel(10);
  controller.move(right_motor);    // Do the move

  delay(50);
  digitalWrite(right_EN, HIGH); //Pull enable pin high to disable motor control

}

void right_Nudge_CCW()
{

  digitalWrite(right_EN, LOW); //Pull enable pin low to allow motor control

  right_motor.setTargetRel(-10);
  controller.move(right_motor);    // Do the move

  delay(50);
  digitalWrite(right_EN, HIGH); //Pull enable pin high to disable motor control

}
