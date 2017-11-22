#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void correlationM_NLO_3()
{
  TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/CorrelationM_NLO_3_ite4.root");
   
  //************************************************************************
    
  TH2D *h1_11 =  (TH2D*)f1->Get("corr_matrix");
          
  int G1_x = h1_11->GetNbinsX();
  int G1_y = h1_11->GetNbinsY();
  //int G1_y = 2;

  TCanvas *c = new TCanvas("c", "",500,400);
  c->Range(2.318121,0.4327759,3.346459,1.683612);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  c->SetLeftMargin(0.1546185);
  //c->SetRightMargin(0.04417671);
  c->SetTopMargin(0.06684492);
  c->SetBottomMargin(0.1336898);
  c->SetLogx();
  c->SetLogy();
   
   gStyle->SetPalette(52);
  h1_11->SetContour(20);
  
  h1_11->SetTitle("");  
  h1_11->SetContour(20);
  h1_11->Draw("colz");
  //h1_11->GetXaxis()->SetRange(22,54);
  h1_11->GetXaxis()->SetRange(22,50);  
  h1_11->GetXaxis()->SetTitleSize(0.06);
  h1_11->GetXaxis()->SetTitleOffset(1.02);
  h1_11->GetXaxis()->SetLabelSize(0.05);
  h1_11->GetXaxis()->SetLabelOffset(0.010);
  h1_11->GetYaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h1_11->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h1_11->GetYaxis()->SetRange(22,50);
  h1_11->GetYaxis()->SetTitleOffset(1.25);
  h1_11->GetYaxis()->SetTitleSize(0.06);
  h1_11->GetYaxis()->SetLabelSize(0.05);
  h1_11->GetYaxis()->SetLabelOffset(0.014);
  h1_11->GetZaxis()->SetRangeUser(-1.0, 1.0);
  h1_11->GetZaxis()->SetTickLength(0.03);
  
  TPaletteAxis *palette = 
    (TPaletteAxis*)h1_11->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(.905);
  palette->SetX2NDC(0.935);
  palette->SetY1NDC(0.135);
  palette->SetY2NDC(0.934);
  c->Modified();
  
  TLatex pt1;pt1.SetNDC();
  pt1.SetTextSize(0.05);
  pt1.DrawLatex(0.16,0.95,"#font[42]{n_{j} #geq 3}");// (Iterations = 10)");
  
  tex = new TLatex(248.3114,288.7939,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();  // y axis
  
  tex1 = new TLatex(278.7581,265.9291,"300");
  tex1->SetTextFont(42);
  tex1->SetLineStyle(2);
  tex1->SetLineWidth(2);
  tex1->Draw(); // x axis
  
  tex = new TLatex(248.3114,1613.59,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw(); //y axis
  
  tex1 = new TLatex(1541,265.9291,"1680");
  tex1->SetTextFont(42);
  tex1->SetLineStyle(2);
  tex1->SetLineWidth(2);
  tex1->Draw(); // x axis
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.82,0.95,"#font[61]{8 TeV}");
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Correlation_Matrix_NLO_3_ite4.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Correlation_Matrix_NLO_3_ite4.pdf");
}
  
