#ifndef STYR_BRANCH_H
#define STYR_BRANCH_H

#include <memory>
#include <string>
#include <typeinfo>

#include "TTree.h"
#include "TLeaf.h"

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
        
        virtual TYPE& get() = 0;
        virtual const TYPE& get() const = 0;
        virtual size_t size() const = 0;
        
        
};

template<class TYPE> const std::string Branch<TYPE>::typeName = typeid(TYPE).name();


template<class TYPE>
using BranchPtr = std::shared_ptr<Branch<TYPE>>;

template<class TYPE>
using ConstBranchPtr = std::shared_ptr<const Branch<TYPE>>;

template<class TYPE>
class InputBranch:
    public Branch<TYPE>
{
    protected:
        TYPE _data;
        TLeaf* _leaf;
    public:
        InputBranch(const std::string& name, TBranch* _branch):
            Branch<TYPE>(name)
        {   
            _branch->GetTree()->SetBranchAddress(name.c_str(),&_data);
            //_branch->SetAddress(&_data);
            _leaf = _branch->GetLeaf(name.c_str());
        }
        
        virtual TYPE& get()
        {
             return _data;
        }
        
        virtual const TYPE& get() const
        {
            return _data;
        }
        
        virtual size_t size() const
        {
            return _leaf->GetLen();
        }
};

template<class TYPE>
class OutputBranch:
    public Branch<TYPE>
{
    protected:
        TYPE _data;
        
    public:
        OutputBranch(const std::string& name):
            Branch<TYPE>(name)
        {
        }
        
        virtual TYPE& get()
        {
            return _data;
        }
        
        virtual const TYPE& get() const
        {
            return _data;
        }
        
        virtual size_t size() const
        {
            return 1;
        }
};

}

#endif
