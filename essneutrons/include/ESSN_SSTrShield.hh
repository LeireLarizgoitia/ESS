#ifndef SSTrShield_H
#define SSTrShield_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

#include "ESSN_ConcTrShield.hh"
#include "ESSN_OutTrShield.hh"

class ESSN_SSTrShield//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  ESSN_SSTrShield();
  /// Destructor
  ~ESSN_SSTrShield();
  /// Returns the Position
  G4ThreeVector Position() const;
  G4double Radius() const;
  G4double Height() const;

  G4ThreeVector Tube_Rad() const;

  G4double TrShVac3_InRad() const;

  G4LogicalVolume* GetLogicalVolume() const;
  void SetLogicalVolume(G4LogicalVolume* lv);

  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  G4double         TrSh1_R1 = 273*cm;
  G4double         TrSh1_Z = (200*cm+TrSh1_Ztop)/2;
  G4double         TrSh1_dz = (200*cm-TrSh1_Z);
  G4ThreeVector    TrSh1_Ps = G4ThreeVector(0,0,TrSh1_dz);

  G4double         TrTubeSS_RMax = 5*cm;
  G4double         TrTubeBe_RMin = TrTubeSS_RMax;
  G4double         TrTubeBe_RMax = 20*cm;
  G4ThreeVector    Tube_rad_ = G4ThreeVector(TrTubeSS_RMax, TrTubeBe_RMin, TrTubeBe_RMax);

  G4double         TrShVac3_R1 = 268*cm;

  ESSN_ConcTrShield ConcTrShield_;
  G4double          TrShVac2_R1 = ConcTrShield_.TrShVac2_InRad();
  ESSN_OutTrShield  OutTrShield_;
  G4double          BeamTubeSS_R1 = OutTrShield_.BeamTubeCS_Rad()[2];
  G4double          OTrSh1_R1 = OutTrShield_.Dimension()[0];


};
inline G4ThreeVector    ESSN_SSTrShield::Position() const { return TrSh1_Ps; }
inline G4ThreeVector    ESSN_SSTrShield::Tube_Rad() const { return Tube_rad_; }

inline G4double         ESSN_SSTrShield::Radius() const { return TrSh1_R1; }
inline G4double         ESSN_SSTrShield::Height() const { return TrSh1_Z; }
inline G4double         ESSN_SSTrShield::TrShVac3_InRad() const { return TrShVac3_R1; }


inline G4LogicalVolume* ESSN_SSTrShield::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_SSTrShield::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
