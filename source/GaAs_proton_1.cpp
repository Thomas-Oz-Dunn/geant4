// FIXME-TD: Copied from B1 to use as template, 
// populate and compile

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "ParticleGun.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"

using namespace GaAs_proton_1;

// FIXME-TD: verify namespace usage
void GaAs_proton_1::SetDefaultParticleGun()
{
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle
                    = particleTable->FindParticle(particleName="p+");
  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
  particleGun->SetParticleEnergy(1.*GeV);
  G4double position = -0.5*(Detector->GetWorldSizeX());
  particleGun->SetParticlePosition(G4ThreeVector(position,0.*mm,0.*mm));
}

int main(int argc, char** argv)
{
    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    // Detector construction
    runManager->SetUserInitialization(new DetectorConstruction());
    
    // Physics list
    particleGun  = new G4ParticleGun(n_particle);
    
    // User action initialization
    runManager->SetUserInitialization(new ActionInitialization());

    // Initialize visualization with the default graphics system
    auto visManager = new G4VisExecutive(argc, argv);
    // Constructors can also take optional arguments:
    // - a graphics system of choice, eg. "OGL"
    // - and a verbosity argument - see /vis/verbose guidance.
    // auto visManager = new G4VisExecutive(argc, argv, "OGL", "Quiet");
    // auto visManager = new G4VisExecutive("Quiet");
    visManager->Initialize();

    auto UImanager = G4UImanager::GetUIpointer();

    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command + fileName);

    delete visManager;
    delete runManager;
}

