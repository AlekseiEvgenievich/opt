#include "SensitiveHit.hh"
#include "G4UnitsTable.hh"

G4Allocator<SensitiveHit>* SensitiveHitAllocator = nullptr;



SensitiveHit::SensitiveHit() {}

SensitiveHit::~SensitiveHit() {}

SensitiveHit::SensitiveHit(const SensitiveHit& right) : G4VHit() {
  fEdep = right.fEdep;
    fNumber = right.fNumber;
}

const SensitiveHit& SensitiveHit::operator=(const SensitiveHit& right) {
  fEdep = right.fEdep;
  fNumber = right.fNumber;
  return *this;
}   

int SensitiveHit::operator==(const SensitiveHit& right) const {
  return (this == &right) ? 1 : 0;
}

void SensitiveHit::Print() {
  G4cout << "Energy deposited: " << G4BestUnit(fEdep, "Energy") << G4endl;
  G4cout << "Cell number: " << fNumber << G4endl;
}

