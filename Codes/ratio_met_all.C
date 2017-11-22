double makeCanvas(TCanvas *c, TH1D *hMC, TH1D *hData, TString plotname, bool twojet)
{
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30; 
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.22; 
  
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
  c->SetLogy();
  
  hData->Scale(1/hData->Integral());
  hMC->Scale(1/hMC->Integral());
  
  hMC->SetFillColor(38);
  hMC->SetMarkerStyle(1);
  hMC->SetMarkerColor(38);
  hMC->SetLineColor(38); 
 
  hData->SetMarkerStyle(20);
  hData->SetLineWidth(2); 
  hData->SetMarkerColor(kBlack);
  hData->SetLineColor(kBlack); 
  hData->SetMarkerSize(0.75);
  
  hMC->GetXaxis()->SetRangeUser(0.0,1.0);  
  hData->GetXaxis()->SetRangeUser(0.0,1.0);
  hMC->GetYaxis()->SetRangeUser(0.00000001,1.);
  hMC->SetTitle("");
  hMC->GetXaxis()->SetTitle("E^{miss}_{T}/#sum E_{T}");
  hMC->GetXaxis()->SetTitleOffset(1.45);
  hMC->GetXaxis()->SetTitleSize(0.04);
  hMC->GetXaxis()->SetMoreLogLabels();
  hMC->GetXaxis()->SetNoExponent();
  hMC->GetXaxis()->SetLabelSize(0.05);
  hMC->GetXaxis()->SetLabelOffset(0.018);
  hMC->GetXaxis()->SetNdivisions(509);  
  hMC->GetXaxis()->SetTickLength(0.03);  
  
  hMC->GetYaxis()->SetTitle("arb. unit");
  hMC->GetYaxis()->SetTitleOffset(1.15);
  hMC->GetYaxis()->SetTitleSize(0.06);
  hMC->GetYaxis()->SetLabelSize(0.05);
  hMC->GetYaxis()->SetTickLength(0.03);  

  hMC->SetStats(0);
  hData->SetStats(0);
  hMC->Draw("hist][");
  hData->Draw("esame");
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  if (twojet) legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  else legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hData,"Data", "pel");
  legend1->AddEntry(hMC,"MG5+P6");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  TLine l(0.3,0.00000001,0.3,1.0);
  l.SetLineWidth(2);
  l.SetLineColor(kRed-3);
  l.SetLineStyle(2);
  l.Draw();
    
  double xmin = -0.075;
  double xmax = xmin + 0.072;
  double ymin = 0.42;
  double ymax = ymin + 1.05;

  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  //tex = new TLatex(-0.07769427,0.6909791,"10^{0}");
  tex = new TLatex(-0.07769427,0.6909791,"10#lower[-0.75]{#scale[0.7]{0}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c->Draw();
  c->Update();
   
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Missing_ET_" + plotname + ".gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Missing_ET_" + plotname + ".pdf"); 
}

#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_met_all(){
  setTDRStyle();
  TFile *fdata = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2_New/Data/HT_2_150/Hadd/RootFiles/Hadd_PFJet_met.root");
  TFile *fMC = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/Rootfile_HT_2_150/Hadd_PFMCJetHT_2_150_met.root");
  
  //*****************************************************************
  char figname[256];
  
  TH1D *hData2 = (TH1D*)fdata->Get("Missing_ET_2");
  TH1D *hMC2 =  (TH1D*)fMC->Get("Missing_ET_Ht_2_2");
  
  TH1D *hData3 = (TH1D*)fdata->Get("Missing_ET_3");
  TH1D *hMC3 =  (TH1D*)fMC->Get("Missing_ET_Ht_2_3");
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"2");
  makeCanvas(c,hMC2,hData2,figname,1);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"3");
  makeCanvas(c,hMC3,hData3,figname,0); 
}
