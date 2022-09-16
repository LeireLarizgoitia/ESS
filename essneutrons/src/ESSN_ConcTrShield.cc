#include "ESSN_ConcTrShield.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"


ESSN_ConcTrShield::ESSN_ConcTrShield()
{}

ESSN_ConcTrShield::~ESSN_ConcTrShield()
{}

void ESSN_ConcTrShield::Construct()
{
  G4Material* fConcrete50Fe = G4Material::GetMaterial("fConcrete50Fe");
  G4Material* fSS316L = G4Material::GetMaterial("fSS316L");
  G4Material* fVacuum = G4Material::GetMaterial("G4_Galactic");
  G4Material* fCarbonSteel =  G4Material::GetMaterial("fCarbonSteel");

  // Concrete regular + 50%Fe below SS316L shielding
  G4VSolid* TrShConc_SV = new G4Tubs("TrShConc_SV",
                                     0, TrShConc_R, TrShConc_Z, 0, 360*deg);

  // Mother volume
  G4LogicalVolume* TrShConc_LV = new  G4LogicalVolume(TrShConc_SV,fConcrete50Fe, "TrShConc_LV");
  this->SetLogicalVolume(TrShConc_LV);

  // ----------------------------------------------------------------------------
  //
  // Surrounding the monolith (bottom)
  //
  // ----------------------------------------------------------------------------

  G4double TrSh3_R2 = 275*cm;
  G4double TrSh3_Z = (214.5*cm-206*cm)/2;
  G4VSolid* TrSh3_SV = new G4Tubs("TrSh3_SV",
                                  0,TrSh3_R2, TrSh3_Z, 0, 360*deg);

  G4double TrShVac4_Z = (206*cm-200*cm)/2;
  G4VSolid* TrShVac4_SV = new G4Tubs("TrShVac4_SV",
                                     0, TrShVac2_R1, TrShVac4_Z, 0, 360*deg);

  /// Relative to TrShConc center
  G4double TrSh3_dz = (214.5*cm-TrSh3_Z);
  G4ThreeVector TrSh3_ConcPs = G4ThreeVector(0,0,TrSh3_dz-TrShConc_dz);

  G4double TrShVac4_dz = (206*cm-TrShVac4_Z);
  G4ThreeVector TrShVac4_ConcPs = G4ThreeVector(0,0,TrShVac4_dz-TrShConc_dz);

  G4LogicalVolume* TrSh3_LV = new  G4LogicalVolume(TrSh3_SV,fSS316L, "TrSh_LV");
  new G4PVPlacement(0,TrSh3_ConcPs,TrSh3_LV,"TrSh3_PV",TrShConc_LV,false,0,1);

  G4LogicalVolume* TrShVac4_LV = new  G4LogicalVolume(TrShVac4_SV,fVacuum, "TrShVac_LV");
  new G4PVPlacement(0,TrShVac4_ConcPs,TrShVac4_LV,"TrShVac4_PV",TrShConc_LV,false,0,1);

  /// TrSh4 Relative to TrShVac4 center
  G4double TrSh4_R1 = 220*cm;
  G4double TrSh4_R2 = 250*cm;
  G4double TrSh4_Z = (206*cm-200*cm)/2;
  G4VSolid* TrSh4_SV = new G4Tubs("TrSh4_SV",
                                  TrSh4_R1, TrSh4_R2, TrSh4_Z, 0, 360*deg);

  G4LogicalVolume* TrSh4_LV = new  G4LogicalVolume(TrSh4_SV,fSS316L, "TrSh_LV");

  new G4PVPlacement(0,G4ThreeVector(),TrSh4_LV,"TrSh4_PV",TrShVac4_LV,false,0,1);

  // ----------------------------------------------------------------------------
  //
  // Standard grade carbon steel AISI 1005 below target shield
  //
  // ----------------------------------------------------------------------------

  G4double TrShCarbonD_Z = (224*cm-214.5*cm)/2;
  G4VSolid* TrShCarbonD_SV = new G4Tubs("TrShCarbonD_SV",
   0, TrShCarbonD_R, TrShCarbonD_Z, 0, 360*deg);

  /// Relative to Concrete regular + 50%Fe below SS316L shielding
  G4double TrShCarbonD_dz = (224*cm-TrShCarbonD_Z);
  G4ThreeVector TrShCarbonD_ConcPs = G4ThreeVector(0,0,TrShCarbonD_dz-TrShConc_dz);

  G4LogicalVolume* TrShCarbonD_LV = new  G4LogicalVolume(TrShCarbonD_SV,fCarbonSteel, "TrShCarbon_LV");
  new G4PVPlacement(0,TrShCarbonD_ConcPs,TrShCarbonD_LV,"TrShCarbonD_ConcPV",TrShConc_LV,false,0,1);

}
