#include <SPI.h>
#include <MFRC522.h>

#define GREEN_LED 6
#define RED_LED 7
#define BUZZER 8
#define SS_PIN 10
#define RST_PIN 9

String tagId = ""; // Store Tag ID
bool allowedAccess = false;

String registeredTags[] = { "556a4cb", "ID_2", "ID_3"};

MFRC522 reader(SS_PIN, RST_PIN); // Create a new instance of the RFID reader using the pins as params

void setup() {
  Serial.begin(9600); // Initiate Serial Communication
  SPI.begin(); // Initiate SPI Communication

  reader.PCD_Init();

  pinMode(GREEN_LED, OUTPUT); // Define Green Led Pin as Output
  pinMode(RED_LED, OUTPUT); // Define Red Led Pin as Output
  pinMode(BUZZER, OUTPUT); // Define Buzzer Pin as Output
}

void loop() {
  readTag();
}

void readTag() {
  tagId = "";

  // Check if there's a tag present
  if (!reader.PICC_IsNewCardPresent() || !reader.PICC_ReadCardSerial()) {
    delay(50);

    return;
  }

  // Get the TagID through read.uid and store it in idTag
  for (byte i = 0; i< reader.uid.size; i++) {
    tagId.concat(String(reader.uid.uidByte[i], HEX));
  }

  // Compare the read value with the stored on in the registeredTags arrays
  for (int i = 0; i < (sizeof(registeredTags)/sizeof(String)); i++) {
    if(tagId.equalsIgnoreCase(registeredTags[i])) {
      // If the read id is registered, allow access
      allowedAccess = true;
    }
  }

  if (allowedAccess == true) {
    allowAccess();
  } else {
    denyAccess();
  }

  delay(2000); // Wait 2 seconds between readings
}

void allowAccess() {
  Serial.println("Registered Tag: " + tagId);
  allowedAccessEffect();
  allowedAccess = false; // reset the access
}

void denyAccess() {
  Serial.println("Tag NOT Registered: " + tagId);
  deniedAccessEffect();
}

void allowedAccessEffect() {
  int beep_amount = 1; // Number of beeps
  
  for(int j = 0; j < beep_amount; j++) {
    // Turn on buzzer at the frequency of 1500hz and turn on the green led
    tone(BUZZER, 3000);
    digitalWrite(GREEN_LED, HIGH);
    delay(500);

    // Shut down buzzer and led
    noTone(BUZZER);
    digitalWrite(GREEN_LED, LOW);
    delay(500);
  }
}

void deniedAccessEffect() {
  int beep_amount = 2; // Number of beeps

  for(int j = 0; j < beep_amount; j++) {
    // Turn on the buzzer at the frequency of 500hz and turn on the red led
    tone(BUZZER, 100);
    digitalWrite(RED_LED, HIGH);
    delay(100);

    // Shut down buzzer and led
    noTone(BUZZER);
    digitalWrite(RED_LED, LOW);
    delay(100);
  }
}
