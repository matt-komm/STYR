#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class ElectronSelection:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Electron>> _electrons;
        
        styr::BranchPtr<std::vector<Particle>> _selectedElectrons;
        styr::BranchPtr<int> _vetoElectrons;
        
        float _minElectronPt;
        float _maxElectronEta;
        float _electronIso;
        
        float _minVetoElectronPt;
        float _maxVetoElectronEta;
        float _electronVetoIso;
    
    public:
        ElectronSelection():
            styr::Module(""),
            _minElectronPt(40.),
            _maxElectronEta(3.0),
            _electronIso(0.05),
            
            _minVetoElectronPt(10.),
            _maxVetoElectronEta(3.0),
            _electronVetoIso(0.25)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minElectronPt = config().getOrDefault<float>("minPt",_minElectronPt);
            _maxElectronEta = config().getOrDefault<float>("maxEta",_maxElectronEta);
            _electronIso = config().getOrDefault<float>("iso",_electronIso);
            
            _minVetoElectronPt = config().getOrDefault<float>("minVetoPt",_minVetoElectronPt);
            _maxVetoElectronEta = config().getOrDefault<float>("maxVetoEta",_maxVetoElectronEta);
            _electronVetoIso = config().getOrDefault<float>("isoVeto",_electronVetoIso);
        
            _electrons = event.getBranch<std::vector<Electron>>(config().get<std::string>("electronSrc"));
            
            _selectedElectrons = event.createBranch<std::vector<Particle>>(config().get<std::string>("output"));
            _vetoElectrons = event.createBranch<int>("nvetoelectrons");
        }
        
        bool passElectronId(const Electron&) const
        {
            return true;
        }
        
        bool passElectronSelection(const Electron& electron) const
        {
            if (electron.PT<_minElectronPt) return false;
            if (std::fabs(electron.Eta)>_maxElectronEta) return false;
            if (electron.IsolationVarRhoCorr>_electronIso) return false;
            if (not passElectronId(electron)) return false;
            return true;
        }
        
        bool passElectronVeto(const Electron& electron) const
        {
            if (electron.PT<_minVetoElectronPt) return false;
            if (std::fabs(electron.Eta)>_maxVetoElectronEta) return false;
            if (electron.IsolationVarRhoCorr>_electronVetoIso) return false;
            return true;
        }
        
        virtual bool analyze(styr::Event&, bool pass) override
        {
            const std::vector<Electron>& electrons = _electrons->get();
            int nVetoElectrons = 0;
            for (size_t i = 0; i < _electrons->size(); ++i)
            {
                if (passElectronSelection(electrons[i]))
                {
                    _selectedElectrons->get().push_back(electrons[i]);
                }
                else if (passElectronVeto(electrons[i]))
                {
                    nVetoElectrons+=1;
                }
            }
            _vetoElectrons->get()=nVetoElectrons;
            return true;
        }        
        
        virtual ~ElectronSelection()
        {
        }
};

}


