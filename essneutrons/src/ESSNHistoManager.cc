//
// ***************************************************************************
// * ESSNHistoManager class is created in the previous implementation        *
// * of the code to manage histogram data. Now we are interested to          *
// * get Ntuple used in post processing with the settings in ESSNAnalysis.hh *                                           *
// ***************************************************************************
//
/// \file eventgenerator/exgps/src/HistoManager.cc
/// \brief Implementation of the HistoManager class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ESSNHistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNHistoManager::ESSNHistoManager()
  : fFileName("exgps")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNHistoManager::~ESSNHistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNHistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  //
  G4AnalysisManager* analysis = G4AnalysisManager::Instance();

  analysis->SetFileName(fFileName);
  analysis->SetVerboseLevel(1);
  analysis->SetActivation(true);     //enable inactivation of histos, nTuples

  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated
  // as we have not yet set nbins, vmin, vmax
  //
  analysis->SetHistoDirectoryName("histo");
  analysis->SetFirstHistoId(1);

  G4int id = analysis->CreateH1("h1.1","kinetic energy", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);

  id = analysis->CreateH1("h1.2","vertex dist dN/dv = f(r)", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);

  id = analysis->CreateH1("h1.3","direction: cos(theta)", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);

  id = analysis->CreateH1("h1.4","direction: phi", nbins, vmin, vmax);
  analysis->SetH1Activation(id, false);

  // histos 2D
  //
  id = analysis->CreateH2("h2.1","vertex: XY",nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.2","vertex: YZ",nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.3","vertex: ZX",nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.4","direction: phi-cos(theta)",
                                          nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  id = analysis->CreateH2("h2.5","direction: phi-theta",
                                          nbins,vmin,vmax, nbins,vmin,vmax);
  analysis->SetH2Activation(id, false);

  // nTuples
  //
  analysis->SetNtupleDirectoryName("ntuple");
  analysis->SetFirstNtupleId(1);
  analysis->CreateNtuple("101", "Primary Particle Tuple");
  analysis->CreateNtupleIColumn("particleID");    //column 0
  analysis->CreateNtupleDColumn("Ekin");          //column 1
  analysis->CreateNtupleDColumn("posX");          //column 2
  analysis->CreateNtupleDColumn("posY");          //column 3
  analysis->CreateNtupleDColumn("posZ");          //column 4
  analysis->CreateNtupleDColumn("dirTheta");      //column 5
  analysis->CreateNtupleDColumn("dirPhi");        //column 6
  analysis->CreateNtupleDColumn("weight");        //column 7
  analysis->FinishNtuple();

  analysis->SetNtupleActivation(false);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
