#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"


#include "classes/DelphesClasses.h"

namespace styr
{

class BTagging:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Particle>> _jets;
        
        styr::BranchPtr<std::vector<Particle>> _selectedLJets;
        styr::BranchPtr<std::vector<Particle>> _selectedBJets;
        
        float _minJetPt;
        float _maxJetEta;
        int _wp;
        /*
        bitpattern
        0 : loose WP
        1 : medium WP
        2 : tight WP
        3 : loose WP w/ MTD
        4 : medium WP w/ MTD
        5 : tight WP w/ MTD 
        */
    
    public:
        BTagging():
            styr::Module(""),
            _minJetPt(40.),
            _maxJetEta(4.0),
            _wp(2)
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minJetPt = config().getOrDefault<float>("minPt",_minJetPt);
            _maxJetEta = config().getOrDefault<float>("maxEta",_maxJetEta);
            _wp = config().getOrDefault<int>("wp",_wp);
        
            _jets = event.getBranch<std::vector<Particle>>(config().get<std::string>("jetSrc"));
            
            _selectedLJets = event.createBranch<std::vector<Particle>>(config().get<std::string>("output")+"L");
            _selectedBJets = event.createBranch<std::vector<Particle>>(config().get<std::string>("output")+"B");
        }
        
        bool passBTag(const Particle& jet) const
        {
            const int bitmask = (1<<_wp);
            const int btag = jet.get<int>("BTag");
            //std::cout<<bitmask<<", "<<jet.BTag<<" = "<<(btag & bitmask)<<std::endl;
            return (btag & bitmask)>0;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Particle>& jets = _jets->get();
            for (size_t i = 0; i < _jets->size(); ++i)
            {
                if (jets[i].P4().Pt()<_minJetPt) continue;
                if (std::fabs(jets[i].P4().Eta())>_maxJetEta) continue;
                if (not passBTag(jets[i]))
                {
                    _selectedLJets->get().push_back(jets[i]);
                }
                else
                {
                    _selectedBJets->get().push_back(jets[i]);
                }
            }
            return true;
        }        
        
        virtual ~BTagging()
        {
        }
};

}


