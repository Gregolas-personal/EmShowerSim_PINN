#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

int main(int argc, char** argv) {
    auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization()); // needed because MT, all related to event must be in here while detector, physics list and vis can be in main

    runManager->Initialize();

    G4VisManager* visManager = new G4VisExecutive("quiet");
    visManager->Initialize();

    auto* UImanager = G4UImanager::GetUIpointer();
    if (argc > 1) {
        UImanager->ApplyCommand(G4String("/control/execute ") + argv[1]);
    } else {
        G4UIExecutive* ui = new G4UIExecutive(argc, argv);
        ui->SessionStart();
        delete ui;
    }

    delete visManager;
    delete runManager;
    return 0;
}
