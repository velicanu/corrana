if [ $# -ne 9 ]
then
  echo "Usage: ./psort.sh <trackqual> <file-list> <tag> <nmin> <nmax> <pttrigmin> <pttrigmax> <ptassmin> <ptassmax>"
  exit 1
fi

now="$3_$(date +"%Y_%m_%d__%H_%M_%S")"
len=`wc -l $2 | awk '{print $1}'`
njobs=$((len))

mkdir $now
cp $2 $now
cp runcorr.sh $now
cat runcorr.condor | sed "s@log_flag@$now@g" | sed "s@dir_flag@$PWD/$now@g" |  sed "s@arglist@$1 $2 $3 $4 $5 $6 $7 $8 $9@g" | sed "s@transfer_filelist@$2,runcorr.exe@g" | sed "s@njobs@$njobs@g" > $now/runcorr.condor

NAME="runcorr.C"
g++ $NAME $(root-config --cflags --libs) -Werror -Wall -O2 -o "${NAME/%.C/}.exe"
cp runcorr.exe $now
echo
cat $now/runcorr.condor
echo 
echo condor_submit $now/runcorr.condor
echo
# condor_submit $now/runcorr.condor

