double makeCanvas(TCanvas *c, TH1D *hUnfold_Meas_NLO, TH1D *hTrue_NLO, TString plotname, bool twojet)
{
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

  hUnfold_Meas_NLO->Divide(hTrue_NLO); 
  hUnfold_Meas_NLO->SetMarkerSize(0.75);
  hUnfold_Meas_NLO->SetMarkerColor(kBlack);
  hUnfold_Meas_NLO->SetLineColor(kBlack);
  hUnfold_Meas_NLO->SetMarkerStyle(20);

  hUnfold_Meas_NLO->GetYaxis()->SetRangeUser(0.98,1.02);
  if (twojet) hUnfold_Meas_NLO->GetXaxis()->SetRangeUser(300.0,2000.0);
  else hUnfold_Meas_NLO->GetXaxis()->SetRangeUser(300.0,1680.0);
  hUnfold_Meas_NLO->SetTitle("");
  hUnfold_Meas_NLO->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hUnfold_Meas_NLO->GetXaxis()->SetTitleOffset(1.04);
  hUnfold_Meas_NLO->GetXaxis()->SetTitleSize(0.06);
  hUnfold_Meas_NLO->GetXaxis()->SetMoreLogLabels();
  hUnfold_Meas_NLO->GetXaxis()->SetNoExponent();
  hUnfold_Meas_NLO->GetXaxis()->SetLabelSize(0.05);
  hUnfold_Meas_NLO->GetXaxis()->SetLabelOffset(0.018);
  hUnfold_Meas_NLO->GetXaxis()->SetTickLength(0.03);
  
  hUnfold_Meas_NLO->GetYaxis()->SetTitle("Unfolded Measured_{Toy}/Gen_{Toy}");
  hUnfold_Meas_NLO->GetYaxis()->SetTitleOffset(1.19);
  hUnfold_Meas_NLO->GetYaxis()->SetTitleSize(0.06);
  hUnfold_Meas_NLO->GetYaxis()->SetLabelSize(0.05);
  hUnfold_Meas_NLO->GetYaxis()->SetTickLength(0.03);
  hUnfold_Meas_NLO->GetYaxis()->SetLabelOffset(0.008);
  hUnfold_Meas_NLO->GetYaxis()->SetNdivisions(507);
  hUnfold_Meas_NLO->GetYaxis()->SetDecimals(4);
  hUnfold_Meas_NLO->Draw("e");
  
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.1;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.1;
  
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
  legend1->Draw();
  
  tex = new TLatex(281,0.977,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  if (!twojet)
  {
   tex = new TLatex(1541,0.977,"1680");
   tex->SetTextFont(42);
   tex->SetTextSize(0.05);
   tex->SetLineWidth(2);
   tex->Draw();
  }
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  c->Draw();
  c->Update();
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Ratio_Unfolding_NLO_" + plotname + "_funcI.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Ratio_Unfolding_NLO_" + plotname + "_funcI.pdf");
  
}
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_unfold_NLO_funcI(){
  setTDRStyle();
  
  char figname[256];
   
  TFile *f_unf_NLO_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_funcI.root");
  
  TFile *f_unf_NLO_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_funcI.root");
  
  //*****************************************************************

  TH1D *hUnfold_Meas_NLO_2 = (TH1D*)f_unf_NLO_2->Get("Unfolded_Meas_NLO");
  TH1D *hTrue_NLO_2 = (TH1D*)f_unf_NLO_2->Get("True");
  
  TH1D *hUnfold_Meas_NLO_3 = (TH1D*)f_unf_NLO_3->Get("Unfolded_Meas_NLO");
  TH1D *hTrue_NLO_3 = (TH1D*)f_unf_NLO_3->Get("True");
   
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"2");
  makeCanvas(c,hUnfold_Meas_NLO_2,hTrue_NLO_2,figname,1);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"3");
  makeCanvas(c,hUnfold_Meas_NLO_3,hTrue_NLO_3,figname,0);
 }
