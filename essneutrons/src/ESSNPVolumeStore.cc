/// \file ESSNPVolumeStore.cc
/// \brief Implementation of the ESSNPVolumeStore class; adapted from
///        Geant4 example B02 B02PVolumeStore.ccs

#include "ESSNPVolumeStore.hh"
#include <sstream>

#include "G4VPhysicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNPVolumeStore::ESSNPVolumeStore(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ESSNPVolumeStore::~ESSNPVolumeStore(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNPVolumeStore::AddPVolume(const G4GeometryCell &cell){

  ESSNSetGeometryCell::iterator it =
    fSetGeometryCell.find(cell);
  if (it != fSetGeometryCell.end()) {
    G4cout << "ESSNPVolumeStore::AddPVolume: cell already stored"
           << G4endl;
    return;
  }

  fSetGeometryCell.insert(cell);


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const G4VPhysicalVolume *ESSNPVolumeStore::
GetPVolume(const G4String &name) const {
  const G4VPhysicalVolume *pvol = 0;
  for (ESSNSetGeometryCell::const_iterator it = fSetGeometryCell.begin();
       it != fSetGeometryCell.end(); ++it) {
    const G4VPhysicalVolume &vol = it->GetPhysicalVolume();
    if (vol.GetName() == name) {
      pvol =  &vol;
    }
  }
  if (!pvol) {
    G4cout << "ESSNPVolumeStore::GetPVolume: no physical volume named: "
           << name << ", found" << G4endl;
  }
  return pvol;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4String ESSNPVolumeStore::GetPNames() const {
  G4String NameString;
  for (ESSNSetGeometryCell::const_iterator it = fSetGeometryCell.begin();
       it != fSetGeometryCell.end(); ++it) {
    const G4VPhysicalVolume &vol = it->GetPhysicalVolume();
    std::ostringstream os;
    os << vol.GetName() << "_" << it->GetReplicaNumber()
       << "\n";
    G4String cellname = os.str();

    //    G4String cellname(vol.GetName());
    //    cellname += G4String("_");
    //    cellname += std::str(it->GetReplicaNumber());

    NameString += cellname;
  }
  return NameString;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4int ESSNPVolumeStore::Size() {
  return fSetGeometryCell.size();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
