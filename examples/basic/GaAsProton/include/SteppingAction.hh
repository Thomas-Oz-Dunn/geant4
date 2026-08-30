/// \file SteppingAction.hh
/// \brief Definition of the GaAsProton::SteppingAction class

#ifndef GaAsProtonSteppingAction_h
#define GaAsProtonSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4NIELCalculator.hh"

class G4LogicalVolume;
class G4Step;

namespace GaAsProton
{

  class EventAction;

  /// Stepping action class

  class SteppingAction : public G4UserSteppingAction
  {
    public:
      SteppingAction(EventAction* eventAction);
      ~SteppingAction() override = default;

      // method from the base class
      void UserSteppingAction(const G4Step*) override;

    private:
      EventAction* fEventAction = nullptr;
      G4LogicalVolume* fScoringVolume = nullptr;
      G4NIELCalculator* fNIELCalculator = nullptr;
  };

}  // namespace GaAsProton

#endif
