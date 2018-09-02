#ifndef STYR_EVENT_H
#define STYR_EVENT_H

#include "styr/Branch.h"

#include "TTree.h"

#include <string>
#include <unordered_map>
#include <iostream>

namespace styr
{

class Event
{
    private:
        TTree* _tree;
        int64_t _entry;
        
        std::unordered_map<std::string, TBranch*> _inputTreeBranches;
        std::unordered_map<std::string, std::shared_ptr<const BranchBase>> _inputBranchMap;
        std::unordered_map<std::string, std::shared_ptr<BranchBase>> _outputBranchMap;
        
    public:
        Event(TTree* tree):
            _tree(tree),
            _entry(0)
        {
            _tree->GetEntry(_entry);
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
        
        int64_t next()
        {
            _entry+=1;
            if (_entry>=_tree->GetEntries())
            {
                return -1;
            }
            _tree->GetEntry(_entry);
            return _entry;
        }
        
        inline int64_t entry() const
        {
            return _entry;
        }
        
        inline int64_t entries() const
        {
            return _tree->GetEntries();
        }
        
        inline int64_t getEntry(int64_t entry)
        {
            if (entry>=_tree->GetEntries())
            {
                return -1;
            }
            _entry = entry;
            _tree->GetEntry(_entry);
            return _entry;
        }
        
        template<class TYPE>
        ConstBranchPtr<TYPE> getBranch(const std::string& name)
        {
            //check if branch has already been loaded
            auto itInput = _inputBranchMap.find(name);
            if (itInput!=_inputBranchMap.end())
            {
                std::shared_ptr<const Branch<TYPE>> branch = std::dynamic_pointer_cast<const Branch<TYPE>>(itInput->second);
                if (not branch)
                {
                    throw std::runtime_error("Cannot cast input branch of type '"+itInput->second->getTypeName()+"' to '"+(typeid(TYPE).name())+"'");
                } 
                return branch;
            }
            
            //check if branch has been written by another module
            auto itCache = _outputBranchMap.find(name);
            if (itCache!=_outputBranchMap.end())
            {
                std::shared_ptr<const Branch<TYPE>> branch = std::dynamic_pointer_cast<const Branch<TYPE>>(itCache->second);
                if (not branch)
                {
                    throw std::runtime_error("Cannot cast cached branch of type '"+itCache->second->getTypeName()+"' to '"+(typeid(TYPE).name())+"'");
                }
                return branch;
            }

            //check if branch is in input tree and create it
            auto itBranch = _inputTreeBranches.find(name);
            if (itBranch == _inputTreeBranches.end())
            {
                throw std::runtime_error("Branch with name '"+name+"' not found in input tree");
            }
            std::shared_ptr<const Branch<TYPE>> branch(new InputBranch<TYPE>(name,itBranch->second));
            std::shared_ptr<const BranchBase> branchBase = std::dynamic_pointer_cast<const BranchBase>(branch);
            _inputBranchMap.emplace(name,branchBase);
            _tree->GetEntry(_entry);
            return branch;
        }
        
        template<class TYPE>
        BranchPtr<TYPE> createBranch(const std::string& name)
        {
            auto it = _outputBranchMap.find(name);
            if (it!=_outputBranchMap.end())
            {
                throw std::runtime_error("Cannot create branch with name '"+name+"'. It does already exist!");
            }
            
            std::shared_ptr<Branch<TYPE>> branch(new OutputBranch<TYPE>(name));
            _outputBranchMap.emplace(std::make_pair(name,branch));
            return branch;
        }
        
        
        ~Event()
        {
            _inputTreeBranches.clear();
            _inputBranchMap.clear();
            _outputBranchMap.clear();
        }
};

}

#endif
