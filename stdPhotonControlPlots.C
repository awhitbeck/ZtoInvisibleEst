#include <iostream>
#include "TH1F.h"
#include "TTree.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include <vector>
#include <utility>
#include <cmath>
#include "TString.h"
#include "photon.cc"

using namespace std;

vector<TH1F*> stdPhotonControlPlots(TString fileName="/uscms/home/awhitbe1/eos/PHYS14productionV1/PHYS14.GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root", 
				    TString tag="GJets_400HT600",
				    double weight = 1. ,
				    int color=kAzure-9
				    ){
	
  bool debug = false;
  bool cutFlow = true;

  cout << "fileName: " << fileName << endl;
  cout << "tag: " << tag << endl;
  cout << "color: " << color << endl;

  if( debug )	cout << "getting root file" << endl;

  TChain* t = new TChain("TreeMaker2/PreSelection");
  t->Add(fileName);
  cout << "got tree: " << t->GetEntries() << " events" << endl;

  photon myPhotons(t);
  if( debug ) cout << "done with photon constructor" << endl;

  // set branches for jet information
  float HT, MHT;
  double rho;
  int NJets;

  t->SetBranchAddress("HT" , &HT );
  t->SetBranchAddress("MHT" , &MHT );
  t->SetBranchAddress("NJets" , &NJets );
  //t->SetBranchAddress("fixedGridRhoFastjetAll" , &rho );	
	
  // initialize histograms	
  TH1F* leadingPhotonPt = new TH1F("leadingPhotonPt",";p_{T} [GeV];Events (a.u.)",40,0,400);
  leadingPhotonPt->SetLineWidth(2);
  leadingPhotonPt->SetLineColor(1);
  leadingPhotonPt->SetFillColor(color);
  TH1F* leadingPhotonEta = new TH1F("leadingPhotonEta",";#eta;Events (a.u.)",20,-2.5,2.5);
  leadingPhotonEta->SetLineWidth(2);
  leadingPhotonEta->SetLineColor(1);
  leadingPhotonEta->SetFillColor(color);
  TH1F* leadingPhotonPixSeed = new TH1F("leadingPhotonPixSeed",";Pixel Seed;Events (a.u.)",2,0,2);
  leadingPhotonPixSeed->SetLineWidth(2);
  leadingPhotonPixSeed->SetLineColor(1);
  leadingPhotonPixSeed->SetFillColor(color);
  TH1F* leadingPhotonHoverE = new TH1F("leadingPhotonHoverE",";H/E;Events (a.u.)",60,0,.15);
  leadingPhotonHoverE->SetLineWidth(2);
  leadingPhotonHoverE->SetLineColor(1);
  leadingPhotonHoverE->SetFillColor(color);
  TH1F* leadingPhotonShowerShape = new TH1F("leadingPhotonShowerShape",";#sigma_{I#etaI#eta};Events (a.u.)",60,0,0.06);
  leadingPhotonShowerShape->SetLineWidth(2);
  leadingPhotonShowerShape->SetLineColor(1);
  leadingPhotonShowerShape->SetFillColor(color);
  TH1F* leadingPhotonPFchIso = new TH1F("leadingPhotonPFchIso",";Iso_{pfCh.}^{R03} [GeV];Events (a.u.);Iso_{pfCh.}^{R03} [GeV]",20,0,2);
  leadingPhotonPFchIso->SetLineWidth(2);
  leadingPhotonPFchIso->SetLineColor(1);
  leadingPhotonPFchIso->SetFillColor(color);
  TH1F* leadingPhotonPFnuIso = new TH1F("leadingPhotonPFnuIso",";Iso_{pfNu.}^{R03} [GeV];Events (a.u.)",20,0,14);
  leadingPhotonPFnuIso->SetLineWidth(2);
  leadingPhotonPFnuIso->SetLineColor(1);
  leadingPhotonPFnuIso->SetFillColor(color);
  TH1F* leadingPhotonPFgaIso = new TH1F("leadingPhotonPFgaIso",";Iso_{pfGa.}^{R03} [GeV];Events (a.u.)",20,0,10);
  leadingPhotonPFgaIso->SetLineWidth(2);
  leadingPhotonPFgaIso->SetLineColor(1);
  leadingPhotonPFgaIso->SetFillColor(color);
  TH1F* leadingPhotonGenMatch = new TH1F("leadingPhotonGenMatch",";Matched To Gen-Photon;Events (a.u.)",2,0,2);
  leadingPhotonGenMatch->SetLineWidth(2);
  leadingPhotonGenMatch->SetLineColor(1);
  leadingPhotonGenMatch->SetFillColor(color);
  TH1F* HThisto = new TH1F("HThisto",";H_{T} [GeV];Events (a.u.)",25,500,3000);
  HThisto->SetLineWidth(2);
  HThisto->SetLineColor(1);
  HThisto->SetFillColor(color);
  TH1F* MHThisto = new TH1F("MHThisto",";missing H_{T} [GeV];Events (a.u.)",40,20,1000);
  MHThisto->SetLineWidth(2);
  MHThisto->SetLineColor(1);
  MHThisto->SetFillColor(color);
  TH1F* NJetshisto = new TH1F("NJetshisto",";N_{jets};Events (a.u.)",10,-.5,10.5);
  NJetshisto->SetLineWidth(2);
  NJetshisto->SetLineColor(1);
  NJetshisto->SetFillColor(color);

  if( debug ) cout << " initialized all histograms" << endl;
	
  for( int i = 0 ; 
       i < 10000 ; //t->GetEntries() ; 
       i++){

    t->GetEntry(i);
    myPhotons.getPhotonsFromEvent(i);

    if( i % 100000 == 0 ) cout << "event: " << i << endl;

    if( debug ) cout << "got photons" << endl;
		
    unsigned int bestPhoton = 99999 ;

    for( unsigned int iPh = 0 ; iPh < myPhotons.fourVec->size() ; iPh++ ){

      /*if( myPhotons.fourVec->at(iPh).Pt() > 100. 
	  && fabs( myPhotons.fourVec->at(iPh).Eta() ) < 2.4
	  && myPhotons.hasPixelSeed->at(iPh) == 0
	  && myPhotons.hadTowOverEM->at(iPh) < 0.05
	  && myPhotons.sigmaIetaIeta->at(iPh) < 0.011 
	  && myPhotons.pfChargedIsoRhoCorr->at(iPh) < 0.7
	  && myPhotons.pfNeutralIsoRhoCorr->at(iPh) < 0.4
	  && myPhotons.pfGammaIsoRhoCorr->at(iPh) < 0.5 )
	  {*/
      if( myPhotons.isGenMatched->at(iPh) == 1 ){
	
	bestPhoton = iPh ;
	break ; 
	
      }

    }

    if( bestPhoton == 99999 ) continue; // no photons that passed all of the ID and isolation requirements

    if( fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	&& myPhotons.hasPixelSeed->at(bestPhoton) == 0
	&& myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	&& myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011
	&& myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	&& myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	&& myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonPt->Fill( myPhotons.fourVec->at(bestPhoton).Pt() , weight );
      
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	  && myPhotons.hasPixelSeed->at(bestPhoton) == 0
	  && myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	  && myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	  && myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	  && myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	  && myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonEta->Fill( myPhotons.fourVec->at(bestPhoton).Eta() , weight );
      
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	  && fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	  && myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	  && myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	  && myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	  && myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	  && myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonPixSeed->Fill( myPhotons.hasPixelSeed->at(bestPhoton) , weight );
      
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	  && fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	  && myPhotons.hasPixelSeed->at(bestPhoton) == 0
	  && myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	  && myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	  && myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	  && myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonHoverE->Fill( myPhotons.hadTowOverEM->at(bestPhoton) , weight ) ;
      
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	  && fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	  && myPhotons.hasPixelSeed->at(bestPhoton) == 0
	  && myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	  && myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	  && myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	  && myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonShowerShape->Fill( myPhotons.sigmaIetaIeta->at(bestPhoton) , weight );
      
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	&& fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	&& myPhotons.hasPixelSeed->at(bestPhoton) == 0
	&& myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	&& myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	&& myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	&& myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonPFchIso->Fill( myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) , weight );
    
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	  && fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	  && myPhotons.hasPixelSeed->at(bestPhoton) == 0
	  && myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	  && myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	  && myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	  && myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonPFnuIso->Fill( myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) , weight );
    
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	 && fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	 && myPhotons.hasPixelSeed->at(bestPhoton) == 0
	 && myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	 && myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	 && myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	 && myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4 )
      leadingPhotonPFgaIso->Fill( myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) , weight );

    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	&& fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	&& myPhotons.hasPixelSeed->at(bestPhoton) == 0
	&& myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	&& myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	&& myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	&& myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	&& myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 )
      leadingPhotonGenMatch->Fill( (bool)myPhotons.isGenMatched->at(bestPhoton) , weight );
      
    //cout << "NJets: " << NJets << " HT: " << HT << " MHT: " << MHT << endl;
    
    if( myPhotons.fourVec->at(bestPhoton).Pt() > 100. 
	&& fabs( myPhotons.fourVec->at(bestPhoton).Eta() ) < 2.4
	&& myPhotons.hasPixelSeed->at(bestPhoton) == 0
	&& myPhotons.hadTowOverEM->at(bestPhoton) < 0.05
	&& myPhotons.sigmaIetaIeta->at(bestPhoton) < 0.011 
	&& myPhotons.pfChargedIsoRhoCorr->at(bestPhoton) < 0.7
	&& myPhotons.pfNeutralIsoRhoCorr->at(bestPhoton) < 0.4
	&& myPhotons.pfGammaIsoRhoCorr->at(bestPhoton) < 0.5 	
	){

      if( HT > 500. &&  MHT > 200. )
	NJetshisto->Fill( NJets , weight );
      if( NJets > 3 &&  MHT > 200.) 
	HThisto->Fill( HT , weight );
      if( NJets > 3 && HT > 500. )
	MHThisto->Fill( MHT , weight );
     
    }
    

  }

  TCanvas* can = new TCanvas("can","can",1200,900);
  can->Divide(4,3);
  can->cd(1);
  leadingPhotonPt->Draw();
  can->cd(2);
  leadingPhotonEta->Draw();
  can->cd(3);
  leadingPhotonPixSeed->Draw();
  can->cd(4);
  leadingPhotonHoverE->Draw();
  can->cd(5);
  leadingPhotonShowerShape->Draw();
  can->cd(6);
  leadingPhotonPFchIso->Draw();
  can->cd(7);
  leadingPhotonPFnuIso->Draw();
  can->cd(8);
  leadingPhotonPFgaIso->Draw();
  can->cd(9);
  leadingPhotonGenMatch->Draw();
  can->cd(10);
  NJetshisto->Draw();	
  can->cd(11);
  HThisto->Draw();
  can->cd(12);
  MHThisto->Draw();

  TH1F* cutFlowHisto = new TH1F("cutFlow","Cut Flow",11,0,11);
  cutFlowHisto->SetBinContent( 1  , leadingPhotonPt->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(1,"No Cuts");
  cutFlowHisto->SetBinContent( 2  , leadingPhotonEta->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(2,"p_{T}>100.");
  cutFlowHisto->SetBinContent( 3  , leadingPhotonPixSeed->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(3,"|#eta|<2.4");
  cutFlowHisto->SetBinContent( 4  , leadingPhotonHoverE->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(4,"No Pixel Seeds");
  cutFlowHisto->SetBinContent( 5  , leadingPhotonShowerShape->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(5,"H/E Cut");
  cutFlowHisto->SetBinContent( 6  , leadingPhotonPFchIso->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(6,"#sigma_{I#etaI#eta} Cut");
  cutFlowHisto->SetBinContent( 7  , leadingPhotonPFnuIso->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(7,"Iso_{pfCh}^{R03} Cut");
  cutFlowHisto->SetBinContent( 8  , leadingPhotonPFgaIso->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(8,"Iso_{pfNu}^{R03} Cut");
  cutFlowHisto->SetBinContent( 9  , NJetshisto->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(9,"Iso_{pfGa}^{R03} Cut");
  cutFlowHisto->SetBinContent( 10 , HThisto->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(10,"N_{jets}>2");
  cutFlowHisto->SetBinContent( 11 , MHThisto->Integral() ) ; ///leadingPhotonPt->Integral() );
  cutFlowHisto->GetXaxis()->SetBinLabel(11,"H_{T}>500.");
  cutFlowHisto->SetLineColor(color);
  cutFlowHisto->SetLineWidth(2);

  TCanvas* can_cutFlow = new TCanvas("can_cutFlow","Cut Flow",500,500);
  cutFlowHisto->Draw();

  TFile* outputFile = new TFile("photonControlPlots_"+tag+".root","RECREATE");

  vector<TH1F*> histos;
  histos.push_back(leadingPhotonPt);
  histos.push_back(leadingPhotonEta);
  histos.push_back(leadingPhotonPixSeed);
  histos.push_back(leadingPhotonHoverE);
  histos.push_back(leadingPhotonShowerShape);
  histos.push_back(leadingPhotonPFchIso);
  histos.push_back(leadingPhotonPFnuIso);
  histos.push_back(leadingPhotonPFgaIso);
  histos.push_back(leadingPhotonGenMatch);
  histos.push_back(NJetshisto);
  histos.push_back(HThisto);
  histos.push_back(MHThisto);
  histos.push_back(cutFlowHisto);

  for( unsigned int iHisto = 0 ; iHisto < histos.size() ; iHisto++ ){
    histos[iHisto]->Write();
  }

  outputFile->Close();

  return histos;

}
