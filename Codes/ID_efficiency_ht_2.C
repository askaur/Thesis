#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ID_efficiency_ht_2(){
  setTDRStyle();
  TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data/Rootfile_Data_HT_2_150/Hadd_PFJetHT_2_150_IDeff.root");
  
  char hname_deno[256], hname_num[256];   
  char titlename[256];
  char histname[256]; 
  char hname[256]; 
  TH1D* h1_11_deno; TH1D* h1_11_num;
  
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
	   
  sprintf(hname_deno, "TagJet");
  sprintf(hname_num, "ProbeJet");
	  
  h1_11_deno = (TH1D*)(f1->FindObjectAny(hname_deno));
  h1_11_num = (TH1D*)(f1->FindObjectAny(hname_num));

  TGraphAsymmErrors* gr = new TGraphAsymmErrors();
  gr->Divide(h1_11_num,h1_11_deno,"");
	 
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.75);
  gr->SetMarkerColor(kBlack);
  gr->SetLineColor(kBlack);
  gr->SetLineWidth(2);
	  
  gr->GetYaxis()->SetRangeUser(0.55,1.15);
  gr->GetXaxis()->SetRangeUser(315.0,2000.0);
  gr->Draw("ape");
  c->Update();
	  
  gr->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  gr->GetXaxis()->SetTitleOffset(1.04);
  gr->GetXaxis()->SetTitleSize(0.06);
  gr->GetXaxis()->SetMoreLogLabels();
  gr->GetXaxis()->SetNoExponent();
  gr->GetXaxis()->SetLabelSize(0.05);
  gr->GetXaxis()->SetLabelOffset(0.018);
  
  gr->GetYaxis()->SetTitle("Efficiency");
  gr->GetYaxis()->SetTitleOffset(1.00);
  gr->GetYaxis()->SetTitleSize(0.06);
  gr->GetYaxis()->SetLabelSize(0.05);
  gr->GetXaxis()->SetDecimals(4);
          
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  TLegend *legend1=new TLegend(0.71,0.83,0.85,0.90);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.04);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry( gr," Tight jet ID","pel");
  legend1->Draw();
	  
  sprintf(histname, "ID_Efficiency");
  gr->SetName(histname);
  
  tex = new TLatex(284.6524,0.5058529,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
      
  TString plotname =  gr->GetName();
          
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/"+plotname+".pdf");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/"+plotname+".gif");
}
