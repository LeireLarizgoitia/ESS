#ifndef GroundRoomR_H
#define GroundRoomR_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4RotationMatrix.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

#include "ESSN_GroundRoomL.hh"

class ESSN_GroundRoomR//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
    ESSN_GroundRoomR();
  /// Destructor
  ~ESSN_GroundRoomR();
  /// Returns the dimensions
  G4ThreeVector Dimension() const;
  G4ThreeVector Position() const;

  /// Get and Set logical volume
    G4LogicalVolume* GetLogicalVolume() const;
    void SetLogicalVolume(G4LogicalVolume* lv);
  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  ESSN_GroundRoomL GroundRoom_;
  G4double         dx_1TL = GroundRoom_.Dimension()[0];
  G4double         dx_2TL = GroundRoom_.Dimension()[1];
  //Maria: compr n1
  G4double         dy_small = 0*cm;
  //Leire: G4double         dy_small = sqrt(3)*dx_2TL;
  G4double         dy_TL = (sqrt(3))*(dx_1TL-dy_small)/2 ;

  G4double         dz_InTL = GroundRoom_.InDimension()[2];

  G4double         dy0_TLeft = GroundRoom_.Position()[1];
  G4double         dz0_InTLeft = -GroundRoom_.InPosition()[1];

  G4ThreeVector    SubCyl_Ps = GroundRoom_.SubCyl_Position();
  G4ThreeVector    SubCylIn_Ps = GroundRoom_.SubCyl_InPosition();
  G4double         SubCyl_Z = GroundRoom_.SubCyl_height();

  //
  G4double         dz_TR1 = (400*cm+250*cm)/2;
  G4double         dz0_TRight1 = 250*cm - dz_TR1;
  G4ThreeVector    TRightG_Ps = G4ThreeVector(0,-dy0_TLeft,dz0_TRight1);

};

inline G4ThreeVector    ESSN_GroundRoomR::Position() const { return TRightG_Ps; }

inline G4LogicalVolume* ESSN_GroundRoomR::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_GroundRoomR::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
