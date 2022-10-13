#include "ESSN_GroundRoomL.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VSolid.hh"
#include "G4Trap.hh"
#include "G4Trd.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4SystemOfUnits.hh"

//#include "ESSNCompleteDetectorConstruction.hh"
#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"

ESSN_GroundRoomL::ESSN_GroundRoomL()
{}

ESSN_GroundRoomL::~ESSN_GroundRoomL()
{}

void ESSN_GroundRoomL::Construct()
{
  G4Material* fConcrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Material* fAir = G4Material::GetMaterial("G4_AIR");

  G4Box* AuxBoxSV = new G4Box("AuxBoxSV",
                      hx_AuxBox,hy_AuxBox,hz_AuxBox);

  G4double dx_AuxBox = (Lx_ConcBox-200*cm)/2 + hx_AuxBox;
  G4ThreeVector AuxBox1_Ps = G4ThreeVector(dx_AuxBox,0,0);
  G4ThreeVector AuxBox2_Ps = G4ThreeVector(-dx_AuxBox,0,0);

  //Detector_.Construct();
  //G4ThreeVector AuxBox1_Ps = Detector_.AuxBox1_Position();
  //G4ThreeVector AuxBox2_Ps = Detector_.AuxBox2_Position();
  //G4Box* AuxBoxSV = Detector_PV.AuxBox_Solid();

  G4RotationMatrix* TrdLeft_Rot = new G4RotationMatrix();
      TrdLeft_Rot->rotateX(-90.*deg);


  G4VSolid* TLeftRoomSV = new G4Trd("TLeftRoomSV",dx_1TL,dx_2TL,dz_TL,dz_TL,dy_TL);

  G4RotationMatrix* TLeft_Rot =  new G4RotationMatrix();
      TLeft_Rot->rotateX(-90.*deg);

  /// SubCylG:  Subtract Cylinder to Outer Trapezoid (Cocnrete) at GROUNG level
  G4double SubCyl_RGround = ConcreteShield_R2;
  G4VSolid* SubCylG_SV = new G4Tubs("SubCylG_SV",0,SubCyl_RGround,SubCyl_Z, 0*deg, 360*deg);

  G4VSolid* TLeftRoomRestSV = new G4SubtractionSolid("TLeftRoomRestSV", TLeftRoomSV,SubCylG_SV,TrdLeft_Rot,SubCyl_Ps);

  /// Subtract both corners of the trapezoid
  G4VSolid* TLeftRest1SV = new G4SubtractionSolid("TLeftRest1SV", TLeftRoomRestSV, AuxBoxSV,0,AuxBox1_Ps);
  G4VSolid* TLeftRestSV = new G4SubtractionSolid("TLeftRestSV", TLeftRest1SV, AuxBoxSV,0,AuxBox2_Ps);


  // Mother volume
  G4LogicalVolume* TLeftLV = new  G4LogicalVolume(TLeftRestSV,fConcrete, "TLeftLV");
  this->SetLogicalVolume(TLeftLV);

  // SV INNER Trapezoid

  G4VSolid* InTLeftSV = new G4Trd("InTLeftSV",dx_1InTL,dx_2InTL,dz_InTL,dz_InTL,dy_InTL);

  /// Subtract both corner of the trapezoid
  G4VSolid* InTLeftRest1SV = new G4SubtractionSolid("InTLeftRest1SV", InTLeftSV, AuxBoxSV,0,AuxBox1_Ps);
  G4VSolid* InTLeftRest2SV = new G4SubtractionSolid("InTLeftRest2SV", InTLeftRest1SV, AuxBoxSV,0,AuxBox2_Ps);

  /// SubCylInG:  Subtract Cylinder to Inner Trapezoid (Air) at GROUNG level
  G4double SubCylIn_RGround = 706*cm;
  G4VSolid* SubCylInG_SV = new G4Tubs("SubCylInG_SV",0,SubCylIn_RGround,SubCyl_Z, 0*deg, 360*deg);

  G4VSolid* InTLeftRestSV = new G4SubtractionSolid("InTLeftRestSV", InTLeftRest2SV,SubCylInG_SV,TrdLeft_Rot,SubCylIn_Ps);
  G4LogicalVolume* InTLeftLV = new  G4LogicalVolume(InTLeftRestSV,fAir, "InTLeftLV");

  new G4PVPlacement(0,InTLeft_Ps,InTLeftLV,"InTLeftPV",TLeftLV,false,0,1);

}
