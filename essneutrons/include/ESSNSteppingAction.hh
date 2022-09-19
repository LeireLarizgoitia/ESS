/// \file ESSNSteppingAction.hh
/// \brief Definition of the ESSNSteppingAction class; adapted from
///        RE01SteppingAction.hh

#ifndef ESSNSteppingAction_h
#define ESSNSteppingAction_h 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class ESSNSteppingAction : public G4UserSteppingAction
{
public:
  ESSNSteppingAction();
  virtual ~ESSNSteppingAction();

  virtual void UserSteppingAction(const G4Step*);
};

#endif
