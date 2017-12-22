#include <RCSwitch.h>
#include <IRremote.h>
#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>


/* ***********************************
 *   CONFIG - START
 *********************************** */
// Pinbelegung: 
// 433MHz rx Data an Pin 2
// Display SCL an A5
// Display SDA an A4
int IR_RECV_PIN = 7;

// Display
boolean useDisplay = true;
#define OLED_I2C_ADDRESS 0x3C
long displayTimeout = 60000; 

/* ***********************************
 *   CONFIG - ENDE
 *********************************** */


SSD1306AsciiAvrI2c oled;

IRrecv irrecv(IR_RECV_PIN);
RCSwitch mySwitch = RCSwitch();

decode_results results;


void setup() {
  // Serial initialisieren
  Serial.begin(9600);
  Serial.println(F("SmartHome yourself - IR/433MHz Signal-Scanner"));
  Serial.println();
  delay(500);
  
  // Display
  if(useDisplay){
    oled.begin(&Adafruit128x64, OLED_I2C_ADDRESS);
    oled.setFont(Verdana12_bold);
    oled.clear();
    oled.home();
    displayIntro();
    clearDisplay(false);
  }

  // 433MHz Rx
  mySwitch.enableReceive(0);        // Receiver auf Pin #2

  // IR Rx
  irrecv.enableIRIn();

  Serial.println("Initialisiert");  
}


void loop() {
  // Check for 433MHz Signal
  if (mySwitch.available()) {
    Serial.println("ID / Signal erkannt:");
    unsigned long value = mySwitch.getReceivedValue();
    Serial.println(value);
    
    if(useDisplay){
      oled.setCursor(0,4);
      oled.print(F("433Mhz Code: "));
      oled.setCursor(0,6);
      oled.print(value);
      oled.print("                       ");
   }
    
    mySwitch.resetAvailable();
  }

  // Check for IR Signal
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    dump(&results);
    irrecv.resume();

    if(useDisplay){
      oled.setCursor(0,4);
      oled.print(F("IR Code: "));
      oled.setCursor(0,6);
      oled.print(results.value);
      oled.print("                       ");
    }
  }
 
}






/**
 * Display vollständig leeren
 */
void clearDisplay(){
  clearDisplay(true);
}

/**
 * Display leeren
 * 
 * @param boolean fullClear: 
 *   true  Bildschirm wird geleert
 *   false wird der Standard-Header/Footer 
 *         nach dem Clear eingeblendet.                   
 */
void clearDisplay(boolean fullClear){
  if(useDisplay){
    oled.clear();
    oled.set1X();

    if (!fullClear){
      oled.setCursor(0, 1);
      oled.print(F("SmartHome yourself"));
      
      oled.setCursor(0, 2);
      oled.print(F("Signal-Scanner"));
    }
  }
}

/**
 * Intro auf Display abspielen
 */
void displayIntro(){
  if(useDisplay){
    Serial.println("Display Intro");
    
    oled.set2X();
    oled.setCursor(0, 1);
    oled.print(F("SmartHome"));
    delay(750);
    oled.setCursor(0, 3);
    oled.print(F("yourself"));
    delay(1500);
    oled.set1X();
    oled.setCursor(0, 6);
    oled.print(F("Signal Scanner"));
  
    delay(5000);
  }
}


void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 1; i < count; i++) {
    if (i & 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}









