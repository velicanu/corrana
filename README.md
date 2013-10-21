corrana
=======

Instructions to run the correlations analysis in CMS on the HiForest format.

```Shell
#step 1, pull this code from git into your current directory
git clone git@github.com:velicanu/corrana.git
cd corrana
ls
```

In here you'll see all the code used in correlations analysis. All the correlations are run in parallel using condor.
Let's start from the  ground up going through each file to see what it does and how to use it. First of all we need 
access to the hiforest analysis libraries. We could either clone them from github or create a symbolic link here, 
I'll show the latter.

1.
```Shell
ln -s /path/to/HiForestAnalysis .
# git clone git@github.com:CmsHI/HiForestAnalysis.git
```

2. corrana.C  this is the main code that loops through events and tracks to create correlation functions of them. 
              some of the main aspects are outlined below

```C
void corrana(const char * infname = "", int trackquality = 1)
{
// This function initializes the internal hiforest class which gives access to all the event variables, 
// it just needs to be run once before trying to do correlations

}

TH2D * TrackTrackSignal(double pttriglow , double pttrighigh , double ptasslow , double ptasshigh
                        , int centmin, int centmax, int nmin, int nmax, double vzrange)
{
// This function loops through all the events in the file satisfying the centrality/multiplicity cut
// within the vz range. Within each event passing these cuts it loops through all tracks that lie
// between pttriglow-high and correlates them with all tracks that lie between ptasslow-high and returns
// the 2D signal histogram. A few more histograms are created as a byproduct to classify the events.
// The returned distribution is constructed in only one quadrant and reflected to all others thus will be
// symmetric by construction.

}

TH2D * TrackTrackBackground(double pttriglow , double pttrighigh , double ptasslow, double ptasshigh, 
                            int centmin, int centmax, int nmin, int nmax, double vzrange, int statfactor)
{
// This function returns the uncorrelated distribution of tracks passing identical cuts as the signal, but 
// with trigger and associate particles originating from different events. Here statfactor sets how many more
// statistics the background should have, default 10, and the event mixing is done on consecutive events which
// performs much better on pre-vzsorted forest files. 

}
```

3.
runcorr.C  
              this code acts as the bridge between the command line and the correlations analysis.
              it takes as parameters the file number, trackquality, list of root files, tag, multiplicity
              cuts and trigger and associate pt cuts. It initializes the corrana class and creates an 
              output file to store the generated signal and background histograms as well as a few more
              for debugging purposes
              
4.
runcorr.sh
              this code acts as the bridge between condor and the runcorr.C code. It takes all arguments
              from condor and passes them to runcorr.sh, it also moves the root files created by the job
              to a scratch directory once the job is done
               
5.
runcorr.condor
              this is the condor submission script with wildcards for all arguments that are set by the
              submit script
              
6. 
submit.sh
              This is the most top level script that sets everything in motion. It gets its arguments
              directly from the user and creates a submission folder in which it puts freshly compiled
              correlations code, the condor submission script with the proper arguments, runcorr.sh 
              that condor picks up, and the text file containing the list of forest. See examples below: 

```Python
# this makes the list of root files we'll run on, remember to first cmsenv in some CMSSW to get root
ls -1 /mnt/hadoop/cms/store/user/velicanu/PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted/*.root > PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted.txt
head PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted.txt  # to see what's in here
# the following command sets up a correlations job on the these input files, in 130-160 mult bin, with trigger and 
# associate particles both in 1-2 GeV/c pT range
./submit.sh 1 PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted.txt PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted 130 160 1 2 1 2
# this creates a directory, lets cd into it, it'll be called something like this:
cd PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_130_160_1_2_1_2_2013_10_21__10_00_35/
ls
# in here you'll find all the files necessary to run the analysis, runcorr.exe is the compiled code, lets try
# to run this interactively once to make sure there are no bugs and the code works
./runcorr.exe 0 1 PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted.txt PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted 130 160 1 2 1 2
# you should see the following output
# mkdir -p /net/hidsk0001/d00/scratch/<your_username>/corrhists/HIRun2013/unmerged
# mv PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted*.root /net/hidsk0001/d00/scratch/<your_username>/corrhists/HIRun2013/unmerged
# job done successfully
ls -tr /net/hidsk0001/d00/scratch/<your_username>/corrhists/HIRun2013/unmerged
# should show you the last created file, something like:
# PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_trkqaul1_nmin130_nmax160_tptmin1_tptmax2_aptmin1_aptmax2_0.root
# lets take a look inside 
root /net/hidsk0001/d00/scratch/<your_username>/corrhists/HIRun2013/unmerged/PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_trkqaul1_nmin130_nmax160_tptmin1_tptmax2_aptmin1_aptmax2_0.root
# now inside root:
```

```C
root [1] .ls
// this should show a few histograms, lets take a look
root [4] signal_trg1_2_ass1_2_nmin130_nmax160->Draw("surf1")
// this shows the uncorrected for detector acceptence signal distribution
root [5] background_trg1_2_ass1_2_nmin130_nmax160->Draw("surf1")
// this shows the data driven detector acceptance correction, by correlating tracks from different events
root [6] corr_trg1_2_ass1_2_nmin130_nmax160->Draw("surf1")
// this shows the correlation function, signal/background, from these limimted statistics, now that we see
// that the correlation runs and there are no bugs lets submit it to condor
root [9] .q
```

```Python
# back in the shell now
condor_submit runcorr.condor
# now we wait for condor to finish, we can check the status by typing the following command
condor_q <your username>
# once all your jobs are done do the following commands
ln -s /net/hidsk0001/d00/scratch/<your username>/corrhists/HIRun2013/merged/ input_hists
hadd -f input_hists/PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_trkqaul1_nmin130_nmax160_tptmin1_tptmax2_aptmin1_aptmax2.root /net/hidsk0001/d00/scratch/dav2105/corrhists/HIRun2013/unmerged/PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_trkqaul1_nmin130_nmax160_tptmin1_tptmax2_aptmin1_aptmax2_*
# this merges all the files created by condor into one
# lets try to draw the correlation function
root
.x dplot.C+(1,"input_hists/PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_trkqaul1_nmin130_nmax160_tptmin1_tptmax2_aptmin1_aptmax2.root","PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted",130,160,1,2,1,2,"corr")
.x dplot.C+(1,"input_hists/PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted_trkqaul1_nmin130_nmax160_tptmin1_tptmax2_aptmin1_aptmax2.root","PP-Run2010B-Apr21ReReco-HLT_PixelTracks_Multiplicity_sorted",130,160,1,2,1,2,"proj")

```

That's it! Happy correlating!
