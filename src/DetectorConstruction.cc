#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "SensitiveDet.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Tubs.hh"

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fScoringVolume(nullptr) {}

DetectorConstruction::~DetectorConstruction() {}


G4VPhysicalVolume* DetectorConstruction::Construct() {
  G4NistManager* nist = G4NistManager::Instance();

  // World
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  G4double world_size = 1*m;

  G4Box* solidWorld = new G4Box("World", 0.5*world_size, 0.5*world_size, 0.5*world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld =
      new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

  // Elements for LYSO
  G4Element* Lu = nist->FindOrBuildElement("Lu");
  G4Element* Y  = nist->FindOrBuildElement("Y");
  G4Element* Si = nist->FindOrBuildElement("Si");
  G4Element* O  = nist->FindOrBuildElement("O");
  G4Element* C  = nist->FindOrBuildElement("C");
  G4Element* H  = nist->FindOrBuildElement("H");

  // LYSO
  G4Material* LYSO = new G4Material("LYSO", 7.1*g/cm3, 4);
  LYSO->AddElement(Lu, 0.714);
  LYSO->AddElement(Y,  0.040);
  LYSO->AddElement(Si, 0.063);
  LYSO->AddElement(O,  0.183);

  G4Material* mat_maylar   = nist->FindOrBuildMaterial("G4_MYLAR");
  //G4Material* mat_detector = nist->FindOrBuildMaterial("G4_AIR");
  G4Material* mat_detector = new G4Material("DetectorMat", 1.0*g/cm3, 2);
  mat_detector->AddElement(C, 1);
  mat_detector->AddElement(H, 1);
  G4Material* mat_teflon = nist->FindOrBuildMaterial("G4_TEFLON");
  G4Material* mat_quartz = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // Custom optical grease (simple silicone-like approximation)
  G4Material* mat_grease = new G4Material("OpticalGrease", 0.97*g/cm3, 4);
  mat_grease->AddElement(C, 2);
  mat_grease->AddElement(H, 6);
  mat_grease->AddElement(O, 1);
  mat_grease->AddElement(Si, 1);

  // Photon energy range
  G4double photonEnergy[] = {2.0*eV, 2.5*eV, 3.0*eV};
  const G4int numEntries = 3;

  // ---------------------------
  // Optical properties for LYSO
  // ---------------------------
const G4int numLYSO = 11;

G4double photonEnergyLYSO[numLYSO] = {
    2.2545 * eV, // 550 nm
    2.3846 * eV, // 520 nm
    2.4800 * eV, // 500 nm
    2.5833 * eV, // 480 nm
    2.6957 * eV, // 460 nm
    2.8182 * eV, // 440 nm
    2.9524 * eV, // 420 nm
    3.1000 * eV, // 400 nm
    3.2632 * eV, // 380 nm
    3.4444 * eV, // 360 nm
    3.6471 * eV  // 340 nm
};

G4double lysoRIndex[numLYSO] = {
    1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82, 1.82
};

G4double lysoAbsLength[numLYSO] = {
    20.0 * cm, 20.0 * cm, 20.0 * cm, 20.0 * cm, 20.0 * cm,
    20.0 * cm, 20.0 * cm, 20.0 * cm, 20.0 * cm, 20.0 * cm, 20.0 * cm
};

// Relative emission intensity (arbitrary units)
// Peak around 420 nm
G4double lysoFast[numLYSO] = {
    0.05, // 550 nm
    0.10, // 520 nm
    0.18, // 500 nm
    0.35, // 480 nm
    0.60, // 460 nm
    0.85, // 440 nm
    1.00, // 420 nm
    0.80, // 400 nm
    0.45, // 380 nm
    0.18, // 360 nm
    0.05  // 340 nm
};

G4MaterialPropertiesTable* mptLYSO = new G4MaterialPropertiesTable();
mptLYSO->AddProperty("RINDEX", photonEnergyLYSO, lysoRIndex, numLYSO)->SetSpline(true);
mptLYSO->AddProperty("ABSLENGTH", photonEnergyLYSO, lysoAbsLength, numLYSO)->SetSpline(true);
mptLYSO->AddProperty("FASTCOMPONENT", photonEnergyLYSO, lysoFast, numLYSO)->SetSpline(true);

mptLYSO->AddConstProperty("SCINTILLATIONYIELD", 32000.0 / MeV);
mptLYSO->AddConstProperty("RESOLUTIONSCALE", 1.0);
mptLYSO->AddConstProperty("FASTTIMECONSTANT", 40.0 * ns);
mptLYSO->AddConstProperty("YIELDRATIO", 1.0);

LYSO->SetMaterialPropertiesTable(mptLYSO);

  // ----------------------------
  // Optical properties for Mylar
  // ----------------------------
  G4double refractiveIndex3[] = {1.55, 1.55, 1.55};
  G4double absorption3[]      = {3.5*um, 3.5*um, 3.5*um};



  G4MaterialPropertiesTable* mptMaylar = new G4MaterialPropertiesTable();
  mptMaylar->AddProperty("RINDEX",    photonEnergy, refractiveIndex3, numEntries)->SetSpline(true);
  mptMaylar->AddProperty("ABSLENGTH", photonEnergy, absorption3,      numEntries)->SetSpline(true);
  mptMaylar->DumpTable();
  mat_maylar->SetMaterialPropertiesTable(mptMaylar);

    //===============================
  // Optical properties for Teflon (for comparison)
   //===============================

   G4double teflonRIndex[] = {1.35, 1.35, 1.35};
   G4double teflonAbs[]    = {1.0*mm, 1.0*mm, 1.0*mm};
   G4MaterialPropertiesTable* mptTeflon = new G4MaterialPropertiesTable();
   mptTeflon->AddProperty("RINDEX",    photonEnergy, teflonRIndex, numEntries)->SetSpline(true);
   mptTeflon->AddProperty("ABSLENGTH", photonEnergy, teflonAbs,    numEntries)->SetSpline(true);
   mat_teflon->SetMaterialPropertiesTable(mptTeflon); 

 //==============================
 //Opticl properties for Quartz (for comparison)
 //==============================
 G4double qR[]   = {1.46,1.46,1.46};
 G4double qAbs[] = {1*m,1*m,1*m};
 auto* mptQ = new G4MaterialPropertiesTable();
 mptQ->AddProperty("RINDEX",    photonEnergy, qR,   numEntries)->SetSpline(true);
 mptQ->AddProperty("ABSLENGTH", photonEnergy, qAbs, numEntries)->SetSpline(true);
 mat_quartz->SetMaterialPropertiesTable(mptQ);


  // ----------------------------
  // Optical properties for World
  // ----------------------------
  G4double refractiveIndex2[] = {1.0, 1.0, 1.0};
  G4double absorption2[]      = {100.*m, 100.*m, 100.*m};

  G4MaterialPropertiesTable* mptWorld = new G4MaterialPropertiesTable();
  mptWorld->AddProperty("RINDEX",    photonEnergy, refractiveIndex2, numEntries)->SetSpline(true);
  mptWorld->AddProperty("ABSLENGTH", photonEnergy, absorption2,      numEntries)->SetSpline(true);
  mptWorld->DumpTable();
  world_mat->SetMaterialPropertiesTable(mptWorld);

  // -------------------------------
  // Optical properties for Grease
  //--------------------------------

    // Grease optical properties
  G4double greaseRIndex[] = {1.46, 1.46, 1.46};
  G4double greaseAbs[]    = {5.*cm, 5.*cm, 5.*cm};
  G4MaterialPropertiesTable* mptGrease = new G4MaterialPropertiesTable();
  mptGrease->AddProperty("RINDEX",    photonEnergy, greaseRIndex, numEntries)->SetSpline(true);
  mptGrease->AddProperty("ABSLENGTH", photonEnergy, greaseAbs,    numEntries)->SetSpline(true);
  mat_grease->SetMaterialPropertiesTable(mptGrease);
  // -------------------------------
  // Optical properties for Detector
  // -------------------------------
  G4double refractiveIndex4[] = {1.5, 1.5, 1.5};
  //G4double absorption4[]      = {1.*m, 1.*m, 1.*m};
  G4double absorption4[] = {1.*um, 1.*um, 1.*um};

  G4MaterialPropertiesTable* mptDetector = new G4MaterialPropertiesTable();
  mptDetector->AddProperty("RINDEX",    photonEnergy, refractiveIndex4, numEntries)->SetSpline(true);
  mptDetector->AddProperty("ABSLENGTH", photonEnergy, absorption4,      numEntries)->SetSpline(true);
  mptDetector->DumpTable();
  mat_detector->SetMaterialPropertiesTable(mptDetector);

  G4double cube_size_x = 3*cm;
G4double cube_size_y = 2.5*cm;
G4double cube_size_z = 2.5*cm;

G4double wrap = 0.1*mm;

G4Box* solidCube = new G4Box("Cube", 0.5*cube_size_x, 0.5*cube_size_y, 0.5*cube_size_z);
G4LogicalVolume* logicCube = new G4LogicalVolume(solidCube, LYSO, "Cube");
G4PVPlacement* physCube =
    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicCube, "Cube", logicWorld, false, 0, true);

// Frame
G4Box* box_1 = new G4Box("Box1", 0.5*cube_size_x, 0.5*cube_size_y, 0.5*cube_size_z);
G4Box* box_2 = new G4Box("Box2",
                         0.5*cube_size_x + wrap,
                         0.5*cube_size_y + wrap,
                         0.5*cube_size_z + wrap);

// Hole through the back wrapping
G4double backCenterZ = -(0.5*cube_size_z + wrap)/2.0 - 0.5*cube_size_z/2.0;
// проще явно:
//backCenterZ = -(1.25*cm + 0.05*mm);

G4Box* hole = new G4Box("Hole", 1.5*mm, 1.5*mm, wrap);

G4SubtractionSolid* solidFrame = new G4SubtractionSolid("Frame", box_2, box_1);
G4SubtractionSolid* finalFrame =
    new G4SubtractionSolid("FinalFrame", solidFrame, hole, 0,
                           G4ThreeVector(0, 0, -(0.5*cube_size_z + 0.5*wrap)));

G4LogicalVolume* logicFrame = new G4LogicalVolume(finalFrame, mat_maylar, "Frame");
G4PVPlacement* physFrame =
    new G4PVPlacement(0, G4ThreeVector(), logicFrame, "Frame", logicWorld, false, 0, true);

G4double holeHX = 1.5*mm;
G4double holeHY = 1.5*mm;
G4double greaseHalfZ = 0.5*(wrap);

G4Box* solidGrease = new G4Box("GreasePlug", holeHX, holeHY, greaseHalfZ);
G4ThreeVector greasePos(0, 0, -(0.5*cube_size_z + greaseHalfZ));
  G4LogicalVolume* logicGrease = new G4LogicalVolume(solidGrease, mat_grease, "Grease");
  G4PVPlacement* physGrease =
      new G4PVPlacement(0, greasePos, logicGrease, "Grease", logicWorld, false, 0, true);

//  quartz plug in front of detector (for comparison)
/*G4double quartzThick = 0.5*mm;
G4double quartzHalfZ = 0.5*quartzThick;
G4Box* solidQuartz = new G4Box("QuartzPlug", 1.5*mm, 1.5*mm, quartzHalfZ);
G4LogicalVolume* logicQuartz = new G4LogicalVolume(solidQuartz, mat_quartz, "Quartz");
G4PVPlacement* physQuartz =
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(0.5*cube_size_z + wrap + quartzHalfZ)),
                      logicQuartz, "Quartz", logicWorld, false, 0, true);  */    


// Detector right behind frame
G4Box* solidDetector = new G4Box("Detector", 1.5*mm, 1.5*mm, 1*mm);
G4LogicalVolume* logicDetector = new G4LogicalVolume(solidDetector, mat_detector, "Detector");
G4PVPlacement* physDetector =
    new G4PVPlacement(0, G4ThreeVector(0, 0, -(0.5*cube_size_z + wrap + 1*mm)),
                      logicDetector, "Detector", logicWorld, false, 0, true);

  // -----------------------------------------
  // Optical surface between Cube and Mylar
  // -----------------------------------------
  G4double refs[] = {0.95, 0.95, 0.95};
   G4double refs_teflon[] = {0.995, 0.995, 0.995};
  G4OpticalSurface* opSurface = new G4OpticalSurface("ScinRefSurface");
  opSurface->SetType(dielectric_metal);
  opSurface->SetFinish(ground);
  opSurface->SetModel(unified);

  G4MaterialPropertiesTable* surfMPT = new G4MaterialPropertiesTable();
  surfMPT->AddProperty("REFLECTIVITY", photonEnergy, refs, numEntries)->SetSpline(true);
  opSurface->SetMaterialPropertiesTable(surfMPT);

  new G4LogicalBorderSurface("ScinRefSurface", physCube, physFrame, opSurface);

  // -----------------------------------------
  // Surface for Detector
  // -----------------------------------------
  G4double refs2[] = {0.0, 0.0, 0.0};

  G4OpticalSurface* opSurface2 = new G4OpticalSurface("WorldDetectorSurface");
  opSurface2->SetType(dielectric_dielectric);
  opSurface2->SetFinish(ground);
  opSurface2->SetModel(unified);

  G4MaterialPropertiesTable* surfMPT2 = new G4MaterialPropertiesTable();
  //surfMPT2->AddProperty("REFLECTIVITY", photonEnergy, refs2, numEntries)->SetSpline(true);
  opSurface2->SetMaterialPropertiesTable(surfMPT2);

  new G4LogicalBorderSurface("WorldDetectorSurface", physDetector, physWorld, opSurface2);

    // 3) LYSO-Grease (high transmission coupling)
 //G4double reflLysoGrease[] = {0.01, 0.01, 0.01}; // low reflection
 //G4double effLysoGrease[]  = {1.0, 1.0, 1.0};

  G4OpticalSurface* opLysoGrease = new G4OpticalSurface("LysoGreaseSurface");
  opLysoGrease->SetType(dielectric_dielectric);
  opLysoGrease->SetFinish(polished);
  opLysoGrease->SetModel(unified);

  G4MaterialPropertiesTable* mptLysoGrease = new G4MaterialPropertiesTable();
  //mptLysoGrease->AddProperty("REFLECTIVITY", photonEnergy, reflLysoGrease, numEntries)->SetSpline(true);
  //mptLysoGrease->AddProperty("EFFICIENCY",   photonEnergy, effLysoGrease,  numEntries)->SetSpline(true);
  opLysoGrease->SetMaterialPropertiesTable(mptLysoGrease);

  new G4LogicalBorderSurface("LysoToGrease", physCube, physGrease, opLysoGrease);
  new G4LogicalBorderSurface("GreaseToLyso", physGrease, physCube, opLysoGrease);


// 3) Grease-Detector (high transmission coupling)
 // G4double pde[]  = {0.40, 0.40, 0.40};   // или твою PDE(lambda)
//G4double reflDet[] = {0.0, 0.0, 0.0};

/*G4OpticalSurface* opGreaseDet = new G4OpticalSurface("GreaseDetectorSurface");
opGreaseDet->SetType(dielectric_metal);
opGreaseDet->SetFinish(polished);
opGreaseDet->SetModel(unified);

G4MaterialPropertiesTable* mptGreaseDet = new G4MaterialPropertiesTable();
//mptGreaseDet->AddProperty("EFFICIENCY", photonEnergy, pde, numEntries)->SetSpline(true);
//mptGreaseDet->AddProperty("REFLECTIVITY", photonEnergy, reflDet, numEntries)->SetSpline(true);
opGreaseDet->SetMaterialPropertiesTable(mptGreaseDet);

new G4LogicalBorderSurface("GreaseToDetector", physGrease, physDetector, opGreaseDet);*/
// NEW Grease <-> Quartz
/*G4OpticalSurface* sGreaseQuartz = new G4OpticalSurface("GreaseQuartz");
sGreaseQuartz->SetType(dielectric_dielectric);
sGreaseQuartz->SetModel(unified);
sGreaseQuartz->SetFinish(polished);
new G4LogicalBorderSurface("GreaseToQuartz", physGrease, physQuartz, sGreaseQuartz);

// Quartz -> Detector (3x3 only): detection surface
G4OpticalSurface* opQuartzDet3x3 = new G4OpticalSurface("QuartzDetSurface_3x3");
opQuartzDet3x3->SetType(dielectric_metal);
opQuartzDet3x3->SetModel(unified);
opQuartzDet3x3->SetFinish(polished);

// border surface for 3x3 channel only
new G4LogicalBorderSurface("QuartzToDetector_3x3", physQuartz, physDetector, opQuartzDet3x3);
new G4LogicalBorderSurface("DetectorToQuartz_3x3", physDetector, physQuartz, opQuartzDet3x3);*/


  return physWorld;
}




void DetectorConstruction::ConstructSDandField() {
  G4String sdName = "SensitiveDetector";
  SensitiveDet* aSD = new SensitiveDet(sdName, "HitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(aSD);

  // НЕ меняем: sensitive detector остаётся на Cube
  SetSensitiveDetector("Cube", aSD);
}