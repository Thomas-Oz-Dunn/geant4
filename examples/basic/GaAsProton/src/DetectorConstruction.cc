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
    
    // TODO-TD: parameterize device thickness to experiment with Bragg's peak
    // TODO-TD: parameterize dopings
    // TODO-TD: triple junction experiment
    G4double junction_xy_size = 2.0 * cm;
    G4double nTypeThickness = 0.3 * um;
    G4double pTypeThickness = 3.5 * um;
    G4double nTypeConcentration = 2e18 / cm3;  // Si emitter
    G4double pTypeConcentration = 2e17 / cm3;  // Zn base

    // Junction parameters
    // TODO-TD: wrap the doped material construction 
    // in a method or inherited class
    G4Element* Ga = nist->FindOrBuildElement("Ga"); 
    G4Element* As = nist->FindOrBuildElement("As"); 
    G4double GaAsdensity = 5.32  * g/cm3;
    G4String nTypeDopantName = "Si";

    G4Element* nTypeDopant = nist->FindOrBuildElement(nTypeDopantName);
    G4double dopantMolarMass = nTypeDopant->GetA() * mole / g; // g/mol

    G4double dopantMassFracNType =
        (nTypeConcentration * dopantMolarMass) /
        (GaAsdensity / (g/cm3) * Avogadro);

    G4Material* NtypeGaAs = new G4Material("NtypeGaAs", GaAsdensity, 3);
    NtypeGaAs->AddElement(Ga, 0.5 * (1.0 - dopantMassFracNType));
    NtypeGaAs->AddElement(As, 0.5 * (1.0 - dopantMassFracNType));
    NtypeGaAs->AddElement(nTypeDopant, dopantMassFracNType);

    G4String pTypeDopantName = "Zn";
    G4Element* pTypeDopant = nist->FindOrBuildElement(pTypeDopantName);
    G4double pTypeDopantMolarMass = pTypeDopant->GetA() * mole / g; // g/mol

    G4double dopantMassFracPType =
        (pTypeConcentration * pTypeDopantMolarMass) /
        (GaAsdensity / (g/cm3) * Avogadro); 

    G4Material* PtypeGaAs = new G4Material("PtypeGaAs", GaAsdensity, 3);
    PtypeGaAs->AddElement(Ga, 0.5 * (1.0 - dopantMassFracPType));
    PtypeGaAs->AddElement(As, 0.5 * (1.0 - dopantMassFracPType));
    PtypeGaAs->AddElement(pTypeDopant, dopantMassFracPType);

    G4bool checkOverlaps = true;

    // World parameters
    G4double world_sizeXY = 1.2 * junction_xy_size;
    G4double world_sizeZ = 1.2 * (nTypeThickness + pTypeThickness);
    G4double density = universe_mean_density; 
    G4double pressure = 3.e-18 * pascal;
    G4double temperature = 2.73 * kelvin;
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
    // create an n-type and p-type box adjacent to each other
    auto solidNType = new G4Box("NType", 
                                0.5 * junction_xy_size, 
                                0.5 * junction_xy_size, 
                                0.5 * nTypeThickness); 

    auto logicNType = new G4LogicalVolume(solidNType,
                                        NtypeGaAs,
                                        "NType");

    new G4PVPlacement(nullptr,
                      G4ThreeVector(), 
                      logicNType,  
                      "Ntype",  
                      logicWorld, 
                      false, 
                      0, 
                      checkOverlaps); 

    auto solidPType = new G4Box("PType", 
                                0.5 * junction_xy_size, 
                                0.5 * junction_xy_size, 
                                0.5 * pTypeThickness); 

    auto logicPType = new G4LogicalVolume(solidPType,
                                          PtypeGaAs,
                                          "PType");

    new G4PVPlacement(nullptr,
                      G4ThreeVector(0, 0, nTypeThickness), 
                      logicPType,  
                      "Ptype",  
                      logicWorld, 
                      false, 
                      0, 
                      checkOverlaps);  

    // p-type (base) region as the target for range/dose
    // reporting in Run::EndOfRun().
    fLogicTarget = logicPType;
    fMaterial = PtypeGaAs;
    fSize = pTypeThickness;

    return physWorld;
  }

}  // namespace GaAsProton
