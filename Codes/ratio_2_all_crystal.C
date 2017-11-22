#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void ratio_2_all_crystal(){
  setTDRStyle(); 
   
    TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_range_funcI.root");
   
  TFile *f1_less = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_less_range_funcI.root");
     
  TFile *f_gen = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/New_RootFiles/MC_total_jethadd_matching.root");
   
  //*****************************************************************
  
  TH1D *h_NLO_true = (TH1D*)f1->Get("True");
  
  TH1D *h_NLO_meas = (TH1D*)f1->Get("Measured");

  TH1D *h_NLO_true_less = (TH1D*)f1_less->Get("True");
  
  TH1D *h_NLO_meas_less = (TH1D*)f1_less->Get("Measured");
 
  TH1D *hMC_Reco  = (TH1D*)f_gen->Get("jet/PFMCJetHt_2_2");
  
 TH1D *hMC_Reco_smear  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_smeared_2");
  
  TH1D *hMC_Reco_smear_less  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_smeared_2_70");
      
  TH1D *hMC_Gen  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_2");
    
  TCanvas *c = new TCanvas("c", "",500,400);
  c->Range(2.318121,0.4327759,3.346459,1.683612);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  //c->SetGridx(1);
  //c->SetGridy(1);
  c->SetLeftMargin(0.1546185);
  c->SetRightMargin(0.04417671);
  c->SetTopMargin(0.06684492);
  c->SetBottomMargin(0.1336898);
  c->SetLogx();
 
  h_NLO_meas->Divide(h_NLO_true);
  hMC_Reco->Divide(hMC_Gen);  
  hMC_Reco_smear->Divide(hMC_Gen);  
  hMC_Reco_smear_less->Divide(hMC_Gen);        
  h_NLO_meas_less->Divide(h_NLO_true_less);
  
  h_NLO_meas->SetMarkerSize(0.75);
  h_NLO_meas->SetMarkerColor(kRed-3);
  h_NLO_meas->SetLineColor(kRed-3);
  h_NLO_meas->SetLineWidth(2);
  h_NLO_meas->SetMarkerStyle(20);
  
  h_NLO_meas_less->SetMarkerSize(0.75);
  h_NLO_meas_less->SetMarkerColor(kGreen+2);
  h_NLO_meas_less->SetLineColor(kGreen+2);
  h_NLO_meas_less->SetLineWidth(2);
  h_NLO_meas_less->SetMarkerStyle(20);
  
  hMC_Reco->SetMarkerSize(0.75);
  hMC_Reco->SetMarkerColor(38);
  hMC_Reco->SetLineColor(38);
  hMC_Reco->SetMarkerStyle(4);
  hMC_Reco->SetLineWidth(2);
  
  hMC_Reco_smear->SetMarkerSize(0.75);
  hMC_Reco_smear->SetMarkerColor(kViolet-6);
  hMC_Reco_smear->SetLineColor(kViolet-6);
  hMC_Reco_smear->SetLineStyle(1);
  hMC_Reco_smear->SetLineWidth(2);
  
  hMC_Reco_smear_less->SetMarkerSize(0.75);
  hMC_Reco_smear_less->SetMarkerColor(kMagenta);
  hMC_Reco_smear_less->SetLineColor(kMagenta);
  hMC_Reco_smear_less->SetLineStyle(1);
  hMC_Reco_smear_less->SetLineWidth(2);
    
  h_NLO_meas->GetYaxis()->SetRangeUser(0.96,1.08);
  h_NLO_meas->GetXaxis()->SetRangeUser(300.0,2000.0);
  h_NLO_meas_less->GetXaxis()->SetRangeUser(300.0,2000.0);  
  hMC_Reco->GetXaxis()->SetRangeUser(300.0,2000.0);
  hMC_Reco_smear->GetXaxis()->SetRangeUser(300.0,2000.0);
  hMC_Reco_smear_less->GetXaxis()->SetRangeUser(300.0,2000.0);
  h_NLO_meas->SetTitle("");
  h_NLO_meas->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_NLO_meas->GetXaxis()->SetTitleOffset(1.04);
  h_NLO_meas->GetXaxis()->SetTitleSize(0.06);
  h_NLO_meas->GetXaxis()->SetMoreLogLabels();
  h_NLO_meas->GetXaxis()->SetNoExponent();
  h_NLO_meas->GetXaxis()->SetLabelSize(0.05);
  h_NLO_meas->GetXaxis()->SetLabelOffset(0.022);
  h_NLO_meas->GetXaxis()->SetTickLength(0.03);
  
  h_NLO_meas->GetYaxis()->SetTitle("Ratios");
  h_NLO_meas->GetYaxis()->SetTitleOffset(1.19);
  h_NLO_meas->GetYaxis()->SetTitleSize(0.06);
  h_NLO_meas->GetYaxis()->SetLabelSize(0.05);
  h_NLO_meas->GetYaxis()->SetTickLength(0.03);
  h_NLO_meas->GetYaxis()->SetLabelOffset(0.008);
  
  h_NLO_meas->SetStats(0);
  h_NLO_meas->Draw("hist][");
  h_NLO_meas_less->Draw("histsame][");  
  hMC_Reco->Draw("histsame][");  
  hMC_Reco_smear->Draw("histsame][");    
  hMC_Reco_smear_less->Draw("histsame][");      
  
  double x1 = 0.53; // 0.62 fixed
  double x2 = x1 + 0.1;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.1;
   
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 2","");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
     
  tex = new TLatex(281,0.9503679,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();  

  c->Draw();
  c->Update();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Ratio_all_2_crystal.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Ratio_all_2_crystal.pdf");
  
  }
