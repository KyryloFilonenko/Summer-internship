#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_deviating()
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
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];
		
		TCanvas* canv_metric = new TCanvas("canv_metric", "canv_metric", 1200, 600);
		TH1D* metric = new TH1D("metric", "d-r metric", 52, -1.0, 50.0);
		
		TCanvas* canv_metric_both = new TCanvas("canv_metric_both", "canv_metric_both", 1200, 600);
		TH2D* metric_both = new TH2D("metric_both", "d-r metric_both", 50000, 0.0, 0.5, 252, -1.0, 250.0);
		
		int num_ev = 0;
		TString save_picture;
		int am_hits = 9;
		int max_count = 20;
		int counter[am_hits + 1];
		TCanvas* canv_deviating[am_hits + 1];
		TH2D* deviating[am_hits + 1];
		for(int i = 0; i < am_hits + 1; i++)
		{
			counter[i] = 0;
			deviating[i] = new TH2D(Form("Deviating %d", i), Form("Deviating %d", i), 50000, 0.0, 0.5, 252, -1.0, 250.0);
			deviating[i]->GetXaxis()->SetTitle("likelihood6(1/n)");
			deviating[i]->GetYaxis()->SetTitle("d-r metrics");
			canv_deviating[i] = new TCanvas(Form("Canv_Deviating %d", i), Form("Canv_Deviating %d", i), 1200, 600);
		}
		

		for(int i = START; i < num; i++)
		{				
			root[i] = new TFile(Form("../%d/root_%d.root", i, i), "read");
			data[i] = (TTree*) root[i]->Get("data");
			
			TKEvent* event = new TKEvent();
			data[i]->SetBranchAddress("TKEvent", &event);
			
			int entries = data[i]->GetEntries();
			cout << endl;
			cout << "Folder: " << i << "  Entries in this folder:" << entries << endl << endl;
			
			double likelihood, re_num_hits, sqr;
			int num_hits;
			
			for(int j = 0; j < entries; j++)
			{
				data[i]->GetEntry(j);
				
				if(j%20000 == 0)
				{
					cout << "Event " << num_ev + j << endl;
				}
			
			
				//for(int k = 0; k < event->get_tracks().size() ; k++)
				if( event->get_tracks().size() == 1 )
				{
					int k = 0;
				
					num_hits = event->get_tracks().at(k)->get_associated_tr_hits().size();
						
					if(num_hits == am_hits)
					{
						double a = event->get_tracks().at(k)->get_a();
						double b = event->get_tracks().at(k)->get_b();
						//cout << "y = ax + b = " << a << " * x + " << b << endl;
						double s_big = 0;
					
						likelihood = event->get_tracks().at(k)->get_likelihood();
						re_num_hits = 1/double(num_hits);
						sqr = pow(likelihood, re_num_hits);
						
						double x, y, d, radius, s_small;
									
						int count = 0;	
						double dev[am_hits];
						int f = 0;
						double av_s = 0;
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
								
								dev[f] = s_small;
								av_s = av_s + abs(s_small);
								f++;
								/*if(s_small > 0)
								{
									count++;
								}*/
								s_big = s_big + pow(s_small, 2);
							}
						}
									
						av_s = av_s / num_hits;			
						s_big = s_big / num_hits;
						metric->Fill(s_big);
						metric_both->Fill(sqr, s_big);
						
						double sigma = 0.9 * av_s;
						/*for(int q = 0; q < f; q++)
						{
							sigma = sigma + pow(dev[q] - av_s, 2);
						}
						sigma = pow(sigma / (f - 1), 0.5);*/
						
						for(int q = 0; q < f; q++)
						{
							if(sigma > 5.0)
							{
								if(dev[q] > sigma || dev[q] > 30)
								{
									count++;
								}
							}
						}
						
						deviating[count]->Fill(sqr, s_big);
						
						/*for(int q = 0; q < f + 1; q++) just for moment
						{
							if(count == q)
							{
								if(counter[q] < max_count)
								{
									cout << sigma << endl;
									event->make_top_projection(2, Form("./deviating/%d/dev_%d/Event %d.png", am_hits, q, num_ev + j));
									counter[q]++;
									
									TCanvas* canv_dev = new TCanvas("canv_dev", "canv_dev", 1200, 600);
									TH1D* metric_dev = new TH1D("metric_dev", "d-r metric_dev", 52, -1.0, 50.0);
									for(int g = 0; g < f; g++)
									{
										metric_dev->Fill(abs(dev[g]));
									}
									metric_dev->Draw();
									canv_dev->Print(Form("./deviating/%d/dev_%d/Event_dev %d.png", am_hits, q, num_ev + j));
									delete metric_dev;
									delete canv_dev;
								}
							}
						}*/
					}
				}
			}
			num_ev = num_ev + entries;
			
			root[i]->Close();
			
			/*cout << endl;
			for(int j = 0; j < 18; j++)
			{
				//if(counter[j] < max_count + 1)
				{
					cout << j + 1 << ": " << counter[j] << endl;
				}
			}*/
		}
		
		metric->Draw();
		canv_metric->Print(Form("./deviating/%d/canv_metric.png", am_hits));
		delete metric;
		delete canv_metric;
		
		metric_both->Draw("colz");
		canv_metric_both->Print(Form("./deviating/%d/canv_metric_both.png", am_hits));
		delete metric_both;
		delete canv_metric_both;
		
		for(int i = 0; i < am_hits + 1; i++)
		{
			deviating[i]->Draw("colz");
			canv_deviating[i]->Print(Form("./deviating/%d/canv_deviating_%d.png", am_hits, i));
			delete deviating[i];
			delete canv_deviating[i];
		}
		
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
