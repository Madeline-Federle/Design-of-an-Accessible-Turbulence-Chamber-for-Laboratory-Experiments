#include <Servo.h> 

#define MOTOR_PIN2 2
#define MOTOR_PIN3 3
#define MOTOR_PIN4 4
#define MOTOR_PIN5 5
#define MOTOR_PIN6 6
#define MOTOR_PIN7 7
#define MOTOR_PIN8 8
#define MOTOR_PIN9 9

Servo motor2, motor3, motor4, motor5, motor6, motor7, motor8, motor9;

bool motorsRunning = false;

void setup() {
  Serial.begin(9600);  
  Serial.println("ELECTRONOOBS ESC calibration..."); 
  delay(1500); 
  Serial.println("Program begin..."); 
  delay(1000); 
  Serial.println("This program will start the ESC."); 

  // Attach motors
  motor2.attach(MOTOR_PIN2);  motor3.attach(MOTOR_PIN3);  motor4.attach(MOTOR_PIN4);  motor5.attach(MOTOR_PIN5);
  motor6.attach(MOTOR_PIN6);  motor7.attach(MOTOR_PIN7);  motor8.attach(MOTOR_PIN8);  motor9.attach(MOTOR_PIN9);

  // Calibration process
  Serial.println("Sending MAX signal. Turn on ESC power, then press any key...");
  setAllMotors(2000);
  while (!Serial.available());
  Serial.read();

  Serial.println("Sending MIN signal...");
  setAllMotors(1000);
  Serial.println("ESCs Calibrated.");
  Serial.println("Enter a number (1 to 9) to run motors, or 0 to stop. Motors will start after X seconds and run for Y seconds or until 0 is sent.");
}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n');
    inputString.trim(); // Remove any whitespace or newlines
    int input = inputString.toInt();

    if (input == 0) {
      if (motorsRunning) {
        Serial.println("Stop command received. Stopping motors...");
        setAllMotors(1000);
        motorsRunning = false;
      } else {
        Serial.println("Motors are already stopped.");
      }
    } 
    else if (input >= 1 && input <= 9) {
      int speedLevels[] = {0, 1120, 1145, 1170, 1195, 1220, 1245, 1270, 1295, 1320}; // *** options 1 - 
      int signal = speedLevels[input];
      Serial.print("Speed level selected: "); Serial.print(input);
      Serial.print(" (signal: "); Serial.print(signal); Serial.println(")");
      Serial.println("Starting in 1 seconds...");
      delay(1000); //wait time before running --> change this for delay before motors start running

      Serial.println("Motors running...");
      setAllMotors(signal);
      motorsRunning = true;

      unsigned long startTime = millis();
      while (millis() - startTime < 3000) { //Run time --> change this for amount of time you want motors to run 
        if (Serial.available() > 0) {
          String stopInput = Serial.readStringUntil('\n');
          stopInput.trim();
          if (stopInput == "0") {
            Serial.println("Stop command received during run. Stopping motors...");
            break;
          }
        }
      }

      setAllMotors(1000);
      Serial.println("Motors stopped.");
      motorsRunning = false;
    } 
    else {
      Serial.println("Invalid input. Enter a number between 0 and 5.");
    }
  }
}

void setAllMotors(int value) {
  motor2.writeMicroseconds(value);
  motor3.writeMicroseconds(value);
  motor4.writeMicroseconds(value);
  motor5.writeMicroseconds(value);
  motor6.writeMicroseconds(value);
  motor7.writeMicroseconds(value);
  motor8.writeMicroseconds(value);
  motor9.writeMicroseconds(value);
}
