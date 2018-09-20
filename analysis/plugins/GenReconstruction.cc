#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class GenReconstruction:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<GenParticle>> _genParticles;
        
        styr::BranchPtr<float> _topMass;
        styr::BranchPtr<float> _cosThetaPL;
    
    public:
        GenReconstruction():
            styr::Module("")
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _genParticles = event.getBranch<std::vector<GenParticle>>(config().get<std::string>("genSrc"));
            
            _topMass = event.createBranch<float>(config().get<std::string>("output")+"_mass");
            
            _cosThetaPL = event.createBranch<float>(config().get<std::string>("output")+"_cosThetaPL");
            
        }
        
        bool isInDecay(const std::vector<GenParticle>& genParticles, const GenParticle& p, const GenParticle& mother)
        {
            if (p.PID==mother.PID and (std::fabs(p.P4().Pt()-mother.P4().Pt())<0.00001)) return true;
            if (p.M1>=0)
            {
                return (isInDecay(genParticles,genParticles[p.M1],mother));
            }
            return false;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            
            const std::vector<GenParticle>& genParticles = _genParticles->get();
            const GenParticle* top = nullptr;
            std::vector<const GenParticle*> leptons;
            std::vector<const GenParticle*> lquarks;
            
            for (size_t i = 0; i < _genParticles->size(); ++i)
            {
                const GenParticle& particle = genParticles[i];
                if (std::abs(particle.PID)==6 and std::abs(particle.Status)==62)
                {
                    if (top==nullptr)
                    {
                        top = &particle;
                    }
                    else
                    {
                        throw std::runtime_error("Found multiple top quarks in event");
                    }
                }
                else if (std::abs(particle.PID)>=1 and std::abs(particle.PID)<5 and std::abs(particle.Status)==23)// and particle.D1==particle.D2)
                {
                    lquarks.push_back(&particle);
                }
                else if (std::abs(particle.PID)==11 or std::abs(particle.PID)==13 or std::abs(particle.PID)==15)
                {
                    if (std::abs(genParticles[particle.M1].PID)==24)
                    {
                        leptons.push_back(&particle);
                    }
                }
                
            }
            if (top==nullptr)
            {
                throw std::runtime_error("Found no top quark in event");
            }
            
            float minDeltaPt = 1000000.;
            const GenParticle* spectatorQuark = nullptr;
            int ambi = 0;
            for (auto lquark: lquarks)
            {

                if (not (isInDecay(genParticles,*lquark,*top) or isInDecay(genParticles,*top,*lquark)))
                {
                    ambi+=1;
                    TLorentzVector vec = top->P4()-lquark->P4();
                    if (minDeltaPt>vec.Pt())
                    {
                        minDeltaPt = vec.Pt();
                        spectatorQuark = lquark;
                    }
                }
            }
            if (not spectatorQuark)
            {
               throw std::runtime_error("Found no spectator quark in event");
            }

            _topMass->get()=top->P4().M();
            
            const GenParticle* leptonFromTop = nullptr;
            
            for (auto lepton: leptons)
            {
                if (isInDecay(genParticles,*lepton,*top))
                {
                    if (leptonFromTop!=nullptr)
                    {
                        throw std::runtime_error("Found multiple leptons in event");
                    }
                    leptonFromTop = lepton;
                }
            }
            
            if (not leptonFromTop)
            {
                throw std::runtime_error("Found no leptons from top in event");
            }
            
            TLorentzVector leptonInTop = leptonFromTop->P4();
            leptonInTop.Boost(-top->P4().BoostVector());
            
            TLorentzVector lquarkInTop = spectatorQuark->P4();
            lquarkInTop.Boost(-top->P4().BoostVector());
            
            _cosThetaPL->get()=(
                (leptonInTop.Px()*lquarkInTop.Px()+leptonInTop.Py()*lquarkInTop.Py()+leptonInTop.Pz()*lquarkInTop.Pz())/
                (leptonInTop.P()*lquarkInTop.P())
            );
            
            return true;
        }        
        
        virtual ~GenReconstruction()
        {
        }
};

}


