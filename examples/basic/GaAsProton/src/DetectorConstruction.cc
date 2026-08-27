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
    G4double nTypeThickness = 0.3 * um;
    G4double pTypeThickness = 3.5 * um;


    // Junction parameters
    // TODO-TD: parameterize dopings, thickness
    // create an n-type and p-type box adjacent to each other

    G4Element* Ga = nist->FindOrBuildElement("Ga"); 
    G4Element* As = nist->FindOrBuildElement("As"); 
    G4double GaAsdensity  = 5.32  * g/cm3;
    G4double nTypeConcentration = 2e18 / cm3;  // Si emitter
    G4String nTypeDopantName = "Si";

    G4Element* nTypeDopant = nist->FindOrBuildElement(nTypeDopantName);
    G4double dopantMolarMass = nTypeDopant->GetA() * mole / g; // g/mol

    G4double dopantMassFrac =
        (nTypeConcentration * dopantMolarMass) /
        (GaAsdensity / (g/cm3) * 6.02214076e23) ;  // Avogadros Const

    G4Material* NtypeGaAs = new G4Material("NtypeGaAs", GaAsdensity, 3);
    NtypeGaAs->AddElement(Ga, 0.5 * (1.0 - dopantMassFrac));
    NtypeGaAs->AddElement(As, 0.5 * (1.0 - dopantMassFrac));
    NtypeGaAs->AddElement(nTypeDopant, dopantMassFrac);

    G4double pTypeConcentration = 2e17 / cm3;  // Zn base
    G4String pTypeDopantName = "Zn";
    G4Element* pTypeDopant = nist->FindOrBuildElement(pTypeDopantName);
    G4double pTypeDopantMolarMass = pTypeDopant->GetA() * mole / g; // g/mol

    G4double dopantMassFrac =
        (pTypeConcentration * pTypeDopantMolarMass) /
        (GaAsdensity / (g/cm3) * 6.02214076e23) ;  // Avogadros Const

    G4Material* PtypeGaAs = new G4Material("PtypeGaAs", GaAsdensity, 3);
    PtypeGaAs->AddElement(Ga, 0.5 * (1.0 - dopantMassFrac));
    PtypeGaAs->AddElement(As, 0.5 * (1.0 - dopantMassFrac));
    PtypeGaAs->AddElement(pTypeDopant, dopantMassFrac);


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

    auto logicWorld = new G4LogicalVolume(solidWorld,
                                          Vacuum,
                                          "World");

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
    // And place ptype and ntype on top of each other, point the proton beam orthogonal
    auto solidEnv = new G4Box("Junction", 
                              0.5 * env_sizeXY, 
                              0.5 * env_sizeXY, 
                              0.5 * env_sizeZ); 

    auto logicEnv = new G4LogicalVolume(solidEnv,
                                        NtypeGaAs,
                                        "Junction");

    new G4PVPlacement(nullptr,
                      G4ThreeVector(), 
                      logicEnv,  
                      "Junction",  
                      logicWorld, 
                      false, 
                      0, 
                      checkOverlaps);  

    return physWorld;
  }

}  // namespace GaAsProton
