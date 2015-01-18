#include "TTree.h"
#include <vector>
#include <iostream>
#include "effArea.cc"

using namespace std;

class photon{

public:

	TTree* t;
	int num;
	vector<double> *pt;
	vector<double> *eta;
	vector<double> *phi;
	vector<double> *e;
	vector<double> *isEB;
	vector<double> *isGenMatched;
	vector<double> *hadTowOverEm,*sigmaIetaIeta;
	vector<double> *pfChargedIso,*pfNeutralIso,*pfGammaIso;
	vector<double> *ConeDR03ecalEtIso,*ConeDR03hcalEtIso,*ConeDR03trkPtIso;
	vector<bool> *passElectronConvVeto, *hasPixelSeed;
	effArea* effAreas;

	photon(TTree* t_){

		t = t_;

		effAreas = new effArea();
		effAreas->addEffA( 0.0,   1.0,   0.012, 0.030, 0.148 );
		effAreas->addEffA( 1.0,   1.479, 0.010, 0.057, 0.130 );
		effAreas->addEffA( 1.479, 2.0,   0.014, 0.039, 0.112 );
		effAreas->addEffA( 2.0,   2.2,   0.012, 0.015, 0.216 );
		effAreas->addEffA( 2.2,   2.3,   0.016, 0.024, 0.262 );
		effAreas->addEffA( 2.3,   2.4,   0.020, 0.039, 0.260 );
		effAreas->addEffA( 2.4,   99.,   0.012, 0.072, 0.266 );
		
		//cout << "copied TTree pointer, zeroing vectors" << endl;
		isEB = 0 ; isGenMatched = 0 ;
		pt = 0 ; eta = 0 ; phi = 0 ; e = 0;
		hadTowOverEm = 0 ; sigmaIetaIeta = 0 ;
		pfChargedIso = 0 ; pfNeutralIso = 0 ; pfGammaIso = 0 ;
		ConeDR03ecalEtIso = 0 ; ConeDR03hcalEtIso = 0 ; ConeDR03trkPtIso = 0 ;
		passElectronConvVeto = 0 ; hasPixelSeed = 0 ;

		//t->SetBranchAddress( "NPhotons" , &num );

		//cout << "set NPhotons" << endl;

		t->SetBranchAddress( "photonPt" , &pt );
		t->SetBranchAddress( "photonEta" , &eta );
		t->SetBranchAddress( "photonPhi" , &phi );
		t->SetBranchAddress( "photonE" , &e );

		//cout << "set kinematics" << endl;

		t->SetBranchAddress( "photon_isEB" , &isEB );
		t->SetBranchAddress( "photon_genMatched" , &isGenMatched );

		//cout << "set fiducial vars" << endl;

		t->SetBranchAddress( "photon_hadTowOverEm" , &hadTowOverEm );
		t->SetBranchAddress( "photon_sigmaIetaIeta" , &sigmaIetaIeta );

		//cout << "set shower vars" << endl;
		
		t->SetBranchAddress( "photon_pfChargedIso" , &pfChargedIso );
		t->SetBranchAddress( "photon_pfNeutralIso" , &pfNeutralIso );
		t->SetBranchAddress( "photon_pfGammaIso" , &pfGammaIso );
			
		//cout << "set pf isolation vars" << endl;

		t->SetBranchAddress( "photon_ConeDR03ecalEtIso" , &ConeDR03ecalEtIso );
		t->SetBranchAddress( "photon_ConeDR03hcalEtIso" , &ConeDR03hcalEtIso );
		t->SetBranchAddress( "photon_ConeDR03trkPtIso" , &ConeDR03trkPtIso );

		//cout << "set cone isolation vars" << endl;

		t->SetBranchAddress( "photon_passElectronConvVeto" , &passElectronConvVeto );
		t->SetBranchAddress( "photon_hasPixelSeed" , &hasPixelSeed );

		//cout << "set vetos" << endl;

	};

	void deletePhoton(int iPhoton ){

		//cout << "deletePhoton" << endl;
		//cout << "iPhoton: " << iPhoton << " will be deleted" << endl;
		isEB->erase( isEB->begin() + iPhoton );
		isGenMatched->erase( isGenMatched->begin() + iPhoton );
		pt->erase( pt->begin() + iPhoton );
		eta->erase( eta->begin() + iPhoton );
		phi->erase( phi->begin() + iPhoton );
		e->erase( e->begin() + iPhoton );
		hadTowOverEm->erase( hadTowOverEm->begin() + iPhoton );
		sigmaIetaIeta->erase( sigmaIetaIeta->begin() + iPhoton );
		pfChargedIso->erase( pfChargedIso->begin() + iPhoton );
		pfNeutralIso->erase( pfNeutralIso->begin() + iPhoton );
		pfGammaIso->erase( pfGammaIso->begin() + iPhoton );
		ConeDR03ecalEtIso->erase( ConeDR03ecalEtIso->begin() + iPhoton );
		ConeDR03hcalEtIso->erase( ConeDR03hcalEtIso->begin() + iPhoton );
		ConeDR03trkPtIso->erase( ConeDR03trkPtIso->begin() + iPhoton );
		passElectronConvVeto->erase( passElectronConvVeto->begin() + iPhoton );
		hasPixelSeed->erase( hasPixelSeed->begin() + iPhoton );

	};

	bool passIDcuts( int iEvent , int iPhoton ){

		//cout << "photon::passIDcuts" << endl;

		getPhotonsFromEvent( iEvent );
		
		if( isEB->at(iPhoton) ){

			if( hasPixelSeed->at(iPhoton) == 1 ) return false;
			if( hadTowOverEm->at(iPhoton) > 0.05 ) return false;
			if( sigmaIetaIeta->at(iPhoton) > 0.011 ) return false;

		}else{

			if( hasPixelSeed->at(iPhoton) == 1 ) return false;
			if( hadTowOverEm->at(iPhoton) > 0.05 ) return false;
			if( sigmaIetaIeta->at(iPhoton) > 0.031 ) return false;

		}

		return true;

	};


	bool passIsoCuts( int iEvent , int iPhoton , double rho ){

		getPhotonsFromEvent( iEvent );

		double chIso = effAreas->rhoCorrectedIso( pfCh , pfChargedIso->at(iPhoton) , eta->at(iPhoton) , rho );
		double nuIso = effAreas->rhoCorrectedIso( pfNu , pfChargedIso->at(iPhoton) , eta->at(iPhoton) , rho );
		double gamIso = effAreas->rhoCorrectedIso( pfGam , pfChargedIso->at(iPhoton) , eta->at(iPhoton) , rho );

		if( isEB ){
		
			if( chIso > 0.7 ) return false;
			if( nuIso > 0.4 + 0.04*pt->at(iPhoton) ) return false;
			if( gamIso > 0.5 + 0.005*pt->at(iPhoton) ) return false;
		
		}else{

			if( chIso > 0.5 ) return false;
			if( nuIso > 1.5 + 0.04*pt->at(iPhoton) ) return false;
			if( gamIso > 1.0 + 0.005*pt->at(iPhoton) ) return false;

		}

		return true;

	};

	void getPhotonsFromEvent(int iEvent , bool print = false ){

		//cout << "photon::getPhotonsFromEvent" << endl;

		t->GetEntry( iEvent );

		//cout << "got entry" << endl;

		// for some reason there is a problem with this... maybe NPhotons is not really being filled?
		/*
		if( num != pt->size() ){
			cout << "num: " << num << " pt size: " << pt->size() << endl;
			return;
		}
		*/

		if( print ){

			cout << "printing..." << endl;



		}

	};

	void printPhotons(int iEvent){

		getPhotonsFromEvent(iEvent);
		printPhotons();

	};
	
	void printPhotons(){

		cout << "============" << endl;
		cout << "num photons: " << pt->size() << endl;
		cout << "============" << endl;	

		for( unsigned int i = 0 ; i < pt->size() ; i++ ){
			cout << "iPhoton: " << i << endl;
			cout << "pt: " << pt->at(i) << endl;	
			cout << "eta: " << eta->at(i) << endl;
			cout << "phi: " << phi->at(i) << endl;
			cout << "energy: " << e->at(i) << endl;
			cout << " - - - - - - " << endl;
		}// end loop over photon candidates

	};
	

};


