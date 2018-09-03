#ifndef STYR_MODULE_H
#define STYR_MODULE_H

#include <string>

#include "TFile.h"

namespace styr
{

class Event;

class Module
{
    protected:
        std::string name_;
    public:
        Module(const std::string& name=""):
            name_(name)
        {
        }
    
        virtual void analyze(Event&) = 0;
        
        virtual void beginFile(const TFile*,styr::Event&) {}
        virtual void endFile(const TFile*,styr::Event&) {}
        
        virtual void beginJob() {}
        virtual void endJob() {}
        
        virtual ~Module()
        {
        }
};

}

#endif
