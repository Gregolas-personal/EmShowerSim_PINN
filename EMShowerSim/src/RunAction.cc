#include "RunAction.hh"

#include "G4Hdf5AnalysisManager.hh"
#include "G4Run.hh"

RunAction::RunAction() {
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
    G4Hdf5AnalysisManager::Instance()->OpenFile("shower_data.h5");
}

void RunAction::EndOfRunAction(const G4Run*) {
    G4Hdf5AnalysisManager::Instance()->Write();
    G4Hdf5AnalysisManager::Instance()->CloseFile();
}
