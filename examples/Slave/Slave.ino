#include <Wire.h>
#include <BetterTransferI2CSlave.h>

#define I2C_SLAVE_ADDRESS 9 
//#define I2C_SLAVE_ADDRESS 10

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

struct STRUCT_ONE {
  int count;
  bool toggle;
};

struct STRUCT_TWO {
  int number;

};


STRUCT_ONE struct_one;
BetterTransferI2CSlave bt_struct_one;

STRUCT_TWO struct_two;
BetterTransferI2CSlave bt_struct_two;


void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SLAVE_ADDRESS);
  bt_struct_one.begin(details(struct_one));
  bt_struct_two.begin(details(struct_two));

  inputString.reserve(10);
  Wire.onReceive(BetterTransferI2CSlave::onReceive);
  Wire.onRequest(BetterTransferI2CSlave::onSend);
}

void displayData() {
  Serial.print("[struct_one{\"count\":\"");
  Serial.print(struct_one.count);
  Serial.print("\", \"toggle:\"");
  Serial.print(struct_one.toggle);
  Serial.print("\"}, struct_two{\"number\":\"");
  Serial.print(struct_two.number);
  Serial.print("\"}]");
}

void loop() {
  if (stringComplete) {
    if (inputString == "count\n") {
      struct_one.count++;
      displayData();
    }
    if (inputString == "toggle\n") {
      struct_one.toggle = !struct_one.toggle;
      displayData();
    }
    if (inputString == "number\n") {
      struct_two.number++;
      displayData();
    }
    
    // clear the string:
    inputString = "";
    stringComplete = false;
  }


  if (bt_struct_one.newData() || bt_struct_two.newData()) {
          displayData();
  }
}


void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
