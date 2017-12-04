#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void comparison_data_MC_EW(){
  setTDRStyle();

  TFile *f_gen = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/Rootfile_HT_2_150/Hadd_GenMCJetHT_2_150_nopileup_New.root");
    
  TFile *fdata_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_range_funcI.root");
  
  TFile *fTotal_Unc_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Exp_Unc/RootFiles_HT_2_150/Exp_Unc_all_2_sources_NP.root");
  
  TFile *fdata_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_funcI.root");

  TFile *fTotal_Unc_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Exp_Unc/RootFiles_HT_2_150/Exp_Unc_all_3_sources_NP.root");
    
  TFile *fEW_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/EW_2.root");
      
  //*****************************************************************
  
  TH1D *hData_2 = (TH1D*)fdata_2->Get("Unfolded_Data_NLO");
  TH1D *hTotal_Unc_up_2 =  (TH1D*)fTotal_Unc_2->Get("Total_up");
  TH1D *hTotal_Unc_down_2 =  (TH1D*)fTotal_Unc_2->Get("Total_down");
  TH1D *hGen_2  = (TH1D*)f_gen->Get("Hadd_GenMCJetHT_2_150_2_nopileup_New");
   
  TH1D *hData_3 = (TH1D*)fdata_3->Get("Unfolded_Data_NLO");
  TH1D *hTotal_Unc_up_3 =  (TH1D*)fTotal_Unc_3->Get("Total_up");
  TH1D *hTotal_Unc_down_3 =  (TH1D*)fTotal_Unc_3->Get("Total_down");
  TH1D *hGen_3  = (TH1D*)f_gen->Get("Hadd_GenMCJetHT_2_150_3_nopileup_New");
  TH1D *hEW_2 = (TH1D*)fEW_2->Get("EW_2_M");
   
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
  
  TH1D *hData_2_new = new TH1D("Data_new_2","Data_new_2",arr_size-1,bins);
  hData_2_new->Sumw2();
  
  TH1D *hData_3_new = new TH1D("Data_new_3","Data_new_3",arr_size-1,bins);
  hData_3_new->Sumw2();
  
  TH1D *hGen_2_new = new TH1D("Gen_new_2","Gen_new_2",arr_size-1,bins);
  
  
  for (int i = 1; i <= arr_size-1  ; i++)
    {
      double content_2 = hData_2->GetBinContent(i);
      double ew_factor_2 = hEW_2->GetBinContent(i);
      hData_2_new->SetBinContent(i,content_2);
      if (hTotal_Unc_up_2->GetBinContent(i) > hTotal_Unc_down_2->GetBinContent(i)) double err_2 = hTotal_Unc_up_2->GetBinContent(i);
      else double err_2 = hTotal_Unc_down_2->GetBinContent(i);
      hData_2_new->SetBinError(i,err_2*content_2);  
      
      double ew_factor_2 = hEW_2->GetBinContent(i);
      hGen_2_new->SetBinContent(i,hGen_2->GetBinContent(i) * ew_factor_2); 
     
      double content_3 = hData_3->GetBinContent(i);
      hData_3_new->SetBinContent(i,content_3);
      if (hTotal_Unc_up_3->GetBinContent(i) > hTotal_Unc_down_3->GetBinContent(i)) double err_3 = hTotal_Unc_up_3->GetBinContent(i);
      else double err_3 = hTotal_Unc_down_3->GetBinContent(i);
      hData_3_new->SetBinError(i,err_3*content_3);  
    
    }
  /*for (int i = 1; i <= arr_size-1  ; i++)
    { 
    double np_factor_2 = hNP_2->GetBinContent(i);
    cout<<" Bin : "<<i<<" Pre : "<<hNLO_2->GetBinContent(i)<<" Factor : "<< np_factor_2 << " New : "<<hGen_2->GetBinContent(i)<<endl;
    }
  */
  hData_2_new->Scale(1/19711.225);
  hData_2_new->Scale(1.0, "width");    
  
  hData_3_new->Scale(1/19711.225);
  hData_3_new->Scale(1.0, "width");
  
  hGen_2_new->Scale(1.0,"width");    
  hGen_3->Scale(1.0,"width");    
  
  // hGen_2->Print("all"); 
   
  hGen_2_new->SetMarkerStyle(20);
  hGen_2_new->SetMarkerColor(38);
  hGen_2_new->SetLineColor(38); 
  hGen_2_new->SetLineWidth(2);  
  
  hData_2_new->SetMarkerStyle(20);
  hData_2_new->SetMarkerColor(kBlack);
  hData_2_new->SetLineColor(kBlack); 
  hData_2_new->SetMarkerSize(0.75);
    
  hGen_3->SetMarkerStyle(20);
  hGen_3->SetMarkerColor(kMagenta);
  hGen_3->SetLineColor(kMagenta); 
  hGen_3->SetLineWidth(2);   
  
  hData_3_new->SetMarkerStyle(22);
  hData_3_new->SetMarkerColor(kBlack);
  hData_3_new->SetLineColor(kBlack); 
  hData_3_new->SetMarkerSize(0.75);
  
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
  c->SetLogy();
  
  hData_2_new->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hData_2_new->GetXaxis()->SetTitleOffset(1.04);
  hData_2_new->GetXaxis()->SetTitleSize(0.06);
  hData_2_new->GetXaxis()->SetMoreLogLabels();
  hData_2_new->GetXaxis()->SetNoExponent();
  hData_2_new->GetXaxis()->SetLabelSize(0.05);
  hData_2_new->GetXaxis()->SetLabelOffset(0.018);
  hData_2_new->GetXaxis()->SetTickLength(0.03);
  
  hData_2_new->GetYaxis()->SetTitle("d#sigma/d(#it{H}_{T,2}/2) (pb/GeV)");
  hData_2_new->GetYaxis()->SetTitleOffset(1.1);
  hData_2_new->GetYaxis()->SetTitleSize(0.06);
  hData_2_new->GetYaxis()->SetLabelSize(0.05);
  hData_2_new->GetYaxis()->SetTickLength(0.03);
  hData_2_new->GetYaxis()->SetLabelOffset(0.008);
  hData_2_new->GetYaxis()->SetNdivisions(508);
  
  hData_2_new->GetXaxis()->SetRangeUser(300.0,2000.0);
  hGen_2_new->GetXaxis()->SetRangeUser(300.0,2000.0);
  hData_3_new->GetXaxis()->SetRangeUser(300.0,1680.0);//2000.0);
  hGen_3->GetXaxis()->SetRangeUser(300.0,1680.0);//2000.0);
  hData_2_new->GetYaxis()->SetRangeUser(0.0000001,1000000.);
  
  hGen_2_new->SetStats(0);
  hData_2_new->SetStats(0);
  hData_2_new->Draw("e");
  hGen_2_new->Draw("histsame][");
  hData_3_new->Draw("esame");
  hGen_3->Draw("histsame][");
  
  double x1 = 0.35; // 0.62 fixed
  double x2 = x1 + 0.55;
  double y2 = 0.90; // fixed
  double y1 = y2 - 0.14;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.04);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->SetNColumns(2);
  legend1->AddEntry(hData_2_new,"n_{j} #geq 2  ");
  legend1->AddEntry(hGen_2_new,"MG+P6 Z2* #otimes EW","l");
  legend1->AddEntry(hData_3_new,"n_{j} #geq 3  ");
  legend1->AddEntry(hGen_3,"MG+P6 Z2*","l");  
  legend1->Draw();
    
  double xmin = 260;
  double xmax = 295;
  double ymin = 0.35;
  double ymax = 33;

  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  tex = new TLatex(257.6578,6.061902,"10#lower[-0.75]{#scale[0.7]{1}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(257.6578,0.6061902,"10#lower[-0.75]{#scale[0.7]{0}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
    
  tex = new TLatex(281,1.105296e-08,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_data_MC_EW.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_data_MC_EW.pdf"); 
}  
