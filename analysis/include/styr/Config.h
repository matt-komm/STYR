#ifndef CONFIG_H
#define CONFIG_H

#include <typeinfo>
#include <memory>
#include <unordered_map>

namespace styr
{

class Config
{ 
    protected:
        class Parameter
        {
            public:
                virtual const std::type_info& getType() const = 0;
                virtual std::unique_ptr<Parameter> clone() const = 0;
        };
        
        template<class TYPE>
        class ParameterTmpl: 
            public Parameter
        {
            protected:
                const TYPE _value;
            public:
                ParameterTmpl(const TYPE& value):
                    _value(value)
                {
                }
                
                virtual const std::type_info& getType() const
                {
                    return typeid(TYPE);
                }
                
                inline const TYPE& value() const
                {
                    return _value;
                }
                
                virtual std::unique_ptr<Parameter> clone() const
                {
                    return std::unique_ptr<Parameter>(new ParameterTmpl<TYPE>(_value));
                }
        };
        
        template<class TYPE>
        static std::unique_ptr<Parameter> makeParameter(const TYPE& value)
        {
            return std::unique_ptr<Parameter>(new ParameterTmpl<TYPE>(value));
        }
        
        std::unordered_map<std::string, std::unique_ptr<Parameter>> _parameters;
    
    public:
        Config()
        {
        }
        
        Config(const Config& config)
        {
            for (auto& p: config._parameters)
            {
                _parameters.emplace(p.first,p.second->clone());
            }
        }
        
        Config& operator=(const Config& config)
        {
            _parameters.clear();
            for (auto& p: config._parameters)
            {
                _parameters.emplace(p.first,p.second->clone());
            }
            return *this;
        }
        
        Config(Config&&) = delete;
        Config& operator=(Config&&) = delete;
    
        Config& set(const char* key, const char* value)
        {
            _parameters.emplace(key,makeParameter<std::string>(value));
            return *this;
        }
        
        Config& set(const char* key, const std::string& value)
        {
            _parameters.emplace(key,makeParameter<std::string>(value));
            return *this;
        }
        
        Config& set(const char* key, int value)
        {
            _parameters.emplace(key,makeParameter<int>(value));
            return *this;
        }
        
        Config& set(const char* key, float value)
        {
            _parameters.emplace(key,makeParameter<float>(value));
            return *this;
        }
        
        Config& set(const char* key, bool value)
        {
            _parameters.emplace(key,makeParameter<bool>(value));
            return *this;
        }
        
        template<class TYPE> const TYPE& get(const std::string& key) const
        {
            auto it = _parameters.find(key);
            if (it == _parameters.end())
            {
                throw std::runtime_error("Key '"+key+"' not found in config");
            }
            auto parameter = dynamic_cast<ParameterTmpl<TYPE>*>(it->second.get());
            if (not parameter)
            {
                throw std::runtime_error("Cannot cast parameter named '"+key+"' of type '"+it->second->getType().name()+"' to '"+typeid(TYPE).name()+"'");
            }
            return parameter->value();
        }
    
};

}

#endif
