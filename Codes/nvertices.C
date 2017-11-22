#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void nvertices(){
  setTDRStyle();
  TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Nvertices/MC_100_250_reweighting.root");
  TFile *f2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Nvertices/MC_250_500_reweighting.root");
  TFile *f3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Nvertices/MC_500_1000_reweighting.root");
  TFile *f4 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Nvertices/MC_1000_Inf_reweighting.root");

  TFile *file1 = new TFile("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Rootfile_HT_2_150/Nvertices.root","RECREATE");
  //*****************************************************************
  TH1F *h1_11 = dynamic_cast<TH1*>(f1->Get("jet/Data_distr"));
  TH1F *h1_12 = dynamic_cast<TH1*>(f2->Get("jet/Data_distr"));
  TH1F *h1_13 = dynamic_cast<TH1*>(f3->Get("jet/Data_distr"));
  TH1F *h1_14 = dynamic_cast<TH1*>(f4->Get("jet/Data_distr"));

  TH1F *h2_11 = dynamic_cast<TH1*>(f1->Get("jet/trupuMC"));
  TH1F *h2_12 = dynamic_cast<TH1*>(f2->Get("jet/trupuMC"));
  TH1F *h2_13 = dynamic_cast<TH1*>(f3->Get("jet/trupuMC"));
  TH1F *h2_14 = dynamic_cast<TH1*>(f4->Get("jet/trupuMC"));
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  c->Range(0,0,1,1);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetFrameBorderMode(0);
  
  // ------------>Primitives in pad: c1_2
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  c1_2->Draw();
  c1_2->cd();
  c1_2->Range(-0.1613298,-4.212891,1.034181,0.262741);
  c1_2->SetFillColor(0);
  c1_2->SetBorderMode(0);
  c1_2->SetBorderSize(2);
  c1_2->SetTickx(1);
  c1_2->SetTicky(1);
  c1_2->SetLeftMargin(0.1349463);
  c1_2->SetRightMargin(0.02859079);
  c1_2->SetTopMargin(0.07996238);
  c1_2->SetBottomMargin(0.04756668);
  c1_2->SetFrameBorderMode(0);
  c1_2->SetFrameBorderMode(0);
  
  h1_11->Add(h1_12);
  h1_11->Add(h1_13);
  h1_11->Add(h1_14);

  h2_11->SetFillColor(38);
  h2_11->SetLineColor(38);  
  h2_11->SetMarkerColor(38);
 
  h1_11->SetMarkerStyle(20);
  h1_11->SetMarkerColor(kBlack);
  h1_11->SetMarkerSize(0.75);
  h1_11->SetLineWidth(2);
  h1_11->SetStats(0);
  h2_11->SetStats(0);
  
  double n1_11 = h1_11->Integral();
  double n2_11 = h2_11->Integral();
  double n2_12 = h2_12->Integral();
  double n2_13 = h2_13->Integral();
  double n2_14 = h2_14->Integral();
 
  h2_11->Scale(1.0/n2_11);
  h2_12->Scale(1.0/n2_12);
  h2_13->Scale(1.0/n2_13);
  h2_14->Scale(1.0/n2_14);
  
  h2_11->Add(h2_12);
  h2_11->Add(h2_13);
  h2_11->Add(h2_14);
  
  h2_11->GetYaxis()->SetRangeUser(0.0,0.25);
  h2_11->GetXaxis()->SetRangeUser(0.0,50.0);
  h2_11->SetTitle(" ");
  h2_11->GetYaxis()->SetTitle("arb. unit");
  h2_11->GetXaxis()->SetTickLength(0.03);
  h2_11->GetXaxis()->SetLabelOffset(999);
  h2_11->GetXaxis()->SetNdivisions(509);    
  h2_11->GetYaxis()->SetTitleOffset(1.00);
  h2_11->GetYaxis()->SetTitleSize(0.07);
  h2_11->GetYaxis()->SetLabelSize(0.055);
  h2_11->GetYaxis()->SetLabelOffset(0.01);
  h2_11->GetYaxis()->SetDecimals(4);
  h2_11->SetStats(0);

  h2_11->Draw("hist][");
  
  h1_11->Draw("psame");
  
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30; 
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.22; 

  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,""," ");
  legend1->AddEntry(h1_11,"Data","pel");
  legend1->AddEntry(h2_11,"MG5+P6");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.06);
  pt3.DrawLatex(0.69,0.94,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  c1_2->Modified();
  c->cd();
                                                         
  TH1F *hrat1_11 = h1_11->Clone("hrat1_11");
 
  hrat1_11->Divide(h2_11);
     
  hrat1_11->SetMarkerColor(kBlack);
  hrat1_11->SetMarkerStyle(20);
  hrat1_11->SetLineColor(kBlack);
  hrat1_11->SetMarkerSize(0.75);

  c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  c1_1->Draw();
  c1_1->cd();
  c1_1->Range(-0.1610322,-0.9528421,1.029367,2.140632);
  c1_1->SetFillColor(0);
  c1_1->SetBorderMode(0);
  c1_1->SetBorderSize(1);
  c1_1->SetTickx(1);
  c1_1->SetTicky(1);
  c1_1->SetLeftMargin(0.1352758);
  c1_1->SetRightMargin(0.02467011);
  c1_1->SetTopMargin(0.04546073);
  c1_1->SetBottomMargin(0.3080169);
  c1_1->SetFrameBorderMode(0);
  c1_1->SetFrameBorderMode(0);
  
  std::cout<<"Debug1"<<std::endl;
  
  hrat1_11->GetYaxis()->SetRangeUser(-0.5,2.0);
  hrat1_11->GetXaxis()->SetRangeUser(0.0,50.0);
  hrat1_11->SetTitle("  ");
  hrat1_11->GetXaxis()->SetTickLength(0.07);
  hrat1_11->GetXaxis()->SetTitle("True interactions");
  hrat1_11->GetXaxis()->SetTitleSize(0.15);
  hrat1_11->GetXaxis()->SetTitleOffset(0.98);
  hrat1_11->GetXaxis()->SetMoreLogLabels();
  hrat1_11->GetXaxis()->SetNoExponent();
  hrat1_11->GetXaxis()->SetLabelSize(0.13);
  hrat1_11->GetYaxis()->SetNdivisions(507);
  hrat1_11->GetYaxis()->SetLabelSize(0.12);
  hrat1_11->GetYaxis()->SetTitle("Data/MC");
  hrat1_11->GetYaxis()->SetTitleOffset(0.48);
  hrat1_11->GetYaxis()->SetTitleSize(0.15);
  hrat1_11->GetYaxis()->SetDecimals(4);
  hrat1_11->GetXaxis()->SetLabelOffset(0.02);
  hrat1_11->GetYaxis()->SetLabelOffset(0.01);
  hrat1_11->GetXaxis()->SetNdivisions(509);  
  hrat1_11->SetStats(0);
  
  hrat1_11->Draw("P");
  TLine l(0.0,1.0,50,1.0);
  l.SetLineWidth(2);
  l.SetLineColor(kGray+2);
  l.SetLineStyle(5);
  l.Draw("same");
  
  c->Draw();
  c->Update();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Nvertices.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Nvertices.pdf");
  c->Write();
  
  file1->Write();
  file1->Close();
}  
