#include "TLegend.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TCanvas.h"
#include <vector>
#include <iostream>

using namespace std;

vector<TH1F*> ZgamRatio_njets(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600);
vector<TH1F*> ZgamRatio_btags(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600);
TH1F* ZgamRatio_HT(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600);
TH1F* ZgamRatio_MHT(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600);

void validations(){

  TFile *fGJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  TFile *fGJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");

  TFile *fZinv400 = new TFile("yields_ZJetsToNuNu400HT600_RA2b.root","READ");
  TFile *fZinv600 = new TFile("yields_ZJetsToNuNu600HTinf_RA2b.root","READ");

  TCanvas* can_njets = new TCanvas("ratio_njets","ratio_njets",500,500);
  vector<TH1F*> ZgamRatioHisto_njets = ZgamRatio_njets( fGJets400 , fGJets600 , fZinv400 , fZinv600 );
  cout << "Z/gam_njets size: " << ZgamRatioHisto_njets.size() << endl;
  ZgamRatioHisto_njets[0]->GetXaxis()->SetNdivisions(505);
  ZgamRatioHisto_njets[0]->Draw("pe1");
  ZgamRatioHisto_njets[1]->Draw("SAMEpe1");
  ZgamRatioHisto_njets[2]->Draw("SAMEpe1");
  can_njets->SaveAs("ZgammaRatio_vs_njets.eps");

  TCanvas* can_btags = new TCanvas("ratio_btags","ratio_btags",500,500);
  vector<TH1F*> ZgamRatioHisto_btags = ZgamRatio_btags( fGJets400 , fGJets600 , fZinv400 , fZinv600 );
  cout << "Z/gam_btags size: " << ZgamRatioHisto_btags.size() << endl;
  ZgamRatioHisto_btags[0]->GetXaxis()->SetNdivisions(505);
  ZgamRatioHisto_btags[0]->Draw("pe1");
  ZgamRatioHisto_btags[1]->Draw("SAMEpe1");
  ZgamRatioHisto_btags[2]->Draw("SAMEpe1");
  can_btags->SaveAs("ZgammaRatio_vs_btags.eps");

  TCanvas* can_HT = new TCanvas("ratio_HT","ratio_HT",500,500);
  TH1F* ZgamRatioHisto_HT = ZgamRatio_HT( fGJets400 , fGJets600 , fZinv400 , fZinv600 );
  ZgamRatioHisto_HT->GetXaxis()->SetNdivisions(505);
  ZgamRatioHisto_HT->Draw("pe1");
  can_HT->SaveAs("ZgammaRatio_vs_HT.eps");

  TCanvas* can_MHT = new TCanvas("ratio_MHT","ratio_MHT",500,500);
  TH1F* ZgamRatioHisto_MHT = ZgamRatio_MHT( fGJets400 , fGJets600 , fZinv400 , fZinv600 );
  ZgamRatioHisto_MHT->GetXaxis()->SetNdivisions(505);
  ZgamRatioHisto_MHT->Draw("pe1");
  can_MHT->SaveAs("ZgammaRatio_vs_MHT.eps");

  cout << "done drawing z/gam in njet/MHT bins" << endl;
  
  TH1F ZgamRatio_denom((TH1F) fGJets400->Get("gen")) ;
  ZgamRatio_denom->Scale(0.0756);
  ZgamRatio_denom->Add( (TH1F*) fGJets600->Get("gen") , 0.0261 );
  ZgamRatio_denom->SetLineColor(2);
  ZgamRatio_denom->SetLineStyle(2);
  ZgamRatio_denom->SetLineWidth(2);
  ZgamRatio_denom->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_denom->GetYaxis()->SetTitle("Relative Yield");
  
  TH1F ZgamRatio_num((TH1F*) fZinv400->Get("gen") );
  ZgamRatio_num->Scale(0.0144);
  ZgamRatio_num->Add( (TH1F*) fZinv600->Get("gen") , 0.00489 );
  ZgamRatio_num->SetLineWidth(2);
  ZgamRatio_num->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_num->GetYaxis()->SetTitle("Relative Yield");

  assert( ZgamRatio_denom->GetNbinsX() == ZgamRatio_num->GetNbinsX() ) ;

  // NOTE: 'gen' here just refers to the fact that the efficiencies of 
  // reconstructing a boson has been infolded back to gen-level bosons... 

  for( int i = 1 ; i <= ZgamRatio_denom->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio_denom->GetBinContent(i) << endl;
    if( ZgamRatio_denom->GetBinContent(i) == 0 ) 
      ZgamRatio_denom->SetBinContent(i,1e-10);
  }

  for( int i = 1 ; i <= ZgamRatio_num->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio_num->GetBinContent(i) << endl;
    if( ZgamRatio_num->GetBinContent(i) == 0 ) 
      ZgamRatio_num->SetBinContent(i,1e-10);
  }

  cout << "computing ZgamRatio" << endl;
    
  TH1F ZgamRatio( ZgamRatio_num );
  //ZgamRatio->Divide( ZgamRatio_denom ) ;

  for( int xBin = 1 ; xBin <= ZgamRatio->GetNbinsX() ; xBin++ ){

    cout << "xBin: " << xBin << endl;
    double ratio = ZgamRatio_num->GetBinContent( xBin )/ZgamRatio_denom->GetBinContent( xBin ) ;
    ZgamRatio->SetBinContent( xBin , ratio ) ;
    cout << "MHT bin(" << xBin << "): " << ZgamRatio_num->GetBinContent( xBin ) << "/" << ZgamRatio_denom->GetBinContent( xBin ) << "=" << ratio << endl;
    double error =  ratio * sqrt( pow( ZgamRatio_num->GetBinError( xBin ) / ZgamRatio_num->GetBinContent( xBin ) , 2 ) + pow( ZgamRatio_denom->GetBinError( xBin ) / ZgamRatio_denom->GetBinContent( xBin ) , 2 ) );
    cout << "error: " << error << endl;
    ZgamRatio->SetBinError( xBin , error );

  }

  //ZgamRatio->GetYaxis()->SetRangeUser(0,2);
  ZgamRatio->GetYaxis()->SetTitle("Z/#gamma Ratio");


  cout << "drawing z/gam ratio" << endl; 

  TCanvas* can_ratio = new TCanvas("can_ratio","can_ratio",500,500);
  ZgamRatio->GetYaxis()->SetRangeUser(0,2);
  ZgamRatio->Draw("pe1");
  can_ratio->SaveAs("ZgammaRatio_vs_RA2bBin.eps");

  cout << "drawing z/gam yields" << endl; 

  TCanvas* can_yields = new TCanvas("can_yields","can_yields",500,500);
  ZgamRatio_num->Scale(1./ZgamRatio_num->Integral());
  ZgamRatio_num->GetYaxis()->SetRangeUser(0.000001,1);
  ZgamRatio_num->Draw();
  ZgamRatio_denom->Scale(1./ZgamRatio_denom->Integral());
  ZgamRatio_denom->Draw("SAME");
  TLegend* leg = new TLegend(.5,.9,.9,.5);

  cout << "legend stuff" << endl;

  gPad->SetLogy();
 
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ZgamRatio_num,"Z(#nu#nu)","l");
  leg->AddEntry(ZgamRatio_denom,"#gamma","l");
  leg->Draw();

  can_yields->SaveAs("ZgammaRelYields_vs_RA2bBin.eps");
}

vector<TH1F*> ZgamRatio_njets(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600){

  TH2F ZgamRatio_denom( (TH2F) fGJets400->Get("gen_njets") ) ;
  ZgamRatio_denom->Sumw2();
  ZgamRatio_denom->Scale(0.0756);
  ZgamRatio_denom->Add( (TH2F*) fGJets600->Get("gen_njets") , 0.0261 );
  ZgamRatio_denom->SetLineColor(2);
  ZgamRatio_denom->SetLineStyle(2);
  ZgamRatio_denom->SetLineWidth(2);
  ZgamRatio_denom->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_denom->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_denom->GetNbinsX() ; i++ ){
    for( int j = 1 ; j <= ZgamRatio_denom->GetNbinsY() ; j++ ){
      cout << "bin: " << i << " " << ZgamRatio_denom->GetBinContent(i,j) << endl;
      if( ZgamRatio_denom->GetBinContent(i,j) == 0 ) 
	ZgamRatio_denom->SetBinContent(i,j,1e-10);
    }
  }
  
  TH2F ZgamRatio_num( (TH2F) fZinv400->Get("gen_njets") );
  ZgamRatio_num->Sumw2();
  ZgamRatio_num->Scale(0.0144);
  ZgamRatio_num->Add( (TH2F*) fZinv600->Get("gen_njets") , 0.00489 );
  ZgamRatio_num->SetLineWidth(2);
  ZgamRatio_num->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_num->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_num->GetNbinsX() ; i++ ){
    for( int j = 1 ; j <= ZgamRatio_num->GetNbinsY() ; j++ ){
      cout << "bin: " << i << " " << ZgamRatio_num->GetBinContent(i,j) << endl;
      if( ZgamRatio_num->GetBinContent(i,j) == 0 ) 
	ZgamRatio_num->SetBinContent(i,j,1e-10);
    }
  }

  assert( ZgamRatio_denom->GetNbinsX() == ZgamRatio_num->GetNbinsX() ) ;

  TH2F ZgamRatio(ZgamRatio_num);
  ZgamRatio->Divide(&ZgamRatio_denom);
  
  vector<TH1F*> ZgamRatios;
  ZgamRatios.push_back( new TH1F("ZgamRatio_njets_lowMHT",";n_{jets};Z/#gamma Ratio",7,2.3,9.3));
  ZgamRatios.push_back( new TH1F("ZgamRatio_njets_medMHT",";n_{jets};Z/#gamma Ratio",7,2.5,9.5));
  ZgamRatios.push_back( new TH1F("ZgamRatio_njets_highMHT",";n_{jets};Z/#gamma Ratio",7,2.7,9.7));

  ZgamRatios[0]->GetYaxis()->SetRangeUser(0,2);
  ZgamRatios[0]->SetLineColor(1);
  ZgamRatios[0]->SetLineWidth(2);
  ZgamRatios[0]->SetMarkerColor(1);
  ZgamRatios[0]->SetMarkerStyle(8);

  ZgamRatios[1]->SetLineColor(4);
  ZgamRatios[1]->SetLineWidth(2);
  ZgamRatios[1]->SetMarkerColor(4);
  ZgamRatios[1]->SetMarkerStyle(8);

  ZgamRatios[2]->SetLineColor(6);
  ZgamRatios[2]->SetLineWidth(2);
  ZgamRatios[2]->SetMarkerColor(6);
  ZgamRatios[2]->SetMarkerStyle(8);

  for( int yBin = 1 ; yBin <= ZgamRatios.size() ; yBin++){
    cout << "yBin: " << yBin << endl;
    cout << "xBins: " << ZgamRatios[yBin-1]->GetNbinsX() << endl;
    for( int xBin = 1 ; xBin <= ZgamRatios[yBin-1]->GetNbinsX();  xBin++){

      cout << "xBin: " << xBin << endl;
      double ratio = ZgamRatio_num->GetBinContent( xBin , yBin )/ZgamRatio_denom->GetBinContent( xBin , yBin ) ;
      ZgamRatios[yBin-1]->SetBinContent( xBin , yBin , ratio ) ;
      cout << "MHT bin(" << xBin << "," << yBin << "): " << ZgamRatio_num->GetBinContent( xBin , yBin ) << "/" << ZgamRatio_denom->GetBinContent( xBin , yBin ) << "=" << ratio << endl;
      double error =  ratio * sqrt( pow( ZgamRatio_num->GetBinError( xBin , yBin ) / ZgamRatio_num->GetBinContent( xBin , yBin ) , 2 ) + pow( ZgamRatio_denom->GetBinError( xBin , yBin ) / ZgamRatio_denom->GetBinContent( xBin , yBin ) , 2 ) );
      cout << "error: " << error << endl;
      ZgamRatios[yBin-1]->SetBinError( xBin , yBin , error );

    }
  }
  
  cout << "returning from ZgamRatio_njets()" << endl;
  return ZgamRatios;

}

vector<TH1F*> ZgamRatio_btags(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600){

  TH2F ZgamRatio_denom( (TH2F) fGJets400->Get("gen_btags") );
  ZgamRatio_denom->Sumw2();
  ZgamRatio_denom->Scale(0.0756);
  ZgamRatio_denom->Add( (TH2F*) fGJets600->Get("gen_btags") , 0.0261 );
  ZgamRatio_denom->SetLineColor(2);
  ZgamRatio_denom->SetLineStyle(2);
  ZgamRatio_denom->SetLineWidth(2);
  ZgamRatio_denom->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_denom->GetYaxis()->SetTitle("Relative Yield");
    
  for( int i = 1 ; i <= ZgamRatio_denom->GetNbinsX() ; i++ ){
    for( int j = 1 ; j <= ZgamRatio_denom->GetNbinsY() ; j++ ){
      cout << "bin: " << i << " " << ZgamRatio_denom->GetBinContent(i,j) << endl;
      if( ZgamRatio_denom->GetBinContent(i,j) == 0 ) 
	ZgamRatio_denom->SetBinContent(i,j,1e-10);
    }
  }

  TH2F ZgamRatio_num( (TH2F) fZinv400->Get("gen_btags") );
  ZgamRatio_num->Sumw2();
  ZgamRatio_num->Scale(0.0144);
  ZgamRatio_num->Add( (TH2F*) fZinv600->Get("gen_btags") , 0.00489 );
  ZgamRatio_num->SetLineWidth(2);
  ZgamRatio_num->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_num->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_num->GetNbinsX() ; i++ ){
    for( int j = 1 ; j <= ZgamRatio_num->GetNbinsY() ; j++ ){
      cout << "bin: " << i << " " << ZgamRatio_num->GetBinContent(i,j) << endl;
      if( ZgamRatio_num->GetBinContent(i,j) == 0 ) 
	ZgamRatio_num->SetBinContent(i,j,1e-10);
    }
  }

  assert( ZgamRatio_denom->GetNbinsX() == ZgamRatio_num->GetNbinsX() ) ;

  vector<TH1F*> ZgamRatios;
  ZgamRatios.push_back( new TH1F("ZgamRatio_btags_lowMHT",";b-tags;Z/#gamma Ratio",4,-0.7,3.3) );
  ZgamRatios.push_back( new TH1F("ZgamRatio_btags_medMHT",";b-tags;Z/#gamma Ratio",4,-0.5,3.5) );
  ZgamRatios.push_back( new TH1F("ZgamRatio_btags_highMHT",";b-tags;Z/#gamma Ratio",4,-0.3,3.7) );

  ZgamRatios[0]->GetYaxis()->SetRangeUser(0,2);
  ZgamRatios[0]->SetLineColor(1);
  ZgamRatios[0]->SetLineWidth(2);
  ZgamRatios[0]->SetMarkerColor(1);
  ZgamRatios[0]->SetMarkerStyle(8);

  ZgamRatios[1]->SetLineColor(4);
  ZgamRatios[1]->SetLineWidth(2);
  ZgamRatios[1]->SetMarkerColor(4);
  ZgamRatios[1]->SetMarkerStyle(8);

  ZgamRatios[2]->SetLineColor(6);
  ZgamRatios[2]->SetLineWidth(2);
  ZgamRatios[2]->SetMarkerColor(6);
  ZgamRatios[2]->SetMarkerStyle(8);

  for( int yBin = 1 ; yBin <= ZgamRatios.size() ; yBin++){
    cout << "yBin: " << yBin << endl;
    cout << "xBins: " << ZgamRatios[yBin-1]->GetNbinsX() << endl;
    for( int xBin = 1 ; xBin <= ZgamRatios[yBin-1]->GetNbinsX();  xBin++){

      cout << "xBin: " << xBin << endl;
      double ratio = ZgamRatio_num->GetBinContent( xBin , yBin )/ZgamRatio_denom->GetBinContent( xBin , yBin ) ;
      ZgamRatios[yBin-1]->SetBinContent( xBin , yBin , ratio ) ;
      cout << "MHT bin(" << xBin << "," << yBin << "): " << ZgamRatio_num->GetBinContent( xBin , yBin ) << "/" << ZgamRatio_denom->GetBinContent( xBin , yBin ) << "=" << ratio << endl;
      double error =  ratio * sqrt( pow( ZgamRatio_num->GetBinError( xBin , yBin ) / ZgamRatio_num->GetBinContent( xBin , yBin ) , 2 ) + pow( ZgamRatio_denom->GetBinError( xBin , yBin ) / ZgamRatio_denom->GetBinContent( xBin , yBin ) , 2 ) );
      cout << "error: " << error << endl;
      ZgamRatios[yBin-1]->SetBinError( xBin , yBin , error );

    }
  }
  
  cout << "returning from ZgamRatio_btags()" << endl;
  return ZgamRatios;

}

TH1F* ZgamRatio_HT(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600){

  TH1F ZgamRatio_denom( (TH1F) fGJets400->Get("gen_HT") );
  ZgamRatio_denom->Sumw2();
  ZgamRatio_denom->Scale(0.0756);
  ZgamRatio_denom->Add( (TH1F*) fGJets600->Get("gen_HT") , 0.0261 );
  ZgamRatio_denom->SetLineColor(2);
  ZgamRatio_denom->SetLineStyle(2);
  ZgamRatio_denom->SetLineWidth(2);
  ZgamRatio_denom->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_denom->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_denom->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio_denom->GetBinContent(i) << endl;
    if( ZgamRatio_denom->GetBinContent(i) == 0 ) 
      ZgamRatio_denom->SetBinContent(i,1e-10);
  }
 
  TH1F ZgamRatio_num((TH1F) fZinv400->Get("gen_HT"));
  ZgamRatio_num->Sumw2();
  ZgamRatio_num->Scale(0.0144);
  ZgamRatio_num->Add( (TH1F*) fZinv600->Get("gen_HT") , 0.00489 );
  ZgamRatio_num->SetLineWidth(2);
  ZgamRatio_num->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_num->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_num->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio_num->GetBinContent(i) << endl;
    if( ZgamRatio_num->GetBinContent(i) == 0 ) 
      ZgamRatio_num->SetBinContent(i,1e-10);
  }

  assert( ZgamRatio_denom->GetNbinsX() == ZgamRatio_num->GetNbinsX() ) ;

  TH1F* ZgamRatio = new TH1F("ZgamRatio_HT_lowMHT",";HT [GeV];Z/#gamma Ratio",21,500,2700);

  ZgamRatio->GetYaxis()->SetRangeUser(0,2);
  ZgamRatio->SetLineColor(1);
  ZgamRatio->SetLineWidth(2);
  ZgamRatio->SetMarkerColor(1);
  ZgamRatio->SetMarkerStyle(8);

  for( int xBin = 1 ; xBin <= ZgamRatio->GetNbinsX();  xBin++){

    //cout << "xBin: " << xBin << endl;
    double ratio = ZgamRatio_num->GetBinContent( xBin )/ZgamRatio_denom->GetBinContent( xBin ) ;
    ZgamRatio->SetBinContent( xBin , ratio ) ;
    //cout << "MHT bin(" << xBin << "): " << ZgamRatio_num->GetBinContent( xBin ) << "/" << ZgamRatio_denom->GetBinContent( xBin ) << "=" << ratio << endl;
    double error =  ratio * sqrt( pow( ZgamRatio_num->GetBinError( xBin ) / ZgamRatio_num->GetBinContent( xBin ) , 2 ) + pow( ZgamRatio_denom->GetBinError( xBin ) / ZgamRatio_denom->GetBinContent( xBin ) , 2 ) );
    //cout << "error: " << error << endl;
    ZgamRatio->SetBinError( xBin , error );

  }
  
  //cout << "returning from ZgamRatio_HT()" << endl;
  return ZgamRatio;

}


TH1F* ZgamRatio_MHT(TFile* fGJets400, TFile* fGJets600, TFile* fZinv400, TFile* fZinv600){

  TH1F ZgamRatio_denom( (TH1F*) fGJets400->Get("gen_MHT") ) ;
  ZgamRatio_denom->Sumw2();
  ZgamRatio_denom->Scale(0.0756);
  ZgamRatio_denom->Add( (TH1F*) fGJets600->Get("gen_MHT") , 0.0261 );
  ZgamRatio_denom->SetLineColor(2);
  ZgamRatio_denom->SetLineStyle(2);
  ZgamRatio_denom->SetLineWidth(2);
  ZgamRatio_denom->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_denom->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_denom->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio_denom->GetBinContent(i) << endl;
    if( ZgamRatio_denom->GetBinContent(i) == 0 ) 
      ZgamRatio_denom->SetBinContent(i,1e-10);
  }
  
  TH1F ZgamRatio_num( (TH1F) fZinv400->Get("gen_MHT") );
  ZgamRatio_num->Sumw2();
  ZgamRatio_num->Scale(0.0144);
  ZgamRatio_num->Add( (TH1F*) fZinv600->Get("gen_MHT") , 0.00489 );
  ZgamRatio_num->SetLineWidth(2);
  ZgamRatio_num->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_num->GetYaxis()->SetTitle("Relative Yield");

  for( int i = 1 ; i <= ZgamRatio_num->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio_num->GetBinContent(i) << endl;
    if( ZgamRatio_num->GetBinContent(i) == 0 ) 
      ZgamRatio_num->SetBinContent(i,1e-10);
  }

  assert( ZgamRatio_denom->GetNbinsX() == ZgamRatio_num->GetNbinsX() ) ;

  TH1F* ZgamRatio = new TH1F("ZgamRatio_MHT_lowMMHT",";MHT [GeV];Z/#gamma Ratio",21,200,2180);

  ZgamRatio->GetYaxis()->SetRangeUser(0,2);
  ZgamRatio->SetLineColor(1);
  ZgamRatio->SetLineWidth(2);
  ZgamRatio->SetMarkerColor(1);
  ZgamRatio->SetMarkerStyle(8);

  for( int xBin = 1 ; xBin <= ZgamRatio->GetNbinsX();  xBin++){

    //cout << "xBin: " << xBin << endl;
    double ratio = ZgamRatio_num->GetBinContent( xBin )/ZgamRatio_denom->GetBinContent( xBin ) ;
    ZgamRatio->SetBinContent( xBin , ratio ) ;
    //cout << "MHT bin(" << xBin << "): " << ZgamRatio_num->GetBinContent( xBin ) << "/" << ZgamRatio_denom->GetBinContent( xBin ) << "=" << ratio << endl;
    double error =  ratio * sqrt( pow( ZgamRatio_num->GetBinError( xBin ) / ZgamRatio_num->GetBinContent( xBin ) , 2 ) + pow( ZgamRatio_denom->GetBinError( xBin ) / ZgamRatio_denom->GetBinContent( xBin ) , 2 ) );
    //cout << "error: " << error << endl;
    ZgamRatio->SetBinError( xBin , error );

  }
  
  //cout << "returning from ZgamRatio_MHT()" << endl;
  return ZgamRatio;

}
