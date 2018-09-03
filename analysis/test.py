import ROOT

ROOT.gSystem.Load("libDelphes.so")
ROOT.gSystem.Load("libstyr.so")
ROOT.gSystem.Load("plugins/libJetSelection.so")


proc = ROOT.styr.Process()
f = ROOT.TFile.Open("/media/matthias/HDD/matthias/Analysis/YR/ST_tch_14TeV_top_incl-powheg-pythia8-madspin_1000_0.root")
jetSelection = ROOT.styr.JetSelection()
proc.addModule(jetSelection)
proc.processFile(f,"Delphes")

