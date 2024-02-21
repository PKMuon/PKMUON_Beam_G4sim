#!/bin/bash

currentpath=$PWD

#for i in {0..999}
for i in {0..99}
do

  echo "generate mac ${i}!!!"
  cp SingleEngMu.mac mac/SingleEngMu_${i}.mac
  ROOTNAME=$currentpath/root/mu100MeV_${i}
  #NUM=100000000
  NUM=10000000
  ENE='39.818643 MeV'
  sed -i "s#OUTROOTNAME#${ROOTNAME}#g" mac/SingleEngMu_${i}.mac
  sed -i "s#JOBNUM#${NUM}#g" mac/SingleEngMu_${i}.mac
  sed -i "s#ENERGY#${ENE}#g" mac/SingleEngMu_${i}.mac

done

ls `pwd`/mac/*.mac > maclist.txt
