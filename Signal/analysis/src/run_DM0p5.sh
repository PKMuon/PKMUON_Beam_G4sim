inpath=/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/build/root_file
DMmass=0p5

echo 100 MeV
root -b -q -l analysis.cc'('\"${inpath}/DM${DMmass}_muon100MeV_maxwell.root\"','\"../root/DM${DMmass}_muon100MeV_maxwell.root\"')'

echo 10 GeV
root -b -q -l analysis.cc'('\"${inpath}/DM${DMmass}_muon10GeV_maxwell.root\"','\"../root/DM${DMmass}_muon10GeV_maxwell.root\"')'

echo 10 MeV
root -b -q -l analysis.cc'('\"${inpath}/DM${DMmass}_muon10MeV_maxwell.root\"','\"../root/DM${DMmass}_muon10MeV_maxwell.root\"')'

echo 1 GeV
root -b -q -l analysis.cc'('\"${inpath}/DM${DMmass}_muon1GeV_maxwell.root\"','\"../root/DM${DMmass}_muon1GeV_maxwell.root\"')'

