/*
# This code was taken for Jim Hirschauer... All errors are his to defend
#
#
#
#*/

using namespace std;

pair<double,double> clopperPearson(double rNi /*numerator*/, 
				   double rNo /*denominator*/){

  //tail = (1-CL)/2.  If you want 95% CL, the tail = (1.-0.95)/2
  //tail = 0.025
  double tail = 0.16;
  double rvrat = rNi/rNo;
  
  double qHi = ROOT::Math::fdistribution_quantile_c(tail   ,2.*(rNi+1.),2.*(rNo)   );
  double qLo = ROOT::Math::fdistribution_quantile_c(1.-tail,2.*(rNi)   ,2.*(rNo+1.));

  double Rhi = 1./(rNo/(rNi+1.)/qHi);
  double Rlo = 0;
  if( rNi > 0 )
    Rlo = 1./((rNo+1.)/rNi/qLo);
  
  double eHi = Rhi-rvrat;
  double eLo = rvrat-Rlo;
  //holdover printouts from Jim's python code...
  //print "%4.2s / %4.2s  =  %4.5s | %8.3s<Ratio<%4.3s  |  %8.6s | %8.6s" % ("Ni","No","Ratio","Rlo","Rhi","+err","-err")
  //print "%4.0f / %4.0f  =  %4.3f | %8.3f<Ratio<%4.3f  |  %8.6f | %8.6f" % (rNi,rNo,rvrat,Rlo,Rhi,eHi,eLo)
  
  return pair<double,double>(eHi,eLo);

}
