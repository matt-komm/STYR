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
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _jets = event.getBranch<std::vector<Jet>>("Jet");
        }
        
        virtual void analyze(styr::Event&) override
        {
            std::cout<<_jets->size()<<std::endl;
            const std::vector<Jet>& jets = _jets->get();
            for (size_t i = 0; i < _jets->size(); ++i)
            {
                std::cout<<"   "<<i<<": "<<jets[i].PT<<std::endl;
            }
        }        
        
        virtual ~JetSelection()
        {
        }
};

}

ClassImp(styr::JetSelection)

