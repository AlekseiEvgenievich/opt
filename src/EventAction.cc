#include "EventAction.hh"
#include "RunAction.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4ios.hh" 
#include "SensitiveHit.hh"
#include "SensitiveDet.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

std::unordered_map<G4int, G4int> EventAction::fAcceptedTracks;

G4int EventAction::fNphotons3x3 = 0;
G4int EventAction::fNphotons1x1 = 0;
G4int EventAction::fNphotons6x6 = 0;
G4int EventAction::fNgen = 0;

EventAction::EventAction() : G4UserEventAction() {
    fHitsCollectionID = -1;
}

EventAction::~EventAction() {}

void EventAction::BeginOfEventAction(const G4Event* anEvent) {
    fAcceptedTracks.clear();
    fNphotons3x3 = 0;
    fNphotons1x1 = 0;
    fNphotons6x6 = 0;
    fNgen = 0;
}

SensitiveHitsCollection* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const {
    G4HCofThisEvent* hce = event->GetHCofThisEvent();
    if (!hce) {
        G4ExceptionDescription msg;
        msg << "No hits collection of this event found.";
        G4Exception("EventAction::GetHitsCollection()", "MyCode0002", JustWarning, msg);
        return nullptr;
    }

    SensitiveHitsCollection* hitsCollection = static_cast<SensitiveHitsCollection*>(hce->GetHC(hcID));
    if (!hitsCollection) {
        G4ExceptionDescription msg;
        msg << "Cannot access hits collection ID " << hcID;
        G4Exception("EventAction::GetHitsCollection()", "MyCode0003", JustWarning, msg);
        return nullptr;
    }

    return hitsCollection;
}

void EventAction::EndOfEventAction(const G4Event* anEvent) {
    if (fHitsCollectionID == -1) {
        fHitsCollectionID = G4SDManager::GetSDMpointer()->GetCollectionID("HitsCollection");
        G4cout << "HitsCollection ID: " << fHitsCollectionID << G4endl;
    }

     SensitiveHitsCollection* hitsCollection = GetHitsCollection(fHitsCollectionID, anEvent);
    if (!hitsCollection) {
        G4ExceptionDescription msg;
        msg << "Cannot access hits collection ID " << fHitsCollectionID;
        G4Exception("EventAction::EndOfEventAction()", "MyCode0001", JustWarning, msg);
        return;
    }

    // all photons that entered detector and never returned to grease are counted
    for (const auto& kv : fAcceptedTracks) {
        G4int detId = kv.second;
        if      (detId == 0) ++fNphotons3x3;
        else if (detId == 1) ++fNphotons1x1;
        else if (detId == 2) ++fNphotons6x6;
    }

    G4cout << "Photons accepted in Detector 3x3: " << fNphotons3x3 << G4endl;
    G4cout << "Photons accepted in Detector 1x1: " << fNphotons1x1 << G4endl;
    G4cout << "Photons accepted in Detector 6x6: " << fNphotons6x6 << G4endl;
    G4cout << "Generated scintillation photons: " << fNgen << G4endl;
     RunAction::FillTreephotons(fNphotons6x6, fNphotons3x3, fNphotons1x1, fNgen);
  /*  G4int nofHits = hitsCollection->entries();
    G4cout << "Number of hits in this event: " << nofHits << G4endl;
    if (nofHits > 0) {
        double edep1 = (*hitsCollection)[0]->GetEdep();
        double edep2 = (*hitsCollection)[1]->GetEdep();
        double edep3 = (*hitsCollection)[2]->GetEdep();
        double edep4 = (*hitsCollection)[3]->GetEdep();
        RunAction::FillTree(edep1, edep2, edep3, edep4);
    }*/

  /*  G4int n3 = GetNphotons3x3();
    G4int n1 = GetNphotons1x1();
    G4int n6 = GetNphotons6x6();

    G4cout << "Photons remaining in Detector 3x3: " << n3 << G4endl;
    G4cout << "Photons remaining in Detector 1x1: " << n1 << G4endl;
    G4cout << "Photons remaining in Detector 6x6: " << n6 << G4endl;
    G4cout << "Generated scintillation photons: " << fNgen << G4endl;

    RunAction::FillTreephotons(n6, n3, n1, fNgen);*/
 
}

void EventAction::MarkEntered(G4int trackID, G4int detId)
{
    fAcceptedTracks[trackID] = detId;
}

void EventAction::MarkReturned(G4int trackID)
{
    fAcceptedTracks.erase(trackID);
}


