/// \file ESSNEventAction.hh
/// \brief Definition of the ESSNEventAction class; adapted from
///        Geant4 example B5 B5EventAction.hh

#ifndef ESSNEventAction_h
#define ESSNEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"

#include <vector>
#include <map>

/// Event action
class ESSNEventAction : public G4UserEventAction
{
public:
    ESSNEventAction();
    virtual ~ESSNEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    std::map<G4int,G4double> sum;
    std::map<G4int,G4double> sum2;

    static ESSNEventAction* Instance();

    private:
    static ESSNEventAction* fgInstance;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
