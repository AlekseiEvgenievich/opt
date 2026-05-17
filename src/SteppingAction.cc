#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"
#include "G4StepPoint.hh"
#include "G4VPhysicalVolume.hh"

SteppingAction::SteppingAction()
  : G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}


void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto* track = step->GetTrack();
    auto* prePoint  = step->GetPreStepPoint();
    auto* postPoint = step->GetPostStepPoint();

    auto* prePV  = prePoint->GetPhysicalVolume();
    auto* postPV = postPoint->GetPhysicalVolume();

    const G4String preName  = prePV  ? prePV->GetName()  : "";
    const G4String postName = postPV ? postPV->GetName() : "";

    // ---------------------------------------------------------
    // 1) Optical photons
    // ---------------------------------------------------------
    if (track->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {

        if (postPoint->GetStepStatus() != fGeomBoundary) return;

         if (step->GetPostStepPoint()->GetStepStatus() == fGeomBoundary) {
       // G4cout << "Boundary crossing: " << preName << " -> " << postName << G4endl;
    }

        G4int trackID = track->GetTrackID();

         
    if (preName !="Detector" && postName == "Detector") {
       // G4cout << "ENTER DETECTOR" << G4endl;
        EventAction::MarkEntered(track->GetTrackID(), 0);
        return;
    }

    if (preName == "Detector" && postName !="Detector") {
       // G4cout << "RETURN TO GREASE" << G4endl;
       // G4cout << "================================" << G4endl;
       EventAction::MarkReturned(track->GetTrackID());
        return;
    }
    if (preName == "Detector" && postName == "World") {
       G4cout << "KILLED INSIDE DETECTOR" << G4endl;
      EventAction::MarkReturned(trackID);
        return;
    }
   // G4cout << "================================" << G4endl;

        return;
    }

    // ---------------------------------------------------------
    // 2) Count generated scintillation photons
    // ---------------------------------------------------------
    if (!preName.contains("Cube")) return;

    const auto* secondaries = step->GetSecondaryInCurrentStep();
    if (!secondaries) return;

    for (const auto* sec : *secondaries) {
        if (sec->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) continue;
        const auto* creator = sec->GetCreatorProcess();
        if (creator && creator->GetProcessName() == "Scintillation") {
            EventAction::AddNgen(1);
        }
    }
}