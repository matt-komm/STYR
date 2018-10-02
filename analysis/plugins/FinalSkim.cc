#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"

#include "classes/DelphesClasses.h"

namespace styr
{

class FinalSkim:
    public styr::Module
{
    protected:
        std::vector<styr::ConstBranchPtr<float>> _topMasses;
        
    
    public:
        FinalSkim():
            styr::Module("")
        {
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            for (auto s: {"nominal","jecUp","jecDown","unclUp","unclDown"})
            {
                _topMasses.push_back(event.getBranch<float>("recotop_"+std::string(s)+"_topMass"));
            }
        }
        
       
        
        virtual bool analyze(styr::Event&, bool pass) override
        {
            bool isNull = true;
            for (auto& massBranch: _topMasses)
            {
                //std::cout<<massBranch->get()<<",";
                isNull = isNull and massBranch->get()<0;
            }
            //std::cout<<"="<<isNull<<std::endl;
            return not isNull;
        }        
        
        
        virtual ~FinalSkim()
        {
        }
};

}


