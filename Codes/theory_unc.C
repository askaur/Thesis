double makeCanvas(TCanvas *c, TH1D *hOne, TH1D *hScale_Up, TH1D *hScale_Down, TH1D *hPDF_Up, TH1D *hPDF_Down, TH1D *hNP_Up, TH1D *hNP_Down, TH1D *hTheory_Unc_Up, TH1D *hTheory_Unc_Down, TString plotname, bool twojet, bool ratio)
{
  hScale_Up->Add(hOne);
  hScale_Down->Add(hOne);
  hPDF_Up->Add(hOne);
  hPDF_Down->Add(hOne);
   
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
  
  hScale_Up->SetLineColor(kRed-3); 
  hScale_Up->SetLineWidth(2); 
  hScale_Up->SetLineStyle(9);    
  
  hScale_Down->SetLineColor(kRed-3); 
  hScale_Down->SetLineWidth(2); 
  hScale_Down->SetLineStyle(9);      
    
  hPDF_Up->SetLineColor(kGreen-6);
  hPDF_Up->SetLineWidth(2);  
    
  hPDF_Down->SetLineColor(kGreen-6); 
  hPDF_Down->SetLineWidth(2);   
  
  hNP_Up->SetLineColor(38);
  hNP_Up->SetLineWidth(2);
  hNP_Up->SetLineStyle(8);             

  hNP_Down->SetLineColor(38); 
  hNP_Down->SetLineWidth(2);
  hNP_Down->SetLineStyle(8);               
  
  hTheory_Unc_Up->SetLineColor(kBlack); 
  hTheory_Unc_Up->SetLineWidth(2);   
  hTheory_Unc_Up->SetLineStyle(2);           
  
  hTheory_Unc_Down->SetLineColor(kBlack); 
  hTheory_Unc_Down->SetLineWidth(2);  
  hTheory_Unc_Down->SetLineStyle(2);
  
  if (twojet) double max = 2000.0;
  else double max = 1680.0;
  
  hNP_Up->SetTitle("");
  hNP_Up->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hNP_Up->GetXaxis()->SetTitleOffset(1.04);
  hNP_Up->GetXaxis()->SetTitleSize(0.06);
  hNP_Up->GetXaxis()->SetMoreLogLabels();
  hNP_Up->GetXaxis()->SetNoExponent();
  hNP_Up->GetXaxis()->SetLabelSize(0.05);
  hNP_Up->GetXaxis()->SetLabelOffset(0.018);
  hNP_Up->GetXaxis()->SetTickLength(0.03);
  
  hNP_Up->GetYaxis()->SetTitle("Fractional Uncertainty");
  hNP_Up->GetYaxis()->SetTitleOffset(1.00);
  hNP_Up->GetYaxis()->SetTitleSize(0.06);
  hNP_Up->GetYaxis()->SetLabelSize(0.05);
  hNP_Up->GetYaxis()->SetTickLength(0.03);
  hNP_Up->GetYaxis()->SetLabelOffset(0.008);
  hNP_Up->GetYaxis()->SetNdivisions(507);
  hNP_Up->GetYaxis()->SetDecimals(4);
    
  hTheory_Unc_Up->GetXaxis()->SetRangeUser(300.0,max);
  hTheory_Unc_Down->GetXaxis()->SetRangeUser(300.0,max); 
  hScale_Up->GetXaxis()->SetRangeUser(300.0,max);
  hScale_Down->GetXaxis()->SetRangeUser(300.0,max);
  hPDF_Up->GetXaxis()->SetRangeUser(300.0,max);
  hPDF_Down->GetXaxis()->SetRangeUser(300.0,max);
  hNP_Up->GetXaxis()->SetRangeUser(300.0,max);
  hNP_Down->GetXaxis()->SetRangeUser(300.0,max);  
  hNP_Up->GetYaxis()->SetRangeUser(0.6,1.6);
 
  hPDF_Up->SetStats(0);
  hNP_Up->SetStats(0);
  
  hNP_Up->Draw("hist][");
  hNP_Down->Draw("histsame][");
  hScale_Up->Draw("histsame][");
  hScale_Down->Draw("histsame][");
  hPDF_Up->Draw("histsame][");
  hPDF_Down->Draw("histsame][");
  hTheory_Unc_Up->Draw("histsame][");
  hTheory_Unc_Down->Draw("histsame][");
    
  double x1 = 0.25; // 0.62 fixed
  double x2 = x1 + 0.41;
  double y2 = 0.9; // fixed
  double y1 = y2 - 0.3;
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.68,0.86,"#font[42]{CT10-NLO}");
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  //legend1->AddEntry((TObject*)0,"CT10-NLO"," ");  
  if (twojet) legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  else if (ratio) legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  else legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hTheory_Unc_Up,"Total","l"); // Uncertainty","l");
  legend1->AddEntry(hScale_Up,"Scale","l"); // Uncertainty","l");
  legend1->AddEntry(hPDF_Up,"PDF","l"); // Uncertainty","l");
  legend1->AddEntry(hNP_Up,"NP","l"); // Uncertainty","l");
  legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
    
  tex = new TLatex(281,0.5264214,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  if (!twojet)
  {
   tex1 = new TLatex(1541,0.5264214,"1680");
   tex1->SetTextFont(42);
   tex1->SetTextSize(0.05);
   tex1->SetLineWidth(2);
   tex1->Draw();
  }
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Theory_Unc_" + plotname + ".gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Theory_Unc_" + plotname + ".pdf"); 

}
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void theory_unc(){
  setTDRStyle();
  
  char figname[256];
  
  TFile *fUnc_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Final_Plots_PAS/RootFiles/Rebin_NLO_PDFS_HT_2_150_2.root");
  TFile *fNP_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/Final_NP_Corr_2.root");
  TFile *fUnc_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Final_Plots_PAS/RootFiles/Rebin_NLO_PDFS_HT_2_150_3.root");
  TFile *fNP_3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/Final_NP_Corr_3.root");
  TFile *fUnc_ratio = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Final_Plots_PAS/RootFiles/Rebin_NLO_PDFS_HT_2_150_ratio_32.root");
  TFile *fNP_ratio = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Final_NP_Corr_Ratio_32.root");
    
  //*****************************************************************
  
  TH1D *hScale_Up_2   =  (TH1D*)fUnc_2->Get("Scale_Up_2");
  TH1D *hScale_Down_2 =  (TH1D*)fUnc_2->Get("Scale_Down_2");
  TH1D *hPDF_Up_2     =  (TH1D*)fUnc_2->Get("PDF_Up_2");
  TH1D *hPDF_Down_2   =  (TH1D*)fUnc_2->Get("PDF_Down_2");
  TH1D *hNP_Central_2 =  (TH1D*)fNP_2->Get("Central Value");
  
  TH1D *hScale_Up_3   =  (TH1D*)fUnc_3->Get("Scale_Up_3");
  TH1D *hScale_Down_3 =  (TH1D*)fUnc_3->Get("Scale_Down_3");
  TH1D *hPDF_Up_3     =  (TH1D*)fUnc_3->Get("PDF_Up_3");
  TH1D *hPDF_Down_3   =  (TH1D*)fUnc_3->Get("PDF_Down_3");
  TH1D *hNP_Central_3 =  (TH1D*)fNP_3->Get("Central Value");
  
  TH1D *hScale_Up_ratio   =  (TH1D*)fUnc_ratio->Get("Scale_Up");
  TH1D *hScale_Down_ratio =  (TH1D*)fUnc_ratio->Get("Scale_Down");
  TH1D *hPDF_Up_ratio     =  (TH1D*)fUnc_ratio->Get("PDF_Up");
  TH1D *hPDF_Down_ratio   =  (TH1D*)fUnc_ratio->Get("PDF_Down");
  TH1D *hScale_Up_frac_ratio   =  (TH1D*)fUnc_ratio->Get("Scale_Up_frac");
  TH1D *hScale_Down_frac_ratio =  (TH1D*)fUnc_ratio->Get("Scale_Down_frac");
  TH1D *hPDF_Up_frac_ratio     =  (TH1D*)fUnc_ratio->Get("PDF_Up_frac");
  TH1D *hPDF_Down_frac_ratio   =  (TH1D*)fUnc_ratio->Get("PDF_Down_frac");  
  TH1D *hNP_Central_ratio =  (TH1D*)fNP_ratio->Get("Central Value");
  
  
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
 
  TH1D *hOne = new TH1D("One","One",arr_size-1,bins);
  TH1D *hTheory_Unc_Up_2 = new TH1D("Theory_Unc_Up_2","Theory_Unc_Up_2",arr_size-1,bins);
  TH1D *hTheory_Unc_Down_2 = new TH1D("Theory_Unc_Down_2","Theory_Unc_Down_2",arr_size-1,bins);  
  TH1D *hNP_Up_2 =  new TH1D("NP_Up_2","NP_Up_2",arr_size-1,bins);
  TH1D *hNP_Down_2 =  new TH1D("NP_Down_2","NP_Down_2",arr_size-1,bins);
  
  TH1D *hTheory_Unc_Up_3 = new TH1D("Theory_Unc_Up_3","Theory_Unc_Up_3",arr_size-1,bins);
  TH1D *hTheory_Unc_Down_3 = new TH1D("Theory_Unc_Down_3","Theory_Unc_Down_3",arr_size-1,bins);  
  TH1D *hNP_Up_3 =  new TH1D("NP_Up_3","NP_Up_3",arr_size-1,bins);
  TH1D *hNP_Down_3 =  new TH1D("NP_Down_3","NP_Down_3",arr_size-1,bins);
  
  TH1D *hTheory_Unc_Up_ratio = new TH1D("Theory_Unc_Up_ratio","Theory_Unc_Up_ratio",arr_size-1,bins);
  TH1D *hTheory_Unc_Down_ratio = new TH1D("Theory_Unc_Down_ratio","Theory_Unc_Down_ratio",arr_size-1,bins);  
  TH1D *hNP_Up_ratio =  new TH1D("NP_Up","NP_Up_ratio",arr_size-1,bins);
  TH1D *hNP_Down_ratio =  new TH1D("NP_Down","NP_Down_ratio",arr_size-1,bins);
  
  double nbinsx = hOne->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  for (int i = 1.; i <= nbinsx; i++)
    {
      hOne->SetBinContent(i,1.);
      // 2-jet
      hNP_Up_2->SetBinContent(i,1 + hNP_Central_2->GetBinError(i));
      hNP_Down_2->SetBinContent(i,1 - hNP_Central_2->GetBinError(i));
      double NP_Unc_up_2 = hNP_Central_2->GetBinError(i);
      double total_unc_up_2 = sqrt(pow(hScale_Up_2->GetBinContent(i),2) + pow(hPDF_Up_2->GetBinContent(i),2) + pow(NP_Unc_up_2,2));
      
      double NP_Unc_down_2 = hNP_Central_2->GetBinError(i);
      double total_unc_down_2 = sqrt(pow(hScale_Down_2->GetBinContent(i),2) + pow(hPDF_Down_2->GetBinContent(i),2) + pow(NP_Unc_down_2,2));
      
      hTheory_Unc_Up_2->SetBinContent(i,1 + total_unc_up_2);
      hTheory_Unc_Down_2->SetBinContent(i,1 - total_unc_down_2); 
       
      //3-jet 
      hNP_Up_3->SetBinContent(i,1 + hNP_Central_3->GetBinError(i));
      hNP_Down_3->SetBinContent(i,1 - hNP_Central_3->GetBinError(i));
      double NP_Unc_up_3 = hNP_Central_3->GetBinError(i);
      double total_unc_up_3 = sqrt(pow(hScale_Up_3->GetBinContent(i),2) + pow(hPDF_Up_3->GetBinContent(i),2) + pow(NP_Unc_up_3,2));
      
      double NP_Unc_down_3 = hNP_Central_3->GetBinError(i);
      double total_unc_down_3 = sqrt(pow(hScale_Down_3->GetBinContent(i),2) + pow(hPDF_Down_3->GetBinContent(i),2) + pow(NP_Unc_down_3,2));
      
      hTheory_Unc_Up_3->SetBinContent(i,1 + total_unc_up_3);
      hTheory_Unc_Down_3->SetBinContent(i,1 - total_unc_down_3);
      
      //Ratio
      hNP_Up_ratio->SetBinContent(i,1 + hNP_Central_ratio->GetBinError(i));
      hNP_Down_ratio->SetBinContent(i,1 - hNP_Central_ratio->GetBinError(i));
      double NP_Unc_up_ratio = hNP_Central_ratio->GetBinError(i);
      double total_unc_up_ratio = sqrt(pow(hScale_Up_ratio->GetBinContent(i),2) + pow(hPDF_Up_ratio->GetBinContent(i),2) + pow(NP_Unc_up_ratio,2));
      
      double NP_Unc_down_ratio = hNP_Central_ratio->GetBinError(i);
      double total_unc_down_ratio = sqrt(pow(hScale_Down_ratio->GetBinContent(i),2) + pow(hPDF_Down_ratio->GetBinContent(i),2) + pow(NP_Unc_down_ratio,2));
      
      hTheory_Unc_Up_ratio->SetBinContent(i,1 + total_unc_up_ratio);
      hTheory_Unc_Down_ratio->SetBinContent(i,1 - total_unc_down_ratio); 
      
      if (i > 21 && i < 55)
	{
	 //cout<<hTheory_Unc_Up_2->GetBinLowEdge(i)<<" - "<<hTheory_Unc_Up_2->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<" $^{+"<<hScale_Up_2->GetBinContent(i)*100<<"}_{"<<hScale_Down_2->GetBinContent(i)*100<<"}$"<<" & $^{+"<<hPDF_Up_2->GetBinContent(i)*100<<"}_{"<<hPDF_Down_2->GetBinContent(i)*100<<"}$ & "<<NP_Unc_up_2*100<<" & $^{+"<<total_unc_up_2*100<<"}_{-"<<total_unc_down_2*100<<"}$ \\\Rbtrrnm \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
	}
	
      if (i > 21 && i < 51)
	{
	 //cout<<hTheory_Unc_Up_3->GetBinLowEdge(i)<<" - "<<hTheory_Unc_Up_3->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<" $^{+"<<hScale_Up_3->GetBinContent(i)*100<<"}_{"<<hScale_Down_3->GetBinContent(i)*100<<"}$"<<" & $^{+"<<hPDF_Up_3->GetBinContent(i)*100<<"}_{"<<hPDF_Down_3->GetBinContent(i)*100<<"}$ & "<<NP_Unc_up_3*100<<" & $^{+"<<total_unc_up_3*100<<"}_{-"<<total_unc_down_3*100<<"}$ \\\Rbtrrnm \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
	}
	
	if (i > 21 && i < 51)
	{
	 cout<<hTheory_Unc_Up_ratio->GetBinLowEdge(i)<<" - "<<hTheory_Unc_Up_ratio->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<" $^{+"<<hScale_Up_ratio->GetBinContent(i)*100<<"}_{"<<hScale_Down_ratio->GetBinContent(i)*100<<"}$"<<" & $^{+"<<hPDF_Up_ratio->GetBinContent(i)*100<<"}_{"<<hPDF_Down_ratio->GetBinContent(i)*100<<"}$ & "<<NP_Unc_up_ratio*100<<" & $^{+"<<total_unc_up_ratio*100<<"}_{-"<<total_unc_down_ratio*100<<"}$ \\\Rbtrrnm \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
	}
    }
  
  /*TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"2");
  makeCanvas(c, hOne, hScale_Up_2, hScale_Down_2, hPDF_Up_2, hPDF_Down_2, hNP_Up_2, hNP_Down_2, hTheory_Unc_Up_2, hTheory_Unc_Down_2, figname, 1, 0);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"3");
  makeCanvas(c, hOne, hScale_Up_3, hScale_Down_3, hPDF_Up_3, hPDF_Down_3, hNP_Up_3, hNP_Down_3, hTheory_Unc_Up_3, hTheory_Unc_Down_3, figname, 0, 0);
  */
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"Ratio_32");
  makeCanvas(c, hOne, hScale_Up_ratio, hScale_Down_ratio, hPDF_Up_ratio, hPDF_Down_ratio, hNP_Up_ratio, hNP_Down_ratio, hTheory_Unc_Up_ratio, hTheory_Unc_Down_ratio, figname, 0, 1);
 
}  
