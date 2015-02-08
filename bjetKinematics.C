#include <algorithm>
#include "TCanvas.h"
#include "TH1F.h"
#include "TChain.h"
#include "photon.cc"
#include <vector>

bool ptOrdered( TLorentzVector i , TLorentzVector j ){  return i.Pt() < j.Pt() ; }

void bjetKinematics(){

  TChain* z400 = new TChain("RA2slim");
  z400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/ZJetsToNuNu_HT-400toInf*slim.root");
  TChain* z600 = new TChain("RA2slim");
  z600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/ZJetsToNuNu_HT-600toInf*slim.root");


  TChain* g400 = new TChain("RA2slim");
  g400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/GJets_HT-400toInf*slim.root");
  TChain* g600 = new TChain("RA2slim");
  g600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/GJets_HT-600toInf*slim.root");

  TH1F* dR = new TH1F("dR",";#sqrt{ #Delta#phi^{2} + #Delta#theta^{2}};Events",40,0,3.1415);
  TH1F* Nb = new TH1F("Nb",";b-quarks;Events",6,-0.5,5.5);
  TH1F* bPt1 = new TH1F("bPt1",";b-quark p_{T} [GeV];Events",40,0,800);
  TH1F* bPt2 = new TH1F("bPt2",";b-quark p_{T} [GeV];Events",40,0,800);
  TH1F* bPt3 = new TH1F("bPt3",";b-quark p_{T} [GeV];Events",40,0,800);
  TH1F* bPt4 = new TH1F("bPt4",";b-quark p_{T} [GeV];Events",40,0,800);

  TH1F* ZgamR_MHT = new TH1F("ZgamR_MHT","ZgamR_MHT",200,1000);

  photon ph[2] = { photon(g400) , photon(g600) };
  double gWeight[2] = { 0.075604015773398 , 0.0261385093009007 };

  for( int s = 0 ; s < 2 ; s++ ){
    for( unsigned int i = 0 ; i < ph[s].t->GetEntries() ; i++){
      
      ph[s].getPhotonsFromEvent( i );
      
      vector<TLorentzVector> bjets;
      
      for( unsigned int p = 0 ; p < ph[s].genParticles->size() ; p++){

	if( abs(ph[s].genParPDGid->at(p)) == 5 ){ 
	
	  bjets.push_back(ph[s].genParticles->at(p));
  
	}// check if gen particle is a b-quark

      }// loop over gen particles

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

