
void closureTest(){

  TFile *fGJets400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  TFile *fGJets600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");

  TFile *fZinv400 = new TFile("yields_ZJetsToNuNU400HT600_RA2b.root","READ");
  TFile *fZinv600 = new TFile("yields_ZJetsToNuNU600HTinf_RA2b.root","READ");

  TH1F* ZgamRatio_denom = (TH1F*) fGJets400->Get("gen") ;
  ZgamRatio_denom->Scale(0.0756);
  ZgamRatio_denom->Add( (TH1F*) fGJets600->Get("gen") , 0.0261 );
  ZgamRatio_denom->SetLineColor(2);
  ZgamRatio_denom->SetLineStyle(2);
  ZgamRatio_denom->SetLineWidth(2);
  ZgamRatio_denom->GetXaxis()->SetTitle("Bin Index");
  ZgamRatio_denom->GetYaxis()->SetTitle("Relative Yield");
  
  TH1F* ZgamRatio_num = (TH1F*) fZinv400->Get("gen");
  ZgamRatio_num->Scale(0.0144);
  ZgamRatio_num->Add( (TH1F*) fGJets600->Get("gen") , 0.00489 );
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
    
  TH1F* ZgamRatio = new TH1F( *ZgamRatio_num );
  ZgamRatio->Divide( ZgamRatio_denom ) ;
  //ZgamRatio->GetYaxis()->SetRangeUser(0,2);
  ZgamRatio->GetYaxis()->SetTitle("Z/#gamma Ratio");

  for( int i = 1 ; i <= ZgamRatio->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << ZgamRatio->GetBinContent(i) << endl;
  }

  ZgamRatio->Draw();

  TCanvas* can_yields = new TCanvas("can_yields","can_yields",500,500);
  
  ZgamRatio_num->DrawNormalized();
  ZgamRatio_denom->DrawNormalized("SAME");
  TLegend* leg = new TLegend(.5,.9,.9,.5);

  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->AddEntry(ZgamRatio_num,"Z(#nu#nu)","l");
  leg->AddEntry(ZgamRatio_denom,"#gamma","l");
  leg->Draw();

}
