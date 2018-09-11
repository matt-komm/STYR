#ifndef STYR_PARTICLE_H
#define STYR_PARTICLE_H


#include <memory>
#include <string>
#include <typeinfo>
#include <iostream>
#include <unordered_map>

#include "classes/DelphesClasses.h"


namespace styr
{

class Particle
{
    
    public:
        class Property
        {
            public:
                
                virtual ~Property()
                {
                }
        };
        
        template<class TYPE>
        class PropertyTmpl:
            public Property
        {
            protected:
                TYPE _type;
            public:
                PropertyTmpl(const TYPE& type):
                    _type(type)
                {
                }
                
                virtual const TYPE value() const
                {
                    return _type;
                }
                
                virtual ~PropertyTmpl()
                {
                }
        };
        
    protected:
        TLorentzVector _p4;
        std::unordered_map<std::string,std::shared_ptr<Property>> _properties;
        
    public:
        Particle(const TLorentzVector& p4 = TLorentzVector(0,0,0,0)):
            _p4(p4)
        {
        }
        
        Particle(const Muon& muon):
            _p4(muon.P4())
        {
        }
        
        Particle(const Electron& electron):
            _p4(electron.P4())
        {
        }
        
        Particle(const Jet& jet):
            _p4(jet.P4())
        {
            set<int>("BTag",jet.BTag);
            set<int>("flavor",jet.Flavor);
        }
        
        Particle(const MissingET& met):
            _p4(met.P4())
        {
        }
        
        template<class TYPE>
        const TYPE get(const std::string& name) const
        {
            auto it = _properties.find(name);
            if (it==_properties.end())
            {
                throw std::runtime_error("Particle does not have property '"+name+"'");
            }
            PropertyTmpl<TYPE>* property = dynamic_cast<PropertyTmpl<TYPE>*>(it->second.get());
            if (not property)
            {
                throw std::runtime_error("Property '"+name+"' of wrong type");
            }
            return property->value();
        }
        
        template<class TYPE>
        void set(const std::string& name, const TYPE& value)
        {
            //std::unique_ptr<Property> property;//(new PropertyTmpl<TYPE>(value));
            _properties.emplace(
                name,
                std::shared_ptr<Property>(new PropertyTmpl<TYPE>(value))
            );
        }
        
        virtual const TLorentzVector& P4() const
        {
            return _p4;
        }
        virtual ~Particle()
        {
        }
};


}

#endif
