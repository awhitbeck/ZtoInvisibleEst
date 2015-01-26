

void plotEffic(TString num_ = "accepted" , TString denom_ = "gen" ){


  TFile *f400 = new TFile("yields_GJets400HT600_RA2b.root","READ");
  TFile *f600 = new TFile("yields_GJets600HTinf_RA2b.root","READ");

  TH1F* denom = (TH1F*) f600->Get(denom_) ;
  denom->GetYaxis()->SetRangeUser(0,1);
  TH1F* num = (TH1F*) f600->Get(num_);
  num->GetYaxis()->SetRangeUser(0,1);

  assert( denom->GetNbinsX() == num->GetNbinsX() ) ;

  for( int i = 1 ; i <= denom->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << denom->GetBinContent(i) << endl;
    if( denom->GetBinContent(i) == 0 ) 
      denom->SetBinContent(i,0.0001);
  }

  for( int i = 1 ; i <= num->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << num->GetBinContent(i) << endl;
    if( num->GetBinContent(i) == 0 ) 
      num->SetBinContent(i,0.0001);
  }
    
  TH1F* eff = new TH1F( *num );
  eff->Divide( denom ) ;

  for( int i = 1 ; i <= eff->GetNbinsX() ; i++ ){
    cout << "bin: " << i << " " << eff->GetBinContent(i) << endl;
  }

  eff->Draw();

}
