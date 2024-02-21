#include "boot.h"
void draw_costheta(const char* MuonEnergy){

TFile* fbkg = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Background/analysis/root/mu" + (TString)MuonEnergy + ".root");
TFile* fsig_0p005 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM0p005_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_0p05 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM0p05_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_0p1 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM0p1_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_0p2 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM0p2_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_0p5 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM0p5_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_1 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM1_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_10 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM10_muon" + (TString)MuonEnergy + "_maxwell.root");
TFile* fsig_100 = new TFile("/home/pku/yuxd/bond/PKMUON_Beam_G4sim/Signal/analysis/root/DM100_muon" + (TString)MuonEnergy + "_maxwell.root");

TTree* tbkg       = (TTree*)fbkg->Get("T1");
TTree* tsig_0p005 = (TTree*)fsig_0p005->Get("T1");
TTree* tsig_0p05  = (TTree*)fsig_0p05->Get("T1");
TTree* tsig_0p1   = (TTree*)fsig_0p1->Get("T1");
TTree* tsig_0p2   = (TTree*)fsig_0p2->Get("T1");
TTree* tsig_0p5   = (TTree*)fsig_0p5->Get("T1");
TTree* tsig_1     = (TTree*)fsig_1->Get("T1");
TTree* tsig_10    = (TTree*)fsig_10->Get("T1");
TTree* tsig_100   = (TTree*)fsig_100->Get("T1");


setStyle();
gStyle->SetTitleYOffset(1.0);     // Y title offset
gStyle->SetTitleXOffset(0.8);     // X title offset

TCanvas *can = new TCanvas("can","can",800,600);setPad();
gPad->SetBottomMargin(0.12);       // pad bottom margin for writing X title
gPad->SetRightMargin(0.02);       // pad right margin  for writing Y title
gPad->SetTopMargin(0.02);         // pad top margin for writing X title
can->SetLogy();

double xbins=100, xmin=-1, xmax=1;

TH1D *hbkg       = new TH1D("hbkg","hbkg",xbins,xmin,xmax);
TH1D *hsig_0p005 = new TH1D("hsig_0p005","hsig_0p005",xbins,xmin,xmax);
TH1D *hsig_0p05  = new TH1D("hsig_0p05","hsig_0p05",xbins,xmin,xmax);
TH1D *hsig_0p1   = new TH1D("hsig_0p1","hsig_0p1",xbins,xmin,xmax);
TH1D *hsig_0p2   = new TH1D("hsig_0p2","hsig_0p2",xbins,xmin,xmax);
TH1D *hsig_0p5   = new TH1D("hsig_0p5","hsig_0p5",xbins,xmin,xmax);
TH1D *hsig_1     = new TH1D("hsig_1","hsig_1",xbins,xmin,xmax);
TH1D *hsig_10    = new TH1D("hsig_10","hsig_10",xbins,xmin,xmax);
TH1D *hsig_100   = new TH1D("hsig_100","hsig_100",xbins,xmin,xmax);

TString a("Events/"); char b[20];  sprintf(b, "(%.2f",(xmax-xmin)/xbins); TString c(")");
TString ytitle = a + b + c;
TString xtitle="cos#it{#theta}";

TCut cut="1";
TString var="costheta_smear";

//project
tbkg->Project("hbkg",var,cut);
tsig_0p005->Project("hsig_0p005",var,cut);
tsig_0p05->Project("hsig_0p05",var,cut);
tsig_0p1->Project("hsig_0p1",var,cut);
tsig_0p2->Project("hsig_0p2",var,cut);
tsig_0p5->Project("hsig_0p5",var,cut);
tsig_1->Project("hsig_1",var,cut);
tsig_10->Project("hsig_10",var,cut);
tsig_100->Project("hsig_100",var,cut);

hbkg->SetLineColor(1);
hsig_0p005->SetLineColor(2);
hsig_0p05->SetLineColor(3);
hsig_0p1->SetLineColor(4);
hsig_0p2->SetLineColor(5);
hsig_0p5->SetLineColor(6);
hsig_1->SetLineColor(7);
hsig_10->SetLineColor(8);
hsig_100->SetLineColor(9);

cout<<"MDM=5MeV, Nevent = "<<hsig_0p005->Integral()<<", eff = "<<hsig_0p005->Integral()/1000000.<<endl;
cout<<"MDM=50MeV, Nevent = "<<hsig_0p05->Integral()<<", eff = "<<hsig_0p05->Integral()/1000000.<<endl;
cout<<"MDM=100MeV, Nevent = "<<hsig_0p1->Integral()<<", eff = "<<hsig_0p1->Integral()/1000000.<<endl;
cout<<"MDM=200MeV, Nevent = "<<hsig_0p2->Integral()<<", eff = "<<hsig_0p2->Integral()/1000000.<<endl;
cout<<"MDM=500MeV, Nevent = "<<hsig_0p5->Integral()<<", eff = "<<hsig_0p5->Integral()/1000000.<<endl;
cout<<"MDM=1GeV, Nevent = "<<hsig_1->Integral()<<", eff = "<<hsig_1->Integral()/1000000.<<endl;
cout<<"MDM=10GeV, Nevent = "<<hsig_10->Integral()<<", eff = "<<hsig_10->Integral()/1000000.<<endl;
cout<<"MDM=100GeV, Nevent = "<<hsig_100->Integral()<<", eff = "<<hsig_100->Integral()/1000000.<<endl;

hsig_0p005->Scale((hbkg->Integral())/(hsig_0p005->Integral()));
hsig_0p05->Scale((hbkg->Integral())/(hsig_0p05->Integral()));
hsig_0p1->Scale((hbkg->Integral())/(hsig_0p1->Integral()));
hsig_0p2->Scale((hbkg->Integral())/(hsig_0p2->Integral()));
hsig_0p5->Scale((hbkg->Integral())/(hsig_0p5->Integral()));
hsig_1->Scale((hbkg->Integral())/(hsig_1->Integral()));
hsig_10->Scale((hbkg->Integral())/(hsig_10->Integral()));
hsig_100->Scale((hbkg->Integral())/(hsig_100->Integral()));

hbkg->SetMaximum(2*(hbkg->GetMaximum()));
hbkg->SetMinimum(0.1);
hbkg->Draw("hist");
hbkg->GetXaxis()->SetTitle(xtitle);
hbkg->GetYaxis()->SetTitle(ytitle);
//hsig_0p005->Draw("hist,same");
hsig_0p05->Draw("hist,same");
hsig_0p1->Draw("hist,same");
hsig_0p2->Draw("hist,same");
hsig_0p5->Draw("hist,same");
hsig_1->Draw("hist,same");
hsig_10->Draw("hist,same");
hsig_100->Draw("hist,same");
hbkg->Draw("hist,same");

TLegend *legend = new TLegend(0.25,0.17,0.40,0.53,NULL,"brNDC");
//legend->AddEntry(hsig_0p005,"#it{M}_{DM}=0.005 GeV","F");
legend->AddEntry(hsig_0p05,"#it{M}_{DM}=0.05 GeV","F");
legend->AddEntry(hsig_0p1,"#it{M}_{DM}=0.1 GeV","F");
legend->AddEntry(hsig_0p2,"#it{M}_{DM}=0.2 GeV","F");
legend->AddEntry(hsig_0p5,"#it{M}_{DM}=0.5 GeV","F");
legend->AddEntry(hsig_1,"#it{M}_{DM}=1 GeV","F");
legend->AddEntry(hsig_10,"#it{M}_{DM}=10 GeV","F");
legend->AddEntry(hsig_100,"#it{M}_{DM}=100 GeV","F");
legend->AddEntry(hbkg,"Air","F");
setLegend(legend);
legend->Draw();

TPaveText* text = new TPaveText(0.2, 0.8, 0.5, 0.95, "NDC");
text->AddText(Form("Muon Energy = %s",MuonEnergy));
text->Draw();

gPad->RedrawAxis();

can->Update();
can->Print("../figure/costheta_muon" + (TString)MuonEnergy + ".eps");

//save Hist
TFile* fout = new TFile(Form("../root/Hist_muon%s.root",MuonEnergy),"recreate");
fout->cd();

hsig_0p005->Scale((100.)/(hsig_0p005->Integral()));
hsig_0p05->Scale((100.)/(hsig_0p05->Integral()));
hsig_0p1->Scale((100.)/(hsig_0p1->Integral()));
hsig_0p2->Scale((100.)/(hsig_0p2->Integral()));
hsig_0p5->Scale((100.)/(hsig_0p5->Integral()));
hsig_1->Scale((100.)/(hsig_1->Integral()));
hsig_10->Scale((100.)/(hsig_10->Integral()));
hsig_100->Scale((100.)/(hsig_100->Integral()));

int lastBinIndex = hbkg->GetNbinsX();

double lastBinError = hbkg->GetBinError(lastBinIndex);
cout<<"error of last bin before scale = "<<lastBinError<<endl;

//hbkg->Scale(3000);
if(strncmp(MuonEnergy,"1GeV",4)==0) hbkg->Scale(3000);
if(strncmp(MuonEnergy,"10GeV",5)==0) hbkg->Scale(300);
if(strncmp(MuonEnergy,"10MeV",5)==0) hbkg->Scale(30000);
if(strncmp(MuonEnergy,"100MeV",6)==0) hbkg->Scale(3000);

lastBinError = hbkg->GetBinError(lastBinIndex);
cout<<"error of last bin after scale = "<<lastBinError<<endl;

for (int i = 1; i <= hbkg->GetNbinsX(); i++) {
    double binContent = hbkg->GetBinContent(i);
    double binError = sqrt(binContent);
    hbkg->SetBinError(i, binError);
}

lastBinError = hbkg->GetBinError(lastBinIndex);
cout<<"error of last bin after reset poission error= "<<lastBinError<<endl;

TTree* tree = new TTree("tree","efficiency");

double nentry_DM0p005, nentry_DM0p05, nentry_DM0p1, nentry_DM0p2, nentry_DM0p5, nentry_DM1, nentry_DM10, nentry_DM100;
double nentry_bkg;

tree->Branch("nentry_DM0p005", &nentry_DM0p005, "nentry_DM0p005/D");
tree->Branch("nentry_DM0p05", &nentry_DM0p05, "nentry_DM0p05/D");
tree->Branch("nentry_DM0p1", &nentry_DM0p1, "nentry_DM0p1/D");
tree->Branch("nentry_DM0p2", &nentry_DM0p2, "nentry_DM0p2/D");
tree->Branch("nentry_DM0p5", &nentry_DM0p5, "nentry_DM0p5/D");
tree->Branch("nentry_DM1", &nentry_DM1, "nentry_DM1/D");
tree->Branch("nentry_DM10", &nentry_DM10, "nentry_DM10/D");
tree->Branch("nentry_DM100", &nentry_DM100, "nentry_DM100/D");
tree->Branch("nentry_bkg", &nentry_bkg, "nentry_bkg/D");

if(strcmp(MuonEnergy,"100MeV")==0) cut="costheta_smear<0.9";
nentry_bkg = tbkg->GetEntries(cut);
nentry_DM0p005 = tsig_0p005->GetEntries(cut);
nentry_DM0p05 = tsig_0p05->GetEntries(cut);
nentry_DM0p1 = tsig_0p1->GetEntries(cut);
nentry_DM0p2 = tsig_0p2->GetEntries(cut);
nentry_DM0p5 = tsig_0p5->GetEntries(cut);
nentry_DM1 = tsig_1->GetEntries(cut);
nentry_DM10 = tsig_10->GetEntries(cut);
nentry_DM100 = tsig_100->GetEntries(cut);

tree->Fill();
tree->Write();

hbkg->Write();
hsig_0p005->Write();
hsig_0p05->Write();
hsig_0p1->Write();
hsig_0p2->Write();
hsig_0p5->Write();
hsig_1->Write();
hsig_10->Write();
hsig_100->Write();

fout->Close();


/*
delete hsig_0p005;
delete hsig_0p05;
delete hsig_0p5;
delete hsig_0p2;
delete hsig_0p1;
delete hsig_1;
delete hsig_10;
delete hsig_100;

delete hbkg;

delete can;
*/

}
