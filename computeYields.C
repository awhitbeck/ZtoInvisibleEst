#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "photon.cc"
#include "TChain.h"
#include "computeBin.C" 
#include "TString.h"
#include "TLorentzVector.h"
#include <vector>
#include <iostream>

using namespace std;

double recomputeHT(vector<TLorentzVector> *jets , TLorentzVector photon );
double recomputeNJets(vector<TLorentzVector> *jets , TLorentzVector photon );
double recomputeMHT(vector<TLorentzVector> *jets , TLorentzVector photon );

void computeYields(TString inputFiles = "~/eos/PHYS14productionV1/PHYS14.GJets_HT-600toInf_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
 TString outputFile = "yields_GJets600HTinf_RA2b.root",
 bool photonEfficiencies = true 
 ){

  TChain* t = new TChain("TreeMaker2/PreSelection");
  t->Add(inputFiles);

  vector<TLorentzVector> *jets = 0;
  float HT, MHT; 
  int NJets, BTags;

  t->SetBranchAddress( "ak4JetsPt10" , &jets);
  t->SetBranchAddress( "HT" , &HT );
  t->SetBranchAddress( "MHT" , &MHT );
  t->SetBranchAddress( "NJets" , &NJets );
  t->SetBranchAddress( "BTags" , &BTags );
  
  photon photons(t);

  //TH1F* dR = new TH1F("dR","dR",100,0,1);
  TH1F* gen = new TH1F("gen","gen",72,0.5,72.5);
  gen->SetLineWidth(2);
  TH1F* accepted = new TH1F("accepted","accepted",72,0.5,72.5);
  accepted->SetLineWidth(2);
  accepted->SetLineColor(4);
  TH1F* IDed = new TH1F("IDed","IDed",72,0.5,72.5);
  IDed->SetLineWidth(2);
  IDed->SetLineColor(4);
  IDed->SetLineStyle(2);
  TH1F* ISOed = new TH1F("ISOed","ISOed",72,0.5,72.5);
  ISOed->SetLineWidth(2);
  ISOed->SetLineColor(6);
  ISOed->SetLineStyle(1);
  TH1F* Matched = new TH1F("Matched","Matched",72,0.5,72.5);
  Matched->SetLineWidth(2);
  Matched->SetLineColor(6);
  Matched->SetLineStyle(2);

  // yields versus njets
  TH2F* gen_njets = new TH2F("gen_njets","gen_njets",5,3.5,8.5,2,200,500);
  TH2F* accepted_njets = new TH2F("accepted_njets","accepted_njets",5,3.5,8.5,2,200,500);
  TH2F* IDed_njets = new TH2F("IDed_njets","IDed_njets",5,3.5,8.5,2,200,500);
  TH2F* ISOed_njets = new TH2F("ISOed_njets","ISOed_njets",5,3.5,8.5,2,200,500);
  TH2F* Matched_njets = new TH2F("Matched_njets","Matched_njets",5,3.5,8.5,2,200,500);

  // yields versus nbtags
  TH2F* gen_btags = new TH2F("gen_btags","gen_btags",3,-0.5,2.5,2,200,500);
  TH2F* accepted_btags = new TH2F("accepted_btags","accepted_btags",3,-0.5,2.5,2,200,500);
  TH2F* IDed_btags = new TH2F("IDed_btags","IDed_btags",3,-0.5,2.5,2,200,500);
  TH2F* ISOed_btags = new TH2F("ISOed_btags","ISOed_btags",3,-0.5,2.5,2,200,500);
  TH2F* Matched_btags = new TH2F("Matched_btags","Matched_btags",3,-0.5,2.5,2,200,500);

  //yields versus HT
  TH1F* gen_HT = new TH1F("gen_HT","gen_HT",20,500,2500);
  TH1F* accepted_HT = new TH1F("accepted_HT","accepted_HT",20,500,2500);
  TH1F* IDed_HT = new TH1F("IDed_HT","IDed_HT",20,500,2500);
  TH1F* ISOed_HT = new TH1F("ISOed_HT","ISOed_HT",20,500,2500);
  TH1F* Matched_HT = new TH1F("Matched_HT","Matched_HT",20,500,2500);

  //yields versus MHT
  TH1F* gen_MHT = new TH1F("gen_MHT","gen_MHT",18,200,2000);
  TH1F* accepted_MHT = new TH1F("accepted_MHT","accepted_MHT",18,200,2000);
  TH1F* IDed_MHT = new TH1F("IDed_MHT","IDed_MHT",18,200,2000);
  TH1F* ISOed_MHT = new TH1F("ISOed_MHT","ISOed_MHT",18,200,2000);
  TH1F* Matched_MHT = new TH1F("Matched_MHT","Matched_MHT",18,200,2000);

  //cout << "events: " << t->GetEntries() << endl;
  
  for( int iEv = 0 ; 
       iEv < t->GetEntries() ; 
       iEv++){

    if( iEv % 100000 == 0 ) cout << "event: " << iEv << endl;

    photons.getPhotonsFromEvent( iEv );

    // find 'best photon' recompute HT and MHT using this photon
    if( photonEfficiencies ){
      if( photons.fourVec->size() == 0) continue;
      //if( photons.fourVec->at(0).Pt() < 100. ) continue;

      // find the 'best' photon, highest pt photon passing ID and ISO cuts
      int bestPhoton = 0 ;

      for( unsigned int iPh = 0 ; iPh < photons.fourVec->size() ; iPh++ ){ 
	if( photons.passID( iPh ) && photons.passISO( iPh ) ){ 
	  bestPhoton = iPh ;
	  break;
	}
      } 

      //cout << " - - - - - - - - - - " << endl;
      //cout << "HT: " <<  HT << " MHT: " << MHT << " NJets: " << NJets << endl;

      HT = recomputeHT(jets,photons.fourVec->at( bestPhoton )) ;
      MHT = recomputeMHT(jets,photons.fourVec->at( bestPhoton )) ;
      NJets = recomputeNJets(jets,photons.fourVec->at( bestPhoton )) ;

      //photons.getGenPhoton();

      //HT = recomputeHT(jets,photons.genPhoton ) ;
      //MHT = recomputeMHT(jets,photons.genPhoton ) ;
      //NJets = recomputeNJets(jets,photons.genPhoton ) ;

      //cout << "HT: " <<  HT << " MHT: " << MHT << " NJets: " << NJets << endl;


    }

    vector< unsigned int > passIDphotons, passIDISOphotons;

    int bin = computeBin( MHT, HT, NJets , BTags, k13TeV);

    gen->Fill( bin );
    if( bin > 0 ){
      gen_njets->Fill( NJets , MHT ) ;
      gen_btags->Fill( BTags , MHT ) ;
      gen_HT->Fill( HT ) ;
      gen_MHT->Fill( MHT ) ;
    }

    if( photonEfficiencies ){
      //cout << "gen eta: " << photons.genPhoton.Eta() << endl;
      if( photons.passAcceptance() ){
	//cout << "passed acceptance cut" << endl;
	accepted->Fill( bin );
	if( bin > 0 ){
	  accepted_njets->Fill( NJets , MHT ) ;
	  accepted_btags->Fill( BTags , MHT ) ;
	  accepted_HT->Fill( HT ) ;
	  accepted_MHT->Fill( MHT ) ;
	}
	for( unsigned int iPh = 0 ; iPh < photons.fourVec->size() ; iPh++ ){
	  //cout << "iPh: " << iPh << "/" << photons.fourVec->size() << endl;
	  if( photons.passID( iPh ) && photons.fourVec->at( iPh ).Pt() > 100. )
	    passIDphotons.push_back( iPh );
	}
	if( passIDphotons.size() > 0 ){
	  IDed->Fill( bin );
	  if( bin > 0 ){
	    IDed_njets->Fill( NJets , MHT ) ;
	    IDed_btags->Fill( BTags , MHT ) ;
	    IDed_HT->Fill( HT ) ;
	    IDed_MHT->Fill( MHT ) ;
	  }
	}
	for( unsigned int iPh = 0 ; iPh < passIDphotons.size() ; iPh++ ){
	  if( photons.passISO( passIDphotons[iPh] ) )
	    passIDISOphotons.push_back( passIDphotons[iPh] );
	}
	if( passIDISOphotons.size() > 0){
	  ISOed->Fill( bin );
	  if( bin > 0 ){
	    ISOed_njets->Fill( NJets , MHT ) ;
	    ISOed_btags->Fill( BTags , MHT ) ;
	    ISOed_HT->Fill( HT ) ;
	    ISOed_MHT->Fill( MHT ) ;
	  }
	  if( photons.passGenMatch( passIDISOphotons[0] ) ){
	    Matched->Fill( bin );
	    if( bin > 0 ){
	      Matched_njets->Fill( NJets , MHT ) ;
	      Matched_btags->Fill( BTags , MHT ) ;
	      Matched_HT->Fill( HT ) ;
	      Matched_MHT->Fill( MHT ) ;
	    }
	  }
	}
      }
    }

  }

gen->Draw();
accepted->Draw();
IDed->Draw("SAME");
ISOed->Draw("SAME");
Matched->Draw("SAME");

TFile *f = new TFile(outputFile,"RECREATE");
if(f){
  gen->Write();
  gen_njets->Write();
  gen_btags->Write();
  gen_HT->Write();
  gen_MHT->Write();
  accepted->Write();
  accepted_njets->Write();
  accepted_btags->Write();
  accepted_HT->Write();
  accepted_MHT->Write();
  IDed->Write();
  IDed_njets->Write();
  IDed_btags->Write();
  IDed_HT->Write();
  IDed_MHT->Write();
  ISOed->Write();
  ISOed_njets->Write();
  ISOed_btags->Write();
  ISOed_HT->Write();
  ISOed_MHT->Write();
  Matched->Write();
  Matched_njets->Write();
  Matched_btags->Write();
  Matched_HT->Write();
  Matched_MHT->Write();

}

}

// grab jets from tree, clean photon,
// and compute HT 
//  = = = = = = = = = = = =
double recomputeHT( vector< TLorentzVector > *jets , TLorentzVector photon ){

  double HT = 0. ; 

  //cout << "compute HT" << endl;
  for( unsigned int iJet = 0 ; iJet < jets->size() ; iJet++ ){

    // delta-R check    
    if( sqrt( pow( jets->at( iJet ).Eta() - photon.Eta() , 2 ) + 
     pow( jets->at( iJet ).Phi() - photon.Phi() , 2 ) 
     ) < 0.5 ) continue;

    // jet kinematics
      if( jets->at( iJet).Pt() < 30. || fabs( jets->at( iJet ).Eta() ) > 2.5 ) continue;

    HT += jets->at( iJet ).Pt() ;

  }
  
  return HT ; 

}

// grab jets from tree, clean photon,
// and compute njets
//  = = = = = = = = = = = =
double recomputeNJets( vector< TLorentzVector > *jets , TLorentzVector photon ){

  int NJets = 0;
  //cout << "compute HT" << endl;
  for( unsigned int iJet = 0 ; iJet < jets->size() ; iJet++ ){

    // delta-R check    
    if( sqrt( pow( jets->at( iJet ).Eta() - photon.Eta() , 2 ) + 
     pow( jets->at( iJet ).Phi() - photon.Phi() , 2 ) 
     ) < 0.5 ) continue;

    // jet kinematics
      if( jets->at( iJet).Pt() < 30. || fabs( jets->at( iJet ).Eta() ) > 2.5 ) continue;

    NJets++;

  }
  
  return NJets ; 

}


// grab jets from tree, clean photon,
// and compute MHT
//  = = = = = = = = = = = =
double recomputeMHT( vector< TLorentzVector > *jets , TLorentzVector photon ){

  TLorentzVector MHTvector;
  MHTvector.SetPtEtaPhiE(0.,0.,0.,0.);
  //cout << "compute MHT" << endl;

  //cout << " - - - - - - - - dR match - - - - - - -- - " << endl;
  //cout << "photon eta: " << photon.Eta() << " photon phi: " << photon.Phi() << endl;
  for( unsigned int iJet = 0 ; iJet < jets->size() ; iJet++ ){


    //cout << "jet eta: " << jets->at( iJet ).Eta() << " jet phi: " << jets->at( iJet ).Phi() << endl;
    //cout << "dR: " << sqrt( pow( jets->at( iJet ).Eta() - photon.Eta() , 2 ) + pow( jets->at( iJet ).Phi() - photon.Phi() , 2 )    ) << endl;
    // delta-R check    
    if( sqrt( pow( jets->at( iJet ).Eta() - photon.Eta() , 2 ) + 
     pow( jets->at( iJet ).Phi() - photon.Phi() , 2 ) 
     ) < 0.5 ) continue;

    // jet kinematics
    if( jets->at( iJet).Pt() < 30. || fabs( jets->at( iJet ).Eta() ) > 5.0 ) continue;

    TLorentzVector temp;
    temp.SetPtEtaPhiE( jets->at( iJet ).Pt() , 
     jets->at( iJet ).Eta() ,
     jets->at( iJet ).Phi() ,
     jets->at( iJet ).E() ) ;

    MHTvector -= temp ;
    
  }

  return MHTvector.Pt();
  
}
