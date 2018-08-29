#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

class MyModule:
    public styr::Module
{
    protected:
        float& ref_;
    public:
        MyModule(const char* name, float& ref):
            Module(name),
            ref_(ref)
        {
        }
        
        virtual void analyze(styr::Event& event) override
        {
            const styr::Branch<float>& value = event.getBranch<float>("value");
            //ASSERT_EQ(ref_,value.get());
            std::cout<<ref_<<", "<<value.get()<<std::endl;
        }        
        
        virtual ~MyModule()
        {
        }
};

void test_createModule()
{
    for (int i = 0; i < 10; ++i)
    {
        float ref = 0;
        MyModule module("mymodule",ref);
        TTree tree(
            ("testTree"+std::to_string(i)).c_str(),
            ("testTree"+std::to_string(i)).c_str()
        );
        float value = 0;
        tree.Branch("value",&value,"value/F");
        
        std::vector<float> values;
        for (int j = 0; j < 10; ++j)
        {
            value = j+100*i;//-0.1*i*i+10.*j;
            std::cout<<"fill: "<<value<<std::endl;
            tree.Fill();
            values.push_back(value);
        }
        ASSERT_EQ(values.size(),tree.GetEntries());
        tree.GetEntry(0);
        styr::Event event(&tree);
        for (int64_t entry = 0; entry < tree.GetEntries(); ++entry)
        {
            tree.GetEntry(entry);
            ref = values[entry];
            module.analyze(event);
        }
    }
}

int main()
{
    RUN_TEST(test_createModule);
    return 0;
}
