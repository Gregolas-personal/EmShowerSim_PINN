#include "PhysicsList.hh"

#include "FTFP_BERT.hh"
#include "G4EmStandardPhysics_option4.hh" //option4 is the most accurate for low energies, accurate ionisation, bremsstrahlung and pair production cs by using Livermore Penelope models
#include "G4StepLimiterPhysics.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : FTFP_BERT(0) {
    ReplacePhysics(new G4EmStandardPhysics_option4(0));
    RegisterPhysics(new G4StepLimiterPhysics());

    //Set production thresholds 
    SetDefaultCutValue(0.1*mm);
}
