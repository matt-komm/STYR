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
        /*
        bool isInDecay(const std::vector<GenParticle>& genParticles, int maxSize, const GenParticle& p, const GenParticle& mother, int index=-1)
        {
            std::cout<<"   testing: "<<p.PID<<", "<<p.M1<<", size="<<maxSize<<", len="<<genParticles.size()<<", index="<<index<<", d="<<p.D1<<"..."<<p.D2<<std::endl;
            if (p.PID==mother.PID) return true;
            if (p.M1>0 and p.M1<maxSize and p.M1<genParticles.size() and index!=p.M1)
            {
                return (isInDecay(genParticles,maxSize,genParticles[p.M1],mother,p.M1));
            }
            return false;
        }
        */
        bool isInDecay2(const std::vector<GenParticle>& genParticles, int maxSize, const GenParticle& p, const GenParticle& mother, int index=-1)
        {
            std::cout<<"   testing: "<<p.PID<<", "<<p.M1<<", size="<<maxSize<<", len="<<genParticles.size()<<", index="<<index<<", d="<<p.D1<<"..."<<p.D2<<std::endl;
            if (mother.PID==p.PID) return true;
            if (mother.D1>=0)
            {
                if (isInDecay2(genParticles,maxSize,p,genParticles[mother.D1],mother.D1))
                {
                    return true;
                }
                /*
                for (int i = mother.D1; i < (mother.D2+1); ++i)
                {
                    if (isInDecay2(genParticles,maxSize,p,genParticles[i],i))
                    {
                        return true;
                    }
                }*/
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
                else if (std::abs(particle.PID)>=1 and std::abs(particle.PID)<5)
                {
                    lquarks.push_back(&particle);
                }
            }
            if (top==nullptr)
            {
                throw std::runtime_error("Found no top quark in event");
            }
            /*
            for (auto lquark: lquarks)
            {
                std::cout<<lquark->PID<<": "<<lquark->Status<<", ";
                std::cout<<isInDecay2(genParticles,_genParticles->size(),*lquark,*top)<<std::endl;
            }*/
            /*
            float minPt = 1000000;
            for (
            */
            _topMass->get()=top->P4().M();
            return true;
        }        
        
        virtual ~GenReconstruction()
        {
        }
};

}


