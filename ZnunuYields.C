#include "TChain.h"
#include "TH1F.h"
#include "TFile.h"
#include <iostream>

using namespace std;

struct cut{

	std::pair<double,double> HT;
	std::pair<double,double> MHT;
	std::pair<double,double> NJets;

};

void ZnunuYields(){
	
	TChain* ZJets400 = new TChain("TreeFiller/AnalysisTree");
	ZJets400->Add("rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.ZJetsToNuNu_HT-400to600_Tune4C_13TeV-madgraph-tauola*");
	TChain* ZJets600 = new TChain("TreeFiller/AnalysisTree");
	ZJets600->Add("rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.ZJetsToNuNu_HT-600toInf_Tune4C_13TeV-madgraph-tauola*");

	vector< TChain* > tree;
	tree.push_back( ZJets400 );
	tree.push_back( ZJets600 );

	vector< double > weight ;
	weight.push_back( 0.0134600803653239 );
	weight.push_back( 0.0045237044090775 );

	cut bin[36];
	double yields[36] = {0.};
	int counts[36] = {0};

	TH1F* binYields = new TH1F("binYields","binYields",36,0,36);
	binYields->Sumw2();
	double htBins[6] = {500,800,1000,1250,1500,99999};
	TH1F* htYields = new TH1F("htYields","htYields",20,500,2500);
	htYields->Sumw2();
	double mhtBins[6] = {200,300,450,600,99999};
	TH1F* mhtYields = new TH1F("mhtYields","mhtYields",18,200,2000);
	mhtYields->Sumw2();
	TH1F* njetsLowMHTYields = new TH1F("njetsLowMHTYields","njetsLowMHTYields",10,2.5,12.5);
	njetsLowMHTYields->Sumw2();
	TH1F* njetsMedMHTYields = new TH1F("njetsMedMHTYields","njetsMedMHTYields",10,2.5,12.5);
	njetsMedMHTYields->Sumw2();
	TH1F* njetsHighMHTYields = new TH1F("njetsHighMHTYields","njetsHighMHTYields",10,2.5,12.5);
	njetsHighMHTYields->Sumw2();
	
	bin[0].NJets.first=3 ; bin[0].NJets.second=5 ; 
	bin[0].HT.first=500 ; bin[0].HT.second=800 ; 
	bin[0].MHT.first=200 ; bin[0].MHT.second=300 ; 
	bin[1] = bin[0] ; bin[1].MHT.first=300 ; bin[1].MHT.second=450 ;
	bin[2] = bin[0] ; bin[2].MHT.first=450 ; bin[2].MHT.second=600 ;
	bin[3] = bin[0] ; bin[3].MHT.first=600 ; bin[3].MHT.second=99999 ;

	bin[4] = bin[0] ; bin[4].HT.first=800 ;  bin[4].HT.second=1000 ;
	bin[5] = bin[4] ; bin[5].MHT.first=300 ; bin[5].MHT.second=450 ;
	bin[6] = bin[4] ; bin[6].MHT.first=450 ; bin[6].MHT.second=600 ;
	bin[7] = bin[4] ; bin[7].MHT.first=600 ; bin[7].MHT.second=99999 ;

	bin[8]  = bin[0] ; bin[8].HT.first=1000 ;  bin[8].HT.second=1250 ;
	bin[9]  = bin[8] ; bin[9].MHT.first=300 ;  bin[9].MHT.second=450 ;
	bin[10] = bin[8] ; bin[10].MHT.first=450 ; bin[10].MHT.second=600 ;
	bin[11] = bin[8] ; bin[11].MHT.first=600 ; bin[11].MHT.second=99999 ;

	bin[12]  = bin[0]  ; bin[12].HT.first=1250 ;  bin[12].HT.second=1500 ;
	bin[13]  = bin[12] ; bin[13].MHT.first=300 ;  bin[13].MHT.second=450 ;
	bin[14]  = bin[12] ; bin[14].MHT.first=450 ;  bin[14].MHT.second=99999 ;

	bin[15]  = bin[0]  ; bin[15].HT.first=1500 ;  bin[15].HT.second=99999 ;
	bin[16]  = bin[15] ; bin[16].MHT.first=300 ;  bin[16].MHT.second=99999 ;

	for( unsigned int iSample = 0 ; iSample < tree.size() ; iSample++ ){

		cout << "iSample: " << iSample << endl;

		double rho;
		double MHT,HT=0.;
		int NJets=0.;
		vector<double>* pt_HTJets = 0;
		vector<double>* eta_HTJets = 0;
		vector<double>* phi_HTJets = 0;
		vector<double>* mass_HTJets = 0;
		
		vector<double>* pt_MHTJets = 0;
		vector<double>* eta_MHTJets = 0;
		vector<double>* phi_MHTJets = 0;
		vector<double>* mass_MHTJets = 0;

		tree[iSample]->SetBranchAddress("HT_HTJets",     &HT);
		tree[iSample]->SetBranchAddress("MHT_MHTJets",   &MHT);
		tree[iSample]->SetBranchAddress("NJets_HTJets",  &NJets);

		tree[iSample]->SetBranchAddress("pt_HTJets",  &pt_HTJets);
		tree[iSample]->SetBranchAddress("eta_HTJets", &eta_HTJets);
		tree[iSample]->SetBranchAddress("phi_HTJets", &phi_HTJets);
		tree[iSample]->SetBranchAddress("mass_HTJets",&mass_HTJets);

		tree[iSample]->SetBranchAddress("pt_MHTJets",  &pt_MHTJets);
		tree[iSample]->SetBranchAddress("eta_MHTJets", &eta_MHTJets);
		tree[iSample]->SetBranchAddress("phi_MHTJets", &phi_MHTJets);
		tree[iSample]->SetBranchAddress("mass_MHTJets",&mass_MHTJets);


		for( int i = 0 ; i < tree[iSample]->GetEntries() ; i++ ){

			if( i % 100000 == 0 ){
				cout << "event: " << i << endl;
			}
			tree[iSample]->GetEntry(i);

			for( int iCut = 0 ; iCut < 36 ; iCut++ ){
				if( HT >= bin[iCut].HT.first && HT <= bin[iCut].HT.second &&
					MHT >= bin[iCut].MHT.first && MHT <= bin[iCut].MHT.second &&
					NJets >= bin[iCut].NJets.first && NJets <= bin[iCut].NJets.second ){

					yields[iCut]+=weight[iSample];

				}
				
				if(NJets>=3 && MHT>200)
					htYields->Fill(HT,weight[iSample]);
				if(NJets>=3 && HT>500)
					mhtYields->Fill(MHT,weight[iSample]);
				if(MHT>200 && MHT<300 && HT >500)
					njetsLowMHTYields->Fill(NJets,weight[iSample]);
				if(MHT>300 && MHT<450 && HT>500)
					njetsMedMHTYields->Fill(NJets,weight[iSample]);
				if(MHT>450 && HT>500)
					njetsHighMHTYields->Fill(NJets,weight[iSample]);

			}

		}// end loop over entries

	}// end loop over samples

	for( int iCut = 0 ; iCut < 36 ; iCut++ ){
		cout << "yield in bin " << iCut << ": " << yields[iCut] << endl;
		binYields->SetBinContent(iCut,yields[iCut]);
	}	


	TFile* file = new TFile("ZJetsYields.root","RECREATE");
	binYields->Write();
	htYields->Write();
	mhtYields->Write();
	njetsLowMHTYields->Write();
	njetsMedMHTYields->Write();
	njetsHighMHTYields->Write();
	file->Close();

}
