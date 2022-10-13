/// \file ESSNPVolumeStore.hh
/// \brief Definition of the ESSNPVolumeStore class; adapted from
///        Geant4 example B02 B02PVolumeStore.hh

#ifndef ESSNPVolumeStore_h
#define ESSNPVolumeStore_h 1

#include "globals.hh"
#include <set>
#include "G4GeometryCell.hh"
#include "G4GeometryCellComp.hh"

typedef std::set< G4GeometryCell, G4GeometryCellComp > ESSNSetGeometryCell;

class ESSNPVolumeStore {
public:
  ESSNPVolumeStore();
  ~ESSNPVolumeStore();

  void AddPVolume(const G4GeometryCell &cell);
  const G4VPhysicalVolume *GetPVolume(const G4String &name) const;
  G4int Size();
  G4String GetPNames() const;

private:
  ESSNSetGeometryCell fSetGeometryCell;
};



#endif
