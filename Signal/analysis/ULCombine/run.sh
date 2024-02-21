muonenergy=("1GeV" "10MeV" "10GeV" "100MeV")
sigtype=("maxwell")
DMmass=("0p005" "0p05" "0p1" "0p2" "0p5" "1" "10" "100")

mypath=$PWD
echo $mypath

for (( i=0; i<${#sigtype[@]}; i++ ))
do
  for (( j=0; j<${#muonenergy[@]}; j++ ))
  do
    if (($j == 1)); then
    #if (($j != 3)); then
      continue
    fi
    echo signal velocity: ${sigtype[$i]} and muon energy: ${muonenergy[$j]}
    if [ -d "UL_${sigtype[$i]}_muon${muonenergy[$j]}" ]; then
      rm -rf UL_${sigtype[$i]}_muon${muonenergy[$j]}
    fi
    mkdir UL_${sigtype[$i]}_muon${muonenergy[$j]}
    cd UL_${sigtype[$i]}_muon${muonenergy[$j]}
    for (( k=0; k<${#DMmass[@]}; k++ ))
    do
      echo Dark Matter mass ${DMmass[$k]}
      cp -r ../datacard.txt datacard_${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]}.txt
      sed -i "s#ROOTFILE#/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/ULCombine/Hist_muon${muonenergy[$j]}.root#g" datacard_${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]}.txt
      sed -i "s/SIG/sig_${DMmass[$k]}/g" datacard_${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]}.txt
      sed -i "s/BKG/bkg/g" datacard_${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]}.txt
      combine -M AsymptoticLimits datacard_${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]}.txt -n ${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]} --run blind > log_${sigtype[$i]}_DM${DMmass[$k]}_muon${muonenergy[$j]} &
    done
    cd $mypath
  done
done
