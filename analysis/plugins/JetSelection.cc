#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class JetSelection:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Particle>> _jets;
        styr::ConstBranchPtr<std::vector<Particle>> _muons;
        styr::ConstBranchPtr<std::vector<Particle>> _electrons;
        
        styr::BranchPtr<std::vector<Particle>> _selectedJets;
        
        float _minJetPt;
        float _maxJetEta;
        float _minDR;
    
    public:
        JetSelection():
            styr::Module(""),
            _minJetPt(40.),
            _maxJetEta(4.0),
            _minDR(0.4)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minJetPt = config().getOrDefault<float>("minPt",_minJetPt);
            _maxJetEta = config().getOrDefault<float>("maxEta",_maxJetEta);
            _minDR = config().getOrDefault<float>("minDR",_minDR);
        
            _jets = event.getBranch<std::vector<Particle>>(config().get<std::string>("jetSrc"));
            if (config().has("muonSrc"))
            {
                _muons = event.getBranch<std::vector<Particle>>(config().get<std::string>("muonSrc"));
            }
            if (config().has("electronSrc"))
            {
                _electrons = event.getBranch<std::vector<Particle>>(config().get<std::string>("electronSrc"));
            }
            _selectedJets = event.createBranch<std::vector<Particle>>(config().get<std::string>("output"));
        }
        
        bool passJetId(const Particle& jet) const
        {
            if (jet.get<int>("NCharged")==0) return false;
            if (jet.get<int>("NNeutrals")==0) return false;
            return true;
        }
        
        virtual bool analyze(styr::Event&, bool) override
        {
            const std::vector<Particle>& jets = _jets->get();
            if (_jets->size()<2)
            {
                return false;
            }
            for (size_t i = 0; i < _jets->size(); ++i)
            {
                if (jets[i].P4().Pt()<_minJetPt) continue;
                if (std::fabs(jets[i].P4().Eta())>_maxJetEta) continue;
                if (not passJetId(jets[i])) continue;
                float minDR = 10000.;
                if (_muons)
                {
                    const std::vector<Particle>& muons = _muons->get();
                    for (size_t j = 0; j < _muons->size(); ++j)
                    {
                        float dR = jets[i].P4().DeltaR(muons[j].P4());
                        minDR = std::min(minDR,dR);
                    }
                }
                if (minDR<_minDR) continue;
                if (_electrons)
                {
                    const std::vector<Particle>& electrons = _electrons->get();
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


