#include "tdrstyle.C"
#include "CMS_lumi.C"
#include "TH1.h"
#include "TH1F.h"

void total_unc_ratio_32_direct_add(){
  setTDRStyle();
  
  TFile *f_ratio_32 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/AN_Final_Plots/RootFiles/Plot_Ratio_32_direct_unfolded_all.root");
  
  TFile *fJEC = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/Monte/NP_Corr/JES/Ratio/RootFiles/JES_Ratio_unc_total.root");
  
  TFile *fUnf = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Unfolding_unc_ratio_32.root");
 
  TFile *fJER = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/JER_total_unc_ratio_32_NLO.root");
  
 double bins[57] = {20., 25., 30., 35., 40., 45., 50., 55., 60., 65., 70., 80., 90., 110., 130., 150., 175., 200., 225., 250., 275., 300., 330., 360., 390., 420., 450., 480., 510., 540., 570., 600., 640., 680., 720., 760., 800., 850., 900., 950., 1000., 1060., 1120., 1180., 1250., 1320., 1390., 1460., 1530., 1600., 1680., 1760., 1840., 1920., 2000., 2500., 3000.};
 
   int arr_size = sizeof(bins)/sizeof(bins[0]);
  cout<<" Array size : "<<arr_size-1<<endl;
  
  TH1D *hratio_up = (TH1D*)f_ratio_32->Get("Unfolded_ratio_32_up_smear2_new");
  TH1D *hratio_down = (TH1D*)f_ratio_32->Get("Unfolded_ratio_32_down_smear2_new");
  TH1D *hratio_symm = (TH1D*)f_ratio_32->Get("Unfolded_ratio_32_symm_smear2_new");
  
  TH1D *hStatMarker = new TH1D("hStatMarker","hStatMarker",arr_size-1,bins);
  TH1D *hStatp = new TH1D("Statp","Statp",arr_size-1,bins);
  TH1D *hStatn = new TH1D("Statn","Statn",arr_size-1,bins);
  
  TH1D *hJECup =  (TH1D*)fJEC->Get("JECup MC");
  TH1D *hJECdown =  (TH1D*)fJEC->Get("JECdown MC");
  
  TH1D *hJECup_frac =  (TH1D*)fJEC->Get("JECup MC frac");
  TH1D *hJECdown_frac =  (TH1D*)fJEC->Get("JECdown MC frac");
  
  TH1D *hJERup_frac =  (TH1D*)fJER->Get("JER_Total_up_ratio_unc_frac");
  TH1D *hJERdown_frac =  (TH1D*)fJER->Get("JER_Total_down_ratio_unc_frac");
  
  TH1D *hJERup =  (TH1D*)fJER->Get("JER_Total_up_ratio_unc");
  TH1D *hJERdown =  (TH1D*)fJER->Get("JER_Total_down_ratio_unc"); 
  
  TH1D *hUnfup_frac =  (TH1D*)fUnf->Get("Unfolding_unc_Up_ratio_32_frac");
  TH1D *hUnfdown_frac =  (TH1D*)fUnf->Get("Unfolding_unc_Down_ratio_32_frac");
  
  TH1D *hUnfup =  (TH1D*)fUnf->Get("Unfolding_unc_Up_ratio_32");
  TH1D *hUnfdown =  (TH1D*)fUnf->Get("Unfolding_unc_Down_ratio_32");
  
  TH1D *hTotal_up_frac = new TH1D("Total_up_frac","Total_up_frac",arr_size-1,bins);
  TH1D *hTotal_down_frac = new TH1D("Total_down_frac","Total_down_frac",arr_size-1,bins);
  TH1D *hTotal_up = new TH1D("Total_up","Total_up",arr_size-1,bins);
  TH1D *hTotal_down = new TH1D("Total_down","Total_down",arr_size-1,bins);
  
  TH1D *hTotal_up_frac_nostat = new TH1D("Total_up_frac_nostat","Total_up_frac_nostat",arr_size-1,bins);
  TH1D *hTotal_down_frac_nostat = new TH1D("Total_down_frac_nostat","Total_down_frac_nostat",arr_size-1,bins);
  TH1D *hTotal_up_nostat = new TH1D("Total_up_nostat","Total_up_nostat",arr_size-1,bins);
  TH1D *hTotal_down_nostat = new TH1D("Total_down_nostat","Total_down_nostat",arr_size-1,bins);
    
  TH1D *hJECup_new_MC = new TH1D("JECup MC new","JECup MC new",arr_size-1,bins);
  TH1D *hJECdown_new_MC = new TH1D("JECdown MC new","JECdown MC new",arr_size-1,bins);
  
  TH1D *hUnf_total_up_new_frac = new TH1D("Unf_total_up_new_frac","Unf_total_up_new_frac",arr_size-1,bins);
  TH1D *hUnf_total_down_new_frac = new TH1D("Unf_total_down_new_frac","Unf_total_down_new_frac",arr_size-1,bins);
  
  TH1D *hUnf_total_up_new = new TH1D("Unf_total_up_new","Unf_total_up_new",arr_size-1,bins);
  TH1D *hUnf_total_down_new = new TH1D("Unf_total_down_new","Unf_total_down_new",arr_size-1,bins);
       
  double nbinsx = hStatp->GetXaxis()->GetNbins();
  //cout<<nbinsx<<endl;

  //for (int i = 1; i <=26 ; i++)
  for (int i = 1; i <= nbinsx  ; i++)
    {
    //Stat
     hStatp->SetBinContent(i,1);
      if (hratio_symm->GetBinContent(i) == 0 )
      double stat_err = 1.;
      else
      double stat_err = (hratio_symm->GetBinError(i)/hratio_symm->GetBinContent(i));      
      hStatp->SetBinError(i,stat_err);  
      hStatMarker->SetBinContent(i,5);
   
      double unf_total_up = sqrt(pow(hUnfup->GetBinContent(i),2) + pow(hJERup->GetBinContent(i),2));
      double unf_total_down = sqrt(pow(hUnfdown->GetBinContent(i),2) + pow(hJERdown->GetBinContent(i),2));
      
      hUnf_total_up_new_frac->SetBinContent(i,1 + unf_total_up);
      hUnf_total_down_new_frac->SetBinContent(i,1 - unf_total_down);
     
     //Total                // stat                   //jec                    //unfolding
     double total_up = pow(stat_err,2) + pow(hJECup->GetBinContent(i),2) + pow(unf_total_up,2); //
     double total_up_sqrt = sqrt(total_up);
     hTotal_up_frac->SetBinContent(i,1+total_up_sqrt); 
     hTotal_up->SetBinContent(i,total_up_sqrt); 
     
     double total_up_nostat = pow(hJECup->GetBinContent(i),2) + pow(unf_total_up,2); 
     double total_up_sqrt_nostat = sqrt(total_up_nostat);
     hTotal_up_frac_nostat->SetBinContent(i,1+total_up_sqrt_nostat); 
     hTotal_up_nostat->SetBinContent(i,total_up_sqrt_nostat); 
     
     double total_down = pow(stat_err,2) + pow(hJECdown->GetBinContent(i),2) + pow(unf_total_down,2); //
     double total_down_sqrt = sqrt(total_down);
     hTotal_down_frac->SetBinContent(i,1-total_down_sqrt); 
     hTotal_down->SetBinContent(i,total_down_sqrt); 
     
     double total_down_nostat = pow(hJECdown->GetBinContent(i),2) + pow(unf_total_down,2); //
     double total_down_sqrt_nostat = sqrt(total_down_nostat);
     hTotal_down_frac_nostat->SetBinContent(i,1-total_down_sqrt_nostat); 
     hTotal_down_nostat->SetBinContent(i,total_down_sqrt_nostat);
     
     if (i > 21 && i < 51)
	{
	  //cout<<hTotal_up->GetBinLowEdge(i)<<"-"<<hTotal_up->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<h2_new->GetBinContent(i)*1000.<<" & "<<std::fixed<<std::setprecision(3)<<(h2_new->GetBinError(i)/h2_new->GetBinContent(i))*100<<" & $^{+"<<hJECup_new_MC_pre->GetBinContent(i)*100<<"}_{-"<<hJECdown_new_MC_pre->GetBinContent(i)*100<<"}$"<<" & $^{+"<<unf_total_up*100<<"}_{-"<<unf_total_down*100<<"}$ & "<<" 2.6 & "<<" 1.0 & $^{+"<<total_up_sqrt*100<<"}_{-"<<total_down_sqrt*100<<"}$ \\\Rbtrrn \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
	
     	cout<<hTotal_up->GetBinLowEdge(i)<<" - "<<hTotal_up->GetBinLowEdge(i+1)<<" & "<<std::fixed<<std::setprecision(3)<<stat_err*100<<" & $^{+"<<hJECup->GetBinContent(i)*100<<"}_{-"<<hJECdown->GetBinContent(i)*100<<"}$"<<" & $^{+"<<unf_total_up*100<<"}_{-"<<unf_total_down*100<<"}$ & $^{+"<<total_up_sqrt*100<<"}_{-"<<total_down_sqrt*100<<"}$ \\\Rbtrrn \\\\ \\\hline"<<std::fixed<<std::setprecision(0)<<endl;
     	}
    }

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
 
  hStatp->SetTitle("");	
  hStatp->GetXaxis()->SetRangeUser(300.,1680.); 
  hJECup->GetXaxis()->SetRangeUser(300.,1680.); 
  hJECdown->GetXaxis()->SetRangeUser(300.,1680.); 
  hJERup_frac->GetXaxis()->SetRangeUser(300.,1680.); 
  hJERdown_frac->GetXaxis()->SetRangeUser(300.,1680.); 
  hTotal_up_frac->GetXaxis()->SetRangeUser(300.,1680.); 
  hTotal_down_frac->GetXaxis()->SetRangeUser(300.,1680.); 
  hUnf_total_up_new_frac->GetXaxis()->SetRangeUser(300.,1680.); 
  hUnf_total_down_new_frac->GetXaxis()->SetRangeUser(300.,1680.);   
  hStatp->GetYaxis()->SetRangeUser(0.65,1.55); 
  
  hTotal_up_frac->SetLineStyle(2);
  hTotal_up_frac->SetLineWidth(2);
  hTotal_up_frac->SetLineColor(kBlack);
  
  hTotal_down_frac->SetLineStyle(2);
  hTotal_down_frac->SetLineWidth(2);
  hTotal_down_frac->SetLineColor(kBlack);
    
  hStatp->SetLineColor(kBlack);
  hStatp->SetLineWidth(2);
  hStatp->SetMarkerSize(0.2);
  hStatMarker->SetMarkerStyle(20);
  hStatMarker->SetMarkerSize(0.2);
  hStatMarker->SetLineWidth(2);
  
  hJECup_frac->SetLineColor(38);
  hJECup_frac->SetLineWidth(2);
  
  hJECdown_frac->SetLineColor(38);
  hJECdown_frac->SetLineWidth(2);
  
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
  hJECup_frac->Draw("histsame][");
  hJECdown_frac->Draw("histsame][");
  hUnf_total_up_new_frac->Draw("histsame][");
  hUnf_total_down_new_frac->Draw("histsame][");  
  /*hUnfup_frac->Draw("histsame][");
  hUnfdown_frac->Draw("histsame][");
  hJERup_frac->Draw("histsame][");
  hJERdown_frac->Draw("histsame][");
  */hTotal_up_frac->Draw("histsame][");
  hTotal_down_frac->Draw("histsame][");
  
  double x1 = 0.25; // 0.62 fixed
  double x2 = x1 + 0.41;
  double y2 = 0.9; // fixed
  double y1 = y2 - 0.25;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"#it{R}_{32}"," ");
  legend1->AddEntry(hStatMarker,"Statistical","pel");
  legend1->AddEntry(hJECup_frac,"JEC","l");
  legend1->AddEntry(hUnf_total_up_new_frac,"Unfolding","l");
  legend1->AddEntry(hTotal_up_frac,"Total","l");
  legend1->Draw();
  
  tex = new TLatex(281,0.5837793,"300");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex1 = new TLatex(1541,0.5837793,"1680");
  tex1->SetTextFont(42);
  tex1->SetTextSize(0.05);
  tex1->SetLineWidth(2);
  tex1->Draw();
 
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.69,0.95,"#font[61]{19.71 fb^{-1} (8 TeV)}");
      
  c->Draw();
 
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Total_Unc_ratio_32_direct_add.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Total_Unc_ratio_32_direct_add.pdf");
  
}
