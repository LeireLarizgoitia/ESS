/// \file ESSNwwPrimaryGeneratorAction.hh
/// \brief Definition of the ESSNwwPrimaryGeneratorAction class

#ifndef ESSNScanPrimaryGeneratorAction_hh
#define ESSNScanPrimaryGeneratorAction_hh 1

#include "G4VUserPrimaryGeneratorAction.hh"

class G4GeneralParticleSource;
class G4Event;
class G4ParticleGun;

class ESSNScanPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    ESSNScanPrimaryGeneratorAction();
    ~ESSNScanPrimaryGeneratorAction();

  public:
    virtual void GeneratePrimaries(G4Event* anEvent);

  private:
    G4ParticleGun* fParticleGun;

};

#endif
