#ifndef Module_h
#define Module_h

#include "Updatable.h"

class Module : public Updatable {
    public:
        virtual void setup(void) = 0;
};

#endif