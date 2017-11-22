double A6 = 8000.0/(2*cosh(0));	//cosh(ymin)  
double g (double x, double A1_5, int i)
{
  return A1_5*pow(log(x/A6),i);
} 
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void extrapolate_theory_2_HT_2_150_funcI()
{
  setTDRStyle();
  TFile *f = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Rootfile_NLO_HT_2_150/Rebin_HT_2_150.root");
  
  double A6 = 8000.0/(2*cosh(0));	//cosh(ymin)
 
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
  c->SetLogy();
      
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls(10000000);
  
  const double xbins[46] = {70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500.};
   
  TH1D *hTrue = new TH1D ("Value"," Value", 45, xbins);
  
  TH1D *hTrue_weight = new TH1D ("True_weight","True_weight", 45, xbins);
  hTrue_weight->Sumw2();
  
  TH1D *h1 = (TH1D*)f->Get("NLO cross_section 2_new"); 
  
  h1->GetXaxis()->SetRangeUser(200,2000); 
  
  TString function_1 = "[0]*(pow(x,-[1]))*";
  TString function_2 = "pow(1-(x/4000),[2])*";
  TString function_3 = "exp(-[3]/x)";
  TString function = function_1 + function_2 + function_3;
  TF1 *f1 = new TF1("f1",function.Data(),200,2000);
 
  f1->SetParameter(0,7.87358e+15);
  f1->SetParameter(1,5.60949);
  f1->SetParameter(2,9.2857);
  f1->SetParameter(3,77.5885);
  
  f1->SetParName(0,"N");
  f1->SetParName(1,"a");
  f1->SetParName(2,"b");
  f1->SetParName(3,"c");
  
  hTrue->SetTitle("");	
  hTrue->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hTrue->GetXaxis()->SetTitleOffset(1.04);
  hTrue->GetXaxis()->SetTitleSize(0.06);
  hTrue->GetXaxis()->SetMoreLogLabels();
  hTrue->GetXaxis()->SetNoExponent();
  hTrue->GetXaxis()->SetLabelSize(0.05);
  hTrue->GetXaxis()->SetLabelOffset(0.018);
  hTrue->GetXaxis()->SetTickLength(0.03);
  
  hTrue->GetYaxis()->SetTitle("d#sigma/d(#it{H}_{T,2}/2) (pb/GeV)");
  hTrue->GetYaxis()->SetTitleOffset(1.00);
  hTrue->GetYaxis()->SetTitleSize(0.06);
  hTrue->GetYaxis()->SetLabelSize(0.05);
  hTrue->GetYaxis()->SetTickLength(0.03);
  hTrue->GetYaxis()->SetLabelOffset(0.008);
    
  h1->SetMarkerStyle(20);
  h1->SetMarkerColor(kGreen+2);
  h1->SetLineColor(kGreen+2);
  h1->SetMarkerSize(0.75);
  h1->Fit("f1","R");
 
  double par_new0 = f1->GetParameter(0);
  double par_new1 = f1->GetParameter(1);
  double par_new2 = f1->GetParameter(2);
  double par_new3 = f1->GetParameter(3);
  
  cout<<"  f1->SetParameter(0,"<<par_new0<<");"<<endl;
  cout<<"  f1->SetParameter(1,"<<par_new1<<");"<<endl;
  cout<<"  f1->SetParameter(2,"<<par_new2<<");"<<endl;
  cout<<"  f1->SetParameter(3,"<<par_new3<<");"<<endl;
  
  hTrue->GetYaxis()->SetRangeUser(0.0000001,1000000000.); 
  hTrue->GetXaxis()->SetRangeUser(80.,2000.); 
  hTrue->GetXaxis()->SetNdivisions(502);
  hTrue->GetYaxis()->SetNdivisions(509);
  
  double x0 = par_new0;
  double x1 = par_new1;
  double x2 = par_new2;
  double x3 = par_new3;
  
  double nbinsx = hTrue->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  for (double i = 1.; i <= nbinsx; i++)
    {
      double xt =hTrue->GetBinCenter(i);
      double y = (x0)*pow(xt,-x1)*pow(1-(xt/4000),x2)*exp(-x3/xt);
      hTrue->SetBinContent(i,y);
      hTrue_weight->SetBinContent(i,y);
      hTrue_weight->SetBinError(i,0.01*y);
    }
  
  f1->SetLineColor(kRed-3);
  f1->SetLineWidth(2);
    
  hTrue->SetLineColor(38);
  hTrue->SetLineStyle(2);
  hTrue->SetLineWidth(2);
  hTrue->SetMarkerColor(38);
  hTrue->SetMarkerStyle(20);
  hTrue->SetMarkerSize(0.75);
  
  h1->SetLineColor(kGreen+2);
  h1->SetLineWidth(2);
  h1->SetMarkerColor(kGreen+2);
  h1->SetMarkerStyle(20);
  h1->SetMarkerSize(0.75);
  
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
  
  hTrue->Draw("l");
  //hTrue_2->Draw("lpsame");
  h1->Draw("psame");
  f1->Draw("same");

  double x1 = 0.32; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.42; // fixed
  double y1 = y2 - 0.27;
   
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 2","");
  legend1->AddEntry(h1,"CT10-NLO","lp");
  legend1->AddEntry(f1,"Fit function","l");
  legend1->AddEntry(hTrue,"Extrapolated function","l");
  legend1->AddEntry((TObject*)0,"(to 80 GeV)","");
  legend1->Draw();
  
  TLatex pt1;pt1.SetNDC();
  TLatex pt2;pt2.SetNDC();
  pt1.SetTextSize(0.06);
  pt1.DrawLatex(0.20,0.84,"#font[61]{Function I}");
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  double xmin = 61;
  double xmax = 77.5;
  double ymin = 2.5;
  double ymax = 68.;

  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  tex = new TLatex(61.798,5.077921,"10#lower[-0.75]{#scale[0.7]{1}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  c->Modified();
  c->cd();
  c->SetSelected(c);
   
  TPaveStats *ptstats = new TPaveStats(0.47,0.62,0.93,0.90,"brNDC");
  ptstats->SetName("stats");
  ptstats->SetBorderSize(1);
  ptstats->SetFillColor(0);
  ptstats->SetLineColor(0);
  ptstats->SetTextAlign(12);
  ptstats->SetTextFont(42);
  ptstats->SetTextSize(0.05);
  ptstats->SetOptStat(0);
  ptstats->SetOptFit(1);
  ptstats->Draw();
  Value->GetListOfFunctions()->Add(f1);
  Value->GetListOfFunctions()->Add(ptstats);
  ptstats->SetParent(Value);
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Extrapolate_Theory_2_HT_2_150_funcI.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Extrapolate_Theory_2_HT_2_150_funcI.pdf");
  c->Draw();
}

