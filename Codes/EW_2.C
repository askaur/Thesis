#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void EW_2(){
  setTDRStyle();
  
  double ew[39] = {9.978384e-01, 9.980765e-01, 9.977172e-01, 9.974209e-01, 9.971089e-01, 9.968706e-01, 9.967038e-01, 9.965736e-01, 9.966196e-01, 9.967555e-01, 9.970193e-01, 9.974478e-01, 9.979740e-01, 9.986149e-01, 9.995117e-01, 1.000398e+00, 1.001648e+00, 1.003310e+00, 1.005060e+00, 1.007142e+00, 1.009511e+00, 1.012283e+00, 1.015626e+00, 1.018963e+00, 1.023191e+00, 1.027454e+00, 1.032799e+00, 1.038341e+00, 1.044416e+00, 1.051096e+00, 1.058894e+00, 1.065887e+00, 1.074049e+00, 1.081841e+00, 1.090637e+00, 1.098830e+00, 1.110315e+00, 1.116780e+00, 1.131001e+00};
  
    double ew_m[56] = {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 9.978384e-01, 9.980765e-01, 9.977172e-01, 9.974209e-01, 9.971089e-01, 9.968706e-01, 9.967038e-01, 9.965736e-01, 9.966196e-01, 9.967555e-01, 9.970193e-01, 9.974478e-01, 9.979740e-01, 9.986149e-01, 9.995117e-01, 1.000398e+00, 1.001648e+00, 1.003310e+00, 1.005060e+00, 1.007142e+00, 1.009511e+00, 1.012283e+00, 1.015626e+00, 1.018963e+00, 1.023191e+00, 1.027454e+00, 1.032799e+00, 1.038341e+00, 1.044416e+00, 1.051096e+00, 1.058894e+00, 1.065887e+00, 1.074049e+00, 1.081841e+00, 1.090637e+00, 1.098830e+00, 1.110315e+00, 1.116780e+00, 1.131001e+00, 1.00, 1.00};

  double bins[40] = {150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000.};
  
  double bins_m[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};

  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
  
  int arr_size_m = sizeof(bins_m)/sizeof(bins_m[0]);
  cout<<" Array size More : "<<arr_size_m-1<<endl;
  
  TH1D *h_ew = new TH1D("EW_2","EW_2",arr_size-1,bins);
  TH1D *h_ew_M = new TH1D("EW_2_M","EW_2_M",arr_size_m-1,bins_m);  

  for (int i = 0; i <= arr_size-1; i++)
    {  
      h_ew->SetBinContent(i+1,ew[i]);
    }
    
  for (int i = 0; i <= arr_size_m-1; i++)
    {  
      h_ew_M->SetBinContent(i+1,ew_m[i]);
    }  
    
  //h_ew->Print("all");
  //h_ew_M->Print("all");  
    
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
  
  h_ew->SetTitle("");
  h_ew->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_ew->GetXaxis()->SetTitleOffset(1.04);
  h_ew->GetXaxis()->SetTitleSize(0.06);
  h_ew->GetXaxis()->SetMoreLogLabels();
  h_ew->GetXaxis()->SetNoExponent();
  h_ew->GetXaxis()->SetLabelSize(0.05);
  h_ew->GetXaxis()->SetLabelOffset(0.018);
  h_ew->GetXaxis()->SetTickLength(0.03);
  
  h_ew->GetYaxis()->SetTitle("Relative Electroweak Factor");
  h_ew->GetYaxis()->SetTitleOffset(1.15);
  h_ew->GetYaxis()->SetTitleSize(0.06);
  h_ew->GetYaxis()->SetLabelSize(0.05);
  h_ew->GetYaxis()->SetTickLength(0.03);
  h_ew->GetYaxis()->SetLabelOffset(0.008);
  h_ew->GetYaxis()->SetNdivisions(507);
  h_ew->GetYaxis()->SetDecimals(4);
   
  h_ew->GetXaxis()->SetRangeUser(300.0,2000.0); 
  h_ew->GetYaxis()->SetRangeUser(0.95,1.2); 
  h_ew->SetLineWidth(2);
  h_ew->Draw("hist]["); 
  
  double x1 = 0.62; // 0.62 fixed
  double x2 = x1 + 0.30; 
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.12; 
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.06);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");  
  legend1->Draw();
 
  tex = new TLatex(281,0.9327758,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLine *l = new TLine(300.0,1.0,2000.0,1.0);
  l->SetLineWidth(1);
  l->SetLineColor(kGray+2);
  l->SetLineStyle(2);
  l->Draw("same");
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
 
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/EW_2.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/EW_2.pdf");
     
}
