#include "RunAction.hh"

#include "G4GenericMessenger.hh"
#include "G4Hdf5AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"

G4String RunAction::fOutputFileName = "shower_data.h5";

RunAction::RunAction() {
    if (isMaster) {
        fMessenger = std::make_unique<G4GenericMessenger>(
            this, "/emshower/", "EM shower simulation controls");
        fMessenger->DeclareMethod("outputFile", &RunAction::SetOutputFileName, 
            "Set the HDF5 output filename for the next run.")
            .SetStates(G4State_PreInit, G4State_Idle);
    }

    auto* am = G4Hdf5AnalysisManager::Instance();
    am->SetVerboseLevel(1);
    am->CreateNtuple("hits", "EM Shower Hits");
    am->CreateNtupleIColumn("event");
    am->CreateNtupleIColumn("layer");
    am->CreateNtupleDColumn("x");
    am->CreateNtupleDColumn("y");
    am->CreateNtupleDColumn("edep");
    am->FinishNtuple();
}

void RunAction::BeginOfRunAction(const G4Run*) {
    G4Hdf5AnalysisManager::Instance()->OpenFile(fOutputFileName);
}

void RunAction::EndOfRunAction(const G4Run*) {
    G4Hdf5AnalysisManager::Instance()->Write();
    G4Hdf5AnalysisManager::Instance()->CloseFile();
}

void RunAction::SetOutputFileName(const G4String& fileName) {
    fOutputFileName = fileName;
}
