#ifndef BunkerS_H
#define BunkerS_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "ESSN_BunkerL.hh"

class ESSN_BunkerS//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
    ESSN_BunkerS();
  /// Destructor
  ~ESSN_BunkerS();

  G4LogicalVolume* GetLogicalVolume() const;
  void SetLogicalVolume(G4LogicalVolume* lv);
  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  ESSN_BunkerL BunkerL_;
  G4double Bunker_Z =  BunkerL_.Bunk_height();
  //G4double Bunk_dz = BunkerL_.Position()[2];

  G4ThreeVector ConcBunkFloorIn_Ps = BunkerL_.ConcBunkFloorIn_Position();
  G4double ConcBunkFloorIn_Z = BunkerL_.ConcBunkFloorIn_height();

  G4ThreeVector BunkAirIn_Ps = BunkerL_.BunkAirIn_Position();
  G4double BunkAirIn_Z = BunkerL_.BunkAirIn_height();

};

inline G4LogicalVolume* ESSN_BunkerS::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_BunkerS::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
