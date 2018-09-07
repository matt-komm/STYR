#ifndef STYR_BRANCH_H
#define STYR_BRANCH_H

#include "TTree.h"
#include "TLeaf.h"
#include "TBranchElement.h"
#include "TClonesArray.h"

#include <memory>
#include <string>
#include <typeinfo>
#include <iostream>

#include<sstream>

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
	    
	    virtual void clear() = 0;
	    
	    virtual const std::type_info& getType() const = 0;
	    virtual const std::string& getTypeName() const = 0;
	    virtual const std::string getInfo() const
	    {
	        std::stringstream ss;
	        ss<<"Branch: "<<_name;
	        return ss.str();
	    }
	    
	    virtual void book(TTree*)
	    {
	    }
	    
	    virtual void write(TTree*)
	    {
	    }
}; 

typedef std::shared_ptr<BranchBase> BranchBasePtr;
typedef std::shared_ptr<const BranchBase> ConstBranchBasePtr;

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
        
        virtual const std::string getInfo() const
	    {
	        std::stringstream ss;
	        ss<<"Branch: "<<_name<<" ("<<getTypeName()<<")";
	        return ss.str();
	    }
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
    public:
        InputBranch(const std::string& name, TBranch* _branch):
            Branch<TYPE>(name)
        {   
            _branch->GetTree()->SetBranchAddress(name.c_str(),&_data);
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
        
        virtual void clear()
        {
        }
};

template<class TYPE>
class InputBranch<std::vector<TYPE>>:
    public Branch<std::vector<TYPE>>
{
    protected:
        mutable std::vector<TYPE> _data;
        TBranch* _branch;
        TLeaf* _leaf;
        TBranchElement* _branchElement;
        TClonesArray* _array;
    public:
        InputBranch(const std::string& name, TBranch* branch):
            Branch<std::vector<TYPE>>(name),
            _branch(branch),
            _leaf(nullptr),
            _branchElement(nullptr),
            _array(nullptr)
        {   
            _branchElement = dynamic_cast<TBranchElement*>(branch);
            if (_branchElement)
            {
                _data = std::vector<TYPE>(_branchElement->GetMaximum());
                _array = new TClonesArray(_branchElement->GetClonesName());
                _branchElement->SetAddress(&_array);
            }
            else
            {
                if (_branch->GetNleaves()==0)
                {
                    throw std::runtime_error("No leaf found for branch: "+name);
                }
                _leaf = dynamic_cast<TLeaf*>(_branch->GetListOfLeaves()->At(0));
                if (not _leaf)
                {
                    throw std::runtime_error("Failed to get leaf for branch: "+name);
                }
                TLeaf* leafCounter = _leaf->GetLeafCount();
                if (not leafCounter)
                {
                    throw std::runtime_error("Failed to retrive counter leaf for branch: "+name);
                }
                _data = std::vector<TYPE>(_leaf->GetLeafCount()->GetMaximum());
                _branch->SetAddress(_data.data());
            }
        }
       
        virtual std::vector<TYPE>& get()
        {   
            if (_branchElement)
            {
                for (size_t i = 0; i < this->size(); ++i)
                {
                    _data[i] = *(TYPE*)_array->At(i);
                }
            }
            return _data;
        }
        
        virtual const std::vector<TYPE>& get() const
        {
            if (_branchElement)
            {
                for (size_t i = 0; i < this->size(); ++i)
                {
                    _data[i] = *(TYPE*)_array->At(i);
                }
            }
            return _data;
        }
        
        virtual size_t size() const
        {
            if (_leaf) return _leaf->GetLen();
            else if (_branchElement) return _branchElement->GetNdata();
            throw std::runtime_error("Unclear how to retrive size for branch: "+this->getName());
            return -1;
            
        }
        
        virtual void clear()
        {
        }
        
        virtual const std::string getInfo() const
	    {
	        std::stringstream ss;
	        ss<<"Branch: "<<this->getName()<<" ("<<this->getTypeName()<<"), size="<<size();
	        return ss.str();
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
        
        virtual void clear()
        {
            _data = TYPE();
        }
        
	    virtual void book(TTree* tree)
	    {
	        tree->Branch(this->getName().c_str(),&_data);
	    }
	    
	    virtual void write(TTree*)
	    {
	    }
};

template<class TYPE>
class OutputBranch<std::vector<TYPE>>:
    public Branch<std::vector<TYPE>>
{
    protected:
        std::vector<TYPE> _data;
        int _size;
    public:
        OutputBranch(const std::string& name):
            Branch<std::vector<TYPE>>(name)
        {
        }
        
        virtual std::vector<TYPE>& get()
        {
            return _data;
        }
        
        virtual const std::vector<TYPE>& get() const
        {
            return _data;
        }
        
        virtual size_t size() const
        {
            return _data.size();
        }
        
        virtual void clear()
        {
            _data.clear();
        }
        
        virtual const std::string getInfo() const
	    {
	        std::stringstream ss;
	        ss<<"Branch: "<<this->getName()<<" ("<<this->getTypeName()<<"), size="<<size();
	        return ss.str();
	    }
	    
	    virtual void book(TTree* tree)
	    {
	        tree->Branch(("n"+this->getName()).c_str(),&_size);
	    }
	    
	    virtual void write(TTree*)
	    {
	        _size = this->size();
	    }
};


}

#endif
