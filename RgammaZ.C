#include "stackZmumuControlPlots.C"
#include "stackedControlPlots.C"
#include "THStack.h"
#include "TList.h"
#include <vector>

TH1F* unstack(THStack* stack){

	cout << "unstacking" << endl;

	TList* histos = stack->GetHists();

	cout << "got TList" << endl;
	stack->Draw();
	//return 0;
	cout << stack->GetXaxis() << endl;
	cout << stack->GetXaxis()->GetNbins() << " " << stack->GetXaxis()->GetXmin() << " " << stack->GetXaxis()->GetXmax() << endl;

	TH1F* sum = new TH1F( "sum" , "sum" , stack->GetXaxis()->GetNbins(), stack->GetXaxis()->GetXmin(), stack->GetXaxis()->GetXmax()  );

	cout << "initialized sum" << endl;

	TIter iter(histos);

	cout << "initialized TIter" << endl;

	TH1F* hist;

	cout << "starting loop" << endl;

	while( hist = (TH1F*) iter.Next() ){
		cout << "check" << endl;
		sum->Add(hist);
	}

	return sum;

}
	
void RgammaZ(){
	
	vector< THStack*> stackedZmumuControlPlots;
	vector< THStack*> stackedGammaControlPlots;

	stackedZmumuControlPlots = stackZmumuControlPlots();
	stackedGammaControlPlots = stackedControlPlots();

	TCanvas* HT = new TCanvas("HT","HT",500,500);
	cout << "made canvas" << endl;
	TH1F* ZmumuHT = unstack( stackedZmumuControlPlots[3] );
	TH1F* GammaHT = unstack( stackedGammaControlPlots[10] );
	TH1F* ZmumuMHT = unstack( stackedZmumuControlPlots[4] );
	TH1F* GammaMHT = unstack( stackedGammaControlPlots[11] );
	ZmumuMHT->DrawNormalized();
	GammaMHT->DrawNormalized("SAME");

	TCanvas* canRzgamma = new TCanvas("canRzgamma","canRzgamma",500,500);
	//ZmumuHT->Scale(1./ZmumuHT->Integral());
	//GammaHT->Scale(1./GammaHT->Integral());
	TH1F* ratio = new TH1F(*ZmumuMHT);
	ratio->Divide(GammaMHT);
	ratio->SetMarkerStyle(8);
	ratio->Draw("E");
}