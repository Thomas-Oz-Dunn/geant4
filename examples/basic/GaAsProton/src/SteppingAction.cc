/// \file SteppingAction.cc
/// \brief Implementation of the GaAsProton::SteppingAction class

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4ICRU49NuclearStoppingModel.hh"
#include "G4NIELCalculator.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"

// geant4\examples\extended\electromagnetic\TestEm1\src\SteppingAction.cc  NIEL

namespace GaAsProton
{

  SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {
    
    fNIELCalculator = new G4NIELCalculator(new G4ICRU49NuclearStoppingModel(), 1);
  }

  void SteppingAction::UserSteppingAction(const G4Step* step)
  {

    Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4double EdepStep = aStep->GetTotalEnergyDeposit();

    if (EdepStep > 0.) {
      run->AddEdep(EdepStep);
      fEventAction->AddEdep(EdepStep);
    }
    G4double niel = fNIELCalculator->ComputeNIEL(aStep);
    if (niel > 0.) {
      run->AddNIEL(niel);
      fEventAction->AddNIEL(niel);
    }

    const G4VProcess* process = aStep->GetPostStepPoint()->GetProcessDefinedStep();
    if (process) run->CountProcesses(process->GetProcessName());

    // step length of primary particle
    G4int ID = aStep->GetTrack()->GetTrackID();
    G4double steplen = aStep->GetStepLength();
    if (ID == 1) analysisManager->FillH1(3, steplen);

    /*
      //debug: charge and mass
      //
      G4int stepNb = aStep->GetTrack()->GetCurrentStepNumber();
      G4StepPoint* postPoint = aStep->GetPostStepPoint();
      G4double charge = postPoint->GetCharge();
      G4double mass   = postPoint->GetMass();
      G4cout << "\n   step= " << stepNb << "   charge= " << charge
            << "  mass= " << G4BestUnit(mass, "Energy");
    */

  }


}  // namespace GaAsProton
