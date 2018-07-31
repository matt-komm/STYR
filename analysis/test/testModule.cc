#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

class MyModule:
    public styr::Module
{
    public:
        MyModule()
        {
        }
        
        virtual void analyze(styr::Event& event) override
        {
        }
        
        virtual ~MyModule()
        {
        }
};

void test_createModule()
{
    MyModule module;
    TTree tree;
    styr::Event event(&tree);
    module.analyze(event);
}

int main()
{
    RUN_TEST(test_createModule);
    return 0;
}
