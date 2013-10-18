#include "TF1.h"
#include "TH1.h"
#include "TMath.h"
#include <sstream>
#include <string>

TF1 * addFitToCanvas(TH1D * hist_to_fit, int nterms = 5, int nbinsexcluded = 0)
{
  stringstream oss;
  oss<<"[0]*(1";
  for(int i = 0 ; i < nterms ; ++i)
  {
    oss<<Form("+2*[%d]*cos(%d*x)",i+1,i+1);
  }
  oss<<")";
  string formula = oss.str();
  
  TF1 * f1d_data1 = new TF1("f1d_data1",formula.data(),0,3.1415926*(1.0 - (nbinsexcluded/16.0)));
  for(int i = 0 ; i <= nterms ; ++i)
    f1d_data1->SetParameter(i,0.1);
    
  hist_to_fit->Fit("f1d_data1","RNO");
  f1d_data1->SetLineStyle(2);
  return f1d_data1;
}

TF1 * addFitPlusGaus(TH1D * hist_to_fit, int nterms = 5, int nbinsexcluded = 0)
{
  stringstream oss;
  oss<<"[0]*(1";
  for(int i = 0 ; i < nterms ; ++i)
  {
    oss<<Form("+2*[%d]*cos(%d*x)",i+1,i+1);
  }
  oss<<Form("+[%d]*exp(-0.5*((x-[%d])/[%d])**2)",nterms+1,nterms+2,nterms+3);
  oss<<")";
  string formula = oss.str();
  
  TF1 * f1d_data1 = new TF1("f1d_data1",formula.data(),0,3.1415926*(1.0 - (nbinsexcluded/16.0)));
  for(int i = 0 ; i <= nterms+3 ; ++i)
    f1d_data1->SetParameter(i,0.1);
  
  f1d_data1->FixParameter(nterms+2,TMath::Pi());
  hist_to_fit->Fit("f1d_data1","RNO");
  f1d_data1->SetLineStyle(2);
  return f1d_data1;
}
