/// \file ESSNTrajectory.cc
/// \brief Implementation of the ESSNTrajectory class

#include "ESSNTrajectory.hh"
#include "G4Track.hh"

G4Allocator<ESSNTrajectory> aTrajectoryAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNTrajectory::ESSNTrajectory(): G4VTrajectory(), fParticleDef(0),
fTrackID(-1), fParentID(-1), fTimeInit(0.), fTimeFinal(0.)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNTrajectory::ESSNTrajectory(const G4Track* track): G4VTrajectory()
{

  // Record relevant information from the input G4Track.
  fParticleDef  = track->GetDefinition();
  fTrackID      = track->GetTrackID();
  fParentID     = track->GetParentID();
  fMomentumInit = track->GetMomentum();
  fPositionInit = track->GetVertexPosition();
  fVolInit      = track->GetVolume()->GetName();
  fTimeInit     = track->GetGlobalTime();

  // Initialize the vector of trajectory points: from G4Trajectory.cc
  positionRecord = new TrajectoryPointContainer();
  positionRecord->push_back(new G4TrajectoryPoint(track->GetPosition()));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNTrajectory::ESSNTrajectory(const ESSNTrajectory& other): G4VTrajectory()
{
  fParticleDef = other.fParticleDef;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNTrajectory::~ESSNTrajectory()
{
  // From G4Trajectory.cc
  if (positionRecord) {
    //  positionRecord->clearAndDestroy();
    size_t i;
    for(i=0;i<positionRecord->size();i++){
      delete  (*positionRecord)[i];
    }
    positionRecord->clear();
    delete positionRecord;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// -----------------------------------------------------------------------------
// From G4Trajectory.cc
void ESSNTrajectory::AppendStep(const G4Step* aStep)
{
  positionRecord->push_back( new G4TrajectoryPoint(aStep->GetPostStepPoint()->
                                  GetPosition() ));
}

void ESSNTrajectory::MergeTrajectory(G4VTrajectory* secondTrajectory)
{
  if(!secondTrajectory) return;

  ESSNTrajectory* seco = (ESSNTrajectory*)secondTrajectory;
  G4int ent = seco->GetPointEntries();
  for(G4int i=1;i<ent;i++) // initial point of the second trajectory should not be merged
  {
    positionRecord->push_back((*(seco->positionRecord))[i]);
    //    positionRecord->push_back(seco->positionRecord->removeAt(1));
  }
  delete (*seco->positionRecord)[0];
  seco->positionRecord->clear();
}
