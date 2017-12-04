#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void kfactor_all()
{
  setTDRStyle();

  TFile *fCT10 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Kfactor_CT10.root");
  TFile *fCT14 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Kfactor_CT14.root");
  TFile *fMMHT2014 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Kfactor_MMHT2014.root");
  TFile *fMSTW2008 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Kfactor_MSTW2008.root");
  TFile *fNNPDF23 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Kfactor_NNPDF23.root");
  
  //*****************************************************************
  
  TH1D *h_ct10_2 = (TH1D*)fCT10->Get("Inclusive_2");
  TH1D *h_ct10_3 =  (TH1D*)fCT10->Get("Inclusive_3");
  
  TH1D *h_ct14_2 = (TH1D*)fCT14->Get("Inclusive_2");
  TH1D *h_ct14_3 =  (TH1D*)fCT14->Get("Inclusive_3");
  
  TH1D *h_ct10_2_r = h_ct10_2->Clone("Inclusive_2_r");
  TH1D *h_ct10_3_r = h_ct10_3->Clone("Inclusive_3_r");
  
  TH1D *h_ct14_2_r = h_ct14_2->Clone("Inclusive_2_r");
  TH1D *h_ct14_3_r = h_ct14_3->Clone("Inclusive_3_r");
   
  h_ct10_3_r->Divide(h_ct10_2_r); 
  h_ct14_3_r->Divide(h_ct14_2_r);

  h_ct10_2->SetLineColor(38); 
  h_ct10_2->SetLineWidth(2); 
  
  h_ct10_3->SetLineColor(30);
  h_ct10_3->SetLineWidth(2); 
  
  h_ct10_3_r->SetLineColor(42);  
  h_ct10_3_r->SetLineWidth(2);
  
  h_ct14_2->SetMarkerStyle(20);
  h_ct14_2->SetMarkerColor(6);
  h_ct14_2->SetLineColor(6); 
  h_ct14_2->SetLineWidth(2); 
  h_ct14_2->SetMarkerSize(0.6); 
  
  h_ct14_3->SetMarkerStyle(20);
  h_ct14_3->SetMarkerColor(12);
  h_ct14_3->SetLineColor(12);
  h_ct14_3->SetLineWidth(2);    
  h_ct14_3->SetMarkerSize(0.6); 
     
  h_ct14_3_r->SetMarkerStyle(20);
  h_ct14_3_r->SetMarkerColor(kRed);
  h_ct14_3_r->SetLineColor(kRed);
  h_ct14_3_r->SetLineWidth(2);    
  h_ct14_3_r->SetMarkerSize(0.6); 
   
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
    
  h_ct10_3->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_ct10_2->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_ct10_3_r->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_ct14_2->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_ct14_3->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_ct14_3_r->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_ct10_3->GetYaxis()->SetRangeUser(0.9,1.7);
  h_ct10_3->SetTitle("");
  h_ct10_3->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_ct10_3->GetXaxis()->SetTitleOffset(1.04);
  h_ct10_3->GetXaxis()->SetTitleSize(0.06);
  h_ct10_3->GetXaxis()->SetMoreLogLabels();
  h_ct10_3->GetXaxis()->SetNoExponent();
  h_ct10_3->GetXaxis()->SetLabelSize(0.05);
  h_ct10_3->GetXaxis()->SetLabelOffset(0.018);
  h_ct10_3->GetXaxis()->SetTickLength(0.03);
  
  h_ct10_3->GetYaxis()->SetTitle("k-factor (#sigma_{NLO}/#sigma_{LO})");
  h_ct10_3->GetYaxis()->SetTitleOffset(1.00);
  h_ct10_3->GetYaxis()->SetTitleSize(0.06);
  h_ct10_3->GetYaxis()->SetLabelSize(0.05);
  h_ct10_3->GetYaxis()->SetTickLength(0.03);
  h_ct10_3->GetYaxis()->SetLabelOffset(0.008);
  h_ct10_3->GetYaxis()->SetNdivisions(507);
  h_ct10_3->GetYaxis()->SetDecimals(4);

  h_ct10_3->Draw("hist][");
  h_ct10_2->Draw("histsame][");
  h_ct10_3_r->Draw("histsame][");
  h_ct14_3->Draw("psame");
  h_ct14_2->Draw("psame");
  h_ct14_3_r->Draw("psame");
  
  double y2 = 0.90;
  double y1 = y2 - 0.25;
  double x1 = 0.19;
  double x2 = x1 + 0.25;

  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"CT10"," ");
  legend1->AddEntry(h_ct10_2,"n_{j} #geq 2","l");
  legend1->AddEntry(h_ct10_3,"n_{j} #geq 3","l");
  legend1->AddEntry(h_ct10_3_r,"#it{R}_{32}","l");
  legend1->Draw("sames");
  
  TLegend *legend1=new TLegend(x1 + 0.2,y1,x2 + 0.2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"CT14"," ");
  legend1->AddEntry(h_ct14_2,"n_{j} #geq 2","p");
  legend1->AddEntry(h_ct14_3,"n_{j} #geq 3","p");
  legend1->AddEntry(h_ct14_3_r,"#it{R}_{32}","p");
  legend1->Draw("sames");
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  tex = new TLatex(184.477,0.8411372,"200");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();

  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Kfactor_all_1.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Kfactor_all_1.pdf"); 
  
  //*****************************************************************  
 
  TH1D *h_mmht2014_2 = (TH1D*)fMMHT2014->Get("Inclusive_2");
  TH1D *h_mmht2014_3 =  (TH1D*)fMMHT2014->Get("Inclusive_3");
  
  TH1D *h_mstw2008_2 = (TH1D*)fMSTW2008->Get("Inclusive_2");
  TH1D *h_mstw2008_3 =  (TH1D*)fMSTW2008->Get("Inclusive_3");
  
  TH1D *h_nnpdf23_2 = (TH1D*)fNNPDF23->Get("Inclusive_2");
  TH1D *h_nnpdf23_3 =  (TH1D*)fNNPDF23->Get("Inclusive_3");
  
  TH1D *h_mmht2014_2_r = h_mmht2014_2->Clone("Inclusive_2_r");
  TH1D *h_mmht2014_3_r = h_mmht2014_3->Clone("Inclusive_3_r");
  
  TH1D *h_mstw2008_2_r = h_mstw2008_2->Clone("Inclusive_2_r");
  TH1D *h_mstw2008_3_r = h_mstw2008_3->Clone("Inclusive_3_r");
  
  TH1D *h_nnpdf23_2_r = h_nnpdf23_2->Clone("Inclusive_2_r");
  TH1D *h_nnpdf23_3_r = h_nnpdf23_3->Clone("Inclusive_3_r");
  
  gStyle->SetLineStyleString(11,"40 20"); 
   
  h_mmht2014_3_r->Divide(h_mmht2014_2_r); 
  h_mstw2008_3_r->Divide(h_mstw2008_2_r); 
  h_nnpdf23_3_r->Divide(h_nnpdf23_2_r);

  h_mmht2014_2->SetLineColor(38); 
  h_mmht2014_2->SetLineWidth(2); 
  
  h_mmht2014_3->SetLineColor(30);
  h_mmht2014_3->SetLineWidth(2); 
  
  h_mmht2014_3_r->SetLineColor(42);
  h_mmht2014_3_r->SetLineWidth(2);
        
  h_mstw2008_2->SetMarkerStyle(20);
  h_mstw2008_2->SetMarkerColor(6);
  h_mstw2008_2->SetLineColor(6); 
  h_mstw2008_2->SetLineWidth(2); 
  h_mstw2008_2->SetMarkerSize(0.6); 
  
  h_mstw2008_3->SetMarkerStyle(20);
  h_mstw2008_3->SetMarkerColor(12);
  h_mstw2008_3->SetLineColor(12);
  h_mstw2008_3->SetLineWidth(2);    
  h_mstw2008_3->SetMarkerSize(0.6); 
  
  h_mstw2008_3_r->SetMarkerStyle(20);
  h_mstw2008_3_r->SetMarkerColor(kRed);
  h_mstw2008_3_r->SetLineColor(kRed);
  h_mstw2008_3_r->SetLineWidth(2);    
  h_mstw2008_3_r->SetMarkerSize(0.6); 
           
  h_nnpdf23_2->SetLineStyle(11);
  h_nnpdf23_2->SetLineColor(kBlack); 
  h_nnpdf23_2->SetLineWidth(2); 
  
  h_nnpdf23_3->SetLineStyle(11);
  h_nnpdf23_3->SetLineColor(kRed);
  h_nnpdf23_3->SetLineWidth(2); 
 
  h_nnpdf23_3_r->SetLineStyle(11);
  h_nnpdf23_3_r->SetLineColor(kBlue);
  h_nnpdf23_3_r->SetLineWidth(2);   
    
  TCanvas *c1 = new TCanvas("c1", "",500,400);
  c1->Range(2.318121,0.4327759,3.346459,1.683612);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetTickx(1);
  c1->SetTicky(1);
  c1->SetLeftMargin(0.1546185);
  c1->SetRightMargin(0.04417671);
  c1->SetTopMargin(0.06684492);
  c1->SetBottomMargin(0.1336898);
  c1->SetLogx();
  
  h_mmht2014_3->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_mmht2014_2->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_mmht2014_3_r->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_nnpdf23_2->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_nnpdf23_3->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_nnpdf23_3_r->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_mstw2008_2->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_mstw2008_3->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_mstw2008_3_r->GetXaxis()->SetRangeUser(200.0,2000.0);
  h_mmht2014_3->GetYaxis()->SetRangeUser(0.9,1.7);
  h_mmht2014_3->SetTitle("");
  h_mmht2014_3->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_mmht2014_3->GetXaxis()->SetTitleOffset(1.04);
  h_mmht2014_3->GetXaxis()->SetTitleSize(0.06);
  h_mmht2014_3->GetXaxis()->SetMoreLogLabels();
  h_mmht2014_3->GetXaxis()->SetNoExponent();
  h_mmht2014_3->GetXaxis()->SetLabelSize(0.05);
  h_mmht2014_3->GetXaxis()->SetLabelOffset(0.018);
  h_mmht2014_3->GetXaxis()->SetTickLength(0.03);
  
  h_mmht2014_3->GetYaxis()->SetTitle("k-factor (#sigma_{NLO}/#sigma_{LO})");
  h_mmht2014_3->GetYaxis()->SetTitleOffset(1.00);
  h_mmht2014_3->GetYaxis()->SetTitleSize(0.06);
  h_mmht2014_3->GetYaxis()->SetLabelSize(0.05);
  h_mmht2014_3->GetYaxis()->SetTickLength(0.03);
  h_mmht2014_3->GetYaxis()->SetLabelOffset(0.008);
  h_mmht2014_3->GetYaxis()->SetNdivisions(507);
  h_mmht2014_3->GetYaxis()->SetDecimals(4);

  h_mmht2014_3->Draw("hist][");
  h_mmht2014_2->Draw("histsame][");
  h_mmht2014_3_r->Draw("histsame][");
  h_mstw2008_3->Draw("psame");
  h_mstw2008_2->Draw("psame");
  h_mstw2008_3_r->Draw("psame");
  h_nnpdf23_3->Draw("histsame][");
  h_nnpdf23_2->Draw("histsame][");
  h_nnpdf23_3_r->Draw("histsame][");
  
  double y2 = 0.90;
  double y1 = y2 - 0.25;
  double x1 = 0.19;
  double x2 = x1 + 0.25;

  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"MMHT2014"," ");
  legend1->AddEntry(h_mmht2014_2,"n_{j} #geq 2","l");
  legend1->AddEntry(h_mmht2014_3,"n_{j} #geq 3","l");
  legend1->AddEntry(h_mmht2014_3_r,"#it{R}_{32}","l");
  legend1->Draw("sames");
  
  TLegend *legend1=new TLegend(x2 + 0.002,y1,x2 + 0.25 + 0.002,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"MSTW2008"," ");
  legend1->AddEntry(h_mstw2008_2,"n_{j} #geq 2","p");
  legend1->AddEntry(h_mstw2008_3,"n_{j} #geq 3","p");
  legend1->AddEntry(h_mstw2008_3_r,"#it{R}_{32}","p");
  legend1->Draw("sames");
  
  TLegend *legend1=new TLegend(x2+0.25+0.002,y1,x2+0.25+0.002+0.25,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"NNPDF23"," ");
  legend1->AddEntry(h_nnpdf23_2,"n_{j} #geq 2","l");
  legend1->AddEntry(h_nnpdf23_3,"n_{j} #geq 3","l");
  legend1->AddEntry(h_nnpdf23_3_r,"#it{R}_{32}","l");
  legend1->Draw("sames");
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  tex = new TLatex(184.477,0.8411372,"200");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
 
  c1->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Kfactor_all_2.gif");
  c1->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Kfactor_all_2.pdf"); 
}  
