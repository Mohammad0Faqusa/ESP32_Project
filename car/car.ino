

#include "thingProperties.h"
#include <ESP8266WiFi.h>

const char* ssid = "ZTE_2.4G_EAA2";
const char* password = "13141516";



#define motor_x_1 12 
#define motor_x_2 13 
#define motor_y_1 14
#define motor_y_2 15 

#define x_pwm_pin 16 
#define y_pwm_pin 17 

#define light_pin 18 

void setup() {

  Serial.begin(9600);

  wifi_connection() ; 

  cloud_setup() ; 


}


void loop() {
  ArduinoCloud.update();
 
  // here you should insert the ultra sonic sensor function 
  
}



void onXPwmChange()  {

  x_pwm_to_direction() ; 
  analogWrite(x_pwm_pin , abs(x_pwm)) ; 

}

void onYPwmChange()  {
  y_pwm_to_direction() ; 
  analogWrite(y_pwm_pin , abs(x_pwm)) ; 
}


void onLightChange()  {
  if (light) {
    digitalWrite(light_pin , HIGH) ; 
  } else {
    digitalWrite(light_pin , LOW) ; 
  }
}


void onBackDistanceChange()  {
  
}

x_pwm_to_direction() {

  if (x_pwm > 0) {
    x_direction(true) ; 
  } else {
    x_direction(false) ; 
  }

}

y_pwm_to_direction() {

  if (x_pwm > 0) {
    y_direction(true) ; 
  } else {
    y_direction(false) ; 
  }

}


void x_direction(bool dir) {
  if (dir) {
    digitalWrite(motor_x_1 , HIGH) ; 
    digitalWrite(motor_x_2 , LOW) ; 
  } else {
    digitalWrite(motor_x_1 , LOW) ; 
    digitalWrite(motor_x_2 , HIGH) ; 
  }
}

void y_direction(bool dir) {
  if (dir) {
    digitalWrite(motor_y_1 , HIGH) ; 
    digitalWrite(motor_y_2 , LOW) ; 
  } else {
    digitalWrite(motor_y_1 , LOW) ; 
    digitalWrite(motor_y_2 , HIGH) ; 
  }
}


void wifi_connection() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  }

}

void cloud_setup() {
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}