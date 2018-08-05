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
        std::unordered_map<std::string, std::shared_ptr<const BranchBase>> _inputBranchMap;
        std::unordered_map<std::string, std::shared_ptr<BranchBase>> _cacheBranchMap;
        
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
            auto itInput = _inputBranchMap.find(name);
            if (itInput!=_inputBranchMap.end())
            {
                std::shared_ptr<const Branch<TYPE>> branch = std::dynamic_pointer_cast<const Branch<TYPE>>(itInput->second);
                if (not branch)
                {
                    throw std::runtime_error("Cannot cast input branch of type '"+itInput->second->getTypeName()+"' to '"+(typeid(TYPE).name())+"'");
                } 
                return *branch;
            }
            auto itCache = _cacheBranchMap.find(name);
            if (itCache!=_cacheBranchMap.end())
            {
                std::shared_ptr<const Branch<TYPE>> branch = std::dynamic_pointer_cast<const Branch<TYPE>>(itCache->second);
                if (not branch)
                {
                    throw std::runtime_error("Cannot cast cached branch of type '"+itCache->second->getTypeName()+"' to '"+(typeid(TYPE).name())+"'");
                }
                return *branch;
            }
            if (_inputTreeBranches.find(name)==_inputTreeBranches.end())
            {
                throw std::runtime_error("Branch with name '"+name+"' does not exist!");
            }
            //TODO: get branch from tree
            std::shared_ptr<Branch<TYPE>> branch(new BranchFromTTree<TYPE>(name,_tree));
            
            return *branch;
            
        }
        
        template<class TYPE>
        Branch<TYPE>& createBranch(const std::string& name)
        {
            auto it = _cacheBranchMap.find(name);
            if (it!=_cacheBranchMap.end())
            {
                throw std::runtime_error("Cannot create branch with name '"+name+"'. It does already exist!");
            }
            
            std::shared_ptr<Branch<TYPE>> branch(new BranchCached<TYPE>(name));
            _cacheBranchMap.emplace(std::make_pair(name,branch));
            return *branch;
        }
};

}

#endif
