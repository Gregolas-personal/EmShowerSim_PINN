#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fGun; }

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    fGun = new G4ParticleGun(1);

    auto* pTable = G4ParticleTable::GetParticleTable();
    fGun->SetParticleDefinition(pTable->FindParticle("e-"));
    fGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fGun->SetParticlePosition(G4ThreeVector(0., 0., -25. * cm));
    fGun->SetParticleEnergy(10.0*GeV);
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event){
    fGun->GeneratePrimaryVertex(event);
}