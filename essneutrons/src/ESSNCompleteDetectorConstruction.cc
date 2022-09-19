/// \file ESSNCompleteDetectorConstruction.cc
/// \brief Implementation of the ESSNCompleteDetectorConstruction class; adapted from
///        EDDetectorConstruction class by Mostafa

#include "ESSNCompleteDetectorConstruction.hh"
#include "ESSNParam.hh"

#include "ESSNMaterialConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Trap.hh"
#include "G4Sphere.hh"
#include "G4Torus.hh"
#include "G4Trd.hh"
#include "G4Orb.hh"
#include "G4Cons.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4MultiUnion.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "G4PVReplica.hh"

#include "G4Transform3D.hh"
#include "G4ReflectionFactory.hh"

/* sqrt example */
#include <stdio.h>      /* printf */
#include <math.h>       /* sqrt */

//ESS Geometries
#include "ESSN_BunkerL.hh"
#include "ESSN_BunkerLR.hh"
#include "ESSN_BunkerS.hh"
#include "ESSN_BunkerSR.hh"

#include "ESSN_OutTrShield.hh"
#include "ESSN_ConcTrShield.hh"
#include "ESSN_SSTrShield.hh"
#include "ESSN_UpperSSTrShield.hh"
//#include "ESSN_TrSurrounding.hh"

#include "ESSN_GroundRoomL.hh"
#include "ESSN_GroundRoomR.hh"

using namespace materials;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNCompleteDetectorConstruction::ESSNCompleteDetectorConstruction()
: G4VUserDetectorConstruction()

{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNCompleteDetectorConstruction::~ESSNCompleteDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ESSNCompleteDetectorConstruction::Construct()
{
  //Declare materials
  G4Material* fVacuum        = materials::fVacuum();
  G4Material* fConcrete      = materials::fConcrete();
  G4Material* fAir           = materials::fAir();
  //G4Material* fPara_hydrogen = materials::fPara_hydrogen();
  G4Material* fWater         = materials::fWater();
  //G4Material* fHelium        = materials::fHelium();
  G4Material* fAluminum      = materials::fAluminum(); // 293.15 kelvin
  //G4Material* fAluminum_20K  = materials::fAluminum_20K(); // 20 kelvin
  G4Material* fSS316L        = materials::fSS316L(); // StainlessSteel 316L
  //G4Material* fSS316LH2O     = materials::fSS316LH2O(); // StainlessSteel 316L H2O
  G4Material* fBeryllium     = materials::fBeryllium();
  G4Material* fTungsten      = materials::fTungsten();
  G4Material* fConcrete50Fe  = materials::fConcrete50Fe();
  G4Material* fCarbonSteel   = materials::fCarbonSteel();
  G4Material* fMagnadense    = materials:: fMagnadense(); // MAGNADENSE8 BY LKAB MINERALS
  G4Material* fAL6061T6      = materials:: fAL6061T6();

  // Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  // World
  G4double hx_w =  worldX*cm/2;
  G4double hy_w =  worldY*cm/2;
  G4double hz_w =  worldZ*cm/2;

  // World volume
  G4Box* worldS = new G4Box("WorldS", hx_w, hy_w, hz_w);
  G4LogicalVolume* worldLV = new G4LogicalVolume(worldS, fVacuum, "WorldLV");
  G4VPhysicalVolume* worldPV = new G4PVPlacement(0,G4ThreeVector(),worldLV,"WorldPV",0,false,0,checkOverlaps);


// ----------------------------------------------------------------------------
//
// ADDED by Leire
//
// G4Trap : pZ Length along z axis
// G4Trd : pZ Half-length along z axis
//
// ----------------------------------------------------------------------------

    // Rotation for objects relative to Left Trd
    G4RotationMatrix* TrdLeft_Rot = new G4RotationMatrix();
    TrdLeft_Rot->rotateX(-90.*deg);

    G4RotationMatrix* Reflection_Rot = new G4RotationMatrix();
    Reflection_Rot->rotateY(180.*deg);

// ----------------------------------------------------------------------------
//
// WATER LOWER LEVEL
//
// ----------------------------------------------------------------------------

    G4double OuterBase_Z = (1750*cm-750*cm)/2;
    G4double OuterBase_dz = 750*cm+OuterBase_Z;
    G4ThreeVector OuterBase_Ps = G4ThreeVector(0,0,OuterBase_dz);

    G4VSolid* OuterBase_SV = new G4Tubs("OuterBase_SV",0,ConcBunk_RMax, OuterBase_Z, 0*deg, 360*deg);

    G4LogicalVolume* OuterBase_LV = new G4LogicalVolume(OuterBase_SV, fWater, "WaterBase_LV");

    new G4PVPlacement(0,OuterBase_Ps,OuterBase_LV,"OuterBase_PV",worldLV,false,0,checkOverlaps);

    // Base Ground in Bunker area
    /// AuxInstallationBox_SV : Axiliar Boxes to Subtract the target and room installation
    ///
    G4double hz_AuxInstallationBox = 2000*cm;
    G4Box* AuxInstallationBox_SV = new G4Box("AuxFloorBox_SV",Lx_ConcBox/2,Ly_ConcBox/2,hz_AuxInstallationBox);

    G4double OuterBase2_Z = (750*cm-250*cm)/2;
    G4double OuterBase2_dz = 250*cm+OuterBase2_Z;
    G4ThreeVector OuterBase2_Ps = G4ThreeVector(0,0,OuterBase2_dz);

    G4VSolid* OuterBase2_SV = new G4Tubs("OuterBase2_SV",0,ConcBunk_RMax, OuterBase2_Z, -120*deg, 240*deg);

    G4VSolid* OuterBase2_RestSV = new G4SubtractionSolid("OuterBase2_RestSV", OuterBase2_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4LogicalVolume* OuterBase2_LV = new G4LogicalVolume(OuterBase2_RestSV, fWater, "WaterBase_LV");

    new G4PVPlacement(0,OuterBase2_Ps,OuterBase2_LV,"OuterBase2_PV",worldLV,false, 0,checkOverlaps);


    G4double OuterBase3_Z = (750*cm-300*cm)/2;
    G4double OuterBase3_dz = 300*cm+OuterBase3_Z;
    G4ThreeVector OuterBase3_Ps = G4ThreeVector(0,0,OuterBase3_dz);
    G4VSolid* OuterBase3_SV = new G4Tubs("OuterBase3_SV",0,ConcBunk_RMax, OuterBase3_Z, 120*deg, 120*deg);

    G4VSolid* OuterBase3_RestSV = new G4SubtractionSolid("OuterBase2_RestSV", OuterBase3_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4LogicalVolume* OuterBase3_LV = new G4LogicalVolume(OuterBase3_RestSV, fWater, "WaterBase_LV");

    new G4PVPlacement(0,OuterBase3_Ps,OuterBase3_LV,"OuterBase3_PV",worldLV,false,0,  checkOverlaps);


// ----------------------------------------------------------------------------
//
// INSTALLATION GROUND LEVEL
//
// ----------------------------------------------------------------------------

    // Base Ground in Bunker area
    /// CShCap_SV : Axiliar Boxes to Subtract the target and room installation

    G4double ConcBaseR_dz = 300*cm-ConcBase_Z;
    G4ThreeVector ConcBaseR_Ps = G4ThreeVector(0,0,ConcBaseR_dz);

    G4VSolid* ConcBaseR_SV = new G4Tubs("ConcBaseR_SV",ConcBunk_RMin,ConcBunk_RMax, ConcBase_Z, 120*deg, 120*deg);

    G4VSolid* ConcBaseR_RestSV = new G4SubtractionSolid("ConcBaseR_RestSV", ConcBaseR_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4LogicalVolume* ConcBaseR_LV = new G4LogicalVolume(ConcBaseR_RestSV, fConcrete, "BaseConc_LV");

    new G4PVPlacement(0,ConcBaseR_Ps,ConcBaseR_LV,"ConcBaseR_PV",worldLV,false,0,checkOverlaps);

    ///
    G4double ConcBase2_dz = ConcBaseR_dz-2*ConcBase_Z;
    G4ThreeVector ConcBase2_Ps = G4ThreeVector(0,0,ConcBase2_dz);

    G4VSolid* ConcBaseS_SV = new G4Tubs("ConcBaseS_SV",ConcBunk_RMin,ConcBunkShort_R2, ConcBase_Z, 180*deg, 60*deg);
    G4VSolid* ConcBaseS_RestSV = new G4SubtractionSolid("ConcBaseS_RestSV", ConcBaseS_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4LogicalVolume* ConcBaseS_LV = new G4LogicalVolume(ConcBaseS_RestSV, fConcrete, "BaseConc_LV");

    G4VSolid* ConcBaseL_SV = new G4Tubs("ConcBaseL_SV",ConcBunk_RMin,ConcBunk_RMax, ConcBase_Z, -120*deg, 300*deg);
    G4VSolid* ConcBaseL_RestSV = new G4SubtractionSolid("ConcBaseL_RestSV", ConcBaseL_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4LogicalVolume* ConcBaseL_LV = new G4LogicalVolume(ConcBaseL_RestSV, fConcrete, "BaseConc_LV");

    new G4PVPlacement(0,ConcBase2_Ps,ConcBaseS_LV,"ConcBaseS_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,ConcBase2_Ps,ConcBaseL_LV,"ConcBaseL_PV",worldLV,false,0,checkOverlaps);


    // Ground Floor for the installation. Outside the rooms (Between target and bunker)

    /// CylFloor_SV : Axiliar Cyliner for Intersection
    G4double hz_GroundFloor = 100*cm/2;
    G4Box* GroundFloor_SV = new G4Box("GroundFloor_SV",Lx_ConcBox/2,Ly_ConcBox/2,hz_GroundFloor);

    G4double ClyFloor_Z = 250*cm/2;
    G4double GroundFloor_dz = 250*cm-hz_GroundFloor;
    G4ThreeVector GroundFloor_Ps = G4ThreeVector(0,0,GroundFloor_dz);

    G4VSolid* CylFloor_SV = new G4Tubs("CylFloor_SV",ConcBunk_RMin,ConcBunk_RMax, ClyFloor_Z, -60*deg,120*deg);
    G4VSolid* GroundFloor_IntSV = new G4IntersectionSolid("GroundFloor_IntSV",GroundFloor_SV, CylFloor_SV, 0, G4ThreeVector());
    G4LogicalVolume* GroundFloor_LV = new G4LogicalVolume(GroundFloor_IntSV, fConcrete, "GroundFloor_LV");

    new G4PVPlacement(0,GroundFloor_Ps,GroundFloor_LV,"GroundFloor_PV",worldLV,false, 0,checkOverlaps);

    new G4PVPlacement(Reflection_Rot,GroundFloor_Ps,GroundFloor_LV,"GroundFloorRight_PV",worldLV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// BUNKER (Bunk) MAGNADENSE8 BY LKAB MINERALS (bunker density)
//
// ----------------------------------------------------------------------------

    // Ground of Bunker

    G4double ConcBunkFloor_Z = ConcBase_Z;
    G4double ConcBunkFloor_dz = ConcBase2_dz-2*ConcBunkFloor_Z;
    G4ThreeVector ConcBunkFloor_Ps = G4ThreeVector(0,0,ConcBunkFloor_dz);

    G4VSolid* ConcBunkLFloor_SV = new G4Tubs("ConcBunkLFloor_SV",
                                             ConcBunk_RMin,ConcBunkLong_R2, ConcBunkFloor_Z, BunkL_phi0,BunkL_dphi);
    G4VSolid* ConcBunkLFloor_RestSV = new G4SubtractionSolid("ConcBunkLFloor_RestSV", ConcBunkLFloor_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4VSolid* ConcBunkSFloor_SV = new G4Tubs("ConcBunkSFloor_SV",
                                             ConcBunk_RMin,ConcBunkShort_R2, ConcBunkFloor_Z, BunkS_phi0,BunkS_dphi);
    G4VSolid* ConcBunkSFloor_RestSV = new G4SubtractionSolid("ConcBunkSFloor_RestSV", ConcBunkSFloor_SV, AuxInstallationBox_SV,0,G4ThreeVector());

    G4LogicalVolume* ConcBunkLFloor_LV = new G4LogicalVolume(ConcBunkLFloor_RestSV, fConcrete, "GroundFloor_LV");
    G4LogicalVolume* ConcBunkSFloor_LV = new G4LogicalVolume(ConcBunkSFloor_RestSV, fConcrete, "GroundFloor_LV");


    new G4PVPlacement(0,ConcBunkFloor_Ps,ConcBunkLFloor_LV,"ConcBunkLFloor_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(Reflection_Rot,ConcBunkFloor_Ps,ConcBunkLFloor_LV,"ConcBunkLFloorRight_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,ConcBunkFloor_Ps,ConcBunkSFloor_LV,"ConcBunkSFloor_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(Reflection_Rot,ConcBunkFloor_Ps,ConcBunkSFloor_LV,"ConcBunkSFloorRight_PV",worldLV,false,0,checkOverlaps);

    // Bunker Walls (Magnadense8)

    ESSN_BunkerL BunkL_PV = ESSN_BunkerL();
    BunkL_PV.Construct();
    G4double Bunk_Z = BunkL_PV.Bunk_height();
    new G4PVPlacement(0, BunkL_PV.Position(),
                     BunkL_PV.GetLogicalVolume(), "BunkL_PV", worldLV,
                     false, 0, checkOverlaps);


    ESSN_BunkerS BunkS_PV = ESSN_BunkerS();
    BunkS_PV.Construct();
    new G4PVPlacement(0, BunkL_PV.Position(),
                     BunkS_PV.GetLogicalVolume(), "BunkS_PV", worldLV,
                     false, 0, checkOverlaps);


   ESSN_BunkerLR BunkLR_PV = ESSN_BunkerLR();
   BunkLR_PV.Construct();
   new G4PVPlacement(0, BunkL_PV.Position(),
                    BunkLR_PV.GetLogicalVolume(), "BunkLR_PV", worldLV,
                    false, 0, checkOverlaps);

    ESSN_BunkerSR BunkSR_PV = ESSN_BunkerSR();
    BunkSR_PV.Construct();
    new G4PVPlacement(0, BunkL_PV.Position(),
                     BunkSR_PV.GetLogicalVolume(), "BunkSR_PV", worldLV,
                     false, 0, checkOverlaps);


    //Bunker Roof

    G4double BunkRoof_Z = (310*cm-180*cm)/2;
    G4double BunkRoof_dz = -(180*cm+BunkRoof_Z);
    G4ThreeVector BunkRoof_Ps = G4ThreeVector(0,0,BunkRoof_dz);

    G4VSolid* BunkLRoof_SV = new G4Tubs("BunkLRoof_SV",
                                    ConcBunk_RMin,ConcBunkLong_R2, BunkRoof_Z, BunkL_phi0,BunkL_dphi);

    G4VSolid* BunkSRoof_SV = new G4Tubs("BunkSRoof_SV",
                                    ConcBunk_RMin,ConcBunkShort_R2, BunkRoof_Z,  BunkS_phi0,BunkS_dphi);

    G4double BunkRoof2_Z = (330*cm-310*cm)/2;
    G4double BunkRoof2_dz = -(310*cm+BunkRoof2_Z);
    G4ThreeVector BunkRoof2_Ps = G4ThreeVector(0,0,BunkRoof2_dz);

    G4VSolid* BunkSRoof2_SV = new G4Tubs("BunkSRoof2_SV",
                                    ConcBunk_RMin,ConcBunkShort_R2, BunkRoof2_Z, BunkL_phi0,BunkL_dphi);

    G4LogicalVolume* BunkLRoof_LV = new G4LogicalVolume(BunkLRoof_SV, fMagnadense, "Bunk_LV");
    G4LogicalVolume* BunkSRoof_LV = new G4LogicalVolume(BunkSRoof_SV, fMagnadense, "Bunk_LV");
    G4LogicalVolume* BunkSRoof2_LV = new G4LogicalVolume(BunkSRoof2_SV, fMagnadense, "Bunk_LV");

    new G4PVPlacement(0,BunkRoof_Ps,BunkLRoof_LV,"BunkLRoof_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,BunkRoof_Ps,BunkSRoof_LV,"BunkSRoof_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(Reflection_Rot,BunkRoof_Ps,BunkLRoof_LV,"BunkLRoof_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(Reflection_Rot,BunkRoof_Ps,BunkSRoof_LV,"BunkSRoof_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,BunkRoof2_Ps,BunkSRoof2_LV,"BunkSRoof2_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(Reflection_Rot,BunkRoof2_Ps,BunkSRoof2_LV,"BunkSRoof2_PV",worldLV,false,0,checkOverlaps);

    //Bunker Wall
    G4double BunkWall_Z = Bunk_Z + BunkRoof_Z;
    G4double BunkWall_dz = 150*cm-BunkWall_Z;
    G4ThreeVector BunkWall_Ps = G4ThreeVector(0,0,BunkWall_dz);
    G4double BunkThick_dist = 199.99*cm;
    G4ThreeVector BunkCenterDist_Ps = G4ThreeVector(-BunkThick_dist*sqrt(2)/2,-BunkThick_dist*sqrt(2)/2,BunkRoof2_dz);

    G4Box* AuxBoxBunk_SV = new G4Box("AuxBoxBunk_SV",
                               5000*cm,BunkThick_dist,5000*cm);

    G4RotationMatrix* AuxBoxBunk_Rot = new G4RotationMatrix();
    AuxBoxBunk_Rot->rotateZ(-BunkL_phi0);

    G4VSolid* BunkWall_SV = new G4Tubs("BunkWall_SV",
                                    ConcBunkShort_R2,ConcBunkLong_R2, BunkWall_Z, 0,BunkL_phi0);

    G4VSolid* BunkWall_IntSV = new G4IntersectionSolid("BunkWall_IntSV",BunkWall_SV, AuxBoxBunk_SV, AuxBoxBunk_Rot, G4ThreeVector());

    G4LogicalVolume*  BunkWall_IntLV = new  G4LogicalVolume(BunkWall_IntSV,fConcrete, "Bunk_LV");
    new G4PVPlacement(0,BunkWall_Ps,BunkWall_IntLV,"BunkWall_PV",worldLV,false,0,checkOverlaps);
    new G4PVPlacement(Reflection_Rot,BunkWall_Ps,BunkWall_IntLV,"BunkWall_PV",worldLV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// CONCRETE SHIELD Roof and Floor (CShCap)
// CONCRETE SHIELD Long and Short Walls (CShLWall, CShSWall)
//
// ----------------------------------------------------------------------------

    // ABOVE TARGET (CShRoof)
    // BELOW TARGET (CShFloor)

    G4double hx_CShCap = Lx_ConcBox/2;
    G4double hy_CShCap = Ly_ConcBox/2;
    G4double hz_CShCap = Lz_ConcBox/2;
    G4Box* CShCap_SV = new G4Box("CShCap_SV",
                               hx_CShCap,hy_CShCap,hz_CShCap);

    /// Relative to world center
    G4double CShRoof_dz = -(800*cm+hz_CShCap);
    G4ThreeVector CShRoof_Ps = G4ThreeVector(0,0,CShRoof_dz);

    G4double CShFloor_dz = 750*cm-hz_CShCap;
    G4ThreeVector CShFloor_Ps = G4ThreeVector(0,0,CShFloor_dz);

    G4LogicalVolume* CShRoof_LV = new  G4LogicalVolume(CShCap_SV,fConcrete, "CSh_LV");
    G4LogicalVolume* CShFloor_LV = new  G4LogicalVolume(CShCap_SV,fConcrete, "CSh_LV");

    new G4PVPlacement(0,CShRoof_Ps,CShRoof_LV,"CShRoof_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,CShFloor_Ps,CShFloor_LV,"CShFloor_PV",worldLV,false,0,checkOverlaps);

    G4double hxy_CShWall = 100*cm/2;
    //G4double hx_CShWall = 1980*cm/2;
    G4double hz_CShWall = (550*cm-250*cm)/2;
    G4Box* CShLWall_SV = new G4Box("CShLWall_SV",hxy_CShWall,hy_CShCap,hz_CShWall);

    /// Relative to world center
    G4double CShWall_dz = 550*cm-hz_CShWall;
    G4double CShLWall_dx = -(1090*cm-hxy_CShWall);
    G4ThreeVector CShLWall_Ps = G4ThreeVector(CShLWall_dx,0,CShWall_dz);

    G4LogicalVolume* CShLWall_LV = new  G4LogicalVolume(CShLWall_SV,fConcrete, "CSh_LV");
    new G4PVPlacement(0,CShLWall_Ps,CShLWall_LV,"CShLWall_PV",worldLV,false,0,checkOverlaps);

    /// REPLICA.

    G4RotationMatrix* CShLWall_Rot=new G4RotationMatrix();
    CShLWall_Rot->rotateY(-180*deg);

    G4Translate3D CShLWallRep_Ps (-CShLWall_dx, 0, CShWall_dz);
    G4Transform3D CShLWallRep_Rotation = G4Rotate3D(*CShLWall_Rot);
    G4ReflectY3D CShLWallRep_Reflection;
    G4Transform3D CShLWallRep_Trans = CShLWallRep_Ps*CShLWallRep_Rotation*CShLWallRep_Reflection;

    G4ReflectionFactory::Instance()->Place(CShLWallRep_Trans, // the transformation with reflection
                     "CShLWallTransPV",
                     CShLWall_LV,
                     worldLV,
                     false,
                     1,
                     checkOverlaps);

    G4double hx_AirGapUp= 200*cm;
    G4double hy_AirGapUp = (2100*cm-806*cm)/2;
    G4double hz_AirGapUp = (950*cm-800*cm)/2;
    G4Box* AirGapUp_SV = new G4Box("AirGapUp_SV",
                               hx_AirGapUp,hy_AirGapUp,hz_AirGapUp);

    /// Relative to CShRoof
    G4double AirGapUp_dz = -(800*cm+hz_AirGapUp);
    G4double AirGapUp_dy = (806*cm+hy_AirGapUp);
    G4ThreeVector AirGapUp_Ps = G4ThreeVector(0,-AirGapUp_dy,AirGapUp_dz-CShRoof_dz);

    G4LogicalVolume* AirGapUp_LV = new  G4LogicalVolume(AirGapUp_SV,fAir, "AirGapRoof_LV");
    new G4PVPlacement(0,AirGapUp_Ps,AirGapUp_LV,"AirGapUp_PV",CShRoof_LV,false,0,checkOverlaps);


// ----------------------------------------------------------------------------
//
// CONCRETE SHIELD AROUND TARGET
//
// ----------------------------------------------------------------------------


    // CONCRETE SHIELD AROUND UPPER PART OF THE TARGET (1st layer)

    G4double ConcreteShield1_R1 = 506*cm;
    G4double ConcreteShield1_R2 = 550*cm;
    G4double ConcreteShield1_Z  = (800*cm-200*cm)/2;
    //
    G4double ConcreteShield1_dz = -(200*cm+ConcreteShield1_Z);
    G4ThreeVector ConcreteShield1_Ps = G4ThreeVector(0,0,ConcreteShield1_dz);

    G4VSolid* ConcreteShield1_SV = new G4Tubs("ConcreteShield1_SV",
     ConcreteShield1_R1,ConcreteShield1_R2, ConcreteShield1_Z, 0*deg, 360*deg);

    G4LogicalVolume* ConcreteShield1_LV = new G4LogicalVolume(ConcreteShield1_SV, fConcrete, "ConcreteShield_LV");

    new G4PVPlacement(0,ConcreteShield1_Ps,ConcreteShield1_LV,"ConcreteShield1_PV",worldLV,false,0,checkOverlaps);

    // Outer Layer

    G4double ConcreteShield_R1 = ConcreteShield1_R2;
    G4double ConcreteShield_Z  = (800*cm-296*cm)/2;
    //
    G4double ConcreteShield_dz = -(296*cm+ConcreteShield_Z);
    G4ThreeVector ConcreteShield_Ps = G4ThreeVector(0,0,ConcreteShield_dz);

    G4VSolid* ConcreteShield_SV = new G4Tubs("ConcreteShield_SV",
     ConcreteShield_R1,ConcreteShield_R2, ConcreteShield_Z, -60*deg, 120*deg);

    G4LogicalVolume* ConcreteShield_LV
    = new G4LogicalVolume(ConcreteShield_SV, fConcrete, "ConcreteShield_LV");

    new G4PVPlacement(0,ConcreteShield_Ps,ConcreteShield_LV,"ConcreteShield_PV",worldLV,false,0,checkOverlaps);
    new G4PVPlacement(Reflection_Rot,ConcreteShield_Ps,ConcreteShield_LV,"ConcreteShield_PV",worldLV,false,0,checkOverlaps);

    // CONCRETE SHIELD UNDER TARGET (CShDown)

    G4double CShDown_R1 = 0*cm;
    G4double CShDown_R2 = 550*cm;
    G4double CShDown_Z = (550*cm-224*cm)/2;

    G4double CShDown1_R1 = 280*cm;
    G4double CShDown1_Z = (224*cm-76*cm)/2;

    G4VSolid* CShDown_SV = new G4Tubs("CShDown_SV",
     CShDown_R1,CShDown_R2, CShDown_Z, 0, 360*deg);

    G4VSolid* CShDown1_SV = new G4Tubs("CShDown1_SV",
     CShDown1_R1, CShDown_R2, CShDown1_Z, 0, 360*deg);

    // Air gap between SS316L shield and Concrete shield
    G4double AirGapDown_R1 = 273*cm;
    G4double AirGapDown_Z = (191*cm-76*cm)/2;
    G4VSolid* AirGapDown_SV = new G4Tubs("AirGapDown_SV",
     AirGapDown_R1,CShDown1_R1, AirGapDown_Z, 0, 360*deg);

    /// Relative to world center
    G4double CShDown_dz = (550*cm-CShDown_Z);
    G4ThreeVector CShDown_Ps = G4ThreeVector(0,0,CShDown_dz);

    G4double CShDown1_dz = (224*cm-CShDown1_Z);
    G4ThreeVector CShDown1_Ps = G4ThreeVector(0,0,CShDown1_dz);

    G4double AirGapDown_dz = (191*cm-AirGapDown_Z);
    G4ThreeVector AirGapDown_Ps = G4ThreeVector(0,0,AirGapDown_dz);

    G4LogicalVolume* CShDown_LV = new  G4LogicalVolume(CShDown_SV,fConcrete, "CSh_LV");
    G4LogicalVolume* CShDown1_LV = new  G4LogicalVolume(CShDown1_SV,fConcrete, "CSh_LV");
    G4LogicalVolume* AirGapDown_LV = new  G4LogicalVolume(AirGapDown_SV,fAir, "AirGapDown_LV");

    new G4PVPlacement(0,CShDown_Ps,CShDown_LV,"CShDown_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,CShDown1_Ps,CShDown1_LV,"CShDown1_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,AirGapDown_Ps,AirGapDown_LV,"AirGapDown_PV",worldLV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// OUTER TARGET SHIELDING (OTrSh)
//
// ----------------------------------------------------------------------------

    ESSN_OutTrShield OTrSh1_PV = ESSN_OutTrShield();
    OTrSh1_PV.Construct();
    G4ThreeVector OTrSh1_dim_ = OTrSh1_PV.Dimension();
    //G4double OTrSh1_R1 = OTrSh1_dim_[0];
    G4double OTrSh1_R2 = OTrSh1_dim_[1];
    //G4double OTrSh1_Z = OTrSh1_dim_[2];

    new G4PVPlacement(0, OTrSh1_PV.Position(),
                     OTrSh1_PV.GetLogicalVolume(), "OTrSh1_PV", worldLV,
                     false, 0, checkOverlaps);


    // Standard grade carbon steel AISI 1005 outer target shield
    G4double OTrSh2_R1 = 292.5*cm;
    G4double OTrSh2_R2 = 506*cm;
    G4double OTrSh2_Z = (395*cm-200*cm)/2;

    G4double OTrSh3_R1 = OTrSh2_R1;
    G4double OTrSh3_R2 = OTrSh1_R2;
    G4double OTrSh3_Z = (200*cm-TrSh1_Ztop)/2;

    G4VSolid* OTrSh2_SV = new G4Tubs("OTrSh2_SV",
                                     OTrSh2_R1,OTrSh2_R2, OTrSh2_Z, 0, 360*deg);

    G4VSolid* OTrSh3_SV = new G4Tubs("OTrSh3_SV",
                                     OTrSh3_R1,OTrSh3_R2, OTrSh3_Z, 0, 360*deg);

    G4double OTrSh2_dz = -(395*cm-OTrSh2_Z);
    G4ThreeVector OTrSh2_Ps = G4ThreeVector(0,0,OTrSh2_dz);

    G4double OTrSh3_dz = -(200*cm-OTrSh3_Z);
    G4ThreeVector OTrSh3_Ps = G4ThreeVector(0,0,OTrSh3_dz);

    G4LogicalVolume* OTrSh2_LV = new  G4LogicalVolume(OTrSh2_SV,fCarbonSteel, "OTrSh_LV");
    G4LogicalVolume* OTrSh3_LV = new  G4LogicalVolume(OTrSh3_SV,fCarbonSteel, "OTrSh_LV");

    new G4PVPlacement(0,OTrSh2_Ps,OTrSh2_LV,"OTrSh2_PV",worldLV,false,0,checkOverlaps);
    new G4PVPlacement(0,OTrSh3_Ps,OTrSh3_LV,"OTrSh3_PV",worldLV,false,0,checkOverlaps);

// OTrSh Top Air Gap

    G4double TrShTopAir1_R1 = OTrSh2_R1;
    G4double TrShTopAir1_R2 = OTrSh2_R2;
    G4double TrShTopAir1_Z = (695*cm-395*cm)/2;
    G4VSolid* TrShTopAir1_SV = new G4Tubs("TrShTopAir1_SV",
                                          TrShTopAir1_R1,TrShTopAir1_R2, TrShTopAir1_Z, 0, 360*deg);

    G4double TrShTopAir_Z = (800*cm-695*cm)/2;
    G4VSolid* TrShTopAir_SV = new G4Tubs("TrShTopAir_SV",
                                         0,TrShTopAir1_R2, TrShTopAir_Z, 0, 360*deg);

    /// Relative to world center
    G4double TrShTopAir1_dz = -(695*cm-TrShTopAir1_Z);
    G4ThreeVector TrShTopAir1_Ps = G4ThreeVector(0,0,TrShTopAir1_dz);

    G4double TrShTopAir_dz = -(800*cm-TrShTopAir_Z);
    G4ThreeVector TrShTopAir_Ps = G4ThreeVector(0,0,TrShTopAir_dz);

    G4LogicalVolume* TrShTopAir1_LV = new  G4LogicalVolume(TrShTopAir1_SV,fAir, "TrShTopAir_LV");
    G4LogicalVolume* TrShTopAir_LV = new  G4LogicalVolume(TrShTopAir_SV,fAir, "TrShTopAir_LV");

    new G4PVPlacement(0,TrShTopAir1_Ps,TrShTopAir1_LV,"TrShTopAir1_PV",worldLV,false,0,checkOverlaps);
    new G4PVPlacement(0,TrShTopAir_Ps,TrShTopAir_LV,"TrShTopAir_PV",worldLV,false,0,checkOverlaps);


    // ----------------------------------------------------------------------------
    //
    // Dome on top of Target Shielding
    //
    // ----------------------------------------------------------------------------

    G4double TrShSSTop_RMax = 292.5*cm;
    G4double TrShVacTop_RMax = 290*cm;
    G4double TrShTop_Z = (695*cm-400*cm)/2;

    G4VSolid* TrShSSTop_SV = new G4Tubs("TrShSSTop_SV",
               TrShVacTop_RMax,TrShSSTop_RMax,TrShTop_Z, 0, 360*deg);

    G4VSolid* TrShVacTop_SV = new G4Tubs("TrShVacTop_SV",
               0,TrShVacTop_RMax,TrShTop_Z, 0, 360*deg);

    G4LogicalVolume* TrShSSTop_LV = new  G4LogicalVolume(TrShSSTop_SV,fSS316L, "TrShSSTop_LV");
    G4LogicalVolume* TrShVacTop_LV = new  G4LogicalVolume(TrShVacTop_SV,fVacuum, "TrShVacTop_LV");

    G4double TrShTop_dz = -(695*cm-TrShTop_Z);
    //G4ThreeVector TrShTop_Sh2Ps = G4ThreeVector(0,0,TrShTop_dz-TrSh2_dz);
    G4ThreeVector TrShTop_Ps = G4ThreeVector(0,0,TrShTop_dz);

    new G4PVPlacement(0,TrShTop_Ps,TrShSSTop_LV,"TrShSSTop_PV",worldLV,false,0,checkOverlaps);

    new G4PVPlacement(0,TrShTop_Ps,TrShVacTop_LV,"TrShVacTop_PV",worldLV,false,0,checkOverlaps);


    G4VSolid* TrShDome1_SV = new G4Orb("TrShDome1_SV",880.5625*cm);
    G4VSolid* TrShDome2_SV = new G4Orb("TrShDome2_SV",878.0625*cm);

    G4double TrShDome_dz = 130.5625*cm;

    G4ThreeVector TrShDome_Ps = G4ThreeVector(0,0,TrShDome_dz-TrShTopAir_dz);

    // Axiliar Tubes to get the dome (Intersection)
    /// AuxBox1:  for SS dome
    /// AuxBox2: for Vac dome
    G4double AuxDome1_R = 292.5*cm;
    G4double AuxDome2_R = 290*cm;
    G4double AuxDome_Z = (780*cm-695*cm)/2;

    G4VSolid* AuxDome1SV = new G4Tubs("AuxDomeSV",
                  0,AuxDome1_R,AuxDome_Z, 0, 360*deg);

    G4VSolid* AuxDome2SV = new G4Tubs("AuxDome2SV",
                                  0,AuxDome2_R,AuxDome_Z, 0, 360*deg);

    G4double AuxDome_dz = -(695*cm + AuxDome_Z);

    // Relative to sphere
    G4ThreeVector AuxBoxDome_Ps = G4ThreeVector(0,0,AuxDome_dz-TrShDome_dz);

    G4VSolid* TrShDomeSS_SV = new G4IntersectionSolid("TrShDomeSS_SV",  TrShDome1_SV, AuxDome1SV ,0,AuxBoxDome_Ps);
    G4VSolid* TrShDomeVac_SV = new G4IntersectionSolid("TrShDomeVac_SV",  TrShDome2_SV, AuxDome2SV ,0,AuxBoxDome_Ps);

    G4LogicalVolume* TrShDomeSS_LV = new  G4LogicalVolume(TrShDomeSS_SV,fSS316L, "TrShDomeSS_LV");
    G4LogicalVolume* TrShDomeVac_LV = new  G4LogicalVolume(TrShDomeVac_SV,fVacuum, "TrShDomeVac_LV");

    new G4PVPlacement(0,TrShDome_Ps,TrShDomeSS_LV,"TrShDomeSS_PV",TrShTopAir_LV,false,0,checkOverlaps);

    new G4PVPlacement(0,G4ThreeVector(),TrShDomeVac_LV,"TrShDomeVac_PV",TrShDomeSS_LV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// INNER TARGET SS316L SHIELDING (TrSh)
//
// ----------------------------------------------------------------------------

    ESSN_SSTrShield TrSh1_PV = ESSN_SSTrShield();
    TrSh1_PV.Construct();
    G4double TrSh1_R1 = TrSh1_PV.Radius();
    //G4double TrSh1_Z = TrSh1_PV.Height();
    //G4double TrSh1_dz =  TrSh1_PV.Position()[2];
    new G4PVPlacement(0, TrSh1_PV.Position(),
                     TrSh1_PV.GetLogicalVolume(), "TrSh1_PV", worldLV,
                     false, 0, checkOverlaps);

    ESSN_UpperSSTrShield TrSh2_PV = ESSN_UpperSSTrShield();
    TrSh2_PV.Construct();
    new G4PVPlacement(0, TrSh2_PV.Position(),
                    TrSh2_PV.GetLogicalVolume(), "TrSh2_PV", worldLV,
                    false, 0, checkOverlaps);

    // Concrete regular + 50%Fe below SS316L shielding
    ESSN_ConcTrShield TrShConc_PV = ESSN_ConcTrShield();
    TrShConc_PV.Construct();
    G4double TrShConc_R = TrShConc_PV.Radius();
    G4double TrShCarbonD_R = TrShConc_PV.TrShCarbonD_Rad();
    new G4PVPlacement(0, TrShConc_PV.Position(),
                     TrShConc_PV.GetLogicalVolume(), "TrShConc_PV", worldLV,
                     false, 0, checkOverlaps);

    ///Coners around SS shield
    G4double TrShConc1_RMin = TrSh1_R1;
    G4double TrShConc1_Z = (200*cm-191*cm)/2;
    G4double TrShConc1_dz = (200*cm-TrShConc1_Z);
    G4ThreeVector TrShConc1_Ps = G4ThreeVector(0,0,TrShConc1_dz);

    G4VSolid* TrShConc1_SV = new G4Tubs("TrShConc1_SV",
                                        TrShConc1_RMin, TrShConc_R, TrShConc1_Z, 0, 360*deg);
    G4LogicalVolume* TrShConc1_LV = new  G4LogicalVolume(TrShConc1_SV,fConcrete50Fe, "TrShConc_LV");
    new G4PVPlacement(0,TrShConc1_Ps,TrShConc1_LV,"TrShConc1_PV",worldLV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// Standard grade carbon steel AISI 1005 below target shield
//
// ----------------------------------------------------------------------------

    // lowet part whicih is inside the fConcrete mother volume
    G4double TrShCarbonD_CShDownZ = (296.5*cm-224*cm)/2;
    G4VSolid* TrShCarbonD_CShDownSV = new G4Tubs("TrShCarbonD_CShDownSV",
     0, TrShCarbonD_R, TrShCarbonD_CShDownZ, 0, 360*deg);

    /// Relative to surrounding concrete shield
    G4double TrShCarbonD_CShDowndz = (296.5*cm-TrShCarbonD_CShDownZ);
    G4ThreeVector TrShCarbonD_CShDownPs = G4ThreeVector(0,0,TrShCarbonD_CShDowndz-CShDown_dz);

    G4LogicalVolume* TrShCarbonD_CShDownLV = new  G4LogicalVolume(TrShCarbonD_CShDownSV,fCarbonSteel, "TrShCarbon_LV");
    new G4PVPlacement(0,TrShCarbonD_CShDownPs,TrShCarbonD_CShDownLV,"TrShCarbonD_CShDownPV",CShDown_LV,false,0,checkOverlaps);


// ----------------------------------------------------------------------------
//
// END OF TARGET DEFINITION
//
// ----------------------------------------------------------------------------



// ----------------------------------------------------------------------------
//
//                               ROOMS AT 3 LEVELS
//
// ----------------------------------------------------------------------------

    //....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

        // Axiliar Boxes to Subtract the coners of Trapeziodal rooms

        G4Box* AuxBoxSV = new G4Box("AuxBoxSV",
                      hx_AuxBox,hy_AuxBox,hz_AuxBox);

        /// AuxBox1: bottom side in floorplan
        /// AuxBox2: top side in floorplan
        G4double dx_AuxBox = (Lx_ConcBox-200*cm)/2 + hx_AuxBox;
        G4ThreeVector AuxBox1_Ps = G4ThreeVector(dx_AuxBox,0,0);
        G4ThreeVector AuxBox2_Ps = G4ThreeVector(-dx_AuxBox,0,0);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// ----------------------------------------------------------------------------
//
// GROUND LEVEL
//
// ----------------------------------------------------------------------------

    G4RotationMatrix* TRight_Rot = new G4RotationMatrix();
    TRight_Rot->rotateX(90.*deg);

    // ----------------------------------------------------------------------------
    //
    // LEFT SIDE
    //
    // ----------------------------------------------------------------------------

    // SV Outer Trapezoid

    G4RotationMatrix* TLeft_Rot =  new G4RotationMatrix();
    TLeft_Rot->rotateX(-90.*deg);

    ESSN_GroundRoomL TLeftPV = ESSN_GroundRoomL();
        TLeftPV.Construct();
        new G4PVPlacement(TLeft_Rot, TLeftPV.Position(),
                         TLeftPV.GetLogicalVolume(), "TLeftPV", worldLV,
                         false, 0, checkOverlaps);


    G4double dx_1TL = TLeftPV.Dimension()[0];
    G4double dx_2TL = TLeftPV.Dimension()[1];
    //G4double dz_TL = TLeftPV.Dimension()[2];
    G4double dy_small = sqrt(3)*dx_2TL;
    G4double dy_TL = (sqrt(3))*(dx_1TL-dy_small)/2 ; //1683.7*cm/2;

    G4double dx_1InTL = TLeftPV.InDimension()[0];
    G4double dx_2InTL = TLeftPV.InDimension()[1];
    //G4double dz_InTL = TLeftPV.InDimension()[2];
    G4double dy_Insmall = sqrt(3)*dx_2InTL;
    G4double dy_InTL = (sqrt(3))*(dx_1InTL-dy_Insmall)/2 ;  // 1420.4*cm/2;

    G4double dy0_TLeft = TLeftPV.Position()[1];
    //G4double dz0_TLeft = TLeftPV.Position()[2];

    G4double dy0_InTLeft = TLeftPV.InPosition()[2];
    //G4double dz0_InTLeft = - TLeftPV.InPosition()[1];

    G4ThreeVector SubCyl_Ps = TLeftPV.SubCyl_Position();
    G4ThreeVector SubCylIn_Ps = TLeftPV.SubCyl_InPosition();
    G4double SubCyl_Z = TLeftPV.SubCyl_height();

    // Air Gap Curved joint at GROUND LEVEL

        // LEFT SIDE
        G4double AirCurve_RMin = ConcreteShield1_R2;
        G4double AirCurve_RMax = 556*cm;
        G4double AirCurveL_Z = (206*cm+76*cm)/2;
        G4VSolid* AirCurveL_SV = new G4Tubs("AirCurveL_SV",AirCurve_RMin,AirCurve_RMax,AirCurveL_Z,60*deg,60*deg);

        /// Relative to world center
        G4double dz_AirCurveL = -(206*cm-AirCurveL_Z);
        G4ThreeVector AirCurveL_Ps(0,0,dz_AirCurveL);
        G4LogicalVolume* AirCurveL_LV = new  G4LogicalVolume(AirCurveL_SV,fAir, "AirCurve_LV");

        new G4PVPlacement(0,AirCurveL_Ps,AirCurveL_LV,"AirCurveL_PV",worldLV,false,0,checkOverlaps);

    // Concrete Curved joint at GROUND LEVEL

        // LEFT SIDE divided in two terms
        G4double ConcCurveL_Z = AirCurveL_Z;
        G4double ConcCurveL_RMin = AirCurve_RMax;
        G4double ConcCurveL_RMax = ConcreteShield_R2;

        G4VSolid* ConcCurveL_SV = new G4Tubs("ConcCurveL_SV",ConcCurveL_RMin,ConcCurveL_RMax,ConcCurveL_Z,60*deg,60*deg);

        G4double ConcCurveLDown_Z =(250*cm-76*cm)/2;
        G4double ConcCurveLDown_RMin = AirCurve_RMin;
        G4double ConcCurveLDown_RMax = ConcreteShield_R2;
        G4VSolid* ConcCurveLDown_SV = new G4Tubs("ConcCurveLDown_SV",ConcCurveLDown_RMin,ConcCurveLDown_RMax,ConcCurveLDown_Z,60*deg,60*deg);

        G4double ConcCurveLUp_Z = (400*cm-206*cm)/2;
        G4double ConcCurveLUp_RMin = ConcCurveLDown_RMin;
        G4double ConcCurveLUp_RMax = ConcCurveLDown_RMax;
        G4VSolid* ConcCurveLUp_SV = new G4Tubs("ConcCurveLUp_SV",ConcCurveLUp_RMin,ConcCurveLUp_RMax,ConcCurveLUp_Z,60*deg,60*deg);


        /// Relative to world center
        G4double dz_ConcCurveL = dz_AirCurveL;
        G4ThreeVector ConcCurveL_Ps(0,0,dz_ConcCurveL);
        G4LogicalVolume* ConcCurveL_LV = new  G4LogicalVolume(ConcCurveL_SV,fConcrete, "ConcCurve_LV");

        G4double dz_ConcCurveLDown = 250*cm-ConcCurveLDown_Z;
        G4ThreeVector ConcCurveLDown_Ps(0,0,dz_ConcCurveLDown);
        G4LogicalVolume* ConcCurveLDown_LV = new  G4LogicalVolume(ConcCurveLDown_SV,fConcrete, "ConcCurve_LV");

        G4double dz_ConcCurveLUp = -(206*cm+ConcCurveLUp_Z);
        G4ThreeVector ConcCurveLUp_Ps(0,0,dz_ConcCurveLUp);
        G4LogicalVolume* ConcCurveLUp_LV = new  G4LogicalVolume(ConcCurveLUp_SV,fConcrete, "ConcCurve_LV");


        new G4PVPlacement(0,ConcCurveL_Ps,ConcCurveL_LV,"ConcCurveL_PV",worldLV,false,0,checkOverlaps);

        new G4PVPlacement(0,ConcCurveLDown_Ps,ConcCurveLDown_LV,"ConcCurveLDown_PV",worldLV,false,0,checkOverlaps);

        new G4PVPlacement(0,ConcCurveLUp_Ps,ConcCurveLUp_LV,"ConcCurveLUp_PV",worldLV,false,0,checkOverlaps);


    // ----------------------------------------------------------------------------
    //
    // RIGHT SIDE
    //
    // ----------------------------------------------------------------------------

    ESSN_GroundRoomR TRightPV = ESSN_GroundRoomR();
        TRightPV.Construct();
        new G4PVPlacement(TRight_Rot, TRightPV.Position(),
                         TRightPV.GetLogicalVolume(), "TRightPV", worldLV,
                         false, 0, checkOverlaps);

    // Air Gap Curved joint with Concrete Shield

        // RIGHT SIDE
        G4double AirCurveR_Z = (206*cm+76*cm)/2;
        G4VSolid* AirCurveR_SV = new G4Tubs("AirCurveR_SV",AirCurve_RMin,AirCurve_RMax,AirCurveR_Z,-120*deg,60*deg);

        /// Relative to world center
        G4double dz_AirCurveR = 76*cm-AirCurveR_Z;
        G4ThreeVector AirCurveR_Ps(0,0,dz_AirCurveR);
        G4LogicalVolume* AirCurveR_LV = new  G4LogicalVolume(AirCurveR_SV,fAir, "AirCurve_LV");

        new G4PVPlacement(0,AirCurveR_Ps,AirCurveR_LV,"AirCurveR_PV",worldLV,false,0,checkOverlaps);

    // Concrete Curved joint below Air Gap Curved joint

        // RIGHT SIDE divided in two terms
        //G4double ConcCurve_RMin = AirCurve_RMax;
        //G4double ConcCurve_RMax = 656*cm;

        G4double ConcCurveRUp_Z = (400*cm-206*cm)/2;
        G4VSolid* ConcCurveRUp_SV = new G4Tubs("ConcCurveRUp_SV",AirCurve_RMin,AirCurve_RMax,ConcCurveRUp_Z,-120*deg,60*deg);

        G4double ConcCurveRDown_Z = (250*cm-76*cm)/2;
        G4VSolid* ConcCurveRDown_SV = new G4Tubs("ConcCurveRDown_SV",AirCurve_RMin,AirCurve_RMax,ConcCurveRDown_Z,-120*deg,60*deg);


        /// Relative to world center
        G4double dz_ConcCurveRUp = -(206*cm+ConcCurveRUp_Z);
        G4ThreeVector ConcCurveRUp_Ps(0,0,dz_ConcCurveRUp);
        G4LogicalVolume* ConcCurveRUp_LV = new  G4LogicalVolume(ConcCurveRUp_SV,fConcrete, "ConcCurve_LV");

        G4double dz_ConcCurveRDown = 250*cm-ConcCurveRDown_Z;
        G4ThreeVector ConcCurveRDown_Ps(0,0,dz_ConcCurveRDown);
        G4LogicalVolume* ConcCurveRDown_LV = new  G4LogicalVolume(ConcCurveRDown_SV,fConcrete, "ConcCurve_LV");

        new G4PVPlacement(0,ConcCurveRUp_Ps,ConcCurveRUp_LV,"ConcCurveRUp_PV",worldLV,false,0,checkOverlaps);
        new G4PVPlacement(0,ConcCurveRDown_Ps,ConcCurveRDown_LV,"ConcCurveRDown_PV",worldLV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// TOP LEVEL
//
// ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------
    //
    // LEFT SIDE
    //
    // ----------------------------------------------------------------------------

    // SV Outer Trapezoid Left Top (TLT)
        G4double dz_TLT = (800*cm-400*cm)/2;
        G4VSolid* TLeftTRoomSV = new G4Trd("TLeftTRoomSV",dx_1TL,dx_2TL,dz_TLT,dz_TLT,dy_TL);

        /// TLeft center relative to world center
        G4double dz0_TLeftT = -(800*cm-dz_TLT);
        G4ThreeVector TLeftT_Ps = G4ThreeVector(0,dy0_TLeft,dz0_TLeftT);

        /// SubCylG:  Subtract Cylinder to Outer Trapezoid (Cocnrete)
        G4double SubCyl_RTop = ConcreteShield1_R2;
        G4VSolid* SubCylT_SV = new G4Tubs("SubCylT_SV",0,SubCyl_RTop,SubCyl_Z, 0*deg, 360*deg);

        G4VSolid* TLeftTRoomRestSV = new G4SubtractionSolid("TLeftTRoomRestSV", TLeftTRoomSV,SubCylT_SV,TrdLeft_Rot,SubCyl_Ps);

        /// Subtract both corners of the trapezoid
        G4VSolid* TLeftTRest1SV = new G4SubtractionSolid("TLeftTRest1SV", TLeftTRoomRestSV, AuxBoxSV,0,AuxBox1_Ps);
        G4VSolid* TLeftTRestSV = new G4SubtractionSolid("TLeftTRestSV", TLeftTRest1SV, AuxBoxSV,0,AuxBox2_Ps);

        G4LogicalVolume* TLeftTLV = new  G4LogicalVolume(TLeftTRestSV,fConcrete, "TLeftLV");

        new G4PVPlacement(TLeft_Rot,TLeftT_Ps,TLeftTLV,"TLeftTPV",worldLV,false,0,checkOverlaps);

    // SV Inner Trapezoid Left Top (InTLT)

        G4double dz_InTLT = dz_TLT;
        G4VSolid* InTLeftTSV = new G4Trd("InTLeftTSV",dx_1InTL,dx_2InTL,dz_InTLT,dz_InTLT,dy_InTL);

        // Relative to world center
        G4ThreeVector InTLeftT_Ps = G4ThreeVector(0,0,dy0_InTLeft);

        /// Subtract both corner of the trapezoid
        G4VSolid* InTLeftTRest1SV = new G4SubtractionSolid("InTLeftTRest1SV", InTLeftTSV, AuxBoxSV,0,AuxBox1_Ps);
        G4VSolid* InTLeftTRest2SV = new G4SubtractionSolid("InTLeftTRest2SV", InTLeftTRest1SV, AuxBoxSV,0,AuxBox2_Ps);
        /// SubCylInG:  Subtract Cylinder to Inner Trapezoid (Air)
        G4double SubCylIn_RTop = 656*cm;
        G4VSolid* SubCylInT_SV = new G4Tubs("SubCylInT_SV",0,SubCylIn_RTop,SubCyl_Z, 0*deg, 360*deg);

        G4VSolid* InTLeftTRestSV = new G4SubtractionSolid("InTLeftTRestSV", InTLeftTRest2SV,SubCylInT_SV,TrdLeft_Rot,SubCylIn_Ps);
        G4LogicalVolume* InTLeftTLV = new  G4LogicalVolume(InTLeftTRestSV,fAir, "InTLeftLV");
        new G4PVPlacement(0,InTLeftT_Ps, InTLeftTLV,"InTLeftTPV",TLeftTLV,false,0,checkOverlaps);

    // ----------------------------------------------------------------------------
    //
    // RIGHT SIDE
    //
    // ----------------------------------------------------------------------------

    // SV outer Trapezoid Right Top (TRT)
        /// TRight center relative to world center
        G4ThreeVector TRightT_Ps = G4ThreeVector(0,-dy0_TLeft,dz0_TLeftT);

        /// SubCyl:  Subtract Cylinder to Outer Trapezoid (Cocnrete)
        G4double SubCyl_RTop_R = ConcreteShield1_R2; //ConcreteShield_R2;

        G4VSolid* SubCylTRight_SV = new G4Tubs("SubCylTRight_SV",0,SubCyl_RTop_R,SubCyl_Z, 0*deg, 360*deg);

        G4VSolid* TRightTRestSV = new G4SubtractionSolid("TRightTRestSV", TLeftTRoomSV,SubCylTRight_SV,TrdLeft_Rot,SubCyl_Ps);
        /// Subtract both corners of the trapezoid
        G4VSolid* TRightTRest1SV = new G4SubtractionSolid("TRightTRest1SV", TRightTRestSV, AuxBoxSV,0,AuxBox1_Ps);
        G4VSolid* TRightTRest2SV = new G4SubtractionSolid("TRightTRest2SV", TRightTRest1SV, AuxBoxSV,0,AuxBox2_Ps);

        G4LogicalVolume* TRightTLV = new  G4LogicalVolume(TRightTRest2SV,fConcrete, "TRightBTLV");
        new G4PVPlacement(TRight_Rot,TRightT_Ps,TRightTLV,"TRightTPV",worldLV,false,0,checkOverlaps);

    // SV Inner Trapezoid Right Bottom (InTRT)
        G4LogicalVolume* InTRightTLV = new  G4LogicalVolume(InTLeftTRestSV,fAir, "InTRightLV");
        new G4PVPlacement(0,InTLeftT_Ps,InTRightTLV,"InTRightTPV",TRightTLV,false,0,checkOverlaps);

// ----------------------------------------------------------------------------
//
// BOTTOM LEVEL
//
// ----------------------------------------------------------------------------
    // ----------------------------------------------------------------------------
    //
    // LEFT SIDE
    //
    // ----------------------------------------------------------------------------

      // SV outer Trapezoid Left Bottom (TLB)
        G4double dz_TLB = (550*cm-250*cm)/2;
        G4VSolid* TLeftBRoomSV = new G4Trd("TLeftBRoomSV",dx_1TL,dx_2TL,dz_TLB,dz_TLB,dy_TL);

        /// TLeft center relative to world center
        G4double dz0_TLeftB = 250*cm+dz_TLB;
        G4ThreeVector TLeftB_Ps = G4ThreeVector(0,dy0_TLeft,dz0_TLeftB);

        /// SubCylG:  Subtract Cylinder to Outer Trapezoid (Cocnrete)
        G4double SubCyl_RBottom = ConcreteShield1_R2;
        G4VSolid* SubCylB_SV = new G4Tubs("SubCylB_SV",0,SubCyl_RBottom,SubCyl_Z, 0*deg, 360*deg);

        G4VSolid* TLeftBRoomRestSV = new G4SubtractionSolid("TLeftBRoomRestSV", TLeftBRoomSV,SubCylB_SV,TrdLeft_Rot,SubCyl_Ps);

        /// Subtract both corners of the trapezoid
        G4VSolid* TLeftBRest1SV = new G4SubtractionSolid("TLeftBRest1SV", TLeftBRoomRestSV, AuxBoxSV,0,AuxBox1_Ps);
        G4VSolid* TLeftBRestSV = new G4SubtractionSolid("TLeftBRestSV", TLeftBRest1SV, AuxBoxSV,0,AuxBox2_Ps);

        G4LogicalVolume* TLeftBLV = new  G4LogicalVolume(TLeftBRestSV,fConcrete, "TLeftLV");
        new G4PVPlacement(TLeft_Rot,TLeftB_Ps,TLeftBLV,"TLeftBPV",worldLV,false,0,checkOverlaps);

      // SV Inner Trapezoid Left Bottom (InTLB)
        G4double dz_InTLB = dz_TLB;
        G4VSolid* InTLeftBSV = new G4Trd("InTLeftBSV",dx_1InTL,dx_2InTL,dz_InTLB,dz_InTLB,dy_InTL);

        // Relative to world center
        G4ThreeVector InTLeftB_Ps = G4ThreeVector(0,0,dy0_InTLeft);

        /// Subtract both corner of the trapezoid
        G4VSolid* InTLeftBRest1SV = new G4SubtractionSolid("InTLeftBRest1SV", InTLeftBSV, AuxBoxSV,0,AuxBox1_Ps);
        G4VSolid* InTLeftBRest2SV = new G4SubtractionSolid("InTLeftBRest2SV", InTLeftBRest1SV, AuxBoxSV,0,AuxBox2_Ps);

        /// SubCylInG:  Subtract Cylinder to Inner Trapezoid (Air)
        G4double SubCylIn_RBottom = SubCylIn_RTop;
        G4VSolid* SubCylInB_SV = new G4Tubs("SubCylInB_SV",0,SubCylIn_RBottom,SubCyl_Z, 0*deg, 360*deg);

        G4VSolid* InTLeftBRestSV = new G4SubtractionSolid("InTLeftBRestSV", InTLeftBRest2SV,SubCylInB_SV,TrdLeft_Rot,SubCylIn_Ps);
        G4LogicalVolume* InTLeftBLV = new  G4LogicalVolume(InTLeftBRestSV,fAir, "InTLeftLV");
        new G4PVPlacement(0,InTLeftB_Ps,InTLeftBLV,"InTLeftBPV",TLeftBLV,false,0,checkOverlaps);

    // ----------------------------------------------------------------------------
    //
    // RIGHT SIDE
    //
    // ----------------------------------------------------------------------------

    // SV Outer Trapezoid Right Bottom (TRB)
        /// TRight center relative to world center
        G4ThreeVector TRightB_Ps = G4ThreeVector(0,-dy0_TLeft,dz0_TLeftB);

        G4LogicalVolume* TRightBLV = new  G4LogicalVolume(TLeftBRestSV,fConcrete, "TRightBTLV");
        new G4PVPlacement(TRight_Rot,TRightB_Ps,TRightBLV,"TRightBPV",worldLV,false,0,checkOverlaps);

    // SV Inner Trapezoid Right Bottom (InTRB)
        G4LogicalVolume* InTRightBLV = new  G4LogicalVolume(InTLeftBRestSV,fAir, "InTRightLV");
        new G4PVPlacement(0,InTLeftB_Ps,InTRightBLV,"InTRightBPV",TRightBLV,false,0,checkOverlaps);


// ----------------------------------------------------------------------------
//
// HALL. Possible detector location | 3.6m from target
//
// ----------------------------------------------------------------------------

//Maria
// Hall walls: HWall_1 and HWall_2

	G4double hx_HWall_1 = 360*cm/2 ;
	G4double hy_HWall_1 = 620*cm/2;
	G4double hz_HWall_1 = 340*cm/2;
	G4Box* HallWallSV_1 = new G4Box("HallWallSV_1",hx_HWall_1,hy_HWall_1,hz_HWall_1);

	G4LogicalVolume* HallWallLV_1 = new G4LogicalVolume(HallWallSV_1,fConcrete,"HallWallLV_1");

	G4double hx_HWall_2 = 580*cm/2 ;
	G4double hy_HWall_2 = 360*cm/2 ; //330*cm/2; Maria
	G4double hz_HWall_2 = 340*cm/2;
	G4Box* HallWallSV_2 = new G4Box("HallWallSV_2",hx_HWall_2,hy_HWall_2,hz_HWall_2);
	G4LogicalVolume* HallWallLV_2 = new G4LogicalVolume(HallWallSV_2,fConcrete,"HallWallLV_2");

	G4double d0x_HWall_1 = -2300*cm - hx_HWall_1; //cambio el signo
  G4double d0y_HWall_1 = -360*cm - hy_HWall_2*2 - hy_HWall_1;
  G4double d0z_HWall_1 = -55*cm;

	G4double d0x_HWall_2 = -2300*cm - hx_HWall_2; //cambio el signo
	G4double d0y_HWall_2 = -360*cm - hy_HWall_2;
	G4double d0z_HWall_2 = -55*cm;

  G4ThreeVector HWallTranscoord_1(d0x_HWall_1,d0y_HWall_1,d0z_HWall_1);
  G4ThreeVector HWallTranscoord_2(d0x_HWall_2,d0y_HWall_2,d0z_HWall_2);
  new G4PVPlacement(0,HWallTranscoord_1,HallWallLV_1,"HallWallPV_1",OuterBase3_LV,false,0,checkOverlaps);
 	new G4PVPlacement(0,HWallTranscoord_2,HallWallLV_2,"HallWallPV_2",OuterBase3_LV,false,0,checkOverlaps);

//  Hall: Hall_1 and Hall_2, inside HWall_1 and HWall_2
	G4double hx_Hall_1 = 300*cm/2;
	G4double hy_Hall_1 = 590*cm/2;
	G4double hz_Hall_1 = 260*cm/2;
	G4Box* HallSV_1 = new G4Box("HallSV_1",hx_Hall_1,hy_Hall_1,hz_Hall_1);
	G4LogicalVolume* HallLV_1 = new G4LogicalVolume(HallSV_1,fAir,"HallLV_1");

	G4double hx_Hall_2 = 520*cm/2; // 550*cm/2; Maria
	G4double hy_Hall_2 = 300*cm/2;
	G4double hz_Hall_2 = 260*cm/2;
	G4Box* HallSV_2 = new G4Box("HallSV_2",hx_Hall_2,hy_Hall_2,hz_Hall_2);
	G4LogicalVolume* HallLV_2 = new G4LogicalVolume(HallSV_2,fAir,"HallLV_2");

  //air union
  G4double hy_Hall_3 = 15*cm;
  G4Box* HallSV_3 = new G4Box("HallSV_3",hx_Hall_1,hy_Hall_3,hz_Hall_2);
  G4LogicalVolume* HallLV_3 = new G4LogicalVolume(HallSV_3,fAir,"HallLV_2");

	G4double d0x_Hall_1 = 0*cm;
	G4double d0y_Hall_1 = 15*cm;
	G4double d0z_Hall_1 = -10*cm;

	G4double d0x_Hall_2 = 0; //15*cm;
	G4double d0y_Hall_2 = 0; //-15*cm;
	G4double d0z_Hall_2 = -10*cm;

  G4double d0x_Hall_3 = hx_Hall_2 -hx_Hall_1;
  G4double d0y_Hall_3 = -(hy_HWall_2 - 15*cm);

	G4ThreeVector HallTranscoord_1(d0x_Hall_1,d0y_Hall_1,d0z_Hall_1);
	G4ThreeVector HallTranscoord_2(d0x_Hall_2,d0y_Hall_2,d0z_Hall_2);
  G4ThreeVector HallTranscoord_3(d0x_Hall_3,d0y_Hall_3,d0z_Hall_2);

  new G4PVPlacement(0,HallTranscoord_1,HallLV_1,"HallPV_1",HallWallLV_1,false,0,checkOverlaps);
  new G4PVPlacement(0,HallTranscoord_2,HallLV_2,"HallPV_2",HallWallLV_2,false,0,checkOverlaps);
  new G4PVPlacement(0,HallTranscoord_3,HallLV_3,"HallPV_3",HallWallLV_2,false,0,checkOverlaps);


  // return the world physical volume ----------------------------------------
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
