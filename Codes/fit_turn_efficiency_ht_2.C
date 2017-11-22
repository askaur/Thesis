#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void fit_turn_efficiency_ht_2(){
  setTDRStyle();
  TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Data/Rootfile_Data_HT_2_150/Hadd_Ht_2_Turn.root");
  
  int  HLT[7] = {40,80,140,200,260,320};
  
  char hname_deno[256], hname_num[256];   
  char titlename[256];
  char histname[256]; 
  char hname[256]; 
  TH1D* h1_11_deno[50]; TH1D* h1_11_num[50];
  
  for (int nj = 2; nj < 3; nj++)
    {
      for ( int ntrig = 0; ntrig < 5; ntrig++)
	{

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
	     
	  gStyle->SetOptFit(0);
	  
	  int htrig = HLT[ntrig];
	  
          sprintf(hname_deno, "PFJetHt_2_%i_%i_v3_Trigger",nj,htrig);
	  sprintf(hname_num, "PFJetHt_2_%i_%i_v3_Trigger_Turn",nj,htrig);
	  
	  h1_11_deno[nj] = (TH1D*)(f1->FindObjectAny(hname_deno));
	  h1_11_num[nj] = (TH1D*)(f1->FindObjectAny(hname_num));

          TGraphAsymmErrors* gr = new TGraphAsymmErrors();
          gr->Divide(h1_11_num[nj],h1_11_deno[nj],"");
 
	  TString function_1 = "0.5 * [0] * (1 + TMath::Erf((x-[1])/(sqrt(2)*[2])))";
          TString function = function_1; 
  
          TF1 *fitfcn = new TF1("fitfcn",function.Data(),20,1200);
	  
	  if (ntrig ==0 )
	    {
	      fitfcn->SetParameter(0,0.996205);
	      fitfcn->SetParameter(1,81.815);
	      fitfcn->SetParameter(2,11.3107);
	    }
	  if (ntrig ==1)
	    {
	      fitfcn->SetParameter(0,0.99911);
	      fitfcn->SetParameter(1,133.63);
	      fitfcn->SetParameter(2,17.3287);
	    }
	  if (ntrig ==2)
	    {
	      fitfcn->SetParameter(0,0.999665);
	      fitfcn->SetParameter(1,192.749);
	      fitfcn->SetParameter(2,23.2194);
	    }	
	  if (ntrig ==3)
	    {
	      fitfcn->SetParameter(0,0.999714);
	      fitfcn->SetParameter(1,251.266);
	      fitfcn->SetParameter(2,29.3018);
	    }
	  if (ntrig ==4)
	    {
	      fitfcn->SetParameter(0,0.999972);
	      fitfcn->SetParameter(1,309.066);
	      fitfcn->SetParameter(2,36.7253);
	    }	 
	  gr->SetMarkerStyle(20);
	  gr->SetMarkerSize(0.75);	  
	  gr->SetMarkerColor(kBlack);
	  gr->SetLineColor(kBlack);
	  gr->SetLineWidth(2);
	  
	  fitfcn->SetLineColor(38);
	  fitfcn->SetLineWidth(2);
	  gr->Fit("fitfcn","MEX0RS");
	  gr->Draw("apz");
	  gr->GetYaxis()->SetRangeUser(0.0,1.19);
	  gr->GetXaxis()->SetRangeUser(40.0,1000.0);
	  if (ntrig ==4) gr->GetXaxis()->SetRangeUser(50.0,1000.0);
	  //gr->Draw("ap");
	  c->Update();
	  
	  gr->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
	  gr->GetXaxis()->SetTitleOffset(1.04);
	  gr->GetXaxis()->SetTitleSize(0.06);
	  gr->GetXaxis()->SetMoreLogLabels();
	  gr->GetXaxis()->SetNoExponent();
	  gr->GetXaxis()->SetLabelSize(0.05);
	  gr->GetXaxis()->SetLabelOffset(0.018);
	  gr->GetXaxis()->SetTickLength(0.03);
  
	  gr->GetYaxis()->SetTitle("Efficiency");
	  gr->GetYaxis()->SetTitleOffset(1.);
	  gr->GetYaxis()->SetTitleSize(0.06);
	  gr->GetYaxis()->SetLabelSize(0.05);
	  gr->GetYaxis()->SetTickLength(0.03);
	  gr->GetYaxis()->SetDecimals(2);
  
	  TLatex pt3;pt3.SetNDC();
          pt3.SetTextSize(0.05);
          pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
          	  
	  double par_new0 = fitfcn->GetParameter(0);
          double par_new1 = fitfcn->GetParameter(1);
          double par_new2 = fitfcn->GetParameter(2);

          cout<<"  fitfcn->SetParameter(0,"<<par_new0<<");"<<endl;
          cout<<"  fitfcn->SetParameter(1,"<<par_new1<<");"<<endl;
          cout<<"  fitfcn->SetParameter(2,"<<par_new2<<");"<<endl;
          
          if (ntrig == 0)
	    {
	      TLatex pt1;pt1.SetNDC();
	      pt1.SetTextSize(0.05);
	      pt1.DrawLatex(0.58,0.85,"#font[42]{#epsilon  > 99% at 120.0 GeV}");
	    }
	  if (ntrig == 1)
	    {
	      TLatex pt1;pt1.SetNDC();
	      pt1.SetTextSize(0.05);
	      pt1.DrawLatex(0.58,0.85,"#font[42]{#epsilon  > 99% at 187.5 GeV}");
	    }
	  if (ntrig == 2)
	    {
	      TLatex pt1;pt1.SetNDC();
	      pt1.SetTextSize(0.05);
	      pt1.DrawLatex(0.58,0.85,"#font[42]{#epsilon  > 99% at 262.5 GeV}");
	    }
	  if (ntrig == 3)
	    {
	      TLatex pt1;pt1.SetNDC();
	      pt1.SetTextSize(0.05);
	      pt1.DrawLatex(0.58,0.85,"#font[42]{#epsilon  > 99% at 345.0 GeV}");
	    }
	  if (ntrig == 4)
	    {
	      TLatex pt1;pt1.SetNDC();
	      pt1.SetTextSize(0.05);
	      pt1.DrawLatex(0.58,0.85,"#font[42]{#epsilon  > 99% at 405.0 GeV}");
	    }
          
	  //TLegend *legend1=new TLegend(0.7008032,0.8475936,0.8413655,0.9171123);
	  TLegend *legend1=new TLegend(0.2108434,0.828877,0.3514056,0.8983957);
	  legend1->SetTextFont(42);
	  legend1->SetTextSize(0.04);
	  legend1->SetLineStyle(1);
	  legend1->SetLineWidth(1);
	  legend1->SetFillColor(0);
	  legend1->SetBorderSize(0);
	  legend1->SetLineColor(1);

	  // 80
	  if (ntrig ==0 )
	    {
	      legend1->AddEntry( gr," HLT_PFJet80","pel");
	    }
	  // 140  
	  if (ntrig ==1 )
	    {
	      legend1->AddEntry( gr," HLT_PFJet140","pel");
	    }
	  // 200   
	  if (ntrig ==2 )
	    {
	      legend1->AddEntry( gr," HLT_PFJet200","pel");
	    }
	  // 260    
	  if (ntrig ==3 )
	    {
	      legend1->AddEntry( gr," HLT_PFJet260","pel");
	    }
	  // 320    
	  if (ntrig ==4 )
	    {
	      legend1->AddEntry( gr," HLT_PFJet320","pel");
	    }
	  legend1->Draw();
	  	  
	  sprintf(histname, "Fit_Turn_Efficiency_%i_%i_ht_2",HLT[ntrig+1],nj);
	  gr->SetName(histname);
	    
	  TString plotname =  gr->GetName();
          
	  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/"+plotname+".pdf");
	  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/"+plotname+".gif");
	}
    }
}
