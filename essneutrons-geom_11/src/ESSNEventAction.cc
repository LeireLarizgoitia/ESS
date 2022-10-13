/// \file ESSNEventAction.cc
/// \brief Implementation of the ESSNEventAction class; adapted from
///        Geant4 example B5 B5EventAction.cc

#include "ESSNEventAction.hh"
#include "ESSNTrajectory.hh"
#include "ESSNTrackingAction.hh"
#include "ESSNParam.hh"
#include "ESSNAnalysis.hh"
#include "ESSNSteppingAction.hh"
#include "ESSNRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4VHitsCollection.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTrajectory.hh"
#include "G4TrajectoryContainer.hh"
#include "G4ios.hh"
//#include "g4analysis.hh"

using std::array;
using std::vector;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNEventAction::ESSNEventAction()
: G4UserEventAction()
      // std::array<T, N> is an aggregate that contains a C array.
      // To initialize it, we need outer braces for the class itself
      // and inner braces for the C array
{

  neutron_tracksID.clear();
  neutron_r.clear();
  neutron_track_Ek.clear();
  neutron_track_Eloss.clear();
  r[0].clear();
  r[1].clear();
  r[2].clear();
  track.clear();
  Etrack.clear();
  Eloss.clear();

  // set printing per each event
  G4RunManager::GetRunManager()->SetPrintProgress(1);
  fgInstance = this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNEventAction::~ESSNEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNEventAction* ESSNEventAction::fgInstance;
ESSNEventAction* ESSNEventAction::Instance()
{
    return fgInstance;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNEventAction::BeginOfEventAction(const G4Event*)
{
  // Find hit collections and histogram Ids by names (just once)
  // and save them in the data members of this class

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNEventAction::EndOfEventAction(const G4Event* event)
{

  // ---------------------------------------------------------------------------
  // Sum hits
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if(!HCE) return;
  G4SDManager* SDMan = G4SDManager::GetSDMpointer();
  G4int colID = SDMan->GetCollectionID("ConcreteSD/WeightEnter");
  G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(colID));
  std::map<G4int,G4double*>::iterator itr = evtMap->GetMap()->begin();
  for(; itr != evtMap->GetMap()->end(); itr++) {
      sum[itr->first] += *(itr->second);
      sum2[itr->first] += (*(itr->second))*(*(itr->second));
  }

  // ---------------------------------------------------------------------------
  // Fill histograms & ntuple
  if(NTUPLE) {

    // Get analysis manager
    auto analysisManager = G4RootAnalysisManager::Instance();
    G4cout << "END OF EVENT" << G4endl;

    // Loop through all trajectories.
    G4TrajectoryContainer * tc = event->GetTrajectoryContainer();
    if(tc) {
      for(unsigned int i = 0; i < tc->size(); i++) {
        ESSNTrajectory * tr = dynamic_cast<ESSNTrajectory*>((*tc)[i]);

        G4ThreeVector posInit = tr->GetInitialPosition();
        G4ThreeVector posFinal = tr->GetFinalPosition();
        G4ThreeVector momentumInit = tr->GetInitialMomentum();
        G4ThreeVector momentumFinal = tr->GetFinalMomentum();

        // Fill all columns in the NTuple.
        analysisManager->FillNtupleIColumn(0,event->GetEventID());
        analysisManager->FillNtupleSColumn(1,tr->GetParticleName());
        analysisManager->FillNtupleIColumn(2,tr->GetTrackID());
        analysisManager->FillNtupleIColumn(3,tr->GetParentID());
        analysisManager->FillNtupleDColumn(4,tr->GetMass());
        analysisManager->FillNtupleDColumn(5,posInit[0]);
        analysisManager->FillNtupleDColumn(6,posInit[1]);
        analysisManager->FillNtupleDColumn(7,posInit[2]);
        analysisManager->FillNtupleDColumn(8,tr->GetInitialTime());
        analysisManager->FillNtupleDColumn(9,posFinal[0]);
        analysisManager->FillNtupleDColumn(10,posFinal[1]);
        analysisManager->FillNtupleDColumn(11,posFinal[2]);
        analysisManager->FillNtupleDColumn(12,tr->GetFinalTime());
        analysisManager->FillNtupleDColumn(13,momentumInit[0]);
        analysisManager->FillNtupleDColumn(14,momentumInit[1]);
        analysisManager->FillNtupleDColumn(15,momentumInit[2]);
        analysisManager->FillNtupleDColumn(16,momentumFinal[0]);
        analysisManager->FillNtupleDColumn(17,momentumFinal[1]);
        analysisManager->FillNtupleDColumn(18,momentumFinal[2]);
        analysisManager->FillNtupleSColumn(19,tr->GetInitialVolume());
        analysisManager->FillNtupleSColumn(20,tr->GetFinalVolume());

        // Add an NTuple row.
        analysisManager->AddNtupleRow();
      }
    }

    // Clear the trajectory map.
    ESSNTrackingAction::ClearTrajectoryMap();


    //María: analisis de la energía depositada y los tracks de un evento
    long unsigned int i,j,k;
    i = 0;
    while (i < neutron_tracksID.size()){
    	k = i + 1;
    	while (neutron_tracksID[i] == neutron_tracksID[k] && k < neutron_tracksID.size()){
    		k = k + 1;
    	}
    	//salgo del bucle: he llegado al último, me guardo el ID del track
    	track.push_back(neutron_tracksID[i]);
    	i = k;
    }

    for (i = 0; i < track.size(); i++){

    	for(j = 0;j<neutron_tracksID.size();j++){

    		if(neutron_tracksID[j] == track[i]){
    			Etrack.push_back(neutron_track_Ek[j]);
    			r[0].push_back(neutron_r[j][0]);
    			r[1].push_back(neutron_r[j][1]);
    			r[2].push_back(neutron_r[j][2]);
    			Eloss.push_back(neutron_track_Eloss[j]);

    		}
    	}
    	analysisManager->FillNtupleIColumn(2,0,event->GetEventID());
    	analysisManager->FillNtupleIColumn(2,1,track[i]);
    	analysisManager->FillNtupleIColumn(2,7,status);
    	analysisManager->FillNtupleSColumn(2,8,volfin);
    	analysisManager->FillNtupleSColumn(2,9,Int_ini);
    	analysisManager->FillNtupleSColumn(2,10,Int_fin);


    	analysisManager->AddNtupleRow(2);

    	//limpiamos los vectores
    	Etrack.clear();
    	Eloss.clear();
    	r[0].clear();
    	r[1].clear();
    	r[2].clear();

    }

    track.clear();
    neutron_tracksID.clear();
    neutron_track_Ek.clear();
    neutron_r.clear();
    neutron_track_Eloss.clear();

  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
