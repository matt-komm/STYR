#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class MergeParticles:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Particle>> _p1;
        styr::ConstBranchPtr<std::vector<Particle>> _p2;
        
        styr::BranchPtr<std::vector<Particle>> _finalParticles;
    
    public:
        MergeParticles():
            styr::Module("")
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            
            _p1 = event.getBranch<std::vector<Particle>>(config().get<std::string>("p1"));
            _p2 = event.getBranch<std::vector<Particle>>(config().get<std::string>("p2"));
            
            _finalParticles = event.createBranch<std::vector<Particle>>(config().get<std::string>("output"));
        }
        
        
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Particle>& p1 = _p1->get();
            const std::vector<Particle>& p2 = _p2->get();
            
            
            for (size_t i = 0; i < _p1->size(); ++i)
            {
                _finalParticles->get().push_back(p1[i]);
            }
            for (size_t i = 0; i < _p2->size(); ++i)
            {
                _finalParticles->get().push_back(p2[i]);
            }
            return true;
        }        
        
        virtual ~MergeParticles()
        {
        }
};

}


