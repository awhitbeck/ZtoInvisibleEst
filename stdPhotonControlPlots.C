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

vector<TH1F*> stdPhotonControlPlots(TString fileName="rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola*.root", int color=kAzure-9){
	
	bool debug = false;
	bool cutFlow = true;

	if( debug )	cout << "getting root file" << endl;

	TChain* t = new TChain("TreeFiller/AnalysisTree");
	t->Add(fileName);
	if( debug )	cout << "got tree: " << t->GetEntries() << " events" << endl;

	photon myPhotons(t);
	if( debug ) cout << "done with photon constructor" << endl;

	// set branches for jet information
	double HT, MHT;
	double rho;
	int NJets;

	/*
	t->SetBranchAddress("HT_patJetsAK5PFPt50Eta25" , &HT );
	t->SetBranchAddress("MHT_patJetsAK5PFPt30" , &MHT );
	t->SetBranchAddress("NJets_patJetsAK5PFPt50Eta25" , &NJets );
	t->SetBranchAddress("rho" , &rho );	
	*/
	
	t->SetBranchAddress("HT_HTJets" , &HT );
	t->SetBranchAddress("MHT_MHTJets" , &MHT );
	t->SetBranchAddress("NJets_HTJets" , &NJets );
	t->SetBranchAddress("fixedGridRhoFastjetAll" , &rho );	
	
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
	
	for( int i = 0 ; i < t->GetEntries() ; i++){
		myPhotons.getPhotonsFromEvent(i);

		if( i % 100000 == 0 ) cout << "event: " << i << endl;

		// hack to use cone isolation instead of pf isolation
		// comment these out when using miniAOD
		// - - - - - - - - - -
		//myPhotons.pfChargedIso = myPhotons.ConeDR03trkPtIso ;
		//myPhotons.pfNeutralIso = myPhotons.ConeDR03hcalEtIso ;
		//myPhotons.pfGammaIso   = myPhotons.ConeDR03ecalEtIso ;
		// - - - - - - - - - - 

		if( debug ) cout << "got photons" << endl;
		
		//if( myPhotons.isEB->size() == 0 ) continue;
		int j = 0;
		//cout << "barrel selection" << endl;
		//cout << "isEB size: " << myPhotons.isEB->size() << endl;
		while( myPhotons.isEB->size() > 0 && j < myPhotons.isEB->size() ){
			//cout << "j: " << j << " isEB size: " << myPhotons.isEB->size() << endl;
			if( myPhotons.isEB->at(j) > 1. && cutFlow ){
				//cout << "failed cut" << endl;
				myPhotons.deletePhoton(j);
				//cout << "sanity isEB size: " << myPhotons.isEB->size() << endl;
			}
			else 
				j++;
		}
		if( myPhotons.pt->size() > 0 )
			leadingPhotonPt->Fill( myPhotons.pt->at(0) );

		j = 0;
		//cout << "pt selection" << endl;
		while( myPhotons.pt->size() > 0 && j < myPhotons.pt->size() ){
			if( myPhotons.pt->at(j) < 100. && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		if( myPhotons.eta->size() > 0 )
			leadingPhotonEta->Fill( myPhotons.eta->at(0) );

		j = 0;
		//cout << "eta selection" << endl;
		while( myPhotons.eta->size() > 0 && j < myPhotons.eta->size() ){
			if( fabs( myPhotons.eta->at(j) ) > 2.4 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		if( myPhotons.hasPixelSeed->size() > 0 )
			leadingPhotonPixSeed->Fill( myPhotons.hasPixelSeed->at(0) );

		j = 0;
		//cout << "pixel seed selection" << endl;
		while( myPhotons.hasPixelSeed->size() > 0 && j < myPhotons.hasPixelSeed->size() ){
			if( myPhotons.hasPixelSeed->at(j) == 1 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		if( myPhotons.hadTowOverEm->size() > 0 )
			leadingPhotonHoverE->Fill( myPhotons.hadTowOverEm->at(0) ) ;

		j = 0;
		//cout << "H/E selection" << endl;
		while( myPhotons.hadTowOverEm->size() > 0 && j < myPhotons.hadTowOverEm->size()){
			if( myPhotons.hadTowOverEm->at(j) > 0.05 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		if( myPhotons.sigmaIetaIeta->size() > 0 )
			leadingPhotonShowerShape->Fill( myPhotons.sigmaIetaIeta->at(0) );

		j = 0;
		//cout << "sigmaIetaIeta selection" << endl;
		while( myPhotons.sigmaIetaIeta->size() > 0 && j < myPhotons.sigmaIetaIeta->size() ){
			if( myPhotons.sigmaIetaIeta->at(j) > 0.011 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		//cout << "flush" << endl;
		if( myPhotons.pfChargedIso->size() > 0 ){
			//cout << "flush" << endl;
			leadingPhotonPFchIso->Fill( myPhotons.effAreas->rhoCorrectedIso( pfCh , myPhotons.pfChargedIso->at(0) , myPhotons.eta->at(0) , rho ) ); 	
		}
		// NOTE THE ISOLATION CUTS ARE NOT PT DEPENDENT!!!!!
		j = 0;
		//cout << "charged ISO selection" << endl;
		while( myPhotons.pfChargedIso->size() > 0 && j < myPhotons.pfChargedIso->size() ){
			if( myPhotons.effAreas->rhoCorrectedIso( pfCh , myPhotons.pfChargedIso->at(j) , myPhotons.eta->at(j) , rho ) > 0.7 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		if( myPhotons.pfNeutralIso->size() > 0)	
			leadingPhotonPFnuIso->Fill( myPhotons.effAreas->rhoCorrectedIso( pfNu , myPhotons.pfNeutralIso->at(0) , myPhotons.eta->at(0) , rho ) );

		j = 0;
		//cout << "neutral ISO selection" << endl;
		while( myPhotons.pfNeutralIso->size() > 0 && j < myPhotons.pfNeutralIso->size() ){
			if( myPhotons.effAreas->rhoCorrectedIso( pfNu , myPhotons.pfNeutralIso->at(j) , myPhotons.eta->at(j) , rho ) > 0.4 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		if( myPhotons.pfGammaIso->size() > 0)	
			leadingPhotonPFgaIso->Fill( myPhotons.effAreas->rhoCorrectedIso( pfGam , myPhotons.pfGammaIso->at(0) , myPhotons.eta->at(0) , rho ) );

		j = 0;
		//cout << "gamma ISO selection" << endl;
		while( myPhotons.pfGammaIso->size() > 0 && j < myPhotons.pfGammaIso->size() ){
			if( myPhotons.effAreas->rhoCorrectedIso( pfGam , myPhotons.pfGammaIso->at(j) , myPhotons.eta->at(j) , rho ) > 0.5 && cutFlow )
				myPhotons.deletePhoton(j);
			else
				j++;
		}

		if( myPhotons.pfGammaIso->size() > 0){	

			leadingPhotonGenMatch->Fill( (bool)myPhotons.isGenMatched->at(0) );

			if( NJets < 3 || HT < 500. ||  MHT < 200. ) continue; 
			NJetshisto->Fill( NJets );
			HThisto->Fill( HT );
			MHThisto->Fill( MHT );

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

	return histos;

}