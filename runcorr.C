#include "corrana.C"
#include <fstream>

void runcorr(int condor_iter, int trackqual, string flist = "", string tag = "", int nmin = 220, int nmax = 1000, float pttrigmin = 1, float pttrigmax = 2, float ptassmin = 1, float ptassmax = 1)
{

  const int nptbins = 1;
  const int ncentbins = 1;
  const int najbins = 1;
  
  string buffer;
  vector<string> listoffiles;
  int nlines = 0;
  // ifstream infile("sortedforests.txt");
  // ifstream infile("doeproposalforests.txt");
  // ifstream infile("PA2013_PromptReco_Json_FullTrack12_v84_prod2.txt");
  // ifstream infile("PA2013_PromptReco_Json_FullTrack12_v84_prod2_MIT.txt");
  // ifstream infile("HIMinBiasUPC_PbPbUPC_pptracking_452p1_forest_v2.txt");
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
  bool dostdhists = (condor_iter%(nptbins * ncentbins * najbins) == 0);
  int ptbin = condor_iter % nptbins;
  int centbin = (condor_iter / nptbins) % ncentbins;
  int ajbin = (condor_iter / (nptbins * ncentbins)) % najbins;
  int filenum = (condor_iter / (nptbins * ncentbins * najbins));
  cout << "ipt: " << ptbin << " icent: " << centbin << " iaj: " << ajbin << " filenum: " << filenum << " dostdhists: " << dostdhists << endl;
 
  // int nmin = 220 , nmax = 1000, one = 1;
  //! for first iteration of forest production
  /*
  if(filenum==0) { nmin = 110 ; nmax = 1000; }
  if(filenum==1) { nmin = 90  ; nmax = 110 ; }
  if(filenum>1 ) { nmin = 0   ; nmax = 35  ; }
  if(filenum>9 ) { nmin = 35  ; nmax = 90  ; }
  */
  //! for second iteration of forest production
  // /*
  // if(filenum<24 ) { nmin = 90  ; nmax = 110 ; }
  // if(filenum<22 ) { nmin = 35  ; nmax = 90  ; }
  // if(filenum<12 ) { nmin = 110 ; nmax = 1000; }
  // if(filenum<10 ) { nmin = 0   ; nmax = 35  ; }
  // */
  //! for second iteration of forest production
  /*
  if(filenum<26 ) { nmin = 90  ; nmax = 110 ; }
  if(filenum<23 ) { nmin = 35  ; nmax = 90  ; }
  if(filenum<13)  { nmin = 110 ; nmax = 1000; }
  if(filenum<10)  { nmin = 0   ; nmax = 35  ; }
  */
  corrana(listoffiles[filenum].data(),trackqual);
  
  
  // double pttriglow[] =  {1   ,14 ,16 ,20, 1};
  // double pttrighigh[] = {2   ,16 ,20 ,24, 3};
  // double ptasslow[] =   {1   ,1  ,1  ,1 , 1};
  // double ptasshigh[] =  {2   ,2  ,2  ,2 , 3};
  
  
  // int centmin[] = {0,4,8,12,16,20,24,28,32};
  // int centmax[] = {41,8,12,16,20,24,28,32,36};
  // TFile * outf = new TFile(Form("%s_trkqaul%d_nmin%d_nmax%d_tptmin%d_tptmax%d_aptmin%d_aptmax%d_%d.root",tag.data(),trackqual,nmin,nmax,(int)pttrigmin/one,(int)pttrigmax/one,(int)ptassmin/one,(int)ptassmax/one,filenum),"recreate");
  cout<<Form("%s_trkqaul%d_nmin%d_nmax%d_tptmin%d_tptmax%d_aptmin%d_aptmax%d_%d.root",tag.data(),trackqual,nmin,nmax,(int)pttrigmin,(int)pttrigmax,(int)ptassmin,(int)ptassmax,filenum)<<endl;
  TFile * outf = new TFile(Form("%s_trkqaul%d_nmin%d_nmax%d_tptmin%d_tptmax%d_aptmin%d_aptmax%d_%d.root",tag.data(),trackqual,nmin,nmax,(int)pttrigmin,(int)pttrigmax,(int)ptassmin,(int)ptassmax,filenum),"recreate");
  
  // int i = 0;
  // int cent = 0;

  // cout<<"cent iteration "<<cent<<endl;
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

