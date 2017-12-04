#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void theory_unc_ratio_32(){
  setTDRStyle();
  
  TFile *fUnc = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Final_Plots_PAS/RootFiles/Rebin_NLO_PDFS_HT_2_150_ratio_32.root");
  TFile *fNP = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Final_NP_Corr_Ratio_32.root");
    
  TFile *file = new TFile("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/RootFiles/Theory_Unc_Ratio_32.root","RECREATE");
  //*****************************************************************
  
  TH1D *hScale_Up =  (TH1D*)fUnc->Get("Scale_Up");
  TH1D *hScale_Down =  (TH1D*)fUnc->Get("Scale_Down");
  TH1D *hPDF_Up =  (TH1D*)fUnc->Get("PDF_Up");
  TH1D *hPDF_Down =  (TH1D*)fUnc->Get("PDF_Down");
  
  TH1D *hScale_Up_frac =  (TH1D*)fUnc->Get("Scale_Up_frac");
  TH1D *hScale_Down_frac =  (TH1D*)fUnc->Get("Scale_Down_frac");
  TH1D *hPDF_Up_frac =  (TH1D*)fUnc->Get("PDF_Up_frac");
  TH1D *hPDF_Down_frac =  (TH1D*)fUnc->Get("PDF_Down_frac");  
  
  TH1D *hNP_Central =  (TH1D*)fNP->Get("Central Value");
  
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
 
  TH1D *hOne = new TH1D("One","One",arr_size-1,bins);
  TH1D *hTheory_Unc_Up = new TH1D("Theory_Unc_Up","Theory_Unc_Up",arr_size-1,bins);
  TH1D *hTheory_Unc_Down = new TH1D("Theory_Unc_Down","Theory_Unc_Down",arr_size-1,bins);  
  TH1D *hNP_Up =  new TH1D("NP_Up","NP_Up",arr_size-1,bins);
  TH1D *hNP_Down =  new TH1D("NP_Down","NP_Down",arr_size-1,bins);

  double nbinsx = hOne->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  for (int i = 1.; i <= nbinsx; i++)
    {
      hOne->SetBinContent(i,1.);
      hNP_Up->SetBinContent(i,1 + hNP_Central->GetBinError(i));
      hNP_Down->SetBinContent(i,1 - hNP_Central->GetBinError(i));
      double NP_Unc_up = hNP_Central->GetBinError(i);
      //cout<<bins[i-1]<<"-"<<bins[i]<<" Unc Up : "<<NP_Unc_up<<endl;
      double total_unc_up = sqrt(pow(hScale_Up->GetBinContent(i),2) + pow(hPDF_Up->GetBinContent(i),2) + pow(NP_Unc_up,2));
      
      double NP_Unc_down = hNP_Central->GetBinError(i);
      //cout<<bins[i-1]<<"-"<<bins[i]<<" Unc Down : "<<NP_Unc_down<<endl;
      double total_unc_down = sqrt(pow(hScale_Down->GetBinContent(i),2) + pow(hPDF_Down->GetBinContent(i),2) + pow(NP_Unc_down,2));
      
      hTheory_Unc_Up->SetBinContent(i,1 + total_unc_up);
      hTheory_Unc_Down->SetBinContent(i,1 - total_unc_down);  
    }
  
  hScale_Up->Add(hOne);
  hScale_Down->Add(hOne);
  hPDF_Up->Add(hOne);
  hPDF_Down->Add(hOne);

  //hScale_Up->Print("all");
  //hTheory_Unc_Up->Print("all");
  //hOne->Print("all");
  
  gStyle->SetLineStyleString(11,"40 15"); 
   
  hScale_Up_frac->SetLineColor(kRed); 
  hScale_Up_frac->SetLineWidth(2); 
  hScale_Up_frac->SetLineStyle(11);    
  
  hScale_Down_frac->SetLineColor(kRed); 
  hScale_Down_frac->SetLineWidth(2); 
  hScale_Down_frac->SetLineStyle(11);      
    
  hPDF_Up_frac->SetLineColor(kGreen-6);
  hPDF_Up_frac->SetLineWidth(2);  
  //hPDF_Up->SetLineStyle(2);      
    
  hPDF_Down_frac->SetLineColor(kGreen-6); 
  hPDF_Down_frac->SetLineWidth(2);   
  //hPDF_Down->SetLineStyle(2);         
  
  hNP_Up->SetLineColor(kBlue);
  hNP_Up->SetLineWidth(2);
  hNP_Up->SetLineStyle(8);             

  hNP_Down->SetLineColor(kBlue); 
  hNP_Down->SetLineWidth(2);
  hNP_Down->SetLineStyle(8);               
  
  hTheory_Unc_Up->SetLineColor(kBlack); 
  hTheory_Unc_Up->SetLineWidth(2);   
  hTheory_Unc_Up->SetLineStyle(2);           
  
  hTheory_Unc_Down->SetLineColor(kBlack); 
  hTheory_Unc_Down->SetLineWidth(2);  
  hTheory_Unc_Down->SetLineStyle(2);                   
        
  TCanvas *c = new TCanvas("c", "",500,400);
  c->Range(2.106318,-8.082687,3.353915,7.77261);
  c->SetBorderSize(2);
  c->SetLeftMargin(0.1560694);
  c->SetRightMargin(0.04238921);
  c->SetTopMargin(0.04872881);
  c->SetBottomMargin(0.1313559);
  
  hNP_Up->SetTitle("");
  hNP_Up->GetXaxis()->SetTitle("H_{T,2}/2 (GeV)");
  hNP_Up->GetXaxis()->SetTitleOffset(1.04);
  hNP_Up->GetYaxis()->SetNdivisions(508);
  hNP_Up->GetYaxis()->SetTitle("Fractional Uncertainty");
  hNP_Up->GetYaxis()->SetTitleOffset(1.15);
  hNP_Up->GetYaxis()->SetTitleSize(0.06);
  hNP_Up->GetYaxis()->SetLabelSize(0.05);
  hNP_Up->GetXaxis()->SetTitleSize(0.06);
  hNP_Up->GetXaxis()->SetMoreLogLabels();
  hNP_Up->GetXaxis()->SetNoExponent();
  hNP_Up->GetXaxis()->SetLabelSize(0.05);
  hNP_Up->GetXaxis()->SetLabelOffset(0.018); 
  
  hTheory_Unc_Up->GetXaxis()->SetRangeUser(300.0,1680.0);
  hTheory_Unc_Down->GetXaxis()->SetRangeUser(300.0,1680.0);
  hScale_Up_frac->GetXaxis()->SetRangeUser(300.0,1680.0);
  hScale_Down_frac->GetXaxis()->SetRangeUser(300.0,1680.0);
  hPDF_Up_frac->GetXaxis()->SetRangeUser(300.0,1680.0);
  hPDF_Down_frac->GetXaxis()->SetRangeUser(300.0,1680.0);
  hNP_Up->GetXaxis()->SetRangeUser(300.0,1680.0);
  hNP_Down->GetXaxis()->SetRangeUser(300.0,1680.0);  
  hNP_Up->GetYaxis()->SetRangeUser(0.6,1.6);
 
  hPDF_Up->SetStats(0);
  hNP_Up->SetStats(0);
  hNP_Up->Draw("hist][");
  hNP_Down->Draw("histsame][");
  hScale_Up_frac->Draw("histsame][");
  hScale_Down_frac->Draw("histsame][");
  hPDF_Up_frac->Draw("histsame][");
  hPDF_Down_frac->Draw("histsame][");
  hTheory_Unc_Up->Draw("histsame][");
  hTheory_Unc_Down->Draw("histsame][");
  
  //TLegend *legend1=new TLegend(0.5201149,0.6991525,0.7974138,0.8792373);
  TLegend *legend1=new TLegend(0.45,0.63,0.75,0.91);
  legend1->SetTextSize(0.04);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"CT10"," ");      
  legend1->AddEntry((TObject*)0,"R_{32}"," ");  
  legend1->AddEntry(hTheory_Unc_Up,"Total Uncertainty","l");
  legend1->AddEntry(hScale_Up_frac,"Scale Uncertainty","l");
  legend1->AddEntry(hPDF_Up_frac,"PDF Uncertainty","l");
  legend1->AddEntry(hNP_Up,"NP Uncertainty","l");
  legend1->Draw();
    
  TLatex pt1;pt1.SetNDC();
  TLatex pt2;pt2.SetNDC();
  TLatex pt3;pt3.SetNDC();
  TLatex pt4;pt4.SetNDC();
  TLatex pt5;pt5.SetNDC();
  TLatex pt6;pt6.SetNDC();
  pt1.SetTextSize(0.04);
  pt1.DrawLatex(0.16,0.96,"#font[61]{CMS}");
  pt2.SetTextSize(0.04);
  pt2.DrawLatex(0.23,0.96,"#font[52]{Simulation}");
  pt3.SetTextSize(0.04);
  pt3.DrawLatex(0.86,0.96,"#font[61]{8 TeV}");
  pt4.SetTextSize(0.04);
  pt4.DrawLatex(0.20,0.88,"#font[61]{anti-k_{t} R = 0.7}");
  pt5.SetTextSize(0.04);
  pt5.DrawLatex(0.20,0.82,"#font[61]{|y| < 2.5}");
 
  tex = new TLatex(281,0.528,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
 
  tex1 = new TLatex(1541,0.528,"1680");
  tex1->SetTextFont(42);
  tex1->SetTextSize(0.05);
  tex1->SetLineWidth(2);
  tex1->Draw();
 
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/Gifs/Theory_Unc_Ratio_32.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Script_Final_Plots_PAS/Pdfs/Theory_Unc_Ratio_32.pdf"); 
  file->Write();  
}  
