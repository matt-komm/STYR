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
        
        styr::BranchPtr<float> _topMass;
        
        styr::BranchPtr<float> _cosThetaPL;
        
        styr::BranchPtr<float> _ljetAbsEta;
    
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
            
            _topMass = event.createBranch<float>(config().get<std::string>("output")+"_mass");
            
            _cosThetaPL = event.createBranch<float>(config().get<std::string>("output")+"_cosThetaPL");
            
            _ljetAbsEta = event.createBranch<float>(config().get<std::string>("output")+"_ljetAbsEta");
        }


        virtual bool analyze(styr::Event&) override
        {
            
            if (_leptons->size()!=1)
            {
                _topMass->get()=-10;
                _cosThetaPL->get()=-10;
                _ljetAbsEta->get()=-10;
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
                _cosThetaPL->get()=-10;
                _ljetAbsEta->get()=-10;
                return true;
            }
            //std::cout<<ljet.P4().Eta()<<", "<<std::fabs(ljet.P4().Eta())<<", "<<_ljets->size()<<std::endl;
            _ljetAbsEta->get() = std::fabs(ljet.P4().Eta());
            
            TLorentzVector topVector(0,0,0,0);
            topVector+=lepton.P4();
            topVector+=_neutrino->get().P4();
            topVector+=bjet.P4();
            
            _topMass->get()=topVector.M();
            
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


