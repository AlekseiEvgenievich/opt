#ifndef SENSITIVE_DET_HH
#define SENSITIVE_DET_HH
#include "G4VSensitiveDetector.hh"
#include "SensitiveHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class SensitiveDet : public G4VSensitiveDetector {
public:
    SensitiveDet(const G4String& name, const G4String& hitsCollectionName);
    virtual ~SensitiveDet();
    void Initialize(G4HCofThisEvent* hce) override;
    void EndOfEvent(G4HCofThisEvent* hce) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory* th) override;
private:
    SensitiveHitsCollection* fHitsCollection;
};
#endif