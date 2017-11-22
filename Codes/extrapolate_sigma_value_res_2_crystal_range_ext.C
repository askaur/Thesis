#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void extrapolate_sigma_value_res_2_crystal_range_ext(){
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
  
  double sigma[44] = {0.101393, 0.0983443, 0.0907317, 0.081573, 0.0744038, 0.0688932, 0.0639074, 0.0600544, 0.0569943, 0.054441, 0.0523783, 0.0505331, 0.0488959, 0.0474157, 0.0461897, 0.0451799, 0.0444256, 0.0436615, 0.0428515, 0.0421507, 0.0416064, 0.0412017, 0.0404884, 0.039966, 0.0395091, 0.0391713, 0.0387955, 0.0381496, 0.0378475, 0.0372572, 0.0367328, 0.0358624, 0.0363352, 0.035987, 0.0351217, 0.0364841, 0.0317143, 0.035852, 0.0333783, 0.0313051, 0.031801, 0.0200181, 0.0169323, 0.0118677};

  double sigma_err[44] = {5.46734e-05, 5.48419e-05, 6.28803e-05, 6.67156e-05, 3.80637e-05, 4.28419e-05, 5.7095e-05, 6.99967e-05, 7.93856e-05, 3.57796e-05, 2.80769e-05, 3.18903e-05, 4.06981e-05, 4.85658e-05, 5.84407e-05, 6.68122e-05, 7.43229e-05, 6.98328e-05, 3.59458e-05, 3.85227e-05, 4.4925e-05, 4.67509e-05, 5.4297e-05, 7.08292e-05, 8.0248e-05, 9.57991e-05, 0.00011196, 0.000130218, 0.000161453, 0.000199244, 0.000224129, 0.000283912, 0.000356921, 0.000400142, 0.000376028, 0.000689005, 0.000839319, 0.001211, 0.00149717, 1.60497e-05, 0.00241998, 0.00337689, 0.00463376, 0.00689125};  
 
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
  legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  legend1->AddEntry(hvalue,"Resolution ");
  legend1->AddEntry(f1,"Fit function","l");
  legend1->AddEntry(hexp,"Extrapolated function","l");
  //legend1->AddEntry((TObject*)0,"(upto 2 TeV)"," ");
  legend1->Draw();
  
  TLegend *legend1=new TLegend(0.1987952,0.1657754,0.5100402,0.3957219);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"N   3.317 #\pm 0.661","");
  legend1->AddEntry((TObject*)0,"S   1.62 #\pm 0.20","");
  legend1->AddEntry((TObject*)0,"C   0.03334 #\pm 0.00021","");
  legend1->AddEntry((TObject*)0,"s   -0.3183 #\pm 0.0386","");
  legend1->SetEntrySeparation(0.1);
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  tex = new TLatex(99.35617,-0.008093641,"110");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  //tex->Draw(); 
          
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Extrapolate_Sigma_Value_Res_2_crystal_range_ext.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Extrapolate_Sigma_Value_Res_2_crystal_range_ext.pdf");
}
