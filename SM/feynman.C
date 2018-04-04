double Text (TLatex *tex)
{
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
  tex->SetLineWidth(2);
  tex->Draw();
}

double TextB (TLatex *tex)
{
  tex->SetTextFont(42);
  tex->SetTextSize(0.05);
  tex->SetLineWidth(2);
  tex->Draw();
}

double Quark (TArrow *arrow)
{
 arrow->SetFillColor(1);
 arrow->SetFillStyle(1001);
 arrow->SetLineWidth(2);
 arrow->SetAngle(56);
 arrow->Draw();
}
void feynman()
{
   //Draw Feynman diagrams
   // To see the output of this macro, click begin_html <a href="gif/feynman.gif">here</a>. end_html
   //Author: Otto Schaile
   
   //TCanvas *c1 = new TCanvas("c1", "A canvas",51,52,596,493);
   TCanvas *c1 = new TCanvas("c1", "A canvas",51,80,1313,667);
   c1->Range(0, 0,140,60);
   Int_t linsav = gStyle->GetLineWidth();
   gStyle->SetLineWidth(2);
   TArrow *quark; TCurlyLine *gluon; TMarker *vertex; TLatex *tex;  
   
   // First
   double xd = 0.0, yd = 4.0;
    
   quark = new TArrow(3 + xd, 53 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(3 + xd, 43 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   
   quark = new TArrow(28 + xd, 48 + yd, 34 + xd, 53 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(28 + xd, 48 + yd, 34 + xd, 43 + yd, 0.02,"");
   Quark(quark);
   
   // Straight line
   quark = new TArrow(2 + xd, 41 + yd, 2 + xd, 54 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(35 + xd, 41 + yd, 35 + xd, 54 + yd, 0.02,"");
   Quark(quark);
      
   gluon = new TCurlyLine(9 + xd, 48 + yd, 28 + xd, 48 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(9 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(28 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(9 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(27 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(36 + xd,53 + yd,"2");
   Text(tex);
   tex = new TLatex(36.5 + xd, 47 + yd,"~ g_{s}^{4} ~ #alpha^{2}_{s}");
   TextB(tex);
   tex = new TLatex(16 + xd,40 + yd,"LO");
   Text(tex);
   
   // Second
   double xd = 84.0; yd = 4.0;
    
   gluon = new TCurlyLine(3 + xd, 53 + yd, 9 + xd, 48 + yd, 0.016, 0.007);
   gluon->Draw();
   gluon = new TCurlyLine(3 + xd, 43 + yd, 9 + xd, 48 + yd, 0.016, 0.007);
   gluon->Draw();
   gluon = new TCurlyLine(28 + xd, 48 + yd, 34 + xd, 53 + yd, 0.016, 0.007);
   gluon->Draw();
   gluon = new TCurlyLine(28 + xd, 48 + yd, 34 + xd, 43 + yd, 0.016, 0.007);
   gluon->Draw();
    
   // Straight line
   quark = new TArrow(2 + xd, 41 + yd, 2 + xd, 54 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(35 + xd, 41 + yd, 35 + xd, 54 + yd, 0.02,"");
   Quark(quark);
      
   gluon = new TCurlyLine(9 + xd, 48 + yd, 28 + xd, 48 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(9 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(28 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(9 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(27 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(36 + xd,53 + yd,"2");
   Text(tex);
   tex = new TLatex(36.5 + xd, 47 + yd,"~ g_{s}^{4} ~ #alpha^{2}_{s}");
   TextB(tex);
   tex = new TLatex(16 + xd,40 + yd,"LO");
   Text(tex);
   
   // Third
   double xd = 0.0, yd = -19.0;
    
   quark = new TArrow(3 + xd, 53 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(3 + xd, 43 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   
   quark = new TArrow(28 + xd, 48 + yd, 34 + xd, 53 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(28 + xd, 48 + yd, 34 + xd, 43 + yd, 0.02,"");
   Quark(quark);
   
   // Straight line
   quark = new TArrow(2 + xd, 41 + yd, 2 + xd, 54 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(35 + xd, 41 + yd, 35 + xd, 54 + yd, 0.02,"");
   Quark(quark);
      
   gluon = new TCurlyLine(9 + xd, 48 + yd, 28 + xd, 48 + yd, 0.016, 0.007);
   gluon->Draw();
   
   gluon = new TCurlyLine(30.895,26.7,33,30,0.008,0.005);
   //gluon->SetLineColor(4);
   gluon->Draw();
   
   vertex = new TMarker(9 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(28 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(41.89482-11, 28.54688-2, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(9 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(27 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(41.36895-11,26.4375-2,"g_{s}");
   Text(tex);
   tex = new TLatex(36 + xd,53 + yd,"2");
   Text(tex);
   tex = new TLatex(36.5 + xd, 47 + yd,"~ g_{s}^{6} ~ #alpha^{3}_{s}");
   TextB(tex);
   tex = new TLatex(16 + xd,40 + yd,"NLO");
   Text(tex);
   
   // Fourth
   double xd = 91.0, yd = -19.0;
   
   quark = new TArrow(6 + xd - 36, 53 + yd, 12 + xd - 38, 48 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(6 + xd - 38, 43 + yd, 12 + xd - 38, 48 + yd, 0.02,"");
   Quark(quark);
   
   quark = new TArrow(28 + xd - 38, 48 + yd, 34 + xd - 38, 53 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(28 + xd - 38, 48 + yd, 34 + xd - 38, 43 + yd, 0.02,"");
   Quark(quark);
     
   gluon = new TCurlyLine(12 + xd - 38, 48 + yd, 28 + xd - 38, 48 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(12 + xd - 38, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(28 + xd - 38, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(12 + xd - 38,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(27 + xd - 38,45.5 + yd,"g_{s}");
   Text(tex);
    
   quark = new TArrow(3 + xd, 53 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(3 + xd, 43 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   
   quark = new TArrow(32 + xd, 48 + yd, 38 + xd, 53 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(32 + xd, 48 + yd, 38 + xd, 43 + yd, 0.02,"");
   Quark(quark);
     
   gluon = new TCurlyLine(9 + xd, 48 + yd, 19.5 + xd, 48 + yd, 0.012, 0.007);
   gluon->Draw();
   
   gluon = new TCurlyLine(23 + xd, 48 + yd, 32 + xd, 48 + yd, 0.012, 0.007);
   gluon->Draw();
   
   TCurlyArc *curlyarc = new TCurlyArc(93.56+19,29,2.032045,344,180,0.008,0.005);
   //curlyarc->SetLineColor(4);
   curlyarc->SetLineWidth(2);
   curlyarc->Draw();
   
   curlyarc = new TCurlyArc(93.5+19,29,2.141623,167,360,0.008,0.005);
   //curlyarc->SetLineColor(4);
   curlyarc->SetLineWidth(2);
   curlyarc->Draw();
   
   vertex = new TMarker(9 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(32 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(19 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(23.5 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(9 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(31 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(17.5 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(24 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(36.5 + xd, 47 + yd,"~ g_{s}^{6} ~ #alpha^{3}_{s}");
   TextB(tex);
   tex = new TLatex(18 + xd-20,40 + yd,"NLO");
   Text(tex);
   
   tex = new TLatex(18 + xd-20,27.46,"#otimes");
   tex->SetTextFont(42);
   tex->SetTextSize(0.08);
   tex->SetLineWidth(2);
   tex->Draw();
   
   // Fifth
   double xd = 44.0; yd = -40.0;
    
   quark = new TArrow(3 + xd, 53 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(3 + xd, 43 + yd, 9 + xd, 48 + yd, 0.02,"");
   Quark(quark);
   
   quark = new TArrow(32 + xd, 48 + yd, 38 + xd, 53 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(32 + xd, 48 + yd, 38 + xd, 43 + yd, 0.02,"");
   Quark(quark);
    
   // Straight line
   quark = new TArrow(2 + xd, 41 + yd, 2 + xd, 54 + yd, 0.02,"");
   Quark(quark);
   quark = new TArrow(40 + xd, 41 + yd, 40 + xd, 54 + yd, 0.02,"");
   Quark(quark);
      
   gluon = new TCurlyLine(9 + xd, 48 + yd, 19.5 + xd, 48 + yd, 0.012, 0.007);
   gluon->Draw();
   
   gluon = new TCurlyLine(23 + xd, 48 + yd, 32 + xd, 48 + yd, 0.012, 0.007);
   gluon->Draw();
   
   TCurlyArc *curlyarc = new TCurlyArc(57+8,8.063,2.032045,344,180,0.008,0.005);
   //curlyarc->SetLineColor(4);
   curlyarc->SetLineWidth(2);
   curlyarc->Draw();
   
   curlyarc = new TCurlyArc(57.0826+8,8.0625,2.141623,167,360,0.008,0.005);
   //curlyarc->SetLineColor(4);
   curlyarc->SetLineWidth(2);
   curlyarc->Draw();
   
   vertex = new TMarker(9 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(32 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(19 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(23.5 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(9 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(31 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(17.5 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(24 + xd,45.5 + yd,"g_{s}");
   Text(tex);
   tex = new TLatex(41 + xd,54 + yd,"2");
   Text(tex);
   tex = new TLatex(43 + xd, 47 + yd,"~ g_{s}^{8} ~ #alpha^{4}_{s}");
   TextB(tex);
   tex = new TLatex(16 + xd,40 + yd,"NNLO");
   Text(tex);
   
   c1->Update();
   gStyle->SetLineWidth(linsav);
   c1->SaveAs("../Figures/Orders.pdf");
   c1->SaveAs("Orders.gif");
}
