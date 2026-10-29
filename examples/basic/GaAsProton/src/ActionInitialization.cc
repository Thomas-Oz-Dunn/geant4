/// \file ActionInitialization.cc
/// \brief Implementation of the GaAsProton::ActionInitialization class

#include "ActionInitialization.hh"

#include "EventAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

namespace GaAsProton
{

  // These two different functions are needed 
  // for multithreading scenarios, master thread
  void ActionInitialization::BuildForMaster() const
  {
    auto runAction = new RunAction;
    SetUserAction(runAction);
  }

  // non-master threads
  void ActionInitialization::Build() const
  {
    SetUserAction(new PrimaryGeneratorAction);

    auto runAction = new RunAction;
    SetUserAction(runAction);

    auto eventAction = new EventAction(runAction);
    SetUserAction(eventAction);

    auto steppingAction = new SteppingAction(eventAction);
    SetUserAction(steppingAction);
  }

}  // namespace GaAsProton
