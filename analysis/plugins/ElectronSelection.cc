#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class ElectronSelection:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Electron>> _electrons;
        
        styr::BranchPtr<std::vector<Electron>> _selectedElectrons;
        
        float _minElectronPt;
        float _maxElectronEta;
        float _electronIso;
    
    public:
        ElectronSelection():
            styr::Module(""),
            _minElectronPt(50.),
            _maxElectronEta(4.0),
            _electronIso(0.1)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minElectronPt = config().getOrDefault<float>("minPt",_minElectronPt);
            _maxElectronEta = config().getOrDefault<float>("maxEta",_maxElectronEta);
            _electronIso = config().getOrDefault<float>("iso",_electronIso);
        
            _electrons = event.getBranch<std::vector<Electron>>(config().get<std::string>("electronSrc"));
            
            _selectedElectrons = event.createBranch<std::vector<Electron>>(config().get<std::string>("output"));
        }
        
        bool passElectronId(const Electron&) const
        {
            return true;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Electron>& electrons = _electrons->get();
            for (size_t i = 0; i < _electrons->size(); ++i)
            {
                if (electrons[i].PT<_minElectronPt) continue;
                if (std::fabs(electrons[i].Eta)>_maxElectronEta) continue;
                if (electrons[i].IsolationVarRhoCorr>_electronIso) continue;
                if (not passElectronId(electrons[i])) continue;
                
                _selectedElectrons->get().push_back(electrons[i]);
            }
            return true;
        }        
        
        virtual ~ElectronSelection()
        {
        }
};

}


