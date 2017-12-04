#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void comparison_data_MC_samples_ratio_32_Pow(){
  setTDRStyle();

    TFile *fdata = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Data_Xsection_Ratio_HT_2_150.root");

  //TFile *fdata = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Data_Xsection_HT_2_150.root");
  
  TFile *fmad = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/Rootfile_HT_2_150/Hadd_GenMCJetHT_2_150_nopileup_New.root");
  
  TFile *fpythia = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/MC_Pythia6.root");
  
  TFile *fherwig = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/MC_Herwig.root");
  
  TFile *fpowheg = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/Powheg-Pythia8_final_total.root");  
  
  TFile *fExp_Unc = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Total_unc_ratio_32_direct_add.root");
  
  TFile *fExp_Unc_copy = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Total_unc_ratio_32_direct_add_copy.root");

 //*****************************************************************
  
  TH1D *hData_3 = (TH1D*)fdata->Get("Unfolded_Ratio");
  
  //TH1D *hData_2 = (TH1D*)fdata->Get("Data_2_new");
  //TH1D *hData_3 = (TH1D*)fdata->Get("Data_3_new"); 
   
  TH1D *hMad_2  = (TH1D*)fmad->Get("Hadd_GenMCJetHT_2_150_2_nopileup_New");
  TH1D *hMad_3  = (TH1D*)fmad->Get("Hadd_GenMCJetHT_2_150_3_nopileup_New");
    
  TH1D *hpythia_2  = (TH1D*)fpythia->Get("jet/GenMCJetHt_2_2");
  TH1D *hherwig_2  = (TH1D*)fherwig->Get("jet/GenMCJetHt_2_2");
  TH1D *hpowheg_S1_2  = (TH1D*)fpowheg->Get("GenMCJetHt_2_2_S1");
  TH1D *hpowheg_M1_2  = (TH1D*)fpowheg->Get("GenMCJetHt_2_2_M1"); 
  
  TH1D *hpythia_3  = (TH1D*)fpythia->Get("jet/GenMCJetHt_2_3");
  TH1D *hherwig_3  = (TH1D*)fherwig->Get("jet/GenMCJetHt_2_3");
  TH1D *hpowheg_S1_3  = (TH1D*)fpowheg->Get("GenMCJetHt_2_3_S1");
  TH1D *hpowheg_M1_3  = (TH1D*)fpowheg->Get("GenMCJetHt_2_3_M1"); 
   
  TH1D *hExp_Unc_Up =  (TH1D*)fExp_Unc->Get("Total_up_nostat"); 
  TH1D *hExp_Unc_Down =  (TH1D*)fExp_Unc->Get("Total_down_nostat");
  
  TH1D *hExp_Unc_Up_2 =  (TH1D*)fExp_Unc_copy->Get("Total_up_nostat");
  TH1D *hExp_Unc_Down_2 =  (TH1D*)fExp_Unc_copy->Get("Total_down_nostat");
  
  TH1D *hStat =  (TH1D*)fExp_Unc->Get("Statp");
  
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};

   int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
  
  TH1D *hData_MC_ratio = new TH1D("Data_MC_ratio_3","Data_MC_ratio_3",arr_size-1,bins);
  hData_MC_ratio->Sumw2();
  
  TH1D *hData_MC_ratio_err = new TH1D("Data_MC_ratio_3_err","Data_MC_ratio_3_err",arr_size-1,bins);
  hData_MC_ratio_err->Sumw2();

  TH1D *hData_MC_ratio_unc = new TH1D("Data_MC_ratio_3_unc","Data_MC_ratio_3_unc",arr_size-1,bins);
  hData_MC_ratio_unc->Sumw2();
  
  TH1D *hherwig_new = new TH1D("herwig_new","herwig_new",arr_size-1,bins);
      
  hpythia_3->Scale(29981599700.0/181279898.0);
  hpythia_3->Scale(1.0, "width");
  
  hpythia_2->Scale(29981599700.0/181279898.0);
  hpythia_2->Scale(1.0, "width");
       
  hMad_2->Scale(1.0, "width"); 
  hMad_3->Scale(1.0, "width");   
        
  hMad_3->Divide(hMad_2);
  //hData_3->Divide(hData_2);
  hpythia_3->Divide(hpythia_2); 
  hpowheg_S1_3->Divide(hpowheg_S1_2);
  hpowheg_M1_3->Divide(hpowheg_M1_2);
  

    
  for (int i = 1; i <= arr_size-1  ; i++)
    {
     if (hpowheg_S1_3->GetBinContent(i) == 0.0 )
     double data_bin_ratio = 1.;
     else
     double data_bin_ratio = hData_3->GetBinContent(i)/hpowheg_S1_3->GetBinContent(i);
     hData_MC_ratio->SetBinContent(i,data_bin_ratio);     
     hData_MC_ratio_unc->SetBinContent(i,data_bin_ratio);     
     
     
     if (hpowheg_S1_3->GetBinContent(i) == 0.0 )
      double stat_err = 1.;
      else
      double stat_err = hStat->GetBinError(i);
      double stat_err_ratio_p = stat_err * data_bin_ratio;

      hData_MC_ratio->SetBinError(i,stat_err_ratio_p);  
     
     if (hpowheg_S1_3->GetBinContent(i) == 0.0 )
      double unc_err = 1.;
      else
      double unc_err = (hExp_Unc_Up->GetBinContent(i));     
      double stat_err_ratio = unc_err * data_bin_ratio;

     hData_MC_ratio_unc->SetBinError(i,stat_err_ratio);  
    
    }
  
  hpythia_3->Divide(hpowheg_S1_3);
  hherwig_3->Divide(hpowheg_S1_3);
  hMad_3->Divide(hpowheg_S1_3);
  hpowheg_M1_3->Divide(hpowheg_S1_3);  
  
  //hherwig_3->Print("all");
  
  for (int i = 1; i <= arr_size-1  ; i++)
    {
    if (i == 33)
    hherwig_new->SetBinContent(i,(0.66/hherwig_2->GetBinContent(i)));
    else
     hherwig_new->SetBinContent(i,(hherwig_3->GetBinContent(i)/hherwig_2->GetBinContent(i)));
    }
   
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
  
  gStyle->SetLineStyleString(11,"40 20"); 
  
  hData_MC_ratio->SetMarkerStyle(20);
  hData_MC_ratio->SetMarkerColor(kBlack);
  hData_MC_ratio->SetLineColor(kBlack);
  hData_MC_ratio->SetMarkerSize(0.75); 
  
  hData_MC_ratio_unc->SetMarkerStyle(20);
  hData_MC_ratio_unc->SetMarkerColor(kBlack);
  hData_MC_ratio_unc->SetLineColor(kBlack);
  hData_MC_ratio_unc->SetMarkerSize(0.05); 
 
  hpythia_3->SetMarkerColor(kGreen+2);
  hpythia_3->SetLineColor(kGreen+2);
  hpythia_3->SetLineWidth(2);
  hpythia_3->SetLineStyle(9);       
  
  hMad_3->SetMarkerColor(38);
  hMad_3->SetLineColor(38);
  hMad_3->SetLineWidth(2);    
  
  hpowheg_M1_3->SetMarkerColor(kBlue);
  hpowheg_M1_3->SetLineColor(kBlue);
  hpowheg_M1_3->SetLineWidth(2);    
  hpowheg_M1_3->SetLineStyle(2);    
         
  hherwig_new->SetMarkerColor(kRed);
  hherwig_new->SetLineColor(kRed);
  hherwig_new->SetLineWidth(2);
  //hherwig_new->SetLineStyle(11);                

  hherwig_3->SetMarkerStyle(24);
     
  hExp_Unc_Up->SetFillColor(kOrange+2);
  hExp_Unc_Up->SetLineColor(kOrange+2);
  hExp_Unc_Up->SetLineWidth(1);
  hExp_Unc_Up->SetFillStyle(3001);
    
  hData_MC_ratio->GetXaxis()->SetRangeUser(300.0,1680.0);
  hData_MC_ratio_unc->GetXaxis()->SetRangeUser(300.0,1680.0);
  hpythia_3->GetXaxis()->SetRangeUser(300.0,1680.0);
  hherwig_new->GetXaxis()->SetRangeUser(300.0,1680.0);
  hExp_Unc_Up->GetXaxis()->SetRangeUser(300.0,1680.0);
  hpowheg_M1_3->GetXaxis()->SetRangeUser(300.0,1680.0);
  hMad_3->GetXaxis()->SetRangeUser(300.0,1680.0);  
  hData_MC_ratio_unc->GetYaxis()->SetRangeUser(0.5,2.6);
  
  hData_MC_ratio_unc->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hData_MC_ratio_unc->GetXaxis()->SetTitleOffset(1.04);
  hData_MC_ratio_unc->GetXaxis()->SetTitleSize(0.06);
  hData_MC_ratio_unc->GetXaxis()->SetMoreLogLabels();
  hData_MC_ratio_unc->GetXaxis()->SetNoExponent();
  hData_MC_ratio_unc->GetXaxis()->SetLabelSize(0.05);
  hData_MC_ratio_unc->GetXaxis()->SetLabelOffset(0.018);
  hData_MC_ratio_unc->GetXaxis()->SetTickLength(0.03);
  
  hData_MC_ratio_unc->GetYaxis()->SetTitle("Ratios to Powheg+Pythia8 CUETS1");
  hData_MC_ratio_unc->GetYaxis()->SetTitleOffset(1.19);
  hData_MC_ratio_unc->GetYaxis()->SetTitleSize(0.05);
  hData_MC_ratio_unc->GetYaxis()->SetLabelSize(0.05);
  hData_MC_ratio_unc->GetYaxis()->SetTickLength(0.03);
  hData_MC_ratio_unc->GetYaxis()->SetLabelOffset(0.008);
  hData_MC_ratio_unc->GetYaxis()->SetNdivisions(508);
  hData_MC_ratio_unc->GetYaxis()->SetDecimals(4);
  
  hData_MC_ratio_unc->SetStats(0);
  hData_MC_ratio->SetStats(0);
  hData_MC_ratio_unc->DrawCopy("e2"); 
  hData_MC_ratio_unc->SetFillColor(kOrange+2);
  hData_MC_ratio_unc->SetLineColor(kOrange+2);
  hData_MC_ratio_unc->SetLineWidth(2);
  hData_MC_ratio_unc->SetFillStyle(3001);
  hData_MC_ratio_unc->Draw("e2same");

  hData_MC_ratio->Draw("esame");
  hpythia_3->Draw("histsame][");
  //hherwig_new->Draw("histsame][");
  hpowheg_M1_3->Draw("histsame][");
  hMad_3->Draw("histsame]["); 
   
  double x1 = 0.4; // 0.62 fixed
  double x2 = x1 + 0.39;
  double y2 = 0.90; // fixed
  double y1 = y2 - 0.35;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  legend1->AddEntry(hData_MC_ratio,"Data");
  legend1->AddEntry(hpythia_3,"Pythia6 Z2*","l");
  legend1->AddEntry(hMad_3,"MG+P6 Z2*","l");
  legend1->AddEntry(hpowheg_M1_3,"Powheg+Pythia8 CUETM1","l");  
  //legend1->AddEntry(hherwig_new,"Herwig++","l");
  legend1->AddEntry(hExp_Unc_Up,"Exp. Uncertainty","f");
  legend1->Draw();
    
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  tex = new TLatex(281,0.345485,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
      
  tex = new TLatex(1514,0.345485,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c->Update();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_data_MC_samples_ratio_32_Pow.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_data_MC_samples_ratio_32_Pow.pdf"); 

}  
