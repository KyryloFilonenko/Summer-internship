#include "/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/include/TKEvent.h" 

R__LOAD_LIBRARY(/sps/nemo/scratch/kfilonen/TKEvent/TKEvent/lib/libTKEvent.so);

const int START = 0;
const int END = 132;

void analysis_categories()
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
	double low_measure[16] = {0.0, 0.02, 0.12, 0.197, 0.205, 0.237, 0.245, 0.280, 0.290, 0.337, 0.345, 0.405, 0.410, 0.485, 0.500, 0.580};
	double high_measure[16] = {0.02, 0.12, 0.18, 0.203, 0.230, 0.243, 0.280, 0.290, 0.335, 0.343, 0.403, 0.410, 0.485, 0.490, 0.580, 0.590};
	string low_dir[16] = {"0.0", "0.02", "0.12", "0.197", "0.205", "0.237", "0.245", "0.280", "0.290", "0.337", "0.345", "0.405", "0.410", "0.485", "0.500", "0.580"};
	string high_dir[16] = {"0.02", "0.12", "0.18", "0.203", "0.230", "0.243", "0.280", "0.290", "0.335", "0.343", "0.403", "0.410", "0.485", "0.490", "0.580", "0.590"};
	
	int max_count = 20;
	for(int i = 0; i < 16; i++)
	{
		counter[i] = 0;
	}
	
	if(num < END + 2)
	{
	
		TFile* root[num];
		TTree* data[num];

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
							likelihood = event->get_tracks().at(k)->get_likelihood();
							re_num_hits = 1/double(num_hits);
							sqr = pow(likelihood, re_num_hits);
							
							for(int p = 0; p < 16; p++)
							{
								if(low_measure[p] < sqr && sqr < high_measure[p])
								{
									if(counter[p] < max_count + 1)
									{
										save_picture = Form("./Colors/#low-#high/Event-%d_2D.png", num_ev + j);
										
										save_picture.ReplaceAll("#low", low_dir[p]);
										save_picture.ReplaceAll("#high", high_dir[p]);
										
										event->make_top_projection(2, save_picture);
										counter[p]++;
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
			
			cout << endl;
			for(int j = 0; j < 16; j++)
			{
				if(counter[j] < max_count + 1)
				{
					cout << j + 1 << ": " << counter[j] << endl;
				}
			}
			cout << endl;
		}
	}
	else
	{
		cout << "We don't have enought data :(" << endl;
	}

}
