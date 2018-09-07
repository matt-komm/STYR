#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class OutputTreeWriter:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Jet>> _jets;
        styr::ConstBranchPtr<std::vector<Muon>> _muons;
        styr::ConstBranchPtr<std::vector<Electron>> _electrons;
        
        std::vector<std::string> _branchNames;
        std::vector<ConstBranchBasePtr> _branches;
    
    public:
        OutputTreeWriter():
            styr::Module("")
            
        {
        }
        
        void addBranch(const char* name)
        {
            _branchNames.push_back(name);
        }
        
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _branches.clear();
            for (auto& branchName: _branchNames)
            {
                _branches.push_back(event.getBranchBase(branchName));
            }
        }

        virtual bool analyze(styr::Event&) override
        {
            for (auto& branch: _branches)
            {
                std::cout<<branch->getInfo()<<std::endl;
            }
            return true;
        }        
        
        virtual ~OutputTreeWriter()
        {
        }
};

}


