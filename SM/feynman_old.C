double Text (TLatex *tex)
{
  tex->SetTextFont(42);
  tex->SetTextSize(0.04);
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
   
   TCanvas *c1 = new TCanvas("c1", "A canvas",51,52,596,493);
   c1->Range(0, 0,140,60);
   Int_t linsav = gStyle->GetLineWidth();
   gStyle->SetLineWidth(2);
   TArrow *quark; TCurlyLine *gluon; TMarker *vertex; TLatex *tex;  
   
   // Top Left
   double xd = 0.0, yd = 0.0;
    
   quark = new TArrow(4 + xd, 48 + yd, 30 + xd, 48 + yd, 0.02,"->-");
   Quark(quark);
   quark = new TArrow(30 + xd, 48 + yd, 58 + xd, 59 + yd, 0.02,"->-");
   Quark(quark);
   
   gluon = new TCurlyLine(30 + xd, 48 + yd, 58 + xd, 39 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(30 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(4 + xd,45.61028 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,57 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,41 + yd,"g");
   Text(tex);
   tex = new TLatex(26 + xd,45.5 + yd,"#alpha_{s}");
   Text(tex);
   tex = new TLatex(23 + xd,57 + yd,"LO");
   Text(tex);
   tex = new TLatex(4 + xd,34 + yd,"#it{X} = c_{0} + c_{1}#alpha^{1}_{s}");
   Text(tex);
   
   // Top Right
   double xd = 75.0; yd = 0.0;
    
   quark = new TArrow(4 + xd, 48 + yd, 30 + xd, 48 + yd, 0.02,"->-");
   Quark(quark);
   quark = new TArrow(30 + xd, 48 + yd, 58 + xd, 59 + yd, 0.02,"->-");
   Quark(quark);
   
   gluon = new TCurlyLine(30 + xd, 48 + yd, 58 + xd, 39 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(30 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(4 + xd,45.61028 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,57 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,41 + yd,"g");
   Text(tex);
   tex = new TLatex(26 + xd,45.5 + yd,"#alpha_{s}");
   Text(tex);
   tex = new TLatex(23 + xd,57 + yd,"NLO");
   Text(tex);
   tex = new TLatex(4 + xd,34 + yd,"#it{X} = c_{0} + c_{1}#alpha^{1}_{s} + c_{2}#alpha^{2}_{s}");
   Text(tex);
   
   gluon = new TCurlyLine(110,50,128,52,0.015, 0.007); //30 + xd + 9, 48 + yd + 3, 64 + xd - 5, 37 + yd + 13,
   gluon->SetLineColor(4);
   gluon->Draw();
   
   vertex = new TMarker(110, 50, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(104.6465,51.1349,"#alpha_{s}");
   Text(tex);
   
   //Bottom Left
   double xd = 0.0; yd = -33.0;
   double x2d = 75.0; 
   quark = new TArrow(4 + xd, 48 + yd, 30 + xd, 48 + yd, 0.02,"->-");
   Quark(quark);
   quark = new TArrow(30 + xd, 48 + yd, 58 + xd, 59 + yd, 0.02,"->-");
   Quark(quark);
   
   gluon = new TCurlyLine(30 + xd, 48 + yd, 58 + xd, 39 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(30 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(4 + xd,45.61028 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,57 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,41 + yd,"g");
   Text(tex);
   tex = new TLatex(26 + xd,45.5 + yd,"#alpha_{s}");
   Text(tex);
   tex = new TLatex(23 + xd,57 + yd,"NNLO");
   Text(tex);
   tex = new TLatex(4 + xd,34 + yd,"#it{X} = c_{0} + c_{1}#alpha^{1}_{s} + c_{2}#alpha^{2}_{s} + c_{3}#alpha^{3}_{s}");
   Text(tex);
   
   gluon = new TCurlyLine(110 - x2d, 50 + yd, 128 - x2d, 52 + yd,0.015, 0.007); //30 + xd + 9, 48 + yd + 3, 64 + xd - 5, 37 + yd + 13,
   gluon->SetLineColor(4);
   gluon->Draw();
   
   vertex = new TMarker(110 - x2d , 50 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   gluon = new TCurlyLine(53.0303,28.394,46.43098,21.4561,0.017,0.007); //30 + xd + 9, 48 + yd + 3, 64 + xd - 5, 37 + yd + 13,
   gluon->SetLineColor(4);
   gluon->Draw();
   
   vertex = new TMarker(46.43098, 21.4561, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(104.6465 - x2d,51.1349 + yd,"#alpha_{s}");
   Text(tex);
   tex = new TLatex(40.30303,22.22698,"#alpha_{s}");
   Text(tex);
   
   // Bottom Right
   double xd = 75.0; yd = -33.0;
    
   quark = new TArrow(4 + xd, 48 + yd, 30 + xd, 48 + yd, 0.02,"->-");
   Quark(quark);
   quark = new TArrow(30 + xd, 48 + yd, 58 + xd, 59 + yd, 0.02,"->-");
   Quark(quark);
   
   //gluon = new TCurlyLine(30 + xd, 48 + yd, 58 + xd, 39 + yd, 0.016, 0.007);
   gluon->Draw();
   
   vertex = new TMarker(30 + xd, 48 + yd, 20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   tex = new TLatex(4 + xd,45.61028 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,57 + yd,"q");
   Text(tex);
   tex = new TLatex(58 + xd,41 + yd,"g");
   Text(tex);
   tex = new TLatex(26 + xd,45.5 + yd,"#alpha_{s}");
   Text(tex);
   tex = new TLatex(23 + xd,57 + yd,"NNLO");
   Text(tex);
   tex = new TLatex(4 + xd,34 + yd,"#it{X} = c_{0} + c_{3}#alpha^{3}_{s}");
   Text(tex);
   
   TCurlyArc *curlyarc = new TCurlyArc(123.973,6,8,0,129,0.014,0.007);
   curlyarc->SetLineColor(4);
   curlyarc->SetLineWidth(2);
   curlyarc->Draw();
   
   curlyarc = new TCurlyArc(127.754,9.962,7.988,167,315,0.014,0.007);
   curlyarc->SetLineColor(4);
   curlyarc->SetLineWidth(2);
   curlyarc->Draw();
   
   gluon = new TCurlyLine(104.8822,14.77516,119.9663,10.27837,0.016,0.007);
   //gluon->SetLineColor(2);
   gluon->Draw();
   
   gluon = new TCurlyLine(131.476,7,138.8215,4.047109,0.016,0.007);
   //gluon->SetLineColor(2);
   gluon->Draw();
   
   tex = new TLatex(130.101,3.468951,"#alpha_{s}");
   Text(tex);
   tex = new TLatex(113.367,9.250535,"#alpha_{s}");
   Text(tex);
   
   vertex = new TMarker(119.2593,9.9,20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   vertex = new TMarker(132.4579,6.038544,20);
   vertex->SetMarkerStyle(20);
   vertex->Draw();
   
   c1->Update();
   gStyle->SetLineWidth(linsav);
   c1->SaveAs("../Figures/Orders.pdf");
}
