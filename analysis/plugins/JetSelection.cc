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
        styr::ConstBranchPtr<std::vector<Muon>> _muons;
        styr::ConstBranchPtr<std::vector<Electron>> _electrons;
        
        styr::BranchPtr<std::vector<Jet>> _selectedJets;
        
        float _minJetPt;
        float _maxJetEta;
        float _minDR;
    
    public:
        JetSelection():
            styr::Module(""),
            _minJetPt(40.),
            _maxJetEta(4.7),
            _minDR(0.4)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minJetPt = config().getOrDefault<float>("minPt",_minJetPt);
            _maxJetEta = config().getOrDefault<float>("maxEta",_maxJetEta);
            _minDR = config().getOrDefault<float>("minDR",_minDR);
        
            _jets = event.getBranch<std::vector<Jet>>(config().get<std::string>("jetSrc"));
            if (config().has("muonSrc"))
            {
                _muons = event.getBranch<std::vector<Muon>>(config().get<std::string>("muonSrc"));
            }
            if (config().has("electronSrc"))
            {
                _electrons = event.getBranch<std::vector<Electron>>(config().get<std::string>("electronSrc"));
            }
            _selectedJets = event.createBranch<std::vector<Jet>>(config().get<std::string>("output"));
        }
        
        bool passJetId(const Jet& jet) const
        {
            if (jet.NCharged==0) return false;
            return true;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Jet>& jets = _jets->get();
            for (size_t i = 0; i < _jets->size(); ++i)
            {
                if (jets[i].PT<_minJetPt) continue;
                if (std::fabs(jets[i].Eta)>_maxJetEta) continue;
                if (not passJetId(jets[i])) continue;
                float minDR = 10000.;
                if (_muons)
                {
                    const std::vector<Muon>& muons = _muons->get();
                    for (size_t j = 0; j < _muons->size(); ++j)
                    {
                        float dR = jets[i].P4().DeltaR(muons[j].P4());
                        minDR = std::min(minDR,dR);
                    }
                }
                if (_electrons)
                {
                    const std::vector<Electron>& electrons = _electrons->get();
                    for (size_t j = 0; j < _electrons->size(); ++j)
                    {
                        float dR = jets[i].P4().DeltaR(electrons[j].P4());
                        minDR = std::min(minDR,dR);
                    }
                }
                if (minDR<_minDR) continue;
                _selectedJets->get().push_back(jets[i]);
            }
            return true;
        }        
        
        virtual ~JetSelection()
        {
        }
};

}


