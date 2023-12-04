#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis()
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

	int max_num = 0;
	int max_ev = 0;
	int num_ev = 0;
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];
		
		TH1D* like_all_a = new TH1D("like_all_a", "Like_all_a", 4100, 0.0, 0.041);
		like_all_a->GetXaxis()->SetTitle("likelihood");
		like_all_a->GetYaxis()->SetTitle("Counts");
		
		TH1D* like_all_b = new TH1D("like_all_b", "Like_all_b", 8200, 0.0, 0.82);
		like_all_b->GetXaxis()->SetTitle("likelihood^(1/n)");
		like_all_b->GetYaxis()->SetTitle("Counts");
		
		TH1D* like_all_c = new TH1D("like_all_c", "Like_all_c", 33000, 0.0, 330.0);
		like_all_c->GetXaxis()->SetTitle("lg(likelihood)");
		like_all_c->GetYaxis()->SetTitle("Counts");
		
		TH1D* like_all_d = new TH1D("like_all_d", "Like_all_d", 20000, 0.0, 20.0);
		like_all_d->GetXaxis()->SetTitle("lg(likelihood^(1/n))");
		like_all_d->GetYaxis()->SetTitle("Counts");
		
		TH1D* like_a[16];
		TH1D* like_b[16];
		TH1D* like_c[16];
		TH1D* like_d[16];
		for(int i = 0; i < 16; i++)
		{
			like_a[i] = new TH1D(Form("like_a-%d", i), Form("num_hits_a = %d", i), 4100, 0.0, 0.041);
			like_a[i]->GetXaxis()->SetTitle("likelihood");
			like_a[i]->GetYaxis()->SetTitle("Counts");
			
			like_b[i] = new TH1D(Form("like_b-%d", i), Form("num_hits_b = %d", i), 6000, 0.0, 0.6);
			like_b[i]->GetXaxis()->SetTitle("likelihood^(1/n)");
			like_b[i]->GetYaxis()->SetTitle("Counts");
			
			like_c[i] = new TH1D(Form("like_c-%d", i), Form("num_hits_c = %d", i), 10000, 0.0, 10.0);
			like_c[i]->GetXaxis()->SetTitle("lg(likelihood)");
			like_c[i]->GetYaxis()->SetTitle("Counts");
			
			like_d[i] = new TH1D(Form("like_d-%d", i), Form("num_hits_d = %d", i), 2000, 0.0, 2.0);
			like_d[i]->GetXaxis()->SetTitle("lg(likelihood^(1/n))");
			like_d[i]->GetYaxis()->SetTitle("Counts");
		}
		
		for(int i = START; i < num; i++)
		{				
			root[i] = new TFile(Form("../%d/root_%d.root", i, i), "read");
			data[i] = (TTree*) root[i]->Get("data");
			
			TKEvent* event = new TKEvent();
			data[i]->SetBranchAddress("TKEvent", &event);
			
			int entries = data[i]->GetEntries();
			cout << endl;
			cout << "Entries in this folder:" << entries << endl << endl;
			
			double likelihood, re_num_hits;
			int num_hits;
			
			for(int j = 0; j < entries; j++)
			{
				data[i]->GetEntry(j);
				
				if(j%50000 == 0)
				{
					cout << "Event " << num_ev + j << endl;
				}
		
				for(int k = 0; k < event->get_tracks().size() ; k++)
				{
					num_hits = event->get_tracks().at(k)->get_associated_tr_hits().size();
					likelihood = event->get_tracks().at(k)->get_likelihood();
					re_num_hits = 1/double(num_hits);
						
					like_all_a->Fill(likelihood);
					like_all_b->Fill(pow(likelihood, re_num_hits));
					like_all_c->Fill(-log10(likelihood));
					like_all_d->Fill(-log10(pow(likelihood, re_num_hits)));
					
					if(num_hits < 16)
					{
						
						like_a[num_hits]->Fill(likelihood);
						like_b[num_hits]->Fill(pow(likelihood, re_num_hits));
						like_c[num_hits]->Fill(-log10(likelihood));
						like_d[num_hits]->Fill(-log10(pow(likelihood, re_num_hits)));	
					}

					if(max_num < num_hits)
					{
						max_num = num_hits;
						max_ev = num_ev + j;
					}				
				}
				
				//event->make_top_projection(2);
				//event->build_event();
			}
			num_ev = num_ev + entries;
			
			root[i]->Close();
		}
		cout << "max_num: " << max_num << "  mav_ev: " << max_ev << endl;

		
		TCanvas* canvas_all_a = new TCanvas("canvas_all_a", "Histogram_all_a", 1200, 600);
		canvas_all_a->SetRightMargin(0.3);
		canvas_all_a->SetLogy();
		like_all_a->Draw();
		gPad->Update();
		canvas_all_a->Print("./Crazy_canvases/Canvases_a/_Canvas_all_a.png");
		delete canvas_all_a;
		
		TCanvas* canvas_all_b = new TCanvas("canvas_all_b", "Histogram_all_b", 1200, 600);
		canvas_all_b->SetRightMargin(0.3);
		canvas_all_b->SetLogy();
		like_all_b->Draw();
		gPad->Update();
		canvas_all_b->Print("./Crazy_canvases/Canvases_b/_Canvas_all_b.png");
		delete canvas_all_b;
		
		TCanvas* canvas_all_c = new TCanvas("canvas_all_c", "Histogram_all_c", 1200, 600);
		canvas_all_c->SetRightMargin(0.3);
		canvas_all_c->SetLogy();
		like_all_c->Draw();
		gPad->Update();
		canvas_all_c->Print("./Crazy_canvases/Canvases_c/_Canvas_all_c.png");
		delete canvas_all_c;
		
		TCanvas* canvas_all_d = new TCanvas("canvas_all_d", "Histogram_all_d", 1200, 600);
		canvas_all_d->SetRightMargin(0.3);
		canvas_all_d->SetLogy();
		like_all_d->Draw();
		gPad->Update();
		canvas_all_d->Print("./Crazy_canvases/Canvases_d/_Canvas_all_d.png");
		delete canvas_all_d;
		
		TCanvas* canvas_a[16];
		TCanvas* canvas_b[16];
		TCanvas* canvas_c[16];
		TCanvas* canvas_d[16];
		for(int i = 3; i < 16; i++)
		{
			canvas_a[i] = new TCanvas(Form("canvas_a_%d", i), Form("Histogram_a_%d", i), 1200, 600);
			canvas_a[i]->SetRightMargin(0.3);
			canvas_a[i]->SetLogy();
			like_a[i]->Draw();
			gPad->Update();
			canvas_a[i]->Print(Form("./Crazy_canvases/Canvases_a/Canvas_a_%d.png", i));
			delete canvas_a[i];
			
			canvas_b[i] = new TCanvas(Form("canvas_b_%d", i), Form("Histogram_b_%d", i), 1200, 600);
			canvas_b[i]->SetRightMargin(0.3);
			canvas_b[i]->SetLogy();
			like_b[i]->Draw();
			gPad->Update();
			canvas_b[i]->Print(Form("./Crazy_canvases/Canvases_b/Canvas_b_%d.png", i));
			delete canvas_b[i];
			
			canvas_c[i] = new TCanvas(Form("canvas_c_%d", i), Form("Histogram_c_%d", i), 1200, 600);
			canvas_c[i]->SetRightMargin(0.3);
			canvas_c[i]->SetLogy();
			like_c[i]->Draw();
			gPad->Update();
			canvas_c[i]->Print(Form("./Crazy_canvases/Canvases_c/Canvas_c_%d.png", i));
			delete canvas_c[i];
			
			canvas_d[i] = new TCanvas(Form("canvas_d_%d", i), Form("Histogram_d_%d", i), 1200, 600);
			canvas_d[i]->SetRightMargin(0.3);
			canvas_d[i]->SetLogy();
			like_d[i]->Draw();
			gPad->Update();
			canvas_d[i]->Print(Form("./Crazy_canvases/Canvases_d/Canvas_d_%d.png", i));
			delete canvas_d[i];
		}
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
