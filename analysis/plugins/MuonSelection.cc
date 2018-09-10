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
        styr::BranchPtr<int> _vetoMuons;
        
        float _minMuonPt;
        float _maxMuonEta;
        float _muonIso;
        
        float _minVetoMuonPt;
        float _maxVetoMuonEta;
        float _muonVetoIso;
    
    public:
        MuonSelection():
            styr::Module(""),
            _minMuonPt(40.),
            _maxMuonEta(3.0),
            _muonIso(0.05),
            
            _minVetoMuonPt(10.),
            _maxVetoMuonEta(3.0),
            _muonVetoIso(0.25)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minMuonPt = config().getOrDefault<float>("minPt",_minMuonPt);
            _maxMuonEta = config().getOrDefault<float>("maxEta",_maxMuonEta);
            _muonIso = config().getOrDefault<float>("iso",_muonIso);
            
            _minVetoMuonPt = config().getOrDefault<float>("minVetoPt",_minVetoMuonPt);
            _maxVetoMuonEta = config().getOrDefault<float>("maxVetoEta",_maxVetoMuonEta);
            _muonVetoIso = config().getOrDefault<float>("isoVeto",_muonVetoIso);
        
            _muons = event.getBranch<std::vector<Muon>>(config().get<std::string>("muonSrc"));
            
            _selectedMuons = event.createBranch<std::vector<Particle>>(config().get<std::string>("output"));
            _vetoMuons = event.createBranch<int>("nvetomuons");
        }
        
        bool passMuonId(const Muon&) const
        {
            return true;
        }
        
        bool passMuonSelection(const Muon& muon) const
        {
            if (muon.PT<_minMuonPt) return false;
            if (std::fabs(muon.Eta)>_maxMuonEta) return false;
            if (muon.IsolationVarRhoCorr>_muonIso) return false;
            if (not passMuonId(muon)) return false;
            return true;
        }
        
        bool passMuonVeto(const Muon& muon) const
        {
            if (muon.PT<_minVetoMuonPt) return false;
            if (std::fabs(muon.Eta)>_maxVetoMuonEta) return false;
            if (muon.IsolationVarRhoCorr>_muonVetoIso) return false;
            return true;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Muon>& muons = _muons->get();
            int nVetoMuons = 0;
            for (size_t i = 0; i < _muons->size(); ++i)
            {
                if (passMuonSelection(muons[i]))
                {
                    _selectedMuons->get().push_back(muons[i]);
                }
                else if (passMuonVeto(muons[i]))
                {
                    nVetoMuons+=1;
                }
            }
            _vetoMuons->get()=nVetoMuons;
            return true;
        }        
        
        virtual ~MuonSelection()
        {
        }
};

}


