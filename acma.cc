// ACceptance MAtrix generator
// use: .x acma.cc(thrown, reconstructed, columns, rows) (5=row=col is usually ok)
TCanvas *acma(TNtuple *th, TNtuple * re, Int_t ncol, Int_t nrow) // ncol = max thrown pions
{
  TCanvas *c = new TCanvas("c");
  
  TPaveText *column[ncol+1];
  for ( Int_t i = 0; i < ncol+1; i++ )
    column[i] = new TPaveText(i/(ncol+1.0),.0,(i+1.0)/(ncol+1.0),1.0,"NB");

  TH2 *hist = new TH2F("hist","Histogram of reco/thrown",6,-0.5,5.5,6,-0.5,5.5);
  hist->SetXTitle("Thrown");
  hist->SetYTitle("Reconstructed");
  
  column[0]->AddText("Reco/Thrown");
  // left column (reco)
  for ( Int_t i = 0; i < nrow; i++ )
    {
      string s = to_string(i);       // c++11 thing, selfexplanatory
      char const* chari = s.c_str(); // string to const char*
      column[0]->AddText(chari);     // only takes in const char*
      column[0]->AddLine(0.,(i+1.0)/(nrow+1.0),1.0,(i+1.0)/(nrow+1.0));
    }

  // top row (thrown)
  for ( Int_t i = 1; i < ncol+1; i++ )
    {
      string s = to_string(i-1);       // c++11 thing, selfexplanatory
      char const* chari = s.c_str(); // string to const char*
      column[i]->AddText(chari);     // only takes in const char*
      
    }

  // grid
  for ( Int_t i = 0; i < ncol; i++ )
    {
      column[i]->AddLine(.99,-1.1,.99,1.0);
      for ( Int_t j = 0; j < nrow; j++ )
	column[i+1]->AddLine(0.,(j+1.0)/(nrow+1.0),1.0,(j+1.0)/(nrow+1.0));
    }

  
  // initialize ftable with all zeroes
  Float_t ftable[nrow][ncol];
  for ( Int_t i = 0; i < nrow; i++ )
    {
      for ( Int_t j = 0; j < ncol; j++ )
	ftable[i][j] = 0;
    }
  

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
      if ( th->GetLeaf(evnt)->GetValue() > evntcount )
  	{
  	  ftable[recount][thcount]++;
	  hist->Fill(thcount,recount);
	  if ( thcount==0 and recount==3 )
	    cout << "check out event " << evntcount << endl;
  	  thcount = 0;
  	  recount = 0;
	  evntcount = th->GetLeaf(evnt)->GetValue();
  	}
      if ( th->GetLeaf("pid")->GetValue() == 8 )
  	thcount++;
      re->GetEntry(jentry);
      if ( re->GetLeaf("pid")->GetValue() == 211 )
  	recount++;
    }
  
  
  
  // add ftable to tpavetext
  for ( Int_t col = 0; col < ncol; col++ )
    {
      for ( Int_t row = 0; row < nrow; row++ )
	{
	  string s = to_string(ftable[row][col]);  // c++11 thing, selfexplanatory
	  char const* chari = s.c_str();           // string to const char*
	  column[col+1]->AddText(chari);           // only takes in const char*
	}
    }
  
  
    //for ( Int_t i = 0; i < ncol+1; i++ )
    //  column[i]->Draw();
    hist->Draw("COLZ");
  return c;
}
