#ifndef _KristallReader_h_included_
#define _KristallReader_h_included_

#include <SPI.h>
#include <MFRC522.h>

class KristallReader{
    private:
        int mp;
        int counter;
        bool mpChanged;
        bool kristallPresent;
        bool kristallCheck();
        bool isReading;
        MFRC522::MIFARE_Key key;
    public:
        void setup();
        bool isMPChanged();
        int getMPInKristall();
        void checkMPInKristall();
        bool isKristallPresent();
        void readMPs();
        void setMPInKristall(int);
};
#endif