
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "TPad.h"
#include "TH2F.h"
#include "TF1.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TLorentzVector.h"
#include <iostream>
#include "TDirectory.h"
#include "photon.cc"
#include "TCanvas.h"

// returns efficiency and statistical error on efficiency
// pair<double,double> 
void photonAcceptance(double ptCutLow = 100. ,
					  double ptCutHigh = 99999. ,
					  double etaCutLow = 0. ,
					  double etaCutHigh = 2.4 ,
					  double HTcutLow = 500. , 
					  double HTcutHigh = 99999. ){

	TChain* t400 = new TChain("TreeFiller/AnalysisTree");
	t400->Add("rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.GJets_HT-400to600_Tune4C_13TeV*root");

	TChain* t600 = new TChain("TreeFiller/AnalysisTree");
	t600->Add("rootFiles_13TeV_25ns20PU/13TeV_25ns20PU.GJets_HT-600toInf_Tune4C_13TeV*root");

	vector< TChain* > tree;
	tree.push_back( t400 );
	tree.push_back( t600 );

	vector< double > weight ;
	weight.push_back( 0.0691 );
	weight.push_back( 0.0249 );

	double effNum[3][10] = {{0.}} ;
	double effNumError[3][10] = {{0.}} ;
 	double effDenom[3][10] = {{0.}} ;
	double effDenomError[3][10] = {{0.}} ;

	for( unsigned int iSample = 0 ; iSample < tree.size() ; iSample++ ){

		cout << "iSample: " << iSample << endl;

		photon photons( tree[ iSample ] ) ;

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

		vector<double>* genPhotonPt  = 0;
		vector<double>* genPhotonEta = 0;
		vector<double>* genPhotonPhi = 0;
		vector<double>* genPhotonE   = 0;

		tree[iSample]->SetBranchAddress("fixedGridRhoFastjetAll",&rho);
		//tree[iSample]->SetBranchAddress("MHT_MHTJets",&MHT);
		//tree[iSample]->SetBranchAddress("HT_HTJets",&HT);
		//tree[iSample]->SetBranchAddress("NJets_HTJets",&NJets);

		tree[iSample]->SetBranchAddress("pt_HTJets",  &pt_HTJets);
		tree[iSample]->SetBranchAddress("eta_HTJets", &eta_HTJets);
		tree[iSample]->SetBranchAddress("phi_HTJets", &phi_HTJets);
		tree[iSample]->SetBranchAddress("mass_HTJets",&mass_HTJets);

		tree[iSample]->SetBranchAddress("pt_MHTJets",  &pt_MHTJets);
		tree[iSample]->SetBranchAddress("eta_MHTJets", &eta_MHTJets);
		tree[iSample]->SetBranchAddress("phi_MHTJets", &phi_MHTJets);
		tree[iSample]->SetBranchAddress("mass_MHTJets",&mass_MHTJets);

		tree[iSample]->SetBranchAddress("genPhotonPt", &genPhotonPt);
		tree[iSample]->SetBranchAddress("genPhotonEta", &genPhotonEta);
		tree[iSample]->SetBranchAddress("genPhotonPhi", &genPhotonPhi);
		tree[iSample]->SetBranchAddress("genPhotonE", &genPhotonE);

		double w  = weight[ iSample ];

		for( int i = 0 ; i < tree[iSample]->GetEntries() ; i++ ){

			if( i % 100000 == 0 ){
				cout << "event: " << i << endl;
			}
			tree[iSample]->GetEntry(i);
			//photons.getPhotonsFromEvent(i);

			bool leadingFound = false;
			int genMatchedPhoton = -1. ;

			// find gen-matched photon
			for( unsigned int iPh = 0 ; iPh < photons.pt->size() ; iPh++ ){
				if( photons.isGenMatched->at(iPh) == 1 ) genMatchedPhoton = iPh ;
			}

			if( genMatchedPhoton == -1 ) continue ;

			// grab jets from tree, clean gen-matched photon,
			// and compute HT from HTJets
			//  = = = = = = = = = = = =
			HT = 0. ; 
			NJets = 0;
			//cout << "compute HT" << endl;
			for( unsigned int iJet = 0 ; iJet < pt_HTJets->size() ; iJet++ ){

				if( sqrt( pow( eta_HTJets->at( iJet ) - photons.eta->at(genMatchedPhoton) , 2 ) + pow( phi_HTJets->at( iJet ) - photons.phi->at(genMatchedPhoton) , 2 ) ) < 0.3 )
					continue;
				// delta-R check
				
				HT += pt_HTJets->at( iJet ) ;
				NJets ++ ;
			}

			// grab jets from tree, clean gen-matched photon,
			// and compute HT from HTJets
			//  = = = = = = = = = = = =
			TLorentzVector MHTvector;
			MHTvector.SetPtEtaPhiM(0.,0.,0.,0.);
			//cout << "compute MHT" << endl;
			for( unsigned int iJet = 0 ; iJet < pt_MHTJets->size() ; iJet++ ){

				if( sqrt( pow( eta_MHTJets->at( iJet ) - photons.eta->at(genMatchedPhoton) , 2 ) + pow( phi_MHTJets->at( iJet ) - photons.phi->at(genMatchedPhoton) , 2 ) ) < 0.3 )
					continue;
				// delta-R check
				
				TLorentzVector temp;
				temp.SetPtEtaPhiM( pt_MHTJets->at( iJet ) , 
								   eta_MHTJets->at( iJet ) ,
								   phi_MHTJets->at( iJet ) ,
								   mass_MHTJets->at( iJet ) ) ;
				MHTvector -= temp ;
				
			}
			MHT = MHTvector.Pt();
			//  = = = = = = = = = = = =		

			/*
			cout << "MHT: " << MHT << endl;
			cout << "HT: " << HT << endl;
			cout << "NJets: " << NJets << endl;
			*/

			if( NJets < 3 ) continue;

			bool genMatchedFound = false;

			//cout << "loop over photons" << endl;
			for( unsigned int iPh = 0 ; iPh < photons.pt->size() ; iPh++ ){

				if( genMatchedFound ) break;

				if( photons.pt->at(iPh) > ptCutLow && 
					photons.pt->at(iPh) < ptCutHigh && 
					fabs(photons.eta->at(iPh)) > etaCutLow && 
					fabs(photons.eta->at(iPh)) < etaCutHigh && 
					HT > HTcutLow && HT < HTcutHigh 					
					){

//					cout << "isGenMatched: " << photons.isGenMatched->at(iPh) << endl;
//					cout << "deltaR with gen photon: " << sqrt( pow( photons.phi->at(iPh) - genPhotonPhi->at(0) , 2 ) +
//																pow( photons.eta->at(iPh) - genPhotonEta->at(0) , 2 ) ) << endl;

					if( photons.isGenMatched->at(iPh) ){

						genMatchedFound = true;

						if( MHT > 200 && MHT < 300 ){
							//cout << "check - 200 - 300" << endl;
							if( NJets >= 12 ){
								effNum[0][9] += w;
								effNumError[0][9]++;
								/*
								cout << "pt: " << photons.pt->at(iPh) << endl;
								cout << "eta: " << photons.eta->at(iPh) << endl;
								cout << "HT: " << HT << endl;
								cout << "MHT: " << MHT << endl;
								cout << "NJets: " << NJets << endl;
								*/
							}else{
								effNum[0][NJets-3] += w;
								effNumError[0][NJets-3]++;
							}
						}else if( MHT > 300 && MHT < 450 ){
							//cout << "check - 300 - 450" << endl;
							if( NJets >= 12 ){
								effNum[1][9] += w;
								effNumError[1][9]++;
								/*
								cout << "pt: " << photons.pt->at(iPh) << endl;
								cout << "eta: " << photons.eta->at(iPh) << endl;
								cout << "HT: " << HT << endl;
								cout << "MHT: " << MHT << endl;
								cout << "NJets: " << NJets << endl;
								*/
							}else{
								effNum[1][NJets-3] += w;
								effNumError[1][NJets-3]++;
							}
						}else if( MHT > 450){
							//cout << "check - 450" << endl;
							if( NJets >= 12 ){
								effNum[2][9] += w;
								effNumError[2][9]++;
								/*
								cout << "pt: " << photons.pt->at(iPh) << endl;
								cout << "eta: " << photons.eta->at(iPh) << endl;
								cout << "HT: " << HT << endl;
								cout << "MHT: " << MHT << endl;
								cout << "NJets: " << NJets << endl;
								*/
							}else{
								effNum[2][NJets-3] += w;
								effNumError[2][NJets-3]++;
							}
						}
						if( !leadingFound ){
							leadingFound = true;
						}
					}//else cout << "check - doesn't pass ID/ISO cuts" << endl;

					if( MHT > 200 && MHT < 300 ){
						if( NJets >= 12 ){
							effDenom[0][9] += w;
							effDenomError[0][9]++;
						}else{
							effDenom[0][NJets-3] += w;
							effDenomError[0][NJets-3]++;
						}
					}else if( MHT > 300 && MHT < 450 ){
						if( NJets >= 12 ){
							effDenom[1][9] += w;
							effDenomError[1][9]++;
						}else{
							effDenom[1][NJets-3] += w;
							effDenomError[1][NJets-3]++;
						}
					}else if( MHT > 450){
						if( NJets >= 12 ){
							effDenom[2][9] += w;
							effDenomError[2][9]++;
						}else{
							effDenom[2][NJets-3] += w;
							effDenomError[2][NJets-3]++;
						}
					}
				}// end if block for kinematic cuts

			}// end loop over photons
		}// end loop over events

	}// end loop over samples

	double njets[10] = {3,4,5,6,7,8,9,10,11,12} ;
	double njetsError[10] = {0.};
	double effic[3][10],efficError[3][10];

	for(int i = 0 ; i < 10 ; i++ ){

		cout << effNum[0][i] << "  " << effDenom[0][i] << endl;

		effic[0][i] = effNum[0][i] / effDenom[0][i] ;
		efficError[0][i] = sqrt(1/effNumError[0][i] + 1/effDenomError[0][i])*effNum[0][i]/effDenom[0][i] ;

		effic[1][i] = effNum[1][i] / effDenom[1][i] ;
		efficError[1][i] = sqrt(1/effNumError[1][i] + 1/effDenomError[1][i])*effNum[1][i]/effDenom[1][i] ;

		effic[2][i] = effNum[2][i] / effDenom[2][i] ;
		efficError[2][i] = sqrt(1/effNumError[2][i] + 1/effDenomError[2][i])*effNum[2][i]/effDenom[2][i] ;

	}

	TGraphErrors* MHT200 = new TGraphErrors(10,njets,effic[0],njetsError,efficError[0]);
	MHT200->SetMarkerStyle(8);
	MHT200->SetMarkerColor(1);
	TGraphErrors* MHT300 = new TGraphErrors(10,njets,effic[1],njetsError,efficError[1]);
	MHT300->SetMarkerStyle(8);
	MHT300->SetMarkerColor(4);
	TGraphErrors* MHT450 = new TGraphErrors(10,njets,effic[2],njetsError,efficError[2]);
	MHT450->SetMarkerStyle(8);
	MHT450->SetMarkerColor(6);

	TF1* poly[3] = { new TF1("poly_MHT200","[0]+x*[1]+x*x*[2]",2.5,10.5),
					 new TF1("poly_MHT300","[0]+x*[1]+x*x*[2]",2.5,10.5),
					 new TF1("poly_MHT450","[0]+x*[1]+x*x*[2]",2.5,10.5) };

	poly[0]->SetLineColor(1);
	poly[1]->SetLineColor(4);
	poly[2]->SetLineColor(6);

	MHT200->Fit("poly_MHT200","R");
	MHT300->Fit("poly_MHT300","R");
	MHT450->Fit("poly_MHT450","R");

	MHT200->GetYaxis()->SetRangeUser(0.,1.);
	MHT200->GetXaxis()->SetTitle("n_{j}");
	MHT200->GetYaxis()->SetTitle("Acceptance Efficiency");


	MHT200->GetXaxis()->SetRangeUser(2.5,10.5);

	TCanvas* can = new TCanvas("can","can",500,500);
	can->cd();

	MHT200->Draw("Ap");
	MHT300->Draw("sameP");
	MHT450->Draw("sameP");

	TLegend* leg = new TLegend(.2,.9,.5,.6);
	leg->SetBorderSize(0);
	leg->SetFillColor(0);

	leg->AddEntry(MHT200,"200<H_{T}^{miss}<300","lp");
	leg->AddEntry(MHT300,"300<H_{T}^{miss}<450","lp");
	leg->AddEntry(MHT450,"450<H_{T}^{miss}","lp");

	leg->Draw();

}
