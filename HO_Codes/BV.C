{
//=========Macro generated from canvas: Canvas_1/Canvas_1
//=========  (Wed Feb  7 12:18:40 2018) by ROOT version5.34/32
   TCanvas *Canvas_1 = new TCanvas("Canvas_1", "Canvas_1",808,239,538,323);
   Canvas_1->Range(0.4375,-0.1411604,4.5625,0.1411604);
   Canvas_1->SetFillColor(0);
   Canvas_1->SetBorderMode(0);
   Canvas_1->SetBorderSize(2);
   Canvas_1->SetFrameBorderMode(0);
   Canvas_1->SetFrameBorderMode(0);
   
   TMultiGraph *multigraph = new TMultiGraph();
   multigraph->SetName("");
   multigraph->SetTitle("");//HO1M08 RM1                          Variation in BV with Time;Run; Variation in BV");
   
   TGraph *graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineWidth(2);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.006099999882);
   graph->SetPoint(1,2,-0.01710000075);
   graph->SetPoint(2,3,-0.00540000014);
   graph->SetPoint(3,4,-0.0006000000285);
   
   TH1F *Graph_Graph1 = new TH1F("Graph_Graph1","Graph",100,0.7,4.3);
   Graph_Graph1->SetMinimum(-0.01875);
   Graph_Graph1->SetMaximum(0.00105);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#0000ff");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#0000ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.01389999967);
   graph->SetPoint(1,2,0.01750000007);
   graph->SetPoint(2,3,0.01640000008);
   graph->SetPoint(3,4,0.02290000021);
   
   TH1F *Graph_Graph2 = new TH1F("Graph_Graph2","Graph",100,0.7,4.3);
   Graph_Graph2->SetMinimum(0.013);
   Graph_Graph2->SetMaximum(0.0238);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff00ff");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff00ff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.006599999964);
   graph->SetPoint(1,2,0.008700000122);
   graph->SetPoint(2,3,0.0005000000237);
   graph->SetPoint(3,4,-0.0027999999);
   
   TH1F *Graph_Graph3 = new TH1F("Graph_Graph3","Graph",100,0.7,4.3);
   Graph_Graph3->SetMinimum(-0.00395);
   Graph_Graph3->SetMaximum(0.00985);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#00cc00");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#00cc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.01070000045);
   graph->SetPoint(1,2,-0.00400000019);
   graph->SetPoint(2,3,-0.009600000456);
   graph->SetPoint(3,4,-0.01070000045);
   
   TH1F *Graph_Graph4 = new TH1F("Graph_Graph4","Graph",100,0.7,4.3);
   Graph_Graph4->SetMinimum(-0.01137);
   Graph_Graph4->SetMaximum(-0.00333);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff0000");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff0000");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.03150000051);
   graph->SetPoint(1,2,0.01920000091);
   graph->SetPoint(2,3,0.03030000068);
   graph->SetPoint(3,4,0.02270000055);
   
   TH1F *Graph_Graph5 = new TH1F("Graph_Graph5","Graph",100,0.7,4.3);
   Graph_Graph5->SetMinimum(0.01797);
   Graph_Graph5->SetMaximum(0.03273);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#cccc00");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#cccc00");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.001300000004);
   graph->SetPoint(1,2,0.003899999894);
   graph->SetPoint(2,3,0.0005000000237);
   graph->SetPoint(3,4,0.00139999995);
   
   TH1F *Graph_Graph6 = new TH1F("Graph_Graph6","Graph",100,0.7,4.3);
   Graph_Graph6->SetMinimum(0.00016);
   Graph_Graph6->SetMaximum(0.00424);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#ff6633");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#ff6633");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,0.01040000003);
   graph->SetPoint(1,2,-0.02730000019);
   graph->SetPoint(2,3,0.004699999932);
   graph->SetPoint(3,4,-0.01169999968);
   
   TH1F *Graph_Graph7 = new TH1F("Graph_Graph7","Graph",100,0.7,4.3);
   Graph_Graph7->SetMinimum(-0.03107);
   Graph_Graph7->SetMaximum(0.01417);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);

   ci = TColor::GetColor("#00ccff");
   graph->SetLineColor(ci);
   graph->SetLineWidth(2);

   ci = TColor::GetColor("#00ccff");
   graph->SetMarkerColor(ci);
   graph->SetMarkerStyle(24);
   graph->SetPoint(0,1,-0.003299999982);
   graph->SetPoint(1,2,-0.01080000028);
   graph->SetPoint(2,3,-0.03050000034);
   graph->SetPoint(3,4,0.009200000204);
   
   TH1F *Graph_Graph8 = new TH1F("Graph_Graph8","Graph",100,0.7,4.3);
   Graph_Graph8->SetMinimum(-0.03447);
   Graph_Graph8->SetMaximum(0.01317);
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
   
   graph = new TGraph(4);
   graph->SetName("Graph");
   graph->SetTitle("Graph");
   graph->SetFillColor(1);
   graph->SetLineStyle(2);
   graph->SetLineWidth(2);
   graph->SetMarkerStyle(26);
   graph->SetPoint(0,1,0.001700000023);
   graph->SetPoint(1,2,0.02390000038);
   graph->SetPoint(2,3,0.01590000093);
   graph->SetPoint(3,4,0.003599999938);
   
   TH1F *Graph_Graph9 = new TH1F("Graph_Graph9","Graph",100,0.7,4.3);
   Graph_Graph9->SetMinimum(0);
   Graph_Graph9->SetMaximum(0.02612);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,-0.0007999999798);
   graph->SetPoint(1,2,-0.02449999936);
   graph->SetPoint(2,3,-0.02480000071);
   graph->SetPoint(3,4,-0.03489999846);
   
   TH1F *Graph_Graph10 = new TH1F("Graph_Graph10","Graph",100,0.7,4.3);
   Graph_Graph10->SetMinimum(-0.03831);
   Graph_Graph10->SetMaximum(0.00261);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,0.04050000012);
   graph->SetPoint(1,2,0.02710000053);
   graph->SetPoint(2,3,0.02559999935);
   graph->SetPoint(3,4,0.01579999924);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","Graph",100,0.7,4.3);
   Graph_Graph11->SetMinimum(0.01333);
   Graph_Graph11->SetMaximum(0.04297);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,0.007499999832);
   graph->SetPoint(1,2,-0.001200000057);
   graph->SetPoint(2,3,-0.004900000058);
   graph->SetPoint(3,4,0.00159999996);
   
   TH1F *Graph_Graph12 = new TH1F("Graph_Graph12","Graph",100,0.7,4.3);
   Graph_Graph12->SetMinimum(-0.00614);
   Graph_Graph12->SetMaximum(0.00874);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,-0.001700000023);
   graph->SetPoint(1,2,0.006699999794);
   graph->SetPoint(2,3,0.00270000007);
   graph->SetPoint(3,4,-0.000699999975);
   
   TH1F *Graph_Graph13 = new TH1F("Graph_Graph13","Graph",100,0.7,4.3);
   Graph_Graph13->SetMinimum(-0.00254);
   Graph_Graph13->SetMaximum(0.00754);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,0.001799999969);
   graph->SetPoint(1,2,-0.001700000023);
   graph->SetPoint(2,3,0.0008999999845);
   graph->SetPoint(3,4,0.005200000014);
   
   TH1F *Graph_Graph14 = new TH1F("Graph_Graph14","Graph",100,0.7,4.3);
   Graph_Graph14->SetMinimum(-0.00239);
   Graph_Graph14->SetMaximum(0.00589);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,-0.005200000014);
   graph->SetPoint(1,2,-0.01460000034);
   graph->SetPoint(2,3,-0.0178999994);
   graph->SetPoint(3,4,-0.01219999976);
   
   TH1F *Graph_Graph15 = new TH1F("Graph_Graph15","Graph",100,0.7,4.3);
   Graph_Graph15->SetMinimum(-0.01917);
   Graph_Graph15->SetMaximum(-0.00393);
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
   
   graph = new TGraph(4);
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
   graph->SetPoint(0,1,0.01649999991);
   graph->SetPoint(1,2,0.00419999985);
   graph->SetPoint(2,3,0.01830000058);
   graph->SetPoint(3,4,0.01569999941);
   
   TH1F *Graph_Graph16 = new TH1F("Graph_Graph16","Graph",100,0.7,4.3);
   Graph_Graph16->SetMinimum(0.00279);
   Graph_Graph16->SetMaximum(0.01971);
   Graph_Graph16->SetDirectory(0);
   Graph_Graph16->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph16->SetLineColor(ci);
   Graph_Graph16->SetMarkerColor(ci);
   Graph_Graph16->SetMarkerStyle(26);
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
   multigraph->Draw("alp");
   multigraph->GetXaxis()->SetTitle("Run");
   multigraph->GetXaxis()->SetLabelFont(42);
   multigraph->GetXaxis()->SetLabelSize(0.035);
   multigraph->GetXaxis()->SetTitleSize(0.035);
   multigraph->GetXaxis()->SetTitleFont(42);
   multigraph->GetYaxis()->SetTitle(" Variation in Breakdown Volatage (BV)");
   multigraph->GetYaxis()->SetLabelFont(42);
   multigraph->GetYaxis()->SetLabelSize(0.035);
   multigraph->GetYaxis()->SetTitleSize(0.035);
   multigraph->GetYaxis()->SetTitleOffset(1.12);
   multigraph->GetYaxis()->SetTitleFont(42);
   
   double x1 = 0.43; // 0.62 fixed
  double x2 = x1 + 0.39;
  double y2 = 0.90; // fixed
  double y1 = y2 - 0.32;
  
  TLegend *legend1=new TLegend(x1,y1,x2,y2);
  legend1->SetTextSize(0.05);
  legend1->SetTextFont(42);
  legend1->SetLineStyle(1);
  legend1->SetLineWidth(1);
  legend1->SetFillColor(0);
  legend1->SetBorderSize(0);
  legend1->SetLineColor(1);
  legend1->AddEntry((TObject*)0,"n_{j} #geq 2"," ");
  legend1->AddEntry(Graph_Graph16,"Data");
  legend1->Draw();
   
   TPaveText *pt = new TPaveText(0.15,0.94,0.85,0.995,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetTextFont(42);
   TText *text = pt->AddText("HO1M08 RM1");//                          Variation in BV with Time");
   pt->Draw();
   Canvas_1->Modified();
   Canvas_1->cd();
   Canvas_1->SetSelected(Canvas_1);
}
