#include "THStack.h"
#include "TChain.h"
#include "TString.h"
#include "TDirectory.h"
#include "TH1F.h"
#include <map>
#include <iostream>

using namespace std;

enum workSys { kLPC , kMacBook , kNumSys };
enum sample  { kGjets400 , kGjets600 , kZinv400 , kZinv600 , kNumSamples };

TString tag[kNumSamples] = {"GJets_HT-400to600",
			    "GJets_HT-600toInf",
			    "ZJetsToNuNu_HT-400to600",
			    "ZJetsToNuNu_HT-600toInf"};

double weights[kNumSamples] = {0.0756,
			       0.0261,
			       0.0144,
			       0.00489
                              };

class analysis{

public:

  TChain* tree[kNumSamples];
  workSys sys;

  // the constructor take workSys as an argument, this is meant
  // to take advantage of xrootd functionality when working 
  // on cmslpc nodes... not yet implemented, though.
  analysis(workSys sys_ = kMacBook, TString inputDir = "./" );

  // method for plotting simple variables from root trees 
  // using the TTree::Draw method

  // the hope is that eventually, this will spit out
  // the gjets and zinv histograms via a map which is 
  // indexed by a string, i.e. "gjets" or "zinv"
  //map<char*,TH1F*> plot(TString var = "HTnoPhotons",
  TH1F* plot(TString var = "HTnoPhotons",
	     TString binning = "(100,500,1500)",
	     TString cuts = "" );

  THStack* CSVvsFlavor(int nbtags = 0);

};
