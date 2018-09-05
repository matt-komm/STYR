#ifndef CONFIG_H
#define CONFIG_H

#include <typeinfo>
#include <memory>
#include <unordered_map>
#include <vector>

#define ADD_CONFIG_TYPE(TYPE) \
    Config& set(const char* key, const TYPE& value) \
    { \
        _parameters.emplace(key,makeParameter<TYPE>(value)); \
        return *this; \
    }
    
#define ADD_CONFIG_TYPE2(TYPE1,TYPE2) \
    Config& set(const char* key, TYPE1 value) \
    { \
        _parameters.emplace(key,makeParameter<TYPE2>(value)); \
        return *this; \
    }

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
    
        ADD_CONFIG_TYPE2(const char*,std::string)
        ADD_CONFIG_TYPE(std::string)
        ADD_CONFIG_TYPE(int)
        ADD_CONFIG_TYPE(float)
        ADD_CONFIG_TYPE(bool)
        
        ADD_CONFIG_TYPE(std::vector<std::string>)
        ADD_CONFIG_TYPE(std::vector<int>)
        ADD_CONFIG_TYPE(std::vector<float>)
        ADD_CONFIG_TYPE(std::vector<bool>)
        
        bool has(const std::string& key) const
        {
            auto it = _parameters.find(key);
            if (it == _parameters.end())
            {
                return false;
            }
            return true;
        }
        
        template<class TYPE> bool hasOfType(const std::string& key) const
        {
            auto it = _parameters.find(key);
            if (it == _parameters.end())
            {
                return false;
            }
            auto parameter = dynamic_cast<ParameterTmpl<TYPE>*>(it->second.get());
            if (not parameter)
            {
                return false;
            }
            return true;
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
        
        template<class TYPE> const TYPE& getOrDefault(const std::string& key, const TYPE& defaultValue) const
        {
            if (hasOfType<TYPE>(key))
            {
                return get<TYPE>(key);
            }
            return defaultValue;
        }
    
};

}

#endif
