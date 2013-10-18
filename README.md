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

```C++
void corrana(const char * infname = "", int trackquality = 1)
{
// This function initializes the internal hiforest class which gives access to all the event variables, 
// it just needs to be run once before trying to do correlations

}

TH2D * TrackTrackSignal(double pttriglow , double pttrighigh , double ptasslow , double ptasshigh, 
                        int centmin, int centmax, int nmin, int nmax, double vzrange)
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

3. runcorr.C  
              this code acts as the bridge between the command line and the correlations analysis.
              it takes as parameters the file number, trackquality, list of root files, tag, multiplicity
              cuts and trigger and associate pt cuts. It initializes the corrana class and creates an 
              output file to store the generated signal and background histograms as well as a few more
              for debugging purposes
              
4. runcorr.sh
              this code acts as the bridge between condor and the runcorr.C code. It takes all arguments
              from condor and passes them to runcorr.sh, it also moves the root files created by the job
              to a scratch directory once the job is done
               
5. runcorr.condor
              this is the condor submission script with wildcards for all arguments that are set by the
              submit script
              
6. submit.sh
              
