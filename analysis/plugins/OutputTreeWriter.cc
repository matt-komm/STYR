#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

#include "classes/DelphesClasses.h"

#include "TH1F.h"

namespace styr
{

class OutputTreeWriter:
    public styr::Module
{
    protected:
        
    
        styr::ConstBranchPtr<std::vector<HepMCEvent>> _event;

        std::vector<std::string> _branchNames;
        std::vector<BranchBasePtr> _branches;
        
        
        std::unique_ptr<TFile> _output;
        TTree* _tree;
        TH1F* _histEvents;    
        
        std::string _prefix;
        
        float _genWeight;
    public:
        OutputTreeWriter():
            styr::Module(""),
            _tree(nullptr),
            _histEvents(nullptr),
            _prefix("")
            
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
            _event = event.getBranch<std::vector<HepMCEvent>>("Event");
            _prefix = config().get<std::string>("output");
        
            if (_output and _tree and _histEvents)
            {
                _histEvents->Write();
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
            _output.reset(new TFile((_prefix+"/"+fileName+"_friend.root").c_str(),"RECREATE"));
            _histEvents = new TH1F("nevents","",2,-2,2);
            _histEvents->SetDirectory(_output.get());
            _histEvents->Sumw2();
            _tree = new TTree("Events","Events");
            _tree->SetDirectory(_output.get());
            _branches.clear();
            for (auto& branchName: _branchNames)
            {
                BranchBasePtr branch = event.getBranchBase(branchName);
                branch->book(_tree);
                _branches.push_back(branch);
            }
            _tree->Branch("genWeight",&_genWeight,"genWeight/F");
        }

        virtual bool analyze(styr::Event&, bool pass) override
        {
            _genWeight = _event->get()[0].Weight;
            if (pass)
            {
                for (auto& branch: _branches)
                {
                    branch->write(_tree);
                    //std::cout<<branch->getInfo()<<std::endl;
                }
                _tree->Fill();
            }
            
            _histEvents->Fill(_genWeight>0?1:-1,_genWeight);
            return true;
        }        
        
        virtual void endFile(const TFile*,styr::Event&) override
        {
            if (_output and _tree)
            {
                _histEvents->Write();
                _tree->Write();
                _output->Close();
                _output.reset();
            }
        }
        
        virtual ~OutputTreeWriter()
        {
        }
};

}


