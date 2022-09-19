#ifndef GroundRoomL_H
#define GroundRoomL_H

#include <G4ThreeVector.hh>
#include <G4LogicalVolume.hh>
#include <G4RotationMatrix.hh>

#include "G4SystemOfUnits.hh"
#include "ESSNParam.hh"

//#include "ESSNCompleteDetectorConstruction.hh"

class ESSN_GroundRoomL//: public G4VUserDetectorConstruction
{
public:
  /// Constructor
    ESSN_GroundRoomL();
  /// Destructor
  ~ESSN_GroundRoomL();
  /// Returns the dimensions

  G4ThreeVector Dimension() const;
  G4ThreeVector InDimension() const;

  G4ThreeVector Position() const;
  G4ThreeVector InPosition() const;

  G4ThreeVector SubCyl_Position() const;
  G4ThreeVector SubCyl_InPosition() const;
  G4double SubCyl_height() const;
  /// Get and Set logical volume
    G4LogicalVolume* GetLogicalVolume() const;
    void SetLogicalVolume(G4LogicalVolume* lv);
  /// Sets a sensitive detector associated to the
  void Construct();
private:
  G4LogicalVolume* logic_vol_;

  G4double	       dx_1TL = 2540*cm/2;
  G4double         dx_2TL = 0*cm;
  G4double         dz_TL = (400*cm+250*cm)/2;
  G4double         dy_TL = (sqrt(3))*dx_1TL/2; //(sqrt(3))*(dx_1TL-dy_small)/2 ;

  G4ThreeVector    Trd_dim_ = G4ThreeVector(dx_1TL,dx_2TL,dz_TL);

  /// TLeft center relative to world center
  G4double         dy0_TLeft = dist_TRoom_corner-dy_TL;
  G4double         dz0_TLeft = -(400*cm - dz_TL);
  G4ThreeVector    TLeft_Ps = G4ThreeVector(0,dy0_TLeft,dz0_TLeft);

  G4double         dx_1InTL = 2193.58*cm/2;
  G4double         dx_2InTL = 0*cm;
  G4double         dz_InTL = (300*cm+150*cm)/2;
  G4double         dy_InTL = (sqrt(3))*dx_1InTL/2;

  G4ThreeVector    InTrd_dim_ = G4ThreeVector(dx_1InTL,dx_2InTL,dz_InTL);

  /// Relative to world center
  G4double         dy0_InTL =  2100*cm-dy_InTL;
  G4double         dz0_InTL = -(300*cm-dz_InTL);
  /// Relative to TLeft
  G4double         dy0_InTLeft= dy0_TLeft - dy0_InTL;
  G4double         dz0_InTLeft= dz0_TLeft - dz0_InTL;
  G4ThreeVector    InTLeft_Ps = G4ThreeVector(0,-dz0_InTLeft,dy0_InTLeft);

  G4ThreeVector    SubCyl_Ps = G4ThreeVector(0,0,dy0_TLeft);
  G4ThreeVector    SubCylIn_Ps = G4ThreeVector(0,0,dy0_InTL);
  G4double         SubCyl_Z = 2000*cm;

  //ESSNCompleteDetectorConstruction Detector_;

};
inline G4ThreeVector    ESSN_GroundRoomL::Dimension() const { return Trd_dim_; }
inline G4ThreeVector    ESSN_GroundRoomL::InDimension() const { return InTrd_dim_; }

inline G4ThreeVector    ESSN_GroundRoomL::Position() const { return TLeft_Ps; }
inline G4ThreeVector    ESSN_GroundRoomL::InPosition() const { return InTLeft_Ps; }
inline G4ThreeVector    ESSN_GroundRoomL::SubCyl_Position() const { return SubCyl_Ps; }
inline G4ThreeVector    ESSN_GroundRoomL::SubCyl_InPosition() const { return SubCylIn_Ps; }
inline G4double         ESSN_GroundRoomL::SubCyl_height() const { return SubCyl_Z; }


inline G4LogicalVolume* ESSN_GroundRoomL::GetLogicalVolume() const { return logic_vol_; }
inline void             ESSN_GroundRoomL::SetLogicalVolume(G4LogicalVolume* lv){ logic_vol_ = lv; }

#endif
