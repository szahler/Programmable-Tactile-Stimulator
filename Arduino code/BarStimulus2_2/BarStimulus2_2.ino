#include "TeensyStep.h"

Stepper left_motor(2, 3);       // STEP pin: 2, DIR pin: 3
Stepper right_motor(6, 7);       // STEP pin: 6, DIR pin: 7

#define Left_EN  4
#define Right_EN  8

#define Left_TRIGGER 10
#define Right_TRIGGER 12

char user_input;
int LeftTriggerState;
int RightTriggerState;
bool stimulusFired;

int delay_length = 0;

int left_motor_direction = -1;  // 1=CCW, -1=CW
int left_motor_num_steps = 200;
int left_motor_max_speed = 60000;
int left_motor_acceleration = 500000;
int left_motor_pullInSpeed = 1500;

int right_motor_direction = -1;  // 1=CCW, -1=CW
int right_motor_num_steps = 200;
int right_motor_max_speed = 60000;
int right_motor_acceleration = 500000;
int right_motor_pullInSpeed = 1500;

int previousTime;
int currentTime;

StepControl controller;    // Use default settings 

void setup()
{
  
    left_motor.setMaxSpeed(left_motor_max_speed);
    left_motor.setAcceleration(left_motor_acceleration);
    left_motor.setPullInSpeed(left_motor_pullInSpeed);

    right_motor.setMaxSpeed(right_motor_max_speed);
    right_motor.setAcceleration(right_motor_acceleration);
    right_motor.setPullInSpeed(right_motor_pullInSpeed);

    pinMode(Left_EN, OUTPUT);
    pinMode(Right_EN, OUTPUT);

    pinMode(Left_TRIGGER, INPUT);
    pinMode(Right_TRIGGER, INPUT);

    digitalWrite(Left_EN, HIGH); //Pull enable pin high to disable motor control
    digitalWrite(Right_EN, HIGH); //Pull enable pin high to disable motor control
}


void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
      user_input = Serial.read(); //Read user input and trigger appropriate function
      
      if (user_input =='l')
      {
         Left_Stimulus();
      }
      if (user_input =='r')
      {
         Right_Stimulus();
      }
      if (user_input =='b')
      {
         Both_Stimulus();
      }

      if (user_input =='q')
      {
         Left_Nudge_CW();
      }
      if (user_input =='a')
      {
         Left_Nudge_CCW();
      }
      if (user_input =='w')
      {
         Right_Nudge_CW();
      }
      if (user_input =='s')
      {
         Right_Nudge_CCW();
      }
      
  }

  LeftTriggerState = digitalRead(Left_TRIGGER);
  RightTriggerState = digitalRead(Right_TRIGGER);

  if (LeftTriggerState==HIGH || RightTriggerState==HIGH) {
    delay(1); // give the other trigger a chance to occur

    LeftTriggerState = digitalRead(Left_TRIGGER);
    RightTriggerState = digitalRead(Right_TRIGGER);
  
    if (LeftTriggerState==HIGH & RightTriggerState==HIGH) {
        Serial.println("Both stimulus trigger detected");
        Both_Stimulus();
        while (LeftTriggerState == HIGH || RightTriggerState==HIGH) {
          LeftTriggerState = digitalRead(Left_TRIGGER);
          RightTriggerState = digitalRead(Right_TRIGGER);
          delay(1);
        }
        return;
    }
    
    if (LeftTriggerState == HIGH & RightTriggerState == LOW) {
        Serial.println("Left stimulus trigger detected");
        Left_Stimulus();
        while (LeftTriggerState == HIGH) {
          LeftTriggerState = digitalRead(Left_TRIGGER);
          delay(1);
        }
        return;
    }
  
    if (LeftTriggerState == LOW & RightTriggerState == HIGH) {
        Serial.println("Right stimulus trigger detected");
        Right_Stimulus();
        while (RightTriggerState == HIGH) {
          RightTriggerState = digitalRead(Right_TRIGGER);
          delay(1);
        }
        return;
    }
  }
}

void Left_Stimulus()
{
  
  Serial.println("Left Stimulus");
  digitalWrite(Left_EN, LOW); //Pull enable pin low to allow motor control

  previousTime = millis();
  
  left_motor.setTargetRel(left_motor_num_steps*left_motor_direction);
  controller.move(left_motor);    // Do the move

  delay(delay_length);

  left_motor.setTargetRel(-left_motor_num_steps*left_motor_direction);
  controller.move(left_motor);    // Do the move

  currentTime = millis();
  Serial.println(currentTime - previousTime);

  delay(50);
  digitalWrite(Left_EN, HIGH); //Pull enable pin high to disable motor control

}

void Right_Stimulus()
{
  
  Serial.println("Right Stimulus");
  digitalWrite(Right_EN, LOW); //Pull enable pin low to allow motor control
  
  right_motor.setTargetRel(right_motor_num_steps*right_motor_direction);
  controller.move(right_motor);    // Do the move

  delay(delay_length);

  right_motor.setTargetRel(-right_motor_num_steps*right_motor_direction);
  controller.move(right_motor);    // Do the move

  delay(50);
  digitalWrite(Right_EN, HIGH); //Pull enable pin high to disable motor control

}


void Both_Stimulus()
{
  
  Serial.println("Both Stimulus");
  digitalWrite(Left_EN, LOW); //Pull enable pin low to allow motor control
  digitalWrite(Right_EN, LOW); //Pull enable pin low to allow motor control
  
  left_motor.setTargetRel(left_motor_num_steps*left_motor_direction);
  right_motor.setTargetRel(right_motor_num_steps*right_motor_direction);
  controller.move(left_motor, right_motor);    // Do the move

  delay(delay_length);

  left_motor.setTargetRel(-left_motor_num_steps*left_motor_direction);
  right_motor.setTargetRel(-right_motor_num_steps*right_motor_direction);
  controller.move(left_motor, right_motor);    // Do the move

  delay(50);
  digitalWrite(Left_EN, HIGH); //Pull enable pin high to disable motor control
  digitalWrite(Right_EN, HIGH); //Pull enable pin high to disable motor control

}


void Right_Nudge_CW()
{

  digitalWrite(Right_EN, LOW); //Pull enable pin low to allow motor control

  right_motor.setTargetRel(10);
  controller.move(right_motor);    // Do the move

  delay(50);
  digitalWrite(Right_EN, HIGH); //Pull enable pin high to disable motor control

}

void Right_Nudge_CCW()
{

  digitalWrite(Right_EN, LOW); //Pull enable pin low to allow motor control

  right_motor.setTargetRel(-10);
  controller.move(right_motor);    // Do the move

  delay(50);
  digitalWrite(Right_EN, HIGH); //Pull enable pin high to disable motor control

}

void Left_Nudge_CW()
{

  digitalWrite(Left_EN, LOW); //Pull enable pin low to allow motor control

  left_motor.setTargetRel(10);
  controller.move(left_motor);    // Do the move

  delay(50);
  digitalWrite(Left_EN, HIGH); //Pull enable pin high to disable motor control

}

void Left_Nudge_CCW()
{

  digitalWrite(Left_EN, LOW); //Pull enable pin low to allow motor control

  left_motor.setTargetRel(-10);
  controller.move(left_motor);    // Do the move

  delay(50);
  digitalWrite(Left_EN, HIGH); //Pull enable pin high to disable motor control

}
