#ifndef STYR_MODULE_H
#define STYR_MODULE_H

#include "styr/Config.h"

#include <string>

#include "TFile.h"

namespace styr
{

class Event;

class Module
{
    protected:
        std::string name_;
        Config _config;
        
    public:
        Module(const std::string& name="", const Config& config=Config()):
            name_(name),
            _config(config)
        {
        }
        
        inline const Config& config() const
        {
            return _config;
        }
        
        inline Config& config()
        {
            return _config;
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
