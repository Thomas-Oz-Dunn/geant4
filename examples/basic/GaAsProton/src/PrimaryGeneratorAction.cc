/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the GaAsProton::PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
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
    G4String particleName;
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName = particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(particleEnergy);
  }


  PrimaryGeneratorAction::~PrimaryGeneratorAction()
  {
    delete fParticleGun;
  }


  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
  {
    // this function is called at the begining of each event
    // In order to avoid dependence of PrimaryGeneratorAction
    // on DetectorConstruction class we get Envelope volume
    // from G4LogicalVolumeStore.

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
      G4Exception("PrimaryGeneratorAction::GeneratePrimaries()", "MyCode0002", JustWarning, msg);
    }

    // TODO-TD: double check this location vs the junction
    G4double xysize = 0.2;
    G4double x0 = xysize * envSizeXY * (G4UniformRand() - 0.5);
    G4double y0 = xysize * envSizeXY * (G4UniformRand() - 0.5);
    G4double z0 = -1.2 * envSizeZ;

    fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
    fParticleGun->GeneratePrimaryVertex(event);
  }

}  // namespace GaAsProton
