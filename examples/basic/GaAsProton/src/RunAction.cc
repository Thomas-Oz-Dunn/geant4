/// \file RunAction.cc
/// \brief Implementation of the GaAsProton::RunAction class

#include "RunAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4AccumulableManager.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4AnalysisManager.hh"

namespace GaAsProton
{

  RunAction::RunAction()
  {
    // Detector is available on both master and worker threads.
    fDetector = static_cast<const DetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    // Primary generator only exists on worker threads (nullptr on master
    // in MT mode); BeginOfRunAction() guards against a null fPrimary.
    fPrimary = static_cast<const PrimaryGeneratorAction*>(
        G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());

    fHistoManager = new HistoManager();
  }

  G4Run* RunAction::GenerateRun()
  {
    fRun = new Run(fDetector);
    return fRun;
  }


  void RunAction::BeginOfRunAction(const G4Run*)
  {
  // show Rndm status
  if (isMaster) G4Random::showEngineStatus();

  // keep run condition
  if (fPrimary) {
    G4ParticleDefinition* particle = fPrimary->GetParticleGun()->GetParticleDefinition();
    G4double energy = fPrimary->GetParticleGun()->GetParticleEnergy();
    fRun->SetPrimary(particle, energy);
  }

  // histograms
  //
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (analysisManager->IsActive()) {
    analysisManager->OpenFile();
  }
  }


  void RunAction::EndOfRunAction(const G4Run* run)
  {
    // compute and print statistic
    if (isMaster) fRun->EndOfRun();

    // save histograms
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    if (analysisManager->IsActive()) {
      analysisManager->Write();
      analysisManager->CloseFile();
    }

    // show Rndm status
    if (isMaster) G4Random::showEngineStatus();
  }

  void RunAction::AddEdep(G4double edep)
  {
    // TODO-TD: is this where we'd add NIEL calculations?
    fEdep += edep;
    fEdep2 += edep * edep;
  }

}  // namespace GaAsProton
