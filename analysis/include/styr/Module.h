#ifndef STYR_MODULE_H
#define STYR_MODULE_H

namespace styr
{

class Event;

class Module
{
    public:
        virtual void analyze(Event& event) = 0;
        
        virtual void beginFile(Event&) {}
        virtual void endFile(Event&) {}
        
        virtual void beginJob(Event&) {}
        virtual void endJob(Event&) {}
        
        virtual ~Module()
        {
        }
};

}

#endif
