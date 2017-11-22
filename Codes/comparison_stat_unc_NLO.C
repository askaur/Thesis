double makeCanvas(TCanvas *c, TH1D *hData_err, TH1D *hData_unfold_err, TString plotname, bool twojet, bool ratio_up, bool ratio_down)
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
  c->SetLogy();
 
  hData_err->SetMarkerSize(0.75);
  hData_err->SetMarkerColor(38);
  hData_err->SetLineColor(38);
  hData_err->SetLineWidth(2);
  hData_err->SetMarkerStyle(20);
 
  hData_unfold_err->SetMarkerSize(0.75);
  hData_unfold_err->SetMarkerColor(kRed-3);
  hData_unfold_err->SetLineColor(kRed-3);
  hData_unfold_err->SetLineWidth(2);
  hData_unfold_err->SetMarkerStyle(20);
  
  if (twojet) double max = 2000.0;
  else double max = 1680.0;
  hData_unfold_err->GetXaxis()->SetRangeUser(300.0,max);
  hData_err->GetXaxis()->SetRangeUser(300.0,max);
  hData_unfold_err->SetTitle("");
  hData_unfold_err->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hData_unfold_err->GetXaxis()->SetTitleOffset(1.04);
  hData_unfold_err->GetXaxis()->SetTitleSize(0.06);
  hData_unfold_err->GetXaxis()->SetMoreLogLabels();
  hData_unfold_err->GetXaxis()->SetNoExponent();
  hData_unfold_err->GetXaxis()->SetLabelSize(0.05);
  hData_unfold_err->GetXaxis()->SetLabelOffset(0.018);
  hData_unfold_err->GetXaxis()->SetTickLength(0.03);
  
  if (ratio_up) hData_unfold_err->GetYaxis()->SetTitle("Fractional Stat. Uncertainty (Up)");
  else if (ratio_down) hData_unfold_err->GetYaxis()->SetTitle("Fractional Stat. Uncertainty (Down)");
  else hData_unfold_err->GetYaxis()->SetTitle("Fractional Stat. Uncertainty");
  hData_unfold_err->GetYaxis()->SetTitleOffset(1.);
  hData_unfold_err->GetYaxis()->SetTitleSize(0.06);
  hData_unfold_err->GetYaxis()->SetLabelSize(0.05);
  hData_unfold_err->GetYaxis()->SetTickLength(0.03);
  hData_unfold_err->GetYaxis()->SetLabelOffset(0.008);
  hData_unfold_err->GetYaxis()->SetNdivisions(509); 
  
  hData_unfold_err->SetStats(0);
  hData_err->SetStats(0);
  
  hData_unfold_err->Scale(1.04);
  
  hData_unfold_err->GetYaxis()->SetRangeUser(0.0003106334,2.482748);
  
  hData_unfold_err->Draw("hist][");
  hData_err->Draw("histsame][");
  
  double x1 = 0.52; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.20;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  if (twojet) legend1->AddEntry((TObject*)0,"n_{j} #geq 2","");
  else if (ratio_up || ratio_down) legend1->AddEntry((TObject*)0,"R_{32}","");
  else legend1->AddEntry((TObject*)0,"n_{j} #geq 3","");
  legend1->AddEntry(hData_err,"Before unfolding","l");
  legend1->AddEntry(hData_unfold_err,"After unfolding","l");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  double xmin = 270;
  double xmax = 295;
  double ymin = 0.7;
  double ymax = 1.5;

  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  tex = new TLatex(261.286,0.8414808,"10#lower[-0.75]{#scale[0.7]{0}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(281,0.0001603593,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  if (!twojet)
  {
   tex = new TLatex(1541,0.0001603593,"1680");
   tex->SetTextFont(42);
   tex->SetTextSize(0.05);
   tex->SetLineWidth(2);
   tex->Draw();
  }
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_stat_unc_" + plotname + ".gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_stat_unc_" + plotname + ".pdf"); 

} 
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void comparison_stat_unc_NLO(){
  setTDRStyle();
  
  char figname[256];
  
  TFile *fdata = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data/Rootfile_Data_HT_2_150/Hadd_PFJetHT_2_150.root");
  
  TFile *fdata_unfold_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_range_funcI.root");

  TFile *fdata_unfold_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_3_funcI.root");
  
  TFile *fdata_ratio = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data_MC_NLO/Rootfile_HT_2_150/Ratio_32_Data_HT_2_150.root");
  TFile *fdata_unfold_ratio = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_ratio_32_range.root");
 
   
  //****************************************************************
  TH1D *hData_2 =  (TH1D*)fdata->Get("Hadd_PFJetHT_2_150_2");
  TH1D *hData_unfold_2 =  (TH1D*)fdata_unfold_2->Get("Unfolded_Data_NLO");

  TH1D *hData_3 =  (TH1D*)fdata->Get("Hadd_PFJetHT_2_150_3");
  TH1D *hData_unfold_3 =  (TH1D*)fdata_unfold_3->Get("Unfolded_Data_NLO");
  
  TH1D *hData_down =  (TH1D*)fdata_ratio->Get("Ratio_32_Data_Down");
  TH1D *hData_unfold_down =  (TH1D*)fdata_unfold_ratio->Get("Unfolded_ratio_32_down_smear2");
  
  TH1D *hData_up =  (TH1D*)fdata_ratio->Get("Ratio_32_Data_Up");
  TH1D *hData_unfold_up =  (TH1D*)fdata_unfold_ratio->Get("Unfolded_ratio_32_up_smear2");

  const double new_bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
 
  TH1D *hData_err_2 = new TH1D("Before Unfolding 2","Before Unfolding 2",56.0,new_bins);
  TH1D *hData_unfold_err_2 = new TH1D("After Unfolding 2","After Unfolding 2",56.0,new_bins);
  TH1D *hData_err_3 = new TH1D("Before Unfolding 3","Before Unfolding 3",56.0,new_bins);
  TH1D *hData_unfold_err_3 = new TH1D("After Unfolding 3","After Unfolding 3",56.0,new_bins);
  TH1D *hData_err_up = new TH1D("Before Unfolding Up","Before Unfolding Up",56.0,new_bins);
  TH1D *hData_unfold_err_up = new TH1D("After Unfolding Up","After Unfolding Up",56.0,new_bins);
  TH1D *hData_err_down = new TH1D("Before Unfolding Down","Before Unfolding Down",56.0,new_bins);
  TH1D *hData_unfold_err_down = new TH1D("After Unfolding Down","After Unfolding Down",56.0,new_bins);
 
  double nbinsx = hData_2->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  for (double i = 1.; i <= nbinsx; i++)
    {
      hData_err_2->SetBinContent(i,hData_2->GetBinError(i)/hData_2->GetBinContent(i));
      hData_unfold_err_2->SetBinContent(i,hData_unfold_2->GetBinError(i)/hData_unfold_2->GetBinContent(i));
      
      hData_err_3->SetBinContent(i,hData_3->GetBinError(i)/hData_3->GetBinContent(i));
      hData_unfold_err_3->SetBinContent(i,hData_unfold_3->GetBinError(i)/hData_unfold_3->GetBinContent(i));
      
      hData_err_up->SetBinContent(i,hData_up->GetBinError(i)/hData_up->GetBinContent(i));
      hData_unfold_err_up->SetBinContent(i,hData_unfold_up->GetBinError(i)/hData_unfold_up->GetBinContent(i));
      
      hData_err_down->SetBinContent(i,hData_down->GetBinError(i)/hData_down->GetBinContent(i));
      hData_unfold_err_down->SetBinContent(i,hData_unfold_down->GetBinError(i)/hData_unfold_down->GetBinContent(i));      
    }
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"2_HT_2_150");
  makeCanvas(c,hData_err_2,hData_unfold_err_2,figname,1,0,0);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"3_HT_2_150");
  makeCanvas(c,hData_err_3,hData_unfold_err_3,figname,0,0,0);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"ratio_32_up");
  makeCanvas(c,hData_err_up,hData_unfold_err_up,figname,0,1,0);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"ratio_32_down");
  makeCanvas(c,hData_err_down,hData_unfold_err_down,figname,0,0,1);
 }  
