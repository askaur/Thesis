/// optional double sided crystal ball fit to response distributions
double fdscb(double*xx,double*pp)
{
  double x   = xx[0];
  // gaussian core
  double N   = pp[0];//norm
  double mu  = pp[1];//mean
  double sig = pp[2];//variance
  // transition parameters
  double a1  = pp[3];
  double p1  = pp[4];
  double a2  = pp[5];
  double p2  = pp[6];
   
  double u   = (x-mu)/sig;
  double A1  = TMath::Power(p1/TMath::Abs(a1),p1)*TMath::Exp(-a1*a1/2);
  double A2  = TMath::Power(p2/TMath::Abs(a2),p2)*TMath::Exp(-a2*a2/2);
  double B1  = p1/TMath::Abs(a1) - TMath::Abs(a1);
  double B2  = p2/TMath::Abs(a2) - TMath::Abs(a2);

  double result(N);
  if      (u<-a1) result *= A1*TMath::Power(B1-u,-p1);
  else if (u<=a2 ) result *= TMath::Exp(-u*u/2);
  else  result *= A2*TMath::Power(B2+u,-p2);
  return result;
}

#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void fit_res_3_crystal_nomet(){
  setTDRStyle();
  
  TFile *f = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/HT_2_150_matching/RootFiles/Hadd_Resolution_total_final_nomet.root");
  TFile *file = new TFile("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/Resolution/New_Gen_50/RootFiles_HT_2_150/Fit_Res_3_final_crystal_nomet.root","RECREATE");
 
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 10000000);
  TH1D* h1;
  char hname[256];   char titlename[256];
  char histname[256];
  cout<<" "<<endl;
     
  sprintf(hname, "Hadd_Resolution_3_genbin_250-275_nomet");
               
  TCanvas *c = new TCanvas("c", "",500,500);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  c->Range(0.1694581,-2.093294,1.66798,4.306122);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetLogy();
  c->SetTickx(1);
  c->SetTicky(1);
  c->SetLeftMargin(0.1538462);
  c->SetRightMargin(0.04536489);
  c->SetTopMargin(0.055);
  c->SetBottomMargin(0.1708428);
  c->SetFrameFillStyle(0);
  c->SetFrameBorderMode(0);
  c->SetFrameFillStyle(0);
  c->SetFrameBorderMode(0);
      
  h1 = (TH1D*)(f->FindObjectAny(hname));
  
  TF1 *fgaus = new TF1("fgaus","gaus");

  TF1* fdscb = new TF1("fdscb",fdscb,0.75,1.4,7);
       
  fgaus->SetLineColor(kRed-3);
  fgaus->SetLineWidth(2); 
  h1->Fit("fgaus");
       
  double norm = fgaus->GetParameter(0);
  double mean = fgaus->GetParameter(1);
  double sigma= fgaus->GetParameter(2);
      
  cout<<" "<<endl;
  cout<<" Norm : "<<norm<<" Mean : "<<mean<<" Sigma : "<<sigma<<endl;
  cout<<" "<<endl;
  double aone(2.0),atwo(2.),pone(5.0),ptwo(5.0);
  TVirtualFitter::SetDefaultFitter("Minuit");
  for (unsigned i=0;i<4;i++) 
    {
	     
      fdscb->SetParameter(0,norm); // N
      fdscb->SetParameter(1,mean); // mean
      fdscb->SetParameter(2,sigma);// sigma
      fdscb->SetParameter(3,aone); // a1
      fdscb->SetParameter(4,pone); // p1
      fdscb->SetParameter(5,atwo); // a2
      fdscb->SetParameter(6,ptwo); // p2
    
      fdscb->SetParName(0,"N");
      fdscb->SetParName(1,"Mean");
      fdscb->SetParName(2,"Sigma");
      fdscb->SetParName(3,"#alpha_{L}");
      fdscb->SetParName(4,"n_{L}");
      fdscb->SetParName(5,"#alpha_{H}");
      fdscb->SetParName(6,"n_{H}");
	  
      //fdscb->FixParameter(1,mean);
      //fdscb->FixParameter(2,sigma);
 
      if (i>0) fdscb->FixParameter(3,aone);
      else fdscb->SetParLimits(3,1,5);

      if (i>1) fdscb->FixParameter(5,atwo);
      else fdscb->SetParLimits(5,1,5);

      fdscb->SetParLimits(4,0.,100.);
      fdscb->SetParLimits(6,0.,100.);
	   
      h1->Fit(fdscb,"RQB0+"); 
           
      norm  = fdscb->GetParameter(0);
      aone  = fdscb->GetParameter(3);
      pone  = fdscb->GetParameter(4);
      atwo  = fdscb->GetParameter(5);
      ptwo  = fdscb->GetParameter(6);
      
    } 
	
  /* fdscb->SetParameter(1,fgaus->GetParameter(1));
     fdscb->SetParError(1,fgaus->GetParError(1));
     fdscb->SetParameter(2,fgaus->GetParameter(2));
     fdscb->SetParError(2,fgaus->GetParError(2));
  */

  h1->SetMarkerStyle(20);
  h1->SetMarkerColor(kBlack);
  h1->SetLineColor(kBlack);  
  h1->SetLineWidth(2);
  h1->SetMarkerSize(0.4);

  Double_t chi2 = fdscb->GetChisquare();
  Double_t ndf = fdscb->GetNDF();
  Double_t chi2_ndf = chi2/ndf;
  cout<<" "<<endl;
  cout<<" Anter : "<<" Mean : "<<fdscb->GetParameter(1)<<" Sigma : "<<fdscb->GetParameter(2)<<" Error : "<<fdscb->GetParError(2)<<endl;
  cout<<" "<<endl;
  cout<<" chi2 : "<<chi2<<" ndf : "<<ndf<<" chi2/ndf : "<<chi2_ndf<<endl;
  cout<<" "<<endl;
 
  std::ostringstream chi;    
  chi<<chi2_ndf;
  cout<<" Value : "<<chi.str().c_str()<<endl; 
  cout<<" "<<endl;
       
  fdscb->SetLineColor(38); 	
  fdscb->SetLineWidth(2); 
      
  h1->GetXaxis()->SetRangeUser(0.4,1.6);
  h1->GetXaxis()->SetTitle("#frac{Reco #it{H}_{T,2}/2}{Gen #it{H}_{T,2}/2}");
  h1->GetXaxis()->SetTitleOffset(1.83);
  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetXaxis()->SetMoreLogLabels();
  h1->GetXaxis()->SetNoExponent();
  h1->GetXaxis()->SetLabelSize(0.04);
  h1->GetXaxis()->SetLabelOffset(0.018);
  h1->GetXaxis()->SetTickLength(0.03);
  h1->GetXaxis()->SetDecimals(4);
 
  h1->GetYaxis()->SetTitle("arb. unit");
  h1->GetYaxis()->SetTitleOffset(1.25);
  h1->GetYaxis()->SetTitleSize(0.05);
  h1->GetYaxis()->SetLabelSize(0.04);
  h1->GetYaxis()->SetTickLength(0.03);
  h1->GetYaxis()->SetRangeUser(0.1,10000.); 
	
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.18;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(fdscb,"Crystal Ball","l");
  legend1->AddEntry(fgaus,"Gaussian","l");
  legend1->Draw();

  TPaveStats *ptstats = new TPaveStats(0.125,0.5783898,0.4252874,0.8919492,"brNDC");
  ptstats->SetName("stats");
  ptstats->SetBorderSize(1);
  ptstats->SetFillColor(0);
  ptstats->SetLineColor(0);
  ptstats->SetTextAlign(12);
  ptstats->SetTextSize(0.035);
  ptstats->SetTextFont(42);
  ptstats->SetOptStat(0);
  ptstats->SetOptFit(1);
  //ptstats->Draw();
  h1->GetListOfFunctions()->Add(fdscb);
  h1->GetListOfFunctions()->Add(ptstats);
  ptstats->SetParent(h1);
  gStyle->SetOptFit(0);
  gStyle->SetOptStat(0);
  //gStyle->SetOptStat("MR");
  
  TLatex pt1;pt1.SetNDC();
  pt1.SetTextSize(0.03);
  pt1.DrawLatex(0.19,0.85,"#font[42]{250 GeV #leq Gen #it{H}_{T,2}/2 < 275 GeV}");
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.84,0.95,"#font[61]{8 TeV}");
  
  double xmin = 0.30;
  double xmax = xmin + 0.09;
  double ymin = 0.7;
  double ymax = 19.;

  TBox *b = new TBox(xmin, ymin, xmax, ymax);
  b->SetFillStyle(1001);
  b->SetFillColor(0);
  b->SetLineColor(0);
  b->Draw();
  
  tex = new TLatex(0.3048667,0.8449578,"10#lower[-0.75]{#scale[0.7]{0}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(0.3048667,8.346791,"10#lower[-0.75]{#scale[0.7]{1}}");
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->Draw();
  
  sprintf(histname, "Fit_Res_3_final_crystal_genbin_250-275");
  h1->SetName(histname);
  
  h1->Write(histname,TObject::kWriteDelete);
  
  TString plotname =  h1->GetName();
      
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/"+plotname+"_crystal_nomet.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/"+plotname+"_crystal_nomet.pdf");
  c->Draw();

  file->Write();
  //file->Close();

}
