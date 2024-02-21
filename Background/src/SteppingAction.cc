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
//
// Previous authors: G. Guerrieri, S. Guatelli and M. G. Pia, INFN Genova, Italy
// Authors (since 2007): S. Guatelli, University of Wollongong, Australia
//#include "Run.hh"

//
#include "SteppingAction.hh"
#include "G4SteppingManager.hh"
#include "G4UnitsTable.hh"
#include "Run.hh"
#include "G4Electron.hh"
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
#include "G4Gamma.hh"
#include "G4Alpha.hh"
#include "G4Positron.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4LogicalVolume.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"

SteppingAction::SteppingAction():fScoringVolume1(nullptr),fScoringVolume2(nullptr)
{}

SteppingAction::~SteppingAction()
{}

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4LogicalVolume* volume 
    = aStep->GetPreStepPoint()->GetTouchableHandle()
    ->GetVolume()->GetLogicalVolume();
  
  G4double energy = aStep->GetTotalEnergyDeposit();
  G4double totalenergy = aStep->GetTrack()->GetTotalEnergy();
  if(energy>0){
 
    if (!fScoringVolume1) { 
      const DetectorConstruction * detectorConstruction
     	= static_cast<const DetectorConstruction*>
     	(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
      fScoringVolume1 = detectorConstruction->GetScoringVolume1();   
    }
    if (!fScoringVolume2) { 
      const DetectorConstruction * detectorConstruction
     	= static_cast<const DetectorConstruction*>
     	(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
      fScoringVolume2 = detectorConstruction->GetScoringVolume2();   
    }
    if ( (volume == fScoringVolume1) || (volume == fScoringVolume2) ) {
      G4int iTrkID = aStep->GetTrack()->GetTrackID();
      G4int iTrkparentID = aStep->GetTrack()->GetParentID();
      //Run::GetInstance()->AddReadoutTrkid(iTrkID);
      //Run::GetInstance()->AddReadoutTrkparentid(iTrkparentID);
      if( !(iTrkID==1 && iTrkparentID==0) ) return;

      G4StepPoint* prePoint  = aStep->GetPreStepPoint();
      G4StepPoint* postPoint = aStep->GetPostStepPoint();
      G4double x = (prePoint->GetPosition().x()+ postPoint->GetPosition().x())/2.;
      G4double y = (prePoint->GetPosition().y()+ postPoint->GetPosition().y())/2.;
      G4double z = (prePoint->GetPosition().z()+ postPoint->GetPosition().z())/2.;
      int igem=-1;
      if(volume == fScoringVolume1) igem=0;
      else if(volume == fScoringVolume2) igem=1;
      else return;

      G4ThreeVector curDirection =  aStep->GetPreStepPoint()->GetMomentumDirection();
      G4double px = curDirection.x();
      G4double py = curDirection.y();
      G4double pz = curDirection.z();

      if(Run::GetInstance()->GetGemTrkStatus(igem)==true) return;

      Run::GetInstance()->SetGemTrkPx(igem,px);
      Run::GetInstance()->SetGemTrkPy(igem,py);
      Run::GetInstance()->SetGemTrkPz(igem,pz);
      Run::GetInstance()->SetGemTrkE(igem,totalenergy/MeV);
      Run::GetInstance()->SetGemTrkEdep(igem,energy/MeV);
      Run::GetInstance()->SetGemTrkX(igem,x/mm);
      Run::GetInstance()->SetGemTrkY(igem,y/mm);
      Run::GetInstance()->SetGemTrkZ(igem,z/mm);
      Run::GetInstance()->SetGemTrkStatus(igem,true);

    }

  }
  
}
  
void SteppingAction::printDaughters(const G4LogicalVolume* mother) {
  int nDaughters = mother->GetNoDaughters();
  for (int i = 0; i < nDaughters; i++) {
    const G4VPhysicalVolume* daughter = mother->GetDaughter(i);
    const G4LogicalVolume* daughterLV = daughter->GetLogicalVolume();
    G4String daughterName = daughterLV->GetName();
    std::cout << "Daughter " << i << ": " << daughterName << " (" << daughter << ")" << std::endl;
  }
}
