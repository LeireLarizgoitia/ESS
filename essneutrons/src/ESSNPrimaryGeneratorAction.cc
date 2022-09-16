/// \file ESSNPrimaryGeneratorAction.cc
/// \brief Implementation of the ESSNPrimaryGeneratorAction class; adapted from
///        Geant4 example B5 B5PrimaryGeneratorAction.cc

#include "ESSNPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Neutron.hh"
#include "G4Proton.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNPrimaryGeneratorAction::ESSNPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   fParticleGun(0)
{
  fParticleGun = new G4GeneralParticleSource();

  //G4int n_particle = 1;
  //fParticleGun  = new G4ParticleGun(n_particle);

  //fParticleGun->SetParticleDefinition(G4Proton::ProtonDefinition());
  //fParticleGun->SetParticleEnergy(2.0*GeV);
  //fParticleGun->SetParticlePosition(G4ThreeVector(0.0, -20.*cm, 0.0));
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, 1.0, 0.0));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNPrimaryGeneratorAction::~ESSNPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //create vertex
  //
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
