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
        
        virtual void beginFile(const TFile*, styr::Event& event) override
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


class MyModuleConfig:
    public styr::Module
{
    protected:
        std::string _valueName;
        float& _ref;
    public:
        MyModuleConfig(const char* name, const styr::Config& config, float& ref):
            Module(name,config),
            _ref(ref)
        {
        }
        
        virtual void beginJob() override
        {
            _valueName = config().get<std::string>("valueName");
        }
        
        virtual void analyze(styr::Event& event) override
        {
            auto value = event.getBranch<float>(_valueName);
            ASSERT_EQ(_ref,value->get());
        }        
        
        virtual ~MyModuleConfig()
        {
        }
};

void test_createModuleInLoop()
{
    for (int i = 0; i < 10; ++i)
    {
        float ref = 0;
        std::unique_ptr<styr::Module> module(new MyModuleInLoop("mymodule",ref));
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
        module->beginFile(nullptr,event);
        for (int64_t entry = 0; entry < tree.GetEntries(); ++entry)
        {
            
            ref = values[entry];
            event.getEntry(entry);
            module->analyze(event);
        }
    }
}

void test_createModuleInBegin()
{
    for (int i = 0; i < 10; ++i)
    {
        float ref = 0;
        std::unique_ptr<styr::Module> module(new MyModuleInBegin("mymodule",ref));
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
        module->beginFile(nullptr,event);
        for (int64_t entry = 0; entry < tree.GetEntries(); ++entry)
        {
            
            ref = values[entry];
            event.getEntry(entry);
            module->analyze(event);
        }
    }
}

void test_moduleConfig()
{
    for (int i = 0; i < 10; ++i)
    {
        float ref = 0;
        std::unique_ptr<styr::Module> module(new MyModuleConfig(
            "mymodule",
            styr::Config().set("valueName","value"+std::to_string(i)),
            ref
        ));
        TTree tree("testTree","testTree");
        float value = 10;
        tree.Branch(("value"+std::to_string(i)).c_str(),&value,("value"+std::to_string(i)+"/F").c_str());
        
        std::vector<float> values;
        for (int j = 0; j < 10; ++j)
        {
            value = 10-0.1*i*i+5.*j;
            tree.Fill();
            values.push_back(value);
        }
        ASSERT_EQ(values.size(),tree.GetEntries());
        styr::Event event(&tree);
        module->beginJob();
        module->beginFile(nullptr,event);
        for (int64_t entry = 0; entry < tree.GetEntries(); ++entry)
        {
            ref = values[entry];
            event.getEntry(entry);
            module->analyze(event);
        }
    }
}

int main()
{
    RUN_TEST(test_createModuleInLoop);
    RUN_TEST(test_createModuleInBegin);
    RUN_TEST(test_moduleConfig);
    return 0;
}
