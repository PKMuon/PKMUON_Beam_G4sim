#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <sys/time.h>

using namespace std;
extern double Z1;
extern double Z2;
extern double Z3;
extern double Z4;

void analysis(const char *infile, const char *outfile){
TRandom *rand = new TRandom();
double sigma=100*0.001;

TFile *f = new TFile(infile,"");
TTree *t = (TTree*)f->Get("T1");

double GemTrkPx[2], GemTrkPy[2], GemTrkPz[2], GemTrkE[2];
double GemTrkEdep[2];
double GemTrkX[2], GemTrkY[2], GemTrkZ[2];

t->SetBranchAddress("GemTrkPx",&GemTrkPx);
t->SetBranchAddress("GemTrkPy",&GemTrkPy);
t->SetBranchAddress("GemTrkPz",&GemTrkPz);
t->SetBranchAddress("GemTrkE",&GemTrkE);
t->SetBranchAddress("GemTrkEdep",&GemTrkEdep);
t->SetBranchAddress("GemTrkX",&GemTrkX);
t->SetBranchAddress("GemTrkY",&GemTrkY);
t->SetBranchAddress("GemTrkZ",&GemTrkZ);
//t->Print();

//new file and new tree
TFile * fn = new TFile(outfile,"recreate");
TTree * tn = new TTree("T1","tree");
tn = t->CloneTree(0);

double m_GemTrkX_smear[2], m_GemTrkY_smear[2], m_GemTrkZ_smear[2];
double m_costheta, m_costheta_smear;
tn->Branch("GemTrkX_smear", m_GemTrkX_smear, "GemTrkX_smear[2]/D");
tn->Branch("GemTrkY_smear", m_GemTrkY_smear, "GemTrkY_smear[2]/D");
tn->Branch("GemTrkZ_smear", m_GemTrkZ_smear, "GemTrkZ_smear[2]/D");
tn->Branch("costheta", &m_costheta, "costheta/D");
tn->Branch("costheta_smear", &m_costheta_smear, "costheta_smear/D");

struct timeval start;
struct timeval end;
unsigned long timer;

gettimeofday(&start, NULL); // 计时开始

double radius, tanphi, phi, deltaphi, newphi;
int zone_lr; //0:0; 1:left; 2:right

int nevent = t->GetEntries();
for(int ievent=0; ievent<nevent; ievent++){
        if (ievent % (int)(nevent / 10) == 0) cout << "Processing progress: " << ievent / (int)(nevent / 10) << "0%" << endl;
        t->GetEntry(ievent);

        for(int igem=0; igem<2; igem++){
        m_GemTrkZ_smear[igem] = GemTrkZ[igem]+rand->Gaus(0,sigma);
        radius = sqrt(GemTrkX[igem]*GemTrkX[igem] + GemTrkY[igem]*GemTrkY[igem]);
        tanphi = GemTrkY[igem]/GemTrkX[igem];
        if(GemTrkX[igem]>0) zone_lr=2;
        else if(GemTrkX[igem]<0) zone_lr=1;
        else zone_lr=0;
        if(zone_lr==2) phi = atan(tanphi);
        else if(zone_lr==1) phi = atan(tanphi)+TMath::Pi();
        else if(zone_lr==0) cout<<"phi=90 or -90, wrong!"<<endl;
        deltaphi = rand->Gaus(0,sigma) / radius;
        newphi = phi+deltaphi;
        m_GemTrkX_smear[igem] = radius*cos(newphi);
        m_GemTrkY_smear[igem] = radius*sin(newphi);
        }

        // angle <incoming vector, outcoming vector>
        TVector3 *Pos1 = new TVector3(GemTrkX[0],GemTrkY[0],GemTrkZ[0]);
        TVector3 *Pos2 = new TVector3(GemTrkX[1],GemTrkY[1],GemTrkZ[1]);
        TVector3 Vec_muon = *Pos2 - *Pos1;
        TVector3 Vec_beam(0,0,1);
        m_costheta = cos(Vec_beam.Angle(Vec_muon));

        TVector3 *Pos1_smear = new TVector3(m_GemTrkX_smear[0],m_GemTrkY_smear[0],m_GemTrkZ_smear[0]);
        TVector3 *Pos2_smear = new TVector3(m_GemTrkX_smear[1],m_GemTrkY_smear[1],m_GemTrkZ_smear[1]);
        TVector3 Vec_muon_smear = *Pos2_smear - *Pos1_smear;
        TVector3 Vec_beam_smear(0,0,1);
        m_costheta_smear = cos(Vec_beam_smear.Angle(Vec_muon_smear));

        tn->Fill();
}
gettimeofday(&end, NULL); // 计时结束
timer = 1000000 * (end.tv_sec - start.tv_sec) + end.tv_usec - start.tv_usec;
printf("time = %ld us\n", timer);

fn->cd();
fn->Write();
fn->Close();
f->Close();

}

