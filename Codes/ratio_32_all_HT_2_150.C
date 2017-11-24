#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_32_all_HT_2_150(){
  setTDRStyle();
  
  TFile *f32_data = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Rootfile_HT_2_150/Ratio_32_Data_HT_2_150.root");
  TFile *f32_mad = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Rootfile_HT_2_150/Ratio_32_Mad_HT_2_150.root");
  TFile *f32_nlo = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Rootfile_HT_2_150/Ratio_32_NLO_HT_2_150.root");
  
  //****************************************************************
  TCanvas *c32_data = (TCanvas*)f32_data->Get("c1");
  TCanvas *c32_mad = (TCanvas*)f32_mad->Get("c1");
    
  TGraphAsymmErrors *h_Data_32 = (TGraphAsymmErrors*)c32_data->GetListOfPrimitives()->FindObject("Graph0");   
  TGraphAsymmErrors *h_MC_32 = (TGraphAsymmErrors*)c32_mad->GetListOfPrimitives()->FindObject("Graph0"); 
  TH1D *h_NLO_32 = dynamic_cast<TH1*>(f32_nlo->Get("h_NLO_32"));
  
  TCanvas *c = new TCanvas("c", "",500,400);
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

  h_Data_32->SetMarkerSize(0.75);
  h_Data_32->SetMarkerStyle(20);
  h_Data_32->SetLineWidth(2);
  
  h_MC_32->SetMarkerSize(0.7);
  h_MC_32->SetMarkerStyle(24);
  h_MC_32->SetMarkerColor(kRed-3);
  h_MC_32->SetLineColor(kRed-3);
  h_MC_32->SetLineWidth(2);
  
  h_NLO_32->SetLineWidth(2);
  h_NLO_32->SetLineColor(38);
           
  h_NLO_32->GetYaxis()->SetRangeUser(0.05,0.35);
  h_NLO_32->GetXaxis()->SetRangeUser(300.,1680.);
  h_NLO_32->SetTitle(" ");
  h_NLO_32->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_NLO_32->GetXaxis()->SetTitleOffset(1.04);
  h_NLO_32->GetXaxis()->SetTitleSize(0.06);
  h_NLO_32->GetXaxis()->SetMoreLogLabels();
  h_NLO_32->GetXaxis()->SetNoExponent();
  h_NLO_32->GetXaxis()->SetLabelSize(0.05);
  h_NLO_32->GetXaxis()->SetLabelOffset(0.018);
  h_NLO_32->GetXaxis()->SetTickLength(0.03);
  
  h_NLO_32->GetYaxis()->SetTitle("Ratio");
  h_NLO_32->GetYaxis()->SetTitleOffset(1.15);
  h_NLO_32->GetYaxis()->SetTitleSize(0.06);
  h_NLO_32->GetYaxis()->SetLabelSize(0.05);
  h_NLO_32->GetYaxis()->SetTickLength(0.03);
  h_NLO_32->GetYaxis()->SetLabelOffset(0.008);
  h_NLO_32->GetYaxis()->SetNdivisions(509);
  h_NLO_32->GetYaxis()->SetDecimals(4);   
  
  h_NLO_32->Draw("hist]["); 
  h_MC_32->Draw("PZsame");
  h_Data_32->Draw("PZsame");
     
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
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  legend1->AddEntry(h_Data_32,"Data","pel");
  legend1->AddEntry(h_NLO_32,"CT10-NLO", "l");
  legend1->AddEntry(h_MC_32,"MG5+P6","pel");
  legend1->Draw();
  
  tex = new TLatex(283.6248,0.02792643,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
      
  tex = new TLatex(1521.18,0.02792643,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  c->Draw();
  c->Update();

  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Ratio_32_all_HT_2_150.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Ratio_32_all_HT_2_150.pdf");
  
}  
