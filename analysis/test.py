import ROOT
import numpy

ROOT.gSystem.Load("libDelphes.so")
ROOT.gSystem.Load("libstyr.so")
ROOT.gSystem.Load("plugins/libJetSelection.so")


proc = ROOT.styr.Process()
f = ROOT.TFile.Open("/media/matthias/HDD/matthias/Analysis/YR/ST_tch_14TeV_top_incl-powheg-pythia8-madspin_1000_0.root")
jetSelection = ROOT.styr.JetSelection()
jetSelection.config().set("jetSrc","Jet").set("minPt",40.).set("maxEta",4.0).set("minDR",0.4).set("output","selectedJets")
proc.addModule(jetSelection)
proc.processFile(f,"Delphes")

