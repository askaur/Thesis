double makeCanvas (TCanvas *c, TPad *c1_2, TPad *c1_1, TH1D *hData, TH1D *hMC, TH1D *hNLO, bool twojet,  TString plotname)
{
  hData->Scale(1/19711.225);
  hData->Scale(1.0, "width");
  
  hMC->Scale(1.0,"width");
  
  c->Range(0,0,1,1);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetFrameBorderMode(0);
  
  c1_2->Draw();
  c1_2->cd();
  c1_2->Range(-0.1613298,-4.212891,1.034181,0.262741);
  c1_2->SetFillColor(0);
  c1_2->SetBorderMode(0);
  c1_2->SetBorderSize(2);
  c1_2->SetLogy();
  c1_2->SetLogx();
  c1_2->SetTickx(1);
  c1_2->SetTicky(1);
  c1_2->SetLeftMargin(0.1349463);
  c1_2->SetRightMargin(0.04601225);
  c1_2->SetTopMargin(0.07996238);
  c1_2->SetBottomMargin(0.04756668);
  c1_2->SetFrameBorderMode(0);
  c1_2->SetFrameBorderMode(0);
  
  hNLO->SetFillColor(38);
  hNLO->SetMarkerStyle(20);
  hNLO->SetMarkerColor(38);
  hNLO->SetLineColor(38); 
  
  hMC->SetMarkerSize(0.75);
  hMC->SetMarkerColor(kRed-3);
  hMC->SetLineColor(kRed-3);
  hMC->SetMarkerStyle(4);
  hMC->SetLineWidth(2);    
 
  hData->SetMarkerStyle(20);
  hData->SetMarkerColor(kBlack);
  hData->SetLineColor(kBlack); 
  hData->SetMarkerSize(0.75);
  hData->SetLineWidth(2);  

  hMC->GetXaxis()->SetRangeUser(200.0,2000.0);  
  hData->GetXaxis()->SetRangeUser(200.0,2000.0);
  hNLO->GetXaxis()->SetRangeUser(200.0,2000.0);
  
  hNLO->GetYaxis()->SetRangeUser(0.0000001,1000.);
  hNLO->SetTitle("");//Inclusive 2-jet");
  hNLO->SetTitleSize(1.0);
  hNLO->GetXaxis()->SetTitle("");
  hNLO->GetYaxis()->SetTitle("d#sigma/d(#it{H}_{T,2}/2) (pb/GeV)");
  hNLO->GetXaxis()->SetTickLength(0.03);
  hNLO->GetXaxis()->SetLabelOffset(999);
  hNLO->GetXaxis()->SetNdivisions(509);    
  hNLO->GetYaxis()->SetTitleOffset(1.12);
  hNLO->GetYaxis()->SetTitleSize(0.06);
  hNLO->GetYaxis()->SetLabelSize(0.055);
  hNLO->SetStats(0);
  hData->SetStats(0);
  hMC->SetStats(0);
  hNLO->Draw("hist");
  hData->Draw("esame");
  hMC->Draw("esame");
  
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30; 
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.28; 
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  if (twojet) legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  else legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hData,"Data","pel");
  legend1->AddEntry(hNLO,"Theory (NLO)", "f");
  legend1->AddEntry(hMC,"MG5+P6","pel");
  legend1->Draw();
  
  double xmin = 167;
  double xmax = xmin + 30.0;
  double ymin = 0.4;
  double ymax = ymin + 30.5;
  
  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  tex = new TLatex(167.3491,0.5659104,"10^{0}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.06);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(167.3491,5.975002,"10^{1}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.06);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.06);
  pt3.DrawLatex(0.67,0.94,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  c1_2->Modified();
  c->cd();

  TH1D *hrat_NLO = hData->Clone("hrat_NLO");
  TH1D *hrat_MC = hData->Clone("hrat_MC");
 
  hrat_NLO->Divide(hNLO);
  hrat_MC->Divide(hMC);
    
  hrat_NLO->SetMarkerColor(38);
  hrat_NLO->SetMarkerStyle(20);
  hrat_NLO->SetLineColor(38);
  hrat_NLO->SetMarkerSize(0.75);
  
  hrat_MC->SetMarkerColor(kRed-3);
  hrat_MC->SetMarkerStyle(4);
  hrat_MC->SetLineColor(kRed-3);
  hrat_MC->SetMarkerSize(0.75);
  
  c1_1->Draw();
  c1_1->cd();
  c1_1->Range(-0.1610322,-0.9528421,1.029367,2.140632);
  c1_1->SetFillColor(0);
  c1_1->SetBorderMode(0);
  c1_1->SetBorderSize(1);
  c1_1->SetLogx();
  c1_1->SetTickx(1);
  c1_1->SetTicky(1);
  c1_1->SetLeftMargin(0.1352758);
  c1_1->SetRightMargin(0.04601225);
  c1_1->SetTopMargin(0.04546073);
  c1_1->SetBottomMargin(0.3080169);
  c1_1->SetFrameBorderMode(0);
  c1_1->SetFrameBorderMode(0);
   
  hrat_NLO->GetYaxis()->SetRangeUser(0.0,2.5);
  //hrat_NLO->GetXaxis()->SetRangeUser(300.0,2000.0);
  hrat_NLO->GetXaxis()->SetRangeUser(200.0,2000.0);
  hrat_NLO->SetTitle("");
  hrat_NLO->GetXaxis()->SetTickLength(0.07);
  hrat_NLO->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hrat_NLO->GetXaxis()->SetTitleSize(0.14);
  hrat_NLO->GetXaxis()->SetTitleOffset(1.02);
  hrat_NLO->GetXaxis()->SetMoreLogLabels();
  hrat_NLO->GetXaxis()->SetNoExponent();
  hrat_NLO->GetXaxis()->SetLabelSize(0.13);
  hrat_NLO->GetYaxis()->SetNdivisions(507);
  hrat_NLO->GetYaxis()->SetLabelSize(0.12);
  hrat_NLO->GetYaxis()->SetTitle("Ratio");
  hrat_NLO->GetYaxis()->SetTitleOffset(0.48);
  hrat_NLO->GetYaxis()->SetTitleSize(0.14);
  hrat_NLO->GetYaxis()->SetDecimals(4);
  hrat_NLO->GetXaxis()->SetLabelOffset(0.032);
  hrat_NLO->GetYaxis()->SetLabelOffset(0.013);
  hrat_NLO->GetXaxis()->SetNdivisions(509);  
  hrat_NLO->SetStats(0);
  
  hrat_NLO->SetStats(0);
  hrat_MC->SetStats(0);
  
  hrat_NLO->Draw("PE1");
  hrat_MC->Draw("PE1same");
    
  TLegend *legend1=new TLegend(0.51,0.67,0.71,0.86);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.08);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry(hrat_NLO,"Data/Theory","pel");
  legend1->AddEntry(hrat_MC,"Data/MG5+P6","pel");
  legend1->Draw();
  
  //TLine l(300.0,1.0,2000.0,1.0);
  TLine l(200.0,1.0,2000.0,1.0);
  l.SetLineWidth(2);
  l.SetLineColor(kGray+2);
  l.SetLineStyle(5);
  l.Draw("same");
  
  //tex = new TLatex(279.8803,-0.5105477,"300");
  tex = new TLatex(183.8389,-0.5105477,"200");
  tex->SetTextFont(42);
  tex->SetTextSize(0.13);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c->Draw();
  c->Update();
 
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_all_" + plotname + "_HT_2_150.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_all_" + plotname + "_HT_2_150.pdf"); 
 
}
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_comparison_all_HT_2_150(){
  setTDRStyle();
  
  TFile *fdata = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data/Rootfile_Data_HT_2_150/Hadd_PFJetHT_2_150.root");
  TFile *fNLO_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Rootfile_NLO_HT_2_150/Theory_plot_2_HT_2_150.root");
  TFile *fNLO_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Rootfile_NLO_HT_2_150/Theory_plot_3_HT_2_150.root");
  TFile *fMC = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/Rootfile_HT_2_150/Hadd_PFMCJetHT_2_150.root");
  
  char figname[256];
  //*****************************************************************
  
  TH1D *hData_2 = (TH1D*)fdata->Get("Hadd_PFJetHT_2_150_2");
  TH1D *hNLO_2 =  (TH1D*)fNLO_2->Get("NLO cross_section 2");
  TH1D *hMC_2 =  (TH1D*)fMC->Get("Hadd_PFMCJetHT_2_150_2");
  
  TH1D *hData_3 = (TH1D*)fdata->Get("Hadd_PFJetHT_2_150_3");
  TH1D *hNLO_3 =  (TH1D*)fNLO_3->Get("NLO cross_section 3");
  TH1D *hMC_3 =  (TH1D*)fMC->Get("Hadd_PFMCJetHT_2_150_3");
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  sprintf(figname,"2");
  makeCanvas(c,c1_2,c1_1,hData_2,hMC_2,hNLO_2,1,figname);
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  sprintf(figname,"3");
  makeCanvas(c,c1_2,c1_1,hData_3, hMC_3, hNLO_3,0,figname);
  
}  
