#pragma once

#include "G4UserRunAction.hh"
#include "globals.hh"

#include <memory>

class G4Run;
class G4GenericMessenger;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    void SetOutputFileName(const G4String& fileName);

private:
    static G4String fOutputFileName;
    std::unique_ptr<G4GenericMessenger> fMessenger;
};
