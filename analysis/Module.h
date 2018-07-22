#ifndef MODULE_H
#define MODULE_H

namespace styr
{

class Event;

class Module
{
    public:
        virtual void analyze(Event& event) = 0;
        virtual ~Module()
        {
        }
};

}

#endif
