#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* DetectorConstruction::Construct() {

    auto* nist = G4NistManager::Instance();
    auto* Pb = nist->FindOrBuildMaterial("G4_Pb");
    auto* LAr = nist->FindOrBuildMaterial("G4_lAr");

    // World
    auto* worldS = new G4Box("World", 50*cm, 50*cm, 60*cm);
    auto* worldLV = new G4LogicalVolume(worldS, LAr, "World");
    auto* worldPV = new G4PVPlacement(0,G4ThreeVector(), worldLV, "World", nullptr, false, 0);

    const G4int nLayers = 50;
    const G4double absTh = 2.0*mm;
    const G4double actTh = 4.0*mm;
    const G4double layerTh = absTh + actTh;
    const G4double calHalf = 20*cm;

    for(G4int i = 0; i < nLayers; ++i){
        G4double zAbs = -calHalf + i*layerTh + absTh/2.0;
        G4double zAct = zAbs + absTh/2.0 + actTh/2.0;

        // ----- Absorber(Pb) -----
        auto* absS = new G4Box("Abs", 20*cm, 20*cm, absTh/2.0);
        auto* absLV = new G4LogicalVolume(absS, Pb, "Absorber");
        new G4PVPlacement(0, G4ThreeVector(0, 0, zAbs), absLV, "Absorber", worldLV, false, i); 

        // ----- Active(LAr) -----
        auto* actS = new G4Box("Active", 20*cm, 20*cm, actTh/2.0);
        auto* actLV = new G4LogicalVolume(actS, LAr, "Active");
        new G4PVPlacement(0, G4ThreeVector(0, 0, zAct), actLV, "Active", worldLV, false, i); 

        fActiveLV.push_back(actLV);
    }
    return worldPV;
}

void DetectorConstruction::ConstructSDandField(){
    auto* sd = new SensitiveDetector("CaloSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(sd);
    for (auto* lv : fActiveLV)
        SetSensitiveDetector(lv, sd);
}