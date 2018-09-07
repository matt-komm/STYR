#include "styr/Process.h"
#include "styr/Module.h"
#include "styr/Event.h"

#include "TTree.h"

#include <iostream>

namespace styr
{

void Process::addModule(Module* module)
{
    if (not module)
    {
        throw std::runtime_error("Error - trying to add nullptr as module");
    }
    modules_.push_back(module);  
}

void Process::processFile(TFile* file, const char* treeName, int max)
{
    TTree* tree = dynamic_cast<TTree*>(file->Get(treeName));
    if (not tree)
    {
        throw std::runtime_error("Error - cannot find tree with name'"+std::string(treeName)+"' in file '"+std::string(file->GetName())+"'");
    }
    //tree->GetEntry(0);
    Event event(tree);
    for (auto module: modules_)
    {
        if (not module)
        {
            throw std::runtime_error("Error - trying to run 'beginFile' on nullptr as module");
        }
        module->beginFile(file,event);
    }
    int maxEntries =tree->GetEntries();
    if (max>0) maxEntries = std::min<int>(20,maxEntries);
    for (int64_t entry = 0; entry < maxEntries; ++entry)
    {
        tree->GetEntry(entry);
        for (auto module: modules_)
        {
            if (not module->analyze(event))
            {
                break;
            }
        }
        event.clearOutputBuffers();
    }
    for (auto module: modules_)
    {
        if (not module)
        {
            throw std::runtime_error("Error - trying to run 'endFile' on nullptr as module");
        }
        module->endFile(file,event);
    }
}

}


