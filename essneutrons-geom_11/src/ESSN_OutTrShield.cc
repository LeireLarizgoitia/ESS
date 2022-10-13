#include "ESSN_OutTrShield.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"

ESSN_OutTrShield::ESSN_OutTrShield()
{}

ESSN_OutTrShield::~ESSN_OutTrShield()
{}

void ESSN_OutTrShield::Construct()
{
  G4Material* fSS316L = G4Material::GetMaterial("fSS316L");
  G4Material* fVacuum = G4Material::GetMaterial("G4_Galactic");
  G4Material* fCarbonSteel =  G4Material::GetMaterial("fCarbonSteel");
  //María: import beryllium form materials
  G4Material* fWater = G4Material::GetMaterial("fWater");
  G4Material* fAL6061T6 = G4Material::GetMaterial("fAL6061T6");
  //G4Material* fBeryllium = G4Material::GetMaterial("m1019");



  // Rotation for objects relative to Left Trd
  G4RotationMatrix* TrdLeft_Rot = new G4RotationMatrix();
  TrdLeft_Rot->rotateX(-90.*deg);

  G4VSolid* OTrSh1_SV = new G4Tubs("OTrSh1_SV",
                                   OTrSh1_R1,OTrSh1_R2, OTrSh1_Z, 0, 360*deg);

  // Mother volume
  G4LogicalVolume* OTrSh1_LV = new  G4LogicalVolume(OTrSh1_SV,fCarbonSteel, "OTrSh_LV");
  this->SetLogicalVolume(OTrSh1_LV);

  // SS316L shielding
  // 63*deg aperture
  G4double OTrShSS_R1 = OTrSh1_R1;
  G4double OTrShSS_R2 = OTrSh1_R2;
  G4double OTrShSS_Z = (42.5*cm+69.5*cm)/2;

  G4VSolid* OTrShSS_SV = new G4Tubs("OTrShSS_SV",
                                    OTrShSS_R1,OTrShSS_R2, OTrShSS_Z,  -63*deg,126*deg);

  /// Relative to OTrSh1
  G4double OTrShSS_dz = (69.5*cm-OTrShSS_Z);
  G4ThreeVector OTrShSS_Ps = G4ThreeVector(0,0,(OTrShSS_dz-OTrSh1_dz));

  G4LogicalVolume* OTrShSS_LV = new G4LogicalVolume(OTrShSS_SV,fSS316L, "OTrShSS_LV");

  new G4PVPlacement(0,OTrShSS_Ps,OTrShSS_LV,"OTrShSS_PV",OTrSh1_LV,false,0,1);

  G4RotationMatrix* OTrShSS_Rot = new G4RotationMatrix();
  OTrShSS_Rot->rotateY(180.*deg);

  new G4PVPlacement(OTrShSS_Rot,OTrShSS_Ps,OTrShSS_LV,"OTrShSS_PV",OTrSh1_LV,false,0,1);

  // Beam Tube Vacuum in Carbon Steel region: BeamTubeVacCS
  G4double BeamTubeCS_center = -375*cm; //y axis

  G4double BeamTubeCS_d1 = -(BeamTubeCS_center+30*cm);
  G4double BeamTubeCS_Z1 = (OTrSh1_R2-BeamTubeCS_d1)/2;
  G4double BeamTubeCS_Z2 = (20.75*cm-4.35*cm)/2;
  G4double BeamTubeCS_Z3 = (BeamTubeCS_d1-OTrSh1_R1)/2 + 1*cm; //to get the cut

  //Leire: G4VSolid* BeamTubeVacCS1_SV = new G4Tubs("BeamTubeVacCS1_SV",0,BeamTubeCS_R1,BeamTubeCS_Z1, 0*deg, 360*deg);
  //G4VSolid* BeamTubeVacCS1_SV = new G4Tubs("BeamTubeVacCS1_SV",0,BeamTubeCS_R1,BeamTubeCS_Z1/2 + BeamTubeCS_Z2, 0*deg, 360*deg);

  G4VSolid* BeamTubeVacCS1_SV = new G4Tubs("BeamTubeVacCS1_SV",0,BeamTubeCS_R1,BeamTubeCS_Z1 - 20*cm, 0*deg, 360*deg);

  //Maria:
  G4VSolid* BeamTubeVacCS1B_SV = new G4Tubs("BeamTubeVacCS1B_SV",0,BeamTubeCS_R1,BeamTubeCS_Z2, 0*deg, 360*deg);
  G4VSolid* BeamTubeVacCS2_SV = new G4Tubs("BeamTubeVacCS2_SV",BeamTubeCS_R1,BeamTubeCS_R2,BeamTubeCS_Z2, 0*deg, 360*deg);
  G4VSolid* BeamTubeVacCS3_SV = new G4Tubs("BeamTubeVacCS3_SV",0,BeamTubeCS_R3,BeamTubeCS_Z3, 0*deg, 360*deg);

  G4double BeamTubeCS1_dy  = OTrSh1_R2 - BeamTubeCS_Z1;
  //G4ThreeVector BeamTubeVacCS1_Ps = G4ThreeVector(0,-BeamTubeCS1_dy,-OTrSh1_dz);
  G4ThreeVector BeamTubeVacCS1_Ps = G4ThreeVector(0,-BeamTubeCS1_dy - BeamTubeCS_Z2 -10*cm,-OTrSh1_dz);
  G4VSolid* SubCylBeamTubeCS1_SV = new G4Tubs("SubCylBeamTubeCS1_SV",0,OTrSh1_R2,2000*cm, 0*deg, 360*deg);
  G4ThreeVector SubCylBeamTubeCS1_Ps(0,0,-BeamTubeCS1_dy);
  G4VSolid* BeamTubeVacCS12_SV = new G4IntersectionSolid("BeamTubeVacCS12_SV", BeamTubeVacCS1_SV,SubCylBeamTubeCS1_SV,TrdLeft_Rot,SubCylBeamTubeCS1_Ps);

  G4double BeamTubeCS2A_dy  = BeamTubeCS_center + 4.35*cm + BeamTubeCS_Z2;
  G4ThreeVector BeamTubeVacCS2A_Ps = G4ThreeVector(0,BeamTubeCS2A_dy,-OTrSh1_dz);

  G4double BeamTubeCS2B_dy  = BeamTubeCS_center - 4.35*cm - BeamTubeCS_Z2;
  G4ThreeVector BeamTubeVacCS2B_Ps = G4ThreeVector(0,BeamTubeCS2B_dy,-OTrSh1_dz);

  G4double BeamTubeCS3_dy  = BeamTubeCS_d1 - BeamTubeCS_Z3;
  G4ThreeVector BeamTubeVacCS3_Ps = G4ThreeVector(0,-BeamTubeCS3_dy,-OTrSh1_dz);
  G4VSolid* SubCylBeamTubeCS3_SV = new G4Tubs("SubCylBeamTubeCS3_SV",0,OTrSh1_R1,2000*cm, 0*deg, 360*deg);
  G4ThreeVector SubCylBeamTubeCS3_Ps(0,0,-BeamTubeCS3_dy);
  G4VSolid* BeamTubeVacCS32_SV = new G4SubtractionSolid("BeamTubeVacCS32_SV", BeamTubeVacCS3_SV,SubCylBeamTubeCS3_SV,TrdLeft_Rot,SubCylBeamTubeCS3_Ps);

  G4LogicalVolume* BeamTubeVacCS1_LV = new  G4LogicalVolume(BeamTubeVacCS12_SV,fVacuum, "BeamTubeVacCS_LV");
  //Maria:
  G4LogicalVolume* BeamTubeVacCS1B_LV = new  G4LogicalVolume(BeamTubeVacCS1B_SV,fVacuum, "BeamTubeVacCS_LV");

  G4LogicalVolume* BeamTubeVacCS2_LV = new  G4LogicalVolume(BeamTubeVacCS2_SV,fVacuum, "BeamTubeVacCS_LV");
  G4LogicalVolume* BeamTubeVacCS3_LV = new  G4LogicalVolume(BeamTubeVacCS32_SV,fVacuum, "BeamTubeVacCS_LV");

  new G4PVPlacement(TrdLeft_Rot,BeamTubeVacCS1_Ps,BeamTubeVacCS1_LV,"BeamTubeVacCS1_PV",OTrSh1_LV,false,0,1);
  //Maria:
  new G4PVPlacement(TrdLeft_Rot,G4ThreeVector(0, BeamTubeCS2A_dy + 18.5*cm/2, -OTrSh1_dz),BeamTubeVacCS1B_LV,"BeamTubeVacCS1B_PV",OTrSh1_LV,false,0,1); //quito +20*cm/2 en x

  new G4PVPlacement(TrdLeft_Rot,BeamTubeVacCS2A_Ps,BeamTubeVacCS2_LV,"BeamTubeVacCS2_PV",OTrSh1_LV,false,0,1);
  new G4PVPlacement(TrdLeft_Rot,BeamTubeVacCS2B_Ps,BeamTubeVacCS2_LV,"BeamTubeVacCS2_PV",OTrSh1_LV,false,0,1);
  new G4PVPlacement(TrdLeft_Rot,BeamTubeVacCS3_Ps,BeamTubeVacCS3_LV,"BeamTubeVacCS3_PV",OTrSh1_LV,false,0,1);

  //María
  //BeWindow:

  //Cubo
  G4double BeWBox_hx =  20*cm/2;
  G4double BeWBox_hy = (BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 - BeamTubeCS_Z2;
  G4double BeWBox_hz = 20*cm/2;


  G4VSolid* BeWindow_Box = new G4Box("BeWindowBox_SV", BeWBox_hx, BeWBox_hy, BeWBox_hz);

  //G4LogicalVolume* BeWindowBox_LV = new G4LogicalVolume(BeWindow_Box,fVacuum,"BeWindowBox_LV");

  G4LogicalVolume* BeWindowBox_LV = new G4LogicalVolume(BeWindow_Box, fVacuum, "BeamTubeVacCS_LV");

  //G4ThreeVector BeamBeWBox_Ps = G4ThreeVector(0,-BeamTubeCS1_dy + (BeamTubeCS_Z1/2 + BeamTubeCS_Z2),-OTrSh1_dz); //<-CHECK POSITION
  //G4ThreeVector BeamBeWBox_Ps = G4ThreeVector(0,(BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2,-OTrSh1_dz);
  G4ThreeVector BeamBeWBox_Ps = G4ThreeVector(0,-BeamTubeCS1_dy + BeamTubeCS_Z1/2 + BeamTubeCS_Z2 + (BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 + 5*mm,-OTrSh1_dz);

  new G4PVPlacement(0,BeamBeWBox_Ps,BeWindowBox_LV,"BeWindowBox_PV", OTrSh1_LV, false,0,1);


  //Cilindro (tub)
  G4double BeWCyl_Rmin =  8.9*cm;
  G4double BeWCyl_Rmax = 9.3*cm;
  G4double BeWCyl_hz = 20*cm/2;
  G4double BeWCyl_Phi = 30*deg;
  G4double BeWCyl_DeltaPhi = 120*deg;

  G4VSolid* BeWindow_Cyl = new G4Tubs("BeWindowCyl_SV", BeWCyl_Rmin, BeWCyl_Rmax, BeWCyl_hz, BeWCyl_Phi, BeWCyl_DeltaPhi);
  //G4LogicalVolume* BeWindowCyl_LV = new G4LogicalVolume(BeWindow_Cyl,fBeryllium,"BeWindow_LV"); //"BeWindowCyl_LV"


  //Relativa a OTrSh1_LV: G4ThreeVector BeamWCyl_Ps = G4ThreeVector(0,-BeamTubeCS1_dy + (BeamTubeCS_Z1/2 + BeamTubeCS_Z2) + 6*cm,-OTrSh1_dz);
  G4ThreeVector BeamWCyl_Ps = G4ThreeVector(0, 6*cm -(BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 - 5*mm, 0);

  //new G4PVPlacement(0,BeWCyl_Ps,BeWindowCyl_LV,"BeWindowCyl_PV", BeWindowBox_LV, false,0,1);
  //new G4PVPlacement(0,BeamWCyl_Ps,BeWindowCyl_LV,"BeWindowCyl_PV", OTrSh1_LV, false,0,1);


  //Alas laterales de la ventana

  G4double BeWLat_hz = 20*cm/2;
  G4double BeWLat_hx = 1.95*cm/2 + 0.2*cm; //5.3/2
  G4double BeWLat_hy = 1*cm/2;

  G4VSolid* BeWLat_Left = new G4Box("BeWLat_Left",BeWLat_hx, BeWLat_hy, BeWLat_hz);
  G4VSolid* BeWLat_Right = new G4Box("BeWLat_Right",BeWLat_hx, BeWLat_hy, BeWLat_hz);

  //posición relativa al cilindro
  G4ThreeVector BeWLatLeft_Ps = G4ThreeVector(8.05*cm + BeWLat_hx - 0.4*cm , (BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 - 8*cm, 0);
  G4ThreeVector BeWLatRight_Ps = G4ThreeVector(-8.05*cm - BeWLat_hx + 0.4*cm, (BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 - 8*cm, 0);

  G4VSolid* BeWind_CylANDLeft = new G4UnionSolid("BeWind_CylANDLeft",BeWindow_Cyl,BeWLat_Left,0,BeWLatLeft_Ps);
  G4VSolid* BeWindow = new G4UnionSolid("BeWindow", BeWind_CylANDLeft, BeWLat_Right, 0, BeWLatRight_Ps);

  G4LogicalVolume* BeWindow_LV = new G4LogicalVolume(BeWindow,fAL6061T6,"BeWindow_LV");

  //new G4PVPlacement(0,BeamWCyl_Ps,BeWind_CylANDLeft_LV,"BeWind_CylANDLeft_PV", OTrSh1_LV, false,0,1);
  new G4PVPlacement(0,BeamWCyl_Ps,BeWindow_LV,"BeWindow_PV", BeWindowBox_LV, false,0,1);

  /*** CONSTRUCCIÓN POR SEPARADO ***/
  /*
  G4LogicalVolume* BeWLatLeft_LV = new G4LogicalVolume(BeWLat_Left, fBeryllium, "BeWindow_LV"); //"BeWLatLeft_LV"
  G4LogicalVolume* BeWLatRight_LV = new G4LogicalVolume(BeWLat_Right, fBeryllium, "BeWindow_LV"); //"BeWLatRight_LV"

  G4ThreeVector BeWLatLeft_Ps = G4ThreeVector(4.65*cm + BeWLat_hx*2,-BeamTubeCS1_dy + BeamTubeCS_Z1/2 + BeamTubeCS_Z2 + (BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 - 2*cm,-OTrSh1_dz);
  G4ThreeVector BeWLatRight_Ps = G4ThreeVector(-4.65*cm - BeWLat_hx*2,-BeamTubeCS1_dy + BeamTubeCS_Z1/2 + BeamTubeCS_Z2 + (BeamTubeCS2A_dy - BeamTubeCS2B_dy)/2 - 2*cm,-OTrSh1_dz);

  new G4PVPlacement(0,BeWLatLeft_Ps,BeWLatLeft_LV,"BeWLatLeft_PV", OTrSh1_LV, false,0,1);
  new G4PVPlacement(0,BeWLatRight_Ps,BeWLatRight_LV,"BeWLatRight_PV", OTrSh1_LV, false,0,1);
  */

  /**** Estructura interna ****/

  G4double BeWCylInt_Rmin =  9.0*cm;
  G4double BeWCylInt_Rmax = 9.2*cm;
  G4double BeWCylInt_hz = 20*cm/2;
  G4double BeWCylInt_Phi = 60*deg;
  G4double BeWCylInt_DeltaPhi = 60*deg;

  G4VSolid* BeWindow_CylInt = new G4Tubs("BeWindowCylInt_SV", BeWCylInt_Rmin, BeWCylInt_Rmax, BeWCylInt_hz, BeWCylInt_Phi, BeWCylInt_DeltaPhi);
  G4LogicalVolume* BeWindowCylInt_LV = new G4LogicalVolume(BeWindow_CylInt,fWater,"BeWindowCylInt_LV");


  new G4PVPlacement(0,G4ThreeVector(0,0,0),BeWindowCylInt_LV,"BeWindowCylInt_PV", BeWindow_LV, false,0,1);

}
