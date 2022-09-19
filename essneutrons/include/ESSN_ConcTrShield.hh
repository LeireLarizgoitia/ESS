#ifndef ConcTrShield_H
#define ConcTrShield_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

class ESSN_ConcTrShield//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
  ESSN_ConcTrShield();
  /// Destructor
  ~ESSN_ConcTrShield();
  /// Returns the Position
  G4ThreeVector Position() const;

  G4double Radius() const;
  G4double TrShCarbonD_Rad() const;
  G4double TrShVac2_InRad() const;

  G4LogicalVolume* GetLogicalVolume() const;
  void SetLogicalVolume(G4LogicalVolume* lv);

  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  G4double         TrShConc_R = 280*cm ;
  G4double         TrShConc_Z = (224*cm-200*cm)/2;
  G4double         TrShConc_dz = (224*cm-TrShConc_Z);
  G4ThreeVector    TrShConc_Ps = G4ThreeVector(0,0,TrShConc_dz);


  G4double         TrShVac2_R1 = 270*cm;
  G4double         TrShCarbonD_R = 201.5*cm;


};
inline G4ThreeVector    ESSN_ConcTrShield::Position() const { return TrShConc_Ps; }

inline G4double         ESSN_ConcTrShield::Radius() const { return TrShConc_R; }
inline G4double         ESSN_ConcTrShield::TrShCarbonD_Rad() const { return TrShCarbonD_R; }
inline G4double         ESSN_ConcTrShield::TrShVac2_InRad() const { return TrShVac2_R1; }


inline G4LogicalVolume* ESSN_ConcTrShield::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_ConcTrShield::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
