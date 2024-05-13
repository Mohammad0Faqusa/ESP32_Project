#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64  // Change to 32 if you have a 128x32 display

#define OLED_RESET    -1 
#define SCREEN_ADDRESS 0x3C  // OLED display address

#define x_pin 25 
#define y_pin 26
#define calibrate_button 5 


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int y_min ; 
int y_max ; 
int x_min ; 
int x_max ; 
int x , y ; 

int y_cord  , x_cord  ; 

int button_state ; 

void setup() {
  Serial.begin(9600);

  // Initialize display
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  // Clear the buffer
  initialize_pins() ; 
  
  calibrate_sensors() ; 

}



void loop() {
  read_analog_sensors() ;
  mapping_sensors() ; 

  // Nothing in the loop for this example
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

  button_state = !(digitalRead(calibrate_button));
  display.print("Back_sense :");
  display.println("") ; 
  
  display.print("Lights : ");
  display.println(button_state) ; 

  // Display buffer contents
  display.display();
  delay(500) ; 
}

void calibrate_display() {
  display.clearDisplay();

  // Set text color to white
  display.setTextColor(SSD1306_WHITE);

  // Set text size
  display.setTextSize(2);

  // Set cursor position
  display.setCursor(0, 0);

  // Display text
  display.println("Calibrating");
  
  // Display buffer contents
  display.display();

}

void calibrate_sensors(){
  	calibrate_display(); 
     
    button_state = digitalRead(calibrate_button);

  	read_analog_sensors() ; 
    y_min = y ; 
    y_max = y ; 

    x_min = x ; 
    x_max = x ; 
 
  for(;;) {
    
    button_state = digitalRead(calibrate_button);

    delay(100); 

    if (button_state ==  0 ) 
      break ; 
    read_analog_sensors() ;   

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

  if (y_cord > -30 && y_cord < 30 ) 
    y_cord = 0 ; 

  if(y_cord > 235) 
    y_cord = 255 ; 


  x_cord = map(x , y_min , y_max , 0 , 500) - 255 ;

  if (x_cord > -30 && x_cord < 30 ) 
    x_cord = 0 ; 

   if(x_cord > 235) 
    x_cord = 255 ; 
  
}