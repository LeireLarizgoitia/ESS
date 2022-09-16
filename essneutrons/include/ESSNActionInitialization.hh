/// \file ESSNActionInitialization.hh
/// \brief Definition of the ESSNActionInitialization class; adapted from
///        Geant4 example B5 B5ActionInitialization.hh

#ifndef ESSNActionInitialization_h
#define ESSNActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

/// Action initialization class
class ESSNActionInitialization : public G4VUserActionInitialization
{
  public:
    ESSNActionInitialization();
    virtual ~ESSNActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
