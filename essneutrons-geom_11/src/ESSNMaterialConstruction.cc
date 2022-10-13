// ----------------------------------------------------------------------------
// ESSNMaterialConstruction.cc
//
// Definition of materials of common use.
//
// Build material functions
// ----------------------------------------------------------------------------

#include "globals.hh"

#include "G4SystemOfUnits.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "ESSNMaterialConstruction.hh"
#include "ESSNParam.hh"

//void ESSNMaterialConstruction::DefineMaterials()

namespace materials
{
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();

  // Vacuum
  G4Material* fVacuum()
    {
      //G4Material* mat =
      //  G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

      G4String name = "G4_Galactic";
      G4double atomicNumb_ = 1.;
      G4double massOfMole = 1.008*g/mole;
      G4double density = 1.e-25*g/cm3; // NO density_factor . The default minimal density: 1e-25g/cm3
      G4double temperature = 2.73*kelvin;
      G4double pressure = 3.e-18*pascal;

      G4Material* mat = new G4Material(name, atomicNumb_, massOfMole, density,
                                      kStateGas, temperature, pressure);

      return mat;
    }

  // Concrete
  G4Material* fConcrete()
    {
      //G4Material* mat =
      //  G4NistManager::Instance()->FindOrBuildMaterial("G4_CONCRETE");

      G4String name = "G4_CONCRETE";
      G4int ncomp = 7;
      G4double density = density_factor * 2.3*g/cm3;

      G4Material* mat = new G4Material(name, density, ncomp, kStateSolid);

      G4Element* Conc_H  = new G4Element("Hydrogen", "H", 1., 1.0079*g/mole);
      G4Element* Conc_O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
      G4Element* Conc_Na  = new G4Element("Sodium", "Na", 11, 22.989769*g/mole);
      G4Element* Conc_Al  = new G4Element("TS_Aluminium_Metal", "Al", 13,26.981*g/mole);
      G4Element* Conc_Si  = new G4Element("Silicon", "Si", 14., 28.09*g/mole);
      G4Element* Conc_Ca  = new G4Element("Calcium", "Ca",  20., 40.078*g/mole);
      G4Element* Conc_Fe  = new G4Element("TS_Iron_Metal", "Fe", 26., 55.85*g/mole);

      mat->AddElement(Conc_H,  0.010000);
      mat->AddElement(Conc_O,  0.532000);
      mat->AddElement(Conc_Na, 0.029000);
      mat->AddElement(Conc_Al, 0.034000);
      mat->AddElement(Conc_Si, 0.337000);
      mat->AddElement(Conc_Ca, 0.044000);
      mat->AddElement(Conc_Fe, 0.014000);

      return mat;
    }

  // Air
  G4Material* fAir()
    {
      //G4Material* mat =
      //  G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

      G4String name = "G4_AIR";
      G4int ncomp = 4;
      G4double density = density_factor * 0.001205*g/cm3;

      G4Material* mat = new G4Material(name, density, ncomp, kStateGas);

      G4Element* Air_C  = new G4Element("Carbon", "C",  6., 12.01*g/mole);
      G4Element* Air_N  = new G4Element("Nitrogen", "N", 7., 14.006*g/mole);
      G4Element* Air_O  = new G4Element("Oxygen", "O", 8., 16.00*g/mole);
      G4Element* Air_Ar  = new G4Element("Argon", "Ar", 18,  39.948*g/mole);

      mat->AddElement(Air_C,  0.000124);
      mat->AddElement(Air_N,  0.755268);
      mat->AddElement(Air_O,  0.231781);
      mat->AddElement(Air_Ar, 0.012827);

      return mat;
    }

  // Lead
  G4Material* fLead()
    {
      //G4Material* mat =
      //  G4NistManager::Instance()->FindOrBuildMaterial("G4_Pb");

    G4String name = "G4_Pb";
    G4double density = density_factor * 11.350000*g/cm3;
    G4Material* mat = new G4Material(name, density, 1, kStateSolid);

    G4Element* Pb  = new G4Element("Lead", "Pb", 82, 207.2*g/mole);
    mat->AddElement(Pb, 1.0);

      return mat;
    }

  // Helium
  G4Material* fHelium()
  {
    //G4Material* mat =
    //  G4NistManager::Instance()->FindOrBuildMaterial("G4_He");

    G4String name = "G4_He";
    G4double density = density_factor * 0.000166*g/cm3;
    G4Material* mat = new G4Material(name, density, 1, kStateGas);

    G4Element* He  = new G4Element("Helium", "He", 2, 4.002602*g/mole);
    mat->AddElement(He, 1.0);

    return mat;
    }

  // Para hydrogen
  G4Material* fPara_hydrogen()
   {
     G4String name = "fPara_hydrogen";

  //       G4Material* mat = G4Material::GetMaterial(name, false);
  //       if (mat == 0) {

      G4double density = density_factor * 7.00012e-2*g/cm3;
      G4Material* mat = new G4Material(name, density, 1, kStateLiquid, 20*kelvin);

      G4Element* Para_H  = new G4Element("TS_H_of_Para_Hydrogen", "H", 1,1.0008*g/mole);
      mat->AddElement(Para_H, 1.0);
  //       }
     return mat;
   }

  // Water
  G4Material* fWater()
   {
     G4String name = "fWater";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 1.000*g/cm3;
      G4Material* mat = new G4Material(name, density,2,kStateLiquid, 293.15*kelvin);

      G4Element* Water_H  = new G4Element("TS_H_of_Water" ,"H" , 1., 1.0079*g/mole);
      G4Element* Water_O  = new G4Element("Oxygen"        ,"O" , 8., 16.00*g/mole);
      mat->AddElement(Water_H, 2);
      mat->AddElement(Water_O, 1);
//       }
     return mat;
   }


  // Aluminum
  //m1008= nistManager->FindOrBuildMaterial("G4_Al");
  G4Material* fAluminum()
  {
     G4String name = "fAluminum";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 2.7*g/cm3;
      G4Material* mat = new G4Material(name, density, 1, kStateSolid, 293.15*kelvin);

      G4Element* e_Al  = new G4Element("TS_Aluminium_Metal", "Al", 13,26.981*g/mole);
      mat->AddElement(e_Al, 1.0);
//       }
     return mat;
   }

  G4Material* fAluminum_20K()
  {
     G4String name = "fAluminum_20K";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 2.7*g/cm3;
      G4Material* mat = new G4Material(name, density, 1, kStateSolid, 20*kelvin);

      G4Element* e_Al  = new G4Element("TS_Aluminium_Metal", "Al", 13,26.981*g/mole);
      mat->AddElement(e_Al, 1.0);
//       }
     return mat;
   }

  // Stainless steel 316L
  G4Material* fSS316L()
  {
     G4String name = "fSS316L";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 7.85*g/cm3;
      G4Material* mat = new G4Material(name, density,9,kStateSolid, 600*kelvin);

      G4Element* SS316L_C   = new G4Element("Carbon"        ,"C" , 6., 12.01*g/mole);
      G4Element* SS316L_Si  = new G4Element("Silicon"       ,"Si", 14., 28.09*g/mole);
      G4Element* SS316L_P   = new G4Element("Phosphorus"    ,"P" , 15., 30.974*g/mole);
      G4Element* SS316L_S   = new G4Element("Sulfur"        ,"S" , 16., 32.06*g/mole);
      G4Element* SS316L_Cr  = new G4Element("Chromium"      ,"Cr", 24., 52.00*g/mole);
      G4Element* SS316L_Mn  = new G4Element("Manganese"     ,"Mn", 25., 54.94*g/mole);
      G4Element* SS316L_Fe  = new G4Element("TS_Iron_Metal"          ,"Fe", 26., 55.85*g/mole);
      G4Element* SS316L_Ni  = new G4Element("Nickel"        ,"Ni", 28., 58.70*g/mole);
      G4Element* SS316L_Mo  = new G4Element("Molybdenum"    ,"Mo", 42., 95.95*g/mole);
      mat->AddElement(SS316L_C, 0.03*perCent);
      mat->AddElement(SS316L_Si,1*perCent);
      mat->AddElement(SS316L_P, 0.0045*perCent);
      mat->AddElement(SS316L_S, 0.03*perCent);
      mat->AddElement(SS316L_Cr, 17*perCent);
      mat->AddElement(SS316L_Mn, 2*perCent);
      mat->AddElement(SS316L_Fe, 65.395*perCent);
      mat->AddElement(SS316L_Ni, 12*perCent);
      mat->AddElement(SS316L_Mo, 2.5*perCent);
//       }
     return mat;
   }

  // Stainless steel 316LH2O
  G4Material* fSS316LH2O()
  {
     G4String name = "fSS316LH2O";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 7.0841*g/cm3;
      G4Material* mat = new G4Material(name, density,12,kStateSolid, 235*kelvin);

      G4Element* SS316LH2O_H  = new G4Element("Hydrogen" ,"H" , 1., 1.0079*g/mole);
      G4Element* SS316LH2O_C   = new G4Element("Carbon"        ,"C" , 6., 12.01*g/mole);
      G4Element* SS316LH2O_O  = new G4Element("Oxygen"        ,"O" , 8., 16.00*g/mole);
      G4Element* SS316LH2O_Si  = new G4Element("Silicon"       ,"Si", 14., 28.09*g/mole);
      G4Element* SS316LH2O_P   = new G4Element("Phosphorus"    ,"P" , 15., 30.974*g/mole);
      G4Element* SS316LH2O_S   = new G4Element("Sulfur"        ,"S" , 16., 32.06*g/mole);
      G4Element* SS316LH2O_Cr  = new G4Element("Chromium"      ,"Cr", 24., 52.00*g/mole);
      G4Element* SS316LH2O_Mn  = new G4Element("Manganese"     ,"Mn", 25., 54.94*g/mole);
      G4Element* SS316LH2O_Fe  = new G4Element("TS_Iron_Metal" ,"Fe", 26., 55.85*g/mole);
      G4Element* SS316LH2O_Co  = new G4Element("Cobalt"        ,"Co", 27., 58.933*g/mole);
      G4Element* SS316LH2O_Ni  = new G4Element("Nickel"        ,"Ni", 28., 58.70*g/mole);
      G4Element* SS316LH2O_Mo  = new G4Element("Molybdenum"    ,"Mo", 42., 95.95*g/mole);
      mat->AddElement(SS316LH2O_H,  7.82280e-02);
      mat->AddElement(SS316LH2O_C,  1.22919e-03);
      mat->AddElement(SS316LH2O_O, 3.91140e-02);
      mat->AddElement(SS316LH2O_Si,7.00885e-03);
      mat->AddElement(SS316LH2O_P, 3.17764e-04);
      mat->AddElement(SS316LH2O_S, 1.53476e-04);
      mat->AddElement(SS316LH2O_Cr,  1.60896e-01);
      mat->AddElement(SS316LH2O_Mn,  1.61238e-02);
      mat->AddElement(SS316LH2O_Fe, 5.79048e-01);
      mat->AddElement(SS316LH2O_Co,   2.50513e-04);
      mat->AddElement(SS316LH2O_Ni, 1.04807e-01);
      mat->AddElement(SS316LH2O_Mo, 1.28239e-02);
//       }
     return mat;
   }

  // Beryllium
  G4Material* fBeryllium()
  {
     G4String name = "fBeryllium";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 1.8480*g/cm3;
      G4Material* mat = new G4Material(name, density, 1, kStateSolid, 293.15*kelvin);

      G4Element* Be_Be = new G4Element("TS_Beryllium_Metal", "Be", 4.,9.012182*g/mole);

      mat->AddElement(Be_Be, 1.0);
//       }
     return mat;
   }

  // Tungsten
  //m1025 = nistManager->FindOrBuildMaterial("G4_W");
  G4Material* fTungsten()
  {
     G4String name = "fTungsten";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 19.25*g/cm3; // Original density 15.1*g/cm3; DON'T KNOW IF IT IS CORRECT
      G4Material* mat = new G4Material(name, density, 1, kStateSolid, 600*kelvin);

      G4Element* e_W = new G4Element("Tungsten", "W", 74.,183.84*g/mole);
      mat->AddElement(e_W, 1.0);
//       }
     return mat;
   }

  // Thermal
  G4Material* carbonTest()
  {
     G4String name = "carbonTest";

//       G4Material* mat = G4Material::GetMaterial(name, false);
//       if (mat == 0) {
      G4double density = density_factor * 2*g/cm3;
      G4Material* mat = new G4Material(name, density, 4, kStateSolid, 293.15*kelvin);

      G4Element* H_ND  = new G4Element("Hydrogen" ,"H" , 1., 1.0079*g/mole);
      G4Element* N_ND  = new G4Element("Nitrogen" ,"N" , 7., 14.006*g/mole);
      G4Element* O_ND  = new G4Element("Oxygen"   ,"O" , 8., 16.00*g/mole);
      G4Element* C_ND = new G4Element("TS_C_of_Graphite", "C", 6,12.0107*g/mole);
      mat->AddElement(H_ND, 0.5*perCent);
      mat->AddElement(N_ND, 2.5*perCent);
      mat->AddElement(O_ND, 2.5*perCent);
      mat->AddElement(C_ND,/* 100*perCent*/94.5*perCent);
//       }
     return mat;
   }

  // Concrete + 50% Fe
  G4Material* fConcrete50Fe()
  {
       G4String name = "fConcrete50Fe";
       G4double density = density_factor * 2.3*g/cm3;
       G4Material* mat = new G4Material(name, density, 8 , kStateSolid, 135.2*kelvin);

       //H
       G4Element* fConcrete50Fe_H  = new G4Element("Hydrogen" ,"H" , 1., 1.0079*g/mole);
       //O
       G4Element* fConcrete50Fe_O   = new G4Element("Oxygen"  ,"O" , 8., 15.995*g/mole);
       //Al
       G4Element* fConcrete50Fe_Al  = new G4Element("TS_Aluminium_Metal", "Al", 13,26.981*g/mole);
       //Si
       G4Isotope* isoSi_28 = new G4Isotope("Si28",14,28, 27.977*g/mole);
       G4Isotope* isoSi_29 = new G4Isotope("Si29",14,29, 28.976*g/mole);
       G4Isotope* isoSi_30 = new G4Isotope("Si30",14,30, 29.974*g/mole);

       G4Element* fConcrete50Fe_Si = new G4Element("Silicon", "Si", 3);
       fConcrete50Fe_Si->AddIsotope(isoSi_28, 92.22*perCent);
       fConcrete50Fe_Si->AddIsotope(isoSi_29, 4.68*perCent);
       fConcrete50Fe_Si->AddIsotope(isoSi_30, 3.10*perCent);

       //S
       G4Element* fConcrete50Fe_S   = new G4Element("Sulfur"    ,"S" , 16., 32.06*g/mole);
       //K
       G4Element* fConcrete50Fe_K   = new G4Element("Potasium"  ,"K" , 19., 39.0983*g/mole);
       //Ca
       G4Isotope* isoCa_40 = new G4Isotope("Ca40",20,40, 39.963*g/mole);
       G4Isotope* isoCa_44 = new G4Isotope("Ca44",20,44, 43.955*g/mole);

       G4Element* fConcrete50Fe_Ca   = new G4Element("Calcium"  ,"Ca" , 2);
       fConcrete50Fe_Ca->AddIsotope(isoCa_40, 97.9*perCent);
       fConcrete50Fe_Ca->AddIsotope(isoCa_44, 2.1*perCent);

       //Fe
       G4Isotope* isoFe_54 = new G4Isotope("Fe54",26,54, 53.939*g/mole);
       G4Isotope* isoFe_56 = new G4Isotope("Fe56",26,56, 55.935*g/mole);
       G4Isotope* isoFe_57 = new G4Isotope("Fe57",26,57, 56.935*g/mole);
       G4Isotope* isoFe_58 = new G4Isotope("Fe58",26,58, 57.933*g/mole);

       G4Element* fConcrete50Fe_Fe   = new G4Element("Iron"  ,"Fe" , 4);
       fConcrete50Fe_Fe->AddIsotope(isoFe_54, 5.91*perCent);
       fConcrete50Fe_Fe->AddIsotope(isoFe_56, 91.70*perCent);
       fConcrete50Fe_Fe->AddIsotope(isoFe_57, 2.11*perCent);
       fConcrete50Fe_Fe->AddIsotope(isoFe_58, 0.28*perCent);

       mat->AddElement(fConcrete50Fe_H, 3.2611423*perCent);
       mat->AddElement(fConcrete50Fe_O, 23.0885184*perCent);
       mat->AddElement(fConcrete50Fe_Al, 0.6962007*perCent);
       mat->AddElement(fConcrete50Fe_Si, 4.2952835*perCent);
       mat->AddElement(fConcrete50Fe_S, 0.2413935*perCent);
       mat->AddElement(fConcrete50Fe_K, 0.3734146*perCent);
       mat->AddElement(fConcrete50Fe_Ca, 9.4067473*perCent);
       mat->AddElement(fConcrete50Fe_Fe, 58.6372997*perCent);

    return mat;
  }

  // STANDARD GRADE CARBON STEEL AISI 1005
  G4Material* fCarbonSteel()
  {
    G4String name = "fCarbonSteel";
    G4double density = density_factor * 7.872*g/cm3;
    G4Material* mat = new G4Material(name, density, 3,kStateSolid);

    //C
    G4Element* AISI1005_C   = new G4Element("Carbon","C" , 6., 12.01*g/mole);
    //Mn
    G4Element* AISI1005_Mn  = new G4Element("Manganese","Mn", 25., 54.94*g/mole);
    //Fe
    G4Isotope* isoFe_54 = new G4Isotope("Fe54",26,54, 53.939*g/mole);
    G4Isotope* isoFe_56 = new G4Isotope("Fe56",26,56, 55.935*g/mole);
    G4Isotope* isoFe_57 = new G4Isotope("Fe57",26,57, 56.935*g/mole);
    G4Isotope* isoFe_58 = new G4Isotope("Fe58",26,58, 57.933*g/mole);

    G4Element* AISI1005_Fe  = new G4Element("Iron","Fe", 4);
    AISI1005_Fe->AddIsotope(isoFe_54,5.85*perCent);
    AISI1005_Fe->AddIsotope(isoFe_56,91.74*perCent);
    AISI1005_Fe->AddIsotope(isoFe_57,2.12*perCent);
    AISI1005_Fe->AddIsotope(isoFe_58,0.29*perCent);

    mat->AddElement(AISI1005_C, 0.2781585*perCent);
    mat->AddElement(AISI1005_Mn, 0.3547362*perCent);
    mat->AddElement(AISI1005_Fe, 99.3671053*perCent);

     return mat;
   }

  // MAGNADENSE 8
  G4Material* fMagnadense()
  {

    G4String name = "fMagnadense";
    G4double density = density_factor * 4.0*g/cm3;
    G4Material* mat = new G4Material(name, density, 8 ,kStateSolid);

    //H
    G4Element* MAGNADENSE8_H  = new G4Element("Hydrogen" ,"H" , 1., 1.0079*g/mole);

    //O
    G4Isotope* isoO_16 = new G4Isotope("O16",8,16, 15.995*g/mole);
    G4Isotope* isoO_18 = new G4Isotope("O18",8,18, 17.999*g/mole);

    G4Element*  MAGNADENSE8_O = new G4Element("Oxygen","O", 2);
    MAGNADENSE8_O->AddIsotope(isoO_16,99.79*perCent);
    MAGNADENSE8_O->AddIsotope(isoO_18,0.21*perCent);

    //Na
    G4Element* MAGNADENSE8_Na  = new G4Element("Sodium", "Na", 11, 22.989769*g/mole);

    //Al
    G4Element* MAGNADENSE8_Al  = new G4Element("TS_Aluminium_Metal", "Al", 13,26.981*g/mole);

    //Si
    G4Isotope* isoSi_28 = new G4Isotope("Si28",14,28, 27.977*g/mole);
    G4Isotope* isoSi_29 = new G4Isotope("Si29",14,29, 28.976*g/mole);
    G4Isotope* isoSi_30 = new G4Isotope("Si30",14,30, 29.974*g/mole);

    G4Element* MAGNADENSE8_Si  = new G4Element("Silicon"   ,"Si", 3);
    MAGNADENSE8_Si->AddIsotope(isoSi_28,92.22*perCent);
    MAGNADENSE8_Si->AddIsotope(isoSi_29,4.68*perCent);
    MAGNADENSE8_Si->AddIsotope(isoSi_30,3.10*perCent);

    //P
    G4Element* MAGNADENSE8_P   = new G4Element("Phosphorus"    ,"P" , 15., 30.974*g/mole);

    //Ca
    G4Element* MAGNADENSE8_Ca   = new G4Element("Calcium"  ,"Ca" , 20., 40.078*g/mole);

    //Fe
    G4Isotope* isoFe_54 = new G4Isotope("Fe54",26,54, 53.939*g/mole);
    G4Isotope* isoFe_56 = new G4Isotope("Fe56",26,56, 55.935*g/mole);
    G4Isotope* isoFe_57 = new G4Isotope("Fe57",26,57, 56.935*g/mole);

    G4Element* MAGNADENSE8_Fe  = new G4Element("Iron"          ,"Fe", 3);
    MAGNADENSE8_Fe->AddIsotope(isoFe_54,5.86*perCent);
    MAGNADENSE8_Fe->AddIsotope(isoFe_56,92.01*perCent);
    MAGNADENSE8_Fe->AddIsotope(isoFe_57,2.13*perCent);

    mat->AddElement(MAGNADENSE8_H, 13.4669044*perCent);
    mat->AddElement(MAGNADENSE8_O, 53.0647783*perCent);
    mat->AddElement(MAGNADENSE8_Na, 0.5156045*perCent);
    mat->AddElement(MAGNADENSE8_Al, 0.6553056*perCent);
    mat->AddElement(MAGNADENSE8_Si, 4.2613981*perCent);
    mat->AddElement(MAGNADENSE8_P, 0.4125101*perCent);
    mat->AddElement(MAGNADENSE8_Ca, 1.2721593*perCent);
    mat->AddElement(MAGNADENSE8_Fe, 26.3513397*perCent);

    return mat;
  }

  G4Material* fAL6061T6()
  {
  	G4String name = "fAL6061T6";
  	G4double density = 2.70*g/cm3;
  	//G4double Temperature = 600*kelvin; //TEMPERATURA NO ESTA BIEN
  	G4Material* mat = new G4Material(name, density, 4 ,kStateSolid );

  	//Mg
  	G4Isotope* isoMg_24 = new G4Isotope("Mg24",12,24, 23.985*g/mole);
  	G4Isotope* isoMg_25 = new G4Isotope("Mg25",12,25, 24.985*g/mole);
  	G4Isotope* isoMg_26 = new G4Isotope("Mg26",12,26, 25.983*g/mole);

  	G4Element* m1360_Mg = new G4Element("Magnesium"   ,"Mg", 3);;
  	m1360_Mg->AddIsotope(isoMg_24,78.99*perCent);
  	m1360_Mg->AddIsotope(isoMg_25,10.00*perCent);
  	m1360_Mg->AddIsotope(isoMg_26,11.01*perCent);

  	//Al
  	G4Element* m1360_Al = new G4Element("Aluminium"   ,"Al", 13, 26.981*g/mole);

  	//Si
  	G4Element* m1360_Si = new G4Element("Silicon"   ,"Si", 14, 27.977*g/mole);

  	//Fe
  	G4Element* m1360_Fe = new G4Element("Iron"   ,"Fe", 26, 55.935*g/mole);

  	mat->AddElement(m1360_Mg, 1.1155952*perCent);
      	mat->AddElement(m1360_Al, 98.1942722*perCent);
      	mat->AddElement(m1360_Si, 0.5342094*perCent);
      	mat->AddElement(m1360_Fe, 0.1559232*perCent);

    return mat;
  }



//    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}
