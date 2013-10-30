#include <iostream>
#include "TFile.h"
#include "TH1D.h"
#include "TF2.h"
#include "TF1.h"
#include "TMath.h"
#include "TAttMarker.h"
#include "TH2D.h"
#include "TCanvas.h" 
#include "TGraphErrors.h"
#include "TLatex.h"
#include "TLegend.h"
#include "/net/hisrv0001/home/dgulhan/run/CMSSW_4_4_4/src/UserCode/HiForest/V2/commonUtility.h"
#include "FitHist.C"

void eplot(int trkqual = 1, string filename = "", int nmin = 130, int nmax = 160, float tptmin = 14, float tptmax = 16, float aptmin = 1, float aptmax = 2, string draw = "corr", float zmax = -1)
{
	// HIHighPt_HIRun2011-15Apr2013-v1-HLT_HIFullTrack14_sort_trkqaul1_nmin185_nmax220_tptmin14_tptmax16_aptmin1_aptmax2.root
	// TFile * file = new TFile(Form(filename.data());
	TFile * file = new TFile(Form("input_hists/%s_trkqaul%d_nmin%d_nmax%d_tptmin%d_tptmax%d_aptmin%d_aptmax%d.root",filename.data(),trkqual,nmin,nmax,(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax));
	TH2D * Sig = (TH2D*)file->Get(Form("signal_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
	TH2D * Back = (TH2D*)file->Get(Form("background_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
	TH1D * Ntrig = (TH1D*)file->Get(Form("hmulttrg_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
	TH2D * Corr = (TH2D*)Sig->Clone(Form("correlation_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
	Corr->Divide(Back);
	int x0 = Back->GetXaxis()->FindBin(0.0);
	int y0 = Back->GetYaxis()->FindBin(0.0);
	double B0 = Back->GetBinContent(x0,y0);
	double bwx = Sig->GetXaxis()->GetBinWidth(2);
	double bwy = Sig->GetYaxis()->GetBinWidth(2);
	Corr->Scale(B0/(bwx*bwy*Ntrig->Integral(2,300)));
	Corr->GetXaxis()->SetRangeUser(-4,4);
	Corr->GetYaxis()->SetRangeUser(-TMath::Pi()/2,3*TMath::Pi()/2);
  if(zmax > 0)
		Corr->SetMaximum(zmax);
	double max = Corr->GetMaximum();
	double min = Corr->GetMinimum();
	int xmn = Corr->GetXaxis()->FindBin(2);
	int xmx = Corr->GetXaxis()->FindBin(3.5);
	double nbins = xmx - xmn + 1;
	TH1D * Proj = Corr->ProjectionY(Form("projection_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax),xmn,xmx,"e");
	Proj->Scale(1/nbins);
  Proj->SetAxisRange(0,TMath::Pi());
  Proj->SetMarkerColor(2);
	// double shift = Proj->GetMinimum();
	int rangedown = Proj->FindBin(0);
	int rangeup = Proj->FindBin(TMath::Pi());
	//!
	for(int i = rangedown; i < rangeup + 1; i++){
		// double value = Proj[imult][ipt]->GetBinContent(i);
		double error = Proj->GetBinError(i)*0.001;
		
		// value = value - shift;
		// Proj[imult][ipt]->SetBinContent(i,value);		
		Proj->SetBinError(i,error);		
	}
	TF1 * fourier = addFitToCanvas(Proj);
	float v2_low = sqrt(fourier->GetParameter(2));
	cout<<"v2(1-2 GeV) = "<<v2_low<<endl;
  string quality[] = {"","no selection","inverted"};
  
  TLatex * lmult;
  // if(tag.compare("PbPbUPC_pptracking_452p1_forest_sorted_v3")==0)
  lmult = new TLatex(0.01, 0.95, Form("CMS PbPb %d #leq N < %d", nmin, nmax));
  if(filename.compare("HIRun2013-PromptReco-v1-HLT_PAPixelTrackMultiplicity130_FullTrack12-sort")==0)
    lmult = new TLatex(0.01, 0.95, Form("CMS pPb %d #leq N < %d", nmin, nmax));
  
  lmult->SetNDC(1);
  lmult->SetTextSize(lmult->GetTextSize()*1.5);

  TLatex * ltrackqual = new TLatex(0.8, 0.95, Form("%s", quality[0].data()));
  ltrackqual->SetNDC(1);
  ltrackqual->SetTextSize(ltrackqual->GetTextSize()*1.5);

  TLatex * lpt = new TLatex(0.01, 0.86, Form("%d #leq p_{T}^{trig} < %d GeV/c", (int)tptmin, (int)tptmax));
  TLatex * lpta = new TLatex(0.5, 0.86, Form("%d #leq p_{T}^{ass} < %d GeV/c", (int)aptmin, (int)aptmax));
  lpt->SetNDC(1);
  lpta->SetNDC(1);
  lpt->SetTextSize(lpt->GetTextSize()*1.5);
  lpta->SetTextSize(lpta->GetTextSize()*1.5);
    
  
	
	TCanvas * c1 = new TCanvas(Form("canvas_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
	if(draw.compare("corr")==0)
		Corr->Draw("Surf1");
	if(draw.compare("proj")==0)
	{
		Proj->Draw();
		fourier->Draw("same");
		// TLatex * lv2 = new TLatex(0.5, 0.77, Form("v_{2} = %2.2f", v2));
		// lv2->SetNDC(1);
		// lv2->SetTextSize(lv2->GetTextSize()*1.5);
	}
  cout<<"5"<<endl;
	lmult->Draw("Same");
	lpt->Draw("Same");
	lpta->Draw("Same");
	// lv2->Draw("Same");
  if(draw.compare("corr")==0)
	{
    c1->SaveAs(Form("corr_%s_trg%d_%d_ass%d_%d_nmin%d_nmax%d.png",filename.data(),(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
  }
	else if(draw.compare("proj")==0)
	{
    c1->SaveAs(Form("proj_%s_trg%d_%d_ass%d_%d_nmin%d_nmax%d.png",filename.data(),(int)tptmin,(int)tptmax,(int)aptmin,(int)aptmax,nmin,nmax));
	}

}

