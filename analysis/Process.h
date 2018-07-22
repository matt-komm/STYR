#ifndef PROCESS_H
#define PROCESS_H

#include "Module.h"

#include "TFile.h"

#include <memory>

namespace styr
{

class Process
{
    private:
     
    public:
        Process()
        {
        }
        
        void addModule(Module* module);
        void processFile(TFile* file);
};

}

#endif
