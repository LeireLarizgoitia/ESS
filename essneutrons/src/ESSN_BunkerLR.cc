#include "ESSN_BunkerLR.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"

ESSN_BunkerLR::ESSN_BunkerLR()
{}

ESSN_BunkerLR::~ESSN_BunkerLR()
{}

void ESSN_BunkerLR::Construct()
{
  G4Material* fMagnadense = G4Material::GetMaterial("fMagnadense");
  G4Material* fConcrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Material* fAir = G4Material::GetMaterial("G4_AIR");

  G4VSolid* BunkLR_SV = new G4Tubs("BunkLR_SV",
                                      ConcBunk_RMin,ConcBunkLong_R2, Bunker_Z, BunkLR_phi0,BunkL_dphi);

  // Mother volume
  G4LogicalVolume* BunkLR_LV = new G4LogicalVolume(BunkLR_SV, fMagnadense, "Bunk_LV");
  this->SetLogicalVolume(BunkLR_LV);


  //Concrete
  G4VSolid* ConcBunkLRFloorIn_SV = new G4Tubs("ConcBunkLRFloorIn_SV",
                                               ConcBunk_RMin,ConcBunkLong_R1,ConcBunkFloorIn_Z,BunkLR_phi0,BunkL_dphi);

  G4LogicalVolume* ConcBunkLRFloorIn_LV = new G4LogicalVolume(ConcBunkLRFloorIn_SV, fConcrete, "BunkInConc_LV");
  new G4PVPlacement(0,ConcBunkFloorIn_Ps,ConcBunkLRFloorIn_LV,"ConcBunkLRFloorIn_PV",BunkLR_LV,false,0,1);

  //Air
  G4VSolid* BunkAirInLR_SV = new G4Tubs("BunkAirInLR_SV",
                                               ConcBunk_RMin,ConcBunkLong_R1, BunkAirIn_Z, BunkLR_phi0,BunkL_dphi);
  G4LogicalVolume* BunkAirInLR_LV = new G4LogicalVolume(BunkAirInLR_SV, fAir, "BunkInAir_LV");
  new G4PVPlacement(0,BunkAirIn_Ps,BunkAirInLR_LV,"BunkAirInLR_PV",BunkLR_LV,false,0,1);

}
