#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class TopReconstruction:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Particle>> _leptons;
        
        styr::ConstBranchPtr<std::vector<Particle>> _ljets;
        styr::ConstBranchPtr<std::vector<Particle>> _bjets;
        
        styr::ConstBranchPtr<Particle> _neutrino;
        
        
        
        styr::BranchPtr<float> _cosThetaPL;
        
        styr::BranchPtr<float> _ljetPt;
        styr::BranchPtr<float> _ljetEta;
        
        styr::BranchPtr<float> _topMass;
        styr::BranchPtr<float> _topPt;
        styr::BranchPtr<float> _topY;
    
    public:
        TopReconstruction():
            styr::Module("")
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _leptons = event.getBranch<std::vector<Particle>>(config().get<std::string>("leptonSrc"));
            
            _ljets = event.getBranch<std::vector<Particle>>(config().get<std::string>("ljetSrc"));
            _bjets = event.getBranch<std::vector<Particle>>(config().get<std::string>("bjetSrc"));
            
            _neutrino = event.getBranch<Particle>(config().get<std::string>("neutrinoSrc"));
            
            _cosThetaPL = event.createBranch<float>(config().get<std::string>("output")+"_cosThetaPL");
            
            _ljetPt = event.createBranch<float>(config().get<std::string>("output")+"_ljetPt");
            _ljetEta = event.createBranch<float>(config().get<std::string>("output")+"_ljetEta");
            
            _topMass = event.createBranch<float>(config().get<std::string>("output")+"_topMass");
            _topPt = event.createBranch<float>(config().get<std::string>("output")+"_topPt");
            _topY = event.createBranch<float>(config().get<std::string>("output")+"_topY");
        }


        virtual bool analyze(styr::Event&, bool pass) override
        {
            if (not pass) return pass;
            if (_leptons->size()==0)
            {
                _topMass->get()=-10;
                _topPt->get()=-10;
                _topY->get()=-10;
                _cosThetaPL->get()=-10;
                _ljetEta->get()=-10;
                _ljetPt->get()=-10;
                return true;
            }
            Particle lepton = _leptons->get()[0];
            
            Particle ljet;
            Particle bjet;
            if (_ljets->size()>=1 and _bjets->size()>=1)
            {
            
                const std::vector<Particle> ljets = _ljets->get();
                const std::vector<Particle> bjets = _bjets->get();
            
                ljet = ljets[0];
                bjet = bjets[0];
                //most forward ljet
                for (size_t i = 1; i < _ljets->size(); ++i)
                {
                    if (std::fabs(ljet.P4().Eta())<std::fabs(ljets[i].P4().Eta()))
                    {
                        ljet = ljets[i];
                    }
                }
                //hardest pt bjet
                for (size_t i = 1; i < _bjets->size(); ++i)
                {
                    if (bjet.P4().Pt()<bjets[i].P4().Pt())
                    {
                        bjet = bjets[i];
                    }
                }
            }
            else
            {
                _topMass->get()=-10;
                _topPt->get()=-10;
                _topY->get()=-10;
                _cosThetaPL->get()=-10;
                _ljetPt->get()=-10;
                _ljetEta->get()=-10;
                return true;
            }
            _ljetPt->get() = ljet.P4().Pt();
            _ljetEta->get() = ljet.P4().Eta();
            
            TLorentzVector topVector(0,0,0,0);
            topVector+=lepton.P4();
            topVector+=_neutrino->get().P4();
            topVector+=bjet.P4();
            
            _topMass->get()=topVector.M();
            
            _topPt->get()=topVector.Pt();
            _topY->get()=topVector.Rapidity();
            
            TLorentzVector ljetInTop = ljet.P4();
            ljetInTop.Boost(-topVector.BoostVector());
            
            TLorentzVector leptonInTop = lepton.P4();
            leptonInTop.Boost(-topVector.BoostVector());
            
            float cosThetaPL = (ljetInTop.Px()*leptonInTop.Px()+ljetInTop.Py()*leptonInTop.Py()+ljetInTop.Pz()*leptonInTop.Pz());
            cosThetaPL = cosThetaPL/(ljetInTop.P()*leptonInTop.P());
            
            _cosThetaPL->get()=cosThetaPL;
            
            return true;
        }        
        
        virtual ~TopReconstruction()
        {
        }
};

}


