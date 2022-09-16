// ----------------------------------------------------------------------------
// ESSNMaterialConstruction.hh
//
// Definition of materials of common use.
//
// ----------------------------------------------------------------------------


#ifndef ESSNMaterialConstruction_h
#define ESSNMaterialConstruction_h 1

#include "globals.hh"

#include <vector>

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

class G4Material;

namespace materials
{
  G4Material* fPara_hydrogen();
  G4Material* fWater();
  G4Material* fHelium();
  G4Material* fAluminum();
  G4Material* fAluminum_20K();
  G4Material* fSS316L();
  G4Material* fSS316LH2O();
  G4Material* fBeryllium();
  G4Material* fTungsten();
  G4Material* carbonTest();
  G4Material* fVacuum();
  G4Material* fConcrete();
  G4Material* fAir();
  G4Material* fLead();
  G4Material* fConcrete50Fe();
  G4Material* fCarbonSteel();
  G4Material* fMagnadense();
  G4Material* fAL6061T6();

}

#endif
