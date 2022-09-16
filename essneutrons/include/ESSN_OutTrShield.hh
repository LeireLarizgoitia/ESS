#ifndef OutTrShield_H
#define OutTrShield_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

class ESSN_OutTrShield//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  ESSN_OutTrShield();
  /// Destructor
  ~ESSN_OutTrShield();
  /// Returns the Position
  G4ThreeVector Dimension() const;
  G4ThreeVector BeamTubeCS_Rad() const;
  G4ThreeVector Position() const;

  G4LogicalVolume* GetLogicalVolume() const;
  void SetLogicalVolume(G4LogicalVolume* lv);

  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  G4double         OTrSh1_R1 = 273*cm;
  G4double         OTrSh1_R2 = 550*cm;
  G4double         OTrSh1_Z = (TrSh1_Ztop+76*cm)/2;
  G4ThreeVector    OTrSh1_dim_ = G4ThreeVector(OTrSh1_R1,OTrSh1_R2,OTrSh1_Z);

  G4double         OTrSh1_dz = (76*cm-OTrSh1_Z);
  G4ThreeVector    OTrSh1_Ps = G4ThreeVector(0,0,OTrSh1_dz);

  G4double         BeamTubeCS_R1 = 10.5*cm;
  G4double         BeamTubeCS_R2 = 14.5*cm;
  G4double         BeamTubeCS_R3 = 16*cm;
  G4ThreeVector    BeamTubeCS_rad_ = G4ThreeVector(BeamTubeCS_R1,BeamTubeCS_R2,BeamTubeCS_R3);


};
inline G4ThreeVector    ESSN_OutTrShield::Position() const { return OTrSh1_Ps; }
inline G4ThreeVector    ESSN_OutTrShield::Dimension() const { return OTrSh1_dim_; }
inline G4ThreeVector    ESSN_OutTrShield::BeamTubeCS_Rad() const { return BeamTubeCS_rad_; }

inline G4LogicalVolume* ESSN_OutTrShield::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_OutTrShield::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
