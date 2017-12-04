#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void comparison_data_NLO_Pdfs_3(){
  setTDRStyle();
  
  TFile *fdata_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Data_Xsection_HT_2_150.root");
  
  TFile *fNLO_Pdfs =  TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Final_Plots_PAS/RootFiles/Rebin_NLO_PDFS_HT_2_150_3.root");
  
  TFile *fNLO_Unc = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Theory_Unc_3.root");
  
  TFile *fNLO_Unc_copy = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Theory_Unc_3_copy.root");
    
  TFile *fExp_Unc = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Exp_Unc/RootFiles_HT_2_150/Exp_Unc_all_3_sources_NP.root");
  
  TFile *fExp_Unc_copy = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Exp_Unc/RootFiles_HT_2_150/Exp_Unc_all_3_sources_NP_copy.root");
  
  TFile *fNP_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/Final_NP_Corr_3.root");
    
  //*****************************************************************
  
  TH1D *hData_3 = (TH1D*)fdata_3->Get("Data_3_new");
  TH1D *hCT10 = (TH1D*)fNLO_Pdfs->Get("CT10_3");
  TH1D *hMSTW =  (TH1D*)fNLO_Pdfs->Get("MSTW_3");
  TH1D *hNNPDF =  (TH1D*)fNLO_Pdfs->Get("NNPDF_3");
  TH1D *hNLO_Unc_Up =  (TH1D*)fNLO_Unc->Get("Theory_Unc_Up");
  TH1D *hNLO_Unc_Up_3 =  (TH1D*)fNLO_Unc_copy->Get("Theory_Unc_Up");  
  TH1D *hNLO_Unc_Down =  (TH1D*)fNLO_Unc->Get("Theory_Unc_Down");
  TH1D *hNLO_Unc_Down_3 =  (TH1D*)fNLO_Unc_copy->Get("Theory_Unc_Down");
  TH1D *hExp_Unc_Up =  (TH1D*)fExp_Unc->Get("Total_up_nostat");
  TH1D *hExp_Unc_Up_3 =  (TH1D*)fExp_Unc_copy->Get("Total_up_nostat");  
  TH1D *hExp_Unc_Down =  (TH1D*)fExp_Unc->Get("Total_down_nostat");
  TH1D *hExp_Unc_Down_3 =  (TH1D*)fExp_Unc_copy->Get("Total_down_nostat");
  TH1D *hNP_3 =  (TH1D*)fNP_3->Get("Central Value");
   
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
  
  TH1D *hData_CT10_ratio = new TH1D("Data_CT10_ratio_3","Data_CT10_ratio_3",arr_size-1,bins);
  hData_CT10_ratio->Sumw2();
  
  TH1D *hData_CT10_ratio_err = new TH1D("Data_CT10_ratio_3_err","Data_CT10_ratio_3_err",arr_size-1,bins);
  hData_CT10_ratio_err->Sumw2();
  
  THStack *hs_th = new THStack("hs_th","");
     
  for (int i = 1; i <= arr_size-1  ; i++)
    {
     if ( hCT10->GetBinContent(i) == 0.0 || hNP_3->GetBinContent(i) == 0.0 )
     double data_bin_ratio = 1.;
     else
     double data_bin_ratio = hData_3->GetBinContent(i)/(hCT10->GetBinContent(i)*hNP_3->GetBinContent(i));
     //cout<<" Bin : "<<i<<" Con : "<<hData_3->GetBinContent(i)<<endl;
     hData_CT10_ratio->SetBinContent(i,data_bin_ratio);
     hData_CT10_ratio_err->SetBinContent(i,data_bin_ratio);     
     
     if (hData_3->GetBinContent(i) == 0.0 )
      double stat_err = 1.;
      else
      double stat_err = (hData_3->GetBinError(i)/hData_3->GetBinContent(i));     
      double stat_err_ratio = stat_err * data_bin_ratio;

     hData_CT10_ratio->SetBinError(i,stat_err_ratio);  
     
     if (hData_3->GetBinContent(i) == 0.0 )
      double unc_err = 1.;
      else
      double unc_err = (hExp_Unc_Up->GetBinContent(i));     
      double stat_err_ratio = unc_err * data_bin_ratio;

     hData_CT10_ratio_err->SetBinError(i,stat_err_ratio);  
    
    }
    
 /*for (int i = 1; i <= arr_size-1  ; i++)
    { 
      double np_factor_3 = hNP_3->GetBinContent(i);
       cout<<" Bin : "<<hNP_3->GetBinLowEdge(i)<<" Data : "<< hData_3->GetBinContent(i)<<" NLO : "<<hCT10->GetBinContent(i) <<" Factor : "<<np_factor_3 <<endl;
 }
*/
  hMSTW->Divide(hCT10);
  hNNPDF->Divide(hCT10);
   
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
  
  gStyle->SetLineStyleString(11,"50 40"); 
  
  hData_CT10_ratio->SetMarkerStyle(20);
  hData_CT10_ratio->SetMarkerColor(kBlack);
  hData_CT10_ratio->SetLineColor(kBlack);
  hData_CT10_ratio->SetLineWidth(2);
  hData_CT10_ratio->SetMarkerSize(0.75); 
  
  hData_CT10_ratio_err->SetMarkerStyle(20);
  hData_CT10_ratio_err->SetMarkerColor(kBlack);
  hData_CT10_ratio_err->SetLineColor(kBlack);
  hData_CT10_ratio_err->SetMarkerSize(0.05); 
   
  hMSTW->SetMarkerColor(kOrange+4);
  hMSTW->SetLineColor(kOrange+4);
  hMSTW->SetLineWidth(2); 
  hMSTW->SetLineStyle(9);             
   
  hNNPDF->SetMarkerColor(kGreen-6);
  hNNPDF->SetLineColor(kGreen-6);
  hNNPDF->SetLineWidth(2);      
  hNNPDF->SetLineStyle(1);          
  
  hExp_Unc_Up->SetMarkerColor(33);
  hExp_Unc_Up->SetLineColor(33); 
  hExp_Unc_Up->SetFillColor(33); 
  hExp_Unc_Up->SetFillStyle(1000);

  hNLO_Unc_Down->SetLineColor(kRed); 
  hNLO_Unc_Down->SetLineWidth(1);
  hNLO_Unc_Down->SetLineStyle(3); 
            
  hNLO_Unc_Down_3->SetLineColor(kRed); 
  hNLO_Unc_Down_3->SetLineWidth(1);
  hNLO_Unc_Down_3->SetLineStyle(3); 
   
  hNLO_Unc_Up->SetLineColor(kRed); 
  hNLO_Unc_Up->SetFillColor(kRed);
  hNLO_Unc_Up->SetFillStyle(3005);   
  hNLO_Unc_Up->SetLineWidth(1);
  hNLO_Unc_Up->SetLineStyle(3);   
  
  //Stack
  hs_th->Add(hNLO_Unc_Down);
  hNLO_Unc_Up->Add(hNLO_Unc_Down_3,-1); 
  hs_th->Add(hNLO_Unc_Up);
  
  hData_CT10_ratio->GetXaxis()->SetRangeUser(300.0,1680.0);
  hData_CT10_ratio_err->GetXaxis()->SetRangeUser(300.0,1680.0);
  hMSTW->GetXaxis()->SetRangeUser(300.0,1680.0);
  hNNPDF->GetXaxis()->SetRangeUser(300.0,1680.0);
  hExp_Unc_Up->GetXaxis()->SetRangeUser(300.0,1680.0);
  hExp_Unc_Down->GetXaxis()->SetRangeUser(300.0,1680.0);
  hs_th->SetMinimum(0.5);
  hs_th->SetMaximum(2.39);
  
   TH1F *hs_stack_1 = new TH1F("hs_stack_1","",56, bins);
   hs_stack_1->SetMinimum(0.6);
   hs_stack_1->SetMaximum(1.68);
   hs_stack_1->SetDirectory(0);
   hs_stack_1->SetStats(0);

   hs_stack_1->GetXaxis()->SetRange(22,50);
   //hs_stack_1->GetXaxis()->SetRange(18,50);
   hs_th->SetHistogram(hs_stack_1);
  
  hs_th->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hs_th->GetXaxis()->SetTitleOffset(1.04);
  hs_th->GetXaxis()->SetTitleSize(0.06);
  hs_th->GetXaxis()->SetMoreLogLabels();
  hs_th->GetXaxis()->SetNoExponent();
  hs_th->GetXaxis()->SetLabelSize(0.05);
  hs_th->GetXaxis()->SetLabelOffset(0.018);
  hs_th->GetXaxis()->SetTickLength(0.03);
  
  hs_th->GetYaxis()->SetTitle("Ratios to NLO (CT10)");
  hs_th->GetYaxis()->SetTitleOffset(1.19);
  hs_th->GetYaxis()->SetTitleSize(0.05);
  hs_th->GetYaxis()->SetLabelSize(0.05);
  hs_th->GetYaxis()->SetTickLength(0.03);
  hs_th->GetYaxis()->SetLabelOffset(0.008);
  hs_th->GetYaxis()->SetNdivisions(508);
  hs_th->GetYaxis()->SetDecimals(4);
  
  hData_CT10_ratio->SetStats(0);
  
  hs_th->Draw("][");
  //hData_CT10_ratio_err->DrawCopy("e2same"); 
  hData_CT10_ratio_err->SetFillColor(33);//kRed);
  hData_CT10_ratio_err->SetMarkerColor(33);//kRed);
  hData_CT10_ratio_err->SetLineColor(33);//kRed);
  hData_CT10_ratio_err->SetLineWidth(2);
  hData_CT10_ratio_err->SetFillStyle(1000);
  hData_CT10_ratio_err->Draw("e2same");
  hData_CT10_ratio->Draw("esame");
  hMSTW->Draw("histsame][");
  hNNPDF->Draw("histsame][");
 
  double x1 = 0.45; // 0.62 fixed
  double x2 = x1 + 0.39;
  double y2 = 0.90; // fixed
  double y1 = y2 - 0.32;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hData_CT10_ratio,"Data/NP","pel");
  legend1->AddEntry(hMSTW,"MSTW2008","l");
  legend1->AddEntry(hNNPDF,"NNPDF2.3","l");
  legend1->AddEntry(hExp_Unc_Up,"Exp. Uncertainty","f");
  legend1->AddEntry(hNLO_Unc_Up,"Theory Uncertainty","f");
  legend1->Draw();
     
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
      
  tex = new TLatex(281,0.3521439,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(1541,0.3521439,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_data_NLO_Pdfs_3.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_data_NLO_Pdfs_3.pdf"); 

}  
