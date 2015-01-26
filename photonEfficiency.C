
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
#include <cmath>
#include "TCanvas.h"

struct cut{

  std::pair<double,double> HT;
  std::pair<double,double> MHT;
  std::pair<double,double> NJets;

};

// returns efficiency and statistical error on efficiency
// pair<double,double> 
void photonEfficiency(double ptCutLow = 100. ,
		      double ptCutHigh = 99999. ,
		      double HTcutLow = 500. , 
		      double HTcutHigh = 99999. ){

  TChain* t400 = new TChain("TreeMaker2/PreSelection");
  t400->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.GJets_HT-400to600_Tune4C_13TeV*root");

  TChain* t600 = new TChain("TreeMaker2/PreSelection");
  t600->Add("/eos/uscms/store/user/awhitbe1/PHYS14productionV1/PHYS14.GJets_HT-600toInf_Tune4C_13TeV*root");

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

  cut bin[36];
  double yields[36] = {0.};

  /*
    TH1F* binYields = new TH1F("binYields","binYields",36,0,36);
    binYields->Sumw2();
    double htBins[6] = {500,800,1000,1250,1500,99999};
    TH1F* htYields = new TH1F("htYields","htYields",20,500,2500);
    htYields->Sumw2();
    double mhtBins[6] = {200,300,450,600,99999};
    TH1F* mhtYields = new TH1F("mhtYields","mhtYields",20,200,2000);
    mhtYields->Sumw2();
    TH1F* njetsLowMHTYields = new TH1F("njetsLowMHTYields","njetsLowMHTYields",6,2.5,8.5);
    njetsLowMHTYields->Sumw2();
    TH1F* njetsMedMHTYields = new TH1F("njetsMedMHTYields","njetsMedMHTYields",6,2.5,8.5);
    njetsMedMHTYields->Sumw2();
    TH1F* njetsHighMHTYields = new TH1F("njetsHighMHTYields","njetsHighMHTYields",6,2.5,8.5);
    njetsHighMHTYields->Sumw2(); 
  */

  TH1F* binYields = new TH1F("binYields","binYields",36,0,36);
  double htBins[20] = {550,650,750,850,950,1500,1150,1250,1350,1450,1550,1650,1750,1850,1950,2050,
		       2150,2250,2350,2450};
  double htYield[18] = {0.};
  double htYieldError[18] = {0.};
  TH1F* htYieldsHisto = new TH1F("htYieldsHisto","htYieldsHisto",20,500,2500);
  double mhtBin[18] = {250,350,450,550,650,750,850,950,1500,1150,1250,1350,1450,1550,1650,1750,1850,1950};
  double mhtYield[18] = {0.};
  double mhtYieldError[18] = {0.};
  TH1F* mhtYieldsHisto = new TH1F("mhtYieldsHisto","mhtYieldsHisto",18,200,2000);
  double njetsLowMHTbin[10] = {3,4,5,6,7,8,9,10,11,12};
  double njetsLowMHTYield[10] = {0,0,0,0,0,0,0,0,0,0};
  double njetsLowMHTYieldError[10] = {0,0,0,0,0,0,0,0,0,0};

  for( int i =0 ; i < 6 ; i++ ){
    cout << "njetsLowMHTbin: " << njetsLowMHTbin[i] << endl;
    cout << "njetsLowMHTYield: " << njetsLowMHTYield[i] << " +/- " << njetsLowMHTYieldError[i] << endl;
    cout << "njetsLowMHTYieldError(" << i << ")" << ": " << njetsLowMHTYieldError[i] << endl;
  }

  TH1F* njetsLowMHTYieldsHisto = new TH1F("njetsLowMHTYieldsHisto","njetsLowMHTYieldsHisto",10,2.5,12.5);
  double njetsMedMHTbin[10] = {3,4,5,6,7,8,9,10,11,12};
  double njetsMedMHTYield[10] = {0,0,0,0,0,0,0,0,0,0};
  double njetsMedMHTYieldError[10] = {0,0,0,0,0,0,0,0,0,0};
  TH1F* njetsMedMHTYieldsHisto = new TH1F("njetsMedMHTYieldsHisto","njetsMedMHTYieldsHisto",10,2.5,12.5);
  double njetsHighMHTbin[10] = {3,4,5,6,7,8,9,10,11,12};
  double njetsHighMHTYield[10] = {0,0,0,0,0,0,0,0,0,0};
  double njetsHighMHTYieldError[10] = {0,0,0,0,0,0,0,0,0,0};
  TH1F* njetsHighMHTYieldsHisto = new TH1F("njetsHighMHTYieldsHisto","njetsHighMHTYieldsHisto",10,2.5,12.5);

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

  for( unsigned int iSample = 0 ; 
       iSample < tree.size() ; 
       iSample++ ){

    cout << "iSample: " << iSample << endl;

    photon photons( tree[ iSample ] ) ;

    double rho;
    double MHT,HT=0.;
    int NJets=0.;

    vector<TLorentzVector>* jets = 0;

    //tree[iSample]->SetBranchAddress("fixedGridRhoFastjetAll",&rho);
    tree[iSample]->SetBranchAddress("ak4JetsPt10", &jets );

    double w  = weight[ iSample ];

    for( int i = 0 ; 
	 i < tree[iSample]->GetEntries() ; 
	 i++ ){

      if( i % 100000 == 0 ){
	cout << "event: " << i << endl;
      }
      tree[iSample]->GetEntry(i);
      //photons.getPhotonsFromEvent(i);

      bool leadingFound = false;
      int genMatchedPhoton = -1. ;

      // find gen-matched photon
      for( unsigned int iPh = 0 ; iPh < photons.fourVec->size() ; iPh++ ){
	if( photons.isGenMatched->at(iPh) == 1 ) genMatchedPhoton = iPh ;
      }

      if( genMatchedPhoton == -1 ) continue ;

      // grab jets from tree, clean gen-matched photon,
      // and compute HT from HTJets
      //  = = = = = = = = = = = =
      HT = 0. ; 
      NJets = 0;
      //cout << "compute HT" << endl;
      for( unsigned int iJet = 0 ; iJet < jets->size() ; iJet++ ){

	if( jets->at( iJet ).Pt() < 30. || fabs( jets->at( iJet ).Eta() ) < 2.5 ) continue;

	if( sqrt( pow( jets->at( iJet ).Eta() - photons.fourVec->at(genMatchedPhoton).Eta() , 2 ) + pow( jets->at( iJet ).Phi() - photons.fourVec->at(genMatchedPhoton).Phi() , 2 ) ) < 0.3 )
	  continue;
	// delta-R check
				
	HT += jets->at( iJet ).Pt() ;
	NJets ++ ;

      }

      // grab jets from tree, clean gen-matched photon,
      // and compute HT from HTJets
      //  = = = = = = = = = = = =
      TLorentzVector MHTvector;
      MHTvector.SetPtEtaPhiM(0.,0.,0.,0.);
      //cout << "compute MHT" << endl;
      for( unsigned int iJet = 0 ; iJet < jets->size() ; iJet++ ){

	if( jets->at( iJet ).Pt() < 30. || fabs( jets->at( iJet ).Eta() ) < 5.0 ) continue;

	if( sqrt( pow( jets->at( iJet ).Eta() - photons.fourVec->at(genMatchedPhoton).Eta() , 2 ) + pow( jets->at( iJet ).Phi() - photons.fourVec->at(genMatchedPhoton).Phi() , 2 ) ) < 0.3 )
	  continue;
	// delta-R check
				
	TLorentzVector temp;
	temp.SetPtEtaPhiM( jets->at( iJet ).Pt() , 
			   jets->at( iJet ).Eta() ,
			   jets->at( iJet ).Phi() ,
			   jets->at( iJet ).M() ) ;
	MHTvector -= temp ;
				
      }
      MHT = MHTvector.Pt();
      //  = = = = = = = = = = = =		

      if( NJets < 3 ) continue;

      for( int iCut = 0 ; iCut < 36 ; iCut++ ){
	if( HT >= bin[iCut].HT.first && HT <= bin[iCut].HT.second &&
	    MHT >= bin[iCut].MHT.first && MHT <= bin[iCut].MHT.second &&
	    NJets >= bin[iCut].NJets.first && NJets <= bin[iCut].NJets.second ){

	  yields[iCut]+=weight[iSample];

	}

	if(NJets>=3 && MHT>200){
	  htYieldsHisto->Fill( HT , weight[iSample] );
	  htYield[ htYieldsHisto->FindBin(HT) ] += weight[iSample];
	  htYieldError[ htYieldsHisto->FindBin(HT) ] ++ ;
	}
	if(NJets>=3 && HT>500){
	  mhtYieldsHisto->Fill( MHT , weight[iSample] );
	  mhtYield[ mhtYieldsHisto->FindBin(MHT) ] += weight[iSample];
	  mhtYieldError[ mhtYieldsHisto->FindBin(HT) ] ++ ;
	}
	if(MHT>200 && MHT<300 && HT >500){
	  njetsLowMHTYieldsHisto->Fill( NJets , weight[iSample] );
	  njetsLowMHTYield[ njetsLowMHTYieldsHisto->FindBin(NJets) ] += weight[iSample];
	  njetsLowMHTYieldError[ njetsLowMHTYieldsHisto->FindBin(NJets) ] ++ ;
	}
	if(MHT>300 && MHT<450 && HT>500){
	  njetsMedMHTYieldsHisto->Fill( NJets , weight[iSample] );
	  njetsMedMHTYield[ njetsMedMHTYieldsHisto->FindBin(NJets) ] += weight[iSample];
	  njetsMedMHTYieldError[ njetsMedMHTYieldsHisto->FindBin(NJets) ] ++ ;
	}
	if(MHT>450 && HT>500){
	  njetsHighMHTYieldsHisto->Fill( NJets , weight[iSample] );
	  njetsHighMHTYield[ njetsHighMHTYieldsHisto->FindBin(NJets) ] += weight[iSample];
	  njetsHighMHTYieldError[ njetsHighMHTYieldsHisto->FindBin(NJets) ] =  sqrt( njetsHighMHTYieldError[ njetsHighMHTYieldsHisto->FindBin(NJets) ]*njetsHighMHTYieldError[ njetsHighMHTYieldsHisto->FindBin(NJets) ] + weight[iSample]*weight[iSample] );
	}
      }


      for( unsigned int iPh = 0 ; iPh < photons.fourVec->size() ; iPh++ ){

	if( leadingFound ) continue;

	if( //photons.pt->at(iPh) > ptCutLow && 
	   //photons.pt->at(iPh) < ptCutHigh && 
	   //( fabs(photons.eta->at(iPh)) < 1.4442 || ( fabs(photons.eta->at(iPh)) > 1.566 && fabs(photons.eta->at(iPh)) < 2.5 ) ) &&
	   HT > HTcutLow && HT < HTcutHigh
	   //&& photons.isGenMatched->at(iPh) 
	   //&& photons.passIDcuts(i,iPh)	
	    ){

	  if( //photons.passIsoCuts(i,iPh,rho) 
	     photons.passIDcuts(i,iPh) 
	     //photons.isGenMatched->at(iPh) && 
	     //photons.pt->at(iPh) > ptCutLow && 
	     //photons.pt->at(iPh) < ptCutHigh && 
	     //( fabs(photons.eta->at(iPh)) < 1.4442 || ( fabs(photons.eta->at(iPh)) > 1.566 && fabs(photons.eta->at(iPh)) < 2.5 ) )						
	      ){

	    if( MHT > 200 && MHT < 300 ){
	      //cout << "check - 200 - 300" << endl;
	      if( NJets >= 12 ){
		effNum[0][9] += w;
		effNumError[0][9]++;
	      }else{
		effNum[0][NJets-3] += w;
		effNumError[0][NJets-3]++;
	      }
	    }else if( MHT > 300 && MHT < 450 ){
	      //cout << "check - 300 - 450" << endl;
	      if( NJets >= 12 ){
		effNum[1][9] += w;
		effNumError[1][9]++;
	      }else{
		effNum[1][NJets-3] += w;
		effNumError[1][NJets-3]++;
	      }
	    }else if( MHT > 450){
	      //cout << "check - 450" << endl;
	      if( NJets >= 12 ){
		effNum[2][9] += w;
		effNumError[2][9]++;
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

  cout << " - - - - - TEST - - - -  " << endl;
  for( int iCut = 0 ; iCut < 36 ; iCut++ ){
    cout << "yield in bin " << iCut << ": " << yields[iCut] << endl;
    binYields->SetBinContent(iCut,yields[iCut]);
  }	

  TFile* file = new TFile("GJetsYields.root","RECREATE");
  binYields->Write();
  htYieldsHisto->Write();
  mhtYieldsHisto->Write();
  njetsLowMHTYieldsHisto->Write();
  njetsMedMHTYieldsHisto->Write();
  njetsHighMHTYieldsHisto->Write();
  file->Close();

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

  for( int i =0 ; i < 18 ; i++ ){
    mhtYieldError[i] = sqrt( mhtYieldError[i] );
  }
  for( int i =0 ; i < 10 ; i++ ){
    njetsLowMHTYieldError[i] = njetsLowMHTYield[i]/sqrt( njetsLowMHTYieldError[i] );
    cout << "njetsLowMHTbin: " << njetsLowMHTbin[i] <<  endl;
    cout << "njetsLowMHTYield: " << njetsLowMHTYield[i] << " +/- " << njetsLowMHTYieldError[i] << endl;
    cout << "njetsLowMHTYieldError(" << i << ")" << ": " << njetsLowMHTYieldError[i] << endl;
  }
  for( int i =0 ; i < 10 ; i++ ){
    njetsMedMHTYieldError[i] = njetsMedMHTYield[i]/sqrt( njetsMedMHTYieldError[i] );
  }
  for( int i =0 ; i < 10 ; i++ ){
    njetsHighMHTYieldError[i] = njetsHighMHTYield[i]/sqrt( njetsHighMHTYieldError[i] );
  }

  double mhtBinError[18] = { 0 , 0 , 0 , 0 , 0 , 0 ,0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
  double htBinError[20] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
  double njetsLowMHTbinError[10] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
  double njetsMedMHTbinError[10] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };
  double njetsHighMHTbinError[10] = { 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 };

  TGraphErrors* njetsLowMHTR = new TGraphErrors(6,njetsLowMHTbin,njetsLowMHTYield,njetsLowMHTbinError,njetsLowMHTYieldError);
  TGraphErrors* njetsMedMHTR = new TGraphErrors(6,njetsMedMHTbin,njetsMedMHTYield,njetsMedMHTbinError,njetsMedMHTYieldError);
  TGraphErrors* njetsHighMHTR = new TGraphErrors(6,njetsHighMHTbin,njetsHighMHTYield,njetsHighMHTbinError,njetsHighMHTYieldError);

  njetsLowMHTR->Draw("Ap");
  njetsMedMHTR->Draw("SAMEp");
  njetsHighMHTR->Draw("SAMEp");

  TGraphErrors* MHT200 = new TGraphErrors(10,njets,effic[0],njetsError,efficError[0]);
  MHT200->SetMarkerStyle(8);
  MHT200->SetMarkerColor(1);
  TGraphErrors* MHT300 = new TGraphErrors(10,njets,effic[1],njetsError,efficError[1]);
  MHT300->SetMarkerStyle(8);
  MHT300->SetMarkerColor(4);
  TGraphErrors* MHT450 = new TGraphErrors(10,njets,effic[2],njetsError,efficError[2]);
  MHT450->SetMarkerStyle(8);
  MHT450->SetMarkerColor(6);

  TF1* poly[3] = { new TF1("poly_MHT200","[0]+x*[1]+x*x*[2]+x*x*x*[3]",2.5,10.5),
		   new TF1("poly_MHT300","[0]+x*[1]+x*x*[2]+x*x*x*[3]",2.5,10.5),
		   new TF1("poly_MHT450","[0]+x*[1]+x*x*[2]+x*x*x*[3]",2.5,10.5) };

  poly[0]->SetLineColor(1);
  poly[1]->SetLineColor(4);
  poly[2]->SetLineColor(6);

  //MHT200->Fit("poly_MHT200","R");
  //MHT300->Fit("poly_MHT300","R");
  //MHT450->Fit("poly_MHT450","R");

  MHT200->GetYaxis()->SetRangeUser(0.,1.);
  MHT200->GetXaxis()->SetTitle("n_{j}");
  MHT200->GetYaxis()->SetTitle("ISO Efficiency");


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
