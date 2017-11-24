#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void extrapolate_theory_ratio_32_funcII()
{
  TFile *f2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Rootfile_NLO_HT_2_150/Extrapolate_theory_2_HT_2_150_funcI.root");
  TFile *f3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Rootfile_NLO_HT_2_150/Extrapolate_theory_3_HT_2_150_funcI.root");
   
  TCanvas *c = new TCanvas("c", "",500,400);
  gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
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
      
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10000000);
  
  const double xbins[46] = {70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500.};
   
  TH1D *hTrue = new TH1D ("Value"," Value", 45, xbins);

  TH1D *h1 = (TH1D*)f3->Get("True_weight"); 
  TH1D *h2_jet = (TH1D*)f2->Get("True_weight"); 
  
  h1->Divide(h2_jet);
  
  h1->GetXaxis()->SetRangeUser(80,2000); 
  h1->GetXaxis()->SetNdivisions(502);
  
 TF1 *f1 = new TF1("f1","pol8",80,2000);
 
  f1->SetParameter(0,0.0308018);
  f1->SetParameter(1,-0.000877086);
  f1->SetParameter(2,7.80865e-06);
  f1->SetParameter(3,-2.1318e-08);
  f1->SetParameter(4,3.06418e-11);
  f1->SetParameter(5,-2.57428e-14);
  f1->SetParameter(6,1.26673e-17);
  f1->SetParameter(7,-3.3805e-21);
  f1->SetParameter(8,3.77658e-25);
  
  f1->SetLineColor(kRed-3);
  f1->SetLineWidth(2);
  
  hTrue->SetTitle("");	
  hTrue->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hTrue->GetXaxis()->SetTitleOffset(1.04);
  hTrue->GetXaxis()->SetTitleSize(0.06);
  hTrue->GetXaxis()->SetMoreLogLabels();
  hTrue->GetXaxis()->SetNoExponent();
  hTrue->GetXaxis()->SetLabelSize(0.05);
  hTrue->GetXaxis()->SetLabelOffset(0.018);
  hTrue->GetXaxis()->SetTickLength(0.03);
  
  hTrue->GetYaxis()->SetTitle("#it{R}_{32}");
  hTrue->GetYaxis()->SetTitleOffset(1.00);
  hTrue->GetYaxis()->SetTitleSize(0.06);
  hTrue->GetYaxis()->SetLabelSize(0.05);
  hTrue->GetYaxis()->SetTickLength(0.03);
  hTrue->GetYaxis()->SetLabelOffset(0.008);

  h1->SetMarkerStyle(20);
  h1->SetMarkerColor(kGreen+2);
  h1->SetLineColor(kGreen+2);
  h1->SetMarkerSize(1.0);
  h1->Fit("f1","R");
 
  double par_new0 = f1->GetParameter(0);
  double par_new1 = f1->GetParameter(1);
  double par_new2 = f1->GetParameter(2);
  double par_new3 = f1->GetParameter(3);
  double par_new4 = f1->GetParameter(4);
  double par_new5 = f1->GetParameter(5);  
  double par_new6 = f1->GetParameter(6);
  double par_new7 = f1->GetParameter(7);
  double par_new8 = f1->GetParameter(8);      
  
  cout<<"  f1->SetParameter(0,"<<par_new0<<");"<<endl;
  cout<<"  f1->SetParameter(1,"<<par_new1<<");"<<endl;
  cout<<"  f1->SetParameter(2,"<<par_new2<<");"<<endl;
  cout<<"  f1->SetParameter(3,"<<par_new3<<");"<<endl;
  cout<<"  f1->SetParameter(4,"<<par_new4<<");"<<endl;
  cout<<"  f1->SetParameter(5,"<<par_new5<<");"<<endl;
  cout<<"  f1->SetParameter(6,"<<par_new6<<");"<<endl;
  cout<<"  f1->SetParameter(7,"<<par_new7<<");"<<endl;
  cout<<"  f1->SetParameter(8,"<<par_new8<<");"<<endl;
        
  double x0 = par_new0;
  double x1 = par_new1;
  double x2 = par_new2;
  double x3 = par_new3;
  double x4 = par_new4;  
  double x5 = par_new5;
  double x6 = par_new6;    
  double x7 = par_new7;    
  double x8 = par_new8;    
        
  double nbinsx = hTrue->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  for (double i = 1.; i <= nbinsx; i++)
    {
      double xt =hTrue->GetBinCenter(i);
      double y = x0 + (x1*xt) + (x2*pow(xt,2)) + (x3*pow(xt,3)) + (x4*pow(xt,4)) + (x5*pow(xt,5)) + (x6*pow(xt,6)) + (x7*pow(xt,7)) + (x8*pow(xt,8));
      hTrue->SetBinContent(i,y);
    }
    
  hTrue->SetLineColor(38);
  hTrue->SetLineStyle(2);
  hTrue->SetLineWidth(2);
  hTrue->SetMarkerColor(38);
  hTrue->SetMarkerStyle(20);
  hTrue->SetMarkerSize(0.75);
  
  TH1D *hTrue_2 = hTrue->Clone("hTrue_2");
  
  hTrue_2->SetLineColor(38);
  hTrue_2->SetLineWidth(2);
  hTrue_2->SetMarkerColor(38);
  hTrue_2->SetMarkerStyle(20);
  hTrue_2->SetMarkerSize(0.75);
  
  Double_t chi2 = f1->GetChisquare();
  Double_t ndf = f1->GetNDF();
  Double_t chi2_ndf = chi2/ndf;
  cout<<" chi2 : "<<chi2<<" ndf : "<<ndf<<" chi2/ndf : "<<chi2_ndf<<endl;
  
  hTrue->GetYaxis()->SetRangeUser(-0.09,0.6); 
  hTrue->GetXaxis()->SetRangeUser(80.,2000.); 
  hTrue->GetYaxis()->SetNdivisions(511);
  hTrue->GetXaxis()->SetNdivisions(502);  
  
  //hTrue_2->GetXaxis()->SetRangeUser(80.,200.); 
  hTrue->Draw("l");
  h1->Draw("psame");
  f1->Draw("same");
  
  double x1 = 0.65; // 0.62 fixed
  double x2 = x1 + 0.28;
  double y2 = 0.89; // fixed
  double y1 = y2 - 0.13;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry(h1,"CT10-NLO","lp");
  legend1->AddEntry(f1,"Fit function", "l");
  legend1->Draw();
  
  TLatex pt1;pt1.SetNDC();
  TLatex pt2;pt2.SetNDC();
  pt1.SetTextSize(0.05);
  //pt1.DrawLatex(0.20,0.84,"#font[61]{polynomial8 function}");
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  c->Modified();
  c->cd();
  c->SetSelected(c);
   
  TPaveStats *ptstats = new TPaveStats(0.175,0.47,0.615,0.89,"brNDC");
  ptstats->SetName("stats");
  ptstats->SetBorderSize(1);
  ptstats->SetFillColor(0);
  ptstats->SetLineColor(0);
  ptstats->SetTextAlign(12);
  ptstats->SetTextFont(42);
  ptstats->SetTextSize(0.04);
  ptstats->SetOptStat(0);
  ptstats->SetOptFit(1);
  ptstats->Draw();
  Value->GetListOfFunctions()->Add(f1);
  Value->GetListOfFunctions()->Add(ptstats);
  ptstats->SetParent(Value);
  c->Draw();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Extrapolate_Theory_Ratio_32_funcII.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Extrapolate_Theory_Ratio_32_funcII.pdf");
}

