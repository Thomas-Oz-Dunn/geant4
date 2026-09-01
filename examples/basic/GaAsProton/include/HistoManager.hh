/// \file HistoManager.hh
/// \brief Definition of the HistoManager class

#ifndef HistoManager_h
#define HistoManager_h 1

#include "G4AnalysisManager.hh"
#include "globals.hh"

// TODO-TD: add to namespace?

class HistoManager
{
  public:
    HistoManager();
    ~HistoManager() = default;

  private:
    void Book();

    G4String fFileName = "GaAsProton";
};

#endif
