#include "TFile.h"
#include "TH1F.h"
#include "TKey.h"
#include "TList.h"

void rebinHistogram1GeV();
void rebinHistogram10GeV();
void rebinHistogram100MeV();

void rebinHistograms() {

  cout<<"1GeV"<<endl;
  rebinHistogram1GeV();
  cout<<"10GeV"<<endl;
  rebinHistogram10GeV();
  cout<<"100MeV"<<endl;
  rebinHistogram100MeV();  

}

void rebinHistogram1GeV() {
    TFile *file = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/Hist_muon1GeV.root", "READ");

    TFile *newFile = new TFile("Hist_muon1GeV.root", "RECREATE");

    Double_t customBins[12];
    customBins[0]=-1;
    double binwidth=(1.-0.8)/10.;
    for (int i = 0; i <= 10; ++i) {
        customBins[i+1] = 0.8 + (double)i*binwidth;
    }
    for(int i=0; i<12; i++) cout<<"bin "<<i<<": "<<customBins[i]<<endl;

    TIter nextkey(file->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)nextkey())) {
        TObject *obj = key->ReadObj();
        if (obj->IsA()->InheritsFrom(TH1::Class())) {
            TH1F *histogram = (TH1F*)obj;

            TH1F *newHistogram = new TH1F(histogram->GetName(), histogram->GetTitle(), 11, customBins);
            newHistogram->SetBinContent(1,histogram->Integral(1,90));
            for (int i = 91; i <= 100; i++) newHistogram->SetBinContent(i-89, histogram->GetBinContent(i));
            histogram->Print();
            newHistogram->Print();

            newHistogram->Write();
        }
        if (obj->IsA()->InheritsFrom(TTree::Class())) {
            TTree *tree = ((TTree*)obj)->CloneTree(-1);
            tree->Write();
        }
    }

    file->Close();
    newFile->Close();
}

void rebinHistogram10GeV() {
    TFile *file = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/Hist_muon10GeV.root", "READ");

    TFile *newFile = new TFile("Hist_muon10GeV.root", "RECREATE");

    Double_t customBins[11];
    customBins[0]=-1;
    double binwidth=(0.9-0.5)/4.;
    for (int i = 0; i < 4; ++i) {
        customBins[i+1] = 0.5 + (double)i*binwidth;
    }
    binwidth=(1.0-0.9)/5.;
    for (int i = 0; i <= 5; ++i) {
        customBins[i+5] = 0.9 + (double)i*binwidth;
    }

    for(int i=0; i<11; i++) cout<<"bin "<<i<<": "<<customBins[i]<<endl;

    TIter nextkey(file->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)nextkey())) {
        TObject *obj = key->ReadObj();
        if (obj->IsA()->InheritsFrom(TH1::Class())) {
            TH1F *histogram = (TH1F*)obj;

            TH1F *newHistogram = new TH1F(histogram->GetName(), histogram->GetTitle(), 10, customBins);
            newHistogram->SetBinContent(1,histogram->Integral(1,75));
            newHistogram->SetBinContent(2,histogram->Integral(76,80));
            newHistogram->SetBinContent(3,histogram->Integral(81,85));
            newHistogram->SetBinContent(4,histogram->Integral(86,90));
            newHistogram->SetBinContent(5,histogram->Integral(91,95));
            for (int i = 96; i <= 100; i++) newHistogram->SetBinContent(i-90, histogram->GetBinContent(i));
            histogram->Print();
            newHistogram->Print();

            newHistogram->Write();
        }
        if (obj->IsA()->InheritsFrom(TTree::Class())) {
            TTree *tree = ((TTree*)obj)->CloneTree(-1);
            tree->Write();
        }
    }

    file->Close();
    newFile->Close();
}

void rebinHistogram100MeV() {
    TFile *file = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/Hist_muon100MeV.root", "READ");

    TFile *newFile = new TFile("Hist_muon100MeV.root", "RECREATE");

    TIter nextkey(file->GetListOfKeys());
    TKey *key;
    while ((key = (TKey*)nextkey())) {
        TObject *obj = key->ReadObj();
        if (obj->IsA()->InheritsFrom(TH1::Class())) {
            TH1F *histogram = (TH1F*)obj;

            TH1F *newHistogram = new TH1F(histogram->GetName(), histogram->GetTitle(), 95, 0, 0.9);
            for (int i = 1; i <= 95; i++) newHistogram->SetBinContent(i, histogram->GetBinContent(i));
            histogram->Print();
            newHistogram->Print();
            if (strcmp(histogram->GetName(),"hbkg")!=0) newHistogram->Scale((1000.)/(newHistogram->Integral()));
            newHistogram->Print();

            newHistogram->Write();
        }
        if (obj->IsA()->InheritsFrom(TTree::Class())) {
            TTree *tree = ((TTree*)obj)->CloneTree(-1);
            newFile->cd();
            tree->Write();
        }
    }

    file->Close();
    newFile->Close();
}
