/// \file ESSNActionInitialization.cc
/// \brief Implementation of the ESSNActionInitialization class; adapted from
///        Geant4 example B5 B5ActionInitialization.cc

#include "ESSNActionInitialization.hh"
#include "ESSNPrimaryGeneratorAction.hh"
#include "ESSNRunAction.hh"
#include "ESSNEventAction.hh"
#include "ESSNTrackingAction.hh"
#include "ESSNSteppingAction.hh"
#include "ESSNScanPrimaryGeneratorAction.hh"
#include "ESSNParam.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNActionInitialization::ESSNActionInitialization()
 : G4VUserActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNActionInitialization::~ESSNActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNActionInitialization::BuildForMaster() const
{
  ESSNEventAction* eventAction = new ESSNEventAction;
  SetUserAction(new ESSNRunAction(eventAction));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNActionInitialization::Build() const
{

  if(SCAN==false) {
    auto eventAction = new ESSNEventAction;
    SetUserAction(eventAction);
    SetUserAction(new ESSNRunAction(eventAction));
    SetUserAction(new ESSNPrimaryGeneratorAction);

    if(NTUPLE) {
      SetUserAction(new ESSNTrackingAction());
    }
  }

  if(SCAN==true) {
    SetUserAction(new ESSNSteppingAction);
    SetUserAction(new ESSNScanPrimaryGeneratorAction);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
