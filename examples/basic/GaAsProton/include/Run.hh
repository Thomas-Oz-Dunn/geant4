/// \file Run.hh
/// \brief Definition of the Run class

#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "globals.hh"

#include <map>

namespace GaAsProton
{
  class DetectorConstruction;
}

class G4ParticleDefinition;

class Run : public G4Run
{
  public:
    Run(const GaAsProton::DetectorConstruction*);
    ~Run() override = default;

    void SetPrimary(const G4ParticleDefinition* particle, G4double energy);

    void CountTraks0(G4int nt) { fNeutralNTracks += nt; }
    void CountTraks1(G4int nt) { fChargedNTracks += nt; }
    void CountSteps0(G4int ns) { fNbOfSteps0 += ns; }
    void CountSteps1(G4int ns) { fNbOfSteps1 += ns; }
    void CountProcesses(const G4String& procName);

    void AddEdep(G4double val) { fEdep += val; }
    void AddEleak(G4double val){ fEleak += val;}
    void AddNIEL(G4double val) { fNIEL += val; }
    void AddTrueRange(G4double l)
    {
      fTrueRange += l;
      fTrueRange2 += l * l;
    }
    void AddProjRange(G4double x)
    {
      fProjRange += x;
      fProjRange2 += x * x;
    }
    void AddTransvDev(G4double y)
    {
      fTransvDev += y;
      fTransvDev2 += y * y;
    }

    void Merge(const G4Run*) override;
    void EndOfRun();

  private:
    const GaAsProton::DetectorConstruction* fDetector = nullptr;
    const G4ParticleDefinition* fParticle = nullptr;

    // Accumulators
    G4double fEkin = 0.;
    G4int fNeutralNTracks = 0;
    G4int fChargedNTracks = 0;
    G4int fNbOfSteps0 = 0;
    G4int fNbOfSteps1 = 0;
    G4double fEdep = 0.;
    G4double fEleak = 0.;
    G4double fNIEL = 0.;
    G4double fTrueRange = 0.;
    G4double fTrueRange2 = 0.;
    G4double fProjRange = 0.;
    G4double fProjRange2 = 0.;
    G4double fTransvDev = 0.;
    G4double fTransvDev2 = 0.;

    std::map<G4String, G4int> fProcCounter;
};


#endif
