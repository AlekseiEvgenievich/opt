#include "PrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Electron.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);
  fParticleGun->SetParticleDefinition(G4Electron::Electron());
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, 20*cm));
  fParticleGun->SetParticleEnergy(2*MeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, -1));
 /* fParticleGun->SetParticleDefinition(G4OpticalPhoton::OpticalPhoton());
  fParticleGun->SetParticleEnergy(2*eV);*/

 /* G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("proton");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleEnergy(100*MeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0, 0, -10*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));*/
  fParticleSource = new G4GeneralParticleSource();
  sourceGun = false;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
  delete fParticleGun;
  delete fParticleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent) {
  if (sourceGun) {
    fParticleGun->GeneratePrimaryVertex(anEvent);
  } else {
    fParticleSource->GeneratePrimaryVertex(anEvent);
    //fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  //fParticleGun->GeneratePrimaryVertex(anEvent);
}