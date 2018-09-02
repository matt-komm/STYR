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
        const std::string _valueName;
        size_t _entry;
        styr::ConstBranchPtr<float> _value;
    public:
        MyModuleProcess(const char* name, const std::vector<float>& values, const std::string valueName):
            Module(name),
            _values(values),
            _valueName(valueName),
            _entry(0)
        {
            
        }
        
        virtual void beginFile(TFile* file, styr::Event& event)
        {
            _value = event.getBranch<float>(_valueName.c_str());
        }
        
        virtual void analyze(styr::Event& event) override
        {
            
            if (_entry>=_values.size())
            {
                throw std::runtime_error("Index outside of reference value list");
            }
            std::cout<<_value->get()<<std::endl;
            /*
            ASSERT_EQ(_values[_entry],_value->get());
            _entry++;
            */
        }        
        
        virtual ~MyModuleProcess()
        {
        }
};

void test_processFile()
{
    for (int i = 0; i < 10; ++i)
    {
        std::vector<float> values;
        std::string treeName = "tree"+std::to_string(i);
        std::string valueName = "value"+std::to_string(i);
        {
            TFile f("test.root","RECREATE");
            TTree tree(treeName.c_str(),treeName.c_str());
            tree.SetDirectory(&f);
            float value = i;
            tree.Branch(valueName.c_str(),&value);
            
            for (int j = 0; j < 10; ++j)
            {
                value = 1.+i*0.2-0.1*i*i-2.*j;
                tree.Fill();
                values.push_back(value);
            }
            tree.Write();
            f.Close();
        }
        styr::Process process;
        MyModuleProcess module("module",values,valueName);
        process.addModule(&module);
        TFile f("test.root");
        process.processFile(&f,treeName.c_str());
    }
}

int main()
{
    RUN_TEST(test_processFile);
    return 0;
}
