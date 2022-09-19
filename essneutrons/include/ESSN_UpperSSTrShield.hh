#ifndef UpperSSTrShield_H
#define UpperSSTrShield_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

#include "ESSN_ConcTrShield.hh"
#include "ESSN_SSTrShield.hh"

class ESSN_UpperSSTrShield//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  ESSN_UpperSSTrShield();
  /// Destructor
  ~ESSN_UpperSSTrShield();
  /// Returns the Position
  G4ThreeVector Position() const;

  G4LogicalVolume* GetLogicalVolume() const;
  void SetLogicalVolume(G4LogicalVolume* lv);

  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  G4double         TrSh2_Z = (400*cm-TrSh1_Ztop)/2;
  G4double         TrSh2_dz = -(400*cm-TrSh2_Z);
  G4ThreeVector    TrSh2_Ps = G4ThreeVector(0,0,TrSh2_dz);

  ESSN_ConcTrShield ConcTrShield_;
  G4double          TrShVac2_R1 = ConcTrShield_.TrShVac2_InRad();
  ESSN_SSTrShield   SSTrShield_;
  G4double          TrShVac3_R1 = SSTrShield_.TrShVac3_InRad();
  //G4double TrTubeSS_RMax = TrSh1_PV.Tube_Rad()[0];
  G4double          TrTubeBe_RMin = SSTrShield_.Tube_Rad()[1];
  G4double          TrTubeBe_RMax = SSTrShield_.Tube_Rad()[2];


};
inline G4ThreeVector    ESSN_UpperSSTrShield::Position() const { return TrSh2_Ps; }

inline G4LogicalVolume* ESSN_UpperSSTrShield::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_UpperSSTrShield::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
