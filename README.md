# GEANT4 LYSO Cube Simulation

This project simulates a single LYSO (Lutetium Yttrium Orthosilicate) cube detector irradiated by protons. The simulation saves energy deposition data to a ROOT tree.

## Prerequisites

- GEANT4 installed with visualization and UI support
- ROOT installed for data analysis
- CMake

## Building the Project

1. Create a build directory:
   ```
   mkdir build
   cd build
   ```

2. Configure with CMake:
   ```
   cmake ..
   ```

3. Build:
   ```
   make
   ```

   This will automatically copy `vis.mac` and `run.mac` to the build directory.
   make
   ```

## Running the Simulation

### Interactive Mode
```
./geant4_lyso_cube
```
This opens an interactive session. Run events with `/run/beamOn N` in the interactive session to generate trajectory visualizations.

### Batch Mode
```
./geant4_lyso_cube run.mac
```

This will run 1000 events and save the output to `output.root`.

## Output

The simulation generates `output.root` containing a TTree named "Hits" with energy deposition values.

## Customization

- Cube size: Modify `cube_size` in `DetectorConstruction.cc`
- Proton energy: Change in `PrimaryGeneratorAction.cc`
- Number of events: Edit `run.mac` or use `/run/beamOn <number>` in interactive mode