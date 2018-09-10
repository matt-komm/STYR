#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class MuonSelection:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Muon>> _muons;
        
        styr::BranchPtr<std::vector<Particle>> _selectedMuons;
        
        float _minMuonPt;
        float _maxMuonEta;
        float _muonIso;
    
    public:
        MuonSelection():
            styr::Module(""),
            _minMuonPt(30.),
            _maxMuonEta(4.0),
            _muonIso(0.1)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minMuonPt = config().getOrDefault<float>("minPt",_minMuonPt);
            _maxMuonEta = config().getOrDefault<float>("maxEta",_maxMuonEta);
            _muonIso = config().getOrDefault<float>("iso",_muonIso);
        
            _muons = event.getBranch<std::vector<Muon>>(config().get<std::string>("muonSrc"));
            
            _selectedMuons = event.createBranch<std::vector<Particle>>(config().get<std::string>("output"));
        }
        
        bool passMuonId(const Muon&) const
        {
            return true;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Muon>& muons = _muons->get();
            for (size_t i = 0; i < _muons->size(); ++i)
            {
                if (muons[i].PT<_minMuonPt) continue;
                if (std::fabs(muons[i].Eta)>_maxMuonEta) continue;
                if (muons[i].IsolationVarRhoCorr>_muonIso) continue;
                if (not passMuonId(muons[i])) continue;
                
                _selectedMuons->get().push_back(muons[i]);
            }
            return true;
        }        
        
        virtual ~MuonSelection()
        {
        }
};

}


