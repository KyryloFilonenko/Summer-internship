#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_average()
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
		
		/*TCanvas* canv_metric = new TCanvas("canv_metric", "canv_metric", 1200, 600);
		TH1D* metric = new TH1D("metric", "d-r metric", 5200, -1.0, 50.0);
		
		TCanvas* canv_metric_both = new TCanvas("canv_metric_both", "canv_metric_both", 1200, 600);
		TH2D* metric_both = new TH2D("metric_both", "d-r metric_both", 50000, 0.0, 0.5, 252, -1.0, 250.0);*/
		
		int num_ev = 0;
		int am_hits = 9;
		
		TCanvas* canv_proj_y = new TCanvas("canv_proj_y", "canv_proj_y", 1200, 600);
		TH1D* proj_y = new TH1D("proj_y", "d-r", 10100, -1.0, 100.0);
		
		TCanvas* canv_av_stdev = new TCanvas("canv_av_stdev", "canv_av_stdev", 1200, 600);
		TH2D* av_stdev = new TH2D("av_stdev", "av_stdev", 5100, -1.0, 50.0, 10100, -1.0, 100.0);
		av_stdev->GetXaxis()->SetTitle("d-r metrics");
		av_stdev->GetYaxis()->SetTitle("st. dev. of d-r metrics");

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
						
						double av_s = 0;
						double st_dev = 0;
						double data_s[am_hits];
						int f = 0;
						for(int hit = 0; hit < event->get_tr_hits().size(); hit++)
						{											
							if( event->get_tr_hits().at(hit)->get_associated_track() != nullptr )
							{	
								//a * x + b - y = 0
								x = event->get_tr_hits().at(hit)->get_xy('x');
								y = event->get_tr_hits().at(hit)->get_xy('y');
								d = abs(a * x + b - y) / sqrt(pow(a, 2) + 1);
								radius = event->get_tr_hits().at(hit)->get_r();
								s_small = abs(d - radius);
								data_s[f] = s_small;
								f++;
								
								av_s = av_s + s_small;
								s_big = s_big + pow(s_small, 2);
							}
						}
								
						av_s = av_s / num_hits;	
						for(int q = 0; q < f; q++)
						{
							st_dev = st_dev + pow(av_s - data_s[q], 2); 
							proj_y->Fill(data_s[q]);
						}
						st_dev = pow(st_dev / num_hits, 0.5);
						
						av_stdev->Fill(av_s, st_dev);
						
						s_big = s_big / num_hits;
						
						//metric_both->Fill(sqr, s_big);
						//metric->Fill(s_big);
					}
				}
			}
			num_ev = num_ev + entries;
			
			root[i]->Close();
		}
		av_stdev->Draw("colz");
		canv_av_stdev->Print(Form("./deviating/d-r/%d/av_stdev.png", am_hits));
		delete av_stdev;
		delete canv_av_stdev;
				
		/*metric_both->Draw("colz");
		canv_metric_both->Print(Form("./deviating/d-r/%d/canv_metric_both.png", am_hits));
		delete metric_both;
		delete canv_metric_both;*/
		
		proj_y->Draw();
		canv_proj_y->SetLogy();
		canv_proj_y->Print(Form("./deviating/d-r/%d/canv_proj_y.png", am_hits));
		delete proj_y;
		delete canv_proj_y;
		
		/*metric->Draw();
		canv_metric->Print(Form("./deviating/d-r/%d/canv_metric.png", am_hits));
		delete metric;
		delete canv_metric;*/		
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
