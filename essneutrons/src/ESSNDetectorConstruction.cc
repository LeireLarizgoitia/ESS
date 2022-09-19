/// \file ESSNDetectorConstruction.cc
/// \brief Implementation of the ESSNDetectorConstruction class; adapted from
///        Geant4 example B5 B5DetectorConstruction.cc

#include "ESSNDetectorConstruction.hh"
#include "ESSNParam.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UserLimits.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNDetectorConstruction::ESSNDetectorConstruction()
: G4VUserDetectorConstruction(),
  fMessenger(nullptr), fWorldSize(300.), fTargetRadius(100.),
  fTargetThickness(8.), fVisAttributes()

{

  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNDetectorConstruction::~ESSNDetectorConstruction()
{
  delete fMessenger;

  for (auto visAttributes: fVisAttributes) {
   delete visAttributes;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* ESSNDetectorConstruction::Construct()
{

  G4bool checkOverlaps = true;

  // Construct materials
  ConstructMaterials();
  G4Material* air = G4Material::GetMaterial("G4_Galactic");
  G4Material* mat_target = G4Material::GetMaterial("G4_W");

  // World
  G4Box*
  solidWorld = new G4Box("World",fWorldSize/2,fWorldSize/2,fWorldSize/2);

  G4LogicalVolume*
  logicWorld = new G4LogicalVolume(solidWorld,             // its solid
                                   air,                    // its material
                                   "World");               // its name
  G4VPhysicalVolume*
  physiWorld = new G4PVPlacement(0,                      // no rotation
                                 G4ThreeVector(),        // at (0,0,0)
                                 logicWorld,             // its logical volume
                                 "World",                // its name
                                 0,                      // its mother  volume
                                 false,                  // no boolean operation
                                 0,                      // copy number
                                 checkOverlaps);

  // The tungsten cylinder
  G4Tubs*
  solidTarget = new G4Tubs("tracker_tube", 0.,
                  fTargetRadius, fTargetThickness/2, 0., 360.*deg);

  G4LogicalVolume*
  logicTarget = new G4LogicalVolume(solidTarget, mat_target, "Target");
  new G4PVPlacement(0,
                    G4ThreeVector(),        // at (0,0,0)
                    logicTarget,            // its logical volume
                    "Target",               // its name
                    logicWorld,           // its mother  volume
                    false,                  // no boolean operation
                    0,                      // copy number
                    checkOverlaps);

  // visualization attributes ------------------------------------------------

  auto visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(true);
  logicWorld->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray
  logicTarget->SetVisAttributes(visAttributes);
  fVisAttributes.push_back(visAttributes);

  // return the world physical volume ----------------------------------------
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNDetectorConstruction::ConstructMaterials()
{
  auto nistManager = G4NistManager::Instance();

  // Air
  nistManager->FindOrBuildMaterial("G4_AIR");

  // Tungsten
  nistManager->FindOrBuildMaterial("G4_W");

  // Vacuum "Galactic"
  nistManager->FindOrBuildMaterial("G4_Galactic");

  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNDetectorConstruction::DefineCommands()
{
  // Define /ESSN/detector command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                       "/essn/detector/",
                                       "Detector control");

  // World size command
  auto& worldSizeCmd
   = fMessenger->DeclareMethodWithUnit("worldSize","cm",
                               &ESSNDetectorConstruction::SetWorldSize,
                               "Set size of world");
  worldSizeCmd.SetParameterName("l", true);
  worldSizeCmd.SetRange("l>=0.");
  worldSizeCmd.SetDefaultValue("300.");

  // Target radius command
  auto& targetRadiusCmd
   = fMessenger->DeclareMethodWithUnit("targetRadius","cm",
                               &ESSNDetectorConstruction::SetTargetRadius,
                               "Set radius of ESSN target");
  targetRadiusCmd.SetParameterName("radius", true);
  targetRadiusCmd.SetRange("radius>=0.");
  targetRadiusCmd.SetDefaultValue("100.");

  // Target thickness command
  auto& targetThicknessCmd
   = fMessenger->DeclareMethodWithUnit("targetThickness","cm",
                               &ESSNDetectorConstruction::SetTargetThickness,
                               "Set thickness of ESSN target");
  targetThicknessCmd.SetParameterName("t", true);
  targetThicknessCmd.SetRange("t>=0.");
  targetThicknessCmd.SetDefaultValue("8.");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
