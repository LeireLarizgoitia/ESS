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
#include "G4SystemOfUnits.hh"

#include "G4AnalysisManager.hh"
#include "G4SteppingManager.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4RootAnalysisManager.hh"


using namespace std;
using std::array;
using std::vector;

//María: líneas de archivo de salida coordinates comentadas
//std::ofstream coordsFile("coordinates.txt");

//María: archivo de salida de comprobación
std::ofstream neutronFile("neutrons.txt");

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ESSNSteppingAction::ESSNSteppingAction(ESSNEventAction* eventAction)
  : G4UserSteppingAction()
{
  fEventAction = eventAction;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
ESSNSteppingAction::~ESSNSteppingAction()
{
  //coordsFile.close();
  neutronFile.close();
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ESSNSteppingAction::UserSteppingAction(const G4Step * theStep)
{
  /*
  // Suspend a track if it is entering into the calorimeter

  // check if it is alive
  G4Track * theTrack = theStep->GetTrack();
  if(theTrack->GetTrackStatus()!=fAlive) { return; }

  // get cordinate position
  //G4StepPoint * thePrePoint = theStep->GetPreStepPoint();

  G4ThreeVector position = theStep->GetPostStepPoint()->GetPosition();
  coordsFile << position.getX() << " " << position.getZ() << G4endl;

  G4cout << theStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()  << G4endl;
  */

  //María
  auto analysisManager = G4RootAnalysisManager::Instance();
  const G4Event* event = G4RunManager::GetRunManager()->GetCurrentEvent();
  G4int eventID = event->GetEventID();
  G4Track *theTrack = theStep->GetTrack();
  //if(theTrack->GetTrackStatus()!=fAlive) { return; }

  G4StepPoint *thePrePoint = theStep->GetPreStepPoint();
  G4StepPoint *thePostPoint = theStep->GetPostStepPoint();
  G4String Particle = theTrack->GetParticleDefinition()->GetParticleName();
  G4String neutron = "neutron";
  G4ThreeVector pre = thePrePoint->GetPosition();
  G4ThreeVector pos = thePostPoint->GetPosition();

  G4TrackStatus estado;

  if (Particle == neutron){
  estado = theTrack->GetTrackStatus();

    if(estado == fAlive){fEventAction->status=1;}
    else{
    	if(estado == fStopButAlive){fEventAction->status=2;}
    	else{
    		fEventAction->status=3;
    	}
    }

      analysisManager->FillNtupleIColumn(1,0,eventID);
  	  analysisManager->FillNtupleSColumn(1,1,Particle);
  	  analysisManager->FillNtupleIColumn(1,2,theTrack->GetTrackID());
  	  analysisManager->FillNtupleIColumn(1,3,theTrack->GetParentID());
  	  analysisManager->FillNtupleDColumn(1,5,(theStep->GetTotalEnergyDeposit())/keV);
  	  analysisManager->FillNtupleDColumn(1,4,(theStep->GetDeltaEnergy())/keV);
  	  analysisManager->FillNtupleDColumn(1,6,(theTrack->GetKineticEnergy())/keV);
  	  analysisManager->FillNtupleSColumn(1,7,thePrePoint->GetPhysicalVolume()->GetName());
  	  analysisManager->FillNtupleSColumn(1,8,thePostPoint->GetPhysicalVolume()->GetName());
  	  analysisManager->FillNtupleDColumn(1,9, sqrt(pre[0]*pre[0] + pre[1]*pre[1] + pre[2]*pre[2]));
  	  analysisManager->FillNtupleDColumn(1,10, sqrt(pos[0]*pos[0] + pos[1]*pos[1] + pos[2]*pos[2]));
  	  analysisManager->AddNtupleRow(1);//añadimos fila por cada step


  	//comprobación en file neutron.txt

  	//neutronFile << Particle << " " << eventID << " " << theTrack->GetTrackID() << " " << theTrack->GetParentID() << " " <<
  	//(theTrack->GetKineticEnergy())/keV << " " << theTrack->GetCreatorProcess()->GetProcessName() << " " << thePostPoint->GetProcessDefinedStep()->GetProcessName() << " " << fEventAction->status << G4endl;

    fEventAction->volfin = thePostPoint->GetPhysicalVolume()->GetName();
    fEventAction->Int_ini = theTrack->GetCreatorProcess()->GetProcessName();
    fEventAction->Int_fin = thePostPoint->GetProcessDefinedStep()->GetProcessName();

    fEventAction->neutron_tracksID.push_back(theTrack->GetTrackID());
    fEventAction->neutron_track_Ek.push_back((theTrack->GetKineticEnergy())/keV);
    fEventAction->neutron_r.push_back(theTrack->GetPosition());
    fEventAction->neutron_track_Eloss.push_back(theStep->GetDeltaEnergy()/keV);

   }

}
