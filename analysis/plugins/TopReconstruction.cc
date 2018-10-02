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
        
        styr::ConstBranchPtr<std::vector<Jet>> _genJets;
        
        styr::BranchPtr<float> _cosThetaPL;
        
        styr::BranchPtr<int> _bjetIsPU;
        styr::BranchPtr<float> _bjetPt;
        styr::BranchPtr<float> _bjetEta;
        
        styr::BranchPtr<int> _ljetIsPU;
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
            
            _bjetIsPU = event.createBranch<int>(config().get<std::string>("output")+"_bjetIsPU");
            _bjetPt = event.createBranch<float>(config().get<std::string>("output")+"_bjetPt");
            _bjetEta = event.createBranch<float>(config().get<std::string>("output")+"_bjetEta");
            
            _ljetIsPU = event.createBranch<int>(config().get<std::string>("output")+"_ljetIsPU");
            _ljetPt = event.createBranch<float>(config().get<std::string>("output")+"_ljetPt");
            _ljetEta = event.createBranch<float>(config().get<std::string>("output")+"_ljetEta");
            
            _topMass = event.createBranch<float>(config().get<std::string>("output")+"_topMass");
            _topPt = event.createBranch<float>(config().get<std::string>("output")+"_topPt");
            _topY = event.createBranch<float>(config().get<std::string>("output")+"_topY");
            
            _genJets = event.getBranch<std::vector<Jet>>("GenJet");
        }


        virtual bool analyze(styr::Event&, bool pass) override
        {
            if (_leptons->size()==0)
            {
                _topMass->get()=-10;
                _topPt->get()=-10;
                _topY->get()=-10;
                _cosThetaPL->get()=-10;
                
                _bjetIsPU->get()=-1;
                _bjetEta->get()=-10;
                _bjetPt->get()=-10;
                
                _ljetIsPU->get()=-1;
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
                
                _bjetIsPU->get()=-1;
                _bjetEta->get()=-10;
                _bjetPt->get()=-10;
                
                _ljetIsPU->get()=-1;
                _ljetEta->get()=-10;
                _ljetPt->get()=-10;
                return true;
            }
            _ljetIsPU->get() = 0;
            _ljetPt->get() = ljet.P4().Pt();
            _ljetEta->get() = ljet.P4().Eta();
            
            _bjetIsPU->get() = 0;
            _bjetPt->get() = bjet.P4().Pt();
            _bjetEta->get() = bjet.P4().Eta();
            
            const std::vector<Jet>& genJets = _genJets->get();
            
            for (size_t igenJet = 0; igenJet < _genJets->size(); ++igenJet)
            {
                if (genJets[igenJet].P4().DeltaR(ljet.P4())<0.2 and std::fabs(1-genJets[igenJet].P4().Pt()/ljet.P4().Pt())<0.1)
                {
                    _ljetIsPU->get() = 1;
                }
                if (genJets[igenJet].P4().DeltaR(bjet.P4())<0.2 and std::fabs(1-genJets[igenJet].P4().Pt()/bjet.P4().Pt())<0.1)
                {
                    _bjetIsPU->get() = 1;
                }
            }
            
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


