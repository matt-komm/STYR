import ROOT
import numpy

#ROOT.gSystem.Load("libDelphes.so")
ROOT.gSystem.Load("libstyr.so")
ROOT.gSystem.Load("plugins/libstyr-plugins.so")


proc = ROOT.styr.Process()
'''
genReco = ROOT.styr.GenReconstruction()
genReco.config().set("genSrc","Particle")
genReco.config().set("output","gentop")
proc.addModule(genReco)
'''
muonSelection = ROOT.styr.MuonSelection()
muonSelection.config().set("muonSrc","MuonTight").set("minPt",40.).set("maxEta",3.0).set("iso",0.05).set("output","selectedMuons")
proc.addModule(muonSelection)

electronSelection = ROOT.styr.ElectronSelection()
electronSelection.config().set("electronSrc","Electron").set("minPt",40.).set("maxEta",3.0).set("iso",0.05).set("output","selectedElectrons")
proc.addModule(electronSelection)

leptons = ROOT.styr.MergeParticles()
leptons.config().set("p1","selectedMuons").set("p2","selectedElectrons").set("output","selectedLeptons")
proc.addModule(leptons)

jetMetUnc = ROOT.styr.JetMetUncertainty()
jetMetUnc.config().set("jetSrc","JetPUPPI").set("metSrc","PuppiMissingET").set("output","jetmet")
proc.addModule(jetMetUnc)


for unc,jet,met in [
    ("nominal","jetmet_jetNominal","jetmet_metNominal"),
    ("jecUp","jetmet_jetJecUp","jetmet_metJecUp"),
    ("jecDown","jetmet_jetJecDown","jetmet_metJecDown"),
    ("unclUp","jetmet_jetNominal","jetmet_metUnclUp"),
    ("unclDown","jetmet_jetNominal","jetmet_metUnclDown"),
]:
    jetSelection = ROOT.styr.JetSelection()
    jetSelection.config().set("jetSrc",jet).set("minPt",40.).set("maxEta",4.0).set("minDR",0.4).set("muonSrc","selectedMuons").set("electronSrc","selectedElectrons")
    jetSelection.config().set("output","selectedJets_"+unc)
    proc.addModule(jetSelection)

    btagging = ROOT.styr.BTagging()
    btagging.config().set("jetSrc","selectedJets_"+unc).set("minPt",40.).set("maxEta",4.0).set("wp",2)
    btagging.config().set("output","btight_"+unc)
    proc.addModule(btagging)

    neutrino = ROOT.styr.NeutrinoPz()
    neutrino.config().set("leptonSrc","selectedLeptons").set("metSrc",met).set("output","neutrino_"+unc)
    proc.addModule(neutrino)

    topReco = ROOT.styr.TopReconstruction()
    topReco.config().set("leptonSrc","selectedLeptons")
    topReco.config().set("ljetSrc","btight_"+unc+"_untagged").set("bjetSrc","btight_"+unc+"_tagged")
    topReco.config().set("neutrinoSrc","neutrino_"+unc)
    topReco.config().set("output","recotop_"+unc)
    proc.addModule(topReco)

    

outputModule = ROOT.styr.OutputTreeWriter()
outputModule.addBranch("selectedElectrons")
outputModule.addBranch("selectedMuons")
outputModule.addBranch("nvetomuons")
outputModule.addBranch("nvetoelectrons")
outputModule.addBranch("selectedLeptons_pt")
outputModule.addBranch("selectedLeptons_eta")
outputModule.addBranch("selectedLeptons_PID")

outputModule.addBranch("btight_nominal_bWeight_nominal")
outputModule.addBranch("btight_nominal_bWeight_bcUp")
outputModule.addBranch("btight_nominal_bWeight_bcDown")
outputModule.addBranch("btight_nominal_bWeight_lUp")
outputModule.addBranch("btight_nominal_bWeight_lDown")
'''
outputModule.addBranch("gentop_topMass")
outputModule.addBranch("gentop_topPt")
outputModule.addBranch("gentop_topY")
outputModule.addBranch("gentop_cosThetaPL")
outputModule.addBranch("gentop_leptonPID")
'''
for unc,jet,met in [
    ("nominal","jetmet_jetNominal","jetmet_metNominal"),
    ("jecUp","jetmet_jetJecUp","jetmet_metJecUp"),
    ("jecDown","jetmet_jetJecDown","jetmet_metJecDown"),
    ("unclUp","jetmet_jetNominal","jetmet_metUnclUp"),
    ("unclDown","jetmet_jetNominal","jetmet_metUnclDown"),
]:
    outputModule.addBranch("selectedJets_"+unc)
    outputModule.addBranch("btight_"+unc+"_untagged")
    outputModule.addBranch("btight_"+unc+"_tagged")

    outputModule.addBranch("neutrino_"+unc+"_met")
    outputModule.addBranch("neutrino_"+unc+"_mtw")
    
    outputModule.addBranch("recotop_"+unc+"_topMass")
    outputModule.addBranch("recotop_"+unc+"_topPt")
    outputModule.addBranch("recotop_"+unc+"_topY")

    outputModule.addBranch("recotop_"+unc+"_cosThetaPL")
    outputModule.addBranch("recotop_"+unc+"_ljetPt")
    outputModule.addBranch("recotop_"+unc+"_ljetEta")
    


proc.addModule(outputModule)

for f in [
    "root://eoscms.cern.ch//store/group/upgrade/delphes_output/YR_Delphes/Delphes342pre15/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8_200PU/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8_1000_0.root",
    "root://eoscms.cern.ch//store/group/upgrade/delphes_output/YR_Delphes/Delphes342pre15/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8_200PU/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8_1000_1.root",
    "root://eoscms.cern.ch//store/group/upgrade/delphes_output/YR_Delphes/Delphes342pre15/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8_200PU/TT_TuneCUETP8M2T4_14TeV-powheg-pythia8_1001_0.root",

   #"root://cmseos.fnal.gov//store/user/snowmass/noreplica/YR_Delphes/Delphes342pre15/W3JetsToLNu_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_200PU/W3JetsToLNu_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_1000_0.root",
   #"root://cmseos.fnal.gov//store/user/snowmass/noreplica/YR_Delphes/Delphes342pre15/W3JetsToLNu_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_200PU/W3JetsToLNu_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_1001_0.root",
   #"root://cmseos.fnal.gov//store/user/snowmass/noreplica/YR_Delphes/Delphes342pre15/W3JetsToLNu_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_200PU/W3JetsToLNu_TuneCUETP8M1_14TeV-madgraphMLM-pythia8_1002_0.root",
]:
    rootFile = ROOT.TFile.Open(f)
    if not rootFile:
        print "skipping",f
    else:
        proc.processFile(rootFile,"Delphes",-1,True)
    rootFile.Close()
