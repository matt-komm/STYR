#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class JetSelection:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Jet>> _jets;
    
    public:
        JetSelection():
            styr::Module("")
        {
        }
        
        virtual void beginFile(TFile*, styr::Event& event)
        {
            _jets = event.getBranch<std::vector<Jet>>("Jet");
        }
        
        virtual void analyze(styr::Event&) override
        {
            std::cout<<"process"<<std::endl;
            std::cout<<_jets->size()<<std::endl;
        }        
        
        virtual ~JetSelection()
        {
        }
};

}

ClassImp(styr::JetSelection)

