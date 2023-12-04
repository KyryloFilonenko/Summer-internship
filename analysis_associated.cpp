#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_associated()
{

	gROOT->SetBatch(kTRUE);
	
	int run_number = 974;	

	int num;
	cout << "How many folders do you want to analyse?" << endl;
	cin >> num;
	if(num == -1)
	{
		num = END + 1;
	}

	int num_ev = 0;
	double sqr;
	TString save_picture;
	
	int counter[16];
	/*double low_measure[16] = {0.0, 0.02, 0.12, 0.197, 0.205, 0.237, 0.245, 0.280, 0.290, 0.337, 0.345, 0.405, 0.410, 0.485, 0.500, 0.580};
	double high_measure[16] = {0.02, 0.12, 0.18, 0.203, 0.230, 0.243, 0.280, 0.290, 0.335, 0.343, 0.403, 0.410, 0.485, 0.490, 0.580, 0.590};
	string low_dir[16] = {"0.0", "0.02", "0.12", "0.197", "0.205", "0.237", "0.245", "0.280", "0.290", "0.337", "0.345", "0.405", "0.410", "0.485", "0.500", "0.580"};
	string high_dir[16] = {"0.02", "0.12", "0.18", "0.203", "0.230", "0.243", "0.280", "0.290", "0.335", "0.343", "0.403", "0.410", "0.485", "0.490", "0.580", "0.590"};*/
	
	int max_count = 20;
	for(int i = 0; i < 16; i++)
	{
		counter[i] = 0;
	}
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];
		
		TCanvas* canv_metric = new TCanvas("canv_metric", "canv_metric", 1200, 600);
		TH1D* metric = new TH1D("metric", "d-r metric", 53, -2.0, 50.0);
		
		TCanvas* canv_metric_small = new TCanvas("canv_metric_small", "canv_metric_small", 1200, 600);
		TH2D* metric_small = new TH2D("metric_small", "d-r metric_small", 2100, 0.0, 0.21, 53, -2.0, 50.0);

		for(int i = START; i < num; i++)
		{				
			root[i] = new TFile(Form("../%d/root_%d.root", i, i), "read");
			data[i] = (TTree*) root[i]->Get("data");
			
			TKEvent* event = new TKEvent();
			data[i]->SetBranchAddress("TKEvent", &event);
			
			int entries = data[i]->GetEntries();
			cout << endl;
			cout << "Folder: " << i << "  Entries in this folder:" << entries << endl << endl;
			
			double likelihood, re_num_hits;
			int num_hits;
			
			//for(int j = 0; j < entries; j++)
			for(int j = 0; j < 1000; j++)
			{
				data[i]->GetEntry(j);
				
				/*if(j%20000 == 0)
				{
					cout << "Event " << num_ev + j << endl;
				}*/
		
				for(int k = 0; k < event->get_tracks().size() ; k++)
				{
					if(event->get_tracks().size() == 1)
					{
						num_hits = event->get_tracks().at(k)->get_associated_tr_hits().size();
						if(num_hits == 9)
						{
							double a = event->get_tracks().at(k)->get_a();
							double b = event->get_tracks().at(k)->get_b();
							cout << "y = ax + b = " << a << " * x + " << b << endl;
							double s_big = 0;
						
							likelihood = event->get_tracks().at(k)->get_likelihood();
							re_num_hits = 1/double(num_hits);
							sqr = pow(likelihood, re_num_hits);
							
							//for(int p = 0; p < 16; p++)
							{
								//if(low_measure[p] < sqr && sqr < high_measure[p])
								{
									//if(counter[p] < max_count + 1)
									{
										/*save_picture = Form("./Colors/#low-#high/Event-%d_2D.png", num_ev + j);
										
										save_picture.ReplaceAll("#low", low_dir[p]);
										save_picture.ReplaceAll("#high", high_dir[p]);
										
										event->make_top_projection(2, save_picture);			
										counter[p]++;*/
										
										TCanvas* canvas = new TCanvas("canvas", "top_view", 5800, 1600);
									
										TH2D* top_view = new TH2D("top_view", Form("Event %d", num_ev + j), 113, -0.5, 112.5, 19, -0.5, 18.5);
										for(int x = 0; x < 113; x++)
										{
											for(int y = 0; y < 19; y++)
											{
												top_view->Fill(x, y);
												if(y == 9)
												{
													top_view->Fill(x, y);
													top_view->Fill(x, y);
												}
											}
										}
										
										for(int cell_num = 0; cell_num < 2034; cell_num++)
										{
											bool is_hit = false;
											bool is_broken = false;
											bool is_associated = false;
											
											int SRL[3];
											
											for(int hit = 0; hit < event->get_tr_hits().size(); hit++)
											{
												if(cell_num == event->get_tr_hits().at(hit)->get_cell_num())
												{
													is_hit = true;
													if( event->get_tr_hits().at(hit)->get_r() > 35.0 || event->get_tr_hits().at(hit)->get_r() == -1.0 )
													{
														is_broken = true;
														SRL[0] = event->get_tr_hits().at(hit)->get_SRL('S'); 
														SRL[1] = event->get_tr_hits().at(hit)->get_SRL('R'); 
														if(SRL[0] == 1)
														{
															SRL[2] = 8 - event->get_tr_hits().at(hit)->get_SRL('L'); 
														}
														else
														{
															SRL[2] = 10 + event->get_tr_hits().at(hit)->get_SRL('L');
														}
														cout << "Event " << num_ev + j << "  Cell " << cell_num << " is broken" << endl;
														cout << "    Side: " << SRL[0] << "  Row: " << SRL[1] << " Layer: " << SRL[2] << endl;
													}	
													
													if( event->get_tr_hits().at(hit)->get_associated_track() != nullptr )
													{
														is_associated = true;
														SRL[0] = event->get_tr_hits().at(hit)->get_SRL('s'); 
														SRL[1] = event->get_tr_hits().at(hit)->get_SRL('r'); 
														if(SRL[0] == 1)
														{
															SRL[2] = 8 - event->get_tr_hits().at(hit)->get_SRL('L'); 
														}
														else
														{
															SRL[2] = 10 + event->get_tr_hits().at(hit)->get_SRL('L');
														} 
														
														//a * x + b - y = 0
														double x = event->get_tr_hits().at(hit)->get_xy('x');
														double y = event->get_tr_hits().at(hit)->get_xy('y');
														double d = abs(a * x + b - y) / sqrt(pow(a, 2) + 1);
														double radius = event->get_tr_hits().at(hit)->get_r();
														double s_small = d - radius;
														s_big = s_big + pow(s_small, 2);
														
														cout << "Event " << num_ev + j << "  Cell " << cell_num << " is associated" << endl;
														cout << "    Side: " << SRL[0] << "  Row: " << SRL[1] << " Layer: " << SRL[2] << endl;
														cout << "    x = " << x << "  y = " << y << "  d = " << d << "  radius = " << radius << endl;
														top_view->Fill(SRL[1], SRL[2]);
														metric_small->Fill(sqr, s_small);
													}			
												}
											}
										}
										
										s_big = s_big / num_hits;
										cout << "S^2 = " << s_big << endl;
										metric->Fill(s_big);
										
										top_view->Draw("colz");
										top_view->SetStats(0);
										
										for (int i = 0; i < 113; ++i) 
										{
										    top_view->GetXaxis()->SetBinLabel(i + 1, Form("%d", i));
										}
										gPad->Update();
										
										for (int x = 1; x < top_view->GetNbinsX(); ++x) 
										{
											double xPos = top_view->GetXaxis()->GetBinLowEdge(x + 1);
											TLine *vLine = new TLine(xPos, top_view->GetYaxis()->GetBinLowEdge(1),
														xPos, top_view->GetYaxis()->GetBinLowEdge(top_view->GetNbinsY() + 1));
											vLine->SetLineWidth(2);
											vLine->SetLineStyle(1); // Set line style
											vLine->Draw();
										}

										// Manually add horizontal grid lines
										for (int y = 1; y < top_view->GetNbinsY(); ++y) 
										{
											double yPos = top_view->GetYaxis()->GetBinLowEdge(y + 1);
											TLine *hLine = new TLine(top_view->GetXaxis()->GetBinLowEdge(1), yPos,
														top_view->GetXaxis()->GetBinLowEdge(top_view->GetNbinsX() + 1), yPos);
											hLine->SetLineWidth(2);
											hLine->SetLineStyle(1); // Set line style
											hLine->Draw();
										}
										
										canvas->Print(Form("./top_view/Event_%d.png", num_ev + j));
										delete top_view;
										delete canvas;
										
									}
								}
							} 
						}	
					}	
				}
				
				//event->make_top_projection(2);
				//event->build_event();
			}
			num_ev = num_ev + entries;
			
			root[i]->Close();
			
			/*cout << endl;
			for(int j = 0; j < 16; j++)
			{
				if(counter[j] < max_count + 1)
				{
					cout << j + 1 << ": " << counter[j] << endl;
				}
			}
			cout << endl;*/
		}
		
		metric->Draw();
		canv_metric->Print("./top_view/canv_metric.png");
		delete metric;
		delete canv_metric;
		
		metric_small->Draw("colz");
		canv_metric_small->Print("./top_view/canv_metric_small.png");
		delete metric_small;
		delete canv_metric_small;
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
