/// \file ESSNCompleteDetectorConstruction.hh
/// \brief Definition of the ESSNCompleteDetectorConstruction class; adapted from
///        EDDetectorConstruction class by Mostafa

#ifndef ESSNCompleteDetectorConstruction_h
#define ESSNCompleteDetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include <vector>
#include <G4ThreeVector.hh>
#include <G4Box.hh>

class G4VPhysicalVolume;
//class G4Material;

/// Detector construction

class ESSNCompleteDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    /// Constructor
    ESSNCompleteDetectorConstruction();

    /// Destructor
    virtual ~ESSNCompleteDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    //void DefineMaterials();

    //G4ThreeVector AuxBox1_Position() const;
    //G4ThreeVector AuxBox2_Position() const;

    //G4Box* AuxBox_Solid();

//private:
    //G4ThreeVector AuxBox1_Ps;
    //G4ThreeVector AuxBox2_Ps;

    //G4Box* AuxBoxSV;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//inline G4ThreeVector    ESSNCompleteDetectorConstruction::AuxBox1_Position() const { return AuxBox1_Ps; }
//inline G4ThreeVector    ESSNCompleteDetectorConstruction::AuxBox2_Position() const { return AuxBox2_Ps; }
//inline G4Box*            ESSNCompleteDetectorConstruction::AuxBox_Solid() { return AuxBoxSV; }


#endif
