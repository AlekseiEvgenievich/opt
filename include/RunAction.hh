#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"

class TFile;
class TTree;

class RunAction : public G4UserRunAction {
public:
  RunAction();
  virtual ~RunAction();
  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  static void FillTree(double edep1, double edep2, double edep3, double edep4);
  static void FillTreephotons(double number_of_photons6x6, double number_of_photons3x3, double number_of_photons1x1, double all_photons);

private:
  static TFile* fFile;
  static TTree* fTree;
  static double fEdep1, fEdep2, fEdep3, fEdep4;
  static double number_of_photons6x6;
  static double number_of_photons3x3;
  static double number_of_photons1x1;
  static double all_photons;
};

#endif