/// \file DetectorConstruction.hh
/// \brief Definition of the GaAsProton::DetectorConstruction class

#ifndef GaAsProtonDetectorConstruction_h
#define GaAsProtonDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

namespace GaAsProton
{

  class DetectorConstruction : public G4VUserDetectorConstruction
  {
    public:
      DetectorConstruction() = default;
      ~DetectorConstruction() override = default;

      G4VPhysicalVolume* Construct() override;

      // Accessors used by Run::EndOfRun() for the range/dose summary.
      G4Material* GetMaterial() const { return fMaterial; }
      G4double GetSize() const { return fSize; }

    protected:
      G4LogicalVolume* fLogicTarget = nullptr;
      G4Material* fMaterial = nullptr;
      // TODO-TD: include all junctions in fSize calculation?
      G4double fSize = 0.;
  };

}  // namespace GaAsProton

#endif
