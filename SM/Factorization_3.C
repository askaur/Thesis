double Text (TLatex *tex)
{
 tex->SetTextFont(132);
 tex->SetTextSize(0.06);
 tex->SetLineWidth(2);
 tex->Draw();
}

double Quark (TArrow *arrow)
{
 arrow->SetFillColor(1);
 arrow->SetFillStyle(1001);
 arrow->SetLineWidth(2);
 arrow->SetAngle(45);
 arrow->Draw();
}
void Factorization_3()
{
   //Draw Feynman diagrams
   // To see the output of this macro, click begin_html <a href="gif/feynman.gif">here</a>. end_html
   //Author: Otto Schaile
   
   Int_t linsav = gStyle->GetLineWidth();
   gStyle->SetLineWidth(2);
   TArrow *arrow; TLatex *tex; TEllipse *circle;
   TCanvas *c1 = new TCanvas("c1", "c1", 400, 400);
   c1->Range(0, 0, 1, 1);
   
   circle = new TEllipse(0.4, 0.67, 0.08, 0.08);
   circle->SetLineWidth(2);
   circle->Draw();
   
   circle = new TEllipse(0.4, 0.33, 0.08, 0.08);
   circle->SetLineWidth(2);
   circle->Draw();
   
   tex = new TLatex(0.335,0.65,"f_{i}(x#lower[0.65]{#scale[0.6]{1}},#mu#lower[0.65]{#scale[0.6]{f}})");
   tex->SetTextColor(4);
   tex->SetTextFont(132);
   tex->SetTextSize(0.05);
   tex->SetLineWidth(2);
   tex->Draw();
   
   tex = new TLatex(0.335,0.31,"f_{j}(x#lower[0.65]{#scale[0.6]{2}},#mu#lower[0.65]{#scale[0.6]{f}})");
   tex->SetTextColor(4);
   tex->SetTextFont(132);
   tex->SetTextSize(0.05);
   tex->SetLineWidth(2);
   tex->Draw();
   
   arrow = new TArrow(0.1256281,0.8770053,0.3668342,0.7459893,0.03,"-|>-");      //1
   Quark(arrow);
   arrow = new TArrow(0.1633166,0.815508,0.3366834,0.7245989,0.03,"-|>-");   //2
   Quark(arrow);
   arrow = new TArrow(0.1934673,0.7593583,0.321608,0.6951872,0.03,"-|>-");  //3 
   Quark(arrow);
   
   arrow = new TArrow(0.1934673,0.2647059,0.321608,0.328877,0.03,"-|>-");   //4
   Quark(arrow);
   //arrow = new TArrow(0.1557789,0.2032086,0.3291457,0.2941176,0.03,"-|>-");   //5
   arrow = new TArrow(0.1557789,0.199,0.3291457,0.2941176,0.03,"-|>-");   //5
   Quark(arrow);
   //arrow = new TArrow(0.138191,0.1524064,0.3547839,0.2673797,0.03,"-|>-");   //6
   arrow = new TArrow(0.1130653,0.131016,0.3547839,0.2673797,0.03,"-|>-");   //6
   Quark(arrow);
   
   TLine *line = new TLine(0.75,0.45,0.75,0.55);
   line->SetLineWidth(2);
   line->Draw();
   
   TLine *line = new TLine(0.85,0.45,0.85,0.55);
   line->SetLineWidth(2);
   line->Draw();
   
   TLine *line = new TLine(0.75,0.45,0.85,0.45);
   line->SetLineWidth(2);
   line->Draw();
   
   TLine *line = new TLine(0.75,0.55,0.85,0.55);
   line->SetLineWidth(2);
   line->Draw();
   
   tex = new TLatex(0.77,0.49,"#hat{#sigma}_{ij}");
   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#cc0000");
   tex->SetTextColor(ci);
   tex->SetTextColor(kGreen+2);
   Text(tex);
   
   arrow = new TArrow(0.4773869,0.6550802,0.7462312,0.5187166,0.03,"-|>-");  //7
   Quark(arrow);
  
   arrow = new TArrow(0.4773869,0.3609626,0.7487437,0.4786096,0.03,"-|>-");  //8
   Quark(arrow);
   
   arrow = new TArrow(0.8467337,0.5508021,0.9296482,0.6390374,0.03,"-|>-"); //9
   Quark(arrow);
   
   arrow = new TArrow(0.8467337,0.4545455,0.9296482,0.3636364,0.03,"-|>-"); //10
   Quark(arrow);
  
   tex = new TLatex(0.16,0.91,"p#lower[0.65]{#scale[0.6]{1}}(P#lower[0.65]{#scale[0.6]{1}})");
   Text(tex);
   
   tex = new TLatex(0.16,0.10,"p#lower[0.65]{#scale[0.6]{2}}(P#lower[0.65]{#scale[0.6]{2}})");
   Text(tex);
   
   tex = new TLatex(0.53,0.66,"x#lower[0.65]{#scale[0.6]{1}}p#lower[0.65]{#scale[0.6]{1}}");
   Text(tex);
   
   tex = new TLatex(0.53,0.32,"x#lower[0.65]{#scale[0.6]{2}}p#lower[0.65]{#scale[0.6]{2}}");
   Text(tex);
   
   c1->Update();
   gStyle->SetLineWidth(linsav);
   c1->SaveAs("../Figures/Factorization_3.pdf");
}
