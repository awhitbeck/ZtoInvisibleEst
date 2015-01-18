
// isoComparison("photon_ConeDR03hcalEtIso","Cone Iso_{nu}","photon_ConeDR03hcalEtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_ConeDR03ecalEtIso","Cone Iso_{ph}","photon_ConeDR03ecalEtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_ConeDR03trkPtIso","Cone Iso_{ch}","photon_ConeDR03trkPtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_sigmaIetaIeta","#sigma_{i#etai#eta}","photonPt>100&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_hadTowOverEm","H/E","photon_hadTowOverEm<0.1&&photonPt>100&&photon_isEB==1")
// isoComparison("photon_hasPixelSeed","Pixel Seed","photonPt>100&&photon_isEB==1")

void isoComparison(TString isoVar = "photon_ConeDR03ecalEtIso" ,
				   TString yAxisTitle = "Cone Iso_{ph}",
				   TString cutString = "photon_ConeDR03ecalEtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05" ){
	
	TFile* GJets_8TeV = new TFile("GJets_HT-400ToInf_8TeV_LPCSUSYPAT_SumJetMass_AnalysisTree.root","READ");
	TTree* tGJets_8TeV = (TTree*) GJets_8TeV->Get("TreeFiller/AnalysisTree");
	TFile* GJets_400HT600_13TeV = new TFile("GJets_400HT600_13TeV_PU_S14_SumJetMass_AnalysisTree.root","READ");
	TTree* tGJets_400HT600_13TeV = (TTree*) GJets_400HT600_13TeV->Get("TreeFiller/AnalysisTree");
	TFile* GJets_600HTinf_13TeV = new TFile("GJets_600HTinf_13TeV_PU_S14_SumJetMass_AnalysisTree.root","READ");
	TTree* tGJets_600HTinf_13TeV = (TTree*) GJets_600HTinf_13TeV->Get("TreeFiller/AnalysisTree");

	tGJets_8TeV->Draw(isoVar+">>hGJets_8TeV",cutString);
	tGJets_400HT600_13TeV->Draw(isoVar+">>hGJets_400HT600_13TeV",cutString);
	tGJets_600HTinf_13TeV->Draw(isoVar+">>hGJets_600HTinf_13TeV",cutString);

	TH1F* hGJets_8TeV = (TH1F*) gDirectory->Get("hGJets_8TeV");
	hGJets_8TeV->Scale(1./hGJets_8TeV->Integral());
	hGJets_8TeV->SetLineColor(kGreen+2);
	hGJets_8TeV->SetLineWidth(2);
	TH1F* hGJets_400HT600_13TeV = (TH1F*) gDirectory->Get("hGJets_400HT600_13TeV");
	TH1F* hGJets_600HTinf_13TeV = (TH1F*) gDirectory->Get("hGJets_600HTinf_13TeV");

	TH1F* hGJets_13TeV = new TH1F(*hGJets_400HT600_13TeV);
	hGJets_13TeV->Scale(3.1025);
	hGJets_13TeV->Add(hGJets_600HTinf_13TeV,1.0435);
	hGJets_13TeV->Scale(1./hGJets_13TeV->Integral());
	hGJets_13TeV->SetLineColor(kAzure+4);
	hGJets_13TeV->SetLineWidth(2);
	hGJets_13TeV->SetLineStyle(2);

	hGJets_13TeV->GetYaxis()->SetRangeUser( 0.0001 , max( hGJets_13TeV->GetMaximum() , hGJets_8TeV->GetMaximum() ) * 1.3);
	hGJets_13TeV->GetYaxis()->SetTitle("Events (a.u.)");
	hGJets_13TeV->GetXaxis()->SetTitle(yAxisTitle);

	hGJets_13TeV->Draw();
	hGJets_8TeV->Draw("SAME");

	gPad->SetLogy();

	TLegend* leg = new TLegend(.6,.9,.9,.6);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);

	leg->AddEntry(hGJets_8TeV,"8 TeV","l");
	leg->AddEntry(hGJets_13TeV,"13 TeV","l");

	leg->Draw();

}