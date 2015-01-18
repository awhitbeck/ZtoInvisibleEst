#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>
#include "TCanvas.h"
#include "TPad.h"

using namespace std;

bool debug = false ;

vector<TH1F*> Zmass(TString fileName = "rootFiles/13TeV_50ns40PU.DYJetsToLL_M-50_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_SumJetMass_AnalysisTree.root",
					int color = kAzure-9
					){

	bool useMuons = true;

	//TFile* f = new TFile(fileName,"READ");
	//TTree* t = (TTree*) f->Get("TreeFiller/AnalysisTree");

	TChain* t = new TChain("TreeFiller/AnalysisTree");
	t->Add(fileName);

	cout << "got tree" << endl;

	vector<double> *pt = 0 ;
	vector<double> *eta = 0 ;
	vector<double> *phi = 0 ;
	vector<double> *e = 0 ;
	vector<int> *id = 0 ;
	vector<double> *iso = 0 ;
	double HT,MHT;
	int NJets;

	//cout << "initialized vectors" << endl;
	if( useMuons ){
		t->SetBranchAddress( "muonPt" , &pt );
		t->SetBranchAddress( "muonEta" , &eta );
		t->SetBranchAddress( "muonPhi" , &phi );
		t->SetBranchAddress( "muonE" , &e );
		t->SetBranchAddress( "muonPassID" , & id );
		t->SetBranchAddress( "muonRelIso" , & iso );
	}else{
		t->SetBranchAddress( "electronPt" , &pt );
		t->SetBranchAddress( "electronEta" , &eta );
		t->SetBranchAddress( "electronPhi" , &phi );
		t->SetBranchAddress( "electronE" , &e );
	}
	t->SetBranchAddress( "MHT_MHTJets" , &MHT );
	t->SetBranchAddress( "HT_HTJets" , &HT );	
	t->SetBranchAddress( "NJets_HTJets" , &NJets );	

	//cout << "set branches" << endl;

	TH1F* zmass = new TH1F("zmass",";m_{ll} [GeV];Events (a.u.)",40,0,120);
	zmass->SetFillColor( color ) ;
	zmass->SetLineColor( 1 ) ;
	zmass->SetLineWidth( 2 ) ;
	TH1F* zpt = new TH1F("zpt",";p_{T,ll} [GeV];Events (a.u.)",40,0,400);
	zpt->SetFillColor( color ) ;
	zpt->SetLineColor( 1 ) ;
	zpt->SetLineWidth( 2 ) ;
	TH1F* HThisto = new TH1F("HThisto",";H_{T} [GeV];Events (a.u.)",25,500,3000);
	HThisto->SetFillColor( color ) ;
	HThisto->SetLineColor( 1 ) ;
	HThisto->SetLineWidth( 2 ) ;
	TH1F* MHThisto = new TH1F("MHThisto",";missing H_{T} [GeV];Events (a.u.)",40,200,1000);
	MHThisto->SetFillColor( color ) ;
	MHThisto->SetLineColor( 1 ) ;
	MHThisto->SetLineWidth( 2 ) ;
	TH1F* NJetshisto = new TH1F("NJetshisto",";n_{jets};Events (a.u.)",10,0,10);
	NJetshisto->SetFillColor( color ) ;
	NJetshisto->SetLineColor( 1 ) ;
	NJetshisto->SetLineWidth( 2 ) ;

	TH2F* MHTvHT = new TH2F("MHTvHT",";MHT [GeV];HT [GeV]",100,100,500,100,300,2000);
	//cout << "starting for loop" << endl;

	for( int iEvt = 0 ; iEvt < t->GetEntries() ; iEvt++ ){

		t->GetEntry( iEvt ) ;

		if( debug ) cout << "iEvt: " << iEvt << endl;

		if( pt->size() < 2 ) continue;
		if( pt->at(0) < 20 ) continue;
		if( pt->at(1) < 20 ) continue;

		if( debug ){
			cout << "leading muon --- " << endl;
			cout << "pt: " << (*pt)[0] << " eta: " << eta->at(0) << " phi: " << phi->at(0) << " E: " << e->at(0) << endl;
			cout << "---------------- " << endl;
			cout << "subleading muon - " << endl;
			cout << "pt: " << (*pt)[1] << " eta: " << eta->at(1) << " phi: " << phi->at(1) << " E: " << e->at(1) << endl;
			cout << "----------------- " << endl;
		}

		vector<TLorentzVector*> lepton;
		TLorentzVector zboson;

		for( unsigned int i = 0 ; i < pt->size() ; i++ ){

			// only true for muons!						
			if( (*iso)[i] < 0.2 ){ //&& (*id)[i] == 1 ){
				lepton.push_back( new TLorentzVector() ) ; 
				lepton.back()->SetPtEtaPhiE( (*pt)[i], (*eta)[i], (*phi)[i], (*e)[i]);				
			}

		}// end loop over leptons			

		cout << "num of good muons: " << lepton.size() << endl;

		if( lepton.size() < 2 ) continue;

		zboson = *(lepton[0]) + *(lepton[1]);
			
		if( debug ){
			cout << "z boson ---- " << endl;
			cout << "pt: " << zboson.Pt() << " eta: " << zboson.Eta() << " phi: " << zboson.Phi() << " e: " << zboson.E() << endl;
			cout << "zboson mass: " << zboson.M() << endl;
			cout << " - - - - - - - - - - - - - - - - - " << endl;
		}

		zmass->Fill( zboson.M() ) ;
		zpt->Fill( zboson.Pt() ) ; 
	
		if( zboson.M() < 111. && zboson.M() > 71 && MHT > 200. && HT > 500. && NJets >= 3 ){
			HThisto->Fill( HT ) ;
			MHThisto->Fill( MHT ) ;
			NJetshisto->Fill( NJets ) ;
		}
		if( zboson.M() < 111. && zboson.M() > 71 && NJets >= 3 ){
			MHTvHT->Fill( MHT , HT ) ;
		}

	}	

	TCanvas* can = new TCanvas("can","can",1200,800);
	can->Divide(3,2);
	can->cd(1);
	zmass->Draw();
	can->cd(2);
	zpt->Draw();
	can->cd(3);
	MHTvHT->Draw("colz");
	can->cd(4);
	NJetshisto->Draw();
	can->cd(5);
	HThisto->Draw();
	can->cd(6);
	gPad->SetLogy();
	MHThisto->Draw();


	vector<TH1F*> histos;
	histos.push_back(zmass);
	histos.push_back(zpt);
	histos.push_back(NJetshisto);
	histos.push_back(HThisto);
	histos.push_back(MHThisto);

	return histos;
}