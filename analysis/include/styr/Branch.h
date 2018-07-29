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
    private:
        Branch(const std::string& name):
            BranchBase(name)
        {
        }

    protected:
        static const std::string typeName;
        
    public:
    
        static std::shared_ptr<Branch<TYPE>> createNewBranch(const std::string& name, TTree*)
        {
            Branch<TYPE>* b = new Branch<TYPE>(name);
            return std::shared_ptr<Branch<TYPE>>(b);
        }
        
        static std::shared_ptr<const Branch<TYPE>> getExistingBranch(const std::string& name, TTree*)
        {
            return new Branch<TYPE>(name);
        }

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

}

#endif
