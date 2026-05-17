#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "G4VTouchable.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "SensitiveDet.hh"

SensitiveDet::SensitiveDet(const G4String& name, const G4String& hitsCollectionName) : G4VSensitiveDetector(name) {
  collectionName.insert(hitsCollectionName);
}
SensitiveDet::~SensitiveDet() {}

void SensitiveDet::Initialize(G4HCofThisEvent* hce) {
    //G4String hcName = "HitsCollection";
    fHitsCollection = new SensitiveHitsCollection(SensitiveDetectorName, collectionName[0]);
    
   G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
   hce->AddHitsCollection(hcID, fHitsCollection);

    for(G4int i=0; i<4; ++i) {
        SensitiveHit* hit = new SensitiveHit();
        hit->SetNumber(i);
        fHitsCollection->insert(hit);
    }
}

G4bool SensitiveDet::ProcessHits(G4Step* step, G4TouchableHistory* th) {
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep == 0.) return false;

    G4int copyNo = step->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber();

    SensitiveHit* hit = (*fHitsCollection)[copyNo];
    hit->SetEdep(edep);

    return true;
}

void SensitiveDet::EndOfEvent(G4HCofThisEvent* hce) {
   G4cout << "\n-------->" <<  fHitsCollection->GetName()
         << ": in this event: " << G4endl;
  
  G4int nofHits = fHitsCollection->entries();
  for ( G4int i=0; i<nofHits; i++ ) {
    SensitiveHit* hit = (*fHitsCollection)[i];
    if ( hit->GetEdep() > 0 ) {
      hit->Print();
    }
  }
}