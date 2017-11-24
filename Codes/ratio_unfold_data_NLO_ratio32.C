#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_unfold_data_NLO_ratio32(){
  setTDRStyle();
  
  TFile *f2_NLO = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_range_funcI.root");
  
   TFile *f2_NLO_less = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_less_range_funcI.root"); 
  
  TFile *f3_NLO = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_funcI.root");
  
   TFile *f3_NLO_less = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_less_funcI.root"); 
     
  TFile *f_Data = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Rootfile_HT_2_150/Ratio_32_Data_HT_2_150.root");
  
  TFile *f32_unf = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_ratio_32_range_funcII.root");
  
  TFile *f32_unf_less = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_ratio_32_range_funcII_less.root");
  
  //*****************************************************************

  TH1D *h2_NLO = (TH1D*)f2_NLO->Get("Unfolded_Data_NLO");
  
  TH1D *h2_NLO_less = (TH1D*)f2_NLO_less->Get("Unfolded_Data_NLO");
    
  TH1D *h3_NLO = (TH1D*)f3_NLO->Get("Unfolded_Data_NLO");
  
  TH1D *h3_NLO_less = (TH1D*)f3_NLO_less->Get("Unfolded_Data_NLO");
 
  TH1D *h_Data =  (TH1D*)f_Data->Get("Ratio_32_Data_Symm");
  
  TH1D *hData_unf =  (TH1D*)f32_unf->Get("Unfolded_ratio_32_symm_smear2");
  
  TH1D *hData_unf_less =  (TH1D*)f32_unf_less->Get("Unfolded_ratio_32_symm_smear2");
      
  const double new_bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};

  TH1D *hData_unf_final = new TH1D("R32","R32",56.0,new_bins);
  hData_unf_final->Sumw2();
  
  TH1D *hData_unf_final_less = new TH1D("R32_less","R32_less",56.0,new_bins);
  hData_unf_final_less->Sumw2();
  
  TCanvas *c=new TCanvas("c","",500,400);
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

  h3_NLO->Divide(h2_NLO); 
  h3_NLO_less->Divide(h2_NLO_less);   
 
  double nbinsx = hData_unf_final->GetXaxis()->GetNbins();
  
  for (double i = 1.; i < nbinsx; i++)
  {
    double xcontent_f = h3_NLO->GetBinContent(i);
    double xcontent_f_less = h3_NLO_less->GetBinContent(i);    
    double xcontent = hData_unf->GetBinContent(i);
    
    double xerr = hData_unf->GetBinError(i)/hData_unf->GetBinContent(i);
    double xerr_less = hData_unf_less->GetBinError(i)/hData_unf_less->GetBinContent(i);    
   
    if ( xcontent == 0.0)
    double newerr = 100;
    else
    double newerr = xerr*xcontent_f;
    
    if ( xcontent == 0.0)
    double newerr_less = 100;
    else
    double newerr_less = xerr_less*xcontent_f_less;
    
    hData_unf_final->SetBinContent(i,xcontent_f);
    hData_unf_final->SetBinError(i,newerr);
    
    hData_unf_final_less->SetBinContent(i,xcontent_f_less);
    hData_unf_final_less->SetBinError(i,newerr_less);
  }  
 
  hData_unf_final->Divide(h_Data);
  hData_unf_final_less->Divide(h_Data); 
   
  hData_unf_final->SetMarkerSize(0.75);
  hData_unf_final->SetMarkerColor(38);
  hData_unf_final->SetLineColor(38);
  hData_unf_final->SetMarkerStyle(20);
  hData_unf_final->SetLineWidth(2);
  
  hData_unf_final_less->SetMarkerSize(0.75);
  hData_unf_final_less->SetMarkerColor(kRed-3);
  hData_unf_final_less->SetLineColor(kRed-3);
  hData_unf_final_less->SetMarkerStyle(20);
  hData_unf_final_less->SetLineStyle(2);
  hData_unf_final_less->SetLineWidth(2);
  
  double max = 1680.0;
  hData_unf_final->GetYaxis()->SetRangeUser(0.65,1.35);
  hData_unf_final->GetXaxis()->SetRangeUser(300.0,max);
  hData_unf_final_less->GetXaxis()->SetRangeUser(300.0,max);
  hData_unf_final->SetTitle("");
  hData_unf_final->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hData_unf_final->GetXaxis()->SetTitleOffset(1.04);
  hData_unf_final->GetXaxis()->SetTitleSize(0.06);
  hData_unf_final->GetXaxis()->SetMoreLogLabels();
  hData_unf_final->GetXaxis()->SetNoExponent();
  hData_unf_final->GetXaxis()->SetLabelSize(0.05);
  hData_unf_final->GetXaxis()->SetLabelOffset(0.018);
  hData_unf_final->GetXaxis()->SetTickLength(0.03);
  
  hData_unf_final->GetYaxis()->SetTitle("Unfolded/Measured");
  hData_unf_final->GetYaxis()->SetTitleOffset(1.00);
  hData_unf_final->GetYaxis()->SetTitleSize(0.06);
  hData_unf_final->GetYaxis()->SetLabelSize(0.05);
  hData_unf_final->GetYaxis()->SetTickLength(0.03);
  hData_unf_final->GetYaxis()->SetLabelOffset(0.008);
  hData_unf_final->GetYaxis()->SetNdivisions(509);
  hData_unf_final->GetYaxis()->SetDecimals(4);
  hData_unf_final->SetStats(0);
  hData_unf_final->Draw("e");
  hData_unf_final_less->Draw("esame");
  
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
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  legend1->AddEntry(hData_unf_final,"using JER","pel");
  legend1->AddEntry(hData_unf_final_less,"using 30% reduced JER","pel");
  legend1->Draw();
  
  tex = new TLatex(281,0.6058529,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(1541,0.6058529,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
   
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");

  c->Draw();
  c->Update();
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Ratio_Unfolding_data_NLO_ratio32.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Ratio_Unfolding_data_NLO_ratio32.pdf");

}  
