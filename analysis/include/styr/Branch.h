#ifndef STYR_BRANCH_H
#define STYR_BRANCH_H

#include <memory>
#include <string>
#include <typeinfo>

#include "TTree.h"

namespace styr
{

class BranchBase
{
    protected:
        std::string _name;
	public:
	    BranchBase(const std::string& name):
	        _name(name)
        {
        }
        
	    virtual const std::string& getName() const
	    {
	        return _name;
	    }
	    
	    virtual const std::type_info& getType() const = 0;
	    virtual const std::string& getTypeName() const = 0;
}; 

template<class TYPE>
class Branch:
    public BranchBase
{
    protected:
        Branch(const std::string& name):
            BranchBase(name)
        {
        }
    
        static const std::string typeName;
        
    public:
    

        virtual const std::type_info& getType() const
        {
            return typeid(TYPE);
        }
        
        virtual const std::string& getTypeName() const
        {
            return typeName;
        }
};

template<class TYPE> const std::string Branch<TYPE>::typeName = typeid(TYPE).name();

template<class TYPE>
class BranchFromTTree:
    public Branch<TYPE>
{
    public:
        BranchFromTTree(const std::string& name, TTree* _tree):
            Branch<TYPE>(name)
        {
        }
};

template<class TYPE>
class BranchCached:
    public Branch<TYPE>
{
    public:
        BranchCached(const std::string& name):
            Branch<TYPE>(name)
        {
        }
};

}

#endif
