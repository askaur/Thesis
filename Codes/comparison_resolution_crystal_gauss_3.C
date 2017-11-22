#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void comparison_resolution_crystal_gauss_3(){
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
  
  double sigma[44] = {0.0947528, 0.0933054, 0.0876554, 0.0799165, 0.073802, 0.0687661, 0.0638187, 0.0601165, 0.0569181, 0.0543565, 0.0523659, 0.050618, 0.0489617, 0.0472193, 0.0462352, 0.0450726, 0.0442843, 0.0433551, 0.0427111, 0.0420095, 0.0413807, 0.0407872, 0.0402271, 0.0395499, 0.0391749, 0.0391274, 0.037894, 0.0378734, 0.0373059, 0.0372338, 0.0360045, 0.0360005, 0.0355753, 0.0359892, 0.0341482, 0.0326578, 0.0317928, 0.0283534, 0.0316295, 0.0262481, 0.0218398, 0.0169227, 0.00226587, 0.10052};

  double sigma_err[44] = {0.000220535, 0.000153652, 9.34521e-05, 6.203e-05, 7.55246e-05, 8.22894e-05, 9.45672e-05, 8.03111e-05, 4.19632e-05, 3.76676e-05, 4.52003e-05, 5.19588e-05, 6.22939e-05, 7.11842e-05, 8.10358e-05, 8.10838e-05, 6.78257e-05, 5.06751e-05, 4.88724e-05, 5.51845e-05, 6.52604e-05, 6.42129e-05, 8.32799e-05, 9.54027e-05, 0.000115808, 0.000138316, 0.000152991, 0.000199277, 0.00023387, 0.000289627, 0.000337095, 0.000412786, 0.000542533, 0.000575726, 0.000838036, 0.00111455, 0.00139914, 6.27406e-05, 0.00159916, 0.00176749, 0.00439887, 0.00383611, 0.021929, 2.93565e-05}; 
  
double sigma_gauss[44] = {0.0909769, 0.0952281, 0.0906817, 0.0826558, 0.0765475, 0.0711649, 0.0660278, 0.0621547, 0.0587496, 0.0563901, 0.054138, 0.0525296, 0.050792, 0.0490216, 0.0480304, 0.0467244, 0.0458792, 0.0450953, 0.0443822, 0.0435624, 0.0430021, 0.0424586, 0.0418658, 0.0411099, 0.0407337, 0.0403248, 0.0396588, 0.039347, 0.0388879, 0.0383027, 0.0379181, 0.0374496, 0.0362643, 0.0365639, 0.0354839, 0.035176, 0.0331127, 0.0297823, 0.0321372, 0.0265389, 0.0276339, 0.0230433, 0.492109, 0.308192};

  double sigma_gauss_err[44] = {0.00014693, 0.000172628, 9.83585e-05, 6.10584e-05, 6.79543e-05, 7.56172e-05, 8.95314e-05, 7.57655e-05, 3.93915e-05, 3.51309e-05, 4.27709e-05, 4.91053e-05, 5.93089e-05, 6.83701e-05, 7.85973e-05, 7.81222e-05, 6.48339e-05, 4.79142e-05, 4.64562e-05, 5.19522e-05, 6.13617e-05, 6.40077e-05, 7.84996e-05, 9.48091e-05, 0.000114969, 0.000139335, 0.000152666, 0.000187693, 0.00023353, 0.000278138, 0.000341255, 0.000415075, 0.000525333, 0.000558794, 0.000789166, 0.00111052, 0.00126452, 0.00131151, 0.0019693, 0.00173579, 0.00459673, 0.00496086, 0.436094, 0.154117};
  
  
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
  legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
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
       
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Comparison_Resolution_Crystal_Gauss_3.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Comparison_Resolution_Crystal_Gauss_3.pdf");
} 
