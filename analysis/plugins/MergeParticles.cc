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
        
        styr::BranchPtr<float> _leptonPt;
        styr::BranchPtr<float> _leptonEta;
    
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
            
            _leptonPt = event.createBranch<float>(config().get<std::string>("output")+"_pt");
            _leptonEta = event.createBranch<float>(config().get<std::string>("output")+"_eta");
        }
        
        
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Particle>& p1 = _p1->get();
            const std::vector<Particle>& p2 = _p2->get();
            
            std::vector<Particle> merged;
            for (size_t i = 0; i < _p1->size(); ++i)
            {
                merged.push_back(p1[i]);
            }
            for (size_t i = 0; i < _p2->size(); ++i)
            {
                merged.push_back(p2[i]);
            }
            //sort descending
            std::sort(merged.begin(),merged.end(),[](const Particle& p1, const Particle& p2)
            {
                return p1.P4().Pt()>p2.P4().Pt();
            });
            
            if (merged.size()>=1)
            {
                _leptonPt->get() = merged.front().P4().Pt();
                _leptonEta->get() = merged.front().P4().Eta();
            }
            else
            {
                _leptonPt->get() = -10;
                _leptonEta->get() = -10;
            }
            
            _finalParticles->get() = std::move(merged);
            return true;
        }        
        
        virtual ~MergeParticles()
        {
        }
};

}


