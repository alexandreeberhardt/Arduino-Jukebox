#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN); 

#define BLOCK_START 4
#define NUM_BLOCKS 3

void setup() {
  Serial.begin(9600);   
  SPI.begin();          
  mfrc522.PCD_Init();   
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }



  String result = readDataFromBlocks();
  Serial.println(result);
  
  mfrc522.PICC_HaltA();  
  mfrc522.PCD_StopCrypto1();  
  delay(1000);  
}

String readDataFromBlocks() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  

  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);
  String result = "";

  for (int block = BLOCK_START; block < BLOCK_START + NUM_BLOCKS; block++) {
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      return "";
    }

    status = mfrc522.MIFARE_Read(block, buffer, &size);
    if (status != MFRC522::STATUS_OK) {
      return "";
    }

    for (byte i = 0; i < 16; i++) {
      if (buffer[i] != 0) {
        result += (char)buffer[i];
      }
    }
  }

  return result;
}
