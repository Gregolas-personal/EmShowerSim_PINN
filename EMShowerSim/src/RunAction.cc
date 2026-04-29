#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"

void RunAction::BeginOfRunAction(const G4Run*) {
    auto* am = G4AnalysisManager::Instance();
    am->OpenFile("shower_data.h5");
    am->CreateNtuple("hits", "EM Shower Hits");
    am->CreateNtupleIColumn("event");
    am->CreateNtupleIColumn("layer");
    am->CreateNtupleDColumn("x");
    am->CreateNtupleDColumn("y");
    am->CreateNtupleDColumn("edep");
    am->FinishNtuple();
}

void RunAction::EndOfRunAction(const G4Run*) {
    G4AnalysisManager::Instance()->Write();
    G4AnalysisManager::Instance()->CloseFile();
}