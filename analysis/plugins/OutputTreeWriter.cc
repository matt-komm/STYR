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
        std::vector<BranchBasePtr> _branches;
        
        std::unique_ptr<TFile> _output;
        std::unique_ptr<TTree> _tree;
    
    public:
        OutputTreeWriter():
            styr::Module("")
            
        {
        }
        
        void addBranch(const char* name)
        {
            _branchNames.push_back(name);
        }
        
        virtual void beginJob() override
        {
            
        }
        
        
        virtual void beginFile(const TFile* f,styr::Event& event) override
        {
            if (_output and _tree)
            {
                _tree->Write();
                _output->Close();
            }
            std::string fileName = f->GetName();
            size_t posSlash = fileName.rfind("/");
            size_t posDot = fileName.rfind(".");
            if (posSlash>=fileName.size())
            {
                fileName = fileName.substr(0,posDot);
            }
            else
            {
                fileName = fileName.substr(posSlash+1,posDot);
            }
            _output.reset(new TFile((fileName+"_friend.root").c_str(),"RECREATE"));
            _tree.reset(new TTree("Events","Events"));
            _tree->SetDirectory(_output.get());
            _branches.clear();
            for (auto& branchName: _branchNames)
            {
                BranchBasePtr branch = event.getBranchBase(branchName);
                branch->book(_tree.get());
                _branches.push_back(branch);
            }
        }

        virtual bool analyze(styr::Event&) override
        {
            for (auto& branch: _branches)
            {
                branch->write(_tree.get());
                //std::cout<<branch->getInfo()<<std::endl;
            }
            _tree->Fill();
            return true;
        }        
        
        virtual void endFile(const TFile*,styr::Event&) override
        {
            if (_output and _tree)
            {
                _tree->Write();
                _output->Close();
            }
        }
        
        virtual ~OutputTreeWriter()
        {
        }
};

}


