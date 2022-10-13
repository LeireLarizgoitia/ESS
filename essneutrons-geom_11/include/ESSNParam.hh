/// \file ESSNConstants.hh
/// \brief Definition of constants.

#ifndef ESSNConstants_h
#define ESSNConstants_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"


// Ntuple flag
const bool NTUPLE = false;

// Control file for the simulation set up
// Size of the world (cm)
const double worldX = 8000;
const double worldY = 8000;
const double worldZ = 8000;

// parameters to define the mesh
const int xCells = 31;
const int yCells = 31;
const int zCells = 5;
const int noCells = xCells*yCells*zCells;

// Weight window parameters
const int C_U=5;     //5
const int C_S=2;     //2
const int split=100; //100

// Pre-run flag
// Set true if running a pre simulation for the weight windows
// Set false if file is read for weight windows
const bool PRE = true;

// Scan flag
// Set true to run scan to get the geometry map
// PRE should also be set to true, choose the yPlane for the scan (cm)
const bool SCAN = false;
const double yPlane = 0.0;


//Size where the target, rooms, bunker are built
const double Lx_ConcBox = 2180*cm;
const double Ly_ConcBox = 4400*cm;
const double Lz_ConcBox = 200*cm;


const double TrSh1_Ztop = 184*cm;
const double Tr_center = 112.2*cm;  //112.5; //

const double  dist_TRoom_corner = 2200*cm;

const double ConcreteShield_R2 = 586*cm;

const double ConcBunk_RMax = 3000*cm;
const double ConcBunk_RMin = 600*cm;
const double ConcBunkLong_R1 = 2450*cm; // Long sector of BunkerL inner radii
const double ConcBunkLong_R2 = 2800*cm; // Long sector of BunkerL outer radii
const double ConcBunkShort_R1 = 1150*cm; // Short sector of Bunker inner radii
const double ConcBunkShort_R2 = 1500*cm; // Short sector of Bunker outer radii
const double ConcBase_Z = 50*cm/2;

const double BunkL_phi0 = 12*deg;
const double BunkL_dphi = 48*deg;
const double BunkLR_phi0 = 120*deg; // Right side

const double BunkS_phi0 = -60*deg;
const double BunkSR_phi0 = 168*deg; //BunkLR_phi0+BunkL_dphi;
const double BunkS_dphi = 72*deg;

// Axiliar Boxes to Subtract the coners of Trapeziodal rooms
const double hx_AuxBox = 5000*cm/2;
const double hy_AuxBox = 5000*cm/2;
const double hz_AuxBox = 5000*cm/2;

const double density_factor = 1; // scale factor

#endif
