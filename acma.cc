// ACceptance MAtrix generator
// use: .x acma.cc(thrown, reconstructed, columns, rows) (5=row=col is usually ok)
void *acma(TNtuple *th, TNtuple * re, Int_t ncol, Int_t nrow) // ncol = max thrown pions
{
  
  
  TH2 *hist = new TH2F("hist","Histogram of reco/thrown",6,-0.5,5.5,6,-0.5,5.5);
  hist->SetXTitle("Thrown");
  hist->SetYTitle("Reconstructed");
  
  
  ////////////////////////////////////////
  //////////////////////////////////////
  /////
  const char *evnt = "evnt";
  Long64_t nentries = th->GetEntries();
  Int_t thcount = 0;
  Int_t recount = 0;
  Long64_t evntcount = 0;
  
  for ( Long64_t jentry = 0; jentry < nentries; jentry++ )
    {
      th->GetEntry(jentry);
      if ( th->GetLeaf(evnt)->GetValue() > evntcount ) // if event is finished, fill
  	{
	  hist->Fill(thcount,recount);
	  if ( thcount==0 and recount==3 )
	    cout << "check out event " << evntcount << endl; // control for rare events
  	  thcount = 0;
  	  recount = 0;
	  evntcount = th->GetLeaf(evnt)->GetValue();  // event update
  	}
      if ( th->GetLeaf("pid")->GetValue() == 8 )
  	thcount++;
      re->GetEntry(jentry);
      if ( re->GetLeaf("pid")->GetValue() == 211 )
  	recount++;
    }
  
}
