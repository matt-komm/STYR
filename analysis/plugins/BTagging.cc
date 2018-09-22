#include "styr/Event.h"
#include "styr/Branch.h"
#include "styr/Module.h"
#include "styr/Particle.h"


#include "classes/DelphesClasses.h"

#include "BTagWeightCalculator.hpp"

namespace styr
{

class BTagging:
    public styr::Module
{
    protected:
        styr::ConstBranchPtr<std::vector<Particle>> _jets;
        
        styr::BranchPtr<std::vector<Particle>> _selectedLJets;
        styr::BranchPtr<std::vector<Particle>> _selectedBJets;
        
        styr::BranchPtr<float> _bWeight_nominal;
        styr::BranchPtr<float> _bWeight_bcUp;
        styr::BranchPtr<float> _bWeight_bcDown;
        styr::BranchPtr<float> _bWeight_lUp;
        styr::BranchPtr<float> _bWeight_lDown;
        
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
        
        BWGHT::BTagWeightCalculator _weightCalculator;
    
    public:
        BTagging():
            styr::Module(""),
            _minJetPt(40.),
            _maxJetEta(4.0),
            _wp(2)
        {
            BWGHT::WorkingPoint bwp(0.5); //need to provide jets as 1. tagged/0. untagged
            
            //taken from https://github.com/delphes/delphes/blob/master/cards/CMS_PhaseII/CMS_PhaseII_200PU_v03.tcl#L3125
            bwp.setEfficiencyFunction(new BWGHT::LambdaEfficiencyFunction([](const BWGHT::Jet& jet, BWGHT::SYS::TYPE) -> double
            {
                const float abseta = std::fabs(jet.eta);
                const float pt = jet.pt;
                if (std::abs(jet.flavor)==5)
                {
                    return (abseta > 0.00 && abseta <= 1.50) * (pt > 20.00 && pt <= 30.00) * (0.449) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 30.00 && pt <= 40.00) * (0.515) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 40.00 && pt <= 50.00) * (0.554) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 50.00 && pt <= 60.00) * (0.575) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 60.00 && pt <= 70.00) * (0.584) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 70.00 && pt <= 80.00) * (0.590) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 80.00 && pt <= 90.00) * (0.591) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 90.00 && pt <= 100.00) * (0.590) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 100.00 && pt <= 120.00) * (0.585) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 120.00 && pt <= 140.00) * (0.567) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 140.00 && pt <= 160.00) * (0.560) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 160.00 && pt <= 180.00) * (0.542) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 180.00 && pt <= 200.00) * (0.530) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 200.00 && pt <= 250.00) * (0.497) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 250.00 && pt <= 300.00) * (0.452) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 300.00 && pt <= 350.00) * (0.413) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 350.00 && pt <= 400.00) * (0.381) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 400.00 && pt <= 500.00) * (0.333) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 500.00 && pt <= 600.00) * (0.308) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 600.00 && pt <= 700.00) * (0.246) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 700.00 && pt <= 800.00) * (0.233) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 800.00 && pt <= 1000.00) * (0.219) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 1000.00 && pt <= 1400.00) * (0.192) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 1400.00 && pt <= 2000.00) * (0.166) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 2000.00 && pt <= 3000.00) * (0.166) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 3000.00) * (0.166) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 20.00 && pt <= 30.00) * (0.308) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 30.00 && pt <= 40.00) * (0.385) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 40.00 && pt <= 50.00) * (0.437) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 50.00 && pt <= 60.00) * (0.463) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 60.00 && pt <= 70.00) * (0.479) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 70.00 && pt <= 80.00) * (0.472) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 80.00 && pt <= 90.00) * (0.465) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 90.00 && pt <= 100.00) * (0.474) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 100.00 && pt <= 120.00) * (0.470) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 120.00 && pt <= 140.00) * (0.457) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 140.00 && pt <= 160.00) * (0.432) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 160.00 && pt <= 180.00) * (0.423) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 180.00 && pt <= 200.00) * (0.408) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 200.00 && pt <= 250.00) * (0.385) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 250.00 && pt <= 300.00) * (0.345) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 300.00 && pt <= 350.00) * (0.306) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 350.00 && pt <= 400.00) * (0.278) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 400.00 && pt <= 500.00) * (0.253) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 500.00 && pt <= 600.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 600.00 && pt <= 700.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 700.00 && pt <= 800.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 800.00 && pt <= 1000.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 1000.00 && pt <= 1400.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 1400.00 && pt <= 2000.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 2000.00 && pt <= 3000.00) * (0.206) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 3000.00) * (0.206) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 20.00 && pt <= 30.00) * (0.190) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 30.00 && pt <= 40.00) * (0.246) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 40.00 && pt <= 50.00) * (0.280) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 50.00 && pt <= 60.00) * (0.296) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 60.00 && pt <= 70.00) * (0.309) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 70.00 && pt <= 80.00) * (0.304) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 80.00 && pt <= 90.00) * (0.304) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 90.00 && pt <= 100.00) * (0.296) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 100.00 && pt <= 120.00) * (0.289) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 120.00 && pt <= 140.00) * (0.284) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 140.00 && pt <= 160.00) * (0.265) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 160.00 && pt <= 180.00) * (0.269) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 180.00 && pt <= 200.00) * (0.272) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 200.00 && pt <= 250.00) * (0.249) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 250.00 && pt <= 300.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 300.00 && pt <= 350.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 350.00 && pt <= 400.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 400.00 && pt <= 500.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 500.00 && pt <= 600.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 600.00 && pt <= 700.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 700.00 && pt <= 800.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 800.00 && pt <= 1000.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 1000.00 && pt <= 1400.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 1400.00 && pt <= 2000.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 2000.00 && pt <= 3000.00) * (0.214) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 3000.00) * (0.214);
                }
                if (std::abs(jet.flavor)==4)
                {
                    return (abseta > 0.00 && abseta <= 1.50) * (pt > 20.00 && pt <= 30.00) * (0.023) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 30.00 && pt <= 40.00) * (0.025) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 40.00 && pt <= 50.00) * (0.029) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 50.00 && pt <= 60.00) * (0.031) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 60.00 && pt <= 70.00) * (0.034) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 70.00 && pt <= 80.00) * (0.035) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 80.00 && pt <= 90.00) * (0.036) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 90.00 && pt <= 100.00) * (0.038) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 100.00 && pt <= 120.00) * (0.039) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 120.00 && pt <= 140.00) * (0.039) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 140.00 && pt <= 160.00) * (0.042) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 160.00 && pt <= 180.00) * (0.042) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 180.00 && pt <= 200.00) * (0.045) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 200.00 && pt <= 250.00) * (0.040) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 250.00 && pt <= 300.00) * (0.038) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 300.00 && pt <= 350.00) * (0.034) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 350.00 && pt <= 400.00) * (0.031) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 400.00 && pt <= 500.00) * (0.026) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 500.00 && pt <= 600.00) * (0.025) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 600.00 && pt <= 700.00) * (0.019) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 700.00 && pt <= 800.00) * (0.017) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 800.00 && pt <= 1000.00) * (0.018) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 1000.00 && pt <= 1400.00) * (0.016) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 1400.00 && pt <= 2000.00) * (0.015) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 2000.00 && pt <= 3000.00) * (0.015) +
                    (abseta > 0.00 && abseta <= 1.50) * (pt > 3000.00) * (0.015) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 20.00 && pt <= 30.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 30.00 && pt <= 40.00) * (0.018) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 40.00 && pt <= 50.00) * (0.023) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 50.00 && pt <= 60.00) * (0.026) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 60.00 && pt <= 70.00) * (0.029) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 70.00 && pt <= 80.00) * (0.028) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 80.00 && pt <= 90.00) * (0.026) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 90.00 && pt <= 100.00) * (0.032) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 100.00 && pt <= 120.00) * (0.032) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 120.00 && pt <= 140.00) * (0.033) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 140.00 && pt <= 160.00) * (0.034) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 160.00 && pt <= 180.00) * (0.036) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 180.00 && pt <= 200.00) * (0.035) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 200.00 && pt <= 250.00) * (0.035) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 250.00 && pt <= 300.00) * (0.032) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 300.00 && pt <= 350.00) * (0.025) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 350.00 && pt <= 400.00) * (0.027) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 400.00 && pt <= 500.00) * (0.020) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 500.00 && pt <= 600.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 600.00 && pt <= 700.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 700.00 && pt <= 800.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 800.00 && pt <= 1000.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 1000.00 && pt <= 1400.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 1400.00 && pt <= 2000.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 2000.00 && pt <= 3000.00) * (0.017) +
                    (abseta > 1.50 && abseta <= 2.50) * (pt > 3000.00) * (0.017) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 20.00 && pt <= 30.00) * (0.018) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 30.00 && pt <= 40.00) * (0.017) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 40.00 && pt <= 50.00) * (0.018) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 50.00 && pt <= 60.00) * (0.018) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 60.00 && pt <= 70.00) * (0.020) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 70.00 && pt <= 80.00) * (0.020) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 80.00 && pt <= 90.00) * (0.019) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 90.00 && pt <= 100.00) * (0.020) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 100.00 && pt <= 120.00) * (0.019) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 120.00 && pt <= 140.00) * (0.018) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 140.00 && pt <= 160.00) * (0.021) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 160.00 && pt <= 180.00) * (0.016) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 180.00 && pt <= 200.00) * (0.022) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 200.00 && pt <= 250.00) * (0.023) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 250.00 && pt <= 300.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 300.00 && pt <= 350.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 350.00 && pt <= 400.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 400.00 && pt <= 500.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 500.00 && pt <= 600.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 600.00 && pt <= 700.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 700.00 && pt <= 800.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 800.00 && pt <= 1000.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 1000.00 && pt <= 1400.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 1400.00 && pt <= 2000.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 2000.00 && pt <= 3000.00) * (0.014) +
                    (abseta > 2.50 && abseta <= 3.50) * (pt > 3000.00) * (0.014);
                }
                return 0.001;
            }));
            
            //taken from https://twiki.cern.ch/twiki/pub/CMS/YR2018Systematics/btag_syst_Phase2.txt
            bwp.setScaleFactorFunction(new BWGHT::LambdaScaleFactorFunction([](const BWGHT::Jet& jet, BWGHT::SYS::TYPE sys) -> double
            {
                    
                const float pt = jet.pt;
                const float unc_b = (pt > 20. && pt <= 30.)*0.018 +
                    (pt > 30. && pt <= 50.)*0.01 +
                    (pt > 50. && pt <= 70.)*0.01 +
                    (pt > 70. && pt <= 100.)*0.01 +
                    (pt > 100. && pt <= 140.)*0.01 +
                    (pt > 140. && pt <= 200.)*0.01 +
                    (pt > 200. && pt <= 300.)*0.016 +
                    (pt > 300. && pt <= 600.)*0.018 +
                    (pt > 600. && pt <= 1000.)*0.029 +
                    (pt > 1000. && pt <= 3000.)*0.058 +
                    (pt > 3000.)*0.058*2.;
                const float unc_c = (pt > 20. && pt <= 30.)*0.064 +
                    (pt > 30. && pt <= 50.)*0.029 +
                    (pt > 50. && pt <= 70.)*0.025 +
                    (pt > 70. && pt <= 100.)*0.027 +
                    (pt > 100. && pt <= 140.)*0.028 +
                    (pt > 140. && pt <= 200.)*0.033 +
                    (pt > 200. && pt <= 300.)*0.053 +
                    (pt > 300. && pt <= 600.)*0.064 +
                    (pt > 600. && pt <= 1000.)*0.102 +
                    (pt > 1000. && pt <= 3000.)*0.205 +
                    (pt > 3000.)*0.205*2.;
                const float unc_l = (pt > 20. && pt <= 30.)*0.15 +
                    (pt > 30. && pt <= 50.)*0.15 +
                    (pt > 50. && pt <= 70.)*0.15 +
                    (pt > 70. && pt <= 100.)*0.15 +
                    (pt > 100. && pt <= 140.)*0.15 +
                    (pt > 140. && pt <= 200.)*0.15 +
                    (pt > 200. && pt <= 300.)*0.15 +
                    (pt > 300. && pt <= 600.)*0.15 +
                    (pt > 600. && pt <= 1000.)*0.15 +
                    (pt > 1000. && pt <= 3000.)*0.15 +
                    (pt > 3000.)*0.15*2.;
                    
                if (std::abs(jet.flavor)==5)
                {
                    if (sys==BWGHT::SYS::BC_UP)
                    {
                        return 1.+unc_b;
                    }
                    else if (sys==BWGHT::SYS::BC_DOWN)
                    {
                        return 1.-unc_b;
                    }
                    return 1.;
                }
                if (std::abs(jet.flavor)==4)
                {
                    if (sys==BWGHT::SYS::BC_UP)
                    {
                        return 1.+unc_c;
                    }
                    else if (sys==BWGHT::SYS::BC_DOWN)
                    {
                        return 1.-unc_c;
                    }
                    return 1.;
                }
                if (sys==BWGHT::SYS::L_UP)
                {
                    return 1.+unc_l;
                }
                else if (sys==BWGHT::SYS::L_DOWN)
                {
                    return 1.-unc_l;
                }
                return 1.;
            }));
            _weightCalculator.addWorkingPoint(bwp);
        }
        
        virtual void beginFile(const TFile*,styr::Event& event) override
        {
            _minJetPt = config().getOrDefault<float>("minPt",_minJetPt);
            _maxJetEta = config().getOrDefault<float>("maxEta",_maxJetEta);
            _wp = config().getOrDefault<int>("wp",_wp);
        
            _jets = event.getBranch<std::vector<Particle>>(config().get<std::string>("jetSrc"));
            
            _selectedLJets = event.createBranch<std::vector<Particle>>(config().get<std::string>("output")+"_untagged");
            _selectedBJets = event.createBranch<std::vector<Particle>>(config().get<std::string>("output")+"_tagged");
            
            _bWeight_nominal = event.createBranch<float>(config().get<std::string>("output")+"_bWeight_nominal");
            _bWeight_bcUp = event.createBranch<float>(config().get<std::string>("output")+"_bWeight_bcUp");
            _bWeight_bcDown = event.createBranch<float>(config().get<std::string>("output")+"_bWeight_bcDown");
            _bWeight_lUp = event.createBranch<float>(config().get<std::string>("output")+"_bWeight_lUp");
            _bWeight_lDown = event.createBranch<float>(config().get<std::string>("output")+"_bWeight_lDown");
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
            std::vector<BWGHT::Jet> jetsForBtagging;
            for (size_t i = 0; i < _jets->size(); ++i)
            {
                if (jets[i].P4().Pt()<_minJetPt) continue;
                if (std::fabs(jets[i].P4().Eta())>_maxJetEta) continue;
                if (not passBTag(jets[i]))
                {
                    _selectedLJets->get().push_back(jets[i]);
                    jetsForBtagging.emplace_back(
                        0.0, //double discriminatorValue=0, 
                        jets[i].get<int>("flavor"), //unsigned int flavor = 0, 
                        jets[i].P4().Pt(), //double pt = 0.0, 
                        jets[i].P4().Eta() //double eta = 0.0
                    );
                }
                else
                {
                    _selectedBJets->get().push_back(jets[i]);
                    jetsForBtagging.emplace_back(
                        1.0, //double discriminatorValue=0, 
                        jets[i].get<int>("flavor"), //unsigned int flavor = 0, 
                        jets[i].P4().Pt(), //double pt = 0.0, 
                        jets[i].P4().Eta() //double eta = 0.0
                    );
                }
            }
            _bWeight_nominal->get() = _weightCalculator.getEventWeight(jetsForBtagging,BWGHT::SYS::NOMINAL);
            _bWeight_bcUp->get() = _weightCalculator.getEventWeight(jetsForBtagging,BWGHT::SYS::BC_UP);
            _bWeight_bcDown->get() = _weightCalculator.getEventWeight(jetsForBtagging,BWGHT::SYS::BC_DOWN);
            _bWeight_lUp->get() = _weightCalculator.getEventWeight(jetsForBtagging,BWGHT::SYS::L_UP);
            _bWeight_lDown->get() = _weightCalculator.getEventWeight(jetsForBtagging,BWGHT::SYS::L_DOWN);
                
            return true;
        }        
        
        virtual ~BTagging()
        {
        }
};

}


