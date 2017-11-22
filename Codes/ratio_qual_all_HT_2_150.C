double makeCanvas (TCanvas *c, TPad *c1_2, TPad *c1_1, TH1D *hData, TH1D *hMC, int num, bool twojet,  TString plotname)
{
  hData->Scale(1/hData->Integral());
  hMC->Scale(1/hMC->Integral());
  
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30; 
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.22; 
  
  c->Range(0,0,1,1);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetFrameBorderMode(0);
  
  // ------------>Primitives in pad: c1_2
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  c1_2->Draw();
  c1_2->cd();
  c1_2->Range(-0.1613298,-4.212891,1.034181,0.262741);
  c1_2->SetFillColor(0);
  c1_2->SetBorderMode(0);
  c1_2->SetBorderSize(2);
  c1_2->SetLogy();
  c1_2->SetTickx(1);
  c1_2->SetTicky(1);
  c1_2->SetLeftMargin(0.1349463);
  c1_2->SetRightMargin(0.02859079);
  c1_2->SetTopMargin(0.07996238);
  c1_2->SetBottomMargin(0.04756668);
  c1_2->SetFrameBorderMode(0);
  c1_2->SetFrameBorderMode(0);
  
  hMC->SetFillColor(38);
  hMC->SetMarkerStyle(20);
  hMC->SetMarkerColor(38);
  hMC->SetLineColor(38); 
 
  hData->SetMarkerStyle(20);
  hData->SetMarkerColor(kBlack);
  hData->SetLineColor(kBlack); 
  hData->SetMarkerSize(0.75);
  hData->SetLineWidth(2);
  
  hMC->GetXaxis()->SetRangeUser(0.0,1.0);  
  hData->GetXaxis()->SetRangeUser(0.0,1.0);
  hMC->GetYaxis()->SetRangeUser(0.0001,1.);
  hMC->GetXaxis()->SetRangeUser(0.0,1.0);
  hMC->SetTitle("");
  hMC->SetTitleSize(1.0);
  hMC->GetXaxis()->SetTitle("");
  hMC->GetYaxis()->SetTitle("arb. unit");
  hMC->GetXaxis()->SetTickLength(0.03);
  hMC->GetXaxis()->SetLabelOffset(999);
  hMC->GetXaxis()->SetNdivisions(509);    
  hMC->GetYaxis()->SetTitleOffset(1.00);
  hMC->GetYaxis()->SetTitleSize(0.07);
  hMC->GetYaxis()->SetLabelSize(0.055);
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
  legend1->AddEntry(hData,"Data","pel");
  legend1->AddEntry(hMC,"MG5+P6");
  legend1->Draw();
  
  double xmin = -0.075;
  double xmax = xmin + 0.072;
  double ymin = 0.42;
  double ymax = ymin + 1.05;

  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  tex = new TLatex(-0.07579456,0.8227901,"10#lower[-0.75]{#scale[0.6]{0}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.06);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.06);
  pt3.DrawLatex(0.69,0.94,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  c1_2->Modified();
  c->cd();

  TH1D *hrat_MC = hData->Clone("hrat_MC");
 
  hrat_MC->Divide(hMC);
   
  hrat_MC->SetMarkerColor(kBlack);
  hrat_MC->SetMarkerStyle(20);
  hrat_MC->SetLineColor(kBlack);
  hrat_MC->SetMarkerSize(0.75);
 
  c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  c1_1->Draw();
  c1_1->cd();
  c1_1->Range(-0.1610322,-0.9528421,1.029367,2.140632);
  c1_1->SetFillColor(0);
  c1_1->SetBorderMode(0);
  c1_1->SetBorderSize(1);
  c1_1->SetTickx(1);
  c1_1->SetTicky(1);
  c1_1->SetLeftMargin(0.1352758);
  c1_1->SetRightMargin(0.02467011);
  c1_1->SetTopMargin(0.04546073);
  c1_1->SetBottomMargin(0.3080169);
  c1_1->SetFrameBorderMode(0);
  c1_1->SetFrameBorderMode(0);
   
  std::cout<<"Debug1"<<std::endl;
  
  hrat_MC->GetYaxis()->SetRangeUser(0.0,2.0);
  hrat_MC->GetXaxis()->SetRangeUser(0.0,1.0);
  hrat_MC->SetTitle("  ");
  hrat_MC->GetXaxis()->SetTickLength(0.07);
  if (num==1) hrat_MC->GetXaxis()->SetTitle("charged hadron fraction");
  if (num==2) hrat_MC->GetXaxis()->SetTitle("neutral hadron fraction");
  if (num==3) hrat_MC->GetXaxis()->SetTitle("photon fraction");
  if (num==4) hrat_MC->GetXaxis()->SetTitle("electron hadron fraction");
  if (num==5) hrat_MC->GetXaxis()->SetTitle("muon hadron fraction");
  hrat_MC->GetXaxis()->SetTitleSize(0.15);
  hrat_MC->GetXaxis()->SetTitleOffset(0.98);
  hrat_MC->GetXaxis()->SetMoreLogLabels();
  hrat_MC->GetXaxis()->SetNoExponent();
  hrat_MC->GetXaxis()->SetLabelSize(0.13);
  hrat_MC->GetYaxis()->SetNdivisions(507);
  hrat_MC->GetYaxis()->SetLabelSize(0.12);
  hrat_MC->GetYaxis()->SetTitle("Data/MC");
  hrat_MC->GetYaxis()->SetTitleOffset(0.48);
  hrat_MC->GetYaxis()->SetTitleSize(0.15);
  hrat_MC->GetXaxis()->SetLabelOffset(0.02);
  hrat_MC->GetYaxis()->SetLabelOffset(0.013);
  hrat_MC->GetXaxis()->SetNdivisions(509);  
  hrat_MC->SetStats(0);
  hrat_MC->Draw("e");   
  
  TLine l(0.0,1.0,1.0,1.0);
  l.SetLineWidth(2);
  l.SetLineColor(kGray+2);
  l.SetLineStyle(5);
  l.Draw("same");
  
  c->Draw();
  c->Update();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_" + plotname + "_HT_2_150.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_" + plotname + "_HT_2_150.pdf"); 
  //c->Close();
}
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void ratio_qual_all_HT_2_150(){
  //setTDRStyle();
  TFile *fdata = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data/Rootfile_Data_HT_2_150/Hadd_PFJetHT_2_150_qual.root");
  TFile *fMC = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/Rootfile_HT_2_150/Hadd_PFMCJetHT_2_150_qual.root");
 
  char figname[256];
  
  //*****************************************************************
  //Charged Hadron fraction
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_ch_2 = (TH1D*)fdata->Get("ChHadFrac_qual_2");
  TH1D *hMC_ch_2 =  (TH1D*)fMC->Get("ChHadFrac_qual_2");
  
  sprintf(figname,"ChHadFrac_2");
  makeCanvas(c,c1_2,c1_1,hData_ch_2, hMC_ch_2, 1, 1,figname);
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);  
  
  TH1D *hData_ch_3 = (TH1D*)fdata->Get("ChHadFrac_qual_3");
  TH1D *hMC_ch_3 =  (TH1D*)fMC->Get("ChHadFrac_qual_3");
  
  sprintf(figname,"ChHadFrac_3");
  makeCanvas(c,c1_2,c1_1,hData_ch_3, hMC_ch_3, 1, 0,figname);
  
  //*****************************************************************
  //Neutral Hadron fraction
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_nu_2 = (TH1D*)fdata->Get("NuHadFrac_qual_2");
  TH1D *hMC_nu_2 =  (TH1D*)fMC->Get("NuHadFrac_qual_2");
  
  sprintf(figname,"NuHadFrac_2");
  makeCanvas(c,c1_2,c1_1,hData_nu_2, hMC_nu_2, 2, 1,figname);
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_nu_3 = (TH1D*)fdata->Get("NuHadFrac_qual_3");
  TH1D *hMC_nu_3 =  (TH1D*)fMC->Get("NuHadFrac_qual_3");
  
  sprintf(figname,"NuHadFrac_3");
  makeCanvas(c,c1_2,c1_1,hData_nu_3, hMC_nu_3, 2, 0,figname);
  
  //***************************************************************** 
  //Photon fraction
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_ph_2 = (TH1D*)fdata->Get("PhFrac_qual_2");
  TH1D *hMC_ph_2 =  (TH1D*)fMC->Get("PhFrac_qual_2");
  
  sprintf(figname,"PhFrac_2");
  makeCanvas(c,c1_2,c1_1,hData_ph_2, hMC_ph_2, 3, 1,figname);
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_ph_3 = (TH1D*)fdata->Get("PhFrac_qual_3");
  TH1D *hMC_ph_3 =  (TH1D*)fMC->Get("PhFrac_qual_3");
  
  sprintf(figname,"PhFrac_3");
  makeCanvas(c,c1_2,c1_1,hData_ph_3, hMC_ph_3, 3, 0,figname);

  //*****************************************************************
  //Electron fraction
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_el_2 = (TH1D*)fdata->Get("ElFrac_qual_2");
  TH1D *hMC_el_2 =  (TH1D*)fMC->Get("ElFrac_qual_2");
  
  sprintf(figname,"ElFrac_2");
  makeCanvas(c,c1_2,c1_1,hData_el_2, hMC_el_2, 4, 1,figname);
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_el_3 = (TH1D*)fdata->Get("ElFrac_qual_3");
  TH1D *hMC_el_3 =  (TH1D*)fMC->Get("ElFrac_qual_3");
  
  sprintf(figname,"ElFrac_3");
  makeCanvas(c,c1_2,c1_1,hData_el_3, hMC_el_3, 4, 0,figname);

  //*****************************************************************
  //Muon fraction
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_mu_2 = (TH1D*)fdata->Get("MuFrac_qual_2");
  TH1D *hMC_mu_2 =  (TH1D*)fMC->Get("MuFrac_qual_2");
  
  sprintf(figname,"MuFrac_2");
  makeCanvas(c,c1_2,c1_1,hData_mu_2, hMC_mu_2, 5, 1,figname);
  
  TCanvas *c = new TCanvas("c", "",66,136,500,502);
  TPad *c1_2 = new TPad("c1_2", "newpad",0.008064516,0.309322,0.9899194,0.9915254);
  TPad *c1_1 = new TPad("c1_1", "newpad",0.007028112,0.01054852,0.9889558,0.3206751);
  
  TH1D *hData_mu_3 = (TH1D*)fdata->Get("MuFrac_qual_3");
  TH1D *hMC_mu_3 =  (TH1D*)fMC->Get("MuFrac_qual_3");
  
  sprintf(figname,"MuFrac_3");
  makeCanvas(c,c1_2,c1_1,hData_mu_3, hMC_mu_3, 5, 0,figname);
 }
