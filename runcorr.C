#include "corrana.C"
#include <fstream>

void runcorr(int condor_iter, int trackqual, string flist = "", string tag = "", int nmin = 220, int nmax = 1000, float pttrigmin = 1, float pttrigmax = 2, float ptassmin = 1, float ptassmax = 1)
{
  string buffer;
  vector<string> listoffiles;
  int nlines = 0;
  ifstream infile(flist.data());
  if (!infile.is_open()) {
    cout << "Error opening file. Exiting." << endl;
    return;
  } else {
    while (!infile.eof()) {
      infile >> buffer;
      listoffiles.push_back(buffer);
      nlines++;
    }
  }
  corrana(listoffiles[condor_iter].data(),trackqual);
  cout<<Form("%s_trkqaul%d_nmin%d_nmax%d_tptmin%d_tptmax%d_aptmin%d_aptmax%d_%d.root",tag.data(),trackqual,nmin,nmax,(int)pttrigmin,(int)pttrigmax,(int)ptassmin,(int)ptassmax,condor_iter)<<endl;
  TFile * outf = new TFile(Form("%s_trkqaul%d_nmin%d_nmax%d_tptmin%d_tptmax%d_aptmin%d_aptmax%d_%d.root",tag.data(),trackqual,nmin,nmax,(int)pttrigmin,(int)pttrigmax,(int)ptassmin,(int)ptassmax,condor_iter),"recreate");
  TH2D * ttsig = TrackTrackSignal(pttrigmin,pttrigmax,ptassmin,ptassmax,nmin,nmax,nmin,nmax);
  TH2D * ttbak = TrackTrackBackground(pttrigmin,pttrigmax,ptassmin,ptassmax,nmin,nmax,nmin,nmax);

  TH1I * hntottrig = new TH1I(Form("nttottrig_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)pttrigmin,(int)pttrigmax,(int)ptassmin,(int)ptassmax,nmin,nmax),"",1,0.5,1.5);
  int myntottrig = GetNTotTrig();
  hntottrig->Fill(1,myntottrig);
  cout<<"ntottrig: "<<myntottrig<<endl;

  TH2D * ttcorr = (TH2D*)ttsig->Clone(Form("corr_trg%d_%d_ass%d_%d_nmin%d_nmax%d",(int)pttrigmin,(int)pttrigmax,(int)ptassmin,(int)ptassmax,nmin,nmax));
  ttcorr->Divide(ttbak);
  ttcorr->Scale(ttbak->GetBinContent(ttbak->FindBin(0,0)));
  ttcorr->Scale(1/0.0594998609); //! bin width
  ttcorr->GetXaxis()->SetRange(ttcorr->GetXaxis()->FindBin(-4.0),ttcorr->GetXaxis()->FindBin(4.0));
  ttcorr->GetYaxis()->SetRange(ttcorr->GetYaxis()->FindBin(-3.1415926/2.0),ttcorr->GetYaxis()->FindBin(3*3.1415926/2.0));
  
  outf->Write();
  outf->Close();
}


int main(int argc, char *argv[])
{
  if(argc != 11)
  {
    std::cout << "Usage: runcorr <condor_iter> <trackqual> <file-list> <tag> <nmin> <nmax> <pttrigmin> <pttrigmax> <ptassmin> <ptassmax>" << std::endl;
    return 1;
  }
  runcorr(std::atoi(argv[1]), std::atoi(argv[2]), argv[3], argv[4], std::atoi(argv[5]), std::atoi(argv[6]), std::atof(argv[7]), std::atof(argv[8]), std::atof(argv[9]), std::atof(argv[10]));
  return 0;
}

