/// \file PrimaryGeneratorAction.hh
/// \brief Definition of the GaAsProton::PrimaryGeneratorAction class

#ifndef GaAsProtonPrimaryGeneratorAction_h
#define GaAsProtonPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4Types.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

namespace GaAsProton
{

  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
    public:
      PrimaryGeneratorAction();
      ~PrimaryGeneratorAction() override;

      // method from the base class
      void GeneratePrimaries(G4Event*) override;

      // method to access particle gun
      const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

    private:
      G4ParticleGun* fParticleGun = nullptr;  
      G4Box* fJunctionBox = nullptr;
      G4double fFluence;
  };

}  // namespace GaAsProton

#endif
