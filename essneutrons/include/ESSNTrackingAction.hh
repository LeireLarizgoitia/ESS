/// \file ESSNTrackingAction.hh
/// \brief Definition of the ESSNTrackingAction class

#ifndef ESSNTrackingAction_h
#define ESSNTrackingAction_h 1

#include "G4UserTrackingAction.hh"

class G4VTrajectory;

class ESSNTrackingAction: public G4UserTrackingAction
{

public:

  // Default constructor
  ESSNTrackingAction();
  // Destructor
  virtual ~ESSNTrackingAction();

  static void ClearTrajectoryMap();

  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

private:

  static std::map<int, G4VTrajectory*> fTrajectoryMap;
};

#endif
