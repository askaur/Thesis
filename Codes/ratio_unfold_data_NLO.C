double makeCanvas(TCanvas *c, TH1D *h_NLO, TH1D *h_NLO_less, TH1D *h_Data,TString plotname, bool twojet)
{
  c->Range(2.318121,0.4327759,3.346459,1.683612);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  c->SetLeftMargin(0.1546185);
  c->SetRightMargin(0.04417671);
  c->SetTopMargin(0.06684492);
  c->SetBottomMargin(0.1336898);
  c->SetLogx();

  h_NLO->Divide(h_Data); 
  h_NLO_less->Divide(h_Data);
    
  h_NLO->SetMarkerSize(0.75);
  h_NLO->SetMarkerColor(38);
  h_NLO->SetLineColor(38);
  h_NLO->SetMarkerStyle(20);
  h_NLO->SetLineWidth(2);
  
  h_NLO_less->SetMarkerSize(0.75);
  h_NLO_less->SetMarkerColor(kRed-3);
  h_NLO_less->SetLineColor(kRed-3);
  h_NLO_less->SetMarkerStyle(20);
  h_NLO_less->SetLineWidth(2);
  h_NLO_less->SetLineStyle(2);
     
  if (twojet) double max = 2000.0;
  else double max = 1680.0;
  h_NLO->GetYaxis()->SetRangeUser(0.65,1.35);
  h_NLO->GetXaxis()->SetRangeUser(300.0,max);
  h_NLO_less->GetXaxis()->SetRangeUser(300.0,max);
  h_NLO->SetTitle("");
  h_NLO->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_NLO->GetXaxis()->SetTitleOffset(1.04);
  h_NLO->GetXaxis()->SetTitleSize(0.06);
  h_NLO->GetXaxis()->SetMoreLogLabels();
  h_NLO->GetXaxis()->SetNoExponent();
  h_NLO->GetXaxis()->SetLabelSize(0.05);
  h_NLO->GetXaxis()->SetLabelOffset(0.018);
  h_NLO->GetXaxis()->SetTickLength(0.03);
  
  h_NLO->GetYaxis()->SetTitle("Unfolded/Measured");
  h_NLO->GetYaxis()->SetTitleOffset(1.00);
  h_NLO->GetYaxis()->SetTitleSize(0.06);
  h_NLO->GetYaxis()->SetLabelSize(0.05);
  h_NLO->GetYaxis()->SetTickLength(0.03);
  h_NLO->GetYaxis()->SetLabelOffset(0.008);
  h_NLO->GetYaxis()->SetNdivisions(509);
  h_NLO->GetYaxis()->SetDecimals(4);
  h_NLO->SetStats(0);
  h_NLO->Draw("e");
  h_NLO_less->Draw("esame");
  
  double x1 = 0.25; // 0.62 fixed
  double x2 = x1 + 0.31;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.24;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
   if (twojet) legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  else legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(h_NLO,"using JER","pel");
  legend1->AddEntry(h_NLO_less,"using 30% reduced JER","pel");
  legend1->Draw();
  
  tex = new TLatex(281,0.6058529,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  if (!twojet)
  { tex = new TLatex(1541,0.6058529,"1680");
     tex->SetTextFont(42);
   tex->SetTextSize(0.05);
   tex->SetLineWidth(2);
   tex->Draw();
  }
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");

  c->Draw();
  c->Update();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Ratio_Unfolding_data_NLO_" + plotname + ".gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Ratio_Unfolding_data_NLO_" + plotname + ".pdf");
}
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_unfold_data_NLO(){
  setTDRStyle();
  
  char figname[256];
    
  TFile *f_Data = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data/Rootfile_Data_HT_2_150/Hadd_PFJetHT_2_150.root");
  
  TFile *f_NLO_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_range_funcI.root");
  TFile *f_NLO_less_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_less_range_funcI.root"); 

  TFile *f_NLO_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_funcI.root");
  TFile *f_NLO_less_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_less_funcI.root"); 
    
  //*****************************************************************

  TH1D *h_NLO_2 = (TH1D*)f_NLO_2->Get("Unfolded_Data_NLO");
  TH1D *h_NLO_less_2 = (TH1D*)f_NLO_less_2->Get("Unfolded_Data_NLO");
  TH1D *h_Data_2 =  (TH1D*)f_Data->Get("Hadd_PFJetHT_2_150_2");
  
  TH1D *h_NLO_3 = (TH1D*)f_NLO_3->Get("Unfolded_Data_NLO");
  TH1D *h_NLO_less_3 = (TH1D*)f_NLO_less_3->Get("Unfolded_Data_NLO");
  TH1D *h_Data_3 =  (TH1D*)f_Data->Get("Hadd_PFJetHT_2_150_3");
    
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"2");
  makeCanvas(c,h_NLO_2,h_NLO_less_2,h_Data_2,figname,1);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"3");
  makeCanvas(c,h_NLO_3,h_NLO_less_3,h_Data_3,figname,0);
 }  
