#include "ActionInitialization.hh"
#include "G4RunManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"

ActionInitialization::ActionInitialization(G4RunManager* runManager) : G4VUserActionInitialization(), fRunManager(runManager) {}

ActionInitialization::~ActionInitialization() {}

void ActionInitialization::BuildForMaster() const {
  SetUserAction(new RunAction());
}

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction());
  SetUserAction(new RunAction());
  SetUserAction(new EventAction());
  SetUserAction(new SteppingAction());
}