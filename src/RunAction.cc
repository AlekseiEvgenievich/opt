#include "RunAction.hh"
#include "TFile.h"
#include "TTree.h"
#include "G4ios.hh"

TFile* RunAction::fFile = nullptr;
TTree* RunAction::fTree = nullptr;
double RunAction::fEdep1 = 0.0;
double RunAction::fEdep2 = 0.0;
double RunAction::fEdep3 = 0.0;
double RunAction::fEdep4 = 0.0;
double RunAction::number_of_photons6x6 = 0.0;
double RunAction::number_of_photons3x3 = 0.0;
double RunAction::number_of_photons1x1 = 0.0;
double RunAction::all_photons = 0.0;

RunAction::RunAction() {}

RunAction::~RunAction() {}

void RunAction::BeginOfRunAction(const G4Run*) {
  fFile = new TFile("output.root", "RECREATE");
  fTree = new TTree("Hits", "Hits");
  fTree->Branch("Edep1", &fEdep1);
  fTree->Branch("Edep2", &fEdep2);
  fTree->Branch("Edep3", &fEdep3);
  fTree->Branch("Edep4", &fEdep4);
  fTree->Branch("NumberOfPhotons6x6", &number_of_photons6x6);
  fTree->Branch("NumberOfPhotons3x3", &number_of_photons3x3);
  fTree->Branch("NumberOfPhotons1x1", &number_of_photons1x1);
  fTree->Branch("AllPhotons", &all_photons);
}

void RunAction::EndOfRunAction(const G4Run*) {
  fTree->Write();
  fFile->Close();
  delete fFile;
  fFile = nullptr;
  fTree = nullptr;
}

void RunAction::FillTree(double edep1, double edep2, double edep3, double edep4) {
  fEdep1 = edep1;
  fEdep2 = edep2;
  fEdep3 = edep3;
  fEdep4 = edep4;
  //if (fTree) {
  //  G4cout << "Filling tree with: " << fEdep1 << ", " << fEdep2 << ", " << fEdep3 << ", " << fEdep4 << G4endl;
    fTree->Fill();
  //}
}

void RunAction::FillTreephotons(double number_of_photons6x6, double number_of_photons3x3, double number_of_photons1x1, double all_photons) {
  RunAction::number_of_photons6x6 = number_of_photons6x6;
  RunAction::number_of_photons3x3 = number_of_photons3x3;
  RunAction::number_of_photons1x1 = number_of_photons1x1;
  RunAction::all_photons = all_photons;
  //if (fTree) {
  //  G4cout << "Filling tree with number of photons: " << number_of_photons << G4endl;
    fTree->Fill();
  //}
}