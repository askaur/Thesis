#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void extrapolate_sigma_value_res_3_crystal_ext(){
  setTDRStyle(); 
   
  //*****************************************************************;
 
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
  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 10000000);
   
  double bins[45] = {70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000.};
  
  double sigma[44] = {0.0947528, 0.0933054, 0.0876554, 0.0799165, 0.073802, 0.0687661, 0.0638187, 0.0601165, 0.0569181, 0.0543565, 0.0523659, 0.050618, 0.0489617, 0.0472193, 0.0462352, 0.0450726, 0.0442843, 0.0433551, 0.0427111, 0.0420095, 0.0413807, 0.0407872, 0.0402271, 0.0395499, 0.0391749, 0.0391274, 0.037894, 0.0378734, 0.0373059, 0.0372338, 0.0360045, 0.0360005, 0.0355753, 0.0359892, 0.0341482, 0.0326578, 0.0317928, 0.0283534, 0.0316295, 0.0262481, 0.0218398, 0.0169227, 0.00226587, 0.10052};

  double sigma_err[44] = {0.000220535, 0.000153652, 9.34521e-05, 6.203e-05, 7.55246e-05, 8.22894e-05, 9.45672e-05, 8.03111e-05, 4.19632e-05, 3.76676e-05, 4.52003e-05, 5.19588e-05, 6.22939e-05, 7.11842e-05, 8.10358e-05, 8.10838e-05, 6.78257e-05, 5.06751e-05, 4.88724e-05, 5.51845e-05, 6.52604e-05, 6.42129e-05, 8.32799e-05, 9.54027e-05, 0.000115808, 0.000138316, 0.000152991, 0.000199277, 0.00023387, 0.000289627, 0.000337095, 0.000412786, 0.000542533, 0.000575726, 0.000838036, 0.00111455, 0.00139914, 6.27406e-05, 0.00159916, 0.00176749, 0.00439887, 0.00383611, 0.021929, 2.93565e-05};  
 
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;

  TH1D *hvalue = new TH1D("Value","Value",arr_size-1,bins);
  
  hvalue->Sumw2();
  
  double nbinsx = hvalue->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
 
  const double xbins[46] = {70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500.};
  
  TH1D *hexp = new TH1D ("Exp"," Exp", 45, xbins);
    
  const string s_sigma="sqrt((TMath::Sign(1,[0])*sq([0]/x))+(sq([1])*(x^([3]-1)))+sq([2]))";
    
  TF1 *f1 = new TF1("f1",s_sigma.c_str(),110,1000);  
  
  for (int i = 1; i <=nbinsx ; i++)
    {
      double bincontent = sigma[i-1];
      hvalue->SetBinContent(i,bincontent);
      double er = sigma_err[i-1];
      hvalue->SetBinError(i,er);
    }

  //hvalue->Print("all");
  //250-1000
 
  f1->SetParName(0,"N");
  f1->SetParName(1,"S");
  f1->SetParName(2,"C");
  f1->SetParName(3,"s");  
      
  hvalue->Fit("f1","R");
  f1->SetLineColor(38);
  f1->SetLineWidth(2);  
  
  double par_new0 = f1->GetParameter(0);
  double par_new1 = f1->GetParameter(1);
  double par_new2 = f1->GetParameter(2);
  double par_new3 = f1->GetParameter(3);
    
  cout<<"  f1->SetParameter(0,"<<par_new0<<");"<<endl;
  cout<<"  f1->SetParameter(1,"<<par_new1<<");"<<endl;
  cout<<"  f1->SetParameter(2,"<<par_new2<<");"<<endl;
  cout<<"  f1->SetParameter(3,"<<par_new3<<");"<<endl;
    
  Double_t chi2 = f1->GetChisquare();
  Double_t ndf = f1->GetNDF();
  Double_t chi2_ndf = chi2/ndf;
  cout<<"  "<<endl;
  cout<<" chi2 : "<<chi2<<" ndf : "<<ndf<<" chi2/ndf : "<<chi2_ndf<<endl;
  cout<<"  "<<endl;
  
  double x0 = par_new0;
  double x1 = par_new1;
  double x2 = par_new2;
  double x3 = par_new3;
  
  double nbinsx_exp = hexp->GetXaxis()->GetNbins();
  cout<<"Exp Bins : "<<nbinsx_exp<<endl;
  for (double i_exp = 1.; i_exp <= nbinsx_exp; i_exp++)
    {
      double xt = hexp->GetBinCenter(i_exp);

      double y = sqrt(((x0/fabs(x0)) * pow(x0/xt,2)) + (pow(x1,2) * pow(xt,x3-1)) + pow(x2,2));
      //cout<<" Bin : "<<i_exp<<" Bin Center : "<<xt<<" y : "<<y<<endl;
      hexp->SetBinContent(i_exp,y);
    }
    
  hexp->GetYaxis()->SetRangeUser(0.0,0.11); 
  hvalue->GetXaxis()->SetRangeUser(110.,2000.); 
  hexp->GetXaxis()->SetRangeUser(80.,2000.); 
   
  hvalue->SetMarkerStyle(20);
  hvalue->SetMarkerColor(kBlack);
  hvalue->SetLineColor(kBlack);
  hvalue->SetMarkerSize(0.75);
  
  hexp->SetMarkerStyle(20);
  hexp->SetMarkerColor(kRed-3);
  hexp->SetLineColor(kRed-3);
  hexp->SetLineStyle(2);
  hexp->SetLineWidth(2);  
  hexp->SetMarkerSize(0.75);
    
  hvalue->SetStats(0);
  hexp->SetStats(0);
  hexp->SetTitle("");
  hexp->GetXaxis()->SetTitle("Gen #it{H}_{T,2}/2 (GeV)");
  hexp->GetXaxis()->SetTitleOffset(1.04);
  hexp->GetXaxis()->SetTitleSize(0.06);
  hexp->GetXaxis()->SetMoreLogLabels();
  hexp->GetXaxis()->SetNoExponent();
  hexp->GetXaxis()->SetLabelSize(0.05);
  hexp->GetXaxis()->SetLabelOffset(0.018);
  hexp->GetXaxis()->SetTickLength(0.03);
  hexp->GetXaxis()->SetNdivisions(502);
  
  hexp->GetYaxis()->SetTitle("#sigma(#it{H}_{T,2}/2)/(#it{H}_{T,2}/2)");
  hexp->GetYaxis()->SetTitleOffset(1.19);
  hexp->GetYaxis()->SetTitleSize(0.06);
  hexp->GetYaxis()->SetLabelSize(0.05);
  hexp->GetYaxis()->SetTickLength(0.03);
  hexp->GetYaxis()->SetLabelOffset(0.008);
  hexp->GetYaxis()->SetDecimals(4);
  
  hexp->Draw("l");
  hvalue->Draw("psame");
  f1->Draw("same");
  
  double x1 = 0.46; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.32;
   
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hvalue,"Resolution ");
  legend1->AddEntry(f1,"Fit function","l");
  legend1->AddEntry(hexp,"Extrapolated function","l");
  legend1->Draw();
  
  TLegend *legend1=new TLegend(0.1987952,0.1657754,0.5100402,0.3957219);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"N   -6.028 #\pm 0.051","");
  legend1->AddEntry((TObject*)0,"S   3.319 #\pm 0.013","");
  legend1->AddEntry((TObject*)0,"C   0.03333 #\pm 0.00005","");
  legend1->AddEntry((TObject*)0,"s   -0.5145 #\pm 0.0013","");
  legend1->Draw();
    
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  tex = new TLatex(99.35617,-0.008093641,"110");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  //tex->Draw(); 
          
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Extrapolate_Sigma_Value_Res_3_crystal_ext.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Extrapolate_Sigma_Value_Res_3_crystal_ext.pdf");
}
