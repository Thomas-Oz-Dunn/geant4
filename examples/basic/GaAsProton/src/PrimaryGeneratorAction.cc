/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the GaAsProton::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"

namespace GaAsProton
{

  PrimaryGeneratorAction::PrimaryGeneratorAction()
  {
    // TODO-TD: pass energy, fluence, and particle type as function parameters?
    G4String particleName = "proton";
    G4double particleEnergy = 10. * MeV;
    G4int n_particle = 1;

    fParticleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(particleEnergy);

    // target fluence in particles / cm^2
    // TODO-TD: update header with var
    fFluence = 1.0e17 / cm2;
  }


  PrimaryGeneratorAction::~PrimaryGeneratorAction()
  {
    delete fParticleGun;
  }


  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
  {
    G4double envSizeXY = 0;
    G4double envSizeZ = 0;

    if (!fJunctionBox) {
      G4LogicalVolume* envLV = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
      if (envLV) fJunctionBox = dynamic_cast<G4Box*>(envLV->GetSolid());
    }

    if (fJunctionBox) {
      envSizeXY = fJunctionBox->GetXHalfLength() * 2.;
      envSizeZ = fJunctionBox->GetZHalfLength() * 2.;
    }
    else {
      G4ExceptionDescription msg;
      msg << "Envelope volume of box shape not found.\n";
      msg << "Perhaps you have changed geometry.\n";
      msg << "The gun will be place at the center.";
      
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "GaAsProton", JustWarning, msg);
      
      envSizeXY = 10. * cm; // fallback so fluence calc doesn't blow up
      envSizeZ  = 10. * cm;
    }

    // Irradiation field: the square region particles are sampled over.
    // Keep this consistent with the area used in PrintFluenceInfo()/GetRequiredEvents().
    G4double xysize = 0.2;
    G4double fieldSize = xysize * envSizeXY;   // full width of the irradiated square

    G4double x0 = fieldSize * (G4UniformRand() - 0.5);
    G4double y0 = fieldSize * (G4UniformRand() - 0.5);
    G4double z0 = -1.5 * envSizeZ;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
    fParticleGun->GeneratePrimaryVertex(event);

    // On the very first event, tell the user how many primaries are needed
    // for the requested fluence, given the current field size.
    if (event->GetEventID() == 0) {
      G4double area = fieldSize * fieldSize / cm2; // cm^2
      G4double nEvents = fFluence * area;
      G4cout << "\n[PrimaryGeneratorAction] Irradiation field: "
             << fieldSize / cm << " x " << fieldSize / cm << " cm^2"
             << "\n  Target fluence: " << fFluence * cm2 << " /cm^2"
             << "\n  => Required primaries for uniform fluence: "
             << nEvents
             << "\n  Run with: /run/beamOn " << static_cast<G4long>(nEvents)
             << G4endl;
    }
  }

}  // namespace GaAsProton