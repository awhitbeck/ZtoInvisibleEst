#include "photonEfficiency.C"
#include "TGraphErrors.h"
#include "TPaveText.h"

using namespace std;

void scanEfficiency(double MHTcutLow = 300 ,
		    double MHTcutHigh = 450 
		    ){
	
  double efficiency[10];
  double efficiencyError[10];
  double njets[10];
  double njetsError[10];

  pair<double,double> temp;

  for( int i = 3 ; i < 13 ; i++ ){


    temp = photonEfficiency(100. , 999999. , // pt cut
			    500. , 99999.    // HT cut
			    );
	
    efficiency[ i - 3 ] = temp.first;
    efficiencyError[ i - 3 ] = temp.second;
    njets[ i - 3 ] = (double) i ;
    njetsError[ i - 3 ] = 0.5 ; 

  }

  TGraphErrors* gr = new TGraphErrors(10,njets,efficiency,njetsError,efficiencyError);
  gr->GetXaxis()->SetTitle("n_{jets}");
  gr->GetYaxis()->SetTitle("Efficiency");
  gr->GetYaxis()->SetRangeUser(0,1);
	
  gr->Draw("Ap");

  TPaveText* pave = new TPaveText(.4,.7,.9,.9);
  char text[256];
  sprintf(text,"%i < E_{T}^{miss} < %i",(int)MHTcutLow,(int)MHTcutHigh);
  pave->AddText(text);
  pave->SetBorderSize(0);
  pave->SetFillColor(0);
  pave->Draw();

}
