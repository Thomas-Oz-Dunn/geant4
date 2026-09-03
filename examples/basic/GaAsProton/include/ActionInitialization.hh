/// \file ActionInitialization.hh
/// \brief Definition of the GaAsProton::ActionInitialization class

#ifndef GaAsProtonActionInitialization_h
#define GaAsProtonActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace GaAsProton
{
  class ActionInitialization : public G4VUserActionInitialization
  {
    public:
      ActionInitialization() = default;
      ~ActionInitialization() override = default;

      void BuildForMaster() const override;
      void Build() const override;
  };

}  // namespace GaAsProton

#endif
