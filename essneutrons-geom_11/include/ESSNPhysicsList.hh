/// \file ESSNPhysicsList.hh
/// \brief Definition of the ESSNPhysicsList class; adapted from
///        Geant4 example B5 B5PhysicsList.cc

#ifndef ESSNPhysicsList_h
#define ESSNPhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"

/// Modular physics list
class ESSNPhysicsList: public G4VModularPhysicsList
{
public:
  ESSNPhysicsList();
  virtual ~ESSNPhysicsList();

  //virtual void ConstructParticle();
  //virtual void ConstructProcess();
  virtual void SetCuts();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
