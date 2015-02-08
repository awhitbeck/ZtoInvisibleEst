



void test(TString tag = "gen"){

  TChain* g400 = new TChain("RA2slim");
  g400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/GJets_HT-400toInf*slim.root");
  TChain* g600 = new TChain("RA2slim");
   g600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/GJets_HT-600toInf*slim.root");
  TChain* z400 = new TChain("RA2slim");
  z400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/ZJetsToNuNu_HT-400toInf*slim.root");
  TChain* z600 = new TChain("RA2slim");
  z600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/ZJetsToNuNu_HT-600toInf*slim.root");

  g400->Draw("MHT>>hg400(100,200,1200)","HT>500&&NJets>=4");
  g600->Draw("MHT>>hg600(100,200,1200)","HT>500&&NJets>=4");

  z400->Draw("MHT>>hz400(100,200,1200)","HT>500&&NJets>=4");
  z600->Draw("MHT>>hz600(100,200,1200)","HT>500&&NJets>=4");

  TH1F* ratio = new TH1F( *((TH1F*)gDirectory->Get("hz400")) );
  ratio->Sumw2();
  ratio->Scale(0.00489467814129328);
  ratio->Add((TH1F*)gDirectory->Get("hz600"),0.0144160859953796);

  for( int i = 1 ; i < ratio->GetNbinsX() ; i++ ){    
    cout << "num: " << ratio->GetBinContent(i) << endl;
  }

  TH1F* denom = new TH1F( *((TH1F*) gDirectory->Get("hg400")));
  denom->Sumw2();
  denom->Scale(0.00245163560886522);
  denom->Add((TH1F*) gDirectory->Get("hg600"),0.00697667875133437);
  for( int i = 1 ; i < denom->GetNbinsX() ; i++ ){    
    cout << "denom: " << denom->GetBinContent(i) << endl;
  }
  ratio->Divide(denom);
  ratio->Draw("pe1");

}
