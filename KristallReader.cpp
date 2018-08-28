#include "KristallReader.h"

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
//unsigned long lastCheck = 0;
int c = 0;
const int KRISTALLKABEL = 7;

void KristallReader::setup(){
    
    mp = 100;
    mpChanged = false;
    kristallPresent = true;
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card
    
    counter = 0;
    isReading = false;
    pinMode(KRISTALLKABEL, INPUT_PULLUP);

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

void KristallReader::checkMPInKristall(){
// Look for new cards

    if(digitalRead(KRISTALLKABEL) == LOW){//Kristall weg
        kristallPresent = true;
    }else{
        kristallPresent = false;
    }


    if(!mfrc522.PICC_IsNewCardPresent())
        return;
    if(!mfrc522.PICC_ReadCardSerial())
        return;

    readMPs();


}

void KristallReader::readMPs(){
    // In this sample we use the second sector,
    // that is: sector #1, covering block #4 up to and including block #7
    byte sector         = 1;
    byte blockAddr      = 4;
    byte trailerBlock   = 7;
    
    MFRC522::StatusCode status;
    byte buffer[18];
    byte size = sizeof(buffer);
    
    // Authenticate using key A
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        //Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }


    // Read data from the block
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    
    mp = buffer[0];
    mpChanged = true;
    Serial.print("Kristall: ");
    Serial.println(buffer[0]);
    //Serial.println(mp);*/


   mfrc522.PICC_HaltA();
   mfrc522.PCD_StopCrypto1();

    isReading = false;
    c = 0;
    checkMPInKristall();
}

bool KristallReader::isKristallPresent(){
    return kristallPresent;
}

bool KristallReader::isMPChanged(){
    return mpChanged;
}

int KristallReader::getMPInKristall(){
    mpChanged = false;
    //Serial.print("MP");Serial.println(mp);
    return mp;
}

bool KristallReader::kristallCheck(){
    
    return mfrc522.PICC_IsNewCardPresent() || mfrc522.PICC_ReadCardSerial();//
}

void KristallReader::setMPInKristall(int newMP){
    mp = newMP;
}

