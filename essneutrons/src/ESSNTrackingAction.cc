/// \file ESSNTrackingAction.cc
/// \brief Implementation of the ESSNTrackingAction class

#include "ESSNTrajectory.hh"
#include "ESSNTrackingAction.hh"

#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTrajectory.hh"
#include "G4TrajectoryContainer.hh"

using std::array;
using std::vector;

std::map<int, G4VTrajectory*> ESSNTrackingAction::fTrajectoryMap;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNTrackingAction::ESSNTrackingAction()
: G4UserTrackingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNTrackingAction::~ESSNTrackingAction()
{
  fTrajectoryMap.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNTrackingAction::PreUserTrackingAction(const G4Track* track)
{
  // Create a new trajectory for this track.
  G4VTrajectory * tr = new ESSNTrajectory(track);

  fpTrackingManager->SetStoreTrajectory(true);
  fpTrackingManager->SetTrajectory(tr);

  // Add this trajectory to the map, if it is not already there.
  G4int trkID = track->GetTrackID();
  std::map<int, G4VTrajectory*>::iterator it = fTrajectoryMap.find(trkID);
  if(it == fTrajectoryMap.end()) {
    fTrajectoryMap[trkID] = tr;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNTrackingAction::PostUserTrackingAction(const G4Track* track)
{
  // Set the end-of-track information for the corresponding trajectory.
  std::map<int, G4VTrajectory*>::iterator it = fTrajectoryMap.find(track->GetTrackID());
  if(it != fTrajectoryMap.end()) {
    ESSNTrajectory * tr = (ESSNTrajectory*) it->second;

    tr->SetFinalTime(track->GetGlobalTime());
    tr->SetFinalMomentum(track->GetMomentum());
    tr->SetFinalVolume(track->GetVolume()->GetName());
    tr->SetFinalPosition(track->GetPosition());
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNTrackingAction::ClearTrajectoryMap()
{
  fTrajectoryMap.clear();
}
