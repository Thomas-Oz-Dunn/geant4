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
    
    G4double env_sizeXY = 20 * cm, env_sizeZ = 30 * cm;


    G4double pTypeConcentration  = 1e-13; // ?
    G4double pTypeThickness  = 3.5 * um; // ?
    G4String pTypeDopant = "Zn"; // ?

    G4double nTypeConcentration  = 1e-13; // ?
    G4String nTypeDopant = "Si"; // ?
    G4double nTypeThickness  = 0.1 * um; // ?

    // Junction parameters
    // TODO-TD: parameterize dopings, thickness
    // create an n-type and p-type box adjacent to each other

    G4Element* Ga = nist->FindOrBuildElement("Ga"); 
    G4Element* As = nist->FindOrBuildElement("As"); 
    G4double GaAsdensity  = 5.32  * g/cm3;
    G4Material* GaAs = new G4Material("GaAs", GaAsdensity, 2);
    GaAs->AddElement(Ga, 50 * perCent);
    GaAs->AddElement(As, 50 * perCent);
    G4bool checkOverlaps = true;

    // World parameters
    G4double world_sizeXY = 1.2 * env_sizeXY;
    G4double world_sizeZ = 1.2 * env_sizeZ;

    G4double density     = universe_mean_density; 
    G4double pressure    = 3.e-18*pascal;
    G4double temperature = 2.73*kelvin;
    G4Material* Vacuum = new G4Material("Vacuum", 
                                        1., 
                                        1.008*g/mole, 
                                        density,
                                        kStateGas,
                                        temperature,
                                        pressure);

    auto solidWorld = new G4Box("World", 
                                0.5 * world_sizeXY, 
                                0.5 * world_sizeXY, 
                                0.5 * world_sizeZ);

    auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
                                          Vacuum,  // its material
                                          "World");  // its name

    auto physWorld = new G4PVPlacement(nullptr, 
                                      G4ThreeVector(),  
                                      logicWorld,  
                                      "World",  
                                      nullptr,  
                                      false, 
                                      0,  
                                      checkOverlaps); 

    // Junction
    // TODO-TD: parameterize device thickness to experiment with Bragg's peak
    auto solidEnv = new G4Box("Junction", 
                              0.5 * env_sizeXY, 
                              0.5 * env_sizeXY, 
                              0.5 * env_sizeZ); 

    auto logicEnv = new G4LogicalVolume(solidEnv,  // its solid
                                        GaAs,  // its material
                                        "Junction");  // its name

    new G4PVPlacement(nullptr,  // no rotation
                      G4ThreeVector(),  // at (0,0,0)
                      logicEnv,  // its logical volume
                      "Junction",  // its name
                      logicWorld,  // its mother  volume
                      false,  // no boolean operation
                      0,  // copy number
                      checkOverlaps);  // overlaps checking

    return physWorld;
  }

}  // namespace GaAsProton
