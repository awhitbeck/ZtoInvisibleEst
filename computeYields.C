#include "TFile.h"
#include "TH1F.h"
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

void computeYields(TString inputFiles = "~/eos/PHYS14productionV1/PHYS14.GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola_*_RA2AnalysisTree.root",
		   TString outputFile = "yields_GJets400HT600_RA2b.root",
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
  TH1F* gen = new TH1F("gen","gen",84,0.5,84.5);
  gen->SetLineWidth(2);
  TH1F* accepted = new TH1F("accepted","accepted",84,0.5,84.5);
  accepted->SetLineWidth(2);
  accepted->SetLineColor(4);
  TH1F* IDed = new TH1F("IDed","IDed",84,0.5,84.5);
  IDed->SetLineWidth(2);
  IDed->SetLineColor(4);
  IDed->SetLineStyle(2);
  TH1F* ISOed = new TH1F("ISOed","ISOed",84,0.5,84.5);
  ISOed->SetLineWidth(2);
  ISOed->SetLineColor(6);
  ISOed->SetLineStyle(1);
  TH1F* Matched = new TH1F("Matched","Matched",84,0.5,84.5);
  Matched->SetLineWidth(2);
  Matched->SetLineColor(6);
  Matched->SetLineStyle(2);

  //cout << "events: " << t->GetEntries() << endl;
  
  for( int iEv = 0 ; 
       iEv < t->GetEntries() ; 
       iEv++){

    if( iEv % 100000 == 0 ) cout << "event: " << iEv << endl;

    photons.getPhotonsFromEvent( iEv );
    
    if( photonEfficiencies ){
      if( photons.fourVec->size() == 0) continue;
      //if( photons.fourVec->at(0).Pt() < 100. ) continue;
      
      // find the 'best' photon, highest pt photon passing ID and ISO cuts
      int bestPhoton = 0 ;
      
      for( unsigned int iPh = 0 ; iPh < photons.fourVec->size() ; iPh++ ){ 
	if( photons.passID( iPh ) && photons.passISO( iPh ) ) bestPhoton = iPh ;
      }
      
      HT = recomputeHT(jets,photons.fourVec->at( bestPhoton )) ;
      MHT = recomputeMHT(jets,photons.fourVec->at( bestPhoton )) ;
      NJets = recomputeNJets(jets,photons.fourVec->at( bestPhoton )) ;
      
    }

    vector< unsigned int > passIDphotons, passIDISOphotons;
      
    int bin = computeBin( MHT, HT, NJets , BTags);

    gen->Fill( bin );

    if( photonEfficiencies ){
      //cout << "gen eta: " << photons.genPhoton.Eta() << endl;
      if( photons.passAcceptance() ){
	//cout << "accepted" << endl;
	accepted->Fill( bin );
	for( unsigned int iPh = 0 ; iPh < photons.fourVec->size() ; iPh++ ){
	  //cout << "iPh: " << iPh << "/" << photons.fourVec->size() << endl;
	  if( photons.passID( iPh ) && photons.fourVec->at( iPh ).Pt() > 100. )
	    passIDphotons.push_back( iPh );
	}
	if( passIDphotons.size() > 0 ) 
	  IDed->Fill( bin );
	for( unsigned int iPh = 0 ; iPh < passIDphotons.size() ; iPh++ ){
	  if( photons.passISO( passIDphotons[iPh] ) )
	    passIDISOphotons.push_back( passIDphotons[iPh] );
	}
	if( passIDISOphotons.size() > 0){
	  ISOed->Fill( bin );
	  if( photons.passGenMatch( passIDISOphotons[0] ) ){
	    Matched->Fill( bin );
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
    accepted->Write();
    IDed->Write();
    ISOed->Write();
    Matched->Write();
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
  MHTvector.SetPtEtaPhiM(0.,0.,0.,0.);
  //cout << "compute MHT" << endl;
  for( unsigned int iJet = 0 ; iJet < jets->size() ; iJet++ ){

    // delta-R check    
    if( sqrt( pow( jets->at( iJet ).Eta() - photon.Eta() , 2 ) + 
	      pow( jets->at( iJet ).Phi() - photon.Phi() , 2 ) 
	      ) < 0.5 ) continue;
    
    // jet kinematics
    if( jets->at( iJet).Pt() < 30. || fabs( jets->at( iJet ).Eta() ) > 5.0 ) continue;

    TLorentzVector temp;
    temp.SetPtEtaPhiM( jets->at( iJet ).Pt() , 
		       jets->at( iJet ).Eta() ,
		       jets->at( iJet ).Phi() ,
		       jets->at( iJet ).E() ) ;

    MHTvector -= temp ;
    
  }

  return MHTvector.Pt();
  
}
