#include "ESSN_BunkerS.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"

ESSN_BunkerS::ESSN_BunkerS()
{}

ESSN_BunkerS::~ESSN_BunkerS()
{}

void ESSN_BunkerS::Construct()
{
  G4Material* fMagnadense = G4Material::GetMaterial("fMagnadense");
  G4Material* fConcrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Material* fAir = G4Material::GetMaterial("G4_AIR");

  G4VSolid* BunkS_SV = new G4Tubs("BunkS_SV",
                                  ConcBunk_RMin,ConcBunkShort_R2, Bunker_Z, BunkS_phi0,BunkS_dphi);

  // Mother volume
  G4LogicalVolume* BunkS_LV = new G4LogicalVolume(BunkS_SV, fMagnadense, "Bunk_LV");
  this->SetLogicalVolume(BunkS_LV);

  //Concrete
  G4VSolid* ConcBunkSFloorIn_SV = new G4Tubs("ConcBunkSFloorIn_SV",
                                             ConcBunk_RMin,ConcBunkShort_R1,
                                             ConcBunkFloorIn_Z,BunkS_phi0,BunkS_dphi);

  G4LogicalVolume* ConcBunkSFloorIn_LV = new G4LogicalVolume(ConcBunkSFloorIn_SV, fConcrete, "BunkInConc_LV");
  new G4PVPlacement(0,ConcBunkFloorIn_Ps,ConcBunkSFloorIn_LV,"ConcBunkSFloorIn_PV",BunkS_LV,false,0,1);

  //Air
  G4VSolid* BunkAirInS_SV = new G4Tubs("BunkAirInS_SV",
                                       ConcBunk_RMin,ConcBunkShort_R1, BunkAirIn_Z, BunkS_phi0,BunkS_dphi);
  G4LogicalVolume* BunkAirInS_LV = new G4LogicalVolume(BunkAirInS_SV, fAir, "BunkInAir_LV");
  new G4PVPlacement(0,BunkAirIn_Ps,BunkAirInS_LV,"BunkAirInS_PV",BunkS_LV,false,0,1);


}
