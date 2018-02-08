{
//=========Macro generated from canvas: Canvas_1/Canvas_1
//=========  (Wed Feb  7 12:23:02 2018) by ROOT version5.34/32
   TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",627,311,538,323);
   Canvas_1->Range(0.06249995,-0.04244548,6.9375,0.07437695);
   Canvas_1->SetFillColor(0);
   Canvas_1->SetBorderMode(0);
   Canvas_1->SetBorderSize(2);
   Canvas_1->SetFrameBorderMode(0);
   Canvas_1->SetFrameBorderMode(0);
   
   TMultiGraph *multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("HO2M12 RM4                          Variation in Gain with Time;Run;Relative Variation in Gain");
   
   TGraph *graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineWidth(2);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.004818839952);
   graph->SetPoint(1,2,-0.001470700023);
   graph->SetPoint(2,3,-0.008980480023);
   graph->SetPoint(3,4,-0.002955599921);
   graph->SetPoint(4,5,-0.005227359943);
   graph->SetPoint(5,6,-0.001195379999);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,0.5,6.5);
   Graph_Graph1->SetMinimum(-0.00975899);
   Graph_Graph1->SetMaximum(-0.00041687);
   Graph_Graph1->SetDirectory(0);
   Graph_Graph1->SetStats(0);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#000099");
   Graph_Graph1->SetLineColor(ci);
   Graph_Graph1->GetXaxis()->SetLabelFont(42);
   Graph_Graph1->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetXaxis()->SetTitleFont(42);
   Graph_Graph1->GetYaxis()->SetLabelFont(42);
   Graph_Graph1->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetYaxis()->SetTitleFont(42);
   Graph_Graph1->GetZaxis()->SetLabelFont(42);
   Graph_Graph1->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph1->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph1);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#0000ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.00155537005);
   graph->SetPoint(1,2,0.008813249879);
   graph->SetPoint(2,3,0.006389869843);
   graph->SetPoint(3,4,0.001999319997);
   graph->SetPoint(4,5,-0.0002587180061);
   graph->SetPoint(5,6,0.004324719775);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0.5,6.5);
   Graph_Graph2->SetMinimum(-0.001165915);
   Graph_Graph2->SetMaximum(0.009720447);
   Graph_Graph2->SetDirectory(0);
   Graph_Graph2->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph2->SetLineColor(ci);
   Graph_Graph2->GetXaxis()->SetLabelFont(42);
   Graph_Graph2->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetXaxis()->SetTitleFont(42);
   Graph_Graph2->GetYaxis()->SetLabelFont(42);
   Graph_Graph2->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetYaxis()->SetTitleFont(42);
   Graph_Graph2->GetZaxis()->SetLabelFont(42);
   Graph_Graph2->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph2->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph2);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff00ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.009792650118);
   graph->SetPoint(1,2,0.004569400102);
   graph->SetPoint(2,3,0.005450360011);
   graph->SetPoint(3,4,0.0007879849873);
   graph->SetPoint(4,5,0.001292480039);
   graph->SetPoint(5,6,0.002827299992);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Graph",100,0.5,6.5);
   Graph_Graph3->SetMinimum(0);
   Graph_Graph3->SetMaximum(0.01069312);
   Graph_Graph3->SetDirectory(0);
   Graph_Graph3->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph3->SetLineColor(ci);
   Graph_Graph3->GetXaxis()->SetLabelFont(42);
   Graph_Graph3->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetXaxis()->SetTitleFont(42);
   Graph_Graph3->GetYaxis()->SetLabelFont(42);
   Graph_Graph3->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetYaxis()->SetTitleFont(42);
   Graph_Graph3->GetZaxis()->SetLabelFont(42);
   Graph_Graph3->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph3->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph3);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.0005728679826);
   graph->SetPoint(1,2,-0.002047660062);
   graph->SetPoint(2,3,-0.004995539784);
   graph->SetPoint(3,4,-0.003065519966);
   graph->SetPoint(4,5,-0.002831940074);
   graph->SetPoint(5,6,-0.003377530025);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Graph",100,0.5,6.5);
   Graph_Graph4->SetMinimum(-0.005552381);
   Graph_Graph4->SetMaximum(0.001129709);
   Graph_Graph4->SetDirectory(0);
   Graph_Graph4->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph4->SetLineColor(ci);
   Graph_Graph4->GetXaxis()->SetLabelFont(42);
   Graph_Graph4->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetXaxis()->SetTitleFont(42);
   Graph_Graph4->GetYaxis()->SetLabelFont(42);
   Graph_Graph4->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetYaxis()->SetTitleFont(42);
   Graph_Graph4->GetZaxis()->SetLabelFont(42);
   Graph_Graph4->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph4->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph4);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff0000");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.0006725119893);
   graph->SetPoint(1,2,-0.003051270032);
   graph->SetPoint(2,3,-0.001042059972);
   graph->SetPoint(3,4,-0.003552329959);
   graph->SetPoint(4,5,-0.00567972986);
   graph->SetPoint(5,6,-0.002014209982);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Graph",100,0.5,6.5);
   Graph_Graph5->SetMinimum(-0.006180452);
   Graph_Graph5->SetMaximum(-0.0001717902);
   Graph_Graph5->SetDirectory(0);
   Graph_Graph5->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph5->SetLineColor(ci);
   Graph_Graph5->GetXaxis()->SetLabelFont(42);
   Graph_Graph5->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetXaxis()->SetTitleFont(42);
   Graph_Graph5->GetYaxis()->SetLabelFont(42);
   Graph_Graph5->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetYaxis()->SetTitleFont(42);
   Graph_Graph5->GetZaxis()->SetLabelFont(42);
   Graph_Graph5->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph5->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph5);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#cccc00");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#cccc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.00156622997);
   graph->SetPoint(1,2,0.002417709911);
   graph->SetPoint(2,3,0.002773530083);
   graph->SetPoint(3,4,0.003642099909);
   graph->SetPoint(4,5,-0.00349760009);
   graph->SetPoint(5,6,0.001835030038);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Graph",100,0.5,6.5);
   Graph_Graph6->SetMinimum(-0.00421157);
   Graph_Graph6->SetMaximum(0.00435607);
   Graph_Graph6->SetDirectory(0);
   Graph_Graph6->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph6->SetLineColor(ci);
   Graph_Graph6->GetXaxis()->SetLabelFont(42);
   Graph_Graph6->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetXaxis()->SetTitleFont(42);
   Graph_Graph6->GetYaxis()->SetLabelFont(42);
   Graph_Graph6->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetYaxis()->SetTitleFont(42);
   Graph_Graph6->GetZaxis()->SetLabelFont(42);
   Graph_Graph6->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph6->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph6);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff6633");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff6633");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.002825770061);
   graph->SetPoint(1,2,0.005268590059);
   graph->SetPoint(2,3,0.001938289963);
   graph->SetPoint(3,4,-0.001395639963);
   graph->SetPoint(4,5,-0.000103448001);
   graph->SetPoint(5,6,0.0003303079866);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","Graph",100,0.5,6.5);
   Graph_Graph7->SetMinimum(-0.002062063);
   Graph_Graph7->SetMaximum(0.005935013);
   Graph_Graph7->SetDirectory(0);
   Graph_Graph7->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph7->SetLineColor(ci);
   Graph_Graph7->GetXaxis()->SetLabelFont(42);
   Graph_Graph7->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetXaxis()->SetTitleFont(42);
   Graph_Graph7->GetYaxis()->SetLabelFont(42);
   Graph_Graph7->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetYaxis()->SetTitleFont(42);
   Graph_Graph7->GetZaxis()->SetLabelFont(42);
   Graph_Graph7->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph7->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph7);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#00ccff");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#00ccff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.003202110063);
   graph->SetPoint(1,2,0.005033819936);
   graph->SetPoint(2,3,0.008796939626);
   graph->SetPoint(3,4,0.01042759977);
   graph->SetPoint(4,5,0.005892219953);
   graph->SetPoint(5,6,0.001657660003);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","Graph",100,0.5,6.5);
   Graph_Graph8->SetMinimum(0.000780666);
   Graph_Graph8->SetMaximum(0.01130459);
   Graph_Graph8->SetDirectory(0);
   Graph_Graph8->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph8->SetLineColor(ci);
   Graph_Graph8->GetXaxis()->SetLabelFont(42);
   Graph_Graph8->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetXaxis()->SetTitleFont(42);
   Graph_Graph8->GetYaxis()->SetLabelFont(42);
   Graph_Graph8->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetYaxis()->SetTitleFont(42);
   Graph_Graph8->GetZaxis()->SetLabelFont(42);
   Graph_Graph8->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph8->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph8);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#990099");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#990099");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.0003486410133);
   graph->SetPoint(1,2,0.00418209983);
   graph->SetPoint(2,3,-0.0005699250032);
   graph->SetPoint(3,4,-0.002028170042);
   graph->SetPoint(4,5,0.001599929994);
   graph->SetPoint(5,6,0.001372279949);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","Graph",100,0.5,6.5);
   Graph_Graph9->SetMinimum(-0.002649197);
   Graph_Graph9->SetMaximum(0.004803127);
   Graph_Graph9->SetDirectory(0);
   Graph_Graph9->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph9->SetLineColor(ci);
   Graph_Graph9->GetXaxis()->SetLabelFont(42);
   Graph_Graph9->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetXaxis()->SetTitleFont(42);
   Graph_Graph9->GetYaxis()->SetLabelFont(42);
   Graph_Graph9->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetYaxis()->SetTitleFont(42);
   Graph_Graph9->GetZaxis()->SetLabelFont(42);
   Graph_Graph9->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph9->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph9);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.003866099985);
   graph->SetPoint(1,2,-0.001830250025);
   graph->SetPoint(2,3,-0.0006977180019);
   graph->SetPoint(3,4,0.001272410038);
   graph->SetPoint(4,5,-0.0005460400134);
   graph->SetPoint(5,6,0.007750739809);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","Graph",100,0.5,6.5);
   Graph_Graph10->SetMinimum(-0.002788349);
   Graph_Graph10->SetMaximum(0.008708839);
   Graph_Graph10->SetDirectory(0);
   Graph_Graph10->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph10->SetLineColor(ci);
   Graph_Graph10->GetXaxis()->SetLabelFont(42);
   Graph_Graph10->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetXaxis()->SetTitleFont(42);
   Graph_Graph10->GetYaxis()->SetLabelFont(42);
   Graph_Graph10->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetYaxis()->SetTitleFont(42);
   Graph_Graph10->GetZaxis()->SetLabelFont(42);
   Graph_Graph10->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph10->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph10);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#0000ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.0152431);
   graph->SetPoint(1,2,0.01606960036);
   graph->SetPoint(2,3,0.0119882999);
   graph->SetPoint(3,4,0.009967260063);
   graph->SetPoint(4,5,0.005035609938);
   graph->SetPoint(5,6,0.009928070009);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","Graph",100,0.5,6.5);
   Graph_Graph11->SetMinimum(0.003932211);
   Graph_Graph11->SetMaximum(0.017173);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph11->SetLineColor(ci);
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph11);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff00ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.001881710021);
   graph->SetPoint(1,2,0.00423592981);
   graph->SetPoint(2,3,0.002986449981);
   graph->SetPoint(3,4,-0.002399140038);
   graph->SetPoint(4,5,-0.006480350159);
   graph->SetPoint(5,6,-0.005430509802);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","Graph",100,0.5,6.5);
   Graph_Graph12->SetMinimum(-0.007551978);
   Graph_Graph12->SetMaximum(0.005307558);
   Graph_Graph12->SetDirectory(0);
   Graph_Graph12->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph12->SetLineColor(ci);
   Graph_Graph12->GetXaxis()->SetLabelFont(42);
   Graph_Graph12->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetXaxis()->SetTitleFont(42);
   Graph_Graph12->GetYaxis()->SetLabelFont(42);
   Graph_Graph12->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetYaxis()->SetTitleFont(42);
   Graph_Graph12->GetZaxis()->SetLabelFont(42);
   Graph_Graph12->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph12->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph12);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.009972129948);
   graph->SetPoint(1,2,0.004161280114);
   graph->SetPoint(2,3,0.005606700201);
   graph->SetPoint(3,4,0.002484370023);
   graph->SetPoint(4,5,0.002812470077);
   graph->SetPoint(5,6,0.005028900225);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","Graph",100,0.5,6.5);
   Graph_Graph13->SetMinimum(0.001735594);
   Graph_Graph13->SetMaximum(0.01072091);
   Graph_Graph13->SetDirectory(0);
   Graph_Graph13->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph13->SetLineColor(ci);
   Graph_Graph13->GetXaxis()->SetLabelFont(42);
   Graph_Graph13->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetXaxis()->SetTitleFont(42);
   Graph_Graph13->GetYaxis()->SetLabelFont(42);
   Graph_Graph13->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetYaxis()->SetTitleFont(42);
   Graph_Graph13->GetZaxis()->SetLabelFont(42);
   Graph_Graph13->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph13->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph13);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff0000");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.007508260198);
   graph->SetPoint(1,2,0.001878750045);
   graph->SetPoint(2,3,0.003654540051);
   graph->SetPoint(3,4,0.000239696994);
   graph->SetPoint(4,5,-0.001016180031);
   graph->SetPoint(5,6,0.001142779947);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","Graph",100,0.5,6.5);
   Graph_Graph14->SetMinimum(-0.001868624);
   Graph_Graph14->SetMaximum(0.008360704);
   Graph_Graph14->SetDirectory(0);
   Graph_Graph14->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph14->SetLineColor(ci);
   Graph_Graph14->GetXaxis()->SetLabelFont(42);
   Graph_Graph14->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetXaxis()->SetTitleFont(42);
   Graph_Graph14->GetYaxis()->SetLabelFont(42);
   Graph_Graph14->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetYaxis()->SetTitleFont(42);
   Graph_Graph14->GetZaxis()->SetLabelFont(42);
   Graph_Graph14->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph14->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph14);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#cccc00");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#cccc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.003190400079);
   graph->SetPoint(1,2,0.008767710067);
   graph->SetPoint(2,3,0.002497859998);
   graph->SetPoint(3,4,0.0007580979727);
   graph->SetPoint(4,5,-0.003632480046);
   graph->SetPoint(5,6,0.002992050024);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","Graph",100,0.5,6.5);
   Graph_Graph15->SetMinimum(-0.004872499);
   Graph_Graph15->SetMaximum(0.01000773);
   Graph_Graph15->SetDirectory(0);
   Graph_Graph15->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph15->SetLineColor(ci);
   Graph_Graph15->GetXaxis()->SetLabelFont(42);
   Graph_Graph15->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetXaxis()->SetTitleFont(42);
   Graph_Graph15->GetYaxis()->SetLabelFont(42);
   Graph_Graph15->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetYaxis()->SetTitleFont(42);
   Graph_Graph15->GetZaxis()->SetLabelFont(42);
   Graph_Graph15->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph15->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph15);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff6633");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff6633");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.005249009933);
   graph->SetPoint(1,2,0.01142449956);
   graph->SetPoint(2,3,0.01837770082);
   graph->SetPoint(3,4,0.01795439981);
   graph->SetPoint(4,5,0.009410309605);
   graph->SetPoint(5,6,0.0004198499955);
   
   TH1F *Graph_Graph16 = new TH1F("Graph_Graph16","Graph",100,0.5,6.5);
   Graph_Graph16->SetMinimum(0);
   Graph_Graph16->SetMaximum(0.02017349);
   Graph_Graph16->SetDirectory(0);
   Graph_Graph16->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph16->SetLineColor(ci);
   Graph_Graph16->GetXaxis()->SetLabelFont(42);
   Graph_Graph16->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetXaxis()->SetTitleFont(42);
   Graph_Graph16->GetYaxis()->SetLabelFont(42);
   Graph_Graph16->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetYaxis()->SetTitleFont(42);
   Graph_Graph16->GetZaxis()->SetLabelFont(42);
   Graph_Graph16->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph16->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph16);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#00ccff");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#00ccff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.01306369994);
   graph->SetPoint(1,2,0.01195340045);
   graph->SetPoint(2,3,0.009610090405);
   graph->SetPoint(3,4,0.003334329929);
   graph->SetPoint(4,5,0.001690839999);
   graph->SetPoint(5,6,0.002383670071);
   
   TH1F *Graph_Graph17 = new TH1F("Graph_Graph17","Graph",100,0.5,6.5);
   Graph_Graph17->SetMinimum(0.000553554);
   Graph_Graph17->SetMaximum(0.01420099);
   Graph_Graph17->SetDirectory(0);
   Graph_Graph17->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph17->SetLineColor(ci);
   Graph_Graph17->GetXaxis()->SetLabelFont(42);
   Graph_Graph17->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetXaxis()->SetTitleFont(42);
   Graph_Graph17->GetYaxis()->SetLabelFont(42);
   Graph_Graph17->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetYaxis()->SetTitleFont(42);
   Graph_Graph17->GetZaxis()->SetLabelFont(42);
   Graph_Graph17->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph17->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph17);
   
   multigraph->Add(graph,"");
   
   graph = new TGraph(6);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#990099");
   graph->SetLineColor(ci);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#990099");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,-0.00854627043);
   graph->SetPoint(1,2,-0.01198759954);
   graph->SetPoint(2,3,-0.006668379996);
   graph->SetPoint(3,4,-0.003537959885);
   graph->SetPoint(4,5,0.002873939928);
   graph->SetPoint(5,6,0.001544130035);
   
   TH1F *Graph_Graph18 = new TH1F("Graph_Graph18","Graph",100,0.5,6.5);
   Graph_Graph18->SetMinimum(-0.01347375);
   Graph_Graph18->SetMaximum(0.004360094);
   Graph_Graph18->SetDirectory(0);
   Graph_Graph18->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph18->SetLineColor(ci);
   Graph_Graph18->GetXaxis()->SetLabelFont(42);
   Graph_Graph18->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetXaxis()->SetTitleFont(42);
   Graph_Graph18->GetYaxis()->SetLabelFont(42);
   Graph_Graph18->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetYaxis()->SetTitleFont(42);
   Graph_Graph18->GetZaxis()->SetLabelFont(42);
   Graph_Graph18->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph18->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph18->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph18);
   
   multigraph->Add(graph,"");
   multigraph->Draw("alp");
   multigraph->GetXaxis()->SetTitle("Run");
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetLabelSize(0.035);
   multigraph->GetXaxis()->SetTitleSize(0.035);
   multigraph->GetXaxis()->SetTitleFont(42);
   multigraph->GetYaxis()->SetTitle("Relative Variation in Gain");
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelSize(0.035);
   multigraph->GetYaxis()->SetTitleSize(0.035);
   multigraph->GetYaxis()->SetTitleOffset(1.12);
   multigraph->GetYaxis()->SetTitleFont(42);
   
   TPaveText *pt = new TPaveText(0.15,0.94,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("HO2M12 RM4                          Variation in Gain with Time");
   pt->Draw();
   Canvas_1->Modified();
   Canvas_1->cd();
   Canvas_1->SetSelected(Canvas_1);
}
