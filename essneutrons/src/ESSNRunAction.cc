/// \file ESSNRunAction.cc
/// \brief Implementation of the ESSNRunAction class; adapted from
///        Geant4 example B5 B5RunAction.cc


#include "ESSNAnalysis.hh"
#include "ESSNRun.hh"
#include "ESSNRunAction.hh"
#include "ESSNEventAction.hh"
#include "ESSNParam.hh"
#include "ESSNTrackingAction.hh"

#include "G4Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

//-- In order to obtain detector information.
#include "G4RunManager.hh"
#include "ESSNCompleteDetectorConstruction.hh"
#include "ESSNImportanceDetectorConstruction.hh"
#include "G4THitsMap.hh"
#include "G4TScoreHistFiller.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNRunAction::ESSNRunAction(ESSNEventAction* eventAction)
 : G4UserRunAction(),
   fEventAction(eventAction),
   fFieldValue(20)
{

  // - Prepare data member for ESSNRun.
  //   vector represents a list of MultiFunctionalDetector names.
  fSDName.push_back(G4String("ConcreteSD"));

  // Set up the generic analysis manager.
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("essn_output");

  // Creating ntuple
  if (NTUPLE) {

    analysisManager->CreateNtuple("essn", "Particles");
    analysisManager->CreateNtupleIColumn("event");         // column Id = 0
    analysisManager->CreateNtupleSColumn("particleName");  // column Id = 1
    analysisManager->CreateNtupleIColumn("trackID");       // column Id = 2
    analysisManager->CreateNtupleIColumn("parentID");      // column Id = 3
    analysisManager->CreateNtupleDColumn("mass");          // column Id = 4
    analysisManager->CreateNtupleDColumn("xi");            // column Id = 5
    analysisManager->CreateNtupleDColumn("yi");            // column Id = 6
    analysisManager->CreateNtupleDColumn("zi");            // column Id = 7
    analysisManager->CreateNtupleDColumn("ti");            // column Id = 8
    analysisManager->CreateNtupleDColumn("xf");            // column Id = 9
    analysisManager->CreateNtupleDColumn("yf");            // column Id = 10
    analysisManager->CreateNtupleDColumn("zf");            // column Id = 11
    analysisManager->CreateNtupleDColumn("tf");            // column Id = 12
    analysisManager->CreateNtupleDColumn("pxi");           // column Id = 13
    analysisManager->CreateNtupleDColumn("pyi");           // column Id = 14
    analysisManager->CreateNtupleDColumn("pzi");           // column Id = 15
    analysisManager->CreateNtupleDColumn("pxf");           // column Id = 16
    analysisManager->CreateNtupleDColumn("pyf");           // column Id = 17
    analysisManager->CreateNtupleDColumn("pzf");           // column Id = 18
    analysisManager->CreateNtupleSColumn("volInit");       // column Id = 19
    analysisManager->CreateNtupleSColumn("volFinal");      // column Id = 20
    analysisManager->FinishNtuple();

    // Set ntuple output file
    analysisManager->SetNtupleFileName(0, "essn_ntuple");


   //María

   //Neutron tracks Step by Step: Ntuple id = 1

    analysisManager->CreateNtuple("essn","Neutron Energy");

    analysisManager->CreateNtupleIColumn("event");        // column Id = 0
    analysisManager->CreateNtupleSColumn("particleName"); // column Id = 1
    analysisManager->CreateNtupleIColumn("trackID");      // column Id = 2
    analysisManager->CreateNtupleIColumn("parentID");     // column Id = 3
    analysisManager->CreateNtupleDColumn("deltaE");	  // column Id = 4
    analysisManager->CreateNtupleDColumn("totalEloss");    // column Id = 5
    analysisManager->CreateNtupleDColumn("kineticEnergy"); //column Id = 6
    analysisManager->CreateNtupleSColumn("Prevolume");    // column Id = 7
    analysisManager->CreateNtupleSColumn("Posvolume");    //column Id = 8
    analysisManager->CreateNtupleDColumn("r_pre");	  //column Id = 9
    analysisManager->CreateNtupleDColumn("r_pos");         //column Id = 10

    analysisManager->FinishNtuple(1);
    analysisManager->SetNtupleFileName(1, "essn_ntuple_energy");

   //Neutron Tracks, Track by track: Ntuple id = 2
    analysisManager->CreateNtuple("essn","track");
    analysisManager->CreateNtupleIColumn("eventID");				//0
    analysisManager->CreateNtupleIColumn("trackID"); 				//1
    analysisManager->CreateNtupleDColumn("EkTrack",fEventAction->GetEk());	//2
    analysisManager->CreateNtupleDColumn("Eloss",fEventAction->Eloss);		//3
    analysisManager->CreateNtupleDColumn("track_x",fEventAction->r[0]);		//4
    analysisManager->CreateNtupleDColumn("track_y",fEventAction->r[1]);		//5
    analysisManager->CreateNtupleDColumn("track_z",fEventAction->r[2]);		//6
    analysisManager->CreateNtupleIColumn("status");				//7
    analysisManager->CreateNtupleSColumn("volFinal");				//8
    analysisManager->CreateNtupleSColumn("Int_ini");				//9
    analysisManager->CreateNtupleSColumn("Int_final");				//10


    analysisManager->FinishNtuple(2);
    analysisManager->SetNtupleFileName(2, "essn_ntuple_track");


  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNRunAction::~ESSNRunAction()
{
  fSDName.clear();
  delete G4RootAnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4Run* ESSNRunAction::GenerateRun()
{
  // Generate new RUN object, which is specially
  // dedicated for MultiFunctionalDetector scheme.
  //  Detail description can be found in wwgRun.hh/cc.
  return new ESSNRun(fSDName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;
  //inform the runManager to save random number seed
  //G4RunManager::GetRunManager()->SetRandomNumberStore(true);

  // Create an instance of G4TScoreHistFiller.
  auto histFiller = new G4TScoreHistFiller<G4RootAnalysisManager>;

  //if(NTUPLE) {

  // Get analysis manager
  auto analysisManager = G4RootAnalysisManager::Instance();

  // Create the 1D histograms for each volume cell
  G4int ijk = 0;
  for (int i=1; i<=xCells; i++) {
    for (int j=1; j<=yCells; j++) {
      for (int k=1; k<=zCells; k++) {
        // analysisManager->CreateH1(name, title,
        //                xdata.fNbins, xdata.fVmin*unit, xdata.fVmax*unit,
        //                xdata.fSunit, xdata.fSfcn, xdata.fSbinScheme);
        // virtual G4int CreateH1(const G4String& name, const G4String& title,
        //                G4int nbins, G4double xmin, G4double xmax,
        //                const G4String& unitName = "none",
        //                const G4String& fcnName = "none",
        //                const G4String& binScheme = "linear") final;
        G4String hname = "wenergy" + std::to_string(ijk);
        analysisManager->CreateH1(hname, "Weighted Energy", 50, 0.0*MeV, 3.0*MeV);
        G4cout << "-- Just created histogram with name " << hname << G4endl;
        ijk++;
      }
    }
  }

  // Open an output file
  // The default file name is set in ESSNRunAction::ESSNRunAction(),
  // it can be overwritten in a macro
  analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4cout << " ###### EndOfRunAction  " <<G4endl;

  //if(NTUPLE) {

  // save histograms & ntuple
  //
  auto analysisManager = G4RootAnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();


  //- ESSNRun object.
  ESSNRun* essnRun = (ESSNRun*)aRun;
  //--- Dump all socred quantities involved in ESSNRun.
  // re02Run->DumpAllScorer();
  //---
  G4RunManager* mgr = G4RunManager::GetRunManager();
  //

  //create a file for the flux and errors
  G4String fname = "Results_";
  fname += std::to_string(G4Threading::G4GetThreadId()) + ".txt";
  std::ofstream ResultsFile(fname);

  for ( G4int i = 0; i < (G4int)fSDName.size(); i++ ){
    const G4VUserDetectorConstruction* vdet =
                                       mgr->GetUserDetectorConstruction();
    ESSNImportanceDetectorConstruction* bdet =
                                      (ESSNImportanceDetectorConstruction*)vdet;
    //

    //---------------------------------------------
    // Dump accumulated quantities for this RUN.
    //  (Display only central region of x-y plane)
    //      0       ConcreteSD/Collisions
    //      1       ConcreteSD/CollWeight
    //      2       ConcreteSD/Population
    //      3       ConcreteSD/TrackEnter
    //      4       ConcreteSD/SL
    //      5       ConcreteSD/SLW
    //      6       ConcreteSD/SLWE
    //      7       ConcreteSD/SLW_V
    //      8       ConcreteSD/SLWE_V
    //      9       ConcreteSD/WeightEnter
    //---------------------------------------------
    G4THitsMap<G4double>* Collisions =
                          essnRun->GetHitsMap(fSDName[i]+"/Collisions");
    G4THitsMap<G4double>* Population =
                          essnRun->GetHitsMap(fSDName[i]+"/Population");
    G4THitsMap<G4double>* TrackEnter =
                          essnRun->GetHitsMap(fSDName[i]+"/TrackEnter");
    G4THitsMap<G4double>* SL = essnRun->GetHitsMap(fSDName[i]+"/SL");
    G4THitsMap<G4double>* SLW = essnRun->GetHitsMap(fSDName[i]+"/SLW");
    G4THitsMap<G4double>* WeightEnter =
      essnRun->GetHitsMap(fSDName[i]+"/WeightEnter");
    G4THitsMap<G4double>* WeightEnterSq =
        essnRun->GetHitsMap(fSDName[i]+"/WeightEnterSq");


    if (IsMaster())
      {
        G4cout <<
          "\n--------------------End of Global Run-----------------------" <<
        G4endl;
        G4cout <<
          " Number of event processed : "<< aRun->GetNumberOfEvent() << G4endl;
      }
    else
      {
        G4cout <<
          "\n--------------------End of Local Run------------------------" <<
        G4endl;
        G4cout <<
          " Number of event processed : "<< aRun->GetNumberOfEvent() << G4endl;
      }

    G4cout << "============================================================="
           <<G4endl;
    G4cout << "============================================================="
           <<G4endl;


    //error calculation

    G4cout << "Preparing error calc..." << G4endl;
    G4int nofEvents = aRun->GetNumberOfEvent();
    if (nofEvents == 0) return;
    G4double sqrt_n = std::sqrt(nofEvents);
    std::map<G4int,G4double> Rsum  = ESSNEventAction::Instance()->sum;// sum in RunAction
    std::map<G4int,G4double> Rsum2 = ESSNEventAction::Instance()->sum2;
    std::map<G4int,G4double> sigma,error;
    std::map<G4int,G4double>::iterator itr = Rsum.begin();
    G4cout << "Preparing sum calc..." << G4endl;

    for(; itr != Rsum.end(); itr++) {
      G4int j = itr->first;
      sigma[j] = Rsum2[j]/nofEvents-(Rsum[j]/nofEvents)*(Rsum[j]/nofEvents);
      if (sigma[j] > 0.) sigma[j] = std::sqrt(sigma[j]);
      else {
        sigma[j] = 0.;
        G4cout << "*** sigma = 0 with nofEvents = " << nofEvents << G4endl;
      }
      error[j] = sigma[j]/sqrt_n;
      error[j] = error[j]/Rsum[j]*nofEvents;
    }

    std::ostream *myout = &G4cout;
    PrintHeader(myout);

    G4cout << "Preparing cell loop..." << G4endl;
    for ( G4int iz = 0; iz < noCells; iz++){
      G4double* SumCollisions = (*Collisions)[iz];
      G4double* Populations   = (*Population)[iz];
      G4double* TrackEnters   = (*TrackEnter)[iz];
      G4double* SLs   = (*SL)[iz];
      G4double* SLWs   = (*SLW)[iz];
      G4double* WeightEnters   = (*WeightEnter)[iz];
      G4double* WeightEntersSq   = (*WeightEnterSq)[iz];
      if ( !SumCollisions ) SumCollisions = new G4double(0.0);
      if ( !Populations   ) Populations   = new G4double(0.0);
      if ( !TrackEnters   ) TrackEnters   = new G4double(0.0);
      if ( !SLs   ) SLs   = new G4double(0.0);
      if ( !SLWs   ) SLWs   = new G4double(0.0);
      if ( !WeightEnters   ) WeightEnters   = new G4double(0.0);
      if ( !WeightEntersSq   ) WeightEntersSq   = new G4double(0.0);

      // Compute errors
      G4double sig = 0.0, err = 0.0;
      if((*WeightEnters) > 0) {
        sig = (*WeightEntersSq)/nofEvents-((*WeightEnters)/nofEvents)*((*WeightEnters)/nofEvents);
        if (sig > 0.) sig = std::sqrt(sig);
        else {
          sig = 0.;
          G4cout << "*** sigma = 0 with nofEvents = " << nofEvents << G4endl;
        }
        err = sig/sqrt_n;
        err = err/(*WeightEnters)*nofEvents;
      }

      G4double AverageTrackWeight=0.0;
      if ( *SLs    !=0. ) AverageTrackWeight  = (*SLWs)/(*SLs);
      G4String cname = bdet->GetCellName(iz);
      G4cout
        << std::setw(fFieldValue) << cname << " |"
        << std::setw(fFieldValue) << (*TrackEnters) << " |"
        << std::setw(fFieldValue) << (*Populations) << " |"
        << std::setw(fFieldValue) << (*SumCollisions) << " |"
        << std::setw(fFieldValue) << AverageTrackWeight << " |"
        //<< std::setw(fFieldValue) << (*SLs) << " |"
        //<< std::setw(fFieldValue) << (*SLWs) << " |"
        << std::setw(fFieldValue) << (*WeightEnters) << " |"
        << std::setw(fFieldValue) <<  err << " |" // error[iz] << " |"
        << G4endl;

      //write the file
      ResultsFile << iz << " " << (*WeightEnters) << " " << err << G4endl; //" " << error[iz] << G4endl;

    }
    G4cout << "============================================="<<G4endl;
  }
  ResultsFile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNRunAction::PrintHeader(std::ostream *out)
{
  std::vector<G4String> vecScoreName;
  vecScoreName.push_back("MC.Tr.Entering");
  vecScoreName.push_back("Population");
  vecScoreName.push_back("Collisions");
  vecScoreName.push_back("Av.Tr.WGT");
  //vecScoreName.push_back("SL");
  //vecScoreName.push_back("SLW");
  vecScoreName.push_back("Analog.Tr.Entering");
  vecScoreName.push_back("RE.Analog.Tr");

  // head line
  // std::string vname;
  // vname = FillString("Volume", ' ', fFieldName+1);
  //*out << vname << '|';
  *out << std::setw(fFieldValue) << "Volume" << " |";
  for (std::vector<G4String>::iterator it = vecScoreName.begin();
      it != vecScoreName.end(); it++) {
//      vname = FillString((*it),
//                         ' ',
//                         fFieldValue+1,
//                         false);
//      *out << vname << '|';
      *out << std::setw(fFieldValue) << (*it) << " |";
  }
  *out << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::string ESSNRunAction::FillString(const std::string &name,
                                       char c, G4int n, G4bool back)
{
  std::string fname("");
  G4int k = n - name.size();
  if (k > 0) {
    if (back) {
      fname = name;
      fname += std::string(k,c);
    }
    else {
      fname = std::string(k,c);
      fname += name;
    }
  }
  else {
    fname = name;
  }
  return fname;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
