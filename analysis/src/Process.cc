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

void Process::processFile(TFile* file, const char* treeName, int max, bool filter)
{
    TTree* tree = dynamic_cast<TTree*>(file->Get(treeName));
    if (not tree)
    {
        throw std::runtime_error("Error - cannot find tree with name'"+std::string(treeName)+"' in file '"+std::string(file->GetName())+"'");
    }
    //tree->GetEntry(0);
    Event event(tree);
    
    int maxEntries =tree->GetEntries();
    if (max>0) maxEntries = std::min<int>(max,maxEntries);
    
    std::cout<<"start: nev="<<maxEntries<<", file="<<file->GetName()<<std::endl;
    for (auto module: modules_)
    {
        if (not module)
        {
            throw std::runtime_error("Error - trying to run 'beginFile' on nullptr as module");
        }
        module->beginFile(file,event);
    }
    
    auto t_start = std::chrono::high_resolution_clock::now();
    
    int64_t skipped = 0;
    for (int64_t entry = 0; entry < maxEntries; ++entry)
    {
        tree->GetEntry(entry);
        if (entry>0 and entry%10000==0)
        {
            auto t_end = std::chrono::high_resolution_clock::now();
            double elaspedTimeMs = std::chrono::duration<double, std::milli>(t_end-t_start).count();
            std::cout<<"processing: "<<entry<<"/"<<maxEntries<<"/"<<skipped<<" (entry/max/skipped), rate="<<10000./(elaspedTimeMs/1000.)<<" ev/s"<<std::endl;
            t_start = std::chrono::high_resolution_clock::now();
        }
        if (filter)
        {
            bool accepted = true;
            for (auto module: modules_)
            {
                accepted = module->analyze(event,accepted) and accepted;
            }
            if (not accepted) skipped+=1;
        }
        else
        {
            for (auto module: modules_)
            {
                module->analyze(event,true);
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
    std::cout<<"done: nev="<<(maxEntries-skipped)<<"/"<<maxEntries<<", file="<<file->GetName()<<std::endl;
}

}


