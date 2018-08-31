#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

class MyModuleInLoop:
    public styr::Module
{
    protected:
        float& ref_;
    public:
        MyModuleInLoop(const char* name, float& ref):
            Module(name),
            ref_(ref)
        {
        }
        
        virtual void analyze(styr::Event& event) override
        {
            auto value = event.getBranch<float>("value");
            ASSERT_EQ(ref_,value->get());
        }        
        
        virtual ~MyModuleInLoop()
        {
        }
};

class MyModuleInBegin:
    public styr::Module
{
    protected:
        float& ref_;
        styr::ConstBranchPtr<float> branch_;
    public:
        MyModuleInBegin(const char* name, float& ref):
            Module(name),
            ref_(ref),
            branch_(nullptr)
        {
        }
        
        virtual void beginFile(TFile*, styr::Event& event)
        {
            branch_ = event.getBranch<float>("value");
        }
        
        virtual void analyze(styr::Event&) override
        {
            ASSERT_EQ(ref_,branch_->get());
        }        
        
        virtual ~MyModuleInBegin()
        {
        }
};

void test_createModuleInLoop()
{
    for (int i = 0; i < 10; ++i)
    {
        float ref = 0;
        MyModuleInLoop module("mymodule",ref);
        TTree tree("testTree","testTree");
        float value = 10;
        tree.Branch("value",&value,"value/F");
        
        std::vector<float> values;
        for (int j = 0; j < 10; ++j)
        {
            value = 10-0.1*i*i+5.*j;
            tree.Fill();
            values.push_back(value);
        }
        ASSERT_EQ(values.size(),tree.GetEntries());
        styr::Event event(&tree);
        module.beginFile(nullptr,event);
        for (int64_t entry = 0; entry < tree.GetEntries(); ++entry)
        {
            
            ref = values[entry];
            event.getEntry(entry);
            module.analyze(event);
        }
    }
}

void test_createModuleInBegin()
{
    for (int i = 0; i < 10; ++i)
    {
        float ref = 0;
        MyModuleInBegin module("mymodule",ref);
        TTree tree("testTree","testTree");
        float value = 10;
        tree.Branch("value",&value,"value/F");
        
        std::vector<float> values;
        for (int j = 0; j < 10; ++j)
        {
            value = 10-0.1*i*i+5.*j;
            tree.Fill();
            values.push_back(value);
        }
        ASSERT_EQ(values.size(),tree.GetEntries());
        styr::Event event(&tree);
        module.beginFile(nullptr,event);
        for (int64_t entry = 0; entry < tree.GetEntries(); ++entry)
        {
            
            ref = values[entry];
            event.getEntry(entry);
            module.analyze(event);
        }
    }
}

int main()
{
    RUN_TEST(test_createModuleInLoop);
    RUN_TEST(test_createModuleInBegin);
    return 0;
}
