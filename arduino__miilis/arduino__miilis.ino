// Include necessary libraries
#include <Servo.h>                  //for airpump
#include <NewPing.h>                //for distance monitor
#include <PulseSensorPlayground.h>  // Includes the PulseSensorPlayground Library.


// Define pins for air pumps, air valve, LEDs, and ultrasonic sensor
Servo servo_8;
Servo servo_9;
Servo servo_10;

const int led2 = 11;
const int triggerPin = 2;
const int echoPin = 3;

int xintiaoPin = 1;   // heartmonitor input A1
int led1 = 13;        // led1 output
int Threshold = 550;  // Determine which Signal to "count as a beat" and which to ignore.



// Define constants for ultrasonic sensor
const int maxDistance = 200;
NewPing sonar(triggerPin, echoPin, maxDistance);

//defines for smoothing the distance
const int numReadings = 5;

int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average


// Define variables for timing
unsigned long previousMillis = 0;
unsigned long previousMillis0 = 0;
unsigned long led2previousMillis = 0;
unsigned long led1previousMillis = 0;
const long interval = 4000;         // 4 seconds interval
unsigned long led1Millis = 0;       //timetable for LED1
const long led1Interval = 5;        // 1 second interval for LED light 1
const long led2Interval = 10;       // 0.01 second interval for LED light 2
unsigned long led2Millis = 0;       //timetable for LED2
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

int val = 0;  // define val

void setup() {
  // Set pin modes
  servo_8.attach(8);
  servo_9.attach(9);
  servo_10.attach(10);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


  // Configure the PulseSensor object, by assigning our variables to it.
  // pulseSensor.analogInput(PulseWire);
  // pulseSensor.blinkOnPulse(led1);  //auto-magically blink Arduino's LED with heartbeat.
  // pulseSensor.setThreshold(Threshold);

  //Configure the PulseSensor object, by assigning our variables to it.


  Serial.begin(9600);  //
}

void loop() {



  unsigned long currentMillis0 = millis();  // to make computer check everything every 0.1 seconds
  if (currentMillis0 - previousMillis0 >= 1)
    ;
  {
    currentMillis0 = previousMillis0;
    // Read distance from ultrasonic sensor all the time
    // The result should be used to turn airpumps on and off
    // and to adjust led2

    int distance = sonar.ping_cm();



    delay(50);
    Serial.print("distance: ");
    Serial.println(sonar.ping_cm());  // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println("cm");

    //for heartmonitor and led

    unsigned long led1currentMillis = millis();
    if (led1currentMillis - led1previousMillis >= 3) {
      int sensorValue = analogRead(xintiaoPin);  //read the number heartmonitor detected
      Serial.println(sensorValue, DEC);          //输出到串口
      if (sensorValue > 346)                     //
      {
        led1previousMillis = led1currentMillis;
        digitalWrite(led1, HIGH);  //
      } else {
        digitalWrite(led1, LOW);
        led1previousMillis = led1currentMillis;
      }
    }



    // Control air pumps and air valve

    // Check if the distance is less than 50 cm and the time elapsed since the
    // last action is greater than or equal to the interval (4 seconds)
    unsigned long currentMillis = millis();
    unsigned long led2currentMillis = millis();

    // Update previousMillis when the condition is met

    // Toggle air pumps and air valve

    if (distance <= 40 && distance > 3) {

      if (led2currentMillis - led2previousMillis >= led2Interval) {
        // save the last time you blinked the LED
        led2currentMillis = led2previousMillis;
        val = map(distance, 0, 40, 180, 0);  // give "distance" to val
        Serial.println(val);                 //shouw val on monitor
        analogWrite(led2, val);              // turn LED on and set its brightness using PWM (maxi=255)
      }

      if (currentMillis - previousMillis <= 2000) {

        servo_8.write(180);
        servo_9.write(0);
        servo_10.write(0);
      }
      if (currentMillis - previousMillis > 2000 && currentMillis - previousMillis <= 2500) {

        servo_8.write(0);
        servo_9.write(0);
        servo_10.write(180);
      }
      if (currentMillis - previousMillis > 2500 && currentMillis - previousMillis <= 4500) {

        servo_8.write(0);
        servo_9.write(180);
        servo_10.write(180);
      }
      if (currentMillis - previousMillis > 4500) {
        previousMillis = currentMillis;
      }
    } else {
      servo_8.write(0);
      servo_9.write(0);
      servo_10.write(0);
      previousMillis = currentMillis;
    }
  }
  delay(50);
}
