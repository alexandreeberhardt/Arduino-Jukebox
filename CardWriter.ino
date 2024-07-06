#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 53
#define RST_PIN 5
MFRC522 mfrc522(SS_PIN, RST_PIN);  

#define BLOCK_START 4 
#define NUM_BLOCKS 3
#define DATA_ID "XXX" // Replace XXX by the id of the song you want to play ("7gfT0AVjW9pgM06rBa1iBK" for Coward by Sarah and the Sundays)

void setup() {
  Serial.begin(9600);   
  SPI.begin();         
  mfrc522.PCD_Init();   
  Serial.println("Place your card to write");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("RFID tag ID: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  writeDataToBlocks();
  
  mfrc522.PICC_HaltA(); 
  mfrc522.PCD_StopCrypto1();  
  delay(1000);  
}

void writeDataToBlocks() {
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  

  MFRC522::StatusCode status;
  const char* data = DATA_ID;
  byte dataBlock[16];
  int dataLen = strlen(data);
  int offset = 0;

  for (int block = BLOCK_START; block < BLOCK_START + NUM_BLOCKS; block++) {
    memset(dataBlock, 0, sizeof(dataBlock));

    for (int i = 0; i < 16; i++) {
      if (offset < dataLen) {
        dataBlock[i] = data[offset++];
      } else {
        dataBlock[i] = 0;  
      }
    }

    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Authentication failed for block ");
      Serial.print(block);
      Serial.print(": ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    status = mfrc522.MIFARE_Write(block, dataBlock, 16);
    if (status != MFRC522::STATUS_OK) {
      Serial.print("Writing failed for block ");
      Serial.print(block);
      Serial.print(": ");
      Serial.println(mfrc522.GetStatusCodeName(status));
      return;
    }

    Serial.print("Data written to block ");
    Serial.print(block);
    Serial.println(" successfully!");
  }
}
