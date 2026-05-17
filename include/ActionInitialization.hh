#ifndef ActionInitialization_h
#define ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

class G4RunManager;

class ActionInitialization : public G4VUserActionInitialization {
public:
  ActionInitialization(G4RunManager* runManager);
  virtual ~ActionInitialization();
  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  G4RunManager* fRunManager;
};

#endif