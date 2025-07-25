#include <Servo.h> 
#include <SoftwareSerial.h> 

SoftwareSerial BTSerial(7, 8); 
// Define servo objects 
Servo servo1; 
Servo servo2;  
Servo servo3;  
const int servoPin1 = 3; 
const int servoPin2 = 5; 
const int servoPin3 = 6; 

const int REST_POSITION = 0; 
int repetitions; 
String X; 

void setup() { 
Serial.begin(38400);  
BTSerial.begin(38400); 
servo1.attach(servoPin1); 
servo2.attach(servoPin2); 
servo3.attach(servoPin3); 
Serial.println("Patient Glove Ready"); 
BTSerial.println("READY"); 
} 
void loop() { 
if (BTSerial.available()) {  
String receivedData = BTSerial.readStringUntil('\n'); 
receivedData.trim(); 
24 
// Handle Repetition Command 
if (receivedData.startsWith("REPS ")){ 
repetitions = receivedData.substring(5).toInt(); 
Serial.print("Repetitions set to: "); 
Serial.println(repetitions); 
return repetitions; 
return; 
} 
// Handle Flex Sensor Data (e.g., "0,180,90") 
int angle1, angle2, angle3; 
if (sscanf(receivedData.c_str(), "%d,%d,%d", &angle1, &angle2, &angle3) == 3) { 
Serial.println("Executing movement..."); 
for (int i = 0; i < repetitions; i++) { 
if (angle1 > 10) servo1.write(angle1); 
if (angle2 > 10) servo2.write(angle2); 
if (angle3 > 10) servo3.write(angle3); 
delay(1000);  
// Return to rest position
servo1.write(REST_POSITION); 
servo2.write(REST_POSITION); 
servo3.write(REST_POSITION); 
delay(1000); 
} 
} 
} 
} 
Serial.println("DONE"); 
BTSerial.println("DONE"); 
