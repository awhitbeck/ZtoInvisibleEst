#include <vector>
#include <iostream>
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TString.h"
#include "TFile.h"

using namespace std;

vector<TH1F*> getEffic_njets(TString num_ , TString denom_ );
vector<TH1F*> getEffic_btags(TString num_ , TString denom_ );
TH1F* getEffic(TString num_ , TString denom_ );

void plotEffic(TString num_ = "ISOed" , TString denom_ = "IDed" , TString yAxis ="Isolation" ){
  
  vector<TH1F*> eff;
  eff.push_back( getEffic(num_+"_HT",denom_+"_HT"));
  eff.push_back(getEffic(num_+"_MHT",denom_+"_MHT"));

  vector<TString> xAxis;
  xAxis.push_back("HT [GeV]");
  xAxis.push_back("MHT [GeV]");

  vector<TString> saveTag;
  saveTag.push_back("HT");
  saveTag.push_back("MHT");

  int color[3] = { 1 , 4 , 6 };
  
  vector<TH1F*> effic_njets = getEffic_njets(num_+"_njets",denom_+"_njets");
  for(unsigned int i = 0 ; i < effic_njets.size() ; i++ ){
    effic_njets[i]->SetLineColor(color[i]);
    effic_njets[i]->SetMarkerColor(color[i]);

    eff.push_back(effic_njets[i]);
    xAxis.push_back("n_{jets}");    
    char buffer[8];
    sprintf(buffer,"njets_%i",i);
    saveTag.push_back(buffer);
  }

  vector<TH1F*> effic_btags = getEffic_btags(num_+"_btags",denom_+"_btags");
  for(unsigned int i = 0 ; i < effic_btags.size() ; i++ ){
    effic_btags[i]->SetLineColor(color[i]);
    effic_btags[i]->SetMarkerColor(color[i]);

    eff.push_back(effic_btags[i]);
    xAxis.push_back("b-tags");    
    char buffer[8];
    sprintf(buffer,"btags_%i",i);
    saveTag.push_back(buffer);
  }

  TCanvas* can = new TCanvas("can","can",500,500);
  
  for(unsigned int i = 0 ; i < eff.size() ; i++){
    cout << "i: " << i << endl;
    eff[i]->SetLineColor(1);
    eff[i]->SetLineWidth(2);
    eff[i]->SetLineStyle(1);
    eff[i]->SetMarkerStyle(8);
    eff[i]->GetXaxis()->SetTitle(xAxis[i]);
    eff[i]->GetXaxis()->SetNdivisions(505);
    eff[i]->GetYaxis()->SetRangeUser(0,1);
    eff[i]->GetYaxis()->SetTitle(yAxis+" Efficiency");
    char buffer[8];
    sprintf(buffer,"can_%i",i);
    //cans.push_back( new TCanvas(buffer,buffer,500,500) );
    if( i > 0 ){
      if( eff[i]->GetXaxis()->GetTitle() == eff[i-1]->GetXaxis()->GetTitle() )
	eff[i]->Draw("ep1,same");
      else
	eff[i]->Draw("ep1");
    }
    sprintf(buffer,"%s_%s_efficiency_versus_%s.eps",num_.Data(),denom_.Data(),saveTag[i].Data());
    can->SaveAs(buffer);
  }

  cout << "test" << endl;
}

TH1F* getEffic(TString num_ , TString denom_ ){


  TFile *GJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  TFile *GJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");

  //TFile *Zinv400 = new TFile("yields__RA2b.root","READ");
  //TFile *Zinv600 = new TFile("yields__RA2b.root","READ");

  TH1F denom( (TH1F) GJets400->Get(denom_) );
  denom.Sumw2();
  denom.Scale(0.0756);
  denom.Add( (TH1F*) GJets600->Get(denom_) , 0.0261 );

  TH1F num( (TH1F) GJets400->Get(num_) ) ;
  num.Sumw2();
  num.Scale(0.0756);
  num.Add( (TH1F*) GJets600->Get(num_) , 0.0261 );
  
  assert( denom.GetNbinsX() == num->GetNbinsX() ) ;

  TH1F* eff = new TH1F( num );

  for(int i = 1 ; i <= denom.GetNbinsX() ; i++ ){
    //cout << "bin: " << i << " " << denom.GetBinContent(i) << endl;
    if( denom.GetBinContent(i) == 0 ) 
      denom.SetBinContent(i,0.0001);
    if( num.GetBinContent(i) == 0 ) 
      num.SetBinContent(i,0.0001);

    double ratio = num.GetBinContent(i)/denom.GetBinContent(i);
    double error = ratio * sqrt( 
				pow( num.GetBinError(i) / num.GetBinContent(i) , 2 ) +
				pow( denom.GetBinError(i) / denom.GetBinContent(i) , 2 ) ) ;

    eff->SetBinContent( i , ratio );
    eff->SetBinError( i , error );

  }

  return eff;

}

vector<TH1F*> getEffic_njets(TString num_ , TString denom_ ){


  TFile *GJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  TFile *GJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");

  TH2F denom( (TH2F) GJets400->Get(denom_) );
  denom.Sumw2();
  denom.Scale(0.0756);
  denom.Add( (TH2F*) GJets600->Get(denom_) , 0.0261 );

  TH2F num( (TH2F) GJets400->Get(num_) );
  num.Sumw2();
  num.Scale(0.0756);
  num.Add( (TH2F*) GJets600->Get(num_) , 0.0261 );
  
  assert( denom.GetNbinsX() == num->GetNbinsX() ) ;

  vector<TH1F*> eff;
  eff.push_back( new TH1F(num_+"_"+denom_+"_effic_njets_lowMHT",num_+"_"+denom_+"_effic_njets_lowMHT",7,2.5,9.5) ) ;
  eff.push_back( new TH1F(num_+"_"+denom_+"_effic_njets_medMHT",num_+"_"+denom_+"_effic_njets_medMHT",7,2.5,9.5) ) ;
  eff.push_back( new TH1F(num_+"_"+denom_+"_effic_njets_highMHT",num_+"_"+denom_+"_effic_njets_highMHT",7,2.5,9.5) ) ;

  for(unsigned int yBin = 1 ; yBin <= eff.size() ; yBin++){
    for(int xBin = 1 ; xBin <= eff[yBin-1]->GetNbinsX();  xBin++){

      double ratio = num.GetBinContent( xBin , yBin )/denom.GetBinContent( xBin , yBin ) ;
      eff[yBin-1]->SetBinContent( xBin , yBin , ratio ) ;
      double error =  ratio * sqrt( pow( num.GetBinError( xBin , yBin ) / num.GetBinContent( xBin , yBin ) , 2 ) + pow( denom.GetBinError( xBin , yBin ) / denom.GetBinContent( xBin , yBin ) , 2 ) );
      eff[yBin-1]->SetBinError( xBin , yBin , error );

    }
  }
  
  return eff;

}

vector<TH1F*> getEffic_btags(TString num_ , TString denom_ ){

  TFile *GJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  TFile *GJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");

  TH2F denom( (TH2F) GJets400->Get(denom_) );
  denom.Sumw2();
  denom.Scale(0.0756);
  denom.Add( (TH2F*) GJets600->Get(denom_) , 0.0261 );

  TH2F num( (TH2F) GJets400->Get(num_) );
  num.Sumw2();
  num.Scale(0.0756);
  num.Add( (TH2F*) GJets600->Get(num_) , 0.0261 );
  
  //assert( denom.GetNbinsX() == num.GetNbinsX() ) ;

  vector<TH1F*> eff;
  eff.push_back( new TH1F(num_+"_"+denom_+"_effic_btags_lowMHT",num_+"_"+denom_+"_effic_btags_lowMHT",4,-0.5,3.5) ) ;
  eff.push_back( new TH1F(num_+"_"+denom_+"_effic_btags_medMHT",num_+"_"+denom_+"_effic_btags_medMHT",4,-0.5,3.5) ) ;
  eff.push_back( new TH1F(num_+"_"+denom_+"_effic_btags_highMHT",num_+"_"+denom_+"_effic_btags_highMHT",4,-0.5,3.5) ) ;

  for(unsigned int yBin = 1 ; yBin <= eff.size() ; yBin++){
    for(int xBin = 1 ; xBin <= eff[yBin-1]->GetNbinsX();  xBin++){

      double ratio = num.GetBinContent( xBin , yBin )/denom.GetBinContent( xBin , yBin ) ;
      eff[yBin-1]->SetBinContent( xBin , yBin , ratio ) ;
      double error =  ratio * sqrt( pow( num.GetBinError( xBin , yBin ) / num.GetBinContent( xBin , yBin ) , 2 ) + pow( denom.GetBinError( xBin , yBin ) / denom.GetBinContent( xBin , yBin ) , 2 ) );
      eff[yBin-1]->SetBinError( xBin , yBin , error );

    }
  }
  
  return eff;

}
