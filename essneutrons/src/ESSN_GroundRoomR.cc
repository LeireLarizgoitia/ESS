#include "ESSN_GroundRoomR.hh"

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

ESSN_GroundRoomR::ESSN_GroundRoomR()
{}

ESSN_GroundRoomR::~ESSN_GroundRoomR()
{}

void ESSN_GroundRoomR::Construct()
{
  G4Material* fConcrete = G4Material::GetMaterial("G4_CONCRETE"); //();
  G4Material* fAir = G4Material::GetMaterial("G4_AIR");
  G4Material* fSS316L = G4Material::GetMaterial("fSS316L");
  G4Material* fVacuum = G4Material::GetMaterial("G4_Galactic");
  G4Material* fMagnadense =  G4Material::GetMaterial("fMagnadense");
  G4Material* fCarbonSteel =  G4Material::GetMaterial("fCarbonSteel");

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

  G4RotationMatrix* TrdLeftReflect_Rot = new G4RotationMatrix();
  TrdLeftReflect_Rot->rotateX(-90.*deg);
  TrdLeftReflect_Rot->rotateY(180.*deg);

  G4double AirCurve_RMin = 550*cm;
  G4double AirCurve_RMax = 556*cm;
  G4double AirCurveR_Z = (206*cm+76*cm)/2;
  G4double dz_AirCurveR = 76*cm-AirCurveR_Z;
  G4VSolid* AirCurveR_SV = new G4Tubs("AirCurveR_SV",AirCurve_RMin,AirCurve_RMax,AirCurveR_Z,-120*deg,60*deg);

  G4LogicalVolume* AirCurveR_LV = new  G4LogicalVolume(AirCurveR_SV,fAir, "AirCurve_LV");

  // SV Outer Trapezoid Right Ground (TRG)
  G4VSolid* TRightRoom1_SV = new G4Trd("TRightRoom1_SV",dx_1TL,dx_2TL,dz_TR1,dz_TR1,dy_TL);

  /// SubCylG:  Subtract Cylinder
  G4VSolid* SubCylGRight1_SV = new G4Tubs("SubCylGRight1_SV",0,AirCurve_RMax,SubCyl_Z, 0*deg, 360*deg);
  G4VSolid* TRight1RoomRest_SV = new G4SubtractionSolid("TRight1RoomRest_SV", TRightRoom1_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCyl_Ps);

  /// Subtract both corners of the trapezoid
  G4VSolid* TRight1Rest1_SV = new G4SubtractionSolid("TRight1Rest1_SV", TRight1RoomRest_SV, AuxBoxSV,0,AuxBox1_Ps);
  G4VSolid* TRight1Rest_SV = new G4SubtractionSolid("TRight1Rest_SV", TRight1Rest1_SV, AuxBoxSV,0,AuxBox2_Ps);

  G4LogicalVolume* TRightGLV = new  G4LogicalVolume(TRight1Rest_SV,fConcrete, "TRightG_LV");
  this->SetLogicalVolume(TRightGLV);

// SV INNER Mn8 Right Ground

  G4double hx_InTR1_1 = 500*cm;
  G4double hy_InTR1_1 = (2200*cm-1225*cm)/2;
  G4double hz_InTR1_1 = (300*cm+250*cm)/2;
  G4Box* InTRight1_SV = new G4Box("InTRight1_SV",
            hx_InTR1_1,hy_InTR1_1,hz_InTR1_1);

  G4double dx_1InTR1_1 = hx_InTR1_1;
  G4double dx_2InTR1_1 = 2*cm/2;
  G4double dz_InTR1_1 = hz_InTR1_1;
  G4double dy_InTR1small = sqrt(3)*dx_2InTR1_1;
  G4double dy_InTR1_1 =  (sqrt(3))*(dx_1InTR1_1-dy_InTR1small)/2 ;

  G4VSolid* InTRight2_SV = new G4Trd("InTRight2_SV",dx_1InTR1_1,dx_2InTR1_1,dz_InTR1_1,dz_InTR1_1,dy_InTR1_1);


  // Relative to world center
  G4double dz0_InTR11 = 250*cm-hz_InTR1_1;
  G4double dy0_InTR11 = 2200*cm-hy_InTR1_1;

  G4double dz0_InTRight1 = dz0_TRight1 - dz0_InTR11;
  G4double dy0_InTRight1 = dy0_TLeft - dy0_InTR11;
  G4ThreeVector InTRight1_Ps = G4ThreeVector(0,dz0_InTRight1,dy0_InTRight1);

  G4double dy0_InTR12 = 1225*cm-dy_InTR1_1;
  G4double dy0_InTRight2= dy0_TLeft - dy0_InTR12;

  G4ThreeVector InTRight2_Ps = G4ThreeVector(0,dz0_InTRight1,dy0_InTRight2);

  G4LogicalVolume* InTRight1_LV = new  G4LogicalVolume(InTRight1_SV,fMagnadense, "InTRightM8_LV");

  /// SubCylInG:  Subtract Cylinder
  G4double SubCylInRight_RGround = 656*cm;
  G4VSolid* SubCylInRightG_SV = new G4Tubs("SubCylInRightG_SV",0,SubCylInRight_RGround,SubCyl_Z, 0*deg, 360*deg);
  G4ThreeVector SubCylInRight_Ps(0,0,dy0_InTR12);

  G4VSolid* InTRightRest12SV = new G4SubtractionSolid("InTRightRest12SV", InTRight2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylInRight_Ps);
  G4LogicalVolume* InTRight2_LV = new  G4LogicalVolume(InTRightRest12SV,fMagnadense, "InTRightM8_LV");

  new G4PVPlacement(TrdLeft_Rot,InTRight1_Ps,InTRight1_LV,"InTRight1_PV",TRightGLV,false,0,1);

  new G4PVPlacement(0,InTRight2_Ps,InTRight2_LV,"InTRight2_PV",TRightGLV,false,0,1);

// Relative to Rectangular Mn8 block - InTRight1 , InTRight2 -

// AIR GAP inside Ground Right Rooms (Beam tube goes inside)
  //
  G4double hx_AirGap_1 = 100*cm;
  G4double hy_AirGap_1 = hy_InTR1_1;
  G4double hz_AirGap_1 = (150*cm+70*cm)/2;
  G4Box* TRightAirGap1_SV = new G4Box("TRightAirGap1_SV",
            hx_AirGap_1,hy_AirGap_1,hz_AirGap_1);

  G4double dz0_AirGap_1 = 150*cm-hz_AirGap_1;

  G4double dz0_AirGap1 = dz0_AirGap_1 - dz0_InTR11;
  G4ThreeVector TRightAirGap1_Ps = G4ThreeVector(0,0,dz0_AirGap1);

  G4LogicalVolume* TRightAirGap1_LV = new  G4LogicalVolume(TRightAirGap1_SV,fAir, "TRightAirGap_LV");

  new G4PVPlacement(0,TRightAirGap1_Ps,TRightAirGap1_LV,"TRightAirGap1_PV",InTRight1_LV,false,0,1);

  G4double hx_AirGap_2 = 100*cm;
  G4double hy_AirGap_2 = 200*cm/2;
  G4double hz_AirGap_2 = (170*cm-70*cm)/2;
  G4Box* TRightAirGap2_SV = new G4Box("TRightAirGap2_SV",
            hx_AirGap_2,hy_AirGap_2,hz_AirGap_2);

  G4double dz0_AirGap_2 = -(70*cm+hz_AirGap_2);
  G4double dz0_AirGap2 = dz0_AirGap_2-dz0_InTR11;

  G4double dy0_AirGap_2 = (2200*cm-hy_AirGap_2) - dy0_InTR11;

  G4ThreeVector TRightAirGap2_Ps = G4ThreeVector(0,-dy0_AirGap_2,dz0_AirGap2);

  G4LogicalVolume* TRightAirGap2_LV = new  G4LogicalVolume(TRightAirGap2_SV,fAir, "TRightAirGap_LV");

  new G4PVPlacement(0,TRightAirGap2_Ps,TRightAirGap2_LV,"TRightAirGap2_PV",InTRight1_LV,false,0,1);

  //
  G4double hy_CurvedGap = 1225*cm/2;
  G4Box* TRightAirGap12_SV = new G4Box("TRightAirGap12_SV",
                                       hx_AirGap_1,hy_CurvedGap,hz_AirGap_1);

  G4double dy0_CurvedGap = hy_CurvedGap - dy0_InTR12;
  G4ThreeVector TRightAirGap12_Ps = G4ThreeVector(0,-dz0_AirGap1,- dy0_CurvedGap);

  G4ThreeVector SubCylGap_Ps(0,hy_CurvedGap,0);
  G4VSolid* TRightAirGap12Rest_SV = new G4SubtractionSolid("TRightAirGap12_SV", TRightAirGap12_SV,SubCylInRightG_SV,0,SubCylGap_Ps);

  G4LogicalVolume* TRightAirGap12_LV = new  G4LogicalVolume(TRightAirGap12Rest_SV,fAir, "TRightAirGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightAirGap12_Ps,TRightAirGap12_LV,"TRightAirGap12_PV",InTRight2_LV,false,0,1);

  //
  G4double h_AirGap_13 = 50*cm;
  G4Box* TRightAirGap13_SV = new G4Box("TRightAirGap13_SV",
                                       h_AirGap_13,hy_CurvedGap,h_AirGap_13);

  G4double dy0_CurvedBorder = hy_CurvedGap - dy0_TLeft;
  G4ThreeVector TRightAirGap13_Ps = G4ThreeVector(0,dz0_TRight1,-dy0_CurvedBorder);

  G4VSolid* TRightAirGap13Rest1_SV = new G4IntersectionSolid("TRightAirGap13Rest1_SV", TRightAirGap13_SV,SubCylInRightG_SV,0,SubCylGap_Ps);
  G4VSolid* TRightAirGap13Rest_SV = new G4SubtractionSolid("TRightAirGap13Rest_SV", TRightAirGap13Rest1_SV,SubCylGRight1_SV,0,SubCylGap_Ps);

  G4LogicalVolume* TRightAirGap13_LV = new  G4LogicalVolume(TRightAirGap13Rest_SV,fAir, "TRightAirGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightAirGap13_Ps,TRightAirGap13_LV,"TRightAirGap13_PV",TRightGLV,false,0,1);

  // CARBON STELL GAP inside Ground Right Rooms
  //
  G4double hx_CarbonGap_1 = 130*cm;
  G4double hy_CarbonGap_1 = (2000*cm-1225*cm)/2;
  G4double hz_CarbonGap_1 = hz_AirGap_2;
  G4Box* TRightCarbonGap1_SV = new G4Box("TRightCarbonGap1_SV",
            hx_CarbonGap_1,hy_CarbonGap_1,hz_CarbonGap_1);

  G4double dz0_CarbonGap1 = dz0_AirGap2;

  G4double dy0_CarbonGap_1 = (2000*cm-hy_CarbonGap_1) - dy0_InTR11;

  G4ThreeVector TRightCarbonGap1_Ps = G4ThreeVector(0,-dy0_CarbonGap_1,dz0_CarbonGap1);

  G4LogicalVolume* TRightCarbonGap1_LV = new  G4LogicalVolume(TRightCarbonGap1_SV,fCarbonSteel, "TRightCarbonGap_LV");

  new G4PVPlacement(0,TRightCarbonGap1_Ps,TRightCarbonGap1_LV,"TRightCarbonGap1_PV",InTRight1_LV,false,0,1);

  G4double hx_CarbonGap_2 = 150*cm;
  G4double hy_CarbonGap_2 = (2020*cm-1225*cm)/2;
  G4double hz_CarbonGap_2 = (250*cm-170*cm)/2;
  G4Box* TRightCarbonGap2_SV = new G4Box("TRightCarbonGap2_SV",
            hx_CarbonGap_2,hy_CarbonGap_2,hz_CarbonGap_2);

  G4double dz0_CarbonGap2 = -(170*cm+hz_CarbonGap_2);
  G4double dz0_CarbonGap_2 = dz0_CarbonGap2 - dz0_InTR11;
  G4double dy0_CarbonGap_2 = (2020*cm-hy_CarbonGap_2) - dy0_InTR11;

  G4ThreeVector TRightCarbonGap2_Ps = G4ThreeVector(0,-dy0_CarbonGap_2,dz0_CarbonGap_2);

  G4LogicalVolume* TRightCarbonGap2_LV = new  G4LogicalVolume(TRightCarbonGap2_SV,fCarbonSteel, "TRightCarbonGap_LV");

  new G4PVPlacement(0,TRightCarbonGap2_Ps,TRightCarbonGap2_LV,"TRightCarbonGap2_PV",InTRight1_LV,false,0,1);
  //
  G4Box* TRightCarbonGap12_SV = new G4Box("TRightCarbonGap12_SV",
            hx_CarbonGap_1,hy_CurvedGap,hz_CarbonGap_1);

  G4ThreeVector TRightCarbonGap12_Ps = G4ThreeVector(0,-dz0_CarbonGap1,-dy0_CurvedGap );

  G4VSolid* TRightCarbonGap12Rest_SV = new G4SubtractionSolid("TRightCarbonGap12Rest_SV", TRightCarbonGap12_SV,SubCylInRightG_SV,0,SubCylGap_Ps);

  G4LogicalVolume* TRightCarbonGap12_LV = new  G4LogicalVolume(TRightCarbonGap12Rest_SV,fCarbonSteel, "TRightCarbonGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightCarbonGap12_Ps,TRightCarbonGap12_LV,"TRightCarbonGap12_PV",InTRight2_LV,false,0,1);

  G4Box* TRightCarbonGap22_SV = new G4Box("TRightCarbonGap22_SV",
            hx_CarbonGap_2,hy_CurvedGap,hz_CarbonGap_2);

  G4ThreeVector TRightCarbonGap22_Ps = G4ThreeVector(0,-dz0_CarbonGap_2,-dy0_CurvedGap );

  G4VSolid* TRightCarbonGap22Rest_SV = new G4SubtractionSolid("TRightCarbonGap22Rest_SV", TRightCarbonGap22_SV,SubCylInRightG_SV,0,SubCylGap_Ps);

  G4LogicalVolume* TRightCarbonGap22_LV = new  G4LogicalVolume(TRightCarbonGap22Rest_SV,fCarbonSteel, "TRightCarbonGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightCarbonGap22_Ps,TRightCarbonGap22_LV,"TRightCarbonGap22_PV",InTRight2_LV,false,0,1);

  //

  G4double dz0_CarbonCurvedBorder1 = dz0_AirGap_2 - dz0_TRight1;
  G4ThreeVector TRightCarbonGap13_Ps = G4ThreeVector(0,-dz0_CarbonCurvedBorder1,-dy0_CurvedBorder);

  G4VSolid* TRightCarbonGap13Rest1_SV = new G4IntersectionSolid("TRightCarbonGap13Rest1_SV", TRightCarbonGap12_SV,SubCylInRightG_SV,0,SubCylGap_Ps);
  G4VSolid* TRightCarbonGap13Rest_SV = new G4SubtractionSolid("TRightCarbonGap13Rest_SV", TRightCarbonGap13Rest1_SV,SubCylGRight1_SV,0,SubCylGap_Ps);

  G4LogicalVolume* TRightCarbonGap13_LV = new  G4LogicalVolume(TRightCarbonGap13Rest_SV,fCarbonSteel, "TRightCarbonGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightCarbonGap13_Ps,TRightCarbonGap13_LV,"TRightCarbonGap13_PV",TRightGLV,false,0,1);

  G4double hz_CarbonGap_23 = (206*cm-170*cm)/2;
  G4double dz0_CarbonGap23 = -(170*cm+hz_CarbonGap_23);
  G4Box* TRightCarbonGap23_SV = new G4Box("TRightCarbonGap23_SV",
                                          hx_CarbonGap_2,hy_CurvedGap,hz_CarbonGap_23);

  G4double dz0_CarbonCurvedBorder23 = dz0_CarbonGap23 - dz0_TRight1;
  G4ThreeVector TRightCarbonGap23_Ps = G4ThreeVector(0,-dz0_CarbonCurvedBorder23,-dy0_CurvedBorder);

  G4VSolid* TRightCarbonGap23Rest1_SV = new G4IntersectionSolid("TRightCarbonGap23Rest1_SV", TRightCarbonGap23_SV,SubCylInRightG_SV,0,SubCylGap_Ps);
  G4VSolid* TRightCarbonGap23Rest_SV = new G4SubtractionSolid("TRightCarbonGap23Rest_SV", TRightCarbonGap23Rest1_SV,SubCylGRight1_SV,0,SubCylGap_Ps);

  G4LogicalVolume* TRightCarbonGap23_LV = new  G4LogicalVolume(TRightCarbonGap23Rest_SV,fCarbonSteel, "TRightCarbonGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightCarbonGap23_Ps,TRightCarbonGap23_LV,"TRightCarbonGap23_PV",TRightGLV,false,0,1);

// CONCRETE GAP inside Ground Right Rooms

  G4double hx_ConcGap_1 = 170*cm;
  G4double hy_ConcGap_1 = (2040*cm-1225*cm)/2;
  G4double hz_ConcGap_1 = (300*cm-250*cm)/2;
  G4Box* TRightConcGap1_SV = new G4Box("TRightConcGap1_SV",
            hx_ConcGap_1,hy_ConcGap_1,hz_ConcGap_1);

  G4double dz0_ConcGap1 = -(250*cm+hz_ConcGap_1);
  G4double dz0_ConcGap_1 = dz0_ConcGap1 - dz0_InTR11;

  G4double dy0_ConcGap_1 = (2040*cm-hy_ConcGap_1) - dy0_InTR11;

  G4ThreeVector TRightConcGap1_Ps = G4ThreeVector(0,-dy0_ConcGap_1,dz0_ConcGap_1);

  G4LogicalVolume* TRightConcGap1_LV = new  G4LogicalVolume(TRightConcGap1_SV,fConcrete, "TRightConcGap_LV");

  new G4PVPlacement(0,TRightConcGap1_Ps,TRightConcGap1_LV,"TRightConcGap1_PV",InTRight1_LV,false,0,1);

  G4double hy_ConcGap_2 = (1225*cm-736*cm)/2;
  G4Box* TRightConcGap2_SV = new G4Box("TRightConcGap2_SV",
            hx_ConcGap_1,hy_ConcGap_2,hz_ConcGap_1);

  G4double dy0_ConcGap_2 = (1225*cm-hy_ConcGap_2) - dy0_InTR12;

  G4ThreeVector TRightConcGap2_Ps = G4ThreeVector(0,-dz0_ConcGap_1,-dy0_ConcGap_2);

  G4LogicalVolume* TRightConcGap2_LV = new  G4LogicalVolume(TRightConcGap2_SV,fConcrete, "TRightConcGap_LV");

  new G4PVPlacement(TrdLeft_Rot,TRightConcGap2_Ps,TRightConcGap2_LV,"TRightConcGap2_PV",InTRight2_LV,false,0,1);



// ----------------------------------------------------------------------------
//
// TRIANGLE ROOMS
//
// ----------------------------------------------------------------------------

    G4double hx_TAir = 490*cm; //874*cm;
    G4double hy_TAir = (2100*cm-1225*cm); //148.9*cm + 355.7*cm;
    G4double hz_TAir = dz_InTL*2;
    G4double hLTX_TAir = 0.001*mm;
    G4Trap* TAirSV = new G4Trap("TAirSV",
                  hz_TAir,hy_TAir,hx_TAir,hLTX_TAir);

    G4double dx_TAir = (hx_TAir-hLTX_TAir)/4;
    G4double px0_TAir = -dx_TAir+hx_TAir/2;

    G4double dy0_TAir= 2100*cm-hy_TAir/2;
    G4double dy0_TRoomAir = dy0_TLeft - dy0_TAir;
    G4double dx0_TAir = 500*cm + px0_TAir;
    G4ThreeVector TAir_Ps(dx0_TAir,-dz0_InTLeft,dy0_TRoomAir);

    G4double dx0_TAirLeft = -500*cm - px0_TAir;
    G4ThreeVector TAirLeft_Ps(dx0_TAirLeft,-dz0_InTLeft,dy0_TRoomAir);

    G4LogicalVolume* TAirLV = new  G4LogicalVolume(TAirSV,fAir, "TAirLV");

    ///Triangle room: Air zone
    new G4PVPlacement(TrdLeft_Rot,TAir_Ps,TAirLV,"TAirPV",TRightGLV,false,0,1);
    new G4PVPlacement(TrdLeftReflect_Rot,TAirLeft_Ps,TAirLV,"TAirPV",TRightGLV,false,0,1);

// ----------------------------------------------------------------------------
//
// BEAM TUBE
//
// ----------------------------------------------------------------------------

    G4double BeamTube_R1 = 23*cm;
    G4double BeamTube_R2 = 25*cm;
    G4double BeamTube_R3 = 49*cm;
    // we will cut the tube in different parts according to the corresponding motherLV
    // from outside to inside (approximating to target)
    G4double BeamTube_Z1 = hy_InTR1_1;
    G4double BeamTube_Z2 = hy_CurvedGap;

    G4VSolid* BeamTubeVac1_SV = new G4Tubs("BeamTubeVac1_SV",0,BeamTube_R1,BeamTube_Z1, 0*deg, 360*deg);
    G4VSolid* BeamTubeSS1_SV = new G4Tubs("BeamTubeSS1_SV",BeamTube_R1,BeamTube_R2,BeamTube_Z1, 0*deg, 360*deg);
    G4VSolid* BeamTubeAir1_SV = new G4Tubs("BeamTubeAir1_SV",0,BeamTube_R3,BeamTube_Z1, 0*deg, 360*deg);

    G4ThreeVector BeamTube1_Ps = G4ThreeVector(0,0,-dz0_AirGap_1);

    G4LogicalVolume* BeamTubeVac1_LV = new  G4LogicalVolume(BeamTubeVac1_SV,fVacuum, "BeamTubeVac_LV");
    G4LogicalVolume* BeamTubeSS1_LV = new  G4LogicalVolume(BeamTubeSS1_SV,fSS316L, "BeamTubeSS_LV");
    G4LogicalVolume* BeamTubeAir1_LV = new  G4LogicalVolume(BeamTubeAir1_SV,fAir, "BeamTubeAir_LV");


    new G4PVPlacement(TrdLeft_Rot,BeamTube1_Ps,BeamTubeAir1_LV,"BeamTube1_PV",TRightAirGap1_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeSS1_LV,"BeamTubeSS1_PV",BeamTubeAir1_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeVac1_LV,"BeamTubeVac1_PV",BeamTubeAir1_LV,false,0,1);

    //
    G4VSolid* BeamTubeVac2_SV = new G4Tubs("BeamTubeVac2_SV",0,BeamTube_R1,BeamTube_Z2, 0*deg, 360*deg);
    G4VSolid* BeamTubeSS2_SV = new G4Tubs("BeamTubeSS2_SV",0,BeamTube_R2,BeamTube_Z2, 0*deg, 360*deg);
    G4VSolid* BeamTubeAir2_SV = new G4Tubs("BeamTubeAir2_SV",0,BeamTube_R3,BeamTube_Z2, 0*deg, 360*deg);

    G4ThreeVector SubCylBeamTube_Ps(0,0,-BeamTube_Z2);
    G4VSolid* BeamTubeAir12_SV = new G4SubtractionSolid("BeamTubeAir12_SV", BeamTubeAir2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylBeamTube_Ps);
    G4VSolid* BeamTubeSS12_SV = new G4SubtractionSolid("BeamTubeSS12_SV", BeamTubeSS2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylBeamTube_Ps);
    G4VSolid* BeamTubeVac12_SV = new G4SubtractionSolid("BeamTubeVac12_SV", BeamTubeVac2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylBeamTube_Ps);

    G4LogicalVolume* BeamTubeAir12_LV = new  G4LogicalVolume(BeamTubeAir12_SV,fAir, "BeamTubeAir_LV");
    G4LogicalVolume* BeamTubeSS12_LV = new  G4LogicalVolume(BeamTubeSS12_SV,fAir, "BeamTubeSS_LV");
    G4LogicalVolume* BeamTubeVac12_LV = new  G4LogicalVolume(BeamTubeVac12_SV,fAir, "BeamTubeVac_LV");


    new G4PVPlacement(TrdLeft_Rot,BeamTube1_Ps,BeamTubeAir12_LV,"BeamTube12_PV",TRightAirGap12_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeSS12_LV,"BeamTubeSS12_PV",BeamTubeAir12_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeVac12_LV,"BeamTubeVac12_PV",BeamTubeSS12_LV,false,0,1);

    //
    G4VSolid* BeamTubeAir21_SV = new G4IntersectionSolid("BeamTubeAir21_SV", BeamTubeAir2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylBeamTube_Ps);
    G4VSolid* BeamTubeAir22_SV = new G4SubtractionSolid("BeamTubeAir22_SV", BeamTubeAir21_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCylBeamTube_Ps);

    G4VSolid* BeamTubeSS21_SV = new G4IntersectionSolid("BeamTubeSS21_SV", BeamTubeSS2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylBeamTube_Ps);
    G4VSolid* BeamTubeSS22_SV = new G4SubtractionSolid("BeamTubeSS22_SV", BeamTubeSS21_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCylBeamTube_Ps);

    G4VSolid* BeamTubeVac21_SV = new G4IntersectionSolid("BeamTubeVac21_SV", BeamTubeVac2_SV,SubCylInRightG_SV,TrdLeft_Rot,SubCylBeamTube_Ps);
    G4VSolid* BeamTubeVac22_SV = new G4SubtractionSolid("BeamTubeVac22_SV", BeamTubeVac21_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCylBeamTube_Ps);

    G4LogicalVolume* BeamTubeAir22_LV = new  G4LogicalVolume(BeamTubeAir22_SV,fAir, "BeamTubeAir_LV");
    G4LogicalVolume* BeamTubeSS22_LV = new  G4LogicalVolume(BeamTubeSS22_SV,fAir, "BeamTubeSS_LV");
    G4LogicalVolume* BeamTubeVac22_LV = new  G4LogicalVolume(BeamTubeVac22_SV,fAir, "BeamTubeVac_LV");

    new G4PVPlacement(TrdLeft_Rot,G4ThreeVector(),BeamTubeAir22_LV,"BeamTube22_PV",TRightAirGap13_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeSS22_LV,"BeamTubeSS22_PV",BeamTubeAir22_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeVac22_LV,"BeamTubeVac22_PV",BeamTubeSS22_LV,false,0,1);

    //
    G4ThreeVector BeamTubeAir32_Ps(0,-AirCurve_RMax,-dz_AirCurveR);
    G4ThreeVector SubCylBeamTube2_Ps(0,0,-AirCurve_RMax);
    G4VSolid* SubCylAirGap_SV = new G4Tubs("SubCylAirGap_SV",0,AirCurve_RMin,SubCyl_Z,0*deg,360*deg);

    G4VSolid* BeamTubeAir31_SV = new G4IntersectionSolid("BeamTubeAir31_SV", BeamTubeAir2_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCylBeamTube2_Ps);
    G4VSolid* BeamTubeAir32_SV = new G4SubtractionSolid("BeamTubeAir32_SV", BeamTubeAir31_SV,SubCylAirGap_SV,TrdLeft_Rot,SubCylBeamTube2_Ps);

    G4VSolid* BeamTubeSS31_SV = new G4IntersectionSolid("BeamTubeSS31_SV", BeamTubeSS2_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCylBeamTube2_Ps);
    G4VSolid* BeamTubeSS32_SV = new G4SubtractionSolid("BeamTubeSS32_SV", BeamTubeSS31_SV,SubCylAirGap_SV,TrdLeft_Rot,SubCylBeamTube2_Ps);

    G4VSolid* BeamTubeVac31_SV = new G4IntersectionSolid("BeamTubeVac31_SV", BeamTubeVac2_SV,SubCylGRight1_SV,TrdLeft_Rot,SubCylBeamTube2_Ps);
    G4VSolid* BeamTubeVac32_SV = new G4SubtractionSolid("BeamTubeVac32_SV", BeamTubeVac31_SV,SubCylAirGap_SV,TrdLeft_Rot,SubCylBeamTube2_Ps);

    G4LogicalVolume* BeamTubeAir32_LV = new  G4LogicalVolume(BeamTubeAir32_SV,fAir, "BeamTubeAir_LV");
    G4LogicalVolume* BeamTubeSS32_LV = new  G4LogicalVolume(BeamTubeSS32_SV,fAir, "BeamTubeSS_LV");
    G4LogicalVolume* BeamTubeVac32_LV = new  G4LogicalVolume(BeamTubeVac32_SV,fAir, "BeamTubeVac_LV");

    new G4PVPlacement(TrdLeft_Rot,BeamTubeAir32_Ps,BeamTubeAir32_LV,"BeamTube32_PV",AirCurveR_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeSS32_LV,"BeamTubeSS32_PV",BeamTubeAir32_LV,false,0,1);

    new G4PVPlacement(0,G4ThreeVector(),BeamTubeVac32_LV,"BeamTubeVac32_PV",BeamTubeSS32_LV,false,0,1);

}
