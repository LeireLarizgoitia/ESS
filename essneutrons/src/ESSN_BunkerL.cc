#include "ESSN_BunkerL.hh"

#include "G4PVPlacement.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SystemOfUnits.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"


ESSN_BunkerL::ESSN_BunkerL()
{}

ESSN_BunkerL::~ESSN_BunkerL()
{}

void ESSN_BunkerL::Construct()
{
  G4Material* fMagnadense = G4Material::GetMaterial("fMagnadense");
  G4Material* fConcrete = G4Material::GetMaterial("G4_CONCRETE");
  G4Material* fAir = G4Material::GetMaterial("G4_AIR");

  G4VSolid* BunkL_SV = new G4Tubs("BunkL_SV",
                                  ConcBunk_RMin,ConcBunkLong_R2, Bunk_Z, BunkL_phi0,BunkL_dphi);

  // Mother volume
  G4LogicalVolume* BunkL_LV = new G4LogicalVolume(BunkL_SV, fMagnadense, "Bunk_LV");
  this->SetLogicalVolume(BunkL_LV);

  //Concrete
  G4VSolid* ConcBunkLFloorIn_SV = new G4Tubs("ConcBunkLFloorIn_SV",
                                           ConcBunk_RMin,ConcBunkLong_R1, ConcBunkFloorIn_Z,
                                             BunkL_phi0,BunkL_dphi);

  G4LogicalVolume* ConcBunkLFloorIn_LV = new G4LogicalVolume(ConcBunkLFloorIn_SV, fConcrete, "BunkInConc_LV");
  new G4PVPlacement(0,ConcBunkFloorIn_Ps,ConcBunkLFloorIn_LV,"ConcBunkLFloorIn_PV",BunkL_LV,false,0,1);

  //Air
  G4VSolid* BunkAirInL_SV = new G4Tubs("BunkAirInL_SV",
                                           ConcBunk_RMin,ConcBunkLong_R1, BunkAirIn_Z, BunkL_phi0,BunkL_dphi);
  G4LogicalVolume* BunkAirInL_LV = new G4LogicalVolume(BunkAirInL_SV, fAir, "BunkInAir_LV");
  new G4PVPlacement(0,BunkAirIn_Ps,BunkAirInL_LV,"BunkAirInL_PV",BunkL_LV,false,0,1);

}
