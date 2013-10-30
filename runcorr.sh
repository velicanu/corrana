if [ $# -ne 10 ]
then
  echo "Usage: ./runcorr.sh <condor_iteration> <trackqual> <file-list> <tag> <nmin> <nmax> <pttrigmin> <pttrigmax> <ptassmin> <ptassmax>"
  exit 1
fi

echo | awk -v i=$1 -v trkq=$2 -v flist=$3 -v tag=$4 -v nmin=$5 -v nmax=$6 -v pttrigmin=$7 -v pttrigmax=$8 -v ptassmin=$9 -v ptassmax=${10} '{print "./runcorr.exe "i" "trkq" "flist" "tag" "nmin" "nmax" "pttrigmin" "pttrigmax" "ptassmin" "ptassmax" "}' 
echo | awk -v i=$1 -v trkq=$2 -v flist=$3 -v tag=$4 -v nmin=$5 -v nmax=$6 -v pttrigmin=$7 -v pttrigmax=$8 -v ptassmin=$9 -v ptassmax=${10} '{print "./runcorr.exe "i" "trkq" "flist" "tag" "nmin" "nmax" "pttrigmin" "pttrigmax" "ptassmin" "ptassmax" "}' | bash
echo | awk -v tag=$4 -v user=$USER '{print "mkdir -p /net/hidsk0001/d00/scratch/"user"/corrhists/HIRun2013/merged"}'
echo | awk -v tag=$4 -v user=$USER '{print "mkdir -p /net/hidsk0001/d00/scratch/"user"/corrhists/HIRun2013/merged"}' | bash
echo | awk -v tag=$4 -v user=$USER '{print "mkdir -p /net/hidsk0001/d00/scratch/"user"/corrhists/HIRun2013/unmerged"}'
echo | awk -v tag=$4 -v user=$USER '{print "mkdir -p /net/hidsk0001/d00/scratch/"user"/corrhists/HIRun2013/unmerged"}' | bash
echo | awk -v tag=$4 -v user=$USER '{print "scp "tag"*.root hidsk0001:/net/hidsk0001/d00/scratch/"user"/corrhists/HIRun2013/unmerged"}'
echo | awk -v tag=$4 -v user=$USER '{print "scp "tag"*.root hidsk0001:/net/hidsk0001/d00/scratch/"user"/corrhists/HIRun2013/unmerged"}' | bash
echo | awk -v tag=$4 '{print "rm "tag"*.root"}'
echo | awk -v tag=$4 '{print "rm "tag"*.root"}' | bash

echo "job done successfully"
