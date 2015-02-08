#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include <vector>
#include <iostream>
#include <map>

using namespace std;

TH1F* divideHistos(TH1F* num , TH1F* denom, char options="");
TH1F* multiplyHistos(TH1F* num , TH1F* denom, char options="");
TH1F* getGJetsYields(TString tag);

TFile* fGJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
TFile* fGJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");
           
TFile* fZinv400 = new TFile("yields_ZJetsToNuNU400HT600_RA2b.root","READ");
TFile* fZinv600 = new TFile("yields_ZJetsToNuNU600HTinf_RA2b.root","READ");

void closureTest(){

  TH1F* ZinvYields = (TH1F*) fZinv400->Get("gen");
  ZinvYields->Sumw2();
  ZinvYields->Scale(0.0144);
  ZinvYields->Add( (TH1F*) fZinv600->Get("gen") , 0.00489 );

  // set all bins with zero entries to 1e-10 (a very small number) 
  for( int iBin = 1 ; iBin <= ZinvYields->GetNbinsX() ; iBin++){
    if( ZinvYields->GetBinContent( iBin ) == 0 ) 
      ZinvYields->SetBinContent( iBin , 1e-10 );
  }

 cout << "======== acceptance ==========" << endl;
 
 TH1F* GJetsGen = getGJetsYields("gen");
 TH1F* GJetsAccep = getGJetsYields("accepted");
 TH1F* GJetsID = getGJetsYields("IDed");
 TH1F* GJetsISO = getGJetsYields("ISOed");
 
 TH1F* effAcc = divideHistos( GJetsAccep , GJetsGen );
 cout << "======== ID  ==========" << endl;
 TH1F* effID = divideHistos( GJetsID , GJetsAccep );
 cout << "======== ISO ==========" << endl;
 TH1F* effISO = divideHistos( GJetsISO , GJetsID );
 
 for( int iBin = 1 ; iBin <= effAcc->GetNbinsX() ; iBin++ ){

   cout << "--- bin: " << iBin << endl;
   cout << "acc. eff: " << GJetsAccep->GetBinContent(iBin) << "/" << GJetsGen->GetBinContent(iBin) << " = " << effAcc->GetBinContent(iBin) << endl;
   cout << "ID eff: " << GJetsID->GetBinContent(iBin) << "/" << GJetsAccep->GetBinContent(iBin) << " = " << effID->GetBinContent(iBin) << endl;
   cout << "ISO eff: " << GJetsISO->GetBinContent(iBin) << "/" << GJetsISO->GetBinContent(iBin) << " = " << effISO->GetBinContent(iBin) << endl;

 }

  effAcc->SetMarkerStyle(8);
  effAcc->Draw("pE");
  effID->SetMarkerColor(4);
  effID->SetMarkerStyle(8);
  effID->Draw("SAMEpE");
  effISO->SetMarkerColor(6);
  effISO->SetMarkerStyle(8);
  effISO->Draw("SAMEpE");

  TH1F* ZgamRatio = divideHistos( ZinvYields , getGJetsYields("gen") ) ;  
  ZgamRatio->SetMarkerStyle(8);
  
  TCanvas* can2 = new TCanvas("can2","can2",500,500);
  ZgamRatio->Draw("pE");

  TH1F* Zestimate = new TH1F( *GJetsISO );
  Zestimate->SetMarkerStyle(22);
  Zestimate->GetYaxis()->SetRangeUser(0.1,3000);
  Zestimate = multiplyHistos( Zestimate , ZgamRatio );
  Zestimate = divideHistos( Zestimate , effAcc );
  Zestimate = divideHistos( Zestimate , effID );
  Zestimate = divideHistos( Zestimate , effISO );

  TCanvas* can = new TCanvas("can1","can1",500,500);

  Zestimate->Draw("E");
  ZinvYields->SetMarkerStyle(32);
  ZinvYields->Draw("SAME,E");

}

TH1F* divideHistos(TH1F* num , TH1F* denom, char options){

  TH1F* result = new TH1F(* num);

  for( int iBin = 1 ; iBin <= result->GetNbinsX(); iBin++ ){

    double ratio = num->GetBinContent( iBin ) / denom->GetBinContent( iBin ) ;
    cout << num->GetBinContent( iBin ) << "/" << denom->GetBinContent( iBin ) << " = " << ratio << endl;
    result->SetBinContent( iBin , ratio ) ;
    double error = ratio * sqrt( pow( num->GetBinError( iBin )/num->GetBinContent( iBin ) , 2 ) +
				 pow( denom->GetBinError( iBin )/denom->GetBinContent( iBin ) , 2 ) ) ;
    result->SetBinError( iBin , error );
   
  }

  return result;

}

TH1F* multiplyHistos(TH1F* num , TH1F* denom, char options){

  TH1F* result = new TH1F(* num);

  for( int iBin = 1 ; iBin <= result->GetNbinsX(); iBin++ ){

    double prod = num->GetBinContent( iBin ) * denom->GetBinContent( iBin ) ;
    result->SetBinContent( iBin , prod );
    double error = prod * sqrt( pow( num->GetBinError( iBin )/num->GetBinContent( iBin ) , 2 ) +
				pow( denom->GetBinError( iBin )/denom->GetBinContent( iBin ) , 2 ) ) ;
    result->SetBinError( iBin , error );
   
  }

  return result;

}

TH1F* getGJetsYields(TString tag){

  TH1F* GJetsYields = (TH1F*) fGJets400->Get(tag);
  GJetsYields->Sumw2();
  GJetsYields->Scale( 0.0756 );
  GJetsYields->Add( (TH1F*) fGJets600->Get(tag) , 0.0261 ) ; 
    
  // set all bins with zero entries to 1e-10 (a very small number) 
  for( int iBin = 1 ; iBin <= GJetsYields->GetNbinsX() ; iBin++){
    if( GJetsYields->GetBinContent( iBin ) == 0 ) 
      GJetsYields->SetBinContent( iBin , 1e-10 );
  }
  
  return GJetsYields;

}



/*
class ZinvEstimator {

  public : 

  TH1F* ZinvYields ;  

  map<TString,TH1F*> GJetsYields ;  
  map<TString,TH1F*> eff ;  
  map<TString,TH1F*>::iterator yieldsIt;

  TFile * fGJets400;
  TFile * fGJets600;
  	           
  TFile * fZinv400 ;
  TFile * fZinv600 ;

  ZinvEstimator();

};

ZinvEstimator::ZinvEstimator(){

  fGJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  fGJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");
           
  fZinv400 = new TFile("yields_ZJetsToNuNU400HT600_RA2b.root","READ");
  fZinv600 = new TFile("yields_ZJetsToNuNU600HTinf_RA2b.root","READ");

  ZinvYields = (TH1F*) fZinv400->Get("gen");
  ZinvYields->Sumw2();
  ZinvYields->Scale(0.0144);
  ZinvYields->Add( (TH1F*) fZinv600->Get("gen") , 0.00489 );

  // set all bins with zero entries to 1e-10 (a very small number) 
  for( int iBin = 1 ; iBin <= ZinvYields->GetNbinsX() ; iBin++){
    if( ZinvYields->GetBinContent( iBin ) == 0 ) 
      ZinvYields->SetBinContent( iBin , 1e-10 );
  }

  GJetsYields["gen"]      = 0;   
  GJetsYields["accepted"] = 0;   
  GJetsYields["IDed"]     = 0; 
  GJetsYields["ISOed"]    = 0; 
  GJetsYields["Matched"]  = 0; 

  for( yieldsIt = GJetsYields.begin() ; yieldsIt != GJetsYields.end() ; yieldsIt++ ){ 
    
    yieldsIt->second = (TH1F*) fGJets400->Get(yieldsIt->first);
    yieldsIt->second->Sumw2();
    yieldsIt->second->Scale( 0.0756 );
    yieldsIt->second->Add( (TH1F*) fGJets600->Get(yieldsIt->first) , 0.0261 ) ; 
    
    //if( yieldsIt == GJetsYields.begin() )
      //yieldsIt->second->Draw();
    //else
      //yieldsIt->second->Draw("SAME");

    // set all bins with zero entries to 1e-10 (a very small number) 
    for( int iBin = 1 ; iBin <= yieldsIt->second->GetNbinsX() ; iBin++){
      if( yieldsIt->second->GetBinContent( iBin ) == 0 ) 
	yieldsIt->second->SetBinContent( iBin , 1e-10 );
    }
    
  }

  cout << "computing acceptance efficiency" << endl;
  eff["acceptance"] = GJetsYields["accepted"];
  cout << "test" << endl;
  cout << eff["acceptance"]->Divide( GJetsYields["accepted"] , GJetsYields["gen"] , 1. , 1. , "B" ) << endl;
 
  // set all bins with zero entries to 1e-10 (a very small number) 
  for( int iBin = 1 ; iBin <= eff["acceptance"]->GetNbinsX() ; iBin++){
    cout << iBin << " " << eff["acceptance"]->GetBinContent(iBin) << endl;
  }
  //eff["accpetance"]->Draw("pe1");
  cout << "computing ID efficiency" << endl;
  eff["ID"] = GJetsYields["IDed"];
  eff["ID"]->Divide( GJetsYields["IDed"] , GJetsYields["accepted"] , 1. , 1. , "B" );
  //eff["ID"]->Draw("SAMEpe1");
  cout << "computing ISO efficiency" << endl;
  eff["ISO"] = GJetsYields["ISOed"];
  eff["ISO"]->Divide( GJetsYields["ISOed"] , GJetsYields["IDed"] , 1. , 1. , "B" );
  //eff["ISO"]->Draw("SAMEpe1");
    
};

*/
