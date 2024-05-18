#include <ESP8266WiFi.h>
#include "thingProperties.h"

// WiFi credentials
const char* ssid = "ZTE_2.4G_EAA2";
const char* password = "13141516";


// Pin connected to built-in LED (NodeMCU: D4 = GPIO2)
const int ledPin = LED_BUILTIN;

const int motor1A = D1;  // INPUT 4 OF DRIVER 
const int motor1B = D2;  // INPUT 3 OF DRIVER 


// Motor B (right motor)
const int motor2A = D4;  //INPUT 1 OF DRIVER 
const int motor2B = D5;  // Input 2 OF DRIVER 
const int motor2Enable = D6;  // ENABLE FOR (BACK , FORWARD ) MOTOR 


const int led_pin = D3 ; 



void setup() {

  initialize_pins() ; 

  // Connect to WiFi
  connectToWiFi();

  Serial.begin(9600);

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

int  distance = 0;
bool back  ; 

void loop() {

  ArduinoCloud.update();

 
  delay(100) ; 

  if (Serial.available() >= sizeof(int)) {
    Serial.readBytes((char*)&distance, sizeof(distance)); // Read distance data from Arduino Uno
    Serial.print("Received distance: ");
    Serial.println(distance); // Print received distance
  }

  delay(100) ; 
  
  back_distance = distance ; 

  

  if (back) {
    if(distance > 20) {
      analogWrite(motor2Enable , 255 );
      y_direction(false) ;
      delay(500) ; 
      analogWrite(motor2Enable , 0 );
    }
    else {
      go_back = false ; 
    }
  }

}


void onGoBackChange() {
  back = go_back ; 
}

void onXPwmChange()  {
  // Add your code here to act upon XPwm change
  if (x_pwm > 0) {
    x_direciton(true) ; 
  } else {
    x_direciton(false) ;
  }
}


void onYPwmChange()  {

  if (y_pwm > 0) {
    analogWrite(motor2Enable , y_pwm );
    y_direction(true) ; 
  } else {
    analogWrite(motor2Enable , - y_pwm );
    y_direction(false) ;
  }

}

void onBackDistanceChange()  {
  // Add your code here to act upon BackDistance change
  
}

void onLightCarChange()  {
  // Add your code here to act upon LightCar change
  if (light_car) {
    digitalWrite(led_pin, HIGH) ; 

  } else {
    digitalWrite(led_pin, LOW) ; 
  }
}

void initialize_pins() {
  // Initialize the ultrasonic sensor pins

  pinMode(motor1A, OUTPUT);
  pinMode(motor1B, OUTPUT);

  pinMode(motor2A, OUTPUT);
  pinMode(motor2B, OUTPUT);
  pinMode(motor2Enable , OUTPUT);

  pinMode(led_pin , OUTPUT);

}


void x_direciton(bool dir) {
  if (dir) {
    digitalWrite(motor1A , HIGH ) ; 
    digitalWrite(motor1B,  LOW ) ; 
  } else {
    digitalWrite(motor1A , LOW ) ; 
    digitalWrite(motor1B,  HIGH ) ; 
  }
}

void y_direction(bool dir) {
  if (dir) {
    digitalWrite(motor2A , HIGH ) ; 
    digitalWrite(motor2B,  LOW ) ; 
  } else {
    digitalWrite(motor2A , LOW ) ; 
    digitalWrite(motor2B,  HIGH ) ; 
  }
}



void connectToWiFi() {
  Serial.print("Connecting to WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

