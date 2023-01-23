#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

Servo myservo;

#define ir_enter 2 // The IR sensor in port 2 that is located at the gate.
#define ir_back  4 // The IR sensor in port 4 that is located at the gate.

#define ir_car1 5 // The IR sensor in port 5 that is located at the first parking slot.
#define ir_car2 6 // The IR sensor in port 6 that is located at the second parking slot.



int S1 = 0, S2 = 0; // Initial values for Slot 1 and Slot 2.
int flag1 = 0, flag2 = 0; // signaling for the wires.
int slot = 2; // Avalible  number of slots. (This project only has 2 slots)
int redPin = 8; // The pin(port) number that is connecting the Arduino UNO to RGB light Red.
int greenPin = 7; // The pin(port) number that is connecting the Arduino UNO to RGB light Green.



void setup() { // Settingup the Arduino UNO.
  Serial.begin(9600); //The beginning point.
  pinMode(ir_car1, INPUT); // Taking the input for the IR sensor of the car slot 1.
  pinMode(ir_car2, INPUT); // Taking the input for the IR sensor of the car slot 2.
  pinMode(ir_enter, INPUT); // Taking the input for the IR sensor of the entering the gate.
  pinMode(ir_back, INPUT); // Taking the input for the IR sensor for exiting the gate.
  myservo.attach(3); // The servo moter that used for the gate.
  myservo.write(90); // The beginning point of the servo moter.


  lcd.init(); // Initializing the LCD Screen.
  lcd.backlight(); // Printing a message to the LCD Screen.
  lcd.setCursor (0, 1); // The LCD screen coordinates.
  lcd.print(" Hi Welcome To "); // The text that will be dislayed on the LCD screen.
  lcd.setCursor (0, 2); // The LCD screen coordinates.
  lcd.print(" -"); // the text thst will display on  LDC screen - 
  delay (5000); // Delay time by 5000.
  lcd.clear(); // To clear the LCD scren.
  lcd.setCursor (0, 0); // The screen coordinates.
  lcd.print("NETWORK Project"); // the text that will display on the LCD screen.
  lcd.setCursor (0, 1); // The LCD screen coordinates.
  lcd.print(" Car Parking system"); // The text that will be dislayed on the LCD screen.
  lcd.setCursor (0, 2); // The LCD screen coordinates.
  lcd.print("          System     "); // The text that will be dislayed on the LCD screen.
  delay (5000); // Delay time by 5000.
  lcd.clear(); // To clear the LCD scren.
  pinMode(redPin, OUTPUT); // Taking the output for the RGB light Red.
  pinMode(greenPin, OUTPUT); // Taking the output for the RGB light Green.

  Read_Sensor(); // Reading sensor info.

  int total = S1 + S2 ; // To calculate the total parking spaces.
  slot = slot - total; // To calculate a single parking space.
}

void loop() { // Loop for the devices

  Read_Sensor(); // Reading sensor info.

  lcd.setCursor (0, 0); // The LCD screen coordinates.
  lcd.print("  Avai Slot: "); // The text that will be dislayed on the LCD screen.
  lcd.print(slot); // Printing number of avalibale slots.
  
  Serial.println(" Avai Slot: "); // The text that will be displays it on the server laptop screen.
  Serial.println(slot); // Printing number of avalibale slots.
  lcd.print("    "); // The text that will be dislayed on the LCD screen.
  Serial.println("    "); // The text that will be displays it on the server laptop screen.

  lcd.setCursor (0, 1); // The LCD screen coordinates.
  if (S1 == 1) { // If statment if the slot 1 was not avalible.
    lcd.print("S1:Full ");  // The text that will be dislayed on the LCD screen if the slot is full.
    Serial.println("S1:Full "); // The text that will be displays it on the server laptop screen if the slot is full.
        digitalWrite(greenPin, LOW); // Not show the RGB Green light (set green light as LOW).
            digitalWrite(redPin, HIGH); // Show the RGB Red light (set red light as HIGH).
  }
  else { // Else for the if statement when slot 1 is avalibale.
    lcd.print("S1:Empty"); // The text that will be dislayed on the LCD screen if the slot is full.
    Serial.println("S1:Empty"); // The text that will be displays it on the server laptop screen if the slot is Empty.
        digitalWrite(redPin, LOW); // Not show the RGB Red light (set red light as LOW).
            digitalWrite(greenPin, HIGH); // Show the RGB Green light (set Green light as HIGH).
  }

  lcd.setCursor (8, 1); // The LCD screen coordinates.
  if (S2 == 1) { // If statment if the slot 2 was not avalible.
    lcd.print("S2:Full ");  // The text that will be dislayed on the LCD screen if the slot is full.
    Serial.println("S2:Full "); // The text that will be ddisplays it on the server laptop screen if the slot is full.
   // there is suppose to be a code here for the RGB lights but the Light has burnedup and does not work.

  }
  else { // Else for the if statement when slot 2 is avalibale.
    lcd.print("S2:Empty"); // The text that will be dislayed on the LCD screen if the slot is full.
    Serial.println("S2:Empty"); // The text that will be displays it on the server laptop screen if the slot is Empty.
   // there is suppose to be a code here for the RGB lights but the Light has burnedup and does not work.

  }

  if (digitalRead (ir_enter) == 0 && flag1 == 0) { //If statment for the gate if the car parking is full.
    if (slot > 0) {
      flag1 = 1;
      if (flag2 == 0) {
        myservo.write(180); 
        slot = slot - 1;
      }
    } else {
      lcd.setCursor (0, 0); // The LCD screen coordinates.
      lcd.print("Parking Full"); // The text that will be dislayed on the LCD screen.
      Serial.print("Parking Full"); // The text that will be displays it on the server laptop screen.
      delay(1500); // Delay by 1500.
    }
  }

  if (digitalRead (ir_back) == 0 && flag2 == 0) { //If statment for the gate if the car parking has avalibe slots.
    flag2 = 1;
    if (flag1 == 0) {
      myservo.write(180); 
      slot = slot + 1;
    }
  }

  if (flag1 == 1 && flag2 == 1)
    delay (1000);
    myservo.write(90);
    flag1 = 0, flag2 = 0;
  }

  delay(1); //Delay by 1.
 
}

void Read_Sensor() {
  S1 = 0, S2 = 0; //Initiation for both sensors.

  if (digitalRead(ir_car1) == 0) { // if slot one = 1 it means its full
    S1 = 1;
  }
  if (digitalRead(ir_car2) == 0) {  // if slot two = 1 it means its full
    S2 = 1;
  }

}
