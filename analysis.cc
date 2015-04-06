#include "analysis.h"

analysis::analysis(workSys sys_ , TString inputDir ){

    sys = sys_;
	  
    for( int iSample = 0 ; iSample < kNumSamples ; iSample++){

      tree[iSample]= new TChain("PreSelection");
      char temp[256];
      sprintf(temp,"%s/*%s*root",inputDir.Data(),tag[iSample].Data());
      tree[iSample]->Add(temp);
      
    }// end loop over trees

  };// end constructor

//map<char*,TH1F*> plot(TString var = "HTnoPhotons",
TH1F* analysis::plot(TString var ,
		     TString binning ,
		     TString cuts ){

  cout << "cuts: " << cuts << endl;

  // plot and grab gjets400 histogram
  tree[kGjets400]->Draw(var+">>gjets400_"+var+binning,cuts);
  TH1F* gjets400 = (TH1F*) gDirectory->Get("gjets400_"+var);
  gjets400->Sumw2();
  gjets400->Scale( weights[kGjets400] );
  
  // plot and grab gjets600 histogram
  tree[kGjets600]->Draw(var+">>gjets600_"+var+binning,cuts);
  TH1F* gjets600 = (TH1F*) gDirectory->Get("gjets600_"+var);
  gjets600->Sumw2();
  gjets600->Scale( weights[kGjets600] );
  
  TH1F* gjets = new TH1F(*gjets400);
  gjets->SetNameTitle("gjets_"+var,"gjets_"+var);
  gjets->Add(gjets600);
  
  /*
  // plot and grab zinv400 histogram
  tree[kZinv400]->Draw(var+">>zinv400_"+var+binning,cuts);
  TH1F* zinv400 = (TH1F*) gDirectory->Get("zinv400_"+var);
  zinv400->Sumw2();
  zinv400->Scale( weights[kZinv400] );
  
  // plot and grab zinv600 histogram
  tree[kZinv600]->Draw(var+">>zinv600_"+var+binning,cuts);
  TH1F* zinv600 = (TH1F*) gDirectory->Get("zinv600_"+var);
  zinv600->Sumw2();
  zinv600->Scale( weights[kZinv600] );
  
  TH1F* zinv = new TH1F(*zinv400);
  zinv->SetNameTitle("zinv_"+var,"zinv_"+var);
  zinv->Add(zinv600);
  */
  
  //map<char*,TH1F*> output;
  //output["gjets"] = gjets;
  //output["zinv"]  = zinv ;
  
  //return output;
  return gjets;  
};

THStack* analysis::CSVvsFlavor( int btags ){

  TString cuts = "bestPhoton.Pt()>100.&&abs(ak4Jets_flavor)==5&&minDeltaPhiNnoPhotons>4.&&BTags=="+TString::Itoa(btags,10);

  cout << "cuts: " << cuts << endl;

  TH1F* bs = analysis::plot("ak4Jets_CSVdisc","(40,0,1.)",cuts);
  bs->SetNameTitle("CSV_bsOnly","CSV_bsOnly");
  TH1F* cs = analysis::plot("ak4Jets_CSVdisc","(40,0,1.)",cuts);
  cs->SetNameTitle("CSV_csOnly","CSV_csOnly");
  TH1F* lf = analysis::plot("ak4Jets_CSVdisc","(40,0,1.)",cuts);
  lf->SetNameTitle("CSV_lfOnly","CSV_lfOnly");
  bs->SetFillColor(kViolet);
  cs->SetFillColor(kMagenta);
  lf->SetFillColor(kBlue);
  
  THStack *stack = new THStack("stack","stack");
  stack->Add(lf);
  stack->Add(cs);
  stack->Add(bs);
  stack->Draw("hist");

  return stack;

};
