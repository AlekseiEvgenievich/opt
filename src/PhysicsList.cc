#include "PhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

PhysicsList::PhysicsList() {
  RegisterPhysics(new G4EmStandardPhysics());
  G4OpticalPhysics* optPhys = new G4OpticalPhysics();
   optPhys->SetScintillationYieldFactor(1.0);

  optPhys->SetTrackSecondariesFirst(kCerenkov, false);
  optPhys->SetTrackSecondariesFirst(kScintillation, false);
  RegisterPhysics(optPhys);
  RegisterPhysics(new G4DecayPhysics());
RegisterPhysics(new G4RadioactiveDecayPhysics());
}