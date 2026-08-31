/// \file DetectorConstruction.hh
/// \brief Definition of the GaAsProton::DetectorConstruction class

#ifndef GaAsProtonDetectorConstruction_h
#define GaAsProtonDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

namespace GaAsProton
{

  class DetectorConstruction : public G4VUserDetectorConstruction
  {
    public:
      DetectorConstruction() = default;
      ~DetectorConstruction() override = default;

      G4VPhysicalVolume* Construct() override;
    protected:
  };

}  // namespace GaAsProton

#endif
