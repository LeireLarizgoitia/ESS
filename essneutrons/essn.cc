/// \file essn.cc
/// \brief Main function for ESS neutron simulation; adapted from
///        Geant4 example B5 exampleB5.cc

#define G4VIS_USE 1
#define G4UI_USE 1
#define G4MULTITHREADED 1

#include "ESSNPhysicsList.hh"
#include "ESSNCompleteDetectorConstruction.hh"
#include "ESSNImportanceDetectorConstruction.hh"
#include "ESSNActionInitialization.hh"
#include "ESSNParam.hh"

#include "G4Timer.hh"
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4GeometryManager.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4ImportanceBiasing.hh"
#include "G4ParallelWorldPhysics.hh"

// Files specific for biasing and scoring
#include "G4GeometrySampler.hh"
#include "G4IStore.hh"
#include "G4VWeightWindowStore.hh"
#include "G4WeightWindowAlgorithm.hh"
#include "G4WeightWindowBiasing.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{

  G4int mode = 0;
  if (argc>1)  mode = atoi(argv[1]);
  if(mode != 0) {
    G4cout << " mode not used yet, refer to B01 to see WeightWindow technique "
           << mode << G4endl;
  }
  G4int numberOfEvents = 1;
  // some user defined parameters
  if(SCAN==true){
    numberOfEvents = 1;
  }
  else{
    numberOfEvents = 20000;
  }
  G4long myseed = 345354;


#ifdef G4MULTITHREADED
  G4MTRunManager * runManager = new G4MTRunManager;
  G4cout << " Number of cores: " << G4Threading::G4GetNumberOfCores() << G4endl;
  //G4cout << " but using two! " << G4endl;
  //runManager->SetNumberOfThreads(2);
  runManager->SetNumberOfThreads(1);
  //runManager->SetNumberOfThreads(G4Threading::G4GetNumberOfCores());
#else
  G4RunManager * runManager = new G4RunManager;
#endif

  G4Random::setTheSeed(myseed);

  G4VWeightWindowAlgorithm *wwAlg = 0; // pointer for WeightWindow (mode>0)

  // create the detector      ---------------------------
  ESSNCompleteDetectorConstruction* detector =
                   new ESSNCompleteDetectorConstruction();
  runManager->SetUserInitialization(detector);

  G4String parallelName("ParallelBiasingWorld");
  ESSNImportanceDetectorConstruction* pdet =
                   new ESSNImportanceDetectorConstruction(parallelName);
  detector->RegisterParallelWorld(pdet);

  G4GeometrySampler pgs(pdet->GetWorldVolume(),"neutron");
  pgs.SetParallel(true);

  // Physics list initialization
  auto physicsList = new ESSNPhysicsList();
  //physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  if(PRE == true) {
    physicsList->RegisterPhysics(new G4ImportanceBiasing(&pgs,parallelName));
  }
  else if(PRE == false) {
    wwAlg = new G4WeightWindowAlgorithm(C_U,    // upper limit factor, C_U=beta
					C_S,    // survival factor C_S
					split); // max. number of splitting

    physicsList->RegisterPhysics(new G4WeightWindowBiasing(&pgs, wwAlg, onBoundary,parallelName));
  }
  physicsList->RegisterPhysics(new G4ParallelWorldPhysics(parallelName));
  runManager->SetUserInitialization(physicsList);

  // Set user action classes through Worker Initialization
  //
  ESSNActionInitialization* actions = new ESSNActionInitialization;
  runManager->SetUserInitialization(actions);

  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  //temporary fix before runManager->BeamOn works...
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  // Start the Run timer

  G4Timer* timer = new G4Timer;
  timer->Start();
  G4cout << "Starting the run timer" << G4endl;

  if (PRE == true) {
    pdet->CreateImportanceStore();
  }
  if (PRE == false) {
    pdet->CreateWeightWindowStore();
  }
  if(mode ==3){
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
#ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    G4cout << "EXECUTING VISUALIZATION" << G4endl;
#else
    UImanager->ApplyCommand("/control/execute init.mac");
#endif
    ui->SessionStart();
    delete ui;
#endif
  }
  if(mode!=3){
    G4String command1 = "/control/cout/setCoutFile threadOut";
    UImanager->ApplyCommand(command1);
    //G4String command2 = "/run/beamOn " +
    //  G4UIcommand::ConvertToString(numberOfEvents);
    G4String command2 = "/control/execute beam.mac";
    UImanager->ApplyCommand(command2);

    //Print out the timing and statistics information
    timer->Stop();

    // Calculate the FOMs
    G4double T = timer->GetRealElapsed();
    G4int zeroCells=0;
    std::fstream reFile("Results.txt");
    G4double reSum = 0.0;
    G4double re = 0.0;
    G4double dummy;
    G4double stdreSum1 = 0.0;
    G4double stdreSum2 = 0.0;
    for (G4int loop=0; loop < noCells; loop++) {  //The number of cells
      reFile >> dummy;
      reFile >> dummy;
      reFile >> re;
      if (re==0) {zeroCells++;}
      reSum = reSum+re*re;
      stdreSum1 = stdreSum1+re*re;
      stdreSum2 = stdreSum2+re;
    }
    reFile.close();

    G4double NN = noCells - zeroCells;
    G4double FOM_1 = 1.0/(reSum*T/NN);   //FOM_1
    G4double FOM_2 = std::sqrt(1/NN*(stdreSum1-1/NN*stdreSum2*stdreSum2));

    G4cout << "|-----------------------Timing and Statistics of the Run-----------------------|" << G4endl;
    G4cout << "Time of run:                " << T << " seconds " << G4endl;
    G4cout << "Total Number of cells:      " << noCells << G4endl;
    G4cout << "Number of zero cells:       " << zeroCells << G4endl;
    G4cout << "FOM_1:                      " << FOM_1 << G4endl;
    G4cout << "FOM_2:                      " << FOM_2 << G4endl;
    G4cout << "|------------------------------------------------------------------------------|" << G4endl;
  }


  // open geometry for clean biasing stores clean-up
  //
  G4GeometryManager::GetInstance()->OpenGeometry();

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !
  if (wwAlg) {
    delete wwAlg;
  }
  pgs.ClearSampling();
#ifdef G4VIS_USE
  delete visManager;
#endif
  //delete runManager;

  return 0;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
