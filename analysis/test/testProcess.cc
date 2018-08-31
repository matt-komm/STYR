#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Process.h"
#include "styr/Module.h"

#include "TFile.h"
#include "TTree.h"

class MyModuleProcess:
    public styr::Module
{
    protected:
        const std::vector<float> _values;
        size_t _entry;
    public:
        MyModuleProcess(const char* name, const std::vector<float>& values):
            Module(name),
            _values(values),
            _entry(0)
        {
        }
        
        virtual void analyze(styr::Event& event) override
        {
            const styr::Branch<float>& value = event.getBranch<float>("value");
            if (_entry>=_values.size())
            {
                throw std::runtime_error("Index outside of reference value list");
            }
            ASSERT_EQ(_values[_entry],value.get());
            _entry++;
        }        
        
        virtual ~MyModuleProcess()
        {
        }
};

void test_processFile()
{
    std::vector<float> values;
    {
        TFile f("test.root","RECREATE");
        TTree tree("tree","tree");
        tree.SetDirectory(&f);
        float value = 10;
        tree.Branch("value",&value,"value/F");
        
        for (int i = 0; i < 10; ++i)
        {
            value = 1.+i*0.2-0.1*i*i;
            tree.Fill();
            values.push_back(value);
        }
        tree.Write();
        f.Close();
    }
    styr::Process process;
    MyModuleProcess module("module",values);
    process.addModule(&module);
    TFile f("test.root");
    process.processFile(&f,"tree");
}

int main()
{
    RUN_TEST(test_processFile);
    return 0;
}
