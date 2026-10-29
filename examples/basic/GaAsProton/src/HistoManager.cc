/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class

#include "HistoManager.hh"

#include "G4UnitsTable.hh"


HistoManager::HistoManager()
{
  Book();
}

void HistoManager::Book()
{
  // Create or get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);

  // start histogram numbering from 1
  analysisManager->SetFirstHistoId(1);

    // enable inactivation of histograms
  analysisManager->SetActivation(true);

  // Define histograms start values
  const G4int kMaxHisto = 9;
  const G4String id[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9" };
  const G4String title[] = {
    "total track length of primary particle",
    "nb steps of primary particle",
    "step size of primary particle",
    "total energy deposit",
    "energy of charged secondaries at creation",
    "energy of neutral secondaries at creation",
    "NIEL energy deposit",
    "energy leakage",
    "energy total: deposit + leakage",
  };

  // Default values (to be reset via /analysis/h1/set command)
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated
  // as we have not yet set nbins, vmin, vmax?
  for (G4int k = 0; k < kMaxHisto; k++) {
    G4int ih = analysisManager->CreateH1(
      id[k], 
      title[k], 
      nbins, 
      vmin, 
      vmax
    );

    analysisManager->SetH1Activation(ih, false);
  }
}
