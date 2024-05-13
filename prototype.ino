#include <Adafruit_LiquidCrystal.h>

int seconds = 0;




// notes in the melody:


#define motor_1_pin 5 
#define motor_2_pin 4
#define motor_enable_pin 3 

#define photo_pin A0 
#define tmp_pin A1 
#define gas_pin A2 
#define motion_pin 7 

#define led_orange 8 
#define led_yellow 9
#define led_green 10 
#define led_blue 11 

#define buzz_pin 12 

#define remote_pin 5 


#define push_button 13

int photo_degree ; 
int tmp_degree ; 
int gas_degree ; 
bool motion_degree ; 

int photo_min ; 
int photo_max ; 
int tmp_min; 
int tmp_max ; 
int gas_min ; 
int gas_max ;

int photo_pwm ; 
int tmp_c ; 
int gas_pwm ; 

int button_state ; 

int pwm_motor ;



Adafruit_LiquidCrystal lcd_1(0);


// this is interrupt come from remote pin from the other arduino board 

const int interruptPin = 2; // choose the appropriate pin
volatile int interruptCounter = 0;


void ISR_function() {
  interruptCounter++; // increment the counter
  pwm_motor = 0 ; 
  while(true) {
    button_state = digitalRead(interruptPin) ; 
    if (button_state == LOW) 
      break ; 
  }
  
}

void setup()
{ 
  pinMode(interruptPin, INPUT_PULLUP); // set the pin as input
  attachInterrupt(digitalPinToInterrupt(interruptPin), ISR_function, RISING); // attach interrupt
  Serial.begin(9600);
  
  initialize_pins();
    
  lcd_1.begin(16, 2);

  lcd_1.print("temprature");

  digitalWrite(led_orange , HIGH) ; 
  
  Serial.print("Please calibrate the sensors then press push button") ; 
  

  
  
  calibrate_sensors() ; 
  
  motor_direction(true) ; 
  
  
}
int remote_state ; 
void loop() {
  
  
  read_sensors();
  
  // display_sensors_inputValue() ;
  
  mapping_sensors();
  
  // display_mapped_sensors() ;
  
  display_lcd_tmp() ; 
  
  motor_controller() ; 
  
  motion_detection() ; 
  
  gas_emergancy() ; 
  
} 

void gas_emergancy(){
  if (gas_pwm > 40) {
    
    motor_direction(false) ;
    pwm_motor = 255 ; 
    
    tone( buzz_pin , 400, 500);
    delay(500) ; 
    noTone(buzz_pin) ; 
  }
  else{
    motor_direction(true) ;

  }
  
}


void motion_detection() {
  if(motion_degree) {
    turn_all_lights(photo_pwm) ; 
  }else{
    turn_all_lights(0) ; 
  }
  
}


void motor_controller() {
  if (gas_pwm > 40 ) {
    analogWrite(motor_enable_pin , pwm_motor) ; 
    return ;}
  
	if ( tmp_c > 20 && tmp_c < 28  ) {
      	pwm_motor = 100 ; 
    }
  	else if (tmp_c > 28 && tmp_c < 40) {
      	pwm_motor = map(tmp_c , tmp_c - 28 , 40 , 0 , 155) + 100 ; 
  	}
  	else if (tmp_c < 20) {
    	pwm_motor = 0 ; 
  	}
  	else {
    	pwm_motor = 255 ; 
  	}
  
  	analogWrite(motor_enable_pin , pwm_motor) ; 
  
}

void display_lcd_tmp(){
  
  lcd_1.setCursor(2, 1);
  lcd_1.print("  "); 
 
  lcd_1.setCursor(0, 1);
  lcd_1.print(tmp_c);
}

  
void read_analog_sensors() {
  	photo_degree = analogRead(photo_pin) ; 
  	tmp_degree = analogRead(tmp_pin) ; 
  	gas_degree = analogRead(gas_pin) ; 
}


void initialize_pins() {
  
  pinMode( motor_1_pin , OUTPUT) ; 
  pinMode( motor_2_pin , OUTPUT) ; 
  pinMode( motor_enable_pin , OUTPUT) ; 
  
  pinMode( led_yellow , OUTPUT) ; 
  pinMode( led_green , OUTPUT) ; 
  pinMode( led_blue , OUTPUT) ; 
  pinMode( led_orange , OUTPUT) ; 
  
  pinMode( photo_pin , INPUT) ; 
  pinMode( tmp_pin , INPUT) ; 
  pinMode( gas_pin , INPUT) ; 
  pinMode( motion_pin , INPUT) ;
  
  pinMode(push_button, INPUT) ; 

  pinMode(remote_pin, INPUT) ; 
}
void display_min_max(){
  Serial.print("photo_min_max : ") ; 
  Serial.print(photo_min) ; 
  Serial.print(photo_max) ; 
  
  Serial.print("gas : ") ; 
  Serial.print(gas_min) ; 
  Serial.print(gas_max) ;
  
  Serial.print("temprature  : ") ; 
  Serial.print(tmp_min) ; 
  Serial.print(tmp_max) ;
}


void read_sensors() {
  photo_degree = analogRead(photo_pin) ; 
  tmp_degree = analogRead(tmp_pin) ; 
  gas_degree = analogRead(gas_pin) ; 
  motion_degree = digitalRead(motion_pin) ; 
}





void mapping_sensors(){
  photo_pwm = map(photo_degree , photo_min , photo_max , 0 , 255) ; 
  tmp_c = map(tmp_degree , tmp_min , tmp_max , 0 , 165) - 40 ; 
  gas_pwm = map(gas_degree , gas_min , gas_max , 0 , 255)  ; 

}

void display_mapped_sensors() {
  Serial.println("------------------The mapped values---------------------");
  Serial.print("The photo pwm value = ") ;
  Serial.println(photo_pwm); 
  
  Serial.print("The temprature cilicus value = ") ;
  Serial.println(tmp_c); 
  
  Serial.print("The gas existance  = ") ;
  Serial.println(gas_pwm); 
  
  Serial.print("The motion boolean value = ") ;
  Serial.println(motion_degree); 
  Serial.println("---------------------------------------");
}


  
void calibrate_sensors(){
  	button_state = digitalRead(push_button);
  
  	read_analog_sensors() ; 
  
  	gas_min = gas_degree ; 
  	gas_max = gas_degree ; 
  	photo_min =photo_degree ; 
  	photo_max = photo_degree; 
  	tmp_min =tmp_degree ; 
  	tmp_max = tmp_degree; 
  
  while(button_state == LOW ) {
    
    button_state = digitalRead(push_button);
    
    read_analog_sensors() ;   
	
    
    if (photo_degree < photo_min) 
    	photo_min = photo_degree ; 
  	if (photo_degree > photo_max) 
    	photo_max = photo_degree ; 
  
  	if (tmp_min > tmp_degree) 
    	tmp_min = tmp_degree ; 
  	if (tmp_max < tmp_degree) 
    	tmp_max = tmp_degree ;
  
  	if (gas_degree < gas_min) 
    	gas_min = gas_degree ; 
  	if (gas_degree > gas_max) 
    	gas_max = gas_degree ;
  }
  display_min_max() ; 
  
}

void turn_all_lights(int light_pwm){
  analogWrite( led_orange , light_pwm) ;
  analogWrite( led_yellow , light_pwm) ;
  analogWrite( led_green , light_pwm) ;
  analogWrite( led_blue , light_pwm) ;
}


void display_sensors_inputValue(){
  
  Serial.println("------------------The input values---------------------");
  Serial.print("The photo analog value = ") ;
  Serial.println(photo_degree); 
  
  Serial.print("The temprature analog value = ") ;
  Serial.println(tmp_degree); 
  
  Serial.print("The gas analog value = ") ;
  Serial.println(gas_degree); 
  
  Serial.print("The motion boolean value = ") ;
  Serial.println(motion_degree); 
  Serial.println("---------------------------------------");
}


void motor_direction(bool dir) {
  if (dir) {
    digitalWrite(motor_1_pin,HIGH);
    digitalWrite(motor_2_pin,LOW);

  }
  else {
    digitalWrite(motor_1_pin,LOW);
    digitalWrite(motor_2_pin,HIGH);
 
  }

}
