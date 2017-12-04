#include "/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/tdrstyle.C"
#include "/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void Sensitivity_double_ratio_32_CT14(){
  setTDRStyle();
  
    TFile *fdata_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Sensitivity/CT14/RootFiles/Sensitivity_2_CT14.root");
    
    TFile *fdata_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Sensitivity/CT14/RootFiles/Sensitivity_3_CT14.root");
    
  TFile *fdata_ratio = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Data_2_3.root");
  
  TH1D *hdata_2 =  (TH1D*)fdata_ratio->Get("Data_new_2_rebin");
  TH1D *hdata_3 =  (TH1D*)fdata_ratio->Get("Data_new_3_rebin");
  
  TH1D *hNP_2 =  (TH1D*)fdata_ratio->Get("NP_2_rebin");
  TH1D *hNP_3 =  (TH1D*)fdata_ratio->Get("NP_3_rebin");
  TH1D *hNP_ratio =  (TH1D*)fdata_ratio->Get("NP_ratio_32_rebin");
  
  TH1D *hTotal_Unc_up_ratio =  (TH1D*)fdata_ratio->Get("Total_up_ratio_rebin");
  TH1D *hTotal_Unc_down_ratio =  (TH1D*)fdata_ratio->Get("Total_down_ratio_rebin");
  
  const double new_bins[40] = {150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000.};
   
  TH1D *hdata_ratio = new TH1D("hdata_ratio","hdata_ratio",39.0,new_bins);  
  hdata_ratio->Sumw2();
    
  TH1D *h2_111 =  (TH1D*)fdata_2->Get("h2_111");  
  TH1D *h2_112 =  (TH1D*)fdata_2->Get("h2_112");
  TH1D *h2_113 =  (TH1D*)fdata_2->Get("h2_113");  
  TH1D *h2_114 =  (TH1D*)fdata_2->Get("h2_114");
  TH1D *h2_115 =  (TH1D*)fdata_2->Get("h2_115");
  TH1D *h2_116 =  (TH1D*)fdata_2->Get("h2_116");
  TH1D *h2_117 =  (TH1D*)fdata_2->Get("h2_117");
  TH1D *h2_118 =  (TH1D*)fdata_2->Get("h2_118");
  TH1D *h2_119 =  (TH1D*)fdata_2->Get("h2_119");
  TH1D *h2_120 =  (TH1D*)fdata_2->Get("h2_120");
  TH1D *h2_121 =  (TH1D*)fdata_2->Get("h2_121");
  TH1D *h2_122 =  (TH1D*)fdata_2->Get("h2_122");
  TH1D *h2_123 =  (TH1D*)fdata_2->Get("h2_123");
  
  TH1D *h3_111 =  (TH1D*)fdata_3->Get("h3_111");      
  TH1D *h3_112 =  (TH1D*)fdata_3->Get("h3_112");
  TH1D *h3_113 =  (TH1D*)fdata_3->Get("h3_113");  
  TH1D *h3_114 =  (TH1D*)fdata_3->Get("h3_114");
  TH1D *h3_115 =  (TH1D*)fdata_3->Get("h3_115");
  TH1D *h3_116 =  (TH1D*)fdata_3->Get("h3_116");
  TH1D *h3_117 =  (TH1D*)fdata_3->Get("h3_117");
  TH1D *h3_118 =  (TH1D*)fdata_3->Get("h3_118");
  TH1D *h3_119 =  (TH1D*)fdata_3->Get("h3_119");
  TH1D *h3_120 =  (TH1D*)fdata_3->Get("h3_120");
  TH1D *h3_121 =  (TH1D*)fdata_3->Get("h3_121");
  TH1D *h3_122 =  (TH1D*)fdata_3->Get("h3_122");
  TH1D *h3_123 =  (TH1D*)fdata_3->Get("h3_123");
 
  h3_111->Divide(h2_111); 
  h3_112->Divide(h2_112);
  h3_113->Divide(h2_113);  
  h3_114->Divide(h2_114);
  h3_115->Divide(h2_115);
  h3_116->Divide(h2_116);
  h3_117->Divide(h2_117);
  h3_118->Divide(h2_118);
  h3_119->Divide(h2_119);
  h3_120->Divide(h2_120);
  h3_121->Divide(h2_121);
  h3_122->Divide(h2_122);
  h3_123->Divide(h2_123);

  double nbinsx = h3_114->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  for (int i = 1.; i <= nbinsx; i++)
    {
      //cout<<" Bin : "<<i<<"  "<<h3_123->GetBinLowEdge(i)<<"-"<<h3_123->GetBinLowEdge(i+1)<<" Content : "<<h3_123->GetBinContent(i)<<endl; 
      double cont_111 = h3_111->GetBinContent(i);
      double cont_112 = h3_112->GetBinContent(i);
      
      if (cont_112 < cont_111) cout<<" Min. not ok for bin"<<i<<" "<<h3_112->GetBinLowEdge(i)<<"-"<<h3_112->GetBinLowEdge(i+1)<<endl;
 
      double cont_122 = h3_122->GetBinContent(i);
      double cont_123 = h3_123->GetBinContent(i);
      
      if (cont_122 > cont_123) cout<<" Max. not ok for bin"<<i<<" "<<h3_112->GetBinLowEdge(i)<<"-"<<h3_112->GetBinLowEdge(i+1)<<endl;
    }
    
    for (int i = 1.; i <= nbinsx; i++)
    {
     double np_2 = hNP_2->GetBinContent(i);
     double np_3 = hNP_3->GetBinContent(i);
     
     double data_ratio = hdata_3->GetBinContent(i)/hdata_2->GetBinContent(i); //data ratio
     
     //double np_ratio = np_3/np_2; // np corr
     
     double np_ratio = hNP_ratio->GetBinContent(i); // np corr
     
     double ratio = data_ratio/(h3_118->GetBinContent(i)*np_ratio);  //data/(theory*np)
     hdata_ratio->SetBinContent(i,ratio);
      
      if (hTotal_Unc_up_ratio->GetBinContent(i) > hTotal_Unc_down_ratio->GetBinContent(i)) double err_ratio = hTotal_Unc_up_ratio->GetBinContent(i);
      else double err_ratio = hTotal_Unc_down_ratio->GetBinContent(i);
      hdata_ratio->SetBinError(i,err_ratio*ratio);   
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
  
  h3_111->SetStats(0); 
  
  hdata_ratio->SetMarkerSize(0.5);
  hdata_ratio->SetMarkerColor(kOrange+4);
  hdata_ratio->SetLineColor(kOrange+4);
  
  h3_111->Divide(h3_118);
  h3_112->Divide(h3_118);
  h3_113->Divide(h3_118);
  h3_114->Divide(h3_118);
  h3_115->Divide(h3_118);
  h3_116->Divide(h3_118);
  h3_117->Divide(h3_118);
  h3_119->Divide(h3_118);
  h3_120->Divide(h3_118);
  h3_121->Divide(h3_118);
  h3_122->Divide(h3_118);
  h3_123->Divide(h3_118);
   
  h3_118->Divide(h3_118);
  
  gStyle->SetLineStyleString(11,"40 15"); 
  
  h3_111->SetLineStyle(11);
  h3_112->SetLineStyle(2);
  h3_113->SetLineStyle(2);
  h3_114->SetLineStyle(2);
  h3_115->SetLineStyle(2);
  h3_116->SetLineStyle(2);
  h3_117->SetLineStyle(2);
  h3_118->SetLineStyle(1);
  h3_119->SetLineStyle(2);
  h3_120->SetLineStyle(2);
  h3_121->SetLineStyle(2);
  h3_122->SetLineStyle(2);
  h3_123->SetLineStyle(11);
   
  h3_111->SetLineColor(kRed);
  h3_112->SetLineColor(kGray);
  h3_113->SetLineColor(kGray);
  h3_114->SetLineColor(kGray);
  h3_115->SetLineColor(kGray);
  h3_116->SetLineColor(kGray);
  h3_117->SetLineColor(kGray);
  h3_118->SetLineColor(kBlack);
  h3_119->SetLineColor(kGray);
  h3_120->SetLineColor(kGray);
  h3_121->SetLineColor(kGray);
  h3_122->SetLineColor(kGray);
  h3_123->SetLineColor(kBlue);
   
  h3_111->GetXaxis()->SetRangeUser(300.,1680.); 
  hdata_ratio->GetXaxis()->SetRangeUser(300.,1680.); 
  h3_111->GetYaxis()->SetRangeUser(0.5,2.0);  
  
  h3_111->SetTitle("");
  h3_111->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h3_111->GetXaxis()->SetTitleOffset(1.04);
  h3_111->GetXaxis()->SetTitleSize(0.06);
  h3_111->GetXaxis()->SetMoreLogLabels();
  h3_111->GetXaxis()->SetNoExponent();
  h3_111->GetXaxis()->SetLabelSize(0.05);
  h3_111->GetXaxis()->SetLabelOffset(0.018);
  h3_111->GetXaxis()->SetTickLength(0.03);
  
  h3_111->GetYaxis()->SetTitle("Ratio to NLO (CT14)");
  h3_111->GetYaxis()->SetTitleOffset(1.04);
  h3_111->GetYaxis()->SetTitleSize(0.06);
  h3_111->GetYaxis()->SetLabelSize(0.05);
  h3_111->GetYaxis()->SetTickLength(0.03);
  h3_111->GetYaxis()->SetLabelOffset(0.008);
  h3_111->GetYaxis()->SetNdivisions(508);
  h3_111->GetYaxis()->SetDecimals(4); 
   
  h3_111->Draw("hist][");
  h3_112->Draw("histsame][");
  h3_113->Draw("histsame][");
  h3_114->Draw("histsame][");  
  h3_115->Draw("histsame][");
  h3_116->Draw("histsame][");
  h3_117->Draw("histsame][");
  h3_118->Draw("histsame][");
  h3_119->Draw("histsame][");
  h3_120->Draw("histsame][");
  h3_121->Draw("histsame][");
  h3_122->Draw("histsame][");
  h3_123->Draw("histsame][");
  hdata_ratio->Draw("esame");
 
  TLegend *legend1=new TLegend(0.40,0.66,0.62,0.91);
  legend1->SetTextSize(0.04);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");    
  legend1->AddEntry(hdata_ratio,"Data/NP");
  legend1->AddEntry(h3_111,"CT14 #alpha_{s}(M_{z}) = 0.111 - Min. Value","l");
  legend1->AddEntry(h3_118,"CT14 #alpha_{s}(M_{z}) = 0.118","l");
  legend1->AddEntry(h3_123,"CT14 #alpha_{s}(M_{z}) = 0.123 - Max. Value","l");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  tex = new TLatex(281,0.4046823,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex1 = new TLatex(1541,0.4046823,"1680");
  tex1->SetTextFont(42);
  tex1->SetTextSize(0.05);
  tex1->SetLineWidth(2);
  tex1->Draw();
    
  c->Draw();
  c->Update();
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Sensitivity_double_ratio_32_CT14.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Sensitivity_double_ratio_32_CT14.pdf");

}
