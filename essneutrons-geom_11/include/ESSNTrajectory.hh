/// \file ESSNTrajectory.hh
/// \brief Definition of the ESSNTrajectory class

#ifndef ESSNTrajectory_h
#define ESSNTrajectory_h 1

#include "G4ParticleDefinition.hh"
#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include "G4TrajectoryPoint.hh"

class G4Track;

// From G4Trajectory.hh
typedef std::vector<G4VTrajectoryPoint*>  TrajectoryPointContainer;

class ESSNTrajectory: public G4VTrajectory
{

public:

  // Default constructor
  ESSNTrajectory();
  // Constructor from G4Track
  ESSNTrajectory(const G4Track*);
  // Copy constructor
  ESSNTrajectory(const ESSNTrajectory&);
  // Destructor
  virtual ~ESSNTrajectory();

  // Operators (from G4Trajectory.hh)
  inline void* operator new(size_t);
  inline void  operator delete(void*);
  inline int operator == (const ESSNTrajectory& right) const
  {return (this==&right);}

  // Trajectory functions (from G4Trajectory.hh)
  virtual void AppendStep(const G4Step* aStep);
  virtual int GetPointEntries() const { return positionRecord->size(); }
  virtual G4VTrajectoryPoint* GetPoint(G4int i) const
  { return (*positionRecord)[i]; }
  virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

  // Getters and setters
  inline G4String GetParticleName() const
  { return fParticleDef->GetParticleName(); }

  inline G4double GetCharge() const
  { return fParticleDef->GetPDGCharge(); }

  inline G4double GetMass() const
  { return fParticleDef->GetPDGMass(); }

  inline G4int GetPDGEncoding() const
  { return fParticleDef->GetPDGEncoding(); }

  inline G4int GetTrackID() const
  { return fTrackID; }
  inline void SetTrackID(const G4int trackID)
  { fTrackID = trackID; }

  inline G4int GetParentID() const
  { return fParentID; }
  inline void SetParentID(const G4int parentID)
  { fParentID = parentID; }

  inline G4double GetInitialTime() const
  { return fTimeInit; }
  inline void SetInitialTime(const G4double tInit)
  { fTimeInit = tInit; }

  inline G4double GetFinalTime() const
  { return fTimeFinal; }
  inline void SetFinalTime(const G4double tFinal)
  { fTimeFinal = tFinal; }

  inline G4ThreeVector GetInitialMomentum() const
  { return fMomentumInit; }
  inline void SetInitialMomentum(const G4ThreeVector& pInit)
  { fMomentumInit = pInit; }

  inline G4ThreeVector GetFinalMomentum() const
  { return fMomentumFinal; }
  inline void SetFinalMomentum(const G4ThreeVector& pFinal)
  { fMomentumFinal = pFinal; }

  inline G4ThreeVector GetInitialPosition() const
  { return fPositionInit; }
  inline void SetInitialPosition(const G4ThreeVector& posInit)
  { fPositionInit = posInit; }

  inline G4ThreeVector GetFinalPosition() const
  { return fPositionFinal; }
  inline void SetFinalPosition(const G4ThreeVector& posFinal)
  { fPositionFinal = posFinal; }

  inline G4String GetInitialVolume() const
  { return fVolInit; }
  inline void SetInitialVolume(const G4String vInit)
  { fVolInit = vInit; }

  inline G4String GetFinalVolume() const
  { return fVolFinal; }
  inline void SetFinalVolume(const G4String vFinal)
  { fVolFinal = vFinal; }

private:

  TrajectoryPointContainer* positionRecord;  // from G4Trajectory.hh

  G4ParticleDefinition* fParticleDef;

  G4int fTrackID;
  G4int fParentID;

  G4double fTimeInit;
  G4double fTimeFinal;

  G4ThreeVector fMomentumInit;
  G4ThreeVector fMomentumFinal;

  G4ThreeVector fPositionInit;
  G4ThreeVector fPositionFinal;

  G4String fVolInit;
  G4String fVolFinal;

};

// -----------------------------------------------------------------------------
// From G4Trajectory.hh
#if defined G4TRACKING_ALLOC_EXPORT
  extern G4DLLEXPORT G4Allocator<ESSNTrajectory> aTrajectoryAllocator;
#else
  extern G4DLLIMPORT G4Allocator<ESSNTrajectory> aTrajectoryAllocator;
#endif

inline void* ESSNTrajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)aTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void ESSNTrajectory::operator delete(void* aTrajectory)
{
  aTrajectoryAllocator.FreeSingle((ESSNTrajectory*)aTrajectory);
}

#endif
