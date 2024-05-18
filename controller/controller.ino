#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include "thingProperties.h"

const char* ssid = "ZTE_2.4G_EAA2";
const char* password = "13141516";


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64  // Change to 32 if you have a 128x32 display

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C  // OLED display address

#define x_pin 34 
#define y_pin 35
#define calibrate_button 5 

 


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int y_min ; 
int y_max ; 
int x_min ; 
int x_max ; 

int x , y ; 

int y_cord  , x_cord  ; 

int button_state ; 

bool toggle_button ;

float back_d ; 


void setup() {

  initialize_pins() ; 

  Serial.begin(9600);

  wifi_connection() ; 

  cloud_setup() ;   

  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(("SSD1306 allocation failed"));
    for(;;);
  }

  calibrate_sensors() ; 

  
}



void loop() {


  ArduinoCloud.update();

  read_analog_sensors() ;

  mapping_sensors() ; 

  button_state = !(digitalRead(calibrate_button));

  if (button_state) {
    toggle_button = ! toggle_button ; 
  }

  middle_button = toggle_button ; 

  display_variables() ; 

  x_cloud = x_cord ; 
  y_cloud = y_cord ; 

}

void onDistanceChange(){
  back_d = distance ; 
}

void onMiddleButtonChange()  {
  
}



void onXCloudChange()  {
  
}


void onYCloudChange()  {
 
}

void cloud_setup() {

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  
  ArduinoCloud.printDebugInfo(); 
  
}


void display_variables() {
  display.clearDisplay();

  // Set text color to white
  display.setTextColor(SSD1306_WHITE);

  // Set text size
  display.setTextSize(1.5);

  // Set cursor position
  display.setCursor(0, 0);


  display.print("the x = ") ; 
  display.println(x_cord) ; 
  display.print("the y = ") ; 
  display.println(y_cord) ;

  
  display.print("Back_sense :");
  display.println(back_d) ; 
  
  display.print("Lights : ");
  display.println(toggle_button) ; 

  // Display buffer contents
  display.display();
  delay(500) ;  
}

void calibrate_display() {
  display.clearDisplay();

  // Set text color to white
  display.setTextColor(SSD1306_WHITE);

  // Set text size
  display.setTextSize(1);

  // Set cursor position
  display.setCursor(0, 0);

  // Display text
  display.println("Calibrating");
  
  // Display buffer contents
  display.print("x value = ") ; 
  display.println(x) ; 
  display.print("The min x = ") ; 
  display.println(x_min) ; 
  display.print("The max x = ") ; 
  display.println(x_max) ;

  display.print("y value = ") ; 
  display.println(y) ; 
  display.print("The min y = ") ; 
  display.println(y_min) ; 
  display.print("The max y = ") ; 
  display.println(y_max) ;

   
  
  display.display();

}

void calibrate_sensors(){

  	read_analog_sensors() ; 

    y_min = y ; 
    y_max = y ; 

    x_min = x ; 
    x_max = x ; 
 
  for(;;) {

    calibrate_display(); 
    
    button_state = !(digitalRead(calibrate_button));

    delay(100); 

    if (button_state !=  0 ) 
      break ; 
    read_analog_sensors() ;   

    display.println(x) ; 

    if (x < x_min) 
    	x_min = x ; 
  	if (x > x_max) 
    	x_max = x ; 
  
  	if (y_min > y) 
    	y_min = y ; 
  	if (y_max < y) 
    	y_max = y ;
  
}
}

void read_analog_sensors() {
  	x = analogRead(x_pin) ; 
    y = analogRead(y_pin) ; 
}

void initialize_pins() {
  pinMode(x_pin , INPUT) ; 
  pinMode(y_pin , INPUT) ; 
  pinMode(calibrate_button , INPUT); 

}
void mapping_sensors(){
  
  y_cord = map(y , y_min , y_max , 0 , 500) - 255 ;

  if (y_cord > -50 && y_cord < 50 ) 
    y_cord = 0 ; 

  if(y_cord > 235) 
    y_cord = 255 ; 


  x_cord = map(x , x_min , x_max , 0 , 500) - 255 ;

  if (x_cord > -50 && x_cord < 50 ) 
    x_cord = 0 ; 

   if(x_cord > 235) 
    x_cord = 255 ; 
  
}


void wifi_connection() {

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
  }
}