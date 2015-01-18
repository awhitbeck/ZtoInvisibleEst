


void ZgammaRatio(){


	TFile* gamFile = new TFile("GJetsYields.root","READ");
	TFile* zFile = new TFile("ZJetsYields.root","READ");

	TH1F* gjets_njetsLowMHTYields = (TH1F*) gamFile->Get("njetsLowMHTYieldsHisto");
	TH1F* gjets_njetsMedMHTYields = (TH1F*) gamFile->Get("njetsMedMHTYieldsHisto");
	TH1F* gjets_njetsHighMHTYields = (TH1F*) gamFile->Get("njetsHighMHTYieldsHisto");

	TH1F* zjets_njetsLowMHTYields = (TH1F*) zFile->Get("njetsLowMHTYields");
	TH1F* zjets_njetsMedMHTYields = (TH1F*) zFile->Get("njetsMedMHTYields");
	TH1F* zjets_njetsHighMHTYields = (TH1F*) zFile->Get("njetsHighMHTYields");

	TH1F* ratio_njetsLowMHTYields = new TH1F(*zjets_njetsLowMHTYields);
	ratio_njetsLowMHTYields->Divide(zjets_njetsLowMHTYields,gjets_njetsLowMHTYields,1,1,"B");
	//for( int i = 1 ; i <= ratio_njetsLowMHTYields->GetNbinsX(); i++ ){
	//	ratio_njetsLowMHTYields->SetBinError( i , sqrt( pow( gjets_njetsLowMHTYields->GetBinError(i)/gjets_njetsLowMHTYields->GetBinContent(i) , 2 ) + pow( zjets_njetsLowMHTYields->GetBinError(i)/zjets_njetsLowMHTYields->GetBinContent(i) , 2 ) ) );
	//}

	TH1F* ratio_njetsMedMHTYields = new TH1F(*zjets_njetsMedMHTYields);
	ratio_njetsMedMHTYields->Divide(gjets_njetsMedMHTYields);
	//for( int i = 1 ; i <= ratio_njetsMedMHTYields->GetNbinsX(); i++ ){
	//	ratio_njetsMedMHTYields->SetBinError( i , sqrt( pow( gjets_njetsMedMHTYields->GetBinError(i)/gjets_njetsMedMHTYields->GetBinContent(i) , 2 ) + pow( zjets_njetsMedMHTYields->GetBinError(i)/zjets_njetsMedMHTYields->GetBinContent(i) , 2 ) ) );
	//}
	
	/*
	for( int i = 1 ; i <= ratio_njetsLowMHTYields->GetNbinsX(); i++ ){
		double err = sqrt( 
			pow( gjets_njetsMedMHTYields->GetBinError(i)*(zjets_njetsMedMHTYields->GetBinContent(i)/gjets_njetsMedMHTYields->GetBinContent(i)*gjets_njetsMedMHTYields->GetBinContent(i)) , 2 ) + 
			pow( zjets_njetsMedMHTYields->GetBinError(i)/gjets_njetsMedMHTYields->GetBinContent(i) , 2 ) 
			) ;

		cout << "med MHT, njets: " << i+2 << endl;
		cout << "z: " << zjets_njetsMedMHTYields->GetBinContent(i) << endl;
	 	cout << "g: " << gjets_njetsMedMHTYields->GetBinContent(i) << endl;
	 	cout << "z error: " << zjets_njetsMedMHTYields->GetBinError(i) << endl;
	 	cout << "g error: " << gjets_njetsMedMHTYields->GetBinError(i) << endl;
	 	cout << "R error: " << err << endl;
		ratio_njetsMedMHTYields->SetBinError( i , err );
	}*/

	TH1F* ratio_njetsHighMHTYields = new TH1F(*zjets_njetsHighMHTYields);
	ratio_njetsHighMHTYields->Divide(gjets_njetsHighMHTYields);
	
	//for( int i = 1 ; i <= ratio_njetsHighMHTYields->GetNbinsX(); i++ ){
	//	ratio_njetsHighMHTYields->SetBinError( i , sqrt( pow( gjets_njetsHighMHTYields->GetBinError(i)/gjets_njetsHighMHTYields->GetBinContent(i) , 2 ) + pow( zjets_njetsHighMHTYields->GetBinError(i)/zjets_njetsHighMHTYields->GetBinContent(i) , 2 ) ) );
	//}
	
	TCanvas* njets = new TCanvas("njets","njets",500,500);

	ratio_njetsLowMHTYields->Draw("pe1");
	ratio_njetsLowMHTYields->GetYaxis()->SetTitle("R_{Z/#gamma}");
	//ratio_njetsLowMHTYields->GetYaxis()->SetRangeUser(0,1);
	ratio_njetsLowMHTYields->GetXaxis()->SetTitle("n_{jets}");
	ratio_njetsLowMHTYields->SetMarkerStyle(8);
	ratio_njetsMedMHTYields->SetMarkerStyle(8);
	ratio_njetsHighMHTYields->SetMarkerStyle(8);
	ratio_njetsLowMHTYields->SetMarkerColor(1);
	ratio_njetsMedMHTYields->SetMarkerColor(4);
	ratio_njetsHighMHTYields->SetMarkerColor(6);
	ratio_njetsMedMHTYields->Draw("pe1SAME");
	ratio_njetsHighMHTYields->Draw("pe1SAME");

	TLegend* leg = new TLegend(.2,.9,.5,.6);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);

	leg->AddEntry(ratio_njetsLowMHTYields,"200<H_{T}^{miss}<300","p");
	leg->AddEntry(ratio_njetsMedMHTYields,"300<H_{T}^{miss}<450","p");
	leg->AddEntry(ratio_njetsHighMHTYields,"450<H_{T}^{miss}","p");

	leg->Draw();

	TCanvas* ht = new TCanvas("ht","ht",500,500);
	TPad* htpad = new TPad("htpad","htpad",0,0.3,1,1);
	TPad* htratiopad = new TPad("htratiopad","htratiopad",0,0,1,0.3);
	htpad->SetBottomMargin(0);
	htratiopad->SetTopMargin(0);
	htratiopad->SetBottomMargin(.3);
	htpad->Draw();
	htratiopad->Draw();
	htpad->cd();

	TH1F* gjets_htYields = (TH1F*) gamFile->Get("htYieldsHisto");
	gjets_htYields->GetYaxis()->SetTitle("Events");
	TH1F* zjets_htYields = (TH1F*) zFile->Get("htYields");
	
	TH1F* htRatio = new TH1F(*zjets_htYields);
	htRatio->Divide(gjets_htYields);
	//for( int i = 1 ; i <= htYields->GetNbinsX(); i++ ){
	//	htYields->SetBinError( i , sqrt( pow( zjets_htYields->GetBinError(i)/zjets_htYields->GetBinContent(i) , 2 ) + pow( gjets_htYields->GetBinError(i)/gjets_htYields->GetBinContent(i) , 2 ) ) );
	//}
	gjets_htYields->SetMarkerColor(2);
	zjets_htYields->SetMarkerColor(4);
	gjets_htYields->SetLineColor(2);
	zjets_htYields->SetLineColor(4);
	gjets_htYields->SetMarkerStyle(8);
	zjets_htYields->SetMarkerStyle(8);
	gPad->SetLogy();
	gjets_htYields->Draw("pe1");
	zjets_htYields->Draw("pe1SAME");

	TLegend* htLeg = new TLegend(.5,.9,.9.,.6);
	htLeg->SetFillColor(0);
	htLeg->SetBorderSize(0);
	htLeg->AddEntry(gjets_htYields,"#gamma+jets","p");
	htLeg->AddEntry(zjets_htYields,"Z+jets","p");
	htLeg->Draw();

	//gPad->SetLogx();
	htRatio->GetYaxis()->SetTitle("R_{Z/#gamma}");
	htRatio->GetXaxis()->SetTitle("H_{T} [GeV]");
	htRatio->GetYaxis()->SetRangeUser(0,.5);
	htRatio->SetMarkerStyle(8);
	htRatio->GetYaxis()->SetTitleOffset(0.6);
	htRatio->GetYaxis()->SetTitleSize(0.1);
	htRatio->GetYaxis()->SetLabelSize(0.08);
	htRatio->GetXaxis()->SetTitleOffset(1.0);
	htRatio->GetXaxis()->SetTitleSize(0.15);
	htRatio->GetXaxis()->SetLabelSize(0.13);
	htRatio->SetLineColor(1);
	htRatio->SetMarkerColor(1);
	htratiopad->cd();
	htRatio->Draw("pe1");

	TCanvas* mht = new TCanvas("mht","mht",500,500);
	TPad* mhtpad = new TPad("mhtpad","mhtpad",0,0.3,1,1);
	TPad* mhtratiopad = new TPad("mhtratiopad","mhtratiopad",0,0,1,0.3);
	mhtpad->SetBottomMargin(0);
	mhtratiopad->SetTopMargin(0);
	mhtratiopad->SetBottomMargin(.3);
	mhtpad->Draw();
	mhtratiopad->Draw();
	mhtpad->cd();
	TH1F* gjets_mhtYields = (TH1F*) gamFile->Get("mhtYieldsHisto");
	gjets_mhtYields->GetYaxis()->SetTitle("Events");
	TH1F* zjets_mhtYields = (TH1F*) zFile->Get("mhtYields");
	//for( int i = 1 ; i <= htYields->GetNbinsX(); i++ ){
	//	htYields->SetBinError( i , sqrt( pow( zjets_mhtYields->GetBinError(i)/zjets_mhtYields->GetBinContent(i) , 2 ) + pow( gjets_mhtYields->GetBinError(i)/gjets_mhtYields->GetBinContent(i) , 2 ) ) );
	//}
	gjets_mhtYields->SetMarkerColor(2);
	zjets_mhtYields->SetMarkerColor(4);
	gjets_mhtYields->SetLineColor(2);
	zjets_mhtYields->SetLineColor(4);
	gjets_mhtYields->SetMarkerStyle(8);
	zjets_mhtYields->SetMarkerStyle(8);
	gPad->SetLogy();
	gjets_mhtYields->Draw("pe1");
	zjets_mhtYields->Draw("pe1SAME");
	TLegend* mhtLeg = new TLegend(.5,.9,.9.,.6);
	mhtLeg->SetFillColor(0);
	mhtLeg->SetBorderSize(0);
	mhtLeg->AddEntry(gjets_mhtYields,"#gamma+jets","p");
	mhtLeg->AddEntry(zjets_mhtYields,"Z+jets","p");
	mhtLeg->Draw();


	TH1F* mhtRatio = new TH1F(*zjets_mhtYields);
	mhtRatio->Divide(gjets_mhtYields);

	//gPad->SetLogx();
	mhtRatio->GetYaxis()->SetTitle("R_{Z/#gamma}");
	mhtRatio->GetXaxis()->SetTitle("MHT [GeV]");
	mhtRatio->GetYaxis()->SetRangeUser(0,.7);
	mhtRatio->SetMarkerStyle(8);
	mhtRatio->GetYaxis()->SetTitleOffset(0.6);
	mhtRatio->GetYaxis()->SetTitleSize(0.1);
	mhtRatio->GetYaxis()->SetLabelSize(0.08);
	mhtRatio->GetXaxis()->SetTitleOffset(1.0);
	mhtRatio->GetXaxis()->SetTitleSize(0.15);
	mhtRatio->GetXaxis()->SetLabelSize(0.13);
	mhtRatio->SetLineColor(1);
	mhtRatio->SetMarkerColor(1);
	mhtratiopad->cd();
	mhtRatio->Draw("e1");

}