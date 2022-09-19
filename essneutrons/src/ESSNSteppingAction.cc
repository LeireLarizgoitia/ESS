/// \file ESSNSteppingAction.cc
/// \brief Implementation of the ESSNSteppingAction class; adapted from
///        RE01SteppingAction.cc

#include "ESSNSteppingAction.hh"

#include "G4Track.hh"
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Region.hh"

std::ofstream coordsFile("coordinates.txt");

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ESSNSteppingAction::ESSNSteppingAction()
  : G4UserSteppingAction()
{;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ESSNSteppingAction::~ESSNSteppingAction()
{
  coordsFile.close();
;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ESSNSteppingAction::UserSteppingAction(const G4Step * theStep)
{
  // Suspend a track if it is entering into the calorimeter

  // check if it is alive
  G4Track * theTrack = theStep->GetTrack();
  if(theTrack->GetTrackStatus()!=fAlive) { return; }

  // get cordinate position
  //G4StepPoint * thePrePoint = theStep->GetPreStepPoint();

  G4ThreeVector position = theStep->GetPostStepPoint()->GetPosition();
  coordsFile << position.getX() << " " << position.getZ() << G4endl;

  G4cout << theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()  << G4endl;


}
