/// \file SteppingAction.cc
/// \brief Implementation of the GaAsProton::SteppingAction class

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "Run.hh"

#include "G4ICRU49NuclearStoppingModel.hh"
#include "G4NIELCalculator.hh"
#include "G4Event.hh"
#include "G4LogicalVolume.hh"
#include "G4RunManager.hh"
#include "G4Step.hh"
#include "G4AnalysisManager.hh"

// NIEL Reference: geant4\examples\extended\electromagnetic\TestEm1\src\SteppingAction.cc  

namespace GaAsProton
{

  SteppingAction::SteppingAction(EventAction* eventAction) : fEventAction(eventAction) {
    fNIELCalculator = new G4NIELCalculator(new G4ICRU49NuclearStoppingModel(), 1);
  }

  void SteppingAction::UserSteppingAction(const G4Step* step)
  {

    Run* run = static_cast<Run*>(
      G4RunManager::GetRunManager()->GetNonConstCurrentRun()
    );
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    G4double EdepStep = step->GetTotalEnergyDeposit();
    if (EdepStep > 0.) {
      run->AddEdep(EdepStep);
      fEventAction->AddEdep(EdepStep);
    }

    G4double niel = fNIELCalculator->ComputeNIEL(step);
    if (niel > 0.) {
      run->AddNIEL(niel);
      fEventAction->AddNIEL(niel);
    }

    const G4VProcess* process = step->GetPostStepPoint()->GetProcessDefinedStep();
    if (process) run->CountProcesses(
      process->GetProcessName()
    );

    const G4Track* track = step->GetTrack();

    // step length of primary particle
    G4int ID = track->GetTrackID();
    G4double steplen = step->GetStepLength();

    // TODO_TD: store the histogram IDs as globals?
    if (ID == 1) analysisManager->FillH1(3, steplen);

    if (track->GetParentID() != 0 && track->GetCurrentStepNumber() == 1){
      const G4VProcess* proc = track->GetCreatorProcess();
      G4String procname = proc ? proc->GetProcessName() : "";
      G4int Z = track->GetParticleDefinition()->GetAtomicNumber();

      // FIXME-TD: these numbers feel wrong
      bool isGaOrAs = Z == 31 || Z == 32;

      bool isNuclearInteraction = procname == "hadElastic" || procname.contains("Inelastic");
      if (isNuclearInteraction && isGaOrAs) {
        G4double Erecoil = track->GetKineticEnergy();
        G4ThreeVector dir = track->GetMomentumDirection();

        // TODO-TD: log these parameters and plot their distribution
      }
    }

    // TODO-TD: do we need to lindhard partition the data as well?
  }

}  // namespace GaAsProton
