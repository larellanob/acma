// ACceptance MAtrix generator
void acma(TString filelist = "simlist.txt" )
{
  // Use as input a file (e.g. ./simlist.txt) with a list of all the input simulation files
  // example usage as macro, from bash terminal:
  // root -l acma.cc\(\"input/simulation_files_C2.txt\"\)
  cout << "Using filelist: " << filelist << endl;
  ifstream in(filelist, ios::in);
  TString dataset;
  // I'm just interested in the filename after an
  // arbitrary number of dirs in the path, delimited by "/"
  // get a load of how how easy it is to split a string by a delimiter in ROOT!
  TObjArray *tx = filelist.Tokenize("/");
  dataset = ((TObjString *)(tx->At(tx->GetEntries()-1)))->String();
  dataset.ReplaceAll(".txt","");
  cout << "Using dataset: " << dataset << endl;
  
  TChain chth("tree_thrown");
  TChain chre("tree_accept");
  while ( in ) {
    char text[200];
    in >> text;
    if ( in.eof() ) {
      break;
    }
    chth.Add(text);
    chre.Add(text);
  }
  
  // /home/luciano/Physics/CLAS/data/simulation_files_C/CFFTree_C_data_98.root
  TH2 *hist_plus = new TH2F("hist_plus","Histogram of reco/thrown for pi+",6,-0.5,5.5,6,-0.5,5.5);
  hist_plus->SetXTitle("Thrown");
  hist_plus->SetYTitle("Reconstructed");

  TH2 *hist_minu = new TH2F("hist_minu","Histogram of reco/thrown for pi-",6,-0.5,5.5,6,-0.5,5.5);
  hist_minu->SetXTitle("Thrown");
  hist_minu->SetYTitle("Reconstructed");


  ////////////////////////////////////////
  //////////////////////////////////
  //////////////////////////////////

  TTreeReader th(&chth);
  TTreeReader re(&chre);
  
  TTreeReaderArray<Int_t> pidth(th,"pid");
  TTreeReaderArray<Int_t> pidre(re,"pid");
  TTreeReaderValue<Int_t> TargTypeth(th,"TargType");
  TTreeReaderValue<Int_t> TargTypere(re,"TargType");
  TTreeReaderValue<Int_t> evntth(th,"evnt");
  TTreeReaderValue<Int_t> evntre(re,"evnt");

  while ( re.Next() ) {
    th.Next();

    int piplus_th = 0;
    int piminu_th = 0;
    int piplus_re = 0;
    int piminu_re = 0;
    
    for ( int i = 0; i < pidth.GetSize(); i++ ) {
      //cout << "size "  << i << endl;
      if ( pidth[i] == 211 ) {
	piplus_th++;
      } else if ( pidth[i] == -211 ) {
	piminu_th++;
      }
    }
    for ( int i = 0; i < pidre.GetSize(); i++ ) {
      if ( pidre[i] == 211 ) {
	piplus_re++;
      } else if ( pidre[i] == -211 ) {
	piminu_re++;
      }
    }
    if ( piplus_th != 0 || piplus_re != 0 ) {
      hist_plus->Fill(piplus_th,piplus_re);
    }
    if ( piminu_th != 0 || piminu_re != 0 ) {
      hist_minu->Fill(piminu_th,piminu_re);
    }
  }
  
  ////////////////////////////////////////
  //////////////////////////////////////
  /////

  /*
  const char *evnt = "evnt";
  Long64_t nentries = th.GetEntries();
  Int_t thcount = 0;
  Int_t recount = 0;
  Long64_t evntcount = 0;
  
  for ( Long64_t jentry = 0; jentry < nentries; jentry++ )
    {
      th.GetEntry(jentry);
      if ( th.GetLeaf(evnt)->GetValue() > evntcount ) // if event is finished, fill
  	{
	  hist->Fill(thcount,recount);
	  if ( thcount==0 and recount==3 )
	    cout << "check out event " << evntcount << endl; // control for rare events
  	  thcount = 0;
  	  recount = 0;
	  evntcount = th.GetLeaf(evnt)->GetValue();  // event update
  	}
      if ( th.GetLeaf("pid")->GetValue() == 8 )
  	thcount++;
      re.GetEntry(jentry);
      if ( re.GetLeaf("pid")->GetValue() == 211 )
  	recount++;
    }
  */
  new TCanvas();
  hist_plus->Draw("colz text");
  new TCanvas();
  hist_minu->Draw("colz text");
  TFile f("output/matrix_plus_"+dataset+".root","recreate",dataset);
  hist_plus->Write();
  f.Close();
  TFile g("output/matrix_minu_"+dataset+".root","recreate",dataset);
  hist_minu->Write();
  g.Close();
}
