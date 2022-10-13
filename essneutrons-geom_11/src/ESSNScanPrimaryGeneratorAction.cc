/// \file ESSNScanPrimaryGeneratorAction.cc
/// \brief Implementation of the ESSNScanPrimaryGeneratorAction class

#include "globals.hh"

#include "ESSNScanPrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4Neutron.hh"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "G4GeneralParticleSource.hh"
#include "G4Geantino.hh"
#include "G4Proton.hh"
#include "ESSNParam.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


ESSNScanPrimaryGeneratorAction::ESSNScanPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(), fParticleGun(0)
{
  // Geometry scan
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fParticleGun->SetParticleDefinition(G4Geantino::GeantinoDefinition());

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNScanPrimaryGeneratorAction::~ESSNScanPrimaryGeneratorAction()
{
  delete fParticleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNScanPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  // geometry scan
  G4double pos = -worldX/2.0;
  G4double end = worldZ/2.0;
  G4double inc =1.0;
  while(pos <= end){
    fParticleGun->SetParticlePosition(G4ThreeVector(pos*cm, yPlane*cm, -end*cm));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0));
    fParticleGun->GeneratePrimaryVertex(anEvent);
    pos = pos +inc;
    //G4cout << pos << G4endl;
  }

  pos = -worldX/2.0;
  end = worldZ/2.0;
  inc =1.0;
  while(pos <= end){
    fParticleGun->SetParticlePosition(G4ThreeVector(-end*cm, yPlane*cm, pos*cm));
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0.0, 0.0));
    fParticleGun->GeneratePrimaryVertex(anEvent);
    pos = pos +inc;
    //G4cout << pos << G4endl;
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
