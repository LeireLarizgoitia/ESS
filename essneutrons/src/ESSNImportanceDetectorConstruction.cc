/// \file ESSNImportanceDetectorConstruction.cc
/// \brief Implementation of the ESSNImportanceDetectorConstruction class; adapted from
///        Geant4 example B02 B02ImportanceDetectorConstruction.cc

#include "ESSNAnalysis.hh"
#include "ESSNParam.hh"
#include "globals.hh"
#include <sstream>

#include "ESSNImportanceDetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"

// For Primitive Scorers
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSNofCollision.hh"
#include "G4PSPopulation.hh"
#include "G4PSTrackCounter.hh"
#include "G4PSTrackLength.hh"

// for importance biasing
#include "G4IStore.hh"

// for weight window technique
#include "G4WeightWindowStore.hh"

#include "G4VisAttributes.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNImportanceDetectorConstruction::
ESSNImportanceDetectorConstruction(G4String worldName)
:G4VUserParallelWorld(worldName),fLogicalVolumeVector()
{
  //  Construct();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNImportanceDetectorConstruction::~ESSNImportanceDetectorConstruction()
{
  fLogicalVolumeVector.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNImportanceDetectorConstruction::Construct()
{
  G4cout << " constructing parallel world " << G4endl;

   G4Material* dummyMat  = 0;

  //GetWorld methods create a clone of the mass world to the parallel world (!)
  // via the transportation manager
  fGhostWorld = GetWorld();
  G4cout << " ESSNImportanceDetectorConstruction:: ghostWorldName = "
         << fGhostWorld->GetName() << G4endl;
  G4LogicalVolume* worldLogical = fGhostWorld->GetLogicalVolume();
  fLogicalVolumeVector.push_back(worldLogical);

  //  fPVolumeStore.AddPVolume(G4GeometryCell(*pWorldVolume, 0));
  //fPVolumeStore.AddPVolume(G4GeometryCell(*fGhostWorld, 0));

  G4double BoxX=worldX/xCells/2.0;
  G4double BoxY=worldY/yCells/2.0;
  G4double BoxZ=worldZ/zCells/2.0;

  G4Box *aShield = new G4Box("aShield_imp", BoxX*cm, BoxY*cm, BoxZ*cm);

  G4LogicalVolume *aShield_log_imp =
    new G4LogicalVolume(aShield, dummyMat, "aShield_log_imp");
  fLogicalVolumeVector.push_back(aShield_log_imp);

  // physical parallel cells
  G4String name = "none";
  G4int i = 1,  j = 1, k = 1, ijk=0;
  G4double startx = -worldX/2.0+BoxX;
  G4double starty = -worldY/2.0+BoxY;
  G4double startz = -worldZ/2.0+BoxZ;

  for (i=1; i<=xCells; i++) {
    for (j=1; j<=yCells; j++) {
      for (k=1; k<=zCells; k++) {
      	name = GetCellName(ijk);
      	G4double pos_x = startx + BoxX*(i-1)*2;
      	G4double pos_y = starty + BoxY*(j-1)*2;
      	G4double pos_z = startz + BoxZ*(k-1)*2;

      	G4VPhysicalVolume *pvol =
      	  new G4PVPlacement(0,
      			    G4ThreeVector(pos_x*cm, pos_y*cm, pos_z*cm),
      			    aShield_log_imp,
      			    name,
      			    worldLogical,
      			    false,
      			    ijk);
      	G4GeometryCell cell(*pvol, ijk);
      	fPVolumeStore.AddPVolume(cell);
      	ijk++;
      }
    }
  }

  // for the visualization
  G4VisAttributes* pShieldVis2 = new
    G4VisAttributes(G4Colour(1.0,0.0,0.0));
  pShieldVis2->SetForceWireframe(true);

  aShield_log_imp->SetVisAttributes(pShieldVis2);

  SetSensitive();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4VPhysicalVolume &ESSNImportanceDetectorConstruction::
GetPhysicalVolumeByName(const G4String& name) const {
  return *fPVolumeStore.GetPVolume(name);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String ESSNImportanceDetectorConstruction::ListPhysNamesAsG4String(){
  G4String names(fPVolumeStore.GetPNames());
  return names;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String ESSNImportanceDetectorConstruction::GetCellName(G4int i) {
  std::ostringstream os;
  os << "cell_";
  if (i<100000) {
    os << "0";
    if (i<10000) {
      os << "0";
      if (i<1000) {
	os << "0";
	if (i<100) {
	  os << "0";
	  if (i<10) {
	    os << "0";
	  }
	}
      }
    }
  }
  os << i;
  G4String name = os.str();
  return name;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4GeometryCell ESSNImportanceDetectorConstruction::GetGeometryCell(G4int i){
  G4String name(GetCellName(i));
  const G4VPhysicalVolume *p=0;
  p = fPVolumeStore.GetPVolume(name);
  if (p) {
    return G4GeometryCell(*p,0);
  }
  else {
    G4cout << "ESSNImportanceDetectorConstruction::GetGeometryCell: " << G4endl
           << " couldn't get G4GeometryCell" << G4endl;
    return G4GeometryCell(*fGhostWorld,-2);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume &ESSNImportanceDetectorConstruction::
GetWorldVolumeAddress() const{
   return *fGhostWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *ESSNImportanceDetectorConstruction::GetWorldVolume() {
  return fGhostWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNImportanceDetectorConstruction::SetSensitive(){

  //  -------------------------------------------------
  //   The collection names of defined Primitives are
  //   0       ConcreteSD/Collisions
  //   1       ConcreteSD/CollWeight
  //   2       ConcreteSD/Population
  //   3       ConcreteSD/TrackEnter
  //   4       ConcreteSD/SL
  //   5       ConcreteSD/SLW
  //   6       ConcreteSD/SLWE
  //   7       ConcreteSD/SLW_V
  //   8       ConcreteSD/SLWE_V
  //  -------------------------------------------------

  // moved to ConstructSD() for MT compliance

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void ESSNImportanceDetectorConstruction::ConstructSD()
{

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  //
  // Sensitive Detector Name
  G4String concreteSDname = "ConcreteSD";

  //------------------------
  // MultiFunctionalDetector
  //------------------------
  //
  // Define MultiFunctionalDetector with name.
  G4MultiFunctionalDetector* MFDet =
                         new G4MultiFunctionalDetector(concreteSDname);
  SDman->AddNewDetector( MFDet );                 // Register SD to SDManager


  G4String fltName,particleName;
  G4SDParticleFilter* neutronFilter =
      new G4SDParticleFilter(fltName="neutronFilter", particleName="neutron");

  MFDet->SetFilter(neutronFilter);


  for (std::vector<G4LogicalVolume *>::iterator it =
                                                fLogicalVolumeVector.begin();
       it != fLogicalVolumeVector.end(); it++){
    //      (*it)->SetSensitiveDetector(MFDet);
    SetSensitiveDetector((*it)->GetName(), MFDet);
  }

  G4String psName;
  G4PSNofCollision*   scorer0 = new G4PSNofCollision(psName="Collisions");
  MFDet->RegisterPrimitive(scorer0);

  G4PSPopulation*   scorer2 = new G4PSPopulation(psName="Population");
  MFDet->RegisterPrimitive(scorer2);

  G4PSTrackCounter* scorer3 =
                new G4PSTrackCounter(psName="TrackEnter",fCurrent_In);
  MFDet->RegisterPrimitive(scorer3);

  G4PSTrackLength* scorer4 = new G4PSTrackLength(psName="SL");
  MFDet->RegisterPrimitive(scorer4);

  G4PSTrackLength* scorer5 = new G4PSTrackLength(psName="SLW");
  scorer5->Weighted(true);
  MFDet->RegisterPrimitive(scorer5);

  G4PSTrackCounter* scorer39 = new G4PSTrackCounter(psName="WeightEnter",fCurrent_In);
  scorer39->Weighted(true);
  MFDet->RegisterPrimitive(scorer39);

  // Set the scorer corresponding to weighted energy to the corresponding histograms.
  G4int ijk = 0;
  for (int i=1; i<=xCells; i++) {
    for (int j=1; j<=yCells; j++) {
      for (int k=1; k<=zCells; k++) {
        scorer39->Plot(ijk,ijk);
        ijk++;
      }
    }
  }

  G4PSTrackCounter* scorer40 = new G4PSTrackCounter(psName="WeightEnterSq",fCurrent_In);
  scorer40->Weighted(true);
  MFDet->RegisterPrimitive(scorer40);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4VIStore* ESSNImportanceDetectorConstruction::CreateImportanceStore()
{


  G4cout << " ESSNImportanceDetectorConstruction:: Creating Importance Store "
         << G4endl;
  if (!fPVolumeStore.Size())
  {
    G4Exception("ESSNImportanceDetectorConstruction::CreateImportanceStore"
               ,"examplewwg_0001",RunMustBeAborted
               ,"no physical volumes created yet!");
  }

  // creating and filling the importance store

  //  G4IStore *istore = new G4IStore(*fWorldVolume);

  G4IStore *istore = G4IStore::GetInstance(GetName());

  G4double imp =1;

  // set importance values and create scorers
  G4int cell(1);
  for (cell=0; cell<noCells; cell++) {
    G4GeometryCell gCell = GetGeometryCell(cell);
    G4cout << " adding cell: " << cell
           << " replica: " << gCell.GetReplicaNumber()
           << " name: " << gCell.GetPhysicalVolume().GetName() << G4endl;
    imp = 1.0;

    G4cout << "Going to assign importance: " << imp << ", to volume: "
           << gCell.GetPhysicalVolume().GetName() << G4endl;
    istore->AddImportanceGeometryCell(imp, gCell.GetPhysicalVolume(), cell);
  }


  return istore;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VWeightWindowStore *ESSNImportanceDetectorConstruction::
CreateWeightWindowStore()
{

  // first read the Results file to get the flux information
  std::fstream fluxFile("Results.txt");    //Reading previous fluxes and finding its max
  G4double previousFlux[noCells] = {};
  G4double dummy;
  G4double maxFlux=0;
  G4double minFlux=INT_MAX;
  for (G4int i=0; i < noCells; i++) {
    fluxFile >> dummy;
    fluxFile >> previousFlux[i];
    fluxFile >> dummy;
    if(previousFlux[i] > maxFlux){
      maxFlux = previousFlux[i];
    }
    if(previousFlux[i] < minFlux && previousFlux[i]>0){
      minFlux = previousFlux[i];
    }
  }
  fluxFile.close();

  G4cout << " ESSNImportanceDetectorConstruction:: Creating Importance Store "
         << G4endl;
  if (!fPVolumeStore.Size())
  {
    G4Exception("ESSNImportanceDetectorConstruction::CreateWeightWindowStore"
               ,"examplewwg_0002",RunMustBeAborted
               ,"no physical volumes created yet!");
  }

  // creating and filling the importance store

  //  G4IStore *istore = new G4IStore(*fWorldVolume);

  G4WeightWindowStore *wwstore = G4WeightWindowStore::GetInstance(GetName());


  // create one energy region covering the energies of the problem
  //
  std::set<G4double, std::less<G4double> > enBounds;
  enBounds.insert(2 * GeV);
  wwstore->SetGeneralUpperEnergyBounds(enBounds);

  G4double lowerWeight =1;
  std::vector<G4double> lowerWeights;
  G4int cell(1);

  for (cell=0; cell<noCells; cell++) {
    G4GeometryCell gCell = GetGeometryCell(cell);
    G4cout << " adding cell: " << cell
           << " replica: " << gCell.GetReplicaNumber()
           << " name: " << gCell.GetPhysicalVolume().GetName() << G4endl;

    if(previousFlux[cell]==0){
      lowerWeight=(2.0/(C_U+1.))*minFlux/maxFlux;
    }
    if(previousFlux[cell]>0){
      lowerWeight=(2.0/(C_U+1.))*previousFlux[cell]/maxFlux;
    }

    G4cout << "Going to assign lower weight: " << lowerWeight
	   << ", to volume: "
	   << gCell.GetPhysicalVolume().GetName() << G4endl;
    lowerWeights.clear();
    lowerWeights.push_back(lowerWeight);
    wwstore->AddLowerWeights(G4GeometryCell(gCell.GetPhysicalVolume(),cell), lowerWeights);
  }



  return wwstore;

}
