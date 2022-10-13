/// \file ESSNRunAction.hh
/// \brief Definition of the ESSNRunAction class; adapted from
///        Geant4 examples B5 B5RunAction.hh and B02 B02RunAction.hh

#ifndef ESSNRunAction_h
#define ESSNRunAction_h 1

#include "G4UserRunAction.hh"
//#include "ESSNHistoManager.hh"
#include "globals.hh"
#include <vector>

class ESSNEventAction;
class G4Run;

/// Run action class
class ESSNRunAction : public G4UserRunAction
{
  public:
    ESSNRunAction(ESSNEventAction* eventAction);
    virtual ~ESSNRunAction();

    virtual G4Run* GenerateRun();
    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

    void PrintHeader(std::ostream *out);
    std::string FillString(const std::string &name, char c, G4int n,
                           G4bool back=true);

  private:

    ESSNEventAction* fEventAction;

    // Data member
    // - vector of MultiFunctionalDetecor names.
    std::vector<G4String> fSDName;
    G4int fFieldValue;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
