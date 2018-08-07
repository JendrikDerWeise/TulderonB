#include "KristallReader.h"

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

void KristallReader::setup(){
    
    KristallReader::mp=0;
    KristallReader::mpChanged = false;
    SPI.begin();        // Init SPI bus
    mfrc522.PCD_Init(); // Init MFRC522 card

    // Prepare the key (used both as key A and as key B)
    // using FFFFFFFFFFFFh which is the default at chip delivery from the factory
    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }
}

void KristallReader::checkMPInKristall(){
// Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    //MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);


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
        return;
    }


    // Read data from the block
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size);
    
    mp = buffer[0];
    mpChanged = true;


    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}

bool KristallReader::isMPChanged(){
    return mpChanged;
}

int KristallReader::getMPInKristall(){
    mpChanged = false;
    return mp;
}

bool KristallReader::isKristallPresent(){
    return mfrc522.PICC_IsNewCardPresent();
}

