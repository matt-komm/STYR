#include "styr/Event.h"

namespace styr
{

Event::Event(TTree* tree):
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

void Event::clearOutputBuffers()
{
    for (auto branchPair: _outputBranchMap)
    {
        branchPair.second->clear();
    }
}

int64_t Event::next()
{
    _entry+=1;
    if (_entry>=_tree->GetEntries())
    {
        return -1;
    }
    _tree->GetEntry(_entry);
    return _entry;
}

int64_t Event::getEntry(int64_t entry)
{
    if (entry>=_tree->GetEntries())
    {
        return -1;
    }
    _entry = entry;
    _tree->GetEntry(_entry);
    return _entry;
}

Event::~Event()
{
    _inputTreeBranches.clear();
    _inputBranchMap.clear();
    _outputBranchMap.clear();
}
       

}

