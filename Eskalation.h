#ifndef _Eskalation_h_included_
#define _Eskalation_h_included_

#include "NeoPixel.h"

class Eskalation{
    private:
        int eStufe;
        int zStufe;
        int fogPin;
        int fogTime;
        int maxZstufe;
        unsigned long startTime;
        bool fogging;
        NeoPixel *light;

    public:
        Eskalation();
        void setup(NeoPixel *l);
        void raiseEstufe();
        void dezEstufe();
        void raiseZstufe();
        void dezZstufe();
        void redBlinking();
        void eskalate();
        void fogger();
        void startFogger();
        void alarm();
        void explosion();
        int getEstufe();
        int getZstufe();
        bool isFogging();

};
#endif