#include "/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/tdrstyle.C"
#include "/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void Sensitivity_ratio_32_MM(){
  setTDRStyle();
  
  TFile *fdata_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Sensitivity/MMHT2014/RootFiles/Sensitivity_2_MMHT2014.root");
  
  TFile *fdata_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Sensitivity/MMHT2014/RootFiles/Sensitivity_3_MMHT2014.root");
    
  TFile *fdata_ratio_np = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Data_2_3.root");
    
  TFile *fdata_ratio = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Comparison_ratio_32.root");
  
  TH1D *hNP =  (TH1D*)fdata_ratio_np->Get("NP_ratio_32_rebin_inv");
  
  TH1D *hdata_ratio =  (TH1D*)fdata_ratio->Get("Data_new_ratio");
      
  TH1D *h2_108=  (TH1D*)fdata_2->Get("h2_108");    
  TH1D *h2_109 =  (TH1D*)fdata_2->Get("h2_109");    
  TH1D *h2_110 =  (TH1D*)fdata_2->Get("h2_110");    
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
  TH1D *h2_124 =  (TH1D*)fdata_2->Get("h2_124");
  TH1D *h2_125 =  (TH1D*)fdata_2->Get("h2_125");
  TH1D *h2_126 =  (TH1D*)fdata_2->Get("h2_126");
  TH1D *h2_127 =  (TH1D*)fdata_2->Get("h2_127");
  TH1D *h2_128 =  (TH1D*)fdata_2->Get("h2_128");
        
  TH1D *h3_108 =  (TH1D*)fdata_3->Get("h3_108");    
  TH1D *h3_109 =  (TH1D*)fdata_3->Get("h3_109");    
  TH1D *h3_110 =  (TH1D*)fdata_3->Get("h3_110");    
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
  TH1D *h3_124 =  (TH1D*)fdata_3->Get("h3_124");
  TH1D *h3_125 =  (TH1D*)fdata_3->Get("h3_125");
  TH1D *h3_126 =  (TH1D*)fdata_3->Get("h3_126");
  TH1D *h3_127 =  (TH1D*)fdata_3->Get("h3_127");
  TH1D *h3_128 =  (TH1D*)fdata_3->Get("h3_128");
    
  h3_108->Divide(h2_108);
  h3_109->Divide(h2_109);
  h3_110->Divide(h2_110);
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
  h3_124->Divide(h2_124);
  h3_125->Divide(h2_125);
  h3_126->Divide(h2_126);
  h3_127->Divide(h2_127);
  h3_128->Divide(h2_128);
  
  h3_108->Divide(hNP);
  h3_109->Divide(hNP);
  h3_110->Divide(hNP);
  h3_111->Divide(hNP);
  h3_112->Divide(hNP);
  h3_113->Divide(hNP);
  h3_114->Divide(hNP);
  h3_115->Divide(hNP);
  h3_116->Divide(hNP);
  h3_117->Divide(hNP);
  h3_118->Divide(hNP);
  h3_119->Divide(hNP);
  h3_120->Divide(hNP);
  h3_121->Divide(hNP);
  h3_122->Divide(hNP);
  h3_123->Divide(hNP);
  h3_124->Divide(hNP);
  h3_125->Divide(hNP);
  h3_126->Divide(hNP);
  h3_127->Divide(hNP);
  h3_128->Divide(hNP); 
 
  double nbinsx = h3_114->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  for (int i = 1.; i <= nbinsx; i++)
    {
      
      double cont_108 = h3_108->GetBinContent(i);
      double cont_109 = h3_109->GetBinContent(i);
      
      if (cont_109 < cont_108) cout<<" Min. not ok for bin"<<i<<" "<<h3_112->GetBinLowEdge(i)<<"-"<<h3_112->GetBinLowEdge(i+1)<<endl;
 
      double cont_127 = h3_127->GetBinContent(i);
      double cont_128 = h3_128->GetBinContent(i);
      
      if (cont_127 > cont_128) cout<<" Max. not ok for bin"<<i<<" "<<h3_112->GetBinLowEdge(i)<<"-"<<h3_112->GetBinLowEdge(i+1)<<endl;
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
  
  h3_108->SetStats(0); 
  
  gStyle->SetLineStyleString(11,"40 15"); 
  
  h3_108->SetLineStyle(11);
  h3_109->SetLineStyle(2);
  h3_110->SetLineStyle(2);
  h3_111->SetLineStyle(2);
  h3_112->SetLineStyle(2);
  h3_113->SetLineStyle(2);
  h3_114->SetLineStyle(2);
  h3_115->SetLineStyle(2);
  h3_116->SetLineStyle(2);
  h3_117->SetLineStyle(2);
  h3_118->SetLineStyle(2);
  h3_119->SetLineStyle(2);
  h3_120->SetLineStyle(1);
  h3_121->SetLineStyle(2);
  h3_122->SetLineStyle(2);
  h3_123->SetLineStyle(2);
  h3_124->SetLineStyle(2); 
  h3_125->SetLineStyle(2);  
  h3_126->SetLineStyle(2); 
  h3_127->SetLineStyle(2);  
  h3_128->SetLineStyle(11);   
     
  h3_108->SetLineColor(kRed);
  h3_109->SetLineColor(kGray);
  h3_110->SetLineColor(kGray);
  h3_111->SetLineColor(kGray);
  h3_112->SetLineColor(kGray);
  h3_113->SetLineColor(kGray);
  h3_114->SetLineColor(kGray);
  h3_115->SetLineColor(kGray);
  h3_116->SetLineColor(kGray);
  h3_117->SetLineColor(kGray);
  h3_118->SetLineColor(kGray);
  h3_119->SetLineColor(kGray);
  h3_120->SetLineColor(kBlack);
  h3_121->SetLineColor(kGray);
  h3_122->SetLineColor(kGray);
  h3_123->SetLineColor(kGray);
  h3_124->SetLineColor(kGray);  
  h3_125->SetLineColor(kGray);
  h3_126->SetLineColor(kGray);
  h3_127->SetLineColor(kGray);
  h3_128->SetLineColor(kBlue);
      
  hdata_ratio->SetMarkerSize(0.5);
  hdata_ratio->SetMarkerColor(kOrange+4);
  hdata_ratio->SetLineColor(kOrange+4);
     
  h3_108->GetXaxis()->SetRangeUser(300.,1680.); 
  hdata_ratio->GetXaxis()->SetRangeUser(300.,1680.);   
  h3_108->GetYaxis()->SetRangeUser(0.05,0.35);  
  
  h3_108->SetTitle("");
  h3_108->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h3_108->GetXaxis()->SetTitleOffset(1.04);
  h3_108->GetXaxis()->SetTitleSize(0.06);
  h3_108->GetXaxis()->SetMoreLogLabels();
  h3_108->GetXaxis()->SetNoExponent();
  h3_108->GetXaxis()->SetLabelSize(0.05);
  h3_108->GetXaxis()->SetLabelOffset(0.018);
  h3_108->GetXaxis()->SetTickLength(0.03);
  
  h3_108->GetYaxis()->SetTitle("#it{R}_{32}");
  h3_108->GetYaxis()->SetTitleOffset(1.15);
  h3_108->GetYaxis()->SetTitleSize(0.06);
  h3_108->GetYaxis()->SetLabelSize(0.05);
  h3_108->GetYaxis()->SetTickLength(0.03);
  h3_108->GetYaxis()->SetLabelOffset(0.008);
  h3_108->GetYaxis()->SetNdivisions(507);
  h3_108->GetYaxis()->SetDecimals(4);
  
  h3_108->Draw("hist][");
  h3_109->Draw("histsame][");
  h3_110->Draw("histsame][");  
  h3_111->Draw("histsame][");
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
  h3_124->Draw("histsame][");
  h3_125->Draw("histsame][");
  h3_126->Draw("histsame][");
  h3_127->Draw("histsame][");
  h3_128->Draw("histsame][");

  hdata_ratio->Draw("esame");
  
  TLegend *legend1=new TLegend(0.40,0.66,0.62,0.91);
  legend1->SetTextSize(0.04);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry(hdata_ratio,"Data/NP");
  legend1->AddEntry(h3_108,"MMHT #alpha_{s} (M_{z}) = 0.108 - Min. Value","l");
  legend1->AddEntry(h3_120,"MMHT #alpha_{s} (M_{z}) = 0.120","l");
  legend1->AddEntry(h3_128,"MMHT #alpha_{s} (M_{z}) = 0.128 - Max. Value","l");
  legend1->Draw();
   
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  tex = new TLatex(281,0.02792643,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex1 = new TLatex(1541,0.02792643,"1680");
  tex1->SetTextFont(42);
  tex1->SetTextSize(0.05);
  tex1->SetLineWidth(2);
  tex1->Draw();
  
  c->Draw();
  c->Update();
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Sensitivity_ratio_32_MMHT2014.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Sensitivity_ratio_32_MMHT2014.pdf");

}
