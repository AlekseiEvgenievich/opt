#ifndef SENSITIVE_HIT_HH
#define SENSITIVE_HIT_HH
#include <G4ThreeVector.hh>

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"

class SensitiveHit:public G4VHit {
public:
    SensitiveHit();
    virtual ~SensitiveHit();

    SensitiveHit(const SensitiveHit& right);
    const SensitiveHit& operator=(const SensitiveHit& right);
    int operator==(const SensitiveHit &right) const;

    inline void* operator new(size_t);
    inline void  operator delete(void* hit);

    void Print() override;


    G4double GetEdep() const { return fEdep; }
    void SetEdep(G4double edep) { fEdep = edep; }
    G4int GetNumber() const { return fNumber; }
    void SetNumber(G4int num) { fNumber = num; }
private:
    G4double fEdep; 
    G4int fNumber;   
};

typedef G4THitsCollection<SensitiveHit> SensitiveHitsCollection;

extern G4Allocator<SensitiveHit>* SensitiveHitAllocator;

inline void* SensitiveHit::operator new(size_t)
{
  if (! SensitiveHitAllocator)
        SensitiveHitAllocator = new G4Allocator<SensitiveHit>;
  return (void*)SensitiveHitAllocator->MallocSingle();
}

inline void SensitiveHit::operator delete(void* hit)
{
  SensitiveHitAllocator->FreeSingle((SensitiveHit*) hit);
}
#endif