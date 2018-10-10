#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

#include "NeutrinoPzSolver.hpp"

namespace styr
{

class NeutrinoPz:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Particle>> _leptons;
        styr::ConstBranchPtr<std::vector<Particle>> _met;
        
        styr::BranchPtr<Particle> _output;
        styr::BranchPtr<float> _mtw;
        styr::BranchPtr<float> _metScalar;
        

    public:
        NeutrinoPz():
            styr::Module("")
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _leptons = event.getBranch<std::vector<Particle>>(config().get<std::string>("leptonSrc"));
            _met = event.getBranch<std::vector<Particle>>(config().get<std::string>("metSrc"));
            _output = event.createBranch<Particle>(config().get<std::string>("output"));
            _mtw = event.createBranch<float>(config().get<std::string>("output")+"_mtw");
            _metScalar = event.createBranch<float>(config().get<std::string>("output")+"_met");
        }
        
        
        virtual bool analyze(styr::Event&, bool) override
        {
            const std::vector<Particle>& leptons = _leptons->get();
            const Particle& met = _met->get()[0];
            _metScalar->get()=met.P4().Pt();
            if (_leptons->size()==0)
            {
                _output->get()=styr::Particle();
                _mtw->get()=-10;
                return false;
            }
            auto solution = NuMomentum(
                leptons[0].P4().Px(), leptons[0].P4().Py(), leptons[0].P4().Pz(), 
                leptons[0].P4().Pt(), leptons[0].P4().E(), met.P4().Px(), met.P4().Py() 
            ).first;
            
            Particle p(solution);
            _output->get()=p;
            
            _mtw->get()=std::sqrt(
                std::pow(leptons[0].P4().Pt()+met.P4().Pt(),2)
                -std::pow(leptons[0].P4().Px()+met.P4().Px(),2)
                -std::pow(leptons[0].P4().Py()+met.P4().Py(),2)
            );
            _output->get()=met;
            
            return true;
        }        
        
        virtual ~NeutrinoPz()
        {
        }
};

}


