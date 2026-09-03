/// \file RunAction.hh
/// \brief Definition of the GaAsProton::RunAction class

#ifndef GaAsProtonRunAction_h
#define GaAsProtonRunAction_h 1

#include "G4UserRunAction.hh"

#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;
class HistoManager;
class Run;

namespace GaAsProton
{
  class DetectorConstruction;
  class PrimaryGeneratorAction;

  /// Run action class
  ///
  /// In EndOfRunAction(), it calculates the dose in the selected volume
  /// from the energy deposit accumulated via stepping and event actions.
  /// The computed dose is then printed on the screen.

  class RunAction : public G4UserRunAction
  {
    public:
      RunAction();
      ~RunAction() override = default;

      // Return a project-specific Run so SteppingAction/EventAction can
      // accumulate NIEL, process counts, and range statistics into it.
      G4Run* GenerateRun() override;

      void BeginOfRunAction(const G4Run*) override;
      void EndOfRunAction(const G4Run*) override;

      void AddEdep(G4double edep);

    private:
      G4Accumulable<G4double> fEdep = 0.;
      G4Accumulable<G4double> fEdep2 = 0.;
      HistoManager* fHistoManager = nullptr;

      Run* fRun = nullptr;
      const DetectorConstruction* fDetector = nullptr;
      const PrimaryGeneratorAction* fPrimary = nullptr;
  };

}  // namespace GaAsProton

#endif
