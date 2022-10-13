/// \file ESSNImportanceDetectorConstruction.hh
/// \brief Definition of the ESSNImportanceDetectorConstruction class; adapted from
///        Geant4 example B02 B02ImportanceDetectorConstruction.hh

#ifndef ESSNImportanceDetectorConstruction_h
#define ESSNImportanceDetectorConstruction_h 1

#include "globals.hh"
#include <map>
#include <vector>
#include "G4GeometryCell.hh"
#include "ESSNPVolumeStore.hh"

#include "G4VUserParallelWorld.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4VIStore;
class G4VWeightWindowStore;



class ESSNImportanceDetectorConstruction : public G4VUserParallelWorld
{
public:
  ESSNImportanceDetectorConstruction(G4String worldName);
  virtual ~ESSNImportanceDetectorConstruction();

  const G4VPhysicalVolume &GetPhysicalVolumeByName(const G4String& name) const;
  G4VPhysicalVolume &GetWorldVolumeAddress() const;
  G4String ListPhysNamesAsG4String();
  G4String GetCellName(G4int i);
  G4GeometryCell GetGeometryCell(G4int i);

  G4VPhysicalVolume* GetWorldVolume();

  void SetSensitive();

  virtual void Construct();
  virtual void ConstructSD();

  G4VIStore* CreateImportanceStore();
    // create an importance store, caller is responsible for deleting it

  G4VWeightWindowStore *CreateWeightWindowStore();
    // create an weight window  store, caller is responsible for
    // deleting it


private:

  ESSNPVolumeStore fPVolumeStore;
  std::vector< G4LogicalVolume * > fLogicalVolumeVector;
  G4VPhysicalVolume* fGhostWorld;

};

#endif
