#include "Zmass.C"
#include "TCanvas.h"
#include "TH1F.h"
#include "TString.h"
#include "THStack.h"


vector< THStack* > stackZmumuControlPlots(){
	
	vector<TString> bkgSample;
	vector<double>  bkgWeight;
	vector<TString> fileName;
	vector<int>     color;

	bkgSample.push_back("DYJetsToLL_600HTinf"); 
	bkgWeight.push_back(2.179);
	fileName.push_back("DYJetsToLL_M-50_HT-600toinf_Tune4C_13TeV-madgraph-tauola_SumJetMass_AnalysisTree.root");
	color.push_back(kAzure-9);

	bkgSample.push_back("DYJetsToLL_400to600"); 
	bkgWeight.push_back(6.546);
	fileName.push_back("DYJetsToLL_M-50_HT-400to600_Tune4C_13TeV-madgraph-tauola_SumJetMass_AnalysisTree.root");
	color.push_back(kAzure+4);

	vector< vector<TH1F*> > bkgControlPlots;
	vector< THStack* > stackedControlPlots;
	for( unsigned int iBkg = 0 ; iBkg < bkgSample.size() ; iBkg++ ){

		bkgControlPlots.push_back( Zmass( fileName[ iBkg ] , color[ iBkg ] ) );

		for( unsigned int iHisto = 0 ; iHisto < bkgControlPlots.back().size() ; iHisto++ ){

			if( iBkg == 0 ){
				stackedControlPlots.push_back( new THStack( bkgControlPlots[ iBkg ][ iHisto ]->GetName() , bkgControlPlots[ iBkg ][ iHisto ]->GetTitle() ) ) ;
			}

			bkgControlPlots[ iBkg ][ iHisto ]->Scale( bkgWeight[ iBkg ] );
			stackedControlPlots[ iHisto ]->Add( bkgControlPlots[ iBkg ][ iHisto ] );

		} 

	}

	TCanvas* can = new TCanvas("canZmumu","Z#to#mu#mu Control Plots",1200,800);
	can->Divide(3,2);
	for( unsigned int iHisto = 0 ; iHisto < stackedControlPlots.size() ; iHisto++ ){

		can->cd(iHisto+1);
		stackedControlPlots[iHisto]->Draw();

	}

	return stackedControlPlots;

}