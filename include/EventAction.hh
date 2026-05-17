#include "G4UserEventAction.hh"

#include "SensitiveDet.hh"

#include "globals.hh"
#include <set>
#include <unordered_map>

class EventAction : public G4UserEventAction {
public:
  EventAction();
  virtual ~EventAction();
  virtual void BeginOfEventAction(const G4Event* anEvent);
  virtual void EndOfEventAction(const G4Event* anEvent);  
  inline static void AddNgen(int n) { fNgen += n; }
    static void MarkEntered(G4int trackID, G4int detId);
    static void MarkReturned(G4int trackID);
     static G4int GetNphotons3x3();
    static G4int GetNphotons1x1();
    static G4int GetNphotons6x6();
private:
    SensitiveHitsCollection* GetHitsCollection(G4int hcID, const G4Event* event) const;
    G4int fHitsCollectionID;
      static std::unordered_map<G4int, G4int> fAcceptedTracks; // trackID -> detId

    static G4int fNphotons3x3;
    static G4int fNphotons1x1;
    static G4int fNphotons6x6;
    static G4int fNgen;
};
