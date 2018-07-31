#ifndef STYR_EVENT_H
#define STYR_EVENT_H

#include "styr/Branch.h"

#include "TTree.h"

#include <string>
#include <unordered_map>

namespace styr
{

class Event
{
    private:
        TTree* _tree;
        
        std::unordered_map<std::string, TBranch*> _inputTreeBranches;
        std::unordered_map<std::string, std::shared_ptr<BranchBase>> _branchMap;
        
    public:
        Event(TTree* tree):
            _tree(tree)
        {
            auto array = _tree->GetListOfBranches();
            for (int i = 0; i < array->GetSize(); ++i)
            {
                auto obj = array->At(i);
                TBranch* branch = dynamic_cast<TBranch*>(obj);
                if (branch)
                {
                    _inputTreeBranches[branch->GetName()] = branch;
                }
            }
        }
        
        template<class TYPE>
        const Branch<TYPE>& getBranch(const std::string& name) const
        {
            auto it = _branchMap.find(name);
            if (it==_branchMap.end())
            {
                if (_inputTreeBranches.find(name)==_inputTreeBranches.end())
                {
                    throw std::runtime_error("Branch with name '"+name+"' does not exist!");
                }
                //TODO: get branch from tree
            }
            //get branch from map
            Branch<TYPE>* branch = dynamic_cast<Branch<TYPE>*>(it->second.get());
            if (not branch)
            {
                throw std::runtime_error("Cannot cast branch with name '"+name+"' of type '"+it->second->getTypeName()+"' to type '"+std::string(typeid(TYPE).name())+"'");
            }
            return *branch;
        }
        
        template<class TYPE>
        Branch<TYPE>& createBranch(const std::string& name)
        {
            auto it = _branchMap.find(name);
            if (it!=_branchMap.end())
            {
                throw std::runtime_error("Cannot create branch with name '"+name+"'. It does already exist!");
            }
            if (_inputTreeBranches.find(name)!=_inputTreeBranches.end())
            {
                throw std::runtime_error("Branch with name '"+name+"' already exists in input tree!");
            }
            
            auto branch = Branch<TYPE>::createNewBranch(name,_tree);
            _branchMap.emplace(std::make_pair(name,branch));
            return *branch;
        }
};

}

#endif
