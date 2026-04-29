#include "SensitiveDetector.hh"

#include "G4Step.hh"
#include "G4TouchableHistory.hh"
#include "G4AnalysisManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector(const G4String& name)
    : G4VSensitiveDetector(name) {}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*) {
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    auto touch = step->GetPreStepPoint()->GetTouchableHandle();
    G4int layer = touch->GetReplicaNumber(0);
    G4ThreeVector pos = step->GetPreStepPoint()->GetPosition();

    fHits.push_back({layer, pos.x(), pos.y(), edep});
    return true;
}

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) {
    G4int eventID =
        G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

    auto* am = G4AnalysisManager::Instance();
    for (const auto& h : fHits) {
        am->FillNtupleIColumn(0, eventID);
        am->FillNtupleIColumn(1, h.layerID);
        am->FillNtupleDColumn(2, h.x / mm);
        am->FillNtupleDColumn(3, h.y / mm);
        am->FillNtupleDColumn(4, h.edep / MeV);
        am->AddNtupleRow();
    }
    fHits.clear();
}
