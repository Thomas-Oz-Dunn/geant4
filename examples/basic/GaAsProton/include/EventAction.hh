/// \file EventAction.hh
/// \brief Definition of the GaAsProton::EventAction class

#ifndef GaAsProtonEventAction_h
#define GaAsProtonEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;

namespace GaAsProton
{

  class RunAction;

  class EventAction : public G4UserEventAction
  {
    public:
      EventAction(RunAction* runAction);
      ~EventAction() override = default;

      void BeginOfEventAction(const G4Event* event) override;
      void EndOfEventAction(const G4Event* event) override;

      // TODO-TD: do we need this function?
      void AddEdep(G4double edep) { fEdep += edep; }

    private:
      RunAction* fRunAction = nullptr;
      G4double fEdep = 0.;
  };

}  // namespace GaAsProton

#endif
