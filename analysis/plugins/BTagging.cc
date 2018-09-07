#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class BTagging:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Jet>> _jets;
        
        styr::BranchPtr<std::vector<Jet>> _selectedLJets;
        styr::BranchPtr<std::vector<Jet>> _selectedBJets;
        
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
        
            _jets = event.getBranch<std::vector<Jet>>(config().get<std::string>("jetSrc"));
            
            _selectedLJets = event.createBranch<std::vector<Jet>>(config().get<std::string>("output")+"L");
            _selectedBJets = event.createBranch<std::vector<Jet>>(config().get<std::string>("output")+"B");
        }
        
        bool passBTag(const Jet& jet) const
        {
            const int bitmask = (1<<_wp);
            std::cout<<bitmask<<", "<<jet.BTag<<" = "<<(jet.BTag & bitmask)<<std::endl;
            return (jet.BTag & bitmask)>0;
        }
        
        virtual bool analyze(styr::Event&) override
        {
            const std::vector<Jet>& jets = _jets->get();
            for (size_t i = 0; i < _jets->size(); ++i)
            {
                if (jets[i].PT<_minJetPt) continue;
                if (std::fabs(jets[i].Eta)>_maxJetEta) continue;
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


