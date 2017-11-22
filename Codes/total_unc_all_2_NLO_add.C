#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void total_unc_all_2_NLO_add(){
  setTDRStyle();
    
  TFile *fdata_2 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_NLO_2_range_funcI.root");
  TFile *fJEC_MC = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/RootFiles_HT_2_150/JES_unc_cal_all_HT_2_150_sources_add_24.root");
  TFile *fJER = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/JER_total_unc_2_range_NLO.root");
  TFile *fUnf = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_unc_2_range.root");
    
  double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
 
  int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
  
  TH1D *hData_2 = (TH1D*)fdata_2->Get("Measured_True");
  TH1D *hTotal_up = new TH1D("Total_up","Total_up",arr_size-1,bins);
  TH1D *hTotal_down = new TH1D("Total_down","Total_down",arr_size-1,bins);
  
  TH1D *hLumi = new TH1D("Lumi","Lumi",arr_size-1,bins);
  TH1D *hUncor = new TH1D("Uncor","Uncor",arr_size-1,bins);
  TH1D *hStatMarker = new TH1D("hStatMarker","hStatMarker",arr_size-1,bins);
  TH1D *hStatp = new TH1D("Statp","Statp",arr_size-1,bins);
  TH1D *hStatn = new TH1D("Statn","Statn",arr_size-1,bins);
  
  TH1D *hJECup_new_MC = new TH1D("JECup MC","JECup MC",arr_size-1,bins);
  TH1D *hJECdown_new_MC = new TH1D("JECdown MC","JECdown MC",arr_size-1,bins);
  
  TH1D *hJECup_new_MC_pre = new TH1D("JECup MC pre","JECup MC pre",arr_size-1,bins);
  TH1D *hJECdown_new_MC_pre = new TH1D("JECdown MC pre","JECdown MC pre",arr_size-1,bins);
   
  TH1D *hUnf_total_up_new_frac = new TH1D("Unf_total_up_new_frac","Unf_total_up_new_frac",arr_size-1,bins);
  TH1D *hUnf_total_down_new_frac = new TH1D("Unf_total_down_new_frac","Unf_total_down_new_frac",arr_size-1,bins);
  
  TH1D *hUnf_total_up_new = new TH1D("Unf_total_up_new","Unf_total_up_new",arr_size-1,bins);
  TH1D *hUnf_total_down_new = new TH1D("Unf_total_down_new","Unf_total_down_new",arr_size-1,bins);
   
  TH1D *hJECup_MC =  (TH1D*)fJEC_MC->Get("PFJetHtUp_2_unc_total");
  TH1D *hJECdown_MC =  (TH1D*)fJEC_MC->Get("PFJetHtDown_2_unc_total");
  
  TH1D *hJERup_frac =  (TH1D*)fJER->Get("JER_Total_up_2_unc_frac");
  TH1D *hJERdown_frac =  (TH1D*)fJER->Get("JER_Total_down_2_unc_frac");
  
  TH1D *hJERup =  (TH1D*)fJER->Get("JER_Total_up_2_unc");
  TH1D *hJERdown =  (TH1D*)fJER->Get("JER_Total_down_2_unc"); 
  
  TH1D *hUnfup_frac =  (TH1D*)fUnf->Get("Unfolding_unc_Up_2_frac");
  TH1D *hUnfdown_frac =  (TH1D*)fUnf->Get("Unfolding_unc_Down_2_frac");
  
  TH1D *hUnfup =  (TH1D*)fUnf->Get("Unfolding_unc_Up_2");
  TH1D *hUnfdown =  (TH1D*)fUnf->Get("Unfolding_unc_Down_2");
   
  double nbinsx = hLumi->GetXaxis()->GetNbins();
  cout<<nbinsx<<endl;
  
  TH1D *h2_new = new TH1D ("h2_new","h2_new", arr_size-1,bins);
  h2_new->Sumw2();

  //for (int i = 1; i <=26 ; i++)
  for (int i = 1; i <= nbinsx  ; i++)
    {
    
      hJECup_new_MC_pre->SetBinContent(i,hJECup_MC->GetBinContent(i));  
      hJECdown_new_MC_pre->SetBinContent(i,hJECdown_MC->GetBinContent(i));  
     
      h2_new->SetBinContent(i,hData_2->GetBinContent(i));  
      h2_new->SetBinError(i,hData_2->GetBinError(i));  
    }

  h2_new->Scale(1/19711.225);
  h2_new->Scale(1.0, "width");
         
  //for (int i = 1; i <=26 ; i++)
  for (int i = 1; i <= nbinsx  ; i++)
    {
      //Luminosity
      hLumi->SetBinContent(i,5);

      //Uncorrelated
      hUncor->SetBinContent(i,5);
     
      //JEC_Mc
      hJECup_new_MC->SetBinContent(i,1 + (hJECup_MC->GetBinContent(i)));  
      hJECdown_new_MC->SetBinContent(i,1 -(hJECdown_MC->GetBinContent(i)));  
        
      //Stat     
      hStatp->SetBinContent(i,1);
      if (hData_2->GetBinContent(i) == 0 )
	double stat_err = 1.;
      else
	double stat_err = (hData_2->GetBinError(i)/hData_2->GetBinContent(i));
      hStatp->SetBinError(i,stat_err);  
      hStatMarker->SetBinContent(i,5);
    
      //JER
      //Sys
      //Unfolding
      
      double unf_total_up = sqrt(pow(hUnfup->GetBinContent(i),2) + pow(hJERup->GetBinContent(i),2));
      double unf_total_down = sqrt(pow(hUnfdown->GetBinContent(i),2) + pow(hJERdown->GetBinContent(i),2));
      
      hUnf_total_up_new_frac->SetBinContent(i,1 + unf_total_up);
      hUnf_total_down_new_frac->SetBinContent(i,1 - unf_total_down);      
      //double bincontent = sigma[i-1];
      //double er = sigma_err[i-1];
     
     
      //Total
      //lumi           uncor       // stat                   //jec                            //unfolding
      double total_up = pow(0.026,2) + pow(0.01,2) + pow(stat_err,2)+ pow(hJECup_new_MC_pre->GetBinContent(i),2) + pow(unf_total_up,2); // jer
      double total_up_sqrt = sqrt(total_up);
      hTotal_up->SetBinContent(i,1+total_up_sqrt); 
     
      double total_down = pow(0.026,2) + pow(0.01,2) + pow(stat_err,2) + pow(hJECdown_new_MC_pre->GetBinContent(i),2) + pow(unf_total_down,2);
      double total_down_sqrt = sqrt(total_down);
      hTotal_down->SetBinContent(i,1-total_down_sqrt); 
      
      if (i > 21 && i < 55)
	{
	  double jec_t = sqrt((pow(hJECup_new_MC_pre->GetBinContent(i),2)) + (pow(hJECdown_new_MC_pre->GetBinContent(i),2))) *100;
	  double unf_t = sqrt((pow(unf_total_up,2)) + (pow(unf_total_down,2))) *100;
	  double total_t = sqrt((pow(total_up_sqrt,2)) + (pow(total_down_sqrt,2))) *100;
	  //cout<<hTotal_up->GetBinLowEdge(i)<<"-"<<hTotal_up->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<h2_new->GetBinContent(i)*1000.<<" & "<<std::fixed<<std::setprecision(3)<<(h2_new->GetBinError(i)/h2_new->GetBinContent(i))*100<<" & $^{+"<<hJECup_new_MC_pre->GetBinContent(i)*100<<"}_{-"<<hJECdown_new_MC_pre->GetBinContent(i)*100<<"}$"<<" & $^{+"<<unf_total_up*100<<"}_{-"<<unf_total_down*100<<"}$ & "<<" 2.6 & "<<" 1.0 & $^{+"<<total_up_sqrt*100<<"}_{-"<<total_down_sqrt*100<<"}$ \\\Rbtrrn \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
	  cout<<hTotal_up->GetBinLowEdge(i)<<" - "<<hTotal_up->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<(h2_new->GetBinError(i)/h2_new->GetBinContent(i))*100<<" & $^{+"<<hJECup_new_MC_pre->GetBinContent(i)*100<<"}_{-"<<hJECdown_new_MC_pre->GetBinContent(i)*100<<"}$"<<" & $^{+"<<unf_total_up*100<<"}_{-"<<unf_total_down*100<<"}$ & "<<" 2.6 & "<<" 1.0 & $^{+"<<total_up_sqrt*100<<"}_{-"<<total_down_sqrt*100<<"}$ \\\Rbtrrn \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
        //cout<<hTotal_up->GetBinLowEdge(i)<<" - "<<hTotal_up->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<(h2_new->GetBinError(i)/h2_new->GetBinContent(i))*100<<" & "<<jec_t<<" ($^{+"<<hJECup_new_MC_pre->GetBinContent(i)*100<<"}_{-"<<hJECdown_new_MC_pre->GetBinContent(i)*100<<"}$)"<<" & "<<unf_t<<" ($^{+"<<unf_total_up*100<<"}_{-"<<unf_total_down*100<<"}$) & "<<" 2.6 & "<<" 1.0 & "<<total_t<<" ($^{+"<<total_up_sqrt*100<<"}_{-"<<total_down_sqrt*100<<"}$) \\\Rbtrrn \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
        }
    }
  //hStatp->Print("all");
  //hTotal_up->Print("all");
  //hJECup_new->Print("all"); 
  //hJECdown_new->Print("all"); 
  //hJECup_MC->Print("all"); 
  //hJECup_new_MC->Print("all"); 
  
  /* hUnf_total_up_new_frac->Print("all"); 
     hUnfup_frac->Print("all"); 
     hJERup_frac->Print("all"); 
  */ 
  hStatp->SetTitle("");	
  gStyle->SetLineStyleString(11,"40 15"); 
  
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

  hLumi->GetXaxis()->SetRangeUser(300.,2000.); 
  hStatp->GetXaxis()->SetRangeUser(300.,2000.); 
  hJERup_frac->GetXaxis()->SetRangeUser(300.,2000.); 
  hJERdown_frac->GetXaxis()->SetRangeUser(300.,2000.); 
  hTotal_up->GetXaxis()->SetRangeUser(300.,2000.); 
  hTotal_down->GetXaxis()->SetRangeUser(300.,2000.); 
  hUnf_total_up_new_frac->GetXaxis()->SetRangeUser(300.,2000.); 
  hUnf_total_down_new_frac->GetXaxis()->SetRangeUser(300.,2000.);     
  hStatp->GetYaxis()->SetRangeUser(0.65,1.55); 
  
  hTotal_up->SetLineStyle(2);
  hTotal_up->SetLineWidth(2);
  hTotal_up->SetLineColor(kBlack);
  
  hTotal_down->SetLineStyle(2);
  hTotal_down->SetLineWidth(2);
  hTotal_down->SetLineColor(kBlack);
  
  hLumi->SetLineStyle(5);
  hLumi->SetLineWidth(2);
  hLumi->SetLineColor(kRed-3);
  
  hUncor->SetLineStyle(1);
  hUncor->SetLineWidth(2);
  hUncor->SetLineColor(kViolet-9);
    
  hStatp->SetLineColor(kBlack);
  hStatp->SetLineWidth(2);
  hStatp->SetMarkerSize(0.2);
  //hStatMarker->SetMarkerStyle(34);
  //hStatMarker->SetMarkerSize(1.15);
  hStatMarker->SetMarkerStyle(20);
  hStatMarker->SetMarkerSize(0.2);
  hStatMarker->SetLineWidth(2);
  
  hJECup_new_MC->SetLineColor(38);
  hJECup_new_MC->SetLineWidth(2);
  
  hJECdown_new_MC->SetLineColor(38);
  hJECdown_new_MC->SetLineWidth(2);
  
  hJERup_frac->SetLineColor(kGreen+2);
  hJERup_frac->SetLineWidth(2);
  
  hJERdown_frac->SetLineColor(kGreen+2);
  hJERdown_frac->SetLineWidth(2);

  hUnf_total_up_new_frac->SetLineColor(kGreen+2);
  hUnf_total_up_new_frac->SetLineWidth(2);
  hUnf_total_up_new_frac->SetLineStyle(9);  
  
  hUnf_total_down_new_frac->SetLineColor(kGreen+2);
  hUnf_total_down_new_frac->SetLineWidth(2);
  hUnf_total_down_new_frac->SetLineStyle(9);  

  hLumi->SetStats(0);
  hStatp->SetStats(0);
  
  hStatp->GetXaxis()->SetTitle("#it{H}_{T,2}/2 (GeV)");
  hStatp->GetXaxis()->SetTitleOffset(1.04);
  hStatp->GetXaxis()->SetTitleSize(0.06);
  hStatp->GetXaxis()->SetMoreLogLabels();
  hStatp->GetXaxis()->SetNoExponent();
  hStatp->GetXaxis()->SetLabelSize(0.05);
  hStatp->GetXaxis()->SetLabelOffset(0.018);
  hStatp->GetXaxis()->SetTickLength(0.03);
  
  hStatp->GetYaxis()->SetTitle("Fractional Uncertainty");
  hStatp->GetYaxis()->SetTitleOffset(1.00);
  hStatp->GetYaxis()->SetTitleSize(0.06);
  hStatp->GetYaxis()->SetLabelSize(0.05);
  hStatp->GetYaxis()->SetTickLength(0.03);
  hStatp->GetYaxis()->SetLabelOffset(0.008);
  hStatp->GetYaxis()->SetNdivisions(507);
  hStatp->GetYaxis()->SetDecimals(4);
    
  hStatp->Draw("e");
  hJECup_new_MC->Draw("histsame][");
  hJECdown_new_MC->Draw("histsame][");
  hTotal_up->Draw("histsame][");
  hTotal_down->Draw("histsame][");
  hUnf_total_up_new_frac->Draw("histsame][");
  hUnf_total_down_new_frac->Draw("histsame][");
  /*  hUnfup_frac->Draw("histsame][");
      hUnfdown_frac->Draw("histsame][");
      hJERup_frac->Draw("histsame][");
      hJERdown_frac->Draw("histsame][");
  */   
  double x1 = 0.25; // 0.62 fixed
  double x2 = x1 + 0.41;
  double y2 = 0.9; // fixed
  double y1 = y2 - 0.36;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0," n_{j} #geq 2"," ");
  legend1->AddEntry(hStatMarker,"Statistical","pel");
  legend1->AddEntry(hJECup_new_MC,"JEC","l");
  legend1->AddEntry(hLumi,"Luminosity","l");
  legend1->AddEntry(hUncor,"Residual","l");//Uncorrelated","l");
  //legend1->AddEntry(hJERup_frac,"JER","l");
  legend1->AddEntry(hUnf_total_up_new_frac,"Unfolding","l");
  legend1->AddEntry(hTotal_up,"Total","l");
  legend1->Draw();
  
  TLine *lp = new TLine(300.0,1.026,2000.0,1.026);
  lp->SetLineStyle(5);
  lp->SetLineWidth(2);
  lp->SetLineColor(kRed-3);
  lp->Draw("same");
  
  TLine *ln = new TLine(300.0,0.974,2000.0,0.974);
  ln->SetLineStyle(5);
  ln->SetLineWidth(2);
  ln->SetLineColor(kRed-3);
  ln->Draw("same");
  
  TLine *lp1 = new TLine(300.0,1.01,2000.0,1.01);
  lp1->SetLineStyle(1);
  lp1->SetLineWidth(2);
  lp1->SetLineColor(kViolet-9);
  lp1->Draw("same");
  
  TLine *ln1 = new TLine(300.0,0.99,2000.0,0.99);
  ln1->SetLineStyle(1);
  ln1->SetLineWidth(2);
  ln1->SetLineColor(kViolet-9);
  ln1->Draw("same");
  
  tex = new TLatex(281,0.5837793,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
  
  c->Draw();
   
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Total_unc_all_2_NLO_add.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Total_unc_all_2_NLO_add.pdf");

}
