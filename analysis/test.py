import ROOT
import numpy

#ROOT.gSystem.Load("libDelphes.so")
ROOT.gSystem.Load("libstyr.so")
ROOT.gSystem.Load("plugins/libstyr-plugins.so")


proc = ROOT.styr.Process()
f = ROOT.TFile.Open("/media/matthias/HDD/matthias/Analysis/YR/ST_tch_14TeV_top_incl-powheg-pythia8-madspin_1000_0.root")

muonSelection = ROOT.styr.MuonSelection()
muonSelection.config().set("muonSrc","MuonTight").set("minPt",50.).set("maxEta",3.0).set("iso",0.1).set("output","selectedMuons")
proc.addModule(muonSelection)

electronSelection = ROOT.styr.ElectronSelection()
electronSelection.config().set("electronSrc","Electron").set("minPt",50.).set("maxEta",3.0).set("iso",0.1).set("output","selectedElectrons")
proc.addModule(electronSelection)

jetSelection = ROOT.styr.JetSelection()
jetSelection.config().set("jetSrc","JetPUPPI").set("minPt",40.).set("maxEta",4.0).set("minDR",0.4).set("muonSrc","selectedMuons").set("electronSrc","selectedElectrons").set("output","selectedJets")
proc.addModule(jetSelection)

btagging = ROOT.styr.BTagging()
btagging.config().set("jetSrc","selectedJets").set("minPt",40.).set("maxEta",4.0).set("wp",2).set("output","tagged")
proc.addModule(btagging)

neutrino = ROOT.styr.NeutrinoPz()
neutrino.config().set("leptonSrc","selectedMuons").set("metSrc","PuppiMissingET").set("output","neutrino")
proc.addModule(neutrino)

outputModule = ROOT.styr.OutputTreeWriter()
outputModule.addBranch("taggedL")
outputModule.addBranch("taggedB")
outputModule.addBranch("selectedMuons")
outputModule.addBranch("selectedMuons")
outputModule.addBranch("neutrino")
outputModule.addBranch("neutrino_mtw")
proc.addModule(outputModule)

proc.processFile(f,"Delphes")

