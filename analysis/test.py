import ROOT
import numpy

#ROOT.gSystem.Load("libDelphes.so")
ROOT.gSystem.Load("libstyr.so")
ROOT.gSystem.Load("plugins/libstyr-plugins.so")


proc = ROOT.styr.Process()
f = ROOT.TFile.Open("/media/matthias/HDD/matthias/Analysis/YR/ST_tch_14TeV_top_incl-powheg-pythia8-madspin_1000_0.root")

muonSelection = ROOT.styr.MuonSelection()
muonSelection.config().set("muonSrc","MuonTight").set("minPt",40.).set("maxEta",3.0).set("iso",0.05).set("output","selectedMuons")
proc.addModule(muonSelection)

electronSelection = ROOT.styr.ElectronSelection()
electronSelection.config().set("electronSrc","Electron").set("minPt",40.).set("maxEta",3.0).set("iso",0.05).set("output","selectedElectrons")
proc.addModule(electronSelection)

leptons = ROOT.styr.MergeParticles()
leptons.config().set("p1","selectedMuons").set("p2","selectedElectrons").set("output","selectedLeptons")
proc.addModule(leptons)

jetSelection = ROOT.styr.JetSelection()
jetSelection.config().set("jetSrc","JetPUPPI").set("minPt",40.).set("maxEta",4.0).set("minDR",0.4).set("muonSrc","selectedMuons").set("electronSrc","selectedElectrons").set("output","selectedJets")
proc.addModule(jetSelection)

btagging = ROOT.styr.BTagging()
btagging.config().set("jetSrc","selectedJets").set("minPt",40.).set("maxEta",4.0).set("wp",2)
btagging.config().set("outputTagged","selectedBJets").set("outputUntagged","selectedLJets")
proc.addModule(btagging)

neutrino = ROOT.styr.NeutrinoPz()
neutrino.config().set("leptonSrc","selectedLeptons").set("metSrc","PuppiMissingET").set("output","neutrino")
proc.addModule(neutrino)

topReco = ROOT.styr.TopReconstruction()
topReco.config().set("leptonSrc","selectedLeptons")
topReco.config().set("ljetSrc","selectedLJets").set("bjetSrc","selectedBJets")
topReco.config().set("neutrinoSrc","neutrino")
topReco.config().set("output","top")
proc.addModule(topReco)

genReco = ROOT.styr.GenReconstruction()
genReco.config().set("genSrc","Particle")
genReco.config().set("output","gentop")
proc.addModule(genReco)

outputModule = ROOT.styr.OutputTreeWriter()
outputModule.addBranch("selectedElectrons")
outputModule.addBranch("nvetomuons")
outputModule.addBranch("nvetoelectrons")

outputModule.addBranch("selectedLJets")
outputModule.addBranch("selectedBJets")
outputModule.addBranch("selectedMuons")

outputModule.addBranch("bWeight_nominal")
outputModule.addBranch("bWeight_bcUp")
outputModule.addBranch("bWeight_bcDown")
outputModule.addBranch("bWeight_lUp")
outputModule.addBranch("bWeight_lDown")

outputModule.addBranch("neutrino")
outputModule.addBranch("neutrino_mtw")

outputModule.addBranch("top_mass")
outputModule.addBranch("top_cosThetaPL")
outputModule.addBranch("top_ljetAbsEta")

outputModule.addBranch("gentop_mass")

proc.addModule(outputModule)

proc.processFile(f,"Delphes",20)

