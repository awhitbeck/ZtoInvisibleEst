
// isoComparison("photon_ConeDR03hcalEtIso","Cone Iso_{nu}","photon_ConeDR03hcalEtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_ConeDR03ecalEtIso","Cone Iso_{ph}","photon_ConeDR03ecalEtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_ConeDR03trkPtIso","Cone Iso_{ch}","photon_ConeDR03trkPtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_sigmaIetaIeta","#sigma_{i#etai#eta}","photonPt>100&&photon_isEB==1&&photon_hadTowOverEm<0.05")
// isoComparison("photon_hadTowOverEm","H/E","photon_hadTowOverEm<0.1&&photonPt>100&&photon_isEB==1")
// isoComparison("photon_hasPixelSeed","Pixel Seed","photonPt>100&&photon_isEB==1")

#include "photon.cc"
#include "TTree.h"
#include "TFile.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TPad.h"
#include "TCanvas.h"
#include <vector>
#include <iostream>

using namespace std;

photon* passID(photon myPhotons){

	if( myPhotons.isEB->size() == 0 ){
		//cout << "ERROR: no photons!" << endl;
		return 0;
	}
	int j = 0;
	//cout << "barrel selection" << endl;
	//cout << "isEB size: " << myPhotons.isEB->size() << endl;
	while( myPhotons.isEB->size() > 0 && j < myPhotons.isEB->size() ){
		//cout << "j: " << j << " isEB size: " << myPhotons.isEB->size() << endl;
		if( myPhotons.isEB->at(j) == 0.){
				//cout << "failed cut" << endl;
				myPhotons.deletePhoton(j);
				//cout << "sanity isEB size: " << myPhotons.isEB->size() << endl;
			}
			else 
				j++;
		}
		j = 0;
		//cout << "pt selection" << endl;
		while( myPhotons.pt->size() > 0 && j < myPhotons.pt->size() ){
			if( myPhotons.pt->at(j) < 100.)
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		j = 0;
		//cout << "eta selection" << endl;
		while( myPhotons.eta->size() > 0 && j < myPhotons.eta->size() ){
			if( fabs( myPhotons.eta->at(j) ) > 2.4)
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		j = 0;
		//cout << "pixel seed selection" << endl;
		while( myPhotons.hasPixelSeed->size() > 0 && j < myPhotons.hasPixelSeed->size() ){
			if( myPhotons.hasPixelSeed->at(j) == 1)
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		j = 0;
		//cout << "H/E selection" << endl;
		while( myPhotons.hadTowOverEm->size() > 0 && j < myPhotons.hadTowOverEm->size()){
			if( myPhotons.hadTowOverEm->at(j) > 0.05)
				myPhotons.deletePhoton(j);
			else
				j++;
		}
		j = 0;
		//cout << "sigmaIetaIeta selection" << endl;
		while( myPhotons.sigmaIetaIeta->size() > 0 && j < myPhotons.sigmaIetaIeta->size() ){
			if( myPhotons.sigmaIetaIeta->at(j) > 0.011)
				myPhotons.deletePhoton(j);
			else
				j++;
		}

		return &myPhotons;

	}

void isoComparison(isoType iso_ = pfCh,
				   TString yAxisTitle = "Cone Iso_{ch}",
				   TString cutString = "photon_ConeDR03trkPtIso<25&&photonPt>100&&photon_sigmaIetaIeta<0.011&&photon_isEB==1&&photon_hadTowOverEm<0.05" ){

	TFile* GJets_8TeV_file = new TFile("GJets_HT-400ToInf_8TeV_LPCSUSYPAT_SumJetMass_AnalysisTree.root","READ");
	TFile* GJets_400HT600_13TeV_file = new TFile("GJets_400HT600_13TeV_PU_S14_SumJetMass_AnalysisTree.root","READ");
	TFile* GJets_600HTinf_13TeV_file = new TFile("GJets_600HTinf_13TeV_PU_S14_SumJetMass_AnalysisTree.root","READ");

	TTree* GJets_8TeV_tree = (TTree*) GJets_8TeV_file->Get("TreeFiller/AnalysisTree");
	TTree* GJets_400HT600_13TeV_tree = (TTree*) GJets_400HT600_13TeV_file->Get("TreeFiller/AnalysisTree");
	TTree* GJets_600HTinf_13TeV_tree = (TTree*) GJets_600HTinf_13TeV_file->Get("TreeFiller/AnalysisTree");

	TH1F* hIso_GJets_8TeV = new TH1F("hIso_GJets_8TeV","hIso_GJets_8TeV",50,0,5);
	TH1F* hIso_GJets_400HT600_13TeV = new TH1F("hIso_GJets_400HT600_13TeV","hIso_GJets_400HT600_13TeV",50,0,5);
	TH1F* hIso_GJets_600HTinf_13TeV = new TH1F("hIso_GJets_600HTinf_13TeV","hIso_GJets_600HTinf_13TeV",50,0,5);

	// effective areas:
	effArea effAreas( 0.0,   1.0,   0.012, 0.030, 0.148 );
	effAreas.addEffA( 1.0,   1.479, 0.010, 0.057, 0.130 );
	effAreas.addEffA( 1.479, 2.0,   0.014, 0.039, 0.112 );
	effAreas.addEffA( 2.0,   2.2,   0.012, 0.015, 0.216 );
	effAreas.addEffA( 2.2,   2.3,   0.016, 0.024, 0.262 );
	effAreas.addEffA( 2.3,   2.4,   0.020, 0.039, 0.260 );
	effAreas.addEffA( 2.4,   99.,   0.012, 0.072, 0.266 );

	photon photons_8TeV(GJets_8TeV_tree);
	photon photons_400HT600_13TeV(GJets_400HT600_13TeV_tree);
	photon photons_600HTinf_13TeV(GJets_600HTinf_13TeV_tree);

 double rho;
 GJets_8TeV_tree->SetBranchAddress("rho" , &rho );
	
	for(int i = 0 ; i < GJets_8TeV_tree->GetEntries() ; i++ ){

		photons_8TeV.getPhotonsFromEvent(i);

		cout << passID( photons_8TeV ) << endl;

		if( passID( photons_8TeV ) ){
		 if( iso_ == pfCh)
				hIso_GJets_8TeV->Fill( effAreas.rhoCorrectedIso( pfCh , passID( photons_8TeV )->pfChargedIso->at(0) , passID( photons_8TeV )->eta->at(0) , rho ) );
		 if( iso_ == pfNu)
				hIso_GJets_8TeV->Fill( effAreas.rhoCorrectedIso( pfNu , passID( photons_8TeV )->pfNeutralIso->at(0) , passID( photons_8TeV )->eta->at(0) , rho ) );
		 if( iso_ == pfGam)
				hIso_GJets_8TeV->Fill( effAreas.rhoCorrectedIso( pfGam , passID( photons_8TeV )->pfGammaIso->at(0) , passID( photons_8TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneCh)
				hIso_GJets_8TeV->Fill( effAreas.rhoCorrectedIso( pfCh , passID( photons_8TeV )->ConeDR03trkPtIso->at(0) , passID( photons_8TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneNu)
				hIso_GJets_8TeV->Fill( effAreas.rhoCorrectedIso( pfNu , passID( photons_8TeV )->ConeDR03hcalEtIso->at(0) , passID( photons_8TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneGam)
				hIso_GJets_8TeV->Fill( effAreas.rhoCorrectedIso( pfGam , passID( photons_8TeV )->ConeDR03ecalEtIso->at(0) , passID( photons_8TeV )->eta->at(0) , rho ) );

	}
}

 GJets_400HT600_13TeV_tree->SetBranchAddress("fixedGridRhoFastjetAll" , &rho );
	
	for(int i = 0 ; i < GJets_400HT600_13TeV_tree->GetEntries() ; i++ ){

		photons_400HT600_13TeV.getPhotonsFromEvent(i);

		cout << passID( photons_400HT600_13TeV ) << endl;

		if( passID( photons_400HT600_13TeV ) ){
		 if( iso_ == pfCh)
				hIso_GJets_400HT600_13TeV->Fill( effAreas.rhoCorrectedIso( pfCh , passID( photons_400HT600_13TeV )->pfChargedIso->at(0) , passID( photons_400HT600_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == pfNu)
				hIso_GJets_400HT600_13TeV->Fill( effAreas.rhoCorrectedIso( pfNu , passID( photons_400HT600_13TeV )->pfNeutralIso->at(0) , passID( photons_400HT600_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == pfGam)
				hIso_GJets_400HT600_13TeV->Fill( effAreas.rhoCorrectedIso( pfGam , passID( photons_400HT600_13TeV )->pfGammaIso->at(0) , passID( photons_400HT600_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneCh)
				hIso_GJets_400HT600_13TeV->Fill( effAreas.rhoCorrectedIso( pfCh , passID( photons_400HT600_13TeV )->ConeDR03trkPtIso->at(0) , passID( photons_400HT600_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneNu)
				hIso_GJets_400HT600_13TeV->Fill( effAreas.rhoCorrectedIso( pfNu , passID( photons_400HT600_13TeV )->ConeDR03hcalEtIso->at(0) , passID( photons_400HT600_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneGam)
				hIso_GJets_400HT600_13TeV->Fill( effAreas.rhoCorrectedIso( pfGam , passID( photons_400HT600_13TeV )->ConeDR03ecalEtIso->at(0) , passID( photons_400HT600_13TeV )->eta->at(0) , rho ) );

	}
}

 GJets_600HTinf_13TeV_tree->SetBranchAddress("fixedGridRhoFastjetAll" , &rho );
	
	for(int i = 0 ; i < GJets_600HTinf_13TeV_tree->GetEntries() ; i++ ){

		photons_600HTinf_13TeV.getPhotonsFromEvent(i);

		cout << passID( photons_600HTinf_13TeV ) << endl;

		if( passID( photons_600HTinf_13TeV ) ){
		 if( iso_ == pfCh)
				hIso_GJets_600HTinf_13TeV->Fill( effAreas.rhoCorrectedIso( pfCh , passID( photons_600HTinf_13TeV )->pfChargedIso->at(0) , passID( photons_600HTinf_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == pfNu)
				hIso_GJets_600HTinf_13TeV->Fill( effAreas.rhoCorrectedIso( pfNu , passID( photons_600HTinf_13TeV )->pfNeutralIso->at(0) , passID( photons_600HTinf_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == pfGam)
				hIso_GJets_600HTinf_13TeV->Fill( effAreas.rhoCorrectedIso( pfGam , passID( photons_600HTinf_13TeV )->pfGammaIso->at(0) , passID( photons_600HTinf_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneCh)
				hIso_GJets_600HTinf_13TeV->Fill( effAreas.rhoCorrectedIso( pfCh , passID( photons_600HTinf_13TeV )->ConeDR03trkPtIso->at(0) , passID( photons_600HTinf_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneNu)
				hIso_GJets_600HTinf_13TeV->Fill( effAreas.rhoCorrectedIso( pfNu , passID( photons_600HTinf_13TeV )->ConeDR03hcalEtIso->at(0) , passID( photons_600HTinf_13TeV )->eta->at(0) , rho ) );
		 if( iso_ == coneGam)
				hIso_GJets_600HTinf_13TeV->Fill( effAreas.rhoCorrectedIso( pfGam , passID( photons_600HTinf_13TeV )->ConeDR03ecalEtIso->at(0) , passID( photons_600HTinf_13TeV )->eta->at(0) , rho ) );

	}
}

TCanvas* can = new TCanvas("can","can",500,500);

hIso_GJets_8TeV->SetLineWidth(2);
hIso_GJets_8TeV->SetLineColor(kGreen+2);
hIso_GJets_8TeV->SetLineStyle(1);
hIso_GJets_8TeV->GetYaxis()->SetTitle("Events (a.u.)");
hIso_GJets_8TeV->GetXaxis()->SetNdivisions(505);
gPad->SetLogy();
if( iso_ == pfCh) hIso_GJets_8TeV->GetXaxis()->SetTitle("Iso_{ch}^{PF} [GeV]");
if( iso_ == pfNu) hIso_GJets_8TeV->GetXaxis()->SetTitle("Iso_{nu}^{PF} [GeV]");
if( iso_ == pfGam) hIso_GJets_8TeV->GetXaxis()->SetTitle("Iso_{#gamma}^{PF} [GeV]");
if( iso_ == coneCh) hIso_GJets_8TeV->GetXaxis()->SetTitle("Cone Iso_{ch} [GeV]");
if( iso_ == coneNu) hIso_GJets_8TeV->GetXaxis()->SetTitle("Cone Iso_{nu} [GeV]");
if( iso_ == coneGam) hIso_GJets_8TeV->GetXaxis()->SetTitle("Cone Iso_{#gamma} [GeV]");

hIso_GJets_8TeV->DrawNormalized();
hIso_GJets_400HT600_13TeV->Scale(3.1025);
TH1F* hIso_GJets_13TeV = new TH1F( *hIso_GJets_400HT600_13TeV );
hIso_GJets_13TeV->SetLineWidth(2);
hIso_GJets_13TeV->SetLineColor(kAzure+4);
hIso_GJets_13TeV->SetLineStyle(2);
hIso_GJets_13TeV->Add( hIso_GJets_600HTinf_13TeV , 1.0435 );
hIso_GJets_13TeV->DrawNormalized("SAME");

	TLegend* leg = new TLegend(.6,.9,.9,.6);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);

	leg->AddEntry(hIso_GJets_8TeV,"8 TeV","l");
	leg->AddEntry(hIso_GJets_13TeV,"13 TeV","l");
	leg->Draw();
}

