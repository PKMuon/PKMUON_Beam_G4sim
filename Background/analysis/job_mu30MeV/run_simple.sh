#! /bin/bash

ls -l /
echo "--------"
ls -l /usr
echo `pwd`
hostname
cat /etc/redhat-release
cat $_CONDOR_JOB_AD
sleep 2
echo "yeah"
echo $3

source ~/.bashrc
source /data/bond/yuxd/root/install/bin/thisroot.sh

ExecPath=/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Background/build
${ExecPath}/muPos $1 > $2
#${ExecPath}/muPos /home/pku/yuxd/bond/PKMUON_G4sim/build/job/testmac/CryMu_test.mac
