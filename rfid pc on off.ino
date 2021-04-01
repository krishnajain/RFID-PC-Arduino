#include <SPI.h>
#include <MFRC522.h>
#include <Keyboard.h>

constexpr uint8_t RST_PIN = 9;  
constexpr uint8_t SS_PIN = 10;  
char ctrlKey = KEY_LEFT_CTRL;

MFRC522 mfrc522(SS_PIN, RST_PIN); 

void setup() 
{ 
  Serial.begin(9600);
  SPI.begin();      
  mfrc522.PCD_Init();   
  mfrc522.PCD_DumpVersionToSerial();  
  Keyboard.begin();
}

void readHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  Serial.print("RFID UID: ");
  readHex(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();

  if (mfrc522.uid.uidByte[0] == 0xB7 && //change the UID
      mfrc522.uid.uidByte[1] == 0x30 &&
      mfrc522.uid.uidByte[2] == 0xEB &&
      mfrc522.uid.uidByte[3] == 0x11) {
      Serial.println("Correct UID");
  }
  else {
    Serial.println("Incorrect UID");
    delay(5000);
    return;
  }
  Keyboard.press(ctrlKey);
  delay(100);
  Keyboard.releaseAll();
  Serial.println("Printing password...");
  Keyboard.print("Your PC Pass");
  delay(1000);
  Keyboard.write(10);
  
  delay(5000);                                          
}
