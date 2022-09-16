/// \file ESSNEventAction.hh
/// \brief Definition of the ESSNEventAction class; adapted from
///        Geant4 example B5 B5EventAction.hh

#ifndef ESSNEventAction_h
#define ESSNEventAction_h 1


#include "G4UserEventAction.hh"
#include "globals.hh"

#include "G4ThreeVector.hh"

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

    //acumuladores: guardan la energía cinética de todas las tracks de neutrones en 1 evento y también el trackID
    std::vector<G4int> neutron_tracksID;
    std::vector<G4double> neutron_track_Ek;
    std::vector<G4ThreeVector> neutron_r;
    std::vector<G4double> neutron_track_Eloss;

    //guarda la lista de ID de las DIFERENTES tracks de un evento
    std::vector<G4int> track;

    //las energías cinéticas y posiciones en cada step para UN SOLO track
    std::vector<G4double> Etrack;
    std::vector<G4double> Eloss;
    std::vector<std::vector<G4double>> r { {}, {}, {} }; //r[0]=x r[1]=y r[2]=z

    std::vector<G4int>& GetTrackList(){return track;}
    std::vector<G4double>& GetEk(){return Etrack;}
    G4int status;

    G4String volfin, Int_ini, Int_fin;

    private:
    static ESSNEventAction* fgInstance;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
