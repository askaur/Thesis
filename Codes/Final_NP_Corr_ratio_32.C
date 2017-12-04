#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void Final_NP_Corr_ratio_32(){
  setTDRStyle();
   
  TFile *fNP = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/NP_Corr_HT_2_150.root");
  TFile *fNP2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Final_NP_Corr_2.root");
  TFile *fNP3 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Final_NP_Corr_3.root");  
   
  TH1D *hNP2 =  (TH1D*)fNP2->Get("Central Value");
  TH1D *hNP3 =  (TH1D*)fNP3->Get("Central Value");
  
  TH1D *hNP_pythia2 =  (TH1D*)fNP->Get("NP_Corr_HT_2_150_2");
  TH1D *hNP_herwig2 =  (TH1D*)fNP->Get("GenMCJetHt_2_2");
  TH1D *hNP_powheg2 =  (TH1D*)fNP->Get("NP_Corr_HT_2_150_2_powheg_S1");
  
  TH1D *hNP_pythia3 =  (TH1D*)fNP->Get("NP_Corr_HT_2_150_3");
  TH1D *hNP_herwig3 =  (TH1D*)fNP->Get("GenMCJetHt_2_3");
  TH1D *hNP_powheg3 =  (TH1D*)fNP->Get("NP_Corr_HT_2_150_3_powheg_S1");
  
  const double new_bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};

  TH1D *h_pythia = new TH1D("Final NP Pythia","Final NP Pythia",56.0,new_bins);
  TH1D *h_herwig = new TH1D("Final NP Herwig","Final NP Herwig",56.0,new_bins);  
  TH1D *h_powheg = new TH1D("Final NP Powheg","Final NP Powheg",56.0,new_bins);   
  TH1D *h_center = new TH1D("Central Value","Central Value",56.0,new_bins);  
  h_center->Sumw2();
  
  hNP3->Divide(hNP2);
  
  double nbinsx = hNP_pythia3->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  TF1 *f_pythia = new TF1("f_pythia","([0]*pow(x,[1])+[2])", 200.,2000.);
  TF1 *f_herwig = new TF1("f_herwig","([0]*pow(x,[1])+[2])", 200.,2000.);
  TF1 *f_powheg = new TF1("f_powheg","([0]*pow(x,[1])+[2])", 200.,2000.);
  
  f_pythia->SetParameter(0,14.1883);
  f_pythia->SetParameter(1,-1.07781);
  f_pythia->SetParameter(2,0.994407);
  
  f_herwig->SetParameter(0,49334.1);
  f_herwig->SetParameter(1,-2.83247);
  f_herwig->SetParameter(2,1.00262);

  f_powheg->SetParameter(0,8563.56);
  f_powheg->SetParameter(1,-2.34943);
  f_powheg->SetParameter(2,1.00581);
  
  /* for (int i = 1.; i <= nbinsx; i++)
     {
     
     double xt = h_center->GetBinCenter(i);
     //cout<<" Bin : "<<i<<" Bin Center : "<<xt<<endl;
     double y_pythia = f_pythia(xt);
     double y_herwig = f_herwig(xt);
      
     h_pythia->SetBinContent(i,y_pythia);
     h_herwig->SetBinContent(i,y_herwig); 
     double y_center = (y_pythia + y_herwig)/2;
     h_center->SetBinContent(i,y_center);
     h_center->SetBinError(i,(y_pythia-y_herwig)/2);                      
     }
  */ 
 
  Double_t x1[100], y1[100], y2[100], y3[100]; int n = 56;
  for (int i = 0.; i < nbinsx; i++)
    {
    double max = -1.;
      double min = 1000.0;
      x1[i] = h_center->GetBinCenter(i+1);
      //cout<<" Bin : "<<i<<" Bin Center : "<<x1[i]<<endl;
      y1[i] = f_pythia(x1[i]); 
      y2[i] = f_herwig(x1[i]);  
      y3[i] = f_powheg(x1[i]);         
      h_pythia->SetBinContent(i+1,y1[i]);
      h_herwig->SetBinContent(i+1,y2[i]);   
      h_powheg->SetBinContent(i+1,y3[i]);               
    
      if (y1[i] > max ) max = y1[i];
      if (y2[i] > max ) max = y2[i]; 
      if (y3[i] > max ) max = y3[i]; 
      
      if (y1[i] < min ) min = y1[i];
      if (y2[i] < min ) min = y2[i]; 
      if (y3[i] < min ) min = y3[i]; 
      
      if (min == max) cout<<" Something is going fishy >><<"<<endl;
       
      if (i > 16) 
	{
	  //cout<<" Bin : "<<h_center->GetBinLowEdge(i+1)<<" - "<<h_center->GetBinLowEdge(i+2)<<" Pythia : "<<y1[i]<<" Herwig : "<<y2[i]<<" Powheg : "<<y3[i]<<endl; 
	  //cout<<" Center value : "<<y_center<<endl; 
	  /*if (max == y1[i]) cout<<" Max Pythia : "<<max<<endl;
	  if (max == y2[i]) cout<<" Max Herwig : "<<max<<endl;
	  if (max == y3[i]) cout<<" Max Powheg : "<<max<<endl;   
	  if (min == y1[i]) cout<<" Min Pythia : "<<min<<endl;
	  if (min == y2[i]) cout<<" Min Herwig : "<<min<<endl;
	  if (min == y3[i]) cout<<" Min Powheg : "<<min<<endl;    
          */    
	  //cout<<""<<endl;    
	}
      double y_center = (max + min)/2;
      h_center->SetBinContent(i+1,y_center);
      h_center->SetBinError(i+1,(max-min)/2);           
    }
  //h_center->Print("all");  
  gr1 = new TGraph(n,x1,y1);

  gr2 = new TGraph(n,x1,y2);
  
  gr3 = new TGraph(n,x1,y3);
  
  gr1->SetLineColor(38);
  gr2->SetLineColor(kRed-3);
  gr3->SetLineColor(kGreen+2);  
  
  gr1->SetLineWidth(2);
  gr2->SetLineWidth(2);
  gr3->SetLineWidth(2);  
     
  h_pythia->SetLineWidth(2);
  h_pythia->SetLineColor(38);
  h_pythia->SetMarkerColor(38);
  
  h_herwig->SetLineWidth(2);
  h_herwig->SetLineColor(kRed-3);
  h_herwig->SetMarkerColor(kRed-3);
  
  h_powheg->SetLineWidth(2);
  h_powheg->SetLineColor(kGreen+2);
  h_powheg->SetMarkerColor(kGreen+2);  
  
  h_center->SetLineColor(kBlack);
  h_center->SetMarkerColor(kBlack);
  h_center->SetMarkerStyle(20);
  h_center->SetMarkerSize(0.5);
  h_center->SetLineWidth(2);
           
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
  
  h_pythia->SetStats(0);  
  
  h_center->GetXaxis()->SetRangeUser(300.,1680.);    
  h_center->GetYaxis()->SetRangeUser(0.98,1.1); 
  
  h_center->SetTitle("");
  h_center->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  h_center->GetXaxis()->SetTitleOffset(1.04);
  h_center->GetXaxis()->SetTitleSize(0.06);
  h_center->GetXaxis()->SetMoreLogLabels();
  h_center->GetXaxis()->SetNoExponent();
  h_center->GetXaxis()->SetLabelSize(0.05);
  h_center->GetXaxis()->SetLabelOffset(0.018);
  h_center->GetXaxis()->SetTickLength(0.03); 
  
  h_center->GetYaxis()->SetTitle("NP Correction Factor");
  h_center->GetYaxis()->SetTitleOffset(1.18);
  h_center->GetYaxis()->SetTitleSize(0.06);
  h_center->GetYaxis()->SetLabelSize(0.05);
  h_center->GetYaxis()->SetTickLength(0.03);
  h_center->GetYaxis()->SetDecimals(4);
  h_center->GetYaxis()->SetNdivisions(507);  
 
  h_center->Draw("e");   
  gr1->Draw("Lsame");
  gr2->Draw("Lsame");
  gr3->Draw("Lsame");
  
  double xl_1 = 0.42; // 0.62 fixed
  double xl_2 = xl_1 + 0.30; 
  double yl_2 = 0.91; // fixed
  double yl_1 = yl_2 - 0.32;  
     
  TLegend *legend1=new TLegend(xl_1,yl_1,xl_2,yl_2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  legend1->AddEntry(h_pythia,"Pythia6 Z2*","l");
  legend1->AddEntry(h_herwig,"Herwig++","l");  
  legend1->AddEntry(h_powheg,"Powheg+Pythia8 CUETS1","l");    
  legend1->AddEntry(h_center,"NP correction","elp");    
  legend1->Draw();
  	  	  	  	  
  TLine *l = new TLine(300.0,1.0,1680.0,1.0);
  l->SetLineWidth(1);
  l->SetLineColor(kGray+2);
  l->SetLineStyle(2);
  l->Draw("same");
  
  tex = new TLatex(281,0.9389632,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(1541,0.9389632,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
  
  c->Draw();
  c->Update();
        
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Final_NP_Corr_Ratio_32.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Final_NP_Corr_Ratio_32.pdf");
}
