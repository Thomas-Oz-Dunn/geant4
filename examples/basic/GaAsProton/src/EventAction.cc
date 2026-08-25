/// \file EventAction.cc
/// \brief Implementation of the GaAsProton::EventAction class

#include "EventAction.hh"

#include "RunAction.hh"

namespace GaAsProton
{

  EventAction::EventAction(RunAction* runAction) : fRunAction(runAction) {}

  void EventAction::BeginOfEventAction(const G4Event*)
  {
    fEdep = 0.;
  }

  void EventAction::EndOfEventAction(const G4Event*)
  {
    // accumulate statistics in run action
    fRunAction->AddEdep(fEdep);
  }

}  // namespace GaAsProton
