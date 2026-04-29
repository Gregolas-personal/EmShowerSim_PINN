#pragma once

#include "G4VSensitiveDetector.hh"
#include "G4Types.hh"
#include <vector>

struct HitData {
    G4int    layerID;
    G4double x, y;  // transverse hit position [mm]
    G4double edep;  // energy deposited [MeV]
};

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class SensitiveDetector : public G4VSensitiveDetector {
public:
    explicit SensitiveDetector(const G4String& name);
    ~SensitiveDetector() override = default;

    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;
    void   EndOfEvent(G4HCofThisEvent*) override;

private:
    std::vector<HitData> fHits;
};
