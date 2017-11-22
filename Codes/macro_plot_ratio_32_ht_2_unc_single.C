double makeCanvas (TCanvas *c1, TH1D *h_up_1, TString plotname)
{
  c1->Range(2.318121,0.4327759,3.346459,1.683612);
  c1->SetFillColor(0);
  c1->SetBorderMode(0);
  c1->SetBorderSize(2);
  c1->SetTickx(1);
  c1->SetTicky(1);
  c1->SetLeftMargin(0.1546185);
  c1->SetRightMargin(0.04417671);
  c1->SetTopMargin(0.06684492);
  c1->SetBottomMargin(0.1336898);
  c1->SetLogx();
      
  h_up_1->SetStats(0);
           
  h_up_1->GetXaxis()->SetRangeUser(300.,1680.);
  h_up_1->GetYaxis()->SetRangeUser(0.93,1.08);
   
  h_up_1->SetTitle("");
  h_up_1->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
      
  h_up_1->GetXaxis()->SetTitleOffset(1.04);
  h_up_1->GetXaxis()->SetTitleSize(0.06);
  h_up_1->GetXaxis()->SetMoreLogLabels();
  h_up_1->GetXaxis()->SetNoExponent();
  h_up_1->GetXaxis()->SetLabelSize(0.05);
  h_up_1->GetXaxis()->SetLabelOffset(0.018);
  h_up_1->GetXaxis()->SetTickLength(0.03);  
  
  h_up_1->GetYaxis()->SetTitle("Fractional Uncertainty");
  h_up_1->GetYaxis()->SetTitleOffset(1.00);
  h_up_1->GetYaxis()->SetTitleSize(0.06);
  h_up_1->GetYaxis()->SetLabelSize(0.05);
  h_up_1->GetYaxis()->SetTickLength(0.03); 
  h_up_1->GetYaxis()->SetNdivisions(509);
  h_up_1->GetYaxis()->SetDecimals(4);
      
  TLine *lp = new TLine(300.0,1,1680.0,1);
  lp->SetLineStyle(2);
  lp->SetLineWidth(1);
  lp->SetLineColor(kGray+2);
  lp->Draw("same");
      
  tex = new TLatex(282,0.9189632,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex = new TLatex(1541,0.9189632,"1680");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.87,0.95,"#font[61]{8 TeV}");
 
  c1->Update();
  c1->Draw();
  
  c1->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Single/MC_Macro_Plot_Ratio_32_HT_2_Unc_" + plotname + ".gif");
  c1->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Single/MC_Macro_Plot_Ratio_32_HT_2_Unc_" + plotname + ".pdf");
}
  
#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void macro_plot_ratio_32_ht_2_unc_single(){
  setTDRStyle();
  
  TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/JES/Ratio/RootFiles/JES_unc_cal_all_HT_2_150_sources_single.root");
 
  //*****************************************************************
  bool unc_54 = false;
  bool plot1 = false;
  bool plot2 = false;
  bool plot3 = false;
  bool plot4 = false;
  bool plot5 = false;
  bool plot6 = true;
    
  bool all = true;
  
  if (all)
   { 
    plot1 = true;
    plot2 = true;
    plot3 = true;
    plot4 = true;
    plot5 = true;
    plot6 = true;
   }
           
  if (unc_54)
    {
      char *unc[54] = {"AbsoluteStat", "AbsoluteScale", "AbsoluteFlavMap", "AbsoluteMPFBias", "Fragmentation", "SinglePionECAL", "SinglePionHCAL", "FlavorQCD", "TimeEta", "TimePt", "RelativeJEREC1", "RelativeJEREC2", "RelativeJERHF", "RelativePtBB", "RelativePtEC1", "RelativePtEC2", "RelativePtHF", "RelativeFSR", "RelativeStatFSR", "RelativeStatEC2", "RelativeStatHF", "PileUpDataMC", "PileUpPtRef", "PileUpPtBB", "PileUpPtEC1", "PileUpPtEC2", "PileUpPtHF", "PileUpMuZero", "PileUpEnvelope", "SubTotalPileUp", "SubTotalRelative", "SubTotalPt", "SubTotalScale", "SubTotalAbsolute", "SubTotalMC", "Total", "TotalNoFlavor", "TotalNoTime", "TotalNoFlavorNoTime", "FlavorZJet", "FlavorPhotonJet", "FlavorPureGluon", "FlavorPureQuark", "FlavorPureCharm", "FlavorPureBottom", "TimeRunA", "TimeRunB", "TimeRunC", "TimeRunD", "CorrelationGroupMPFInSitu", "CorrelationGroupIntercalibration", "CorrelationGroupbJES", "CorrelationGroupFlavor", "CorrelationGroupUncorrelated"};
      int nsize = sizeof(unc) / sizeof(unc[0]);
      cout<<" No of sources : "<<nsize<<endl;
    }
  else 
    {
      char *unc[26] = {"AbsoluteScale","AbsoluteStat","AbsoluteMPFBias","Fragmentation","SinglePionECAL","SinglePionHCAL","FlavorQCD",
		       "TimeEta","TimePt","RelativeJEREC1","RelativeJEREC2","RelativeJERHF","RelativePtBB","RelativePtEC1","RelativePtEC2",
		       "RelativePtHF","RelativeFSR","RelativeStatEC2", "RelativeStatHF","RelativeStatFSR","PileUpDataMC","PileUpPtRef","PileUpPtBB", "PileUpPtEC1","PileUpPtEC2","PileUpPtHF"};
  
      int nsize = sizeof(unc) / sizeof(unc[0]);
      cout<<" No of sources : "<<nsize<<endl;
    }
    
    char figname[256];
  //-------------------------------------------------------------------------------    
  if (plot1)
    { 
      TH1D * h_up_1 = (TH1D*)f1->Get("Ht_2_Up_3_uncAbsoluteStat"); 
      TH1D * h_down_1 = (TH1D*)f1->Get("Ht_2_Down_3_uncAbsoluteStat"); 
      TH1D * h_up_2 = (TH1D*)f1->Get("Ht_2_Up_3_uncAbsoluteScale"); 
      TH1D * h_down_2 = (TH1D*)f1->Get("Ht_2_Down_3_uncAbsoluteScale"); 
      TH1D * h_up_3 = (TH1D*)f1->Get("Ht_2_Up_3_uncAbsoluteMPFBias"); 
      TH1D * h_down_3 = (TH1D*)f1->Get("Ht_2_Down_3_uncAbsoluteMPFBias"); 
      TH1D * h_up_4 = (TH1D*)f1->Get("Ht_2_Up_3_uncFragmentation"); 
      TH1D * h_down_4 = (TH1D*)f1->Get("Ht_2_Down_3_uncFragmentation"); 
  
      TCanvas *c1 = new TCanvas("c1", "",500,400);
      c1->Draw();
      
      h_up_1->SetLineColor(38);
      h_up_1->SetLineWidth(2);
      h_down_1->SetLineColor(38);
      h_down_1->SetLineWidth(2);
      
      h_up_2->SetLineColor(kRed+2);
      h_up_2->SetLineWidth(2);
      h_down_2->SetLineColor(kRed+2);
      h_down_2->SetLineWidth(2);
      
      h_up_3->SetLineColor(kGreen+2);
      h_up_3->SetLineWidth(2); 
      h_down_3->SetLineColor(kGreen+2);
      h_down_3->SetLineWidth(2);

      h_up_4->SetLineColor(kMagenta);
      h_up_4->SetLineWidth(2); 
      h_up_4->SetLineStyle(11);
      h_down_4->SetLineColor(kMagenta);
      h_down_4->SetLineWidth(2);
      h_down_4->SetLineStyle(11);
      
      h_up_1->Draw("hist]["); 
      h_down_1->Draw("histsame]["); 
      h_up_2->Draw("histsame][");  
      h_down_2->Draw("histsame]["); 
      h_up_3->Draw("histsame][");  
      h_down_3->Draw("histsame][");
      h_up_4->Draw("histsame][");  
      h_down_4->Draw("histsame]["); 
            
      double x1 = 0.20; // 0.62 fixed
      double x2 = x1 + 0.36; 
      double y2 = 0.92; // fixed
      double y1 = y2 - 0.29;  
      
      TLegend *legend1=new TLegend(x1,y1,x2,y2);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry((TObject*)0,"R_{32}"," ");
      legend1->AddEntry(h_up_1,"AbsoluteStat", "l");
      legend1->AddEntry(h_up_2,"AbsoluteScale", "l");
      legend1->AddEntry(h_up_3,"AbsoluteMPFBias", "l");
      legend1->AddEntry(h_up_4,"Fragmentation", "l");
      legend1->Draw();
      
      sprintf(figname,"Abs_1");
      
      makeCanvas(c1,h_up_1,figname);
    }
  //------------------------------------------------------------------------------- 
  if (plot2)
    {
      TH1D * h_up_1 = (TH1D*)f1->Get("Ht_2_Up_3_uncSinglePionECAL"); 
      TH1D * h_down_1 = (TH1D*)f1->Get("Ht_2_Down_3_uncSinglePionECAL"); 
      TH1D * h_up_2 = (TH1D*)f1->Get("Ht_2_Up_3_uncSinglePionHCAL"); 
      TH1D * h_down_2 = (TH1D*)f1->Get("Ht_2_Down_3_uncSinglePionHCAL"); 
      TH1D * h_up_3 = (TH1D*)f1->Get("Ht_2_Up_3_uncFlavorQCD"); 
      TH1D * h_down_3 = (TH1D*)f1->Get("Ht_2_Down_3_uncFlavorQCD"); 
      
      TCanvas *c1 = new TCanvas("c1", "",500,400);
      c1->Draw();
      
      h_up_1->SetLineColor(38);
      h_up_1->SetLineWidth(2);
      h_down_1->SetLineColor(38);
      h_down_1->SetLineWidth(2);
      
      h_up_2->SetLineColor(kRed+2);
      h_up_2->SetLineWidth(2);
      h_down_2->SetLineColor(kRed+2);
      h_down_2->SetLineWidth(2);
      
      h_up_3->SetLineColor(kGreen+2);
      h_up_3->SetLineWidth(2); 
      h_down_3->SetLineColor(kGreen+2);
      h_down_3->SetLineWidth(2);
    
      h_up_1->Draw("hist]["); 
      h_down_1->Draw("histsame]["); 
      h_up_2->Draw("histsame][");  
      h_down_2->Draw("histsame]["); 
      h_up_3->Draw("histsame][");  
      h_down_3->Draw("histsame][");
            
      double x1 = 0.20; // 0.62 fixed
      double x2 = x1 + 0.36; 
      double y2 = 0.92; // fixed
      double y1 = y2 - 0.25;  
      
      TLegend *legend1=new TLegend(x1,y1,x2,y2);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry((TObject*)0,"R_{32}"," ");
      legend1->AddEntry(h_up_1,"SinglePionECAL", "l");
      legend1->AddEntry(h_up_2,"SinglePionHCAL", "l");
      legend1->AddEntry(h_up_3,"FlavorQCD", "l");
      legend1->Draw();
      
      sprintf(figname,"Single_1");
      
      makeCanvas(c1,h_up_1,figname);
   }
  //------------------------------------------------------------------------------- 
  if (plot3)
    { 
      TH1D * h_up_1 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeJEREC1"); 
      TH1D * h_down_1 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeJEREC1"); 
      TH1D * h_up_2 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeJEREC2"); 
      TH1D * h_down_2 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeJEREC2"); 
      TH1D * h_up_3 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativePtBB"); 
      TH1D * h_down_3 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativePtBB"); 
      TH1D * h_up_4 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeJERHF"); 
      TH1D * h_down_4 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeJERHF"); 
      
      TCanvas *c1 = new TCanvas("c1", "",500,400);
      c1->Draw();
      
      h_up_1->SetLineColor(38);
      h_up_1->SetLineWidth(2);
      h_down_1->SetLineColor(38);
      h_down_1->SetLineWidth(2);
      
      h_up_2->SetLineColor(kRed+2);
      h_up_2->SetLineWidth(2);
      h_down_2->SetLineColor(kRed+2);
      h_down_2->SetLineWidth(2);
      
      h_up_3->SetLineColor(kGreen+2);
      h_up_3->SetLineWidth(2); 
      h_down_3->SetLineColor(kGreen+2);
      h_down_3->SetLineWidth(2);

      h_up_4->SetLineColor(kMagenta);
      h_up_4->SetLineWidth(2); 
      h_up_4->SetLineStyle(11);
      h_down_4->SetLineColor(kMagenta);
      h_down_4->SetLineWidth(2);
      h_down_4->SetLineStyle(11);
           
      h_up_1->Draw("hist]["); 
      h_down_1->Draw("histsame]["); 
      h_up_2->Draw("histsame][");  
      h_down_2->Draw("histsame]["); 
      h_up_3->Draw("histsame][");  
      h_down_3->Draw("histsame][");
      h_up_4->Draw("histsame][");  
      h_down_4->Draw("histsame]["); 
          
      double x1 = 0.20; // 0.62 fixed
      double x2 = x1 + 0.36; 
      double y2 = 0.92; // fixed
      double y1 = y2 - 0.29;  
      
      TLegend *legend1=new TLegend(x1,y1,x2,y2);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry((TObject*)0,"R_{32}"," ");
      legend1->AddEntry(h_up_1,"RelativeJEREC1", "l");
      legend1->AddEntry(h_up_2,"RelativeJEREC2", "l");
      legend1->AddEntry(h_up_3,"RelativePtBB", "l");
      legend1->AddEntry(h_up_4,"RelativeJERHF", "l");
      legend1->Draw();
      
      sprintf(figname,"Relative_1");
      
      makeCanvas(c1,h_up_1,figname);
    }
  //-------------------------------------------------------------------------------
  if (plot4)
    {
      TH1D * h_up_1 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativePtEC1"); 
      TH1D * h_down_1 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativePtEC1"); 
      TH1D * h_up_2 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativePtEC2"); 
      TH1D * h_down_2 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativePtEC2"); 
      TH1D * h_up_3 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeFSR"); 
      TH1D * h_down_3 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeFSR");
      TH1D * h_up_4 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativePtHF"); 
      TH1D * h_down_4 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativePtHF"); 
  
      TCanvas *c1 = new TCanvas("c1", "",500,400);
      c1->Draw();
      
      h_up_1->SetLineColor(38);
      h_up_1->SetLineWidth(2);
      h_down_1->SetLineColor(38);
      h_down_1->SetLineWidth(2);
      
      h_up_2->SetLineColor(kRed+2);
      h_up_2->SetLineWidth(2);
      h_down_2->SetLineColor(kRed+2);
      h_down_2->SetLineWidth(2);
      
      h_up_3->SetLineColor(kGreen+2);
      h_up_3->SetLineWidth(2); 
      h_down_3->SetLineColor(kGreen+2);
      h_down_3->SetLineWidth(2);
    
      h_up_4->SetLineColor(kMagenta);
      h_up_4->SetLineWidth(2); 
      h_up_4->SetLineStyle(11);
      h_down_4->SetLineColor(kMagenta);
      h_down_4->SetLineWidth(2);
      h_down_4->SetLineStyle(11);
      
      h_up_1->Draw("hist]["); 
      h_down_1->Draw("histsame]["); 
      h_up_2->Draw("histsame][");  
      h_down_2->Draw("histsame]["); 
      h_up_3->Draw("histsame][");  
      h_down_3->Draw("histsame][");
      h_up_4->Draw("histsame][");  
      h_down_4->Draw("histsame][");
            
      double x1 = 0.20; // 0.62 fixed
      double x2 = x1 + 0.36; 
      double y2 = 0.92; // fixed
      double y1 = y2 - 0.29;  
      
      TLegend *legend1=new TLegend(x1,y1,x2,y2);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry((TObject*)0,"R_{32}"," ");
      legend1->AddEntry(h_up_1,"RelativePtEC1", "l");
      legend1->AddEntry(h_up_2,"RelativePtEC2", "l");
      legend1->AddEntry(h_up_3,"RelativeFSR", "l");
      legend1->AddEntry(h_up_4,"RelativePtHF", "l");      
      legend1->Draw();
      
      sprintf(figname,"Relative_2");
      
      makeCanvas(c1,h_up_1,figname);
   } 
  //------------------------------------------------------------------------------- 
      if (plot5)
    {
      TH1D * h_up_1 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeStatFSR"); 
      TH1D * h_down_1 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeStatFSR"); 
      TH1D * h_up_2 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeStatEC2"); 
      TH1D * h_down_2 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeStatEC2"); 
      TH1D * h_up_3 = (TH1D*)f1->Get("Ht_2_Up_3_uncRelativeStatHF"); 
      TH1D * h_down_3 = (TH1D*)f1->Get("Ht_2_Down_3_uncRelativeStatHF"); 
     
      TCanvas *c1 = new TCanvas("c1", "",500,400);
      c1->Draw();
      
      h_up_1->SetLineColor(38);
      h_up_1->SetLineWidth(2);
      h_down_1->SetLineColor(38);
      h_down_1->SetLineWidth(2);
      
      h_up_2->SetLineColor(kRed+2);
      h_up_2->SetLineWidth(2);
      h_down_2->SetLineColor(kRed+2);
      h_down_2->SetLineWidth(2);
      
      h_up_3->SetLineColor(kGreen+2);
      h_up_3->SetLineWidth(2); 
      h_down_3->SetLineColor(kGreen+2);
      h_down_3->SetLineWidth(2);
      
      h_up_1->Draw("hist]["); 
      h_down_1->Draw("histsame]["); 
      h_up_2->Draw("histsame][");  
      h_down_2->Draw("histsame]["); 
      h_up_3->Draw("histsame][");  
      h_down_3->Draw("histsame][");
            
      double x1 = 0.20; // 0.62 fixed
      double x2 = x1 + 0.36; 
      double y2 = 0.92; // fixed
      double y1 = y2 - 0.29;  
      
      TLegend *legend1=new TLegend(x1,y1,x2,y2);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry((TObject*)0,"R_{32}"," ");
      legend1->AddEntry(h_up_1,"RelativeStatFSR", "l");
      legend1->AddEntry(h_up_2,"RelativeStatEC2", "l");
      legend1->AddEntry(h_up_3,"RelativeStatHF", "l");
      legend1->Draw();
      
      sprintf(figname,"RelativeStat_1");
      
      makeCanvas(c1,h_up_1,figname);
   } 
  //------------------------------------------------------------------------------- 
  if (plot6)
    { 
      TH1D * h_up_1 = (TH1D*)f1->Get("Ht_2_Up_3_uncPileUpDataMC"); 
      TH1D * h_down_1 = (TH1D*)f1->Get("Ht_2_Down_3_uncPileUpDataMC"); 
      TH1D * h_up_2 = (TH1D*)f1->Get("Ht_2_Up_3_uncPileUpPtRef"); 
      TH1D * h_down_2 = (TH1D*)f1->Get("Ht_2_Down_3_uncPileUpPtRef"); 
      TH1D * h_up_3 = (TH1D*)f1->Get("Ht_2_Up_3_uncPileUpPtBB"); 
      TH1D * h_down_3 = (TH1D*)f1->Get("Ht_2_Down_3_uncPileUpPtBB"); 
      TH1D * h_up_4 = (TH1D*)f1->Get("Ht_2_Up_3_uncPileUpPtEC1"); 
      TH1D * h_down_4 = (TH1D*)f1->Get("Ht_2_Down_3_uncPileUpPtEC1"); 
      TH1D * h_up_5 = (TH1D*)f1->Get("Ht_2_Up_3_uncPileUpPtEC2"); 
      TH1D * h_down_5 = (TH1D*)f1->Get("Ht_2_Down_3_uncPileUpPtEC2"); 
      TH1D * h_up_6 = (TH1D*)f1->Get("Ht_2_Up_3_uncPileUpPtHF"); 
      TH1D * h_down_6 = (TH1D*)f1->Get("Ht_2_Down_3_uncPileUpPtHF");
      
      TCanvas *c1 = new TCanvas("c1", "",500,400);
      c1->Draw();
      
      h_up_1->SetLineColor(38);
      h_up_1->SetLineWidth(2);
      h_down_1->SetLineColor(38);
      h_down_1->SetLineWidth(2);
      
      h_up_2->SetLineColor(kRed+2);
      h_up_2->SetLineWidth(2);
      h_down_2->SetLineColor(kRed+2);
      h_down_2->SetLineWidth(2);
      
      h_up_3->SetLineColor(kGreen+2);
      h_up_3->SetLineWidth(2); 
      h_down_3->SetLineColor(kGreen+2);
      h_down_3->SetLineWidth(2);

      h_up_4->SetLineColor(kMagenta);
      h_up_4->SetLineWidth(2); 
      h_up_4->SetLineStyle(11);
      h_down_4->SetLineColor(kMagenta);
      h_down_4->SetLineWidth(2);
      h_down_4->SetLineStyle(11);
      
      h_up_5->SetLineColor(kOrange-3);
      h_up_5->SetLineWidth(2); 
      h_down_5->SetLineColor(kOrange-3);
      h_down_5->SetLineWidth(2);
      
      h_up_6->SetLineColor(kGray+3);
      h_up_6->SetLineWidth(2); 
      h_down_6->SetLineColor(kGray+3);
      h_down_6->SetLineWidth(2);
      
      h_up_1->Draw("hist]["); 
      h_down_1->Draw("histsame]["); 
      h_up_2->Draw("histsame][");  
      h_down_2->Draw("histsame]["); 
      h_up_3->Draw("histsame][");  
      h_down_3->Draw("histsame][");
      h_up_4->Draw("histsame][");  
      h_down_4->Draw("histsame]["); 
      h_up_5->Draw("histsame][");  
      h_down_5->Draw("histsame][");
      h_up_6->Draw("histsame][");  
      h_down_6->Draw("histsame]["); 
            
      double x1 = 0.20; // 0.62 fixed
      double x2 = x1 + 0.36; 
      double y2 = 0.92; // fixed
      double y1 = y2 - 0.29;  
      
      TLegend *legend1=new TLegend(x1,y1,x2,y2);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry((TObject*)0,"R_{32}"," ");
      legend1->AddEntry(h_up_1,"PileUpDataMC", "l");
      legend1->AddEntry(h_up_2,"PileUpPtRef", "l");
      legend1->AddEntry(h_up_3,"PileUpPtBB", "l");
      legend1->AddEntry(h_up_4,"PileUpPtEC1", "l");
      legend1->Draw();
      
      TLegend *legend1=new TLegend(x1,y1-0.36,x2,y2-0.50);
      legend1->SetTextFont(42);
      legend1->SetTextSize(0.05);
      legend1->SetLineStyle(1);
      legend1->SetLineWidth(1);
      legend1->SetFillColor(0);
      legend1->SetBorderSize(0);
      legend1->SetLineColor(1);
      legend1->AddEntry(h_up_5,"PileUpPtEC2", "l");
      legend1->AddEntry(h_up_6,"PileUpPtHF", "l");
      legend1->Draw();
      
      sprintf(figname,"Pileup_1");
      
      makeCanvas(c1,h_up_1,figname);
    }
}           
