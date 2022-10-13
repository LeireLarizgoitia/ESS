// ***************************************************************************
// * ESSNHistoManager class is created in the previous implementation        *
// * of the code to manage histogram data. Now we are interested to          *
// * get Ntuple used in post processing with the settings in ESSNAnalysis.hh *                                           *
// ***************************************************************************
//
/// \file eventgenerator/exgps/include/HistoManager.hh
/// \brief Definition of the HistoManager class
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"

#include "G4AnalysisManager.hh"
//#include "g4xml.hh"
//#include "g4csv.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ESSNHistoManager
{
  public:
   ESSNHistoManager();
  ~ESSNHistoManager();

  private:
    void Book();
    G4String fFileName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
