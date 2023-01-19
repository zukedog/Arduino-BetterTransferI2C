#include <Wire.h>
#include <BetterTransferI2CMaster.h>

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

struct STRUCT_ONE {
  int count;
  bool toggle;
};

struct STRUCT_TWO {
  int number;

};


BT_Address NINE(9, &Wire);
BT_Address TEN(10, &Wire);


STRUCT_ONE struct_one;
BetterTransferI2CMaster struct_one_nine(details(struct_one), &NINE);
BetterTransferI2CMaster struct_one_ten(details(struct_one), &TEN);


STRUCT_TWO struct_two;
BetterTransferI2CMaster struct_two_nine(details(struct_two), &NINE);
BetterTransferI2CMaster struct_two_ten(details(struct_two), &TEN);


void setup() {
  Serial.begin(115200);
  Wire.begin();

  inputString.reserve(10);
}

void displayData() {
  Serial.print("{struct_one:{\"count\":\"");
  Serial.print(struct_one.count);
  Serial.print("\", \"toggle\":\"");
  Serial.print(struct_one.toggle);
  Serial.print("\"}, struct_two:{\"number\":\"");
  Serial.print(struct_two.number);
  Serial.print("\"}}");
}


void loop() {
  if (stringComplete) {
    if (inputString == "count\n") {
      struct_one.count++;
      struct_one_nine.sendData();
      struct_one_ten.sendData();
      displayData();
    }
    if (inputString == "toggle\n") {
      struct_one.toggle = !struct_one.toggle;
      struct_one_nine.sendData();
      struct_one_ten.sendData();
      displayData();
    }
    if (inputString == "number\n") {
      struct_two.number++;
      struct_two_nine.sendData();
      struct_two_ten.sendData();
      displayData();
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }


  if (struct_one_nine.receiveData()) {
    displayData();
    struct_one_ten.sendData();
  }

  if (struct_two_nine.receiveData()) {
    displayData();
    struct_two_ten.sendData();
  }

  if (struct_one_ten.receiveData()) {
    displayData();
    struct_one_nine.sendData();
  }

  if (struct_two_ten.receiveData()) {
    displayData();
    struct_two_nine.sendData();
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
