#include "ESSN_SSTrShield.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Ellipsoid.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4UnionSolid.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"


ESSN_SSTrShield::ESSN_SSTrShield()
{}

ESSN_SSTrShield::~ESSN_SSTrShield()
{}

void ESSN_SSTrShield::Construct()
{
  G4Material* fSS316L = G4Material::GetMaterial("fSS316L");
  G4Material* fVacuum = G4Material::GetMaterial("G4_Galactic");
  G4Material* fBeryllium = G4Material::GetMaterial("fBeryllium");
  G4Material* fTungsten = G4Material::GetMaterial("fTungsten");

  G4RotationMatrix* TrdLeft_Rot = new G4RotationMatrix();
  TrdLeft_Rot->rotateX(-90.*deg);

  G4VSolid* TrSh1_SV = new G4Tubs("TrSh1_SV",
                                  0,TrSh1_R1, TrSh1_Z, 0, 360*deg);

  // Mother volume
  G4LogicalVolume* TrSh1_LV = new  G4LogicalVolume(TrSh1_SV,fSS316L, "TrSh1_LV");
  this->SetLogicalVolume(TrSh1_LV);


  // Vacuum clad in SS316L shielding
  G4double TrShVac3_Z =  TrSh1_Z;
  G4VSolid* TrShVac3_SV = new G4Tubs("TrShVac3_SV",
                                     TrShVac3_R1, TrShVac2_R1, TrShVac3_Z, 0, 360*deg);

  G4double TrShVac3_dz = (200*cm-TrShVac3_Z);
  G4ThreeVector TrShVac3_Sh1Ps = G4ThreeVector(0,0,TrShVac3_dz-TrSh1_dz);

  G4LogicalVolume* TrShVac3_LV = new  G4LogicalVolume(TrShVac3_SV,fVacuum, "TrShVac_LV");
  new G4PVPlacement(0,TrShVac3_Sh1Ps,TrShVac3_LV,"TrShVac3_PV",TrSh1_LV,false,0,1);

  // ----------------------------------------------------------------------------
  //
  // Beam Tube Vacuum in SS region: BeamTubeVacSS
  //
  // ----------------------------------------------------------------------------

  G4double BeamTubeSS_Z1 = (OTrSh1_R1-TrShVac2_R1)/2 + 1*cm; //to get the cut

  G4VSolid* BeamTubeVacSS1_SV = new G4Tubs("BeamTubeVacSS1_SV",0,BeamTubeSS_R1,BeamTubeSS_Z1, 0*deg, 360*deg);

  G4double BeamTubeSS1_dy  = OTrSh1_R1 - BeamTubeSS_Z1;
  G4ThreeVector BeamTubeVacSS1_Ps = G4ThreeVector(0,-BeamTubeSS1_dy,-TrSh1_dz);
  G4VSolid* SubCylBeamTubeSS1_SV = new G4Tubs("SubCylBeamTubeSS1_SV",0,TrShVac2_R1,2000*cm, 0*deg, 360*deg);
  G4ThreeVector SubCylBeamTubeSS1_Ps(0,0,-BeamTubeSS1_dy);

  /// !!!!!!! REDEFINED in ESSN_OutTrShield !!!! FIX THIS
  G4VSolid* SubCylBeamTubeCS3_SV = new G4Tubs("SubCylBeamTubeCS3_SV",0,OTrSh1_R1,2000*cm, 0*deg, 360*deg);


  G4VSolid* BeamTubeVacSS11_SV = new G4IntersectionSolid("BeamTubeVacSS11_SV", BeamTubeVacSS1_SV,SubCylBeamTubeCS3_SV,TrdLeft_Rot,SubCylBeamTubeSS1_Ps);
  G4VSolid* BeamTubeVacSS12_SV = new G4SubtractionSolid("BeamTubeVacSS12_SV", BeamTubeVacSS11_SV,SubCylBeamTubeSS1_SV,TrdLeft_Rot,SubCylBeamTubeSS1_Ps);

  G4LogicalVolume* BeamTubeVacSS1_LV = new  G4LogicalVolume(BeamTubeVacSS12_SV,fVacuum, "BeamTubeVacSS_LV");

  new G4PVPlacement(TrdLeft_Rot,BeamTubeVacSS1_Ps,BeamTubeVacSS1_LV,"BeamTubeVacSS1_PV",TrSh1_LV,false,0,1);

  G4double Tr510_y = -173*cm;
  G4double hx_BeamTubeVacSS3 = 9*cm;
  G4double hy_BeamTubeVacSS3 = 25*cm;  //to get the cut
  G4double hz_BeamTubeVacSS = 3.5*cm;
  G4Box* BeamTubeVacSS3_SV = new G4Box("BeamTubeVacSS3_SV",
                                       hx_BeamTubeVacSS3,hy_BeamTubeVacSS3,hz_BeamTubeVacSS);

  G4ThreeVector BeamTubeVacSS3_Ps = G4ThreeVector(0,Tr510_y,-TrSh1_dz);

  G4LogicalVolume* BeamTubeVacSS3_LV = new  G4LogicalVolume(BeamTubeVacSS3_SV,fVacuum, "BeamTubeVacSS_LV");

  new G4PVPlacement(0,BeamTubeVacSS3_Ps,BeamTubeVacSS3_LV,"BeamTubeVacSS3_PV",TrSh1_LV,false,0,1);


  // SIDE TRAPEZOIDS

  G4double hx_BeamTubeVacSS2 = 15*cm;
  //G4double BeamTubeVacSS2_d1 = 132.8*cm-Tr_center;
  G4double BeamTubeVacSS2_d2 = -Tr510_y + hy_BeamTubeVacSS3;
  G4double hy_BeamTubeVacSS2 = (TrShVac3_R1 - BeamTubeVacSS2_d2)/2;  //to get the cut
  G4Box* BeamTubeVacSS2_SV = new G4Box("BeamTubeVacSS2_SV",
                                       hx_BeamTubeVacSS2,hy_BeamTubeVacSS2,hz_BeamTubeVacSS);

  G4double BeamTubeSS2_dx = hx_BeamTubeVacSS2-9*cm;
  G4double BeamTubeSS2_dy  = TrShVac3_R1 - hy_BeamTubeVacSS2;
  //G4ThreeVector BeamTubeVacSS2_Ps = G4ThreeVector(0,-BeamTubeSS2_dy,-TrSh1_dz);
  G4ThreeVector BeamTubeVacSS2_Ps = G4ThreeVector(-BeamTubeSS2_dx,-BeamTubeSS2_dy,-TrSh1_dz);

  /// Auxiliary Plane on xy axis

  G4double hTr510 = 20*cm;
  //G4double p30020_dy  = Tr510_y; //-(1./3.278689E-03)*cm + Tr510_y;
  //G4double p30020_dx  = (1/4.590164E-02)*cm; //(this point will be in the plane)
  G4Box* pTr510_SV = new G4Box("pTr510_SV",
                             hTr510,500*cm,hz_BeamTubeVacSS+1*cm);

  G4double p30020_ang = 4.086*deg;
  G4RotationMatrix* p30020_Rot = new G4RotationMatrix();
  p30020_Rot->rotateZ(-p30020_ang);
  G4ThreeVector p30020_Ps = G4ThreeVector(hTr510-BeamTubeSS2_dx,-Tr510_y-BeamTubeSS2_dy,0);
  //G4ThreeVector p30020_Ps = G4ThreeVector(-(hTr510+p30020_dx)-BeamTubeSS2_dx,-Tr510_y-BeamTubeSS2_dy,0);

  G4VSolid* BeamTubeVac_p30020SubSV = new G4IntersectionSolid("SideTrap_SubSV",BeamTubeVacSS2_SV, pTr510_SV, p30020_Rot, p30020_Ps);

  G4VSolid* SubCylBeamTubeSS2_SV = new G4Tubs("SubCylBeamTubeSS21_SV",0,TrShVac3_R1,2000*cm, 0*deg, 360*deg);
  G4ThreeVector SubCylBeamTubeSS2_Ps(BeamTubeSS2_dx,BeamTubeSS2_dy,0);
  G4VSolid* BeamTubeVacSS2_SubSV = new G4IntersectionSolid("BeamTubeVacSS2_SubSV", BeamTubeVac_p30020SubSV,SubCylBeamTubeSS2_SV,0,SubCylBeamTubeSS2_Ps);

  G4LogicalVolume* BeamTubeVacSS2_LV = new  G4LogicalVolume(BeamTubeVacSS2_SubSV,fVacuum, "BeamTubeVacSS_LV");

  new G4PVPlacement(0,BeamTubeVacSS2_Ps,BeamTubeVacSS2_LV,"BeamTubeVacSS2_PV",TrSh1_LV,false,0,1);

  G4double BeamTubeVacSS4_d1 = 132.8*cm - Tr_center;
  G4double BeamTubeVacSS4_d2 = -Tr510_y - hy_BeamTubeVacSS3;
  G4double hy_BeamTubeVacSS4 = (BeamTubeVacSS4_d1 + BeamTubeVacSS4_d2 )/2;  //to get the cut
  G4Box* BeamTubeVacSS4_SV = new G4Box("BeamTubeVacSS4_SV",
                                       hx_BeamTubeVacSS2,hy_BeamTubeVacSS4,hz_BeamTubeVacSS);

  G4double BeamTubeSS4_dy  = BeamTubeVacSS4_d1 - hy_BeamTubeVacSS4;
  G4double BeamTubeSS4_dx = hx_BeamTubeVacSS2-9*cm;
  G4ThreeVector BeamTubeVacSS4_Ps = G4ThreeVector(BeamTubeSS4_dx,BeamTubeSS4_dy,-TrSh1_dz);

  //G4double p30019_dy  = Tr510_y; // (1./2.328590E-03)*cm + Tr510_y;
  //G4double p30019_dx  = -(1/4.708926E-02)*cm;
  G4double p30019_ang = 2.831*deg;
  G4RotationMatrix* p30019_Rot = new G4RotationMatrix();
  p30019_Rot->rotateZ(-p30019_ang);
  G4ThreeVector p30019_Ps = G4ThreeVector(-hTr510-BeamTubeSS4_dx, -Tr510_y-BeamTubeSS4_dy,0);
  //G4ThreeVector p30019_Ps = G4ThreeVector(-p30019_dx-hTr510-BeamTubeSS4_dx, -p30019_dy-BeamTubeSS4_dy,0);

  G4VSolid* BeamTubeVac_p30019SubSV = new G4IntersectionSolid("SideTrap_SubSV",BeamTubeVacSS4_SV, pTr510_SV, p30019_Rot, p30019_Ps);

  G4VSolid* SubCylBeamTubeSS4_SV = new G4Tubs("SubCylBeamTubeSS22_SV",0,132.8*cm,2000*cm, 0*deg, 360*deg);
  G4double BeamTubeSS41_dy  = Tr_center - BeamTubeSS4_dy;
  G4ThreeVector SubCylBeamTubeSS4_Ps(-BeamTubeSS4_dx,BeamTubeSS41_dy,0);
  G4VSolid* BeamTubeVacSS4_SubSV = new G4SubtractionSolid("BeamTubeVacSS4_SV", BeamTubeVac_p30019SubSV,SubCylBeamTubeSS4_SV,0,SubCylBeamTubeSS4_Ps);

  G4LogicalVolume* BeamTubeVacSS4_LV = new  G4LogicalVolume(BeamTubeVacSS4_SubSV,fVacuum, "BeamTubeVacSS_LV");

  new G4PVPlacement(0,BeamTubeVacSS4_Ps,BeamTubeVacSS4_LV,"BeamTubeVacSS4_PV",TrSh1_LV,false,0,1);


  // ----------------------------------------------------------------------------
  //
  // TARGET TUBE (Relative to TrSh1)
  //
  // ----------------------------------------------------------------------------

  /// Beryllium tube around target
  //G4double Tr_center = Tr_center; // translation to target center

  G4double TrTube_1Z  = (TrSh1_Ztop-12*cm)/2;

  G4VSolid* TrTubeBe_1SV = new G4Tubs("TrTubeBe_1SV",
  TrTubeBe_RMin,TrTubeBe_RMax, TrTube_1Z, 0, 360*deg);

  /// Relative to TrSh1
  G4double TrTube_1dz = -(12*cm+TrTube_1Z);
  G4ThreeVector TrTube_TrSh1Ps = G4ThreeVector(0,Tr_center,TrTube_1dz-TrSh1_dz);

  G4LogicalVolume* TrTubeBe_1LV = new  G4LogicalVolume(TrTubeBe_1SV,fBeryllium, "TrTubeBe_LV");
  new G4PVPlacement(0,TrTube_TrSh1Ps,TrTubeBe_1LV,"TrTubeBe_1PV",TrSh1_LV,false,0,1);

    // ----------------------------------------------------------------------------
    //
    // TARGET SURROUNDING
        // Relative to TrSh1 and TrSh2 (for proper material definition)
    //
    // ----------------------------------------------------------------------------


    // Vacuum clad
    ///
    G4double TrSurVac1_R1  = 69.6*cm;
    G4double TrSurVac1_R2  = 132.8*cm;
    G4double TrSurVac1_Z  = 15.6*cm/2;
    G4VSolid* TrSurVac1_SV = new G4Tubs("TrSurVac1_SV",
                                        TrSurVac1_R1,TrSurVac1_R2,TrSurVac1_Z, 0, 360*deg);

    G4double TrSurVac2_R1  = 48*cm;
    G4double TrSurVac2_R2  = TrSurVac1_R1;
    G4double TrSurVac2_Z  = 19.6*cm/2;
    G4VSolid* TrSurVac2_SV = new G4Tubs("TrSurVac2_SV",
                                        TrSurVac2_R1,TrSurVac2_R2,TrSurVac2_Z, 0, 360*deg);

    G4double TrSurVac3_R  = TrSurVac2_R1;
    G4double TrSurVac3_Z = 16.8*cm/2;
    G4VSolid* TrSurVac3_SV = new G4Tubs("TrSurVac3_SV",
                                        0,TrSurVac3_R, TrSurVac3_Z, 0, 360*deg);

    G4double TrSurVac4_R  = 50*cm;
    G4double TrSurVac4_Z = (18*cm-9.8*cm)/2;
    G4VSolid* TrSurVac4_SV = new G4Tubs("TrSurVac4_SV",
                                        0,TrSurVac4_R, TrSurVac4_Z, 0, 360*deg);

    G4double TrSurVac4_1R  = 48*cm;
    G4double TrSurVac4_1Z = 2*cm/2;
    G4VSolid* TrSurVac4_1SV = new G4Tubs("TrSurVac4_1SV",
                                        0,TrSurVac4_1R, TrSurVac4_1Z, 0, 360*deg);

    G4double TrSurVac5_R1  = 22*cm;
    G4double TrSurVac5_R2  = TrSurVac4_R;
    G4double TrSurVac5_Z = 4*cm/2;
    G4VSolid* TrSurVac5_SV = new G4Tubs("TrSurVac5_SV",
                                        TrSurVac5_R1,TrSurVac5_R2, TrSurVac5_Z, 0, 360*deg);

    G4double TrSurVac6_R1  = TrSurVac5_R1;
    G4double TrSurVac6_R2  = 29.5*cm;
    G4double TrSurVac6_Z = 4*cm/2;
    G4VSolid* TrSurVac6_SV = new G4Tubs("TrSurVac6_SV",
                                        TrSurVac6_R1,TrSurVac6_R2,TrSurVac6_Z, 0, 360*deg);

    G4double TrSurVac7_R1  = 23*cm;
    G4double TrSurVac7_R2  = 25*cm;
    G4double TrSurVac7_Z = (TrSh1_Ztop-25*cm)/2;
    G4VSolid* TrSurVac7_SV = new G4Tubs("TrSurVac7_SV",
                                        TrSurVac7_R1,TrSurVac7_R2, TrSurVac7_Z, 0, 360*deg);

    G4double TrSurVac9_R1  = TrSurVac7_R1;
    G4double TrSurVac9_R2  = 29.5*cm;;
    G4double TrSurVac9_Z = 5*cm/2;
    G4VSolid* TrSurVac9_SV = new G4Tubs("TrSurVac9_SV",
                                        TrSurVac9_R1,TrSurVac9_R2,TrSurVac9_Z, 0, 360*deg);


    // Position for TrSurVac1 , TrSurVac2
    G4double TrSurVac_dz = 0*cm;
    G4ThreeVector TrSurVac_Ps = G4ThreeVector(0,Tr_center,TrSurVac_dz-TrSh1_dz);
    // Position for TrSurVac3
    G4double TrSurVac3_dz = -(12*cm-TrSurVac3_Z);
    G4ThreeVector TrSurVac3_Ps = G4ThreeVector(0,Tr_center,TrSurVac3_dz-TrSh1_dz);
    // Position for TrSurVac4
    G4ThreeVector TrSurVac4_Ps = G4ThreeVector(0,Tr_center,(18*cm-TrSurVac4_Z)-TrSh1_dz);
    G4ThreeVector TrSurVac4_1Ps = G4ThreeVector(0,Tr_center,(9.8*cm-TrSurVac4_1Z)-TrSh1_dz);
    // Position for TrSurVac5
    G4double TrSurVac5_dz = -(12*cm+TrSurVac5_Z);
    G4ThreeVector TrSurVac5_Ps = G4ThreeVector(0,Tr_center,TrSurVac5_dz-TrSh1_dz);
    // Position for TrSurVac6
    G4double TrSurVac6_dz = -(16*cm+TrSurVac6_Z);
    G4ThreeVector TrSurVac6_Ps = G4ThreeVector(0,Tr_center,TrSurVac6_dz-TrSh1_dz);
    // Position for TrSurVac7
    G4double TrSurVac7_dz = (-(25*cm+TrSurVac7_Z));
    G4ThreeVector TrSurVac7_Ps = G4ThreeVector(0,Tr_center,TrSurVac7_dz-TrSh1_dz);
    // Position for TrSurVac9
    G4double TrSurVac9_dz = -(20*cm+TrSurVac9_Z);
    G4ThreeVector TrSurVac9_Ps = G4ThreeVector(0,Tr_center,TrSurVac9_dz-TrSh1_dz);


    G4LogicalVolume* TrSurVac3_LV = new  G4LogicalVolume(TrSurVac3_SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac4_LV = new  G4LogicalVolume(TrSurVac4_SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac4_1LV = new  G4LogicalVolume(TrSurVac4_1SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac5_LV = new  G4LogicalVolume(TrSurVac5_SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac6_LV = new  G4LogicalVolume(TrSurVac6_SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac7_LV = new  G4LogicalVolume(TrSurVac7_SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac9_LV = new  G4LogicalVolume(TrSurVac9_SV,fVacuum, "TrSurVac_LV");


    // ----------------------------------------------------------------------------
    //
    // VACUUM POCKET AROUND TARGET (TrVac)
    //
    // ----------------------------------------------------------------------------

    // SIDE TRAPEZOID

    //G4double TrShVac3_R1 = SSTrShield_.TrShVac3_InRad();
    G4double TrVacPocket_Z = 39.3*cm;
    G4VSolid* TrVacPocket_SV = new G4Tubs("TrVacPocket_SV",
                                         200*cm,TrShVac3_R1,TrVacPocket_Z,-20*deg,40*deg);

    G4double TrVacPocket_dz = 39.3*cm - TrVacPocket_Z;
    G4ThreeVector TrVacPocket_Ps = G4ThreeVector(0,0,TrVacPocket_dz-TrSh1_dz);

    /// Auxiliary Plan on xz axis
    G4double h_p36066 = 50*cm;
    G4double Tr631_x  = (1.668176/23.31190)*cm;
    G4double Tr633_x  = -(1.668176/23.31190)*cm;
    G4Box* p36066_SV = new G4Box("p36066_SV",
                               500*cm,h_p36066,50*cm);

    G4double h_p36102 = 60*cm;
    G4double Tr632_z  = (206.7821/14.30067)*cm;
    G4Box* p36102_SV = new G4Box("p36102_SV",
                              500*cm,h_p36102,h_p36102);

    G4double pXY_Rot = 9.156*deg; // 2.456*deg in original coord system
    G4RotationMatrix* p36066_Rot = new G4RotationMatrix();
    p36066_Rot->rotateZ(-pXY_Rot);
    G4RotationMatrix* p36061_Rot = new G4RotationMatrix();
    p36061_Rot->rotateZ(pXY_Rot);
    G4ThreeVector p36066_Ps = G4ThreeVector(Tr633_x, h_p36066,-TrVacPocket_dz);
    G4ThreeVector p36061_Ps = G4ThreeVector(Tr631_x,-h_p36066,-TrVacPocket_dz);

    G4double pXZ_Rot = 4*deg;
    G4RotationMatrix* p36102_Rot = new G4RotationMatrix();
    p36102_Rot->rotateY(-pXZ_Rot);
    G4ThreeVector p36102_Ps = G4ThreeVector(0,0,-(h_p36102+Tr632_z)-TrVacPocket_dz);

    G4VSolid* TrVacPocket_SubSV = new G4SubtractionSolid("TrVacPocket_SubSV",TrVacPocket_SV, p36066_SV, p36066_Rot, p36066_Ps);
    G4VSolid* TrVacPocket_Sub2SV = new G4SubtractionSolid("TrVacPocket_Sub2SV",TrVacPocket_SubSV, p36066_SV, p36061_Rot, p36061_Ps);
    G4VSolid* TrVacPocket_Sub3SV = new G4SubtractionSolid("TrVacPocket_Sub3SV",TrVacPocket_Sub2SV, p36102_SV, p36102_Rot, p36102_Ps);

    G4LogicalVolume* TrVacPocket_LV = new  G4LogicalVolume(TrVacPocket_Sub3SV,fVacuum, "TrVacPocket_LV");

    new G4PVPlacement(0,TrVacPocket_Ps,TrVacPocket_LV,"TrVacPocket_PV",TrSh1_LV,false,0,1);

    G4double TrVacPocketBase_R1 = 19.5*cm;
    //G4double TrVacPocketBase_R1 = 132.8*cm; //- Tr_center;
    G4double TrVacPocketBase_R2 = 200*cm;
    G4double TrVacPocketBase_Z = (36*cm-9*cm)/2;
    G4VSolid* TrVacPocketBase_SV = new G4Tubs("TrVacPocketBase_SV",
                                         TrVacPocketBase_R1,TrVacPocketBase_R2,TrVacPocketBase_Z, -63*deg,126*deg);
    //Right side
    G4VSolid* TrVacPocketBaseR_SV = new G4Tubs("TrVacPocketBaseR_SV",
                                          TrVacPocketBase_R1,TrVacPocketBase_R2,TrVacPocketBase_Z, 117*deg,126*deg);

    /// Relative to TrSh1
    G4double TrVacPocketBase_dz = (36*cm-TrVacPocketBase_Z); // relative to world center
    G4ThreeVector TrVacPocketBase_Ps = G4ThreeVector(0,0,TrVacPocketBase_dz-TrSh1_dz);

    /// Auxiliary Cone on z axis
    G4double k70003_t = 4639.409; //tangent to opening angle
    G4double k70003_dz = 12.13629*cm;
    G4double k70003_Z = k70003_dz/2;
    G4double k70003_R2 = sqrt(k70003_t)* k70003_dz ;
    G4VSolid* k70003_SV = new G4Cons("k70003_SV",
                                         0,0,0,k70003_R2,k70003_Z, 0*deg,360*deg);

    G4double k70003_dz0 = k70003_dz-k70003_Z; // relative to world center
    G4ThreeVector k70003_Ps = G4ThreeVector(0,0,k70003_dz0-TrVacPocketBase_dz-TrSurVac_dz) ;
    G4RotationMatrix* kY_Rot = new G4RotationMatrix();
    kY_Rot->rotateY(180*deg);

    G4VSolid* TrVacPocketBase_SubSV = new G4SubtractionSolid("TrVacPocketBase_SubSV",TrVacPocketBase_SV, k70003_SV, kY_Rot, k70003_Ps);
    G4LogicalVolume* TrVacPocketBase_LV = new  G4LogicalVolume(TrVacPocketBase_SubSV,fVacuum, "TrVacPocket_LV");

    new G4PVPlacement(0,TrVacPocketBase_Ps,TrVacPocketBase_LV,"TrVacPocketBase_PV",TrSh1_LV,false,0,1);

     G4VSolid* TrVacPocketBaseR_SubSV = new G4SubtractionSolid("TrVacPocketBaseR_SubSV",TrVacPocketBaseR_SV, k70003_SV, kY_Rot, k70003_Ps);
     G4LogicalVolume* TrVacPocketBaseR_LV = new  G4LogicalVolume(TrVacPocketBaseR_SubSV,fVacuum, "TrVacPocket_LV");

     new G4PVPlacement(0,TrVacPocketBase_Ps,TrVacPocketBaseR_LV,"TrVacPocketBase_PV",TrSh1_LV,false,0,1);


    // TOP PART OF THE VACUUM POCKET

    /// Relative to TrSh1
    G4double TrVacPocketTop_Z = 12*cm;
    G4VSolid* TrVacPocketTop_SV = new G4Tubs("TrVacPocketTop_SV",
                                       TrVacPocketBase_R1,TrVacPocketBase_R2,TrVacPocketTop_Z, -63*deg,126*deg);
    //Right side
    G4VSolid* TrVacPocketTopR_SV = new G4Tubs("TrVacPocketTopR_SV",
                                        TrVacPocketBase_R1,TrVacPocketBase_R2,TrVacPocketTop_Z, 117*deg,126*deg);

    /// Relative to TrSh1
    G4double TrVacPocketTop_dz = -(2*cm+TrVacPocketTop_Z);
    G4ThreeVector TrVacPocketTop_Ps = G4ThreeVector(0,0,TrVacPocketTop_dz-TrSh1_dz); //TrVacPocketTop_dz-TrSh1_dz);

    /// Auxiliary Cone on z axis
    G4double k70001_t = 1891.726; //tangent to opening angle
    G4double k70001_dz = 12.29834*cm;
    G4double k70001_Z = k70001_dz/2;
    G4double k70001_R2 = sqrt(k70001_t)* k70001_dz ;
    G4VSolid* k70001_SV = new G4Cons("k70001_SV",
                                       0,0,0,k70001_R2,k70001_Z, 0*deg,360*deg);

    G4double k70001_dz0 = -(k70001_dz-k70001_Z); // relative to world center
    G4ThreeVector k70001_Ps = G4ThreeVector(0,0,k70001_dz0-TrVacPocketTop_dz);
    G4ThreeVector k70001W_Ps = G4ThreeVector(0,0,k70001_dz0-TrSh1_dz);


    G4double k70002_t = 1891.726; //tangent to opening angle
    G4double k70002_dz = 15.10166*cm; //cm
    G4double k70002_Z = k70002_dz/2; //cm
    G4double k70002_R2 = sqrt(k70002_t) * k70002_dz;
    G4VSolid* k70002_SV = new G4Cons("k70002_SV",0,0,0,k70002_R2,k70002_Z, 0*deg,360*deg);

    G4double k70002_dz0 = -(k70002_dz+k70002_Z); // relative to world center
    G4ThreeVector k70002_Ps = G4ThreeVector(0,0,k70002_dz0-TrVacPocketTop_dz); //-TrVacPocketTop_dz);

    G4ThreeVector k70002W_Ps = G4ThreeVector(0,0,k70002_dz0-TrSh1_dz); //-TrVacPocketTop_dz);

    G4VSolid* TrVacPocketTop1_SubSV = new G4SubtractionSolid("TrVacPocketTop1_SubSV",TrVacPocketTop_SV, k70001_SV, 0, k70001_Ps);
    G4VSolid* TrVacPocketTop_SubSV = new G4SubtractionSolid("TrVacPocketTop_SubSV",TrVacPocketTop1_SubSV, k70002_SV, kY_Rot, k70002_Ps);

    G4VSolid* TrVacPocketTopR1_SubSV = new G4SubtractionSolid("TrVacPocketTopR1_SubSV",TrVacPocketTopR_SV, k70001_SV, 0, k70001_Ps);
    G4VSolid* TrVacPocketTopR_SubSV = new G4SubtractionSolid("TrVacPocketTopR_SubSV",TrVacPocketTopR1_SubSV, k70002_SV, kY_Rot, k70002_Ps);

    G4ThreeVector TrVacPocketTop_Vac1Ps = G4ThreeVector(0,-Tr_center,TrVacPocketTop_dz);

    G4VSolid* TrVacPocket_UnionSV = new G4UnionSolid("TrVacPocket_UnionSV",TrSurVac1_SV, TrVacPocketTop_SubSV, 0, TrVacPocketTop_Vac1Ps);
    G4VSolid* TrVacPocket_Union1SV = new G4UnionSolid("TrVacPocket_Union1SV",TrVacPocket_UnionSV, TrVacPocketTopR_SubSV, 0, TrVacPocketTop_Vac1Ps);

    G4VSolid* TrVacPocket_Union2SV = new G4UnionSolid("TrVacPocket_Union2SV",TrVacPocket_Union1SV, TrSurVac2_SV, 0, G4ThreeVector());
    //G4LogicalVolume* TrVacPocket_UnionLV = new  G4LogicalVolume(TrVacPocket_UnionSV,fVacuum, "TrVacPocketUnion_LV");

    G4LogicalVolume* TrSurVac1_LV = new  G4LogicalVolume(TrVacPocket_Union2SV,fVacuum, "TrSurVac_LV");
    G4LogicalVolume* TrSurVac2_LV = new  G4LogicalVolume(TrVacPocket_Union2SV,fVacuum, "TrSurVac_LV");

    new G4PVPlacement(0,TrSurVac_Ps,TrSurVac1_LV,"TrVacPocket_PV",TrSh1_LV,false,0,1);


    //G4double dx_Wedge = 13.8*cm;
    G4double dz_Wedge = 16*cm/2; //16.0634*cm/2;

    //G4double Wedge_angle = atan(dx_Wedge/2/TrVacPocketBase_R2);

    G4double Dangle_Wedge = 1.976*deg;
    G4double D0angle_Wedge = -(36*deg+Dangle_Wedge);
    G4double D0angle_WedgeR = 216*deg-Dangle_Wedge;

    G4double PsZ_Wedge = -dz_Wedge; //-0.833333*cm; //relative to world center
    G4ThreeVector TrVacPocketWedge_Ps = G4ThreeVector(0,0,PsZ_Wedge-TrSh1_dz);

    G4VSolid* TrVacPocketWedge_SV = new G4Tubs("TrVacPocketWedge_SV",TrVacPocketBase_R1,TrVacPocketBase_R2, dz_Wedge, D0angle_Wedge,2*Dangle_Wedge);
    G4VSolid* TrVacPocketWedgeR_SV = new G4Tubs("TrVacPocketWedgeR_SV",TrVacPocketBase_R1,TrVacPocketBase_R2, dz_Wedge, D0angle_WedgeR,2*Dangle_Wedge);

    G4ThreeVector k70001Wedge_Ps = G4ThreeVector(0,0,k70001_dz0-PsZ_Wedge);
    G4VSolid* TrVacPocketWedge_SubSV = new G4IntersectionSolid("TrVacPocketWedge_SubSV",TrVacPocketWedge_SV, k70001_SV, 0, k70001Wedge_Ps);
    G4VSolid* TrVacPocketWedgeR_SubSV = new G4IntersectionSolid("TrVacPocketWedgeR_SubSV",TrVacPocketWedgeR_SV, k70001_SV, 0, k70001Wedge_Ps);


/*** DEFINICIÓN DE VACÍO ALREDEDOR DEL TARGET: María


//Medidas mcnp
	G4double VacEllip_pXSemiAxis = 130.60*cm;
	G4double VacEllip_pYSemiAxis = 23.66*cm;
	G4double VacEllip_pZSemiAxis = 130.60*cm;

	G4double VacCyl_RMin = 0;
	G4double VacCyl_RMax = 125*cm;
	G4double VacCyl_Dz = 200*cm/2;
	G4double VacCyl_Phi = 0*deg;
	G4double VacCyl_DPhi = 360*deg;


	G4VSolid* Vac_ellipsoid = new G4Ellipsoid("Vac_ellipsoid",VacEllip_pXSemiAxis,VacEllip_pYSemiAxis,VacEllip_pZSemiAxis,0,0);
	G4VSolid* Vac_cyl = new G4Tubs("Vac_cyl", VacCyl_RMin, VacCyl_RMax, VacCyl_Dz, VacCyl_Phi, VacCyl_DPhi);

	G4VSolid* Vac_Target = new G4SubtractionSolid("Vac_Target",Vac_ellipsoid,Vac_cyl,TrdLeft_Rot,G4ThreeVector(0,0,0));

	G4LogicalVolume* Vac_Target_LV = new  G4LogicalVolume(Vac_Target,fVacuum, "Vac_Target_LV");
	G4ThreeVector Vac_Target_Ps = G4ThreeVector (0,110*cm,0);

	new G4PVPlacement(TrdLeft_Rot,Vac_Target_Ps,Vac_Target_LV,"Vac_Target_PV",TrSh1_LV,false,0,1); //probablemente haya que rotarlo
***/


/***********************************************/

 /**Aqui las lineas están comentadas

    G4ThreeVector TrVacPocketWedge_UnionPs = G4ThreeVector(0,0,PsZ_Wedge-TrVacPocketTop_dz);
    G4VSolid* TrVacPocketWedge_UnionSV = new G4UnionSolid("TrVacPocketWedge_SubSV",TrVacPocket_Union2SV,TrVacPocketWedge_SV, 0, TrVacPocketWedge_UnionPs);
    G4VSolid* TrVacPocketWedgeR_SubSV = new G4UnionSolid("TrVacPocketWedgeR_SubSV",TrVacPocketWedge_UnionSV,TrVacPocketWedgeR_SV, 0, k70001Wedge_Ps);


    G4LogicalVolume* TrVacPocketWedge_LV = new  G4LogicalVolume(TrVacPocketWedge_SubSV,fVacuum, "TrVacPocketWedge_LV");
    G4LogicalVolume* TrVacPocketWedgeR_LV = new  G4LogicalVolume(TrVacPocketWedgeR_SubSV,fVacuum, "TrVacPocketWedge_LV");


    new G4PVPlacement(0,     //Rotation of mother frame
    TrVacPocketWedge_Ps,                        //position in rotated frame
    TrVacPocketWedge_LV,                         //its logical volume
    "TrVacPocketWedge_PV",                   //its name
    TrSh1_LV,                          //its mother  volume
    false,                            //no boolean operation
    0,                                //copy number
    1);                   //overlaps checking


     new G4PVPlacement(0,     //Rotation of mother frame
     TrVacPocketWedge_Ps,                        //position in rotated frame
     TrVacPocketWedgeR_LV,                         //its logical volume
     "TrVacPocketWedge_PV",                   //its name
     TrSh1_LV,                          //its mother  volume
     false,                            //no boolean operation
     0,                                //copy number
     1);                   //overlaps checking


	hasta aqui**/

    new G4PVPlacement(0,TrSurVac3_Ps,TrSurVac3_LV,"TrSurVac3_PV",TrSh1_LV,false,0,1);

    new G4PVPlacement(0,TrSurVac4_Ps,TrSurVac4_LV,"TrSurVac4_PV",TrSh1_LV,false,0,1);

    new G4PVPlacement(0,TrSurVac4_1Ps,TrSurVac4_1LV,"TrSurVac4_1PV",TrSh1_LV,false,0,1);

    new G4PVPlacement(0,TrSurVac5_Ps,TrSurVac5_LV,"TrSurVac5_PV",TrSh1_LV,false,0,1);

    new G4PVPlacement(0,TrSurVac6_Ps,TrSurVac6_LV,"TrSurVac6_PV",TrSh1_LV,false,0,1);

    new G4PVPlacement(0,TrSurVac7_Ps,TrSurVac7_LV,"TrSurVac7_PV",TrSh1_LV,false,0,1);

    new G4PVPlacement(0,TrSurVac9_Ps,TrSurVac9_LV,"TrSurVac9_PV",TrSh1_LV,false,0,1);


    // SS316L clad around target (axisymmetric)
    ///
    G4double TrSur1_R1 = 75*cm;
    G4double TrSur1_R2 = 125*cm+4.3*cm;
    G4double TrSur1_Z = 1.8*cm/2;
    G4VSolid* TrSur1_SV = new G4Tubs("TrSur1_SV",
                                     TrSur1_R1,TrSur1_R2, TrSur1_Z, 0, 360*deg);

    G4double TrSur11_Z = 1.7*cm/2;
    G4VSolid* TrSur11_SV = new G4Tubs("TrSur11_SV",
                                      TrSur1_R1,TrSur1_R2, TrSur11_Z , 0, 360*deg);

    G4double TrSur2_R1 = TrSurVac2_R1;
    G4double TrSur2_R2 = 67.6*cm;
    G4double TrSur2_Z = 4*cm/2;
    G4VSolid* TrSur2_SV = new G4Tubs("TrSur2_SV",
                                     TrSur2_R1,TrSur2_R2, TrSur2_Z, 0, 360*deg);

    G4double TrSur3_R1 = TrSurVac1_R1;
    G4double TrSur3_R2 = TrSur1_R1;
    G4double TrSur3_Z = 1*cm/2;
    G4VSolid* TrSur3_SV = new G4Tubs("TrSur3_SV",
                                     TrSur3_R1,TrSur3_R2,TrSur3_Z, 0, 360*deg);

    G4double TrSur3_2R1 = TrSur2_R2;
    G4double TrSur3_2R2 = TrSurVac1_R1;
    G4VSolid* TrSur3_2SV = new G4Tubs("TrSur3_2SV",
                                     TrSur3_2R1,TrSur3_2R2,TrSur3_Z, 0, 360*deg);

    G4double TrSur4_R1 = TrSurVac2_R2;
    G4double TrSur4_R2 = 74.5*cm;
    G4double TrSur4_Z = 0.5*cm/2;
    G4VSolid* TrSur4_SV = new G4Tubs("TrSur4_SV",
                                     TrSur4_R1,TrSur4_R2, TrSur4_Z, 0, 360*deg);

    G4double TrSur4_2R1 = TrSurVac2_R1;
    G4double TrSur4_2R2 = TrSurVac2_R2;
    G4VSolid* TrSur4_2SV = new G4Tubs("TrSur4_2SV",
                                     TrSur4_2R1,TrSur4_2R2, TrSur4_Z, 0, 360*deg);

    G4double TrSur5_R1 = TrSur4_R2;
    G4double TrSur5_R2 = TrSur1_R1;
    G4double TrSur5_Z = 1*cm/2;
    G4VSolid* TrSur5_SV = new G4Tubs("TrSur5_SV",
                                     TrSur5_R1,TrSur5_R2, TrSur5_Z, 0, 360*deg);

    G4double TrSur7_R =  27*cm;
    G4double TrSur7_Z = 1.3*cm/2;
    G4VSolid* TrSur7_SV = new G4Tubs("TrSur7_SV",
                                     0,TrSur7_R, TrSur7_Z, 0, 360*deg);

    G4double TrSur8_R1 = 22*cm;
    G4double TrSur8_R2 = TrSur2_R1;
    G4double TrSur8_Z = 3*cm/2;
    G4VSolid* TrSur8_SV = new G4Tubs("TrSur8_SV",
                                     TrSur8_R1,TrSur8_R2, TrSur8_Z, 0, 360*deg);

    //G4double TrSur9_R1 = 23*cm;
    //G4double TrSur9_R2 = 33*cm;
    //G4double TrSur9_Z = (281*cm-201*cm)/2;
    //G4VSolid* TrSur9_SV = new G4Tubs("TrSur9_SV",
    //                                  TrSur9_R1,TrSur9_R2,TrSur9_Z, 0, 360*deg);

    G4double TrSur10_R1 =  TrSurVac9_R1;
    G4double TrSur10_R2 = 27.5*cm;
    G4double TrSur10_Z = 4*cm/2;
    G4VSolid* TrSur10_SV = new G4Tubs("TrSur10_SV",
                                      TrSur10_R1,TrSur10_R2, TrSur10_Z, 0, 360*deg);


    /// Position relative to target center ( TrSurVac1, TrSurVac2 )
    G4double TrSur1Vac_dz = -(4.1*cm+TrSur1_Z);
    G4ThreeVector TrSur1_VacPs = G4ThreeVector(0,0,TrSur1Vac_dz);
    G4double TrSur11Vac_dz = (5.8*cm-TrSur11_Z);
    G4ThreeVector TrSur11_VacPs = G4ThreeVector(0,0,TrSur11Vac_dz);

    G4double TrSur2VacD_dz = 7.8*cm-TrSur2_Z;
    G4double TrSur2VacU_dz = -(4.8*cm+TrSur2_Z);
    G4ThreeVector TrSur2Down_VacPs = G4ThreeVector(0,0,TrSur2VacD_dz);
    G4ThreeVector TrSur2Up_VacPs = G4ThreeVector(0,0,TrSur2VacU_dz);

    G4double TrSur3VacD_dz = (5.8*cm-TrSur3_Z);
    G4double TrSur3VacU_dz = -(4.8*cm+TrSur3_Z);
    G4ThreeVector TrSur3Down_VacPs = G4ThreeVector(0,0, TrSur3VacD_dz);
    G4ThreeVector TrSur3Up_VacPs = G4ThreeVector(0,0,TrSur3VacU_dz);

    G4double TrSur4VacD_dz = (3.8*cm-TrSur4_Z);
    G4double TrSur4VacU_dz = -(3.3*cm+TrSur4_Z);
    G4ThreeVector TrSur4Down_VacPs = G4ThreeVector(0,0,TrSur4VacD_dz);
    G4ThreeVector TrSur4Up_VacPs = G4ThreeVector(0,0,TrSur4VacU_dz);

    G4double TrSur5VacD_dz = (4.3*cm-TrSur5_Z);
    G4double TrSur5VacU_dz = -(3.3*cm+TrSur5_Z);
    G4ThreeVector TrSur5Down_VacPs = G4ThreeVector(0,0,TrSur5VacD_dz);
    G4ThreeVector TrSur5Up_VacPs = G4ThreeVector(0,0,TrSur5VacU_dz);

    /// Position relative to TrSurVac3
        // distance to target center
    G4double TrSur7_dz = (4.8*cm-TrSur7_Z);
    G4ThreeVector TrSur7_Vac3Ps = G4ThreeVector(0,0,TrSur7_dz-TrSurVac3_dz);  //relative to TrSurVac3

    /// Position relative toTrSurVac5
        // distance to target center
    G4double TrSur8_dz = -(12*cm+TrSur8_Z);
    G4ThreeVector TrSur8_Vac5Ps = G4ThreeVector(0,0,(TrSur8_dz-TrSurVac5_dz)); //relative to TrSurVac5

    /// Position relative toTrSurVac8
        // distance to target center
    //G4double TrSur9_dz = -(201*cm+TrSur9_Z);
    //G4ThreeVector TrSur9_Vac8Ps = G4ThreeVector(0,0,TrSur9_dz-TrSurVac8_dz); //relative to TrSurVac9

    /// Position relative toTrSurVac9
        // distance to target center
    G4double TrSur10_dz = -(20*cm+TrSur10_Z);
    G4ThreeVector TrSur10_Vac9Ps = G4ThreeVector(0,0,TrSur10_dz-TrSurVac9_dz); //relative to TrSurVac9

    G4LogicalVolume* TrSur1_LV = new  G4LogicalVolume(TrSur1_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur11_LV = new  G4LogicalVolume(TrSur11_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur2_LV = new  G4LogicalVolume(TrSur2_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur3_LV = new  G4LogicalVolume(TrSur3_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur3_2LV = new  G4LogicalVolume(TrSur3_2SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur4_LV = new  G4LogicalVolume(TrSur4_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur4_2LV = new  G4LogicalVolume(TrSur4_2SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur5_LV = new  G4LogicalVolume(TrSur5_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur7_LV = new  G4LogicalVolume(TrSur7_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur8_LV = new  G4LogicalVolume(TrSur8_SV,fSS316L, "TrSur_LV");
    //G4LogicalVolume* TrSur9_LV = new  G4LogicalVolume(TrSur9_SV,fSS316L, "TrSur_LV");
    G4LogicalVolume* TrSur10_LV = new  G4LogicalVolume(TrSur10_SV,fSS316L, "TrSur_LV");

    // Placement relative to TrSurVac1_LV (mother volume)
    new G4PVPlacement(0,TrSur1_VacPs,TrSur1_LV,"TrSur1_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur11_VacPs,TrSur11_LV,"TrSur11_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur3Down_VacPs,TrSur3_LV,"TrSur3_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur3Up_VacPs,TrSur3_LV,"TrSur3_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur4Down_VacPs,TrSur4_LV,"TrSur4_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur4Up_VacPs,TrSur4_LV,"TrSur4_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur5Down_VacPs,TrSur5_LV,"TrSur5_PV",TrSurVac1_LV,false,0,1);

    new G4PVPlacement(0,TrSur5Up_VacPs,TrSur5_LV,"TrSur5_PV",TrSurVac1_LV,false,0,1);

    // Placement relative to TrSurVac2_LV (mother volume)
    new G4PVPlacement(0,TrSur2Up_VacPs,TrSur2_LV,"TrSur2Up_PV",TrSurVac2_LV,false,0,1);

    new G4PVPlacement(0,TrSur3Down_VacPs,TrSur3_2LV,"TrSur3_2PV",TrSurVac2_LV,false,0,1);

    new G4PVPlacement(0,TrSur3Up_VacPs,TrSur3_2LV,"TrSur3_2PV",TrSurVac2_LV,false,0,1);

    new G4PVPlacement(0,TrSur4Down_VacPs,TrSur4_2LV,"TrSur4_2PV",TrSurVac2_LV,false,0,1);

    new G4PVPlacement(0,TrSur4Up_VacPs,TrSur4_2LV,"TrSur4_2PV",TrSurVac2_LV,false,0,1);

    new G4PVPlacement(0,TrSur2Down_VacPs,TrSur2_LV,"TrSur2Down_PV",TrSurVac2_LV,false,0,1);

    // Placement relative to TrSurVac3_LV (mother volume)
    new G4PVPlacement(0,TrSur7_Vac3Ps,TrSur7_LV,"TrSur7_PV",TrSurVac3_LV,false,0,1);

    // Placement relative to TrSurVac5_LV (mother volume)
    new G4PVPlacement(0,TrSur8_Vac5Ps,TrSur8_LV,"TrSur8_PV",TrSurVac5_LV,false,0,1);

    // Placement relative to TrSurVac9_LV (mother volume)
    new G4PVPlacement(0,TrSur10_Vac9Ps,TrSur10_LV,"TrSur10_PV",TrSurVac9_LV,false,0,1);

    // Placement relative to TrSurVac8_LV (mother volume)
    //new G4PVPlacement(0,TrSur9_Vac8Ps,TrSur9_LV,"TrSur9_PV",TrSurVac8_LV,false,0,1);

  //G4VSolid* TrSurVacTorus_SV = new G4Torus("TrSurVacTorus_SV", pRMin,pRMax, pRTor, pSphi, pDphi)


  // ----------------------------------------------------------------------------
  //
  // TARGET DEFINITION Relative to TrSh1
  //
  // ----------------------------------------------------------------------------

      ///////////////////////////////////////////////////////////////
      // Target tungsten 1025
      G4double Tr_R1 = 77*cm;
      G4double Tr_R2 = 125*cm;
      G4double Tr_Z= 4*cm;
      //
      //G4ThreeVector Tr_Ps = G4ThreeVector(0,Tr_center,0);

      // Relative to TrSh1
      G4ThreeVector Tr_Ps = G4ThreeVector(0,Tr_center,0-TrSh1_dz);
      G4ThreeVector Tr_to_SurVac_Ps = G4ThreeVector(0,0,-TrSurVac_dz);
      //
      G4VSolid* TrS = new G4Tubs("TrS", Tr_R1,Tr_R2, Tr_Z, 0, 360*deg);
      //
      // //////////////////////////////////////////////////////////////////
      // Target pores inside mat 1013
      G4double Tr_Hx = 0.5*cm;
      G4double Tr_Hy = 130*cm;
      G4double Tr_Hz= 5*cm;
      //
      G4VSolid* Tr_HS = new G4Box("Tr_HS", Tr_Hx,Tr_Hy,Tr_Hz);

      G4RotationMatrix* TRR=new G4RotationMatrix();
      TRR->rotateZ(5*deg);

    //  for (G4int i=0; i<36;i++){
          // //////////////////////////////////////////////////////////////////
          // Target pores inside mat 1013
    //      G4VSolid* SubTrHS= new G4IntersectionSolid ("SubTrHS",TrS, Tr_HS, TRR,G4ThreeVector());
    //      G4LogicalVolume* Tr_HLV= new G4LogicalVolume(SubTrHS,fSS316L, "Tr_HLV");

    //      new G4PVPlacement(0,Tr_to_SurVac_Ps,Tr_HLV,"Tr_HPV",TrSurVac1_LV,false,i,1);
          //new G4PVPlacement(0,Tr_Ps,Tr_HLV,"Tr_HPV",TrSh1_LV,false,i,1);
    //      TRR->rotateZ(10*deg);
    //  }

      for (G4int i=0; i<18;i++){
          // Target pores inside mat 1013
          G4VSolid* SubTrHS= new G4IntersectionSolid ("SubTrHS",TrS, Tr_HS, TRR,G4ThreeVector());
          G4LogicalVolume* Tr_HLV= new G4LogicalVolume(SubTrHS,fSS316L, "Tr_HLV");

          new G4PVPlacement(0,Tr_to_SurVac_Ps,Tr_HLV,"Tr_HPV",TrSurVac1_LV,false,i,1);
          //new G4PVPlacement(0,Tr_Ps,Tr_HLV,"Tr_HPV",TrSh1_LV,false,i,1);
          TRR->rotateZ(10*deg);
      }

      ///////////////////////////////////////////////////////////////
      // Target tungsten 1025
      G4RotationMatrix* TRR0=new G4RotationMatrix();
      TRR0->rotateZ(5*deg);
      G4VSolid* SubTrS=new G4SubtractionSolid ("SubTrS",TrS, Tr_HS, TRR0,G4ThreeVector(0,0,0));

      for (G4int i=1; i<36;i++){
          TRR0->rotateZ(10*deg);
          SubTrS= new G4SubtractionSolid ("SubTrS",SubTrS, Tr_HS, TRR0,G4ThreeVector(0,0,0));
      }

      G4LogicalVolume* TrLV = new G4LogicalVolume(SubTrS,fTungsten, "TrLV");
      new G4PVPlacement(0,Tr_to_SurVac_Ps,TrLV,"TrPV",TrSurVac1_LV,false,0,1);

      // /////////////////////////////////////////////////////////////////////////////////
      // upper gap of target void
      //cell 360
      G4double UTr_G=1*mm;
      // upper clad of target 1013
      //cell 362
      G4double UTrC_R1 = 74.5*cm;
      //G4double Target_R2 = 125*cm;
      G4double UTrC_Z=1*mm;
      //
      G4ThreeVector UTrC_Ps = G4ThreeVector(0,Tr_center,
      Tr_Z+UTr_G+UTrC_Z);
      //
      G4VSolid* UTrCS = new G4Tubs("UTrCS",
      UTrC_R1,Tr_R2,UTrC_Z, 0, 360*deg);
      //
      G4LogicalVolume* UTrCLV
      = new G4LogicalVolume(UTrCS,fSS316L, "UTrCLV");
      //  G4VPhysicalVolume* UpperTargetCladPV=
      //  new G4PVPlacement(0,
      //  UTrC_Ps,       //at (0,0,0)
      //  UTrCLV,                //its logical volume
      //  "UTrCPV",                //its name
      //  worldLV,               //its mother  volume
      //  false,                 //no boolean operation
      //  0,   //copy number
      //  1);        //overlaps checking

      // /////////////////////////////////////////////////////////////////////////////////
      // cell 364
      // upper clad of target 1017
      G4double UTrC2_R1 = 75*cm;
      //G4double Target_R2 = 125*cm;
      G4double UTrC2_Z=2.5*mm;
      //
      G4ThreeVector UTrC2_Ps = G4ThreeVector(0,Tr_center,
      Tr_Z+UTr_G+UTrC_Z*2+UTrC2_Z);
      //
      G4VSolid* UTrC2S = new G4Tubs("UTrC2S",
      UTrC2_R1,Tr_R2, UTrC2_Z, 0, 360*deg);

      G4LogicalVolume* UTrC2LV
      = new G4LogicalVolume(UTrC2S,fVacuum, "UTrC2LV");
      //G4VPhysicalVolume* UTrC2PV=
      //  new G4PVPlacement(0,
      //  UTrC2_Ps,       //at (0,0,0)
      //  UTrC2LV,                //its logical volume
      //  "UTrC2PV",                //its name
      //  worldLV,               //its mother  volume
      //  false,                 //no boolean operation
      //  0,   //copy number
      //  1);        //overlaps checking

      // /////////////////////////////////////////////////////////////////////////////////
      // upper gap of target
      //cell 402
      // upper clad of target 1013
      G4double UTrC3_R1 = 45*cm;
      G4double UTrC3_R2 = 125*cm+4.3*cm;
      G4double UTrC3_Z=5*mm;
      //
      G4ThreeVector UTrC3_Ps = G4ThreeVector(0,Tr_center,
      Tr_Z+UTr_G+UTrC_Z*2+UTrC2_Z*2+UTrC3_Z);
      //
      G4VSolid* UTrC3S = new G4Tubs("UTrC3S",
      UTrC3_R1,UTrC3_R2, UTrC3_Z, 0, 360*deg);
      //
      G4LogicalVolume* UTrC3LV
      = new G4LogicalVolume(UTrC3S,fSS316L, "UTrC3LV");
      //  G4VPhysicalVolume* UpperTargetClad3PV=
      //  new G4PVPlacement(0,
      //  UTrC3_Ps,       //at (0,0,0)
      //  UTrC3LV,                //its logical volume
      //  "UTrC3PV",                //its name
      //  worldLV,               //its mother  volume
      //  false,                 //no boolean operation
      //  0,   //copy number
      //  1);        //overlaps checking

      //


    // ----------------------------------------------------------------------------
    //
    // Beryllium REFLECTOR
    //
    // ----------------------------------------------------------------------------

      G4double Ref_R1 = 34*cm;
      G4double Ref_R2 = 35.3*cm;
      G4double RefBe_H = (37.1*cm-17.1*cm)/2;
      G4double RefVac_H = (38.9*cm-11.1*cm)/2;
      //
      G4VSolid* RefBe_SV = new G4Tubs("RefS", 0,Ref_R1, RefBe_H, 0*deg, 360*deg);
      G4VSolid* RefVac_SV = new G4Tubs("RefS", Ref_R1,Ref_R2, RefVac_H, 0*deg, 360*deg);

      G4double RefBe_ZPs = -(17.1*cm+RefBe_H);
      G4double RefVac_ZPs = -(11.1*cm+RefVac_H);

      G4ThreeVector RefBe_Ps = G4ThreeVector(0,0,RefBe_ZPs-TrSh1_dz);
      G4ThreeVector RefVac_Ps = G4ThreeVector(0,0,RefVac_ZPs-TrSh1_dz);
      //
      //
      G4ThreeVector RefVac_RelativePs = G4ThreeVector(0,Tr_center,TrSurVac_dz-RefVac_ZPs);
      G4VSolid* RefVac_SubSV = new G4SubtractionSolid("RefVac_SubSV",RefVac_SV, TrVacPocket_Union2SV, 0, RefVac_RelativePs);
      //
      //
      G4LogicalVolume* RefBe_LV= new G4LogicalVolume(RefBe_SV,fBeryllium, "RefBe_LV");
      //G4LogicalVolume* RefVac_LV= new G4LogicalVolume(RefVac_SV,fVacuum, "RefVac_LV");
      G4LogicalVolume* RefVac_LV= new G4LogicalVolume(RefVac_SubSV,fVacuum, "RefVac_LV");

      new G4PVPlacement(0,RefBe_Ps,RefBe_LV,"RefBe_PV",TrSh1_LV,false,0,1);
      new G4PVPlacement(0,RefVac_Ps,RefVac_LV,"RefVac_PV",TrSh1_LV,false,0,1);


    // ----------------------------------------------------------------------------
    //
    // END OF TARGET DEFINITION
    //
    // ----------------------------------------------------------------------------


}
