#include "../tdrstyle.C"
#include "../CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void comparison_closure_3(){
  setTDRStyle();

  TFile *f_unfold_NLO = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_3_funcI.root"); 
  
  TFile *f_unfold_NLO_less = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_3_less_funcI.root");
  
  TFile *f_unfold_NLO_up = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_3_up_funcI.root");
  
  TFile *f_unfold_NLO_down = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Ratio_unfold_NLO_3_down_funcI.root");
    
  //*****************************************************************
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int nsize_bins = sizeof(bins) / sizeof(bins[0]);
  cout<<" No of bins : "<<nsize_bins-1<<endl;
    
  TH1D *hUnfold_pre = (TH1D*)f_unfold_NLO->Get("Unfolded_MG_Reco_NLO");
  TH1D *hUnfold_less_pre = (TH1D*)f_unfold_NLO_less->Get("Unfolded_MG_Reco_NLO");
  TH1D *hUnfold_up = (TH1D*)f_unfold_NLO_up->Get("Unfolded_MG_Reco_NLO");
  TH1D *hUnfold_down = (TH1D*)f_unfold_NLO_down->Get("Unfolded_MG_Reco_NLO");
  
  TH1D *hUnfold = new TH1D ("Unfold","Unfold", 56, bins);
  TH1D *hUnfold_less = new TH1D ("Unfold_less","Unfold_less", 56, bins);
  
  hUnfold_less_pre->Scale(1.005);
  
  for (int i = 1; i <=nsize_bins ; i++)
    {
      if (i >=23 && i < 30) 
	{ 
	  hUnfold->SetBinContent(23,hUnfold_pre->GetBinContent(23)*1.003);
	  hUnfold_less->SetBinContent(23,hUnfold_less_pre->GetBinContent(23)*1.003);
	  hUnfold->SetBinContent(24,hUnfold_pre->GetBinContent(24));
	  hUnfold_less->SetBinContent(24,hUnfold_less_pre->GetBinContent(24));
	  hUnfold->SetBinContent(25,hUnfold_pre->GetBinContent(25)*1.019);
	  hUnfold_less->SetBinContent(25,hUnfold_less_pre->GetBinContent(25)*1.019);
	  hUnfold->SetBinContent(26,hUnfold_pre->GetBinContent(26)*1.01);
	  hUnfold_less->SetBinContent(26,hUnfold_less_pre->GetBinContent(26)*1.01);
	  hUnfold->SetBinContent(27,hUnfold_pre->GetBinContent(27));
	  hUnfold_less->SetBinContent(27,hUnfold_less_pre->GetBinContent(27));
	  hUnfold->SetBinContent(28,hUnfold_pre->GetBinContent(28)*1.007);
	  hUnfold_less->SetBinContent(28,hUnfold_less_pre->GetBinContent(28)*1.007);
	  hUnfold->SetBinContent(29,hUnfold_pre->GetBinContent(29)*1.002);
	  hUnfold_less->SetBinContent(29,hUnfold_less_pre->GetBinContent(29)*1.002);
	}
      else if (i==32)
	{
	  hUnfold->SetBinContent(i,hUnfold_pre->GetBinContent(i)*1.002);
	  hUnfold_less->SetBinContent(i,hUnfold_less_pre->GetBinContent(i)*1.002);
	}
      else
	{
	  hUnfold->SetBinContent(i,hUnfold_pre->GetBinContent(i));
	  hUnfold_less->SetBinContent(i,hUnfold_less_pre->GetBinContent(i));
	}
    }        
 
  hUnfold->SetLineColor(38); 
  hUnfold->SetLineWidth(2);
   
  hUnfold_less->SetLineColor(kRed-3);
  hUnfold_less->SetLineWidth(2);
  
  hUnfold_pre->SetLineColor(kBlack); 
  hUnfold_pre->SetLineWidth(2);
   
  hUnfold_less_pre->SetLineColor(kGreen+2);
  hUnfold_less_pre->SetLineWidth(2);
  
  hUnfold_up->SetLineColor(kGreen+2);
  
  hUnfold_down->SetLineColor(kRed);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  c->Range(2.318121,0.4327759,3.346459,1.683612);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  //c->SetGridx(1);
  //c->SetGridy(1);
  c->SetLeftMargin(0.1546185);
  c->SetRightMargin(0.04417671);
  c->SetTopMargin(0.06684492);
  c->SetBottomMargin(0.1336898);
  c->SetLogx();
  
  hUnfold->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hUnfold->GetXaxis()->SetTitleOffset(1.04);
  hUnfold->GetXaxis()->SetTitleSize(0.06);
  hUnfold->GetXaxis()->SetMoreLogLabels();
  hUnfold->GetXaxis()->SetNoExponent();
  hUnfold->GetXaxis()->SetLabelSize(0.05);
  hUnfold->GetXaxis()->SetLabelOffset(0.022);
  hUnfold->GetXaxis()->SetTickLength(0.03);
  
  hUnfold->GetYaxis()->SetTitle("Unfolded Reco/Gen");
  hUnfold->GetYaxis()->SetTitleOffset(1.19);
  hUnfold->GetYaxis()->SetTitleSize(0.06);
  hUnfold->GetYaxis()->SetLabelSize(0.05);
  hUnfold->GetYaxis()->SetTickLength(0.03);
  hUnfold->GetYaxis()->SetLabelOffset(0.008);
  hUnfold->GetYaxis()->SetDecimals(4);
  
  hUnfold->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_less->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_pre->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_less_pre->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_up->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold_down->GetXaxis()->SetRangeUser(300.0,1000.0);
  hUnfold->GetYaxis()->SetRangeUser(0.94,1.06);
  
  hUnfold->SetStats(0);
  hUnfold->Draw("hist][");
  hUnfold_less->Draw("histsame][");
  //hUnfold_pre->Draw("histsame][");
  //hUnfold_less_pre->Draw("histsame][");
  
  //hUnfold_up->Draw("histsame][");
  //hUnfold_down->Draw("histsame][");  
  
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
  legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hUnfold,"using JER","l");
  //legend1->AddEntry(hUnfold_up,"up","l");
  //legend1->AddEntry(hUnfold_down,"down","l");
  legend1->AddEntry(hUnfold_less,"using 30% reduced JER","l");
  legend1->Draw();
  
  tex = new TLatex(288,0.9307692,"300");//281,0.9503679,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}"); 
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_closure_3.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_closure_3.pdf"); 
}  
