//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PhysicalConstants.hh"  //twopi
#include "G4LogicalSkinSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4GenericTrap.hh"
#include "G4UserLimits.hh"
#include "Run.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),fScoringVolume1(nullptr),fScoringVolume2(nullptr)
{
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
     delete vacum;
     delete air;
     delete pb;
     delete cu;
     delete cuLess;
     delete pbMore;
     delete kapton;
     delete kaptonLess;
     delete gasMixture;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  G4cout << "Defining the materials" << G4endl;
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();
  // Build materials
  vacum   = nistManager->FindOrBuildMaterial("G4_Galactic");
  air   = nistManager->FindOrBuildMaterial("G4_AIR");
  pb   = nistManager->FindOrBuildMaterial("G4_Pb");
  fe   = nistManager->FindOrBuildMaterial("G4_Fe");
  w   = nistManager->FindOrBuildMaterial("G4_W");
  cu   = nistManager->FindOrBuildMaterial("G4_Cu");

  G4cout<<"define high density Pb"<<G4endl;
  // define high density Pb
  G4int nComponent;
  G4double fracmass=1.0;
  pbMore   = new G4Material("pbMore",11.34*1*g/cm3,nComponent=1);
  pbMore -> AddMaterial(pb,fracmass);

  G4cout<<"define low density Cu"<<G4endl;
  // define low density Cu
  G4double ratio_outter = 0.77790212; // 1-(3600*pi)/(29400*sqrt(3))
  G4double density = ratio_outter * 8.94 * g/cm3;
  cuLess   = new G4Material("cuLess",density,nComponent=1);
  cuLess -> AddMaterial(cu,fracmass);

  G4cout<<"define Kapton"<<G4endl;
  // Define Kapton material
  density = 1.42 * g/cm3; // Density of Kapton
  kapton = new G4Material("Kapton", density, 3);
  kapton->AddElement(nistManager->FindOrBuildElement("H"), 0.0273);
  kapton->AddElement(nistManager->FindOrBuildElement("C"), 0.7213);
  kapton->AddElement(nistManager->FindOrBuildElement("O"), 0.2514);

  G4double ratio_inner = 0.81183374; // 1-(3600*pi+2500*pi)/2/(29400*sqrt(3))
  density = ratio_inner * 1.42 * g/cm3; // Density of Kapton
  kaptonLess = new G4Material("KaptonLess", density, 3);
  kaptonLess->AddElement(nistManager->FindOrBuildElement("H"), 0.0273);
  kaptonLess->AddElement(nistManager->FindOrBuildElement("C"), 0.7213);
  kaptonLess->AddElement(nistManager->FindOrBuildElement("O"), 0.2514);

  G4cout<<"define mixgass"<<G4endl;
  // Define 70%Ar and 30%CO2 gas mixture
  density = 1.822 * mg/cm3; // Density of the gas mixture
  gasMixture = new G4Material("ArCO2GasMixture", density, 3);
  gasMixture->AddElement(nistManager->FindOrBuildElement("Ar"), 0.7);
  gasMixture->AddElement(nistManager->FindOrBuildElement("C"), 0.3/3);
  gasMixture->AddElement(nistManager->FindOrBuildElement("O"), 0.3/3*2);

  G4cout<<"define detector material"<<G4endl;
  // Define detector material
  Gem_driftcathode_Mat = cu;
  Gem_foil_inner_Mat = kaptonLess;
  Gem_foil_outter_Mat = cuLess;
  Gem_readout_Mat = cu;
  Gem_Mat = gasMixture;
  world_Mat = air;
  //world_Mat = vacum;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineConstants()
{
  pi=CLHEP::pi;

  Rcore=25*mm;
  Rmin=0;
  Rmax=0;
  Dz=1*m;
  Sphi=0;
  Dphi=2*pi;

  Gem_foil_outter_h=5*um;
  Gem_foil_inner_h=50*um;
  Gem_driftcathode_h=0.1*mm;
  Gem_readout_h=0.2*mm;
  Gem_gap1 = 4.8*mm;
  Gem_gap2 = 2*mm;

  experimentalHall_x=1.1*m;
  experimentalHall_y=1.1*m;
  experimentalHall_z=1.1*m;
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::DefineVolumes()
{

    /*******************************
   * Gem 1 drift cathode      *
   *******************************/
  Rmin=Rcore;
  Rmax=Rmin+Gem_driftcathode_h;
  G4VSolid* Gem_1_driftcathode_tub
    = new G4Tubs("Gem_1_driftcathode_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_driftcathode Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_driftcathode_Log 
    = new G4LogicalVolume(Gem_1_driftcathode_tub,
			  Gem_driftcathode_Mat,
                          "Gem_1_driftcathode_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_driftcathode_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 1 o1      *
   *******************************/
  Rmin=Rmax+Gem_gap1;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_1_foil_1_o1_tub
    = new G4Tubs("Gem_1_foil_1_o1_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_1_o1 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_1_o1_Log 
    = new G4LogicalVolume(Gem_1_foil_1_o1_tub,
			  Gem_foil_outter_Mat,
                          "Gem_1_foil_1_o1_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_1_o1_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 1 i     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_inner_h;
  G4VSolid* Gem_1_foil_1_i_tub
    = new G4Tubs("Gem_1_foil_1_i_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_1_i Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_1_i_Log 
    = new G4LogicalVolume(Gem_1_foil_1_i_tub,
			  Gem_foil_inner_Mat,
                          "Gem_1_foil_1_i_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_1_i_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 1 o2     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_1_foil_1_o2_tub
    = new G4Tubs("Gem_1_foil_1_o2_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_1_o2 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_1_o2_Log 
    = new G4LogicalVolume(Gem_1_foil_1_o2_tub,
			  Gem_foil_outter_Mat,
                          "Gem_1_foil_1_o2_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_1_o2_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 2 o1      *
   *******************************/
  Rmin=Rmax+Gem_gap2;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_1_foil_2_o1_tub
    = new G4Tubs("Gem_1_foil_2_o1_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_2_o1 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_2_o1_Log 
    = new G4LogicalVolume(Gem_1_foil_2_o1_tub,
			  Gem_foil_outter_Mat,
                          "Gem_1_foil_2_o1_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_2_o1_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 2 i     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_inner_h;
  G4VSolid* Gem_1_foil_2_i_tub
    = new G4Tubs("Gem_1_foil_2_i_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_2_i Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_2_i_Log 
    = new G4LogicalVolume(Gem_1_foil_2_i_tub,
			  Gem_foil_inner_Mat,
                          "Gem_1_foil_2_i_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_2_i_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 2 o2     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_1_foil_2_o2_tub
    = new G4Tubs("Gem_1_foil_2_o2_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_2_o2 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_2_o2_Log 
    = new G4LogicalVolume(Gem_1_foil_2_o2_tub,
			  Gem_foil_outter_Mat,
                          "Gem_1_foil_2_o2_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_2_o2_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 3 o1      *
   *******************************/
  Rmin=Rmax+Gem_gap2;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_1_foil_3_o1_tub
    = new G4Tubs("Gem_1_foil_3_o1_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_3_o1 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_3_o1_Log 
    = new G4LogicalVolume(Gem_1_foil_3_o1_tub,
			  Gem_foil_outter_Mat,
                          "Gem_1_foil_3_o1_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_3_o1_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 3 i     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_inner_h;
  G4VSolid* Gem_1_foil_3_i_tub
    = new G4Tubs("Gem_1_foil_3_i_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_3_i Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_3_i_Log 
    = new G4LogicalVolume(Gem_1_foil_3_i_tub,
			  Gem_foil_inner_Mat,
                          "Gem_1_foil_3_i_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_3_i_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 foil 3 o2     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_1_foil_3_o2_tub
    = new G4Tubs("Gem_1_foil_3_o2_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_foil_3_o2 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_foil_3_o2_Log 
    = new G4LogicalVolume(Gem_1_foil_3_o2_tub,
			  Gem_foil_outter_Mat,
                          "Gem_1_foil_3_o2_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_foil_3_o2_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 1 Readout       *
   *******************************/
  Rmin=Rmax+Gem_gap2;
  Rmax=Rmin+Gem_readout_h;
  G4VSolid* Gem_1_readout_tub
    = new G4Tubs("Gem_1_readout_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_1_readout Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_1_readout_Log 
    = new G4LogicalVolume(Gem_1_readout_tub,
			  Gem_readout_Mat,
                          "Gem_1_readout_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_1_readout_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 drift cathode 1      *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_driftcathode_h;
  G4VSolid* Gem_2_driftcathode_tub
    = new G4Tubs("Gem_2_driftcathode_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_driftcathode Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_driftcathode_Log 
    = new G4LogicalVolume(Gem_2_driftcathode_tub,
			  Gem_driftcathode_Mat,
                          "Gem_2_driftcathode_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_driftcathode_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 1 o1      *
   *******************************/
  Rmin=Rmax+Gem_gap1;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_2_foil_1_o1_tub
    = new G4Tubs("Gem_2_foil_1_o1_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_1_o1 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_1_o1_Log 
    = new G4LogicalVolume(Gem_2_foil_1_o1_tub,
			  Gem_foil_outter_Mat,
                          "Gem_2_foil_1_o1_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_1_o1_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 1 i     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_inner_h;
  G4VSolid* Gem_2_foil_1_i_tub
    = new G4Tubs("Gem_2_foil_1_i_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_1_i Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_1_i_Log 
    = new G4LogicalVolume(Gem_2_foil_1_i_tub,
			  Gem_foil_inner_Mat,
                          "Gem_2_foil_1_i_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_1_i_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 1 o2     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_2_foil_1_o2_tub
    = new G4Tubs("Gem_2_foil_1_o2_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_1_o2 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_1_o2_Log 
    = new G4LogicalVolume(Gem_2_foil_1_o2_tub,
			  Gem_foil_outter_Mat,
                          "Gem_2_foil_1_o2_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_1_o2_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 2 o1      *
   *******************************/
  Rmin=Rmax+Gem_gap2;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_2_foil_2_o1_tub
    = new G4Tubs("Gem_2_foil_2_o1_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_2_o1 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_2_o1_Log 
    = new G4LogicalVolume(Gem_2_foil_2_o1_tub,
			  Gem_foil_outter_Mat,
                          "Gem_2_foil_2_o1_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_2_o1_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 2 i     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_inner_h;
  G4VSolid* Gem_2_foil_2_i_tub
    = new G4Tubs("Gem_2_foil_2_i_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_2_i Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_2_i_Log 
    = new G4LogicalVolume(Gem_2_foil_2_i_tub,
			  Gem_foil_inner_Mat,
                          "Gem_2_foil_2_i_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_2_i_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 2 o2     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_2_foil_2_o2_tub
    = new G4Tubs("Gem_2_foil_2_o2_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_2_o2 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_2_o2_Log 
    = new G4LogicalVolume(Gem_2_foil_2_o2_tub,
			  Gem_foil_outter_Mat,
                          "Gem_2_foil_2_o2_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_2_o2_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 3 o1      *
   *******************************/
  Rmin=Rmax+Gem_gap2;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_2_foil_3_o1_tub
    = new G4Tubs("Gem_2_foil_3_o1_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_3_o1 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_3_o1_Log 
    = new G4LogicalVolume(Gem_2_foil_3_o1_tub,
			  Gem_foil_outter_Mat,
                          "Gem_2_foil_3_o1_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_3_o1_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 3 i     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_inner_h;
  G4VSolid* Gem_2_foil_3_i_tub
    = new G4Tubs("Gem_2_foil_3_i_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_3_i Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_3_i_Log 
    = new G4LogicalVolume(Gem_2_foil_3_i_tub,
			  Gem_foil_inner_Mat,
                          "Gem_2_foil_3_i_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_3_i_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 foil 3 o2     *
   *******************************/
  Rmin=Rmax;
  Rmax=Rmin+Gem_foil_outter_h;
  G4VSolid* Gem_2_foil_3_o2_tub
    = new G4Tubs("Gem_2_foil_3_o2_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_foil_3_o2 Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_foil_3_o2_Log 
    = new G4LogicalVolume(Gem_2_foil_3_o2_tub,
			  Gem_foil_outter_Mat,
                          "Gem_2_foil_3_o2_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_foil_3_o2_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem 2 Readout       *
   *******************************/
  Rmin=Rmax+Gem_gap2;
  Rmax=Rmin+Gem_readout_h;
  G4VSolid* Gem_2_readout_tub
    = new G4Tubs("Gem_2_readout_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem_2_readout Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_2_readout_Log 
    = new G4LogicalVolume(Gem_2_readout_tub,
			  Gem_readout_Mat,
                          "Gem_2_readout_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_2_readout_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

    /*******************************
   * Gem       *
   *******************************/
  Rmin=Rcore;
  G4VSolid* Gem_tub
    = new G4Tubs("Gem_tub", Rmin, Rmax, Dz/2, Sphi, Dphi);
  std::cout<<"Gem Rmin = "<<Rmin<<" , Rmax = "<<Rmax<<std::endl;

  G4LogicalVolume* Gem_Log 
    = new G4LogicalVolume(Gem_tub,
			  Gem_Mat,
                          "Gem_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits
  Gem_Log->SetVisAttributes(G4VisAttributes::GetInvisible());

  // put Gem component in Gem
  G4VPhysicalVolume* Gem_1_driftcathode_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_driftcathode_Log,             //its logical volume
                    "Gem_1_driftcathode_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_1_o1_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_1_o1_Log,             //its logical volume
                    "Gem_1_foil_1_o1_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_1_i_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_1_i_Log,             //its logical volume
                    "Gem_1_foil_1_i_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_1_o2_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_1_o2_Log,             //its logical volume
                    "Gem_1_foil_1_o2_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_2_o1_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_2_o1_Log,             //its logical volume
                    "Gem_1_foil_2_o1_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_2_i_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_2_i_Log,             //its logical volume
                    "Gem_1_foil_2_i_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_2_o2_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_2_o2_Log,             //its logical volume
                    "Gem_1_foil_2_o2_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_3_o1_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_3_o1_Log,             //its logical volume
                    "Gem_1_foil_3_o1_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_3_i_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_3_i_Log,             //its logical volume
                    "Gem_1_foil_3_i_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_foil_3_o2_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_foil_3_o2_Log,             //its logical volume
                    "Gem_1_foil_3_o2_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_1_readout_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_1_readout_Log,             //its logical volume
                    "Gem_1_readout_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_driftcathode_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_driftcathode_Log,             //its logical volume
                    "Gem_2_driftcathode_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_1_o1_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_1_o1_Log,             //its logical volume
                    "Gem_2_foil_1_o1_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_1_i_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_1_i_Log,             //its logical volume
                    "Gem_2_foil_1_i_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_1_o2_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_1_o2_Log,             //its logical volume
                    "Gem_2_foil_1_o2_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_2_o1_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_2_o1_Log,             //its logical volume
                    "Gem_2_foil_2_o1_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_2_i_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_2_i_Log,             //its logical volume
                    "Gem_2_foil_2_i_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_2_o2_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_2_o2_Log,             //its logical volume
                    "Gem_2_foil_2_o2_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_3_o1_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_3_o1_Log,             //its logical volume
                    "Gem_2_foil_3_o1_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_3_i_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_3_i_Log,             //its logical volume
                    "Gem_2_foil_3_i_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_foil_3_o2_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_foil_3_o2_Log,             //its logical volume
                    "Gem_2_foil_3_o2_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

  G4VPhysicalVolume* Gem_2_readout_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),                    //at position
                    Gem_2_readout_Log,             //its logical volume
                    "Gem_2_readout_Phys",                //its name
                    Gem_Log,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

    /*******************************
   * The Experimental Hall       *
   *******************************/

  G4VSolid* experimentalHall_box 
    = new G4Box("expHall_box",             // World Volume
                experimentalHall_x/2,        // x size
                experimentalHall_y/2,        // y size
                experimentalHall_z/2);       // z size
  
  G4LogicalVolume* experimentalHallLog 
    = new G4LogicalVolume(experimentalHall_box,
			  world_Mat,
                          "expHallLog",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits

  G4VPhysicalVolume* experimentalHallPhys 
    = new G4PVPlacement(0,
                        G4ThreeVector(),   //at (0,0,0)
                        "expHall",
                        experimentalHallLog,
                        0,
                        false, 
                        0);
    
  experimentalHallLog->SetVisAttributes(G4VisAttributes::GetInvisible());

  // put Gem in world
  //
  G4VPhysicalVolume* Gem_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),                    //at position
                    Gem_Log,             //its logical volume
                    "Gem_Phys",                //its name
                    experimentalHallLog,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking

    /*******************************
   * Pb Tubs       *
   *******************************/
  G4VSolid* Pb_tub
    = new G4Tubs("Pb_tub", 0, Rcore, 10*cm, Sphi, Dphi);

  G4LogicalVolume* Pb_Log 
    = new G4LogicalVolume(Pb_tub,
			  pb,
                          "Pb_Log",
                          0,               //opt: fieldManager
                          0,               //opt: SensitiveDetector
                          0);              //opt: UserLimits

  // put Pb tubs in world
  //
/*
  G4VPhysicalVolume* Pb_Phys 
    = new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),                    //at position
                    Pb_Log,             //its logical volume
                    "Pb_Phys",                //its name
                    experimentalHallLog,                //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    1);          //overlaps checking
*/

  fScoringVolume1 = Gem_1_readout_Log;
  fScoringVolume2 = Gem_2_readout_Log;

  // visualization attributes ------------------------------------------------

  auto visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  visAttributes->SetVisibility(false);
  experimentalHallLog->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray
  //Pb_Log->SetVisAttributes(visAttributes);

  //visAttributes = new G4VisAttributes(G4Colour(233/256.,206/256.,238./256,0.1));
  visAttributes = new G4VisAttributes(G4Colour(0.9,0.9,0.9));   // LightGray
  Gem_Log->SetVisAttributes(visAttributes);

  return experimentalHallPhys;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4cout << "Construt the DetectorGeometry" <<G4endl;
  
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // define a material
  DefineMaterials();
    
  // define some constant
  DefineConstants();

  // Define volumes
  return DefineVolumes();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
