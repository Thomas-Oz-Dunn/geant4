/// \file DetectorConstruction.cc
/// \brief Implementation of the GaAsProton::DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4LogicalVolume.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4Trd.hh"

namespace GaAsProton
{
  G4VPhysicalVolume* DetectorConstruction::Construct()
  {
    G4NistManager* nist = G4NistManager::Instance();

    // Envelope parameters
    G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;
    G4Material* env_mat = nist->FindOrBuildMaterial("G4_Ga"); // TODO-TD: GaAs
    G4bool checkOverlaps = true;

    // World parameters
    G4double world_sizeXY = 1.2 * env_sizeXY;
    G4double world_sizeZ = 1.2 * env_sizeZ;

    G4double density     = universe_mean_density; 
    G4double pressure    = 3.e-18*pascal;
    G4double temperature = 2.73*kelvin;
    G4Material* Vacuum =   
    new G4Material("Vacuum", 1., 1.008*g/mole, density,
                              kStateGas,temperature,pressure);

    auto solidWorld =
      new G4Box("World", 
                0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

    auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                          Vacuum,  // its material
                                          "World");  // its name

    auto physWorld = new G4PVPlacement(nullptr,  // no rotation
                                      G4ThreeVector(),  // at (0,0,0)
                                      logicWorld,  // its logical volume
                                      "World",  // its name
                                      nullptr,  // its mother  volume
                                      false,  // no boolean operation
                                      0,  // copy number
                                      checkOverlaps);  // overlaps checking

    //
    // Envelope
    //
    auto solidEnv = new G4Box("Envelope",  // its name
                              0.5 * env_sizeXY, 0.5 * env_sizeXY, 0.5 * env_sizeZ);  // its size

    auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                        env_mat,  // its material
                                        "Envelope");  // its name

    new G4PVPlacement(nullptr,  // no rotation
                      G4ThreeVector(),  // at (0,0,0)
                      logicEnv,  // its logical volume
                      "Envelope",  // its name
                      logicWorld,  // its mother  volume
                      false,  // no boolean operation
                      0,  // copy number
                      checkOverlaps);  // overlaps checking

    //
    // always return the physical World
    //
    return physWorld;
  }



}  // namespace GaAsProton
