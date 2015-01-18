#include "stdPhotonControlPlots.C"
#include "TString.h"
#include <vector>
#include "TH1F.h"
#include "THStack.h"

using namespace std;

vector< THStack* > stackedControlPlots(){
	
	vector<TString> bkgSample;
	vector<double>  bkgWeight;
	vector<TString> fileName;
	vector<int>     color;

	bkgSample.push_back("GJets_600HTinf"); 
	bkgWeight.push_back(1.0435);
	fileName.push_back("rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.GJets_HT-400to600_Tune4C_13TeV-madgraph-tauola*.root");
	color.push_back(kAzure-9);

	bkgSample.push_back("GJets_400HT600"); 
	bkgWeight.push_back(3.1025);
	fileName.push_back("rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.GJets_HT-600toInf_Tune4C_13TeV-madgraph-tauola*.root");
	color.push_back(kAzure-9);

	vector< vector<TH1F*> > bkgControlPlots;
	for( unsigned int i = 0 ; i < bkgSample.size() ; i++ ){
		bkgControlPlots.push_back( stdPhotonControlPlots(fileName[i],color[i]) );
	}

	cout << "got control plots for each bkg sample" << endl;

	vector<	THStack* > stackedControlPlots;

	cout << "initialized all THStacks" << endl;

	/*
	if( bkgControlPlots.size() <= 1 ){
		cout << "only one set of control plots... no need to stack!" << endl;
		return NULL;
	}
	*/

	for( unsigned int i = 0 ; i < bkgControlPlots.size() ; i++ ){

		//cout << "i : " << i << " size: " << bkgControlPlots[i].size() << endl;
		for( unsigned int j = 0 ; j < bkgControlPlots[i].size() ; j++ ){
			if( i == 0 ){
				cout << bkgControlPlots[0][j]->GetTitle() << endl;
				stackedControlPlots.push_back( new THStack("control plot",bkgControlPlots[0][j]->GetName()) );
			}
			//cout << "j: " << j << endl;
			bkgControlPlots[i][j]->Scale(bkgWeight[i]);
			bkgControlPlots[i][j]->SetLineColor(color[i]);
			stackedControlPlots[j]->Add( bkgControlPlots[i][j] ) ;

		}// loop of different control plots
	}// loop over sets of control plots for different bkg samples

	stackedControlPlots[0]->Draw();

	TCanvas* can = new TCanvas("can","can",1200,900);
	can->Divide(4,3);
	for( int i = 1 ; i <= 12 ; i++ ){
		can->cd(i);
		stackedControlPlots[i-1]->Draw();
	}

	TCanvas* can_CutFlow = new TCanvas("can_CutFlow","can_CutFlow",500,500);
	stackedControlPlots.back()->Draw();

	return stackedControlPlots;

}