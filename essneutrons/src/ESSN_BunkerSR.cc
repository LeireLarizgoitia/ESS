#include "ESSN_BunkerSR.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"

ESSN_BunkerSR::ESSN_BunkerSR()
{}

ESSN_BunkerSR::~ESSN_BunkerSR()
{}

void ESSN_BunkerSR::Construct()
{
  G4Material* fMagnadense = G4Material::GetMaterial("fMagnadense");
  G4Material* fConcrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Material* fAir = G4Material::GetMaterial("G4_AIR");

  G4VSolid* BunkSR_SV = new G4Tubs("BunkSR_SV",
                                      ConcBunk_RMin,ConcBunkShort_R2, Bunker_Z, BunkSR_phi0,BunkS_dphi);


  // Mother volume
  G4LogicalVolume* BunkSR_LV = new G4LogicalVolume(BunkSR_SV, fMagnadense, "Bunk_LV");
  this->SetLogicalVolume(BunkSR_LV);


  //Concrete
  G4VSolid* ConcBunkSRFloorIn_SV = new G4Tubs("ConcBunkSRFloorIn_SV",
                                               ConcBunk_RMin,ConcBunkShort_R1, ConcBunkFloorIn_Z, BunkSR_phi0,BunkS_dphi);


  G4LogicalVolume* ConcBunkSRFloorIn_LV = new G4LogicalVolume(ConcBunkSRFloorIn_SV, fConcrete, "BunkInConc_LV");
  new G4PVPlacement(0,ConcBunkFloorIn_Ps,ConcBunkSRFloorIn_LV,"ConcBunkSRFloorIn_PV",BunkSR_LV,false,0,1);

  //Air
  G4VSolid* BunkAirInSR_SV = new G4Tubs("BunkAirInSR_SV",
                                               ConcBunk_RMin,ConcBunkShort_R1, BunkAirIn_Z, BunkSR_phi0,BunkS_dphi);

  G4LogicalVolume* BunkAirInSR_LV = new G4LogicalVolume(BunkAirInSR_SV, fAir, "BunkInAir_LV");
  new G4PVPlacement(0,BunkAirIn_Ps,BunkAirInSR_LV,"BunkAirInSR_PV",BunkSR_LV,false,0,1);

}
