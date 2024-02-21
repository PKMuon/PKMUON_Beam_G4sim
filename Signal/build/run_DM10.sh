#!/bin/bash

infile=(
'/data/bond/yimengzhang/GEM/MuBeam-12.14/Mu_beam(mDM=10.0GeV,Mu=100MeV).dat'
'/data/bond/yimengzhang/GEM/MuBeam-12.14/Mu_beam(mDM=10.0GeV,Mu=10GeV).dat'
'/data/bond/yimengzhang/GEM/MuBeam-12.14/Mu_beam(mDM=10.0GeV,Mu=10MeV).dat'
'/data/bond/yimengzhang/GEM/MuBeam-12.14/Mu_beam(mDM=10.0GeV,Mu=1GeV).dat'
)

outfile=(
'root_file/DM10_muon100MeV_maxwell.root'
'root_file/DM10_muon10GeV_maxwell.root'
'root_file/DM10_muon10MeV_maxwell.root'
'root_file/DM10_muon1GeV_maxwell.root'
)

eventnumber=1000000

length=${#outfile[@]}

for ((i=0; i<$length; i++))
do
    echo ${infile[$i]} ${outfile[$i]} $eventnumber
    cp DMmuon.mac DM_${i}.mac
    sed -i "s#inputdatfile#${infile[$i]}#g" DM_${i}.mac
    sed -i "s#outputrootfile#${outfile[$i]}#g" DM_${i}.mac
    sed -i "s#EVTNUMBER#${eventnumber}#g" DM_${i}.mac
    ./muPos DM_${i}.mac &
done

