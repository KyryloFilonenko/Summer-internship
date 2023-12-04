#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_metric()
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
	
	int void_ev = 0; //void events
	
	
	TString save_picture;
	int counter[18];
	int max_count = 1;
	for(int i = 0; i < 18; i++)
	{
		counter[i] = 0;
	}
	
	double low_measure[8] = {0.086, 0.097, 0.11, 0.125, 0.14, 0.16, 0.18, 0.2};
	double high_measure[8] = {0.094, 0.105, 0.12, 0.135, 0.154, 0.175, 0.198, 0.25};
	string low_dir[16] = {"0.086", "0.097", "0.11", "0.125", "0.14", "0.16", "0.18", "0.2", "0.086_l", "0.097_l", "0.11_l", "0.125_l", "0.14_l", "0.16_l", "0.18_l", "0.2_l"};
	string high_dir[16] = {"0.094", "0.105", "0.12", "0.135", "0.154", "0.175", "0.198", "0.25", "0.094_l", "0.105_l", "0.12_l", "0.135_l", "0.154_l", "0.175_l", "0.198_l", "0.25_l"};
	double low_s[8] = {7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0};
	double high_s[8] = {8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
	
	
	
	int counter_group[9];
	for(int i = 0; i < 9; i++)
	{
		counter_group[i] = 0;
	}
	double group_l_low[9] = {0.0, 0.01, 0.05, 0.1, 0.15, 0.17, 0.24, 0.3, 0.05};
	double group_l_high[9] = {0.01, 0.05, 0.1, 0.15, 0.17, 0.24, 0.3, 1.0, 0.15};
	string group_dir_low[9] = {"0.0", "0.01", "0.05", "0.1", "0.15", "0.17", "0.24", "0.3", "emp"};
	string group_dir_high[9] = {"0.01", "0.05", "0.1", "0.15", "0.17", "0.24", "0.3", "1.0", "ty"};
	double group_d_low[9] = {50.0, 35.0, 25.0, 20.0, 15.0, 10.0, 0.0, 0.0, 130.0};
	double group_d_high[9] = {200.0, 70.0, 50.0, 40.0, 70.0, 230.0, 1e10, 1e10, 250.0};
	
	
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];
		
		TCanvas* canv_metric = new TCanvas("canv_metric", "canv_metric", 1200, 600);
		TH1D* metric = new TH1D("metric", "d-r metric", 52, -1.0, 50.0);
		
		TCanvas* canv_metric_both = new TCanvas("canv_metric_both", "canv_metric_both", 1200, 600);
		TH2D* metric_both = new TH2D("metric_both", "d-r metric_both", 50000, 0.0, 0.5, 252, -1.0, 250.0);
		metric_both->GetXaxis()->SetTitle("likelihood^(1/n)");
		metric_both->GetYaxis()->SetTitle("d-r metrics");

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
			
			for(int j = 0; j < entries; j++)
			{
				data[i]->GetEntry(j);
				
				if(j%50000 == 0)
				{
					cout << "Event " << num_ev + j << endl;
				}
				
				
				
				
				
				//if(event->get_tracks().size() > 2)
				
				/*double best = 0;
				int k_best = 0;
				for(int k = 0; k < event->get_tracks().size(); k++)
				{
					likelihood = event->get_tracks().at(k)->get_likelihood();
					re_num_hits = 1/double(num_hits);
					sqr = pow(likelihood, re_num_hits);
					
					if(best < sqr)
					{
						best = sqr;
						k_best = k;
					}
				}
				//cout << "Size = " << event->get_tracks().size() << endl;
				//cout << "k_best = " << k_best << endl;*/
			
			
			
				//for(int k = 0; k < event->get_tracks().size() ; k++)
				if( event->get_tracks().size() == 1 )
				{
					int k = 0;
				//int k = k_best;
				
					num_hits = event->get_tracks().at(k)->get_associated_tr_hits().size();
						
					if(num_hits == 9)
					{
						double a = event->get_tracks().at(k)->get_a();
						double b = event->get_tracks().at(k)->get_b();
						//cout << "y = ax + b = " << a << " * x + " << b << endl;
						double s_big = 0;
					
						likelihood = event->get_tracks().at(k)->get_likelihood();
						re_num_hits = 1/double(num_hits);
						sqr = pow(likelihood, re_num_hits);
						
						double x, y, d, radius, s_small;
										
						for(int hit = 0; hit < event->get_tr_hits().size(); hit++)
						{											
							if( event->get_tr_hits().at(hit)->get_associated_track() != nullptr )
							{	
								//a * x + b - y = 0
								x = event->get_tr_hits().at(hit)->get_xy('x');
								y = event->get_tr_hits().at(hit)->get_xy('y');
								d = abs(a * x + b - y) / sqrt(pow(a, 2) + 1);
								radius = event->get_tr_hits().at(hit)->get_r();
								s_small = d - radius;
								s_big = s_big + pow(s_small, 2);
							}
						}
												
						s_big = s_big / num_hits;
						metric->Fill(s_big);
						//metric_both->Fill(sqr, s_big);
						
						//cout << "S^2 = " << s_big << endl;
						
						/*bool exist = false;
						for(int p = 0; p < 8; p++)
						{
							if(low_measure[p] < sqr && sqr < high_measure[p])
							{
								//if(low_s[p] < s_big && s_big < high_s[p])
								if(s_big < high_s[p])
								{
									if(counter[p + 8] < max_count + 1)
									{
										//cout << "Event " << num_ev + j << "  sqr = " << sqr << "  s_big = " << s_big << endl;
										save_picture = Form("./metric/#low-#high/Event-%d_2D.png", num_ev + j);
												
										save_picture.ReplaceAll("#low", low_dir[p + 8]);
										save_picture.ReplaceAll("#high", high_dir[p + 8]);
												
										//event->make_top_projection(2, save_picture);
										counter[p + 8]++;
									}
								}
								else
								{
									if(p < 3)
									{
										if(20 < s_big && s_big < 35)
										{
											if(counter[p] < max_count + 1)
											{
												//cout << "Event " << num_ev + j << "  sqr = " << sqr << "  s_big = " << s_big << endl;
												save_picture = Form("./metric/#low-#high/Event-%d_2D.png", num_ev + j);
														
												save_picture.ReplaceAll("#low", low_dir[p]);
												save_picture.ReplaceAll("#high", high_dir[p]);
														
												//event->make_top_projection(2, save_picture);
												counter[p]++;
											}
										}
									}
									else
									{
										if(counter[p] < max_count + 1)
										{
											//cout << "Event " << num_ev + j << "  sqr = " << sqr << "  s_big = " << s_big << endl;
											save_picture = Form("./metric/#low-#high/Event-%d_2D.png", num_ev + j);
													
											save_picture.ReplaceAll("#low", low_dir[p]);
											save_picture.ReplaceAll("#high", high_dir[p]);
													
											//event->make_top_projection(2, save_picture);
											counter[p]++;
										}
									}
								}
							}
							else
							{
								if(exist == false)
								{
									if(0.04 < sqr && sqr < 0.086)
									{
										if(30 < s_big && s_big < 45)
										{
											if(counter[16] < max_count + 1)
											{
													//cout << "Event " << num_ev + j << "  sqr = " << sqr << "  s_big = " << s_big << endl;
													save_picture = Form("./metric/0.04-0.086/Event-%d_2D.png", num_ev + j);
													
													//event->make_top_projection(2, save_picture);
													counter[16]++;
											}
										}
									}
									
									if(sqr < 0.04)
									{
										if(40 < s_big)
										{
											if(counter[17] < max_count + 1)
											{
													//cout << "Event " << num_ev + j << "  sqr = " << sqr << "  s_big = " << s_big << endl;
													save_picture = Form("./metric/0.0-0.04/Event-%d_2D.png", num_ev + j);
													
													//event->make_top_projection(2, save_picture);
													counter[17]++;
											}
										}
									}
									exist = true;
								}
							}
						}*/
						
						
						for(int p = 0; p < 9; p++)
						{
							if(group_l_low[p] < sqr && sqr < group_l_high[p])
							{
								if(group_d_low[p] < s_big && s_big < group_d_high[p])
								{
									if(counter_group[p] < max_count + 1)
									{
										//cout << "Event " << num_ev + j << "  sqr = " << sqr << "  s_big = " << s_big << endl;
										save_picture = Form("./metric/groups/#low-#high/Event-%d_2D.png", num_ev + j);
												
										save_picture.ReplaceAll("#low", group_dir_low[p]);
										save_picture.ReplaceAll("#high", group_dir_high[p]);
												
										//event->make_top_projection(2, save_picture);
										counter_group[p]++;
									}
									metric_both->Fill(sqr, s_big);
								}
							}
						}
						
						
					}
				}
				/*else
				{
					//cout << "Event " << num_ev +j << endl;
					void_ev++;
				}*/
			}
			num_ev = num_ev + entries;
			
			root[i]->Close();
			
			cout << endl;
			/*for(int j = 0; j < 18; j++)
			{
				//if(counter[j] < max_count + 1)
				{
					cout << j + 1 << ": " << counter[j] << endl;
				}
			}*/
			
			/*for(int j = 0; j < 9; j++)
			{
					cout << j + 1 << ": " << counter_group[j] << endl;
			}
			cout << endl;*/
		}
		
		/*cout << endl;
		cout << "Num_void = " << void_ev << endl;
		cout << endl;*/
		
		metric->Draw();
		//canv_metric->Print("./metric/canv_metric.png");
		delete metric;
		delete canv_metric;
		
		metric_both->Draw("colz");
		canv_metric_both->Print("./metric/canv_metric_both.png");
		delete metric_both;
		delete canv_metric_both;
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
