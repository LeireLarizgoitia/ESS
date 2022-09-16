/// \file ESSNDetectorConstruction.hh
/// \brief Definition of the ESSNDetectorConstruction class; adapted from
///        Geant4 example B5 B5DetectorConstruction.hh

#ifndef ESSNDetectorConstruction_h
#define ESSNDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include <vector>

class G4VPhysicalVolume;
class G4Material;
class G4VisAttributes;
class G4GenericMessenger;

/// Detector construction

class ESSNDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    ESSNDetectorConstruction();
    virtual ~ESSNDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    void ConstructMaterials();

    inline G4double GetWorldSize()                 { return fWorldSize;        }
    inline void SetWorldSize(G4double l)           { fWorldSize = l;           }
    inline G4double GetTargetRadius()              { return fTargetRadius;     }
    inline void SetTargetRadius(G4double radius)   { fTargetRadius = radius;   }
    inline G4double GetTargetThickness()           { return fTargetThickness;  }
    inline void SetTargetThickness(G4double t)     { fTargetThickness = t;     }


  private:
    void DefineCommands();

    G4GenericMessenger* fMessenger;

    G4double fWorldSize;        // side length of the world
    G4double fTargetRadius;     // radius of the target
    G4double fTargetThickness;  // thickness of the target

    std::vector<G4VisAttributes*> fVisAttributes;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
