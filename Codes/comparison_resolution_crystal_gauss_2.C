#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void comparison_resolution_crystal_gauss_2(){
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
 
  double bins[45] = {70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000.};
  
  double sigma[44] = {0.101393, 0.0983443, 0.0907317, 0.081573, 0.0744038, 0.0688932, 0.0639074, 0.0600544, 0.0569943, 0.054441, 0.0523783, 0.0505331, 0.0488959, 0.0474157, 0.0461897, 0.0451799, 0.0444256, 0.0436615, 0.0428515, 0.0421507, 0.0416064, 0.0412017, 0.0404884, 0.039966, 0.0395091, 0.0391713, 0.0387955, 0.0381496, 0.0378475, 0.0372572, 0.0367328, 0.0358624, 0.0363352, 0.035987, 0.0351217, 0.0364841, 0.0317143, 0.035852, 0.0333783, 0.0313051, 0.031801, 0.0200181, 0.0169323, 0.0118677};

  double sigma_err[44] = {5.46734e-05, 5.48419e-05, 6.28803e-05, 6.67156e-05, 3.80637e-05, 4.28419e-05, 5.7095e-05, 6.99967e-05, 7.93856e-05, 3.57796e-05, 2.80769e-05, 3.18903e-05, 4.06981e-05, 4.85658e-05, 5.84407e-05, 6.68122e-05, 7.43229e-05, 6.98328e-05, 3.59458e-05, 3.85227e-05, 4.4925e-05, 4.67509e-05, 5.4297e-05, 7.08292e-05, 8.0248e-05, 9.57991e-05, 0.00011196, 0.000130218, 0.000161453, 0.000199244, 0.000224129, 0.000283912, 0.000356921, 0.000400142, 0.000376028, 0.000689005, 0.000839319, 0.001211, 0.00149717, 1.60497e-05, 0.00241998, 0.00337689, 0.00463376, 0.00689125}; 
  
  double sigma_gauss[44] = {0.0960203, 0.100295, 0.093573, 0.0845317, 0.0772213, 0.0713899, 0.0661665, 0.0622114, 0.0589988, 0.0564541, 0.054255, 0.0524025, 0.0507192, 0.0491165, 0.0479169, 0.0468251, 0.0460699, 0.0452972, 0.0444254, 0.0438586, 0.043242, 0.0427835, 0.0421163, 0.0415956, 0.0410655, 0.0407454, 0.0402249, 0.0396268, 0.0393101, 0.0389428, 0.0382705, 0.0376832, 0.0373842, 0.0368411, 0.0358491, 0.0369124, 0.0327464, 0.036597, 0.0341051, 0.031768, 0.0330041, 0.0359897, 0.0414622, 0.0432028};

  double sigma_gauss_err[44] = {3.22642e-05, 6.5723e-05, 6.43071e-05, 7.06483e-05, 3.71611e-05, 3.93007e-05, 5.29822e-05, 6.59819e-05, 7.61781e-05, 3.38311e-05, 2.6812e-05, 3.00164e-05, 3.83611e-05, 4.61757e-05, 5.64289e-05, 6.43896e-05, 7.15447e-05, 6.75035e-05, 3.41313e-05, 3.58078e-05, 4.21652e-05, 4.3864e-05, 5.41473e-05, 6.60789e-05, 7.98829e-05, 9.70546e-05, 0.000104423, 0.000130106, 0.000163475, 0.000201394, 0.000225583, 0.000292074, 0.000356532, 0.000397128, 0.000467468, 0.000691171, 0.000823963, 0.00119447, 0.00134227, 0.00151462, 0.00231565, 0.00469392, 0.00920232, 0.0138823};
  
  
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;

  TH1D *hvalue = new TH1D("Value","Value",arr_size-1,bins);
  TH1D *hvalue_gauss = new TH1D("Value_gauss","Value_gauss",arr_size-1,bins);
  
  hvalue->Sumw2();
  hvalue_gauss->Sumw2();
  
  double nbinsx = hvalue->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
 
  const double xbins[46] = {70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500.};
  
  for (int i = 1; i <=nbinsx ; i++)
    {
      double bincontent = sigma[i-1];
      hvalue->SetBinContent(i,bincontent);
      double er = sigma_err[i-1];
      hvalue->SetBinError(i,er);
      
      double bincontent_gauss = sigma_gauss[i-1];
      hvalue_gauss->SetBinContent(i,bincontent_gauss);
      double er_gauss = sigma_gauss_err[i-1];
      hvalue_gauss->SetBinError(i,er_gauss);
    }
    
  hvalue->SetTitle("");	
  hvalue->GetYaxis()->SetRangeUser(0.0,0.11); 
  hvalue->GetXaxis()->SetRangeUser(110.,2000.);
  hvalue_gauss->GetXaxis()->SetRangeUser(110.,2000.);  
   
  hvalue_gauss->SetMarkerStyle(20);
  hvalue_gauss->SetMarkerColor(kRed-3);
  hvalue_gauss->SetLineColor(kRed-3);
  hvalue_gauss->SetMarkerSize(0.75);
  
  hvalue->SetMarkerStyle(20);
  hvalue->SetMarkerColor(38);
  hvalue->SetLineColor(38);
  hvalue->SetMarkerSize(0.75);
     
  hvalue->SetStats(0);
  hvalue_gauss->SetStats(0);
  hvalue->SetTitle("");
  hvalue->GetXaxis()->SetTitle("Gen #it{H}_{T,2}/2 (GeV)");
  hvalue->GetXaxis()->SetTitleOffset(1.04);
  hvalue->GetXaxis()->SetTitleSize(0.06);
  hvalue->GetXaxis()->SetMoreLogLabels();
  hvalue->GetXaxis()->SetNoExponent();
  hvalue->GetXaxis()->SetLabelSize(0.05);
  hvalue->GetXaxis()->SetLabelOffset(0.018);
  hvalue->GetXaxis()->SetTickLength(0.03);
  
  hvalue->GetYaxis()->SetTitle("#sigma(#it{H}_{T,2}/2)/(#it{H}_{T,2}/2)");
  hvalue->GetYaxis()->SetTitleOffset(1.19);
  hvalue->GetYaxis()->SetTitleSize(0.06);
  hvalue->GetYaxis()->SetLabelSize(0.05);
  hvalue->GetYaxis()->SetTickLength(0.03);
  hvalue->GetYaxis()->SetLabelOffset(0.008);
  hvalue->GetYaxis()->SetDecimals(4);
  hvalue->Draw("p");
  hvalue_gauss->Draw("psame");
    
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.22;
   
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  legend1->AddEntry(hvalue,"Crystal Ball","pel");
  legend1->AddEntry(hvalue_gauss,"Gaussian","pel");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  tex = new TLatex(99.35617,-0.008093641,"110");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
       
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_Resolution_Crystal_Gauss_2.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_Resolution_Crystal_Gauss_2.pdf");
} 
