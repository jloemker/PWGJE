// Copyright 2019-2020 CERN and copyright holders of ALICE O2.
// See https://alice-o2.web.cern.ch/copyright for details of the copyright holders.
// All rights not expressly granted are reserved.
//
// This software is distributed under the terms of the GNU General Public
// License v3 (GPL Version 3), copied verbatim in the file "COPYING".
//
// In applying this license CERN does not waive the privileges and immunities
// granted to it by virtue of its status as an Intergovernmental Organization
// or submit itself to any jurisdiction.
///
//  \author
//  Johanna Lömker
//  \since Dec 2022
// The goal would be to set up the jet, track and event/collision QA's for the validation framework
// Staring with the hybrid tracks - i tried, i failed, i will try again ...

// 1)Configuration for run2 validation on ESD:
// i)  turn dummy from jetTrackCollisionQa -> false and processESD -> true
// ii) turn dummy from mcJetTrackCollisionQa -> true and processMcRun3 -> false
// iii)choose "hybridTracksJE" in jetfinder configuration
// Pipe: o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} 
//      | o2-analysis-trackselection ${OPTIONS} | o2-analysis-trackextension ${OPTIONS} 
//      | o2-analysis-je-jet-finder ${OPTIONS} | o2-analysis-zdc-converter ${OPTIONS} 
//      | o2-analysis-je-jet-validation-qa ${OPTIONS}

// 2)Configuration for MC validation on run2/3 data (might need the zdc converter too):
// i)  turn dummy from jetTrackCollisionQa -> true and processESD -> false
// ii) turn dummy from mcJetTrackCollisionQa -> false and processMcRun2/3 -> true
// Pipe(run3): o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} 
//           | o2-analysis-trackselection ${OPTIONS} | o2-analysis-track-propagation ${OPTIONS} 
//           | o2-analysis-collision-converter ${OPTIONS} | o2-analysis-zdc-converter ${OPTIONS} 
//           | o2-analysis-je-jet-finder ${OPTIONS} | o2-analysis-je-jet-validation-qa ${OPTIONS} 
// Pipe(run2): o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} |  
//           | o2-analysis-trackselection ${OPTIONS} | o2-analysis-trackextension ${OPTIONS} 
//           | o2-analysis-collision-converter ${OPTIONS} | o2-analysis-zdc-converter ${OPTIONS}
//           | o2-analysis-je-jet-finder ${OPTIONS} | o2-analysis-je-jet-validation-qa ${OPTIONS} 
           

// 3)Configuration for validation on run3 data:

#include "Framework/runDataProcessing.h"
#include "Framework/AnalysisTask.h"
#include "Framework/AnalysisDataModel.h"
#include "Framework/ASoA.h"
#include "Framework/HistogramRegistry.h"

#include "PWGJE/DataModel/Jet.h"

#include "Common/DataModel/EventSelection.h"
#include "Common/Core/TrackSelection.h"
#include "Common/Core/TrackSelectionDefaults.h"

using namespace o2;
using namespace o2::framework;
using namespace o2::framework::expressions;

// tracks for 1) validation on ESD 2) Run2 MC validatio on AO2D's 3) Run2 MC validation on AO2D's
using TracksJE = soa::Join<aod::Tracks, aod::TracksExtra, aod::TracksDCA, aod::TrackSelection>;
using MCTracksRun3JE = soa::Join<aod::Tracks, aod::TracksExtra, aod::TracksDCA, aod::TrackSelection, aod::McTrackLabels>;// 
using MCTracksRun2JE = soa::Join<aod::Tracks, aod::TracksExtra, aod::TracksDCA, aod::TrackSelection, aod::McTrackLabels>;// Actually alll the same ?

// struct for jetfinder validation on run2 ESD's
struct jetTrackCollisionQa {

  HistogramRegistry mHistManager{"JetCollisionQAHistograms"};
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};// maybe adding logarithmic x-axis already ?
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};

  TrackSelection validationTracks;

  void init(InitContext const&)
  {
    //set trackselections - initial idea for now on hold
    //we should not touch the getJEGlobalTrackSelectionRun2 anymore, but here we are free to modify the defaults
    validationTracks = getJEGlobalTrackSelectionRun2();//the trackselection that we should keep in mind !
    /*validationTracks.SetTrackType(o2::aod::track::Run2Track); // Run 2 track asked by default
    validationTracks.SetMinNCrossedRowsTPC(70);
    validationTracks.SetMinNCrossedRowsOverFindableClustersTPC(0.8f);
    validationTracks.SetMaxChi2PerClusterTPC(4.f);
    validationTracks.SetRequireTPCRefit(true);
    validationTracks.SetRequireITSRefit(true);
    validationTracks.SetRequireHitsInITSLayers(1, {0, 1}); // one hit in any SPD layer
    validationTracks.SetMaxChi2PerClusterITS(36.f);
    validationTracks.SetPtRange(0.15f, 1e10f); // the 0.15 is the original hybrid cuts
    validationTracks.SetEtaRange(-0.9f, 0.9f);
    validationTracks.SetMaxDcaXY(2.4f);
    validationTracks.SetMaxDcaZ(3.2f);
    validationTracks.print();*/
    //validationTracks = getGlobalTrackSelection(); // using global tracks instead

    // histograms
    // 1)Jetvalidation on data
    mHistManager.add("collisionVtxZ", "control collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -15, 15}});
    // process jet qa
    mHistManager.add("jetPt", "inclusive jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetPhi", "inclusive jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("jetEta", "inclusive jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // process jet constituent qa - constituents as tracks
    mHistManager.add("jetConstTrackPt", "inclusive jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetConstTrackPhi", "inclusive jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetConstTrackEta", "inclusive jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // cross check the cuts from Run2Hybrid selection
    mHistManager.add("selectedTrackPt", "hybrid track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("selectedTrackPhi", "hybrid track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("selectedTrackEta", "hybrid track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

    // leading jets per collision
    mHistManager.add("leadJetPt", "track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetPhi", "track constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetEta", "track constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // leading constituents per jet in collision
    mHistManager.add("leadJetConstPt", "leading jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetConstPhi", "leading jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetConstEta", "leading jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // leading selected tracks per collision
    mHistManager.add("leadTrackPt", "leading selected track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadTrackPhi", "leading selected track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadTrackEta", "leading selected track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
  }

  void processESD(soa::Join<aod::Collisions, aod::EvSels>::iterator const& collision, soa::Join<aod::ChargedJets, aod::ChargedJetConstituents> const& jets, TracksJE const& tracks)
  {//check for tracks - add configuarable for both structs to switch between global and hybrid tracks 

  /* 
  // function that performs track selections on each track
template <typename T>
bool selectTrack(T const& track, std::string trackSelection)
{
  if (trackSelection == "globalTracks" && !track.isGlobalTrackWoPtEta()) {
    return false;
  } else if (trackSelection == "QualityTracks" && !track.isQualityTrack()) {
    return false;
  } else if (trackSelection == "hybridTracksJE" && !track.trackCutFlagFb5()) {//isQualityTrack
    return false;
  } else {
    return true;
  }
}
  */
    if (!collision.sel7() || abs(collision.posZ()) > 10) {
      return;
    }
    mHistManager.fill(HIST("collisionVtxZ"), collision.posZ());

    double leadingJetPt = -1;
    double leadingJetPhi = -1;
    double leadingJetEta = -1;
    // jet QA hists per jet in this collision
    for (const auto& j : jets) {
      mHistManager.fill(HIST("jetPt"), j.pt());
      mHistManager.fill(HIST("jetPhi"), j.phi());
      mHistManager.fill(HIST("jetEta"), j.eta());
      if (j.pt() > leadingJetPt) {
        leadingJetPt = j.pt();
        leadingJetPhi = j.phi();
        leadingJetEta = j.eta();
      }

      double leadingConstTrackPt = -1;
      double leadingConstTrackPhi = -1;
      double leadingConstTrackEta = -1;
      // access jet constituents as tracks
      for (auto& jct : j.tracks_as<TracksJE>()) {
        mHistManager.fill(HIST("jetConstTrackPt"), jct.pt());
        mHistManager.fill(HIST("jetConstTrackPhi"), jct.phi());
        mHistManager.fill(HIST("jetConstTrackEta"), jct.eta());
        if (jct.pt() > leadingConstTrackPt) {
          leadingConstTrackPt = jct.pt();
          leadingConstTrackPhi = jct.phi();
          leadingConstTrackEta = jct.eta();
        }
      } // end of jet constituent loop

      // fill leading jet constituent qa
      mHistManager.fill(HIST("leadJetConstPt"), leadingConstTrackPt);
      mHistManager.fill(HIST("leadJetConstPhi"), leadingConstTrackPhi);
      mHistManager.fill(HIST("leadJetConstEta"), leadingConstTrackEta);
    } // end of jet loop

    // fill leading jet qa
    mHistManager.fill(HIST("leadJetPt"), leadingJetPt);
    mHistManager.fill(HIST("leadJetPhi"), leadingJetPhi);
    mHistManager.fill(HIST("leadJetEta"), leadingJetEta);

    double leadingTrackPt = -1;
    double leadingTrackPhi = -1;
    double leadingTrackEta = -1;
    // qa histograms for selected tracks in collision
    for (const auto& t : tracks) {
      //if (!validationTracks.IsSelected(t)) { // we need a crosscheck for Fb5 - first we need Fb5 then we can add this here
      //  continue;
      //} // check if this is really a global track - maybe adding hists for rejected tracks ?
      mHistManager.fill(HIST("selectedTrackPt"), t.pt());
      mHistManager.fill(HIST("selectedTrackPhi"), t.phi());
      mHistManager.fill(HIST("selectedTrackEta"), t.eta());
      if (t.pt() > leadingTrackPt) {
        leadingTrackPt = t.pt();
        leadingTrackPhi = t.phi();
        leadingTrackEta = t.eta();
      }
    } // end of tracks loop

    // fill leading selected track qa
    mHistManager.fill(HIST("leadTrackPt"), leadingTrackPt);
    mHistManager.fill(HIST("leadTrackPhi"), leadingTrackPhi);
    mHistManager.fill(HIST("leadTrackEta"), leadingTrackEta);
  } // end process
  PROCESS_SWITCH(jetTrackCollisionQa, processESD, "validate jet-finder output on run2 ESD", true);

  // To Do: add run3 data valdation qa process
  // void processRun3()

  // dummy process to run jetfinder validation code on ESD, but MC validation for run3 on hyperloop
  void processDummy(aod::Collisions const& collision)
  {
  }
  PROCESS_SWITCH(jetTrackCollisionQa, processDummy, "Dummy process function turned on by default", false);
};

// MC validation for run2 and run3 on AO2D's
struct mcJetTrackCollisionQa {

  HistogramRegistry mHistManager{"JetCollisionQAHistograms"};
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};

  TrackSelection validationTracks;

  void init(InitContext const&)
  {
    validationTracks = getGlobalTrackSelection(); // using global tracks instead - will automatically select run2/3 cuts

    // histograms
    // 2)Jetvalidation on MC: generator = particle = mcTruth and reconstruction = detector (=tracks) 
    mHistManager.add("collisionVtxZ", "Control collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -15, 15}});
    mHistManager.add("genMCcollisionVtxZ", "MC control gen.collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -15, 15}});
    mHistManager.add("recMCcollisionVtxZ", "MC control rec.collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -15, 15}});
    // 2D for reco vs. truth level - not yet filled 
    /*
    mHistManager.add("collkMatchPosZ","MC reco vs truth; MC truth posZ (cm); MC reco posZ (cm)", {HistType::kTH2F, {{nBins, -15, 15}, {nBins, -15, 15}}});
    // 2D for 'relative resolution figure'
    mHistManager.add("collResolutionPt","Collision reso #Delta posZ = (MC reco - MC truth)/ MC truth; MC truth posZ (cm); #Delta posZ (cm)", {HistType::kTH2F, {{nBinsPt, -15, 15}, {nBins, -5, 5}}});
    */

    // process jet qa
    mHistManager.add("genMCjetPt", "MC inclusive gen jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("genMCjetPhi", "MC inclusive gen jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("genMCjetEta", "MC inclusive gen jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    mHistManager.add("recMCjetPt", "MC inclusive rec jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("recMCjetPhi", "MC inclusive rec jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("recMCjetEta", "MC inclusive rec jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // process mc matching from particle to det
    mHistManager.add("genRecMCjetPt", "MC rec to part jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("genRecMCjetPhi", "MC reco to part jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("genRecMCjetEta", "MC rec to part jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // process jet constituent qa - constituents as tracks
    mHistManager.add("genMCjetConstTrackPt", "MC inclusive part jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("genMCjetConstTrackPhi", "MC inclusive part jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("genMCjetConstTrackEta", "MC inclusive part jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    mHistManager.add("recMCjetConstTrackPt", "MC inclusive reco jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("recMCjetConstTrackPhi", "MC inclusive reco jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("recMCjetConstTrackEta", "MC inclusive reco jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // process mc matching from partice to detector - needs matching from nime / aimeric has something for it 
    mHistManager.add("genRecMCjetConstTrackPt", "MC rec to part jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("genRecMCjetConstTrackPhi", "MC rec to part inclusive part jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("genRecMCjetConstTrackEta", "MC rec to part part jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}}); 

    // cross check the cuts from Run2Hybrid selection
    mHistManager.add("genMCselectedTrackPt", "MC track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("genMCselectedTrackPhi", "MC track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("genMCselectedTrackEta", "MC track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    mHistManager.add("recMCselectedTrackPt", "reconstructed MC track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("recMCselectedTrackPhi", "reconstructed MC track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("recMCselectedTrackEta", "reconstructed MC track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // tracks from mc data not mc associated
    mHistManager.add("selectedTrackPt", "selected collission tracks Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("selectedTrackPhi", "selected collission tracks #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("selectedTrackEta", "selected collission tracks #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    // 2D for reco vs. truth level - we want this for jets too, but first we need proper matching there !
    /*
    mHistManager.add("trackMatchPt","MC reco vs truth; MC truth p_{T} (GeV/#it{c}); MC reco p_{T} (GeV/#it{c})", {HistType::kTH2F, {{nBinsPt, 0, 20}, {nBinsPt, 0, 20}}});
    mHistManager.add("trackMatchEta","MC reco vs truth; MC truth #eta; MC reco  #eta", {HistType::kTH2F, {{nBinsPt, -0.9, 0.9}, {nBins, -0.9, 0.9}}});
    mHistManager.add("trackMatchPhi","MC reco vs truth; MC truth #phi; MC reco #phi", {HistType::kTH2F, {{nBinsPt, 0, 6.32}, {nBins, 0, 6.32}}});

    // 2D for 'relative resolution figure'
    mHistManager.add("trackResolutionPt","Track reso #Delta p_{T} = (MC reco - MC truth)/ MC truth; MC truth p_{T} (GeV/#it{c}); #Delta p_{T}", {HistType::kTH2F, {{nBinsPt, 0, 20}, {nBins, -3, 3}}});
    mHistManager.add("trackResolutionEta","Track reso #Delta #eta = (MC reco - MC truth)/ MC truth; MC truth #Delta #eta", {HistType::kTH2F, {{nBinsPt, -0.9, 0.9}, {nBins, -1, 1}}});
    mHistManager.add("trackResolutionPhi","Track reso #Delta #phi = (MC reco - MC truth)/ MC truth; MC truth #phi (GeV/#it{c}); #Delta #phi", {HistType::kTH2F, {{nBinsPt, 0, 6.32}, {nBins, -5, 5}}});
    */
  }

  // fill qa histograms for selected tracks in collision - bool for if .has_mcCollision()
  template <class TMyTracks> 
  void fillMcTrackHistos(TMyTracks const& mct, bool mc)//could give collision as argument for additional association
  {  
    for (const auto& track : mct) {
      //if (!validationTracks.IsSelected(track)) {//with this in place all tracks are skipped !!! 
      //  return;
      //}
      if(mc == true){
        if(track.has_mcParticle()){
          auto mcParticle =  track.mcParticle();//t.mcParticle_as<aod::McParticles>();
          mHistManager.fill(HIST("genMCselectedTrackPt"), mcParticle.pt());
          mHistManager.fill(HIST("genMCselectedTrackPhi"), mcParticle.phi());
          mHistManager.fill(HIST("genMCselectedTrackEta"), mcParticle.eta());

        }
        // fill histos for all reconstructed particles from mc associated collision (includes mcParticles)
        mHistManager.fill(HIST("recMCselectedTrackPt"), track.pt());
        mHistManager.fill(HIST("recMCselectedTrackPhi"), track.phi());
        mHistManager.fill(HIST("recMCselectedTrackEta"), track.eta()); 
      }//end of if mc
      if(mc == false){
        mHistManager.fill(HIST("selectedTrackPt"), track.pt());
        mHistManager.fill(HIST("selectedTrackPhi"), track.phi());
        mHistManager.fill(HIST("selectedTrackEta"), track.eta()); 
      } 
    }// end of tracks loop
  }//end of mcTrack template

  template <typename detectorJet>
  void fillMcDetJets(detectorJet const& mcdJet)
  { 
    mHistManager.fill(HIST("recMCjetPt"), mcdJet.pt());
    mHistManager.fill(HIST("recMCjetPhi"), mcdJet.eta());
    mHistManager.fill(HIST("recMCjetEta"), mcdJet.phi());
  }// end of mcDetJet template

  template <typename particleJet>
  void fillMcPartJets(particleJet const& mcpJet)
  { 
    mHistManager.fill(HIST("genMCjetPt"), mcpJet.pt());
    mHistManager.fill(HIST("genMCjetPhi"), mcpJet.phi());
    mHistManager.fill(HIST("genMCjetEta"), mcpJet.eta());
  }// end of mcPartJet template

  template <typename detectorJetConstituent>
  void fillMcDetJetConstituents(detectorJetConstituent const& mcdJConst)
  {
    mHistManager.fill(HIST("recMCjetConstTrackPt"), mcdJConst.pt());
    mHistManager.fill(HIST("recMCjetConstTrackPhi"), mcdJConst.phi());
    mHistManager.fill(HIST("recMCjetConstTrackEta"), mcdJConst.eta());
  }// end of mcDetJetConstituent template

  void processMcRun3(soa::Join<aod::Collisions, aod::McCollisionLabels>::iterator const& collision, 
                      soa::Join<aod::FullMCParticleLevelJets, aod::FullMCParticleLevelJetConstituents> const& mcPartJets, 
                      soa::Join<aod::FullMCDetectorLevelJets, aod::FullMCDetectorLevelJetConstituents> const& mcDetJets, 
                      aod::McParticles const& mcParticles, aod::McCollisions const& mcCollisions,
                      MCTracksRun3JE const& tracks)
  {
    // we need jet Qa & jetConstituent QA
    // loop over particle jets - fillParticleLevel
    // loop over detector jets - fillDetectorLevel
    // loop over matched jets (requires mtching and additional tables in process function) 
    //see https://github.com/bhofman/AliPhysics/blob/721d787f96746f81a8b74238c6a6eb5cef8c1592/PWGJE/EMCALJetTasks/AliAnalysisTaskPWGJEQA.cxx#L1379
    // look at matching https://github.com/AliceO2Group/O2Physics/blob/723d78931b446e7b5f6e0673c0345fcef584e796/Tutorials/src/mcHistograms.cxx#L154

    // and then we do the track (and mc particle) Qa for the event on collision base
    // To Do: qa histograms for mc particles in collision 
    // https://github.com/AliceO2Group/O2Physics/blob/cd24862b6ad4d884fe9ec95dbc685d7c75a8a142/Tutorials/src/mcHistograms.cxx#L78
    if (abs(collision.posZ()) > 10) {//sel8 for run3: !collision.sel8() -> only on run3 data with EvSels in process function !
      return;
    }
    mHistManager.fill(HIST("collisionVtxZ"), collision.posZ());
    if(collision.has_mcCollision()){
      mHistManager.fill(HIST("genMCcollisionVtxZ"), collision.mcCollision().posZ());
      mHistManager.fill(HIST("recMCcollisionVtxZ"), collision.posZ());
      //differences at function of mcTruth
      /*
      mHistManager.fill(HIST("collkMatchPosZ"), collision.mcCollision().posZ(), collision.posZ());
      // 2D for 'relative resolution figure'
      mHistManager.fill(HIST("collResolutionPt"_, collision.mcCollision().posZ(), (collision.posZ() - collision.mcCollision().posZ())/collision.mcCollision().posZ());
      */
      fillMcTrackHistos(tracks, true);
    } // end of loop if mc collision
    fillMcTrackHistos(tracks, false);

    // loop over mc detector level jets for cross check
    for(const auto& detJet : mcDetJets){
      fillMcDetJets(detJet);
      for (auto& detConst : detJet.tracks_as<MCTracksRun3JE>()) {// had problems to pass the correct arguments into template
        fillMcDetJetConstituents(detConst);
      }// end of loop detector level constituents
    }// end of loop detector level jets

    for(const auto& genJet : mcPartJets ){// or we put here the det - or / gen as additional set of plots - maybe we still need the matching first ?
      fillMcPartJets(genJet);
        //make differences in TH2F's

        //ask for this - mcparticleleveljets should point to mcparticles -> how to access ?
        
        for (auto& mcParticle : genJet.tracks_as<aod::McParticles>()) {
        //for (auto& genConst : genJet.track()) {
          //if(genConst.has_mcParticle()){
            //auto mcParticle = genConst.mcParticle_as<aod::McParticles>();
            //auto mcParticle = genConst.mcParticle();
            mHistManager.fill(HIST("genMCjetConstTrackPt"), mcParticle.pt());
            mHistManager.fill(HIST("genMCjetConstTrackPhi"), mcParticle.phi());
            mHistManager.fill(HIST("genMCjetConstTrackEta"), mcParticle.eta());
            // here we could add reso and eff by mcParticle.pt()- t.pt() as function of mcParticle = truth
            // and Th2F's with both
          //}
        } // end of jet constituent loop 
         
        for(const auto& detJet : mcDetJets){
          mHistManager.fill(HIST("genRecMCjetPt"), detJet.pt());
          mHistManager.fill(HIST("genRecMCjetPhi"), detJet.phi());
          mHistManager.fill(HIST("genRecMCjetEta"), detJet.eta());
          for (auto& detConst : detJet.tracks_as<MCTracksRun3JE>()) {// this works, but empty for now..
            mHistManager.fill(HIST("genRecMCjetConstTrackPt"), detConst.pt());
            mHistManager.fill(HIST("genRecMCjetConstTrackPhi"), detConst.phi());
            mHistManager.fill(HIST("genRecMCjetConstTrackEta"), detConst.eta());
          } // end of loop detector level constituents
        } // end of loop detector level (in particle level) jet constituents 
      } // end of loop particle level jets 



  } // end process
  PROCESS_SWITCH(mcJetTrackCollisionQa, processMcRun3, "validate jet-finder output on run3 mc AOD's", false);

  void processMcRun2(soa::Join<aod::Collisions, aod::McCollisionLabels>::iterator const& collision, 
                      soa::Join<aod::FullMCParticleLevelJets, aod::FullMCParticleLevelJetConstituents> const& mcPartJets, 
                      soa::Join<aod::FullMCDetectorLevelJets, aod::FullMCDetectorLevelJetConstituents> const& mcDetJets, 
                      aod::McParticles const& mcParticles, aod::McCollisions const& mcCollisions,
                      MCTracksRun2JE const& tracks)
  {
    if (abs(collision.posZ()) > 10) {//sel7 for run2: !collision.sel7()
      return;
    }
    mHistManager.fill(HIST("collisionVtxZ"), collision.posZ());
    if(collision.has_mcCollision()){
      mHistManager.fill(HIST("genMCcollisionVtxZ"), collision.mcCollision().posZ());
      mHistManager.fill(HIST("recMCcollisionVtxZ"), collision.posZ());
      fillMcTrackHistos(tracks, true);
    } // end if has mc collision
    fillMcTrackHistos(tracks, false);

    for(const auto& detJet : mcDetJets){
      fillMcDetJets(detJet);
      for (auto& detConst : detJet.tracks_as<MCTracksRun2JE>()) {// had problems to pass the correct arguments into template
        fillMcDetJetConstituents(detConst);
      }// end of loop detector level constituents
    }// end of loop detector level jets

    for(const auto& genJet : mcPartJets){
      fillMcPartJets(genJet);
      // and here we have a problem for the constituents!
    }//end of loop particle level jets
  } // end process
  PROCESS_SWITCH(mcJetTrackCollisionQa, processMcRun2, "validate jet-finder output on converted run2 mc AOD's", false);

  // dummy process to run jetfinder validation code on AO2D's, but MC validation for run3 on hyperloop
  void processDummy(aod::Collisions const& collision)
  {
  }
  PROCESS_SWITCH(mcJetTrackCollisionQa, processDummy, "Dummy process function turned off by default", true);
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{ 
    adaptAnalysisTask<jetTrackCollisionQa>(cfgc, TaskName{"jet-validation-track-collision-qa"}),
    adaptAnalysisTask<mcJetTrackCollisionQa>(cfgc, TaskName{"mc-jet-validation-track-collision-qa"})};
}
