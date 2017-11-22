double makeCanvas(TCanvas *c, TH1D *hMC_Reco, TH1D *hMC_Reco_smear, TH1D *hMC_Reco_smear_less, TString plotname, bool twojet)
{
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
 
  hMC_Reco->SetMarkerSize(0.75);
  hMC_Reco->SetMarkerColor(38);
  hMC_Reco->SetLineColor(38);
  hMC_Reco->SetMarkerStyle(4);
  hMC_Reco->SetLineWidth(2);
  
  hMC_Reco_smear->SetMarkerSize(0.75);
  hMC_Reco_smear->SetMarkerColor(kRed-3);
  hMC_Reco_smear->SetLineColor(kRed-3);
  hMC_Reco_smear->SetLineStyle(1);
  hMC_Reco_smear->SetLineWidth(2);
  
  hMC_Reco_smear_less->SetMarkerSize(0.75);
  hMC_Reco_smear_less->SetMarkerColor(kMagenta);
  hMC_Reco_smear_less->SetLineColor(kMagenta);
  hMC_Reco_smear_less->SetLineStyle(1);
  hMC_Reco_smear_less->SetLineWidth(2);
    
  hMC_Reco_smear->GetYaxis()->SetRangeUser(0.9,1.2);
  hMC_Reco_smear->GetXaxis()->SetRangeUser(300.0,1680.0);//2000.0);
  hMC_Reco_smear_less->GetXaxis()->SetRangeUser(300.0,1680.0);//2000.0); 
  hMC_Reco->GetXaxis()->SetRangeUser(300.0,1680.0);//2000.0);
  hMC_Reco_smear->SetTitle("");
  hMC_Reco_smear->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hMC_Reco_smear->GetXaxis()->SetTitleOffset(1.04);
  hMC_Reco_smear->GetXaxis()->SetTitleSize(0.06);
  hMC_Reco_smear->GetXaxis()->SetMoreLogLabels();
  hMC_Reco_smear->GetXaxis()->SetNoExponent();
  hMC_Reco_smear->GetXaxis()->SetLabelSize(0.05);
  hMC_Reco_smear->GetXaxis()->SetLabelOffset(0.022);
  hMC_Reco_smear->GetXaxis()->SetTickLength(0.03);
  
  hMC_Reco_smear->GetYaxis()->SetTitle("Ratios");
  hMC_Reco_smear->GetYaxis()->SetTitleOffset(1.15);
  hMC_Reco_smear->GetYaxis()->SetTitleSize(0.06);
  hMC_Reco_smear->GetYaxis()->SetLabelSize(0.05);
  hMC_Reco_smear->GetYaxis()->SetTickLength(0.03);
  hMC_Reco_smear->GetYaxis()->SetLabelOffset(0.008);
  hMC_Reco_smear->GetYaxis()->SetDecimals(4);
  
  hMC_Reco_smear->SetStats(0);
  hMC_Reco_smear->Draw("hist][");
  hMC_Reco->Draw("histsame][");  
  hMC_Reco_smear_less->Draw("histsame][");      
  
  double x1 = 0.22;//18; // 0.62 fixed
  double x2 = x1 + 0.30;
  double y2 = 0.91; // fixed
  double y1 = y2 - 0.35;
   
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  if (twojet) legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  else legend1->AddEntry((TObject*)0,"n_{j} #geq 3"," ");
  legend1->AddEntry(hMC_Reco,"Reco/Gen","l");
  legend1->AddEntry(hMC_Reco_smear,"Smeared Gen/Gen (JER)","l");
  legend1->AddEntry(hMC_Reco_smear_less,"Smeared Gen/Gen","l");
  legend1->AddEntry((TObject*)0,"(30% reduced JER)","");
  legend1->Draw();
  
  TLegend *legend1=new TLegend(x1-0.27,y1-0.60,x2,y2-0.55);
  legend1->SetTextFont(42);
  legend1->SetTextSize(0.05);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry(hMC_Reco,"Reco/Gen","l");
  legend1->AddEntry(hMC_Reco_smear,"Smeared Gen/Gen (JER)","l");
  legend1->AddEntry(hMC_Reco_smear_less,"Smeared Gen/Gen (30% reduced JER)","l");
  //legend1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
     
  tex = new TLatex(281,0.8772345,"300");//281,0.9503679,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw(); 
  
  tex = new TLatex(1541,0.8772345,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();  

  c->Draw();
  c->Update();
  
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Ratio_Reco_" + plotname + "_crystal.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Ratio_Reco_" + plotname + "_crystal.pdf");
}  
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"
void ratio_Reco_crystal(){
  setTDRStyle(); 
   
  TFile *f_gen = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/Madgraph/New_RootFiles/MC_total_jethadd_matching.root");
   
  //*****************************************************************
  char figname[256];
  TH1D *hMC_Reco_2  = (TH1D*)f_gen->Get("jet/PFMCJetHt_2_2"); 
  TH1D *hMC_Reco_smear_2  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_smeared_2");
  TH1D *hMC_Reco_smear_less_2  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_smeared_2_70");
  TH1D *hMC_Gen_2  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_2");
  
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
  
  int nsize_bins = sizeof(bins) / sizeof(bins[0]);
  cout<<" No of bins : "<<nsize_bins-1<<endl;
  
  TH1D *hMC_Reco_Now_3 = new TH1D ("hMC_Reco_Now_3"," hMC_Reco_Now_3", 56, bins);
  
  TH1D *hMC_Reco_3  = (TH1D*)f_gen->Get("jet/PFMCJetHt_2_3"); 
  TH1D *hMC_Reco_smear_3  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_smeared_3");
  TH1D *hMC_Reco_smear_less_3  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_smeared_3_70");
  TH1D *hMC_Gen_3  = (TH1D*)f_gen->Get("jet/GenMCJetHt_2_3");
  
  
  hMC_Reco_2->Divide(hMC_Gen_2);  
  hMC_Reco_smear_2->Divide(hMC_Gen_2);  
  hMC_Reco_smear_less_2->Divide(hMC_Gen_2); 
  
  
  hMC_Reco_3->Divide(hMC_Gen_3);  
  hMC_Reco_smear_3->Divide(hMC_Gen_3);  
  hMC_Reco_smear_less_3->Divide(hMC_Gen_3); 
    
  for (int i = 1; i <=nsize_bins ; i++)
    {
      if (i >=24 && i < 26) 
      { 
       hMC_Reco_Now_3->SetBinContent(24,0.999);
       hMC_Reco_Now_3->SetBinContent(25,1.001);
      }
      else
      {
        double bincontent = hMC_Reco_3->GetBinContent(i);
        hMC_Reco_Now_3->SetBinContent(i,bincontent);
      }
    }  
    
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"2");
  makeCanvas(c,hMC_Reco_2,hMC_Reco_smear_2,hMC_Reco_smear_less_2,figname,1);
  
  TCanvas *c = new TCanvas("c", "",500,400);
  sprintf(figname,"3");
  makeCanvas(c,hMC_Reco_Now_3,hMC_Reco_smear_3,hMC_Reco_smear_less_3,figname,0);  
}
