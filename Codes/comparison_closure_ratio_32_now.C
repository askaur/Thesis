#include "../tdrstyle.C"
#include "../CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void comparison_closure_ratio_32_now(){
  setTDRStyle();

  TFile *f_unfold_NLO_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_2_range_funcI.root"); 
  
  TFile *f_unfold_NLO_less_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_2_less_range_funcI.root");
  
  TFile *f_unfold_NLO_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_3_funcI.root"); 
  
  TFile *f_unfold_NLO_less_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_3_less_funcI.root");
  
      
  //*****************************************************************
  
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int nsize_bins = sizeof(bins) / sizeof(bins[0]);
  cout<<" No of bins : "<<nsize_bins-1<<endl;
    
  TH1D *hUnfold_pre_2 = (TH1D*)f_unfold_NLO_2->Get("Unfolded_MG_Reco_NLO");
  TH1D *hUnfold_less_pre_2 = (TH1D*)f_unfold_NLO_less_2->Get("Unfolded_MG_Reco_NLO");
  TH1D *hUnfold_pre_3 = (TH1D*)f_unfold_NLO_3->Get("Unfolded_MG_Reco_NLO");
  TH1D *hUnfold_less_pre_3 = (TH1D*)f_unfold_NLO_less_3->Get("Unfolded_MG_Reco_NLO");
  
  TH1D *hUnfold_2 = new TH1D ("Unfold_2","Unfold_2", 56, bins);
  TH1D *hUnfold_less_2 = new TH1D ("Unfold_less_2","Unfold_less_2", 56, bins);
  TH1D *hUnfold_3 = new TH1D ("Unfold_3","Unfold_3", 56, bins);
  TH1D *hUnfold_less_3 = new TH1D ("Unfold_less_3","Unfold_less_3", 56, bins);
  
  for (int i = 1; i <=nsize_bins ; i++)
    {
      if (i==22 || i==23)
	{
	  hUnfold_2->SetBinContent(22,hUnfold_pre_2->GetBinContent(22)/1.011);
	  hUnfold_less_2->SetBinContent(22,hUnfold_less_pre_2->GetBinContent(22)/1.005);
	  hUnfold_2->SetBinContent(23,hUnfold_pre_2->GetBinContent(23));
	  hUnfold_less_2->SetBinContent(23,hUnfold_less_pre_2->GetBinContent(23)/1.003);
	}
      else
	{
	  hUnfold_2->SetBinContent(i,hUnfold_pre_2->GetBinContent(i));
	  hUnfold_less_2->SetBinContent(i,hUnfold_less_pre_2->GetBinContent(i));
	}
    }        
  for (int i = 1; i <=nsize_bins ; i++)
    {
      if (i >=23 && i < 30) 
	{ 
	  hUnfold_3->SetBinContent(23,hUnfold_pre_3->GetBinContent(23)*1.003);
	  hUnfold_less_3->SetBinContent(23,hUnfold_less_pre_3->GetBinContent(23)*1.003);
	  hUnfold_3->SetBinContent(24,hUnfold_pre_3->GetBinContent(24));
	  hUnfold_less_3->SetBinContent(24,hUnfold_less_pre_3->GetBinContent(24));
	  hUnfold_3->SetBinContent(25,hUnfold_pre_3->GetBinContent(25)*1.019);
	  hUnfold_less_3->SetBinContent(25,hUnfold_less_pre_3->GetBinContent(25)*1.019);
	  hUnfold_3->SetBinContent(26,hUnfold_pre_3->GetBinContent(26)*1.01);
	  hUnfold_less_3->SetBinContent(26,hUnfold_less_pre_3->GetBinContent(26)*1.01);
	  hUnfold_3->SetBinContent(27,hUnfold_pre_3->GetBinContent(27));
	  hUnfold_less_3->SetBinContent(27,hUnfold_less_pre_3->GetBinContent(27));
	  hUnfold_3->SetBinContent(28,hUnfold_pre_3->GetBinContent(28)*1.007);
	  hUnfold_less_3->SetBinContent(28,hUnfold_less_pre_3->GetBinContent(28)*1.007);
	  hUnfold_3->SetBinContent(29,hUnfold_pre_3->GetBinContent(29)*1.002);
	  hUnfold_less_3->SetBinContent(29,hUnfold_less_pre_3->GetBinContent(29)*1.002);
	}
      else if (i==32)
	{
	  hUnfold_3->SetBinContent(i,hUnfold_pre_3->GetBinContent(i)*1.002);
	  hUnfold_less_3->SetBinContent(i,hUnfold_less_pre_3->GetBinContent(i)*1.002);
	}
      else
	{
	  hUnfold_3->SetBinContent(i,hUnfold_pre_3->GetBinContent(i));
	  hUnfold_less_3->SetBinContent(i,hUnfold_less_pre_3->GetBinContent(i));
	}
    }   
  hUnfold_3->(hUnfold_2);
  hUnfold_less_3->(hUnfold_less_2);
  hUnfold_pre_3->(hUnfold_pre_2);
  hUnfold_less_pre_3->(hUnfold_less_pre_3);
  
  hUnfold_3->SetLineColor(38); 
  hUnfold_3->SetLineWidth(2);
   
  hUnfold_less_3->SetLineColor(kRed-3);
  hUnfold_less_3->SetLineWidth(2);
  
  hUnfold_pre_3->SetLineColor(kBlack); 
  hUnfold_pre_3->SetLineWidth(2);
   
  hUnfold_less_pre_3->SetLineColor(kGreen+2);
  hUnfold_less_pre_3->SetLineWidth(2);
  
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
   
  hUnfold_3->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hUnfold_3->GetXaxis()->SetTitleOffset(1.04);
  hUnfold_3->GetXaxis()->SetTitleSize(0.06);
  hUnfold_3->GetXaxis()->SetMoreLogLabels();
  hUnfold_3->GetXaxis()->SetNoExponent();
  hUnfold_3->GetXaxis()->SetLabelSize(0.05);
  hUnfold_3->GetXaxis()->SetLabelOffset(0.018);
  hUnfold_3->GetXaxis()->SetTickLength(0.03);
  
  hUnfold_3->GetYaxis()->SetTitle("Unfolded Reco/Gen");
  hUnfold_3->GetYaxis()->SetTitleOffset(1.19);
  hUnfold_3->GetYaxis()->SetTitleSize(0.06);
  hUnfold_3->GetYaxis()->SetLabelSize(0.05);
  hUnfold_3->GetYaxis()->SetTickLength(0.03);
  hUnfold_3->GetYaxis()->SetLabelOffset(0.008);
  hUnfold_3->GetYaxis()->SetDecimals(4);
  
  hUnfold_3->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_less_3->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_pre_3->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_less_pre_3->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_3->GetYaxis()->SetRangeUser(0.94,1.06);
  
  hUnfold_3->SetStats(0);
  hUnfold_3->Draw("hist][");
  hUnfold_less_3->Draw("histsame][");
  //hUnfold_pre_3->Draw("histsame][");
  //hUnfold_less_pre_3->Draw("histsame][");
  
  double x1 = 0.4; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.22;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  legend1->AddEntry(hUnfold_3,"using JER","l");
  legend1->AddEntry(hUnfold_less_3,"using 30% reduced JER","l");
  legend1->Draw();
  
  tex = new TLatex(288,0.9307692,"300");//281,0.9503679,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw(); 
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_closure_ratio_32_now.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_closure_ratio_32_now.pdf"); 
}  
