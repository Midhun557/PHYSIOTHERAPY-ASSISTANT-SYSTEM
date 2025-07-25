#include <SoftwareSerial.h> 
SoftwareSerial BTSerial(7, 8); 

const int flexPin1 = A0; 
const int flexPin2 = A1;  
const int flexPin3 = A2; 
// Threshold values (adjust based on your flex sensor calibration) 
const int REST_VALUE1 = 40;  
const int REST_VALUE2 = 37; 
const int REST_VALUE3 = 35; 

int lastAngle1 = 0, lastAngle2 = 0, lastAngle3 = 0; 

bool waitingForAcknowledgment = false;  
bool sessionActive = false;   
int repetitions = 5;  // Default repetitions 
void setup() { 
Serial.begin(38400);  
BTSerial.begin(38400); 
Serial.println("Waiting for Bluetooth pairing..."); 
Serial.println("Enter the no of Repetitions"); 
delay(3000);   
Serial.println("READY");   
BTSerial.println("READY"); 
} 
void loop() { 
// Check for USB Commands from Serial USB Terminal App (Phone/PC) 
if (Serial.available()) { 
String usbCommand = Serial.readStringUntil('\n'); 
usbCommand.trim(); 
processCommand(usbCommand); 
19 
} 
//  Check for Bluetooth Commands from Patient's Glove 
if (BTSerial.available()) { 
String btCommand = BTSerial.readStringUntil('\n'); 
btCommand.trim(); 
processCommand(btCommand); 
} 
//  Send flex sensor data only when session is active 
if (sessionActive && !waitingForAcknowledgment) { 
sendFlexData(); 
} 
} 
// Function to Process Commands from USB or Bluetooth 
void processCommand(String command) { 
if (command == "RTMODE") { 
sessionActive = true; 
Serial.println("Therapy Started"); 
BTSerial.println("Therapy Started"); 
}  
else if (command == "STOP") { 
sessionActive = false; 
Serial.println("Therapy Stopped"); 
BTSerial.println("Therapy Stopped"); 
}  
else if (command.startsWith("REPS ")) { 
repetitions = command.substring(5).toInt(); 
Serial.print("REPS "); 
Serial.println(repetitions); 
BTSerial.print("REPS "); 
BTSerial.println(repetitions); 
}  
else if(command=="MOD1") 
{ 
int angle1=0; 
int angle2=0; 
int angle3=180; 
sessionActive=false; 
Serial.println("Going in Mod1"); 
String data = String(angle1) + "," + String(angle2) + "," + String(angle3); 
20 
Serial.println("Sending Data: " + data); 
BTSerial.println(data);   
} 
else if(command=="MOD2") 
{ 
int angle1=0; 
int angle2=180; 
int angle3=180; 
sessionActive=false; 
Serial.println("Going in Mod2"); 
String data = String(angle1) + "," + String(angle2) + "," + String(angle3); 
Serial.println("Sending Data: " + data); 
BTSerial.println(data);   
} 
else if(command=="MOD3") 
{ 
int angle1=180; 
int angle2=180; 
int angle3=180; 
sessionActive=false; 
Serial.println("Going in Mod1"); 
String data = String(angle1) + "," + String(angle2) + "," + String(angle3); 
Serial.println("Sending Data: " + data); 
BTSerial.println(data);   
} 
else if(command=="MOD4") 
{ 
int angle1=180; 
int angle2=0; 
int angle3=180; 
sessionActive=false; 
Serial.println("Going in Mod1"); 
String data = String(angle1) + "," + String(angle2) + "," + String(angle3); 
Serial.println("Sending Data: " + data); 
BTSerial.println(data);   
} 
else if (command == "CALIBRATE") { 
Serial.println("Calibration Started..."); 
delay(3000); 
Serial.println("Calibration Complete"); 
21 
} 
else if (command == "DONE") { 
Serial.println("Received DONE, ready for next session."); 
waitingForAcknowledgment = false; 
delay(1000); 
}    
} 
// Function to Read Flex Sensors and Send Data via Bluetooth 
void sendFlexData() { 

int sum1 = 0, sum2 = 0, sum3 = 0; 
const int samples = 20;   
for (int i = 0; i < samples; i++) { 
sum1 += analogRead(flexPin1); 
sum2 += analogRead(flexPin2); 
sum3 += analogRead(flexPin3); 
delay(50); 
} 
 
int avg1 = sum1 / samples; 
int avg2 = sum2 / samples; 
int avg3 = sum3 / samples; 
 
int angle1 = (avg1 > REST_VALUE1) ? 0 : 180; 
int angle2 = (avg2 > REST_VALUE2) ? 0 : 180; 
int angle3 = (avg3 > REST_VALUE3) ? 0 : 180; 

if (angle1 != lastAngle1 || angle2 != lastAngle2 || angle3 != lastAngle3) { 
lastAngle1 = angle1; 
lastAngle2 = angle2; 
lastAngle3 = angle3; 

if (angle1 == 0 && angle2 == 0 && angle3 == 0) { 
Serial.println("RESET DETECTED: Fingers at rest"); 
} else { 

String data = String(angle1) + "," + String(angle2) + "," + String(angle3); 
Serial.println("Sending Data: " + data); 
BTSerial.println(data);   
22 
Serial.println("Waiting for DONE from Patient's Glove..."); 
waitingForAcknowledgment = true; 
} 
} 
} 

void checkForAcknowledgment() { 
if (waitingForAcknowledgment && BTSerial.available()) { 
String response = BTSerial.readStringUntil('\n'); 
response.trim(); 
Serial.println(response); 
if (response == "DONE") { 
Serial.println("Received DONE, ready for next session."); 
waitingForAcknowledgment = false; 
delay(5000); 
} 
} 
}
