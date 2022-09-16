#ifndef BunkerL_H
#define BunkerL_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

class ESSN_BunkerL//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  ESSN_BunkerL();
  /// Destructor
  ~ESSN_BunkerL();
  /// Returns the Position
  G4ThreeVector Position() const;
  G4ThreeVector ConcBunkFloorIn_Position() const;
  G4ThreeVector BunkAirIn_Position() const;

  G4double Bunk_height() const;
  G4double ConcBunkFloorIn_height() const;
  G4double BunkAirIn_height() const;

  G4LogicalVolume* GetLogicalVolume() const;
  void SetLogicalVolume(G4LogicalVolume* lv);

  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  G4double Bunk_Z = (180*cm+150*cm)/2;
  G4double Bunk_dz = 150*cm-Bunk_Z;
  G4ThreeVector Bunk_Ps = G4ThreeVector(0,0,Bunk_dz);

  G4double ConcBunkFloorIn_Z = ConcBase_Z/2;
  G4double ConcBunkFloorIn_dz = 150*cm-ConcBunkFloorIn_Z;
  G4ThreeVector ConcBunkFloorIn_Ps = G4ThreeVector(0,0,ConcBunkFloorIn_dz-Bunk_dz);

  G4double BunkAirIn_Z = (180*cm+125*cm)/2;
  G4double BunkAirIn_dz = 125*cm-BunkAirIn_Z;
  G4ThreeVector BunkAirIn_Ps = G4ThreeVector(0,0,BunkAirIn_dz-Bunk_dz);


  //G4ThreeVector    Bunk_Ps;
  //G4ThreeVector    ConcBunkFloorIn_Ps;
  //G4ThreeVector    BunkAirIn_Ps;

  //G4double         Bunk_Z;
  //G4double         ConcBunkFloorIn_Z;
  //G4double         BunkAirIn_Z;

};
inline G4ThreeVector    ESSN_BunkerL::Position() const { return Bunk_Ps; }
inline G4ThreeVector    ESSN_BunkerL::ConcBunkFloorIn_Position() const { return ConcBunkFloorIn_Ps; }
inline G4ThreeVector    ESSN_BunkerL::BunkAirIn_Position() const { return BunkAirIn_Ps; }

inline G4double         ESSN_BunkerL::Bunk_height() const { return Bunk_Z; }
inline G4double         ESSN_BunkerL::ConcBunkFloorIn_height() const { return ConcBunkFloorIn_Z; }
inline G4double         ESSN_BunkerL::BunkAirIn_height() const { return BunkAirIn_Z; }


inline G4LogicalVolume* ESSN_BunkerL::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_BunkerL::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
