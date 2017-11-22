{
  TFile *f1 = TFile::Open("/home/anter/Desktop/Analysis_8_HT_2/NLO/Final_Unfolding/Rootfile_NLO_HT_2_150/Response_Matrix_NLO_range.root");

  //************************************************************************
    
  TH2D *h1_11 =  (TH2D*)f1->Get("Response Matrix 2");
      
  TH2D *Matrix_GenReco_norm = (TH2D*)h1_11->Clone(); 
      
  TCanvas *c = new TCanvas("c", "",500,400);
  c->Range(2.318121,0.4327759,3.346459,1.683612);
  c->SetFillColor(0);
  c->SetBorderMode(0);
  c->SetBorderSize(2);
  c->SetTickx(1);
  c->SetTicky(1);
  c->SetLeftMargin(0.1546185);
  //c->SetRightMargin(0.04417671);
  c->SetTopMargin(0.06684492);
  c->SetBottomMargin(0.1336898);
  c->SetLogx();
  c->SetLogy();
   
  int G1_x = h1_11->GetNbinsX();
  int G1_y = h1_11->GetNbinsY();
  //int G1_y = 2;

  cout<<"G1_x : "<<G1_x<<"   G1_y : "<<G1_y<<endl;
  
  double recoweight[G1_y+2];
  for (int i = 0; i < G1_x+2; i++)
    {
      recoweight[i] = 0.0;
      
      for(int j =0; j < G1_y+2; j++)
	{
	  recoweight[i]+ = h1_11->GetBinContent(i,j);
	  //cout<< " y bin pre : "  << i << " x bin pre : "<< j <<" response :" << h1_11->GetBinContent(j,i) <<std::endl;
	  // printf ("sum : %f \n", recoweight[i]);
	  //cout<< "   "  << i << "   "   <<"response :" << recoweight[i] <<std::endl;
	}
      //cout<< "  Bin "  << i <<endl;   
      //printf ("sum total : %f \n", recoweight[i]);//
      // cout<< "   "  << i << "   "   <<"response total :" << recoweight[i] <<std::endl;
      for(int j = 0; j < G1_y+2; j++)
	{
	  double matrix_bincontent=h1_11->GetBinContent(i,j);
	  //cout<< " y bin now : "  << i << " x bin now : "<< j <<" response :" << matrix_bincontent <<std::endl;
	  Matrix_GenReco_norm->SetBinContent(i, j,(matrix_bincontent/(recoweight[i]))); 
	}
    }
  //Matrix_GenReco_norm->Print("all");
  int G2_x = Matrix_GenReco_norm->GetNbinsX();
  int G2_y = Matrix_GenReco_norm->GetNbinsY();
      
  //cout<<"G2_x : "<<G2_x<<"   G2_y : "<<G2_y<<endl;

  for (int i = 3; i < G2_x+2; i++)
    {
      for(int j =0; j < G2_y+2; j++)
	{
	  //cout<< " y bin new : "<<i<< " x bin new : "<<j<<" response new : "<<Matrix_GenReco_norm->GetBinContent(j,i)<<std::endl;
	}
    }
  int n1_11 = h1_11->GetEntries();
  int n1_11_now = Matrix_GenReco_norm->GetEntries();
  cout<<" Entries A : "<<n1_11<<" Entries now : "<<n1_11_now<<endl;
  
  gStyle->SetPalette(54);
  Matrix_GenReco_norm->SetTitle("");  
  Matrix_GenReco_norm->SetContour(20);
  Matrix_GenReco_norm->Draw("colz");
  Matrix_GenReco_norm->GetXaxis()->SetRange(16,54);
  Matrix_GenReco_norm->GetXaxis()->SetTitleSize(0.06);
  Matrix_GenReco_norm->GetXaxis()->SetTitleOffset(1.04);
  Matrix_GenReco_norm->GetXaxis()->SetLabelSize(0.05);
  Matrix_GenReco_norm->GetXaxis()->SetLabelOffset(0.010);
  //Matrix_GenReco_norm->GetYaxis()->SetTitle("True #it{H}_{T,2}/2 (GeV)");
  //Matrix_GenReco_norm->GetXaxis()->SetTitle("Measured #it{H}_{T,2}/2 (GeV)");
  Matrix_GenReco_norm->GetYaxis()->SetTitle("Gen_{Toy} #it{H}_{T,2}/2 (GeV)");
  Matrix_GenReco_norm->GetXaxis()->SetTitle("Measured_{Toy} #it{H}_{T,2}/2 (GeV)");
  Matrix_GenReco_norm->GetYaxis()->SetRange(14,54);
  Matrix_GenReco_norm->GetYaxis()->SetTitleOffset(1.25);
  Matrix_GenReco_norm->GetYaxis()->SetTitleSize(0.06);
  Matrix_GenReco_norm->GetYaxis()->SetLabelSize(0.05);
  Matrix_GenReco_norm->GetYaxis()->SetLabelOffset(0.014);
  Matrix_GenReco_norm->GetZaxis()->SetRangeUser(0.0, 1.0);
  Matrix_GenReco_norm->GetZaxis()->SetTickLength(0.03);
  
  TPaletteAxis *palette = 
    (TPaletteAxis*)Matrix_GenReco_norm->GetListOfFunctions()->FindObject("palette");
  palette->SetX1NDC(.905);
  palette->SetX2NDC(0.935);
  palette->SetY1NDC(0.135);
  palette->SetY2NDC(0.934);
  c->Modified();
  
  double x1 = 0.50; // 0.62 fixed
  double x2 = x1 + 0.1;
  double y2 = 0.88; // fixed
  double y1 = y2 - 0.1;
   
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
  
  c->Update();
  
  tex = new TLatex(136,91.47,"150");
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
  
  tex1 = new TLatex(114.9,103.19,"110");
  tex1->SetTextFont(42);
  tex1->SetLineStyle(2);
  tex1->SetLineWidth(2);
  tex1->Draw();
  
  TLatex pt3;pt3.SetNDC();
  pt3.SetTextSize(0.05);
  pt3.DrawLatex(0.82,0.95,"#font[61]{8 TeV}");
  
  /*  TArrow *arrow = new TArrow(164,480.3109,164,263.9154,0.05,">");
      arrow->SetFillColor(2);
      arrow->SetFillStyle(1001);
      arrow->SetLineColor(2);
      arrow->SetLineWidth(2);
      arrow->Draw();
  
      TArrow *arrow = new TArrow(308,131,265,176,0.05,">");
      arrow->SetFillColor(2);
      arrow->SetFillStyle(1001);
      arrow->SetLineColor(2);
      arrow->SetLineWidth(2);
      arrow->Draw();
  */   
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Gifs/Normalized_Response_Matrix_NLO_2_range_column.gif");
  c->SaveAs("/home/anter/Desktop/Analysis_8_HT_2/Thesis_Plots/Pdfs/Normalized_Response_Matrix_NLO_2_range_column.pdf");
}
     
