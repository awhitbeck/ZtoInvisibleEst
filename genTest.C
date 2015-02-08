#include <algorithm>
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "photon.cc"
#include <vector>

bool ptOrdered( TLorentzVector i , TLorentzVector j ){  return i.Pt() < j.Pt() ; }

void genTest(){

  TChain* z400 = new TChain("TreeMaker2/PreSelection");
  z400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.ZJetsToNuNu_HT-400to600*.root");
  TChain* z600 = new TChain("TreeMaker2/PreSelection");
  z600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.ZJetsToNuNu_HT-600toInf*.root");

  TChain* g400 = new TChain("TreeMaker2/PreSelection");
  g400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.GJets_HT-400to600*.root");
  TChain* g600 = new TChain("TreeMaker2/PreSelection");
  g600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.GJets_HT-600toInf*.root");

  TH1F* dR = new TH1F("dR",";#sqrt{ #Delta#phi^{2} + #Delta#theta^{2}};Events",40,0,3.1415);
  TH1F* Nb = new TH1F("Nb",";b-quarks;Events",6,-0.5,5.5);
  TH1F* bPt1 = new TH1F("bPt1",";b-quark p_{T} [GeV];Events",40,0,800);
  TH1F* bPt2 = new TH1F("bPt2",";b-quark p_{T} [GeV];Events",40,0,800);
  TH1F* bPt3 = new TH1F("bPt3",";b-quark p_{T} [GeV];Events",40,0,800);
  TH1F* bPt4 = new TH1F("bPt4",";b-quark p_{T} [GeV];Events",40,0,800);
    
  TH1F* ZgamR_MHT = new TH1F("ZgamR_MHT",";H_{T}^{miss};Z/#gamma Ratio",100,0,1000);
  TH1F* MHT_Z = new TH1F("MHT_Z",";H_{T}^{miss} [GeV];Events",100,0,1000);
  TH1F* MHT_g = new TH1F("MHT_g",";H_{T}^{miss} [GeV];Events",100,0,1000);

  TH1F* ZgamR_HT = new TH1F("ZgamR_HT",";H_{T};Z/#gamma Ratio",100,0,2000);
  TH1F* HT_Z = new TH1F("HT_Z",";H_{T} [GeV];Events",100,0,2000);
  TH1F* HT_g = new TH1F("HT_g",";H_{T} [GeV];Events",100,0,2000);

  photon ph[4] = { photon(g400) , photon(g600) , photon(z400) , photon(z600) } ;
  //double gWeight[4] = { 0.075604015773398 , 0.0261385093009007 , 0.0144160859953796 , 0.00489467814129328 };

  double gWeight[4] = { 0.6205 , 0.2087 , 0.1199 , 0.04113 } ;

  for( int s = 0 ; s < 4 ; s++ ){

    cout << "s: " << s << endl;

    for( unsigned int i = 0 ; i < 500000 /*ph[s].t->GetEntries()*/ ; i++){

      if(  i % 100000 == 0 ) cout << "event: " << i << endl;
      
      ph[s].getPhotonsFromEvent( i );
      
      vector<TLorentzVector> bjets;

      TLorentzVector MHTvec( 0. , 0. , 0. , 0. ) ;
      double HT = 0. ;

      for( unsigned int p = 0 ; p < ph[s].genParticles->size() ; p++){

	if( abs(ph[s].genParPDGid->at(p)) == 5 ){ 
	
	  bjets.push_back(ph[s].genParticles->at(p));
  
	}// check if gen particle is a b-quark

	if( abs(ph[s].genParPDGid->at(p)) < 6 ){
	  
	  MHTvec -= ph[s].genParticles->at(p) ;
	  HT += ph[s].genParticles->at(p).Pt() ;
	  
	}
	
      }// loop over gen particles

      if( s <= 1 ){
	MHT_g->Fill( MHTvec.Pt() , gWeight[s] ) ;
	HT_g->Fill( HT , gWeight[s] ) ;
      }else{
	MHT_Z->Fill( MHTvec.Pt() , gWeight[s] ) ;
	HT_Z->Fill( HT , gWeight[s] ) ;
      }

      Nb->Fill( bjets.size() , gWeight[s] );
      sort( bjets.begin() , bjets.end() , ptOrdered ) ;
      if( bjets.size() > 0 ) 
	bPt1->Fill( bjets[0].Pt() , gWeight[s] );
      if( bjets.size() > 1 ) 
	bPt2->Fill( bjets[1].Pt() , gWeight[s] );
      if( bjets.size() > 2 ) 
	bPt3->Fill( bjets[2].Pt() , gWeight[s] );
      if( bjets.size() > 3 ) 
	bPt4->Fill( bjets[3].Pt() , gWeight[s] );
      if( bjets.size() >=2 ){
	dR->Fill( sqrt( pow( bjets[0].Phi() - bjets[1].Phi() , 2 ) 
			+ pow( bjets[0].Theta() - bjets[1].Theta() , 2 ) 
			) , gWeight[s] );
      }

    }// loop over events

  }// loop over samples

  TCanvas* can_ZgamR_MHT = new TCanvas("can_ZgamR_MHT","can_ZgamR_MHT",500,500);
  TPad* htpad = new TPad("htpad","htpad",0,0.3,1,1);
  TPad* htratiopad = new TPad("htratiopad","htratiopad",0,0,1,0.3);
  htpad->SetBottomMargin(0);
  htratiopad->SetTopMargin(0);
  htratiopad->SetBottomMargin(.3);
  htpad->Draw();
  htratiopad->Draw();
  htpad->cd();
  
  MHT_g->SetLineColor(4);
  MHT_g->SetLineWidth(2);
  MHT_g->SetLineStyle(1);
  MHT_Z->SetLineColor(6);
  MHT_Z->SetLineWidth(2);
  MHT_Z->SetLineStyle(2);
  MHT_g->Draw();
  MHT_Z->Draw("SAME");

  ZgamR_MHT->Divide( MHT_Z , MHT_g , 1. , 1. ) ;
  htratiopad->cd();
  ZgamR_MHT->SetMarkerStyle(8);
  ZgamR_MHT->Draw();

  TCanvas* can_ZgamR_HT = new TCanvas("can_ZgamR_HT","can_ZgamR_HT",500,500);
  TPad* htpadHT = new TPad("htpadHT","htpadHT",0,0.3,1,1);
  TPad* htratiopadHT= new TPad("htratiopadHT","htratiopadHT",0,0,1,0.3);
  htpadHT->SetBottomMargin(0);
  htratiopadHT->SetTopMargin(0);
  htratiopadHT->SetBottomMargin(.3);
  htpadHT->Draw();
  htratiopadHT->Draw();
  htpadHT->cd();
  
  HT_g->SetLineColor(4);
  HT_g->SetLineWidth(2);
  HT_g->SetLineStyle(1);
  HT_Z->SetLineColor(6);
  HT_Z->SetLineWidth(2);
  HT_Z->SetLineStyle(2);
  HT_g->Draw();
  HT_Z->Draw("SAME");

  ZgamR_HT->Divide( HT_Z , HT_g , 1. , 1. ) ;
  htratiopadHT->cd();
  ZgamR_HT->SetMarkerStyle(8);
  ZgamR_HT->Draw();

  TCanvas* can_dR = new TCanvas("can_dR","can_dR",500,500);
  dR->Draw();
  TCanvas* can_Nb = new TCanvas("can_Nb","can_Nb",500,500);
  Nb->Draw();

  TCanvas* can_bPt = new TCanvas("can_bPt","can_bPt",500,500);
  bPt1->SetLineWidth(2);
  bPt1->SetLineColor(1);
  bPt1->SetLineStyle(1);
  bPt1->DrawNormalized();
  bPt2->SetLineWidth(2);
  bPt2->SetLineColor(2);
  bPt2->SetLineStyle(1);
  bPt2->DrawNormalized("SAME");
  bPt3->SetLineWidth(2);
  bPt3->SetLineColor(4);
  bPt3->SetLineStyle(2);
  bPt3->DrawNormalized("SAME");
  bPt4->SetLineWidth(2);
  bPt4->SetLineColor(6);
  bPt4->SetLineStyle(2);
  bPt4->DrawNormalized("SAME");
 
}

