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


        virtual bool analyze(styr::Event&) override
        {
            
            const std::vector<GenParticle>& genParticles = _genParticles->get();
            const GenParticle* top = nullptr;
            std::vector<GenParticle*> leptons;
            std::vector<GenParticle*> lquarks;
            
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
            }
            if (top==nullptr)
            {
                throw std::runtime_error("Found no top quark in event");
            }
            _topMass->get()=top->P4().M();
            return true;
        }        
        
        virtual ~GenReconstruction()
        {
        }
};

}


