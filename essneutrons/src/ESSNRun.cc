/// \file ESSNRun.cc
/// \brief Implementation of the ESSNRun class; adapted from
///        Geant4 example B02 B02Run.cc
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//  (Description: from B02Run.cc)
//    B02Run Class is for accumulating scored quantities which is
//  scored using G4MutiFunctionalDetector and G4VPrimitiveScorer.
//  Accumulation is done using G4THitsMap object.
//
//    The constructor B02Run(const std::vector<G4String> mfdName)
//  needs a vector filled with MultiFunctionalDetector names which
//  was assigned at instantiation of MultiFunctionalDetector(MFD).
//  Then B02Run constructor automatically scans primitive scorers
//  in the MFD, and obtains collectionIDs of all collections associated
//  to those primitive scorers. Futhermore, the G4THitsMap objects
//  for accumulating during a RUN are automatically created too.
//  (*) Collection Name is same as primitive scorer name.
//
//    The resultant information is kept inside B02Run objects as
//  data members.
//  std::vector<G4String> fCollName;            // Collection Name,
//  std::vector<G4int> fCollID;                 // Collection ID,
//  std::vector<G4THitsMap<G4double>*> fRunMap; // HitsMap for RUN.
//
//  The resualtant HitsMap objects are obtain using access method,
//  GetHitsMap(..).
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ESSNAnalysis.hh"
#include "ESSNRun.hh"
#include "G4SDManager.hh"
#include "G4THitsMap.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"

//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//  Constructor.
//   (The vector of MultiFunctionalDetector name has to given.)
ESSNRun::ESSNRun(const std::vector<G4String> mfdName): G4Run()
{

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  //=================================================
  //  Initalize RunMaps for accumulation.
  //  Get CollectionIDs for HitCollections.
  //=================================================
  G4int Nmfd = mfdName.size();
  for ( G4int idet = 0; idet < Nmfd ; idet++){  // Loop for all MFD.
    G4String detName = mfdName[idet];
    //--- Seek and Obtain MFD objects from SDmanager.
    G4MultiFunctionalDetector* mfd =
      (G4MultiFunctionalDetector*)(SDman->FindSensitiveDetector(detName));
    //
    if ( mfd ){
    //--- Loop over the registered primitive scorers.
        for (G4int icol = 0; icol < mfd->GetNumberOfPrimitives(); icol++){
    // Get Primitive Scorer object.
            G4VPrimitiveScorer* scorer=mfd->GetPrimitive(icol);
    // collection name and collectionID for HitsCollection,
    // where type of HitsCollection is G4THitsMap in case of primitive scorer.
    // The collection name is given by <MFD name>/<Primitive Scorer name>.
            G4String collectionName = scorer->GetName();
            G4String fullCollectionName = detName+"/"+collectionName;
            G4int    collectionID = SDman->GetCollectionID(fullCollectionName);

            if ( collectionID >= 0 ){
                G4cout << "++ "<<fullCollectionName<< " id " << collectionID
                       << G4endl;
    // Store obtained HitsCollection information into data members.
    // And, creates new G4THitsMap for accumulating quantities during RUN.
                fCollName.push_back(fullCollectionName);
                fCollID.push_back(collectionID);
                fRunMap.push_back(new G4THitsMap<G4double>(detName
                                                          ,collectionName));
            }else{
                G4cout << "** collection " << fullCollectionName
                       << " not found. "<<G4endl;
            }
        }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// Destructor
//    clear all data members.
ESSNRun::~ESSNRun()
{
  //--- Clear HitsMap for RUN
  G4int Nmap = fRunMap.size();
  for ( G4int i = 0; i < Nmap; i++){
    if(fRunMap[i] ) fRunMap[i]->clear();
  }
  fCollName.clear();
  fCollID.clear();
  fRunMap.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//  RecordEvent is called at end of event.
//  For scoring purpose, the resultant quantity in a event,
//  is accumulated during a Run.
void ESSNRun::RecordEvent(const G4Event* aEvent)
{
  numberOfEvent++;  // This is an original line.

  //=============================
  // HitsCollection of This Event
  //============================
  G4HCofThisEvent* HCE = aEvent->GetHCofThisEvent();
  if (!HCE) return;

  //=======================================================
  // Sum up HitsMap of this Event  into HitsMap of this RUN
  //=======================================================
  G4int Ncol = fCollID.size();
  for ( G4int i = 0; i < Ncol ; i++ ){  // Loop over HitsCollection
    G4THitsMap<G4double>* EvtMap=0;
    if ( fCollID[i] >= 0 ){           // Collection is attached to HCE
      EvtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID[i]));
    }else{
      G4cout <<" Error EvtMap Not Found "<< i << G4endl;
    }
    if ( EvtMap )  {
      //=== Sum up HitsMap of this event to HitsMap of RUN.===
      if(fCollName[i] == "ConcreteSD/WeightEnterSq") {  // sum the squares for WeightEnterSq
        G4THitsMap<G4double> sqmap;
        std::map<G4int,G4double*>::iterator itr = EvtMap->GetMap()->begin();
        for(; itr != EvtMap->GetMap()->end(); itr++) {
            G4double addVal = (*(itr->second))*(*(itr->second));
            sqmap.add(itr->first,addVal);
        }
        *fRunMap[i] += sqmap;
      }
      else {
        *fRunMap[i] += *EvtMap;
      }
      //======================================================
    }
  }


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
//  Access method for HitsMap of the RUN
//
//-----
// Access HitsMap.
//  By  MultiFunctionalDetector name and Collection Name.
G4THitsMap<G4double>* ESSNRun::GetHitsMap(const G4String& detName,
                                         const G4String& colName){
    G4String fullName = detName+"/"+colName;
    return GetHitsMap(fullName);
}

//-----
// Access HitsMap.
//  By full description of collection name, that is
//    <MultiFunctional Detector Name>/<Primitive Scorer Name>
G4THitsMap<G4double>* ESSNRun::GetHitsMap(const G4String& fullName){
    G4int Ncol = fCollName.size();
    for ( G4int i = 0; i < Ncol; i++){
        if ( fCollName[i] == fullName ){
            return fRunMap[i];
        }
    }
    return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// - Dump All HitsMap of this RUN. (for debuging and monitoring of quantity).
//   This method calls G4THisMap::PrintAll() for individual HitsMap.
void ESSNRun::DumpAllScorer(){

  // - Number of HitsMap in this RUN.
  G4int n = GetNumberOfHitsMap();
  // - GetHitsMap and dump values.
  for ( G4int i = 0; i < n ; i++ ){
    G4THitsMap<G4double>* RunMap =GetHitsMap(i);
    if ( RunMap ) {
      G4cout << " PrimitiveScorer RUN "
             << RunMap->GetSDname() <<","<< RunMap->GetName() << G4endl;
      G4cout << " Number of entries " << RunMap->entries() << G4endl;
      std::map<G4int,G4double*>::iterator itr = RunMap->GetMap()->begin();
      for(; itr != RunMap->GetMap()->end(); itr++) {
        G4cout << "  copy no.: " << itr->first
               << "  Run Value : " << *(itr->second)
               << G4endl;
      }
    }
  }
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ESSNRun::Merge(const G4Run* aRun)
{
  const ESSNRun * localRun = static_cast<const ESSNRun *>(aRun);
  //=======================================================
  // Merge HitsMap of working threads
  //=======================================================
  G4int nCol = localRun->fCollID.size();
  for ( G4int i = 0; i < nCol ; i++ ){  // Loop over HitsCollection
    if ( localRun->fCollID[i] >= 0 ){
      *fRunMap[i] += *localRun->fRunMap[i];
    }
  }

  G4Run::Merge(aRun);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
