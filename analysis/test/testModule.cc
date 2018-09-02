#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"


class MyModuleInBegin:
    public styr::Module
{
    protected:
        float& _ref;
        styr::ConstBranchPtr<float> _branch;
    public:
        MyModuleInBegin(const char* name, float& ref):
            Module(name),
            _ref(ref),
            _branch(nullptr)
        {
        }
        
        virtual void beginFile(TFile*, styr::Event& event)
        {
            _branch = event.getBranch<float>("value");
        }
        
        virtual void analyze(styr::Event&) override
        {
            ASSERT_EQ(_ref,_branch->get());
        }        
        
        virtual ~MyModuleInBegin()
        {
        }
};

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
    RUN_TEST(test_createModuleInBegin);
    return 0;
}
