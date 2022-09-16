#include "ESSN_UpperSSTrShield.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"


ESSN_UpperSSTrShield::ESSN_UpperSSTrShield()
{}

ESSN_UpperSSTrShield::~ESSN_UpperSSTrShield()
{}

void ESSN_UpperSSTrShield::Construct()
{
    G4Material* fBeryllium = G4Material::GetMaterial("fBeryllium");
    G4Material* fSS316L = G4Material::GetMaterial("fSS316L");
    G4Material* fVacuum = G4Material::GetMaterial("G4_Galactic");

    G4RotationMatrix* TrdLeft_Rot = new G4RotationMatrix();
    TrdLeft_Rot->rotateX(-90.*deg);

    G4double TrSh2_R2 = 292.5*cm;
    G4VSolid* TrSh2_SV = new G4Tubs("TrSh2_SV",
                                    0,TrSh2_R2, TrSh2_Z, 0, 360*deg);

    // Mother volume
    G4LogicalVolume* TrSh2_LV = new  G4LogicalVolume(TrSh2_SV,fSS316L, "TrSh2_LV");
    this->SetLogicalVolume(TrSh2_LV);

  // Vacuum clad in SS316L shielding

    G4double TrShVac1_R1 = 288*cm;
    G4double TrShVac1_R2 = 290*cm;
    G4double TrShVac1_Z = (400*cm-210*cm)/2;

    G4double TrShVac2_Z = (210*cm-209*cm)/2;

    G4double TrShVac31_Z = (210*cm-TrSh1_Ztop)/2;

    G4VSolid* TrShVac1_SV = new G4Tubs("TrShVac1_SV",
                                       TrShVac1_R1, TrShVac1_R2, TrShVac1_Z, 0, 360*deg);

    G4VSolid* TrShVac2_SV = new G4Tubs("TrShVac2_SV",
                                       TrShVac2_R1, TrShVac1_R2, TrShVac2_Z, 0, 360*deg);

    G4VSolid* TrShVac31_SV = new G4Tubs("TrShVac31_SV",
                                       TrShVac3_R1, TrShVac2_R1, TrShVac31_Z, 0, 360*deg);

    /// Relative to TrSh2 center
    ///Upper part of TrShVac3_LV Defined over TrSh2 too (avoids material identification problems)
    G4double TrShVac31_dz = -(210*cm-TrShVac31_Z);
    G4ThreeVector TrShVac31_Sh2Ps = G4ThreeVector(0,0,TrShVac31_dz-TrSh2_dz);

    G4double TrShVac2_dz = -(210*cm-TrShVac2_Z);
    G4ThreeVector TrShVac2_Sh2Ps = G4ThreeVector(0,0,TrShVac2_dz-TrSh2_dz);

    G4double TrShVac1_dz = -(400*cm-TrShVac1_Z);
    G4ThreeVector TrShVac1_Sh2Ps = G4ThreeVector(0,0,TrShVac1_dz-TrSh2_dz);

    G4LogicalVolume* TrShVac1_LV = new  G4LogicalVolume(TrShVac1_SV,fVacuum, "TrShVac_LV");
    G4LogicalVolume* TrShVac2_LV = new  G4LogicalVolume(TrShVac2_SV,fVacuum, "TrShVac_LV");
    G4LogicalVolume* TrShVac31_LV = new  G4LogicalVolume(TrShVac31_SV,fVacuum, "TrShVac_LV");

    new G4PVPlacement(0,TrShVac31_Sh2Ps,TrShVac31_LV,"TrShVac31_PV",TrSh2_LV,false,0,1);
    new G4PVPlacement(0,TrShVac2_Sh2Ps,TrShVac2_LV,"TrShVac2_PV",TrSh2_LV,false,0,1);
    new G4PVPlacement(0,TrShVac1_Sh2Ps,TrShVac1_LV,"TrShVac1_PV",TrSh2_LV,false,0,1);

    // ----------------------------------------------------------------------------
    //
    // TARGET TUBE (Relative to TrSh2)
    //
    // ----------------------------------------------------------------------------

    /// Beryllium tube around target
    //G4double Tr_center = Tr_center; // translation to target center

    G4double TrTube_2Z  = (400*cm-TrSh1_Ztop)/2;


    G4VSolid* TrTubeBe_2SV = new G4Tubs("TrTubeBe_2SV",
    TrTubeBe_RMin,TrTubeBe_RMax, TrTube_2Z, 0, 360*deg);

    /// Relative to TrSh2
    G4double TrTube_2dz = -(TrSh1_Ztop+TrTube_2Z);
    G4ThreeVector TrTube_TrSh2Ps = G4ThreeVector(0,Tr_center,TrTube_2dz-TrSh2_dz);

    G4LogicalVolume* TrTubeBe_2LV = new  G4LogicalVolume(TrTubeBe_2SV,fBeryllium, "TrTubeBe_LV");
    new G4PVPlacement(0,TrTube_TrSh2Ps,TrTubeBe_2LV,"TrTubeBe_2PV",TrSh2_LV,false,0,1);


    // ----------------------------------------------------------------------------
    //
    // TARGET SURROUNDING
    //   Relative to TrSh2 (for proper material definition)
    //
    // ----------------------------------------------------------------------------

    // Vacuum clad
        ///

    G4double TrSurVac7_1Z = (200*cm-TrSh1_Ztop)/2;
    G4VSolid* TrSurVac7_1SV = new G4Tubs("TrSurVac7_1SV",
                                        23*cm,25*cm, TrSurVac7_1Z, 0, 360*deg);

    G4double TrSurVac7_2Z = (400*cm-282*cm)/2;
    G4VSolid* TrSurVac7_2SV = new G4Tubs("TrSurVac7_2SV",
                                        23*cm,25*cm, TrSurVac7_2Z, 0, 360*deg);

    G4double TrSurVac8_Z = (282*cm-200*cm)/2;
    G4VSolid* TrSurVac8_SV = new G4Tubs("TrSurVac8_SV",
                                        23*cm,35*cm, TrSurVac8_Z, 0, 360*deg);

    /// Relative to TrSh2
    G4double TrSurVac7_1dz = -(TrSh1_Ztop+TrSurVac7_1Z);
    G4ThreeVector TrSurVac7_TrSh2Ps = G4ThreeVector(0,Tr_center,TrSurVac7_1dz-TrSh2_dz);
    G4double TrSurVac7_2dz = -(282*cm+TrSurVac7_2Z);
    G4ThreeVector TrSurVac7_2TrSh2Ps = G4ThreeVector(0,Tr_center,TrSurVac7_2dz-TrSh2_dz);
    // Position for TrSurVac8
    G4double TrSurVac8_dz = (-(200*cm+TrSurVac8_Z));
    G4ThreeVector TrSurVac8_TrSh2Ps = G4ThreeVector(0,Tr_center,TrSurVac8_dz-TrSh2_dz);


    G4LogicalVolume* TrSurVac7_1LV = new  G4LogicalVolume(TrSurVac7_1SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac7_2LV = new  G4LogicalVolume(TrSurVac7_2SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac8_LV = new  G4LogicalVolume(TrSurVac8_SV,fVacuum, "TrSurVac_LV");


    new G4PVPlacement(0,TrSurVac7_TrSh2Ps,TrSurVac7_1LV,"TrSurVac7_1PV",TrSh2_LV,false,0,1);
    new G4PVPlacement(0,TrSurVac7_2TrSh2Ps,TrSurVac7_2LV,"TrSurVac7_2PV",TrSh2_LV,false,0,1);
    new G4PVPlacement(0,TrSurVac8_TrSh2Ps,TrSurVac8_LV,"TrSurVac8_PV",TrSh2_LV,false,0,1);


    G4double TrSur9_R1 = 23*cm;
    G4double TrSur9_R2 = 33*cm;
    G4double TrSur9_Z = (281*cm-201*cm)/2;
    G4VSolid* TrSur9_SV = new G4Tubs("TrSur9_SV",
                                      TrSur9_R1,TrSur9_R2,TrSur9_Z, 0, 360*deg);

    /// Position relative toTrSurVac8
        // distance to target center
    G4double TrSur9_dz = -(201*cm+TrSur9_Z);
    G4ThreeVector TrSur9_Vac8Ps = G4ThreeVector(0,0,TrSur9_dz-TrSurVac8_dz); //relative to TrSurVac9


    G4LogicalVolume* TrSur9_LV = new  G4LogicalVolume(TrSur9_SV,fSS316L, "TrSur_LV");
    // Placement relative to TrSurVac8_LV (mother volume)
    new G4PVPlacement(0,TrSur9_Vac8Ps,TrSur9_LV,"TrSur9_PV",TrSurVac8_LV,false,0,1);

}
