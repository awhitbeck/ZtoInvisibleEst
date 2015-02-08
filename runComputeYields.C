



void runComputeYields(){

  gROOT->ProcessLine(".L computeYields.C+");
  // photon stuff...
  computeYields("~/eos/PHYS14productionV1/PHYS14.GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root","yields_GJets400HT600_RA2b.root",true);
  computeYields("~/eos/PHYS14productionV1/PHYS14.GJets_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root","yields_GJets600HTinf_RA2b.root",true);

  // Z->vv 
  computeYields("~/eos/PHYS14productionV1/PHYS14.ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root","yields_ZJetsToNuNU400HT600_RA2b.root",true);
  computeYields("~/eos/PHYS14productionV1/PHYS14.ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root","yields_ZJetsToNuNu600HTinf_RA2b.root",true);


}
