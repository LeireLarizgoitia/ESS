/// \file ESSNPrimaryGeneratorAction.hh
/// \brief Implementation of the ESSNPrimaryGeneratorAction class; adapted from
///        Geant4 example B5 B5PrimaryGeneratorAction.hh

#ifndef ESSNPrimaryGeneratorAction_h
#define ESSNPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
//#include "G4ParticleGun.hh"

class G4GeneralParticleSource;
class G4Event;

/// Primary generator
///
/// A single particle is generated.
/// User can select
/// - the initial momentum and angle
/// - the momentum and angle spreads
/// - random selection of a particle type from proton, kaon+, pi+, muon+, e+


class ESSNPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ESSNPrimaryGeneratorAction();
   ~ESSNPrimaryGeneratorAction();

   virtual void GeneratePrimaries(G4Event*);
   //G4ParticleGun* GetParticleGun() { return fParticleGun;} ;

  private:
    G4GeneralParticleSource* fParticleGun;
    //G4ParticleGun*  fParticleGun;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
