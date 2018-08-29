#ifndef STYR_PROCESS_H
#define STYR_PROCESS_H

#include "TFile.h"

#include <memory>

namespace styr
{

class Module;

class Process
{
    private:
        std::vector<Module*> modules_;
        
    public:
        Process()
        {
        }
        
        void addModule(Module* module);
        void processFile(TFile* file, const char* treeName);
};

}

#endif
