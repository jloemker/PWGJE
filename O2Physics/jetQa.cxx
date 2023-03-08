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
//	Johanna Lömker
//	\since Dec 2022
// The goal would be to set up the jet, track and event/collision QA's for the validation framework 
// Staring with the hybrid tracks 

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

using selectedTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::TrackSelection>;

struct jetTrackQa{

  HistogramRegistry mHistManager{"JetQAHistograms"};
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};

  void init(InitContext const&)
  {
    //control track qa
    mHistManager.add("controlTrackPt", "control track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("controlTrackPhi", "control track constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("controlTrackEta", "control track constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    mHistManager.add("controlCollisionVtxZ", "control collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -20, 20}});
    mHistManager.add("selectedControlCollisionVtxZ", "control collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -20, 20}});

    //process jet qa
    mHistManager.add("jetPt", "inclusive jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetPhi", "inclusive jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("jetEta", "inclusive jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

    mHistManager.add("jetConstPt", "inclusive jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetConstPhi", "inclusive jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetConstEta", "inclusive jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

    mHistManager.add("leadJetConstPt", "leading jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetConstPhi", "leading jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetConstEta", "leading jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

    mHistManager.add("jetTrackPt", "track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetTrackPhi", "track constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetTrackEta", "track constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

    mHistManager.add("leadJetTrackPt", "leading track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetTrackPhi", "leading track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetTrackEta", "leading track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

  }
  //loop over tracks per collision - maybe add an event qa here too 
  void processTrackQA(aod::Collision const& collision, aod::Tracks const& tracks)
  {//maybe already for collision and event qa ?
    mHistManager.fill(HIST("controlCollisionVtxZ"), collision.posZ());
    if(collision.posZ() > 10){return;}
    mHistManager.fill(HIST("selectedControlCollisionVtxZ"), collision.posZ());
    for (auto& track : tracks) {
      mHistManager.fill(HIST("controlTrackPt"), track.pt());
      mHistManager.fill(HIST("controlTrackPhi"), track.phi());
      mHistManager.fill(HIST("controlTrackEta"), track.eta());
    }
  }
  PROCESS_SWITCH(jetTrackQa, processTrackQA, "process selected track qa", true);
  
  double leadingJetConstPt = -1;
  double leadingJetConstPhi = -1;
  double leadingJetConstEta = -1;
  double leadingJetConstVtxZ = -1;

  double leadingTrackPt = -1;
  double leadingTrackPhi = -1;
  double leadingTrackEta = -1;
  double leadingTrackVtxZ = -1;

  //loops over jet table - jets have constituents - constituents have tracks -> now the selected tracks ?
  // for vertex cut we need at least EvSel .. ?
  void processJetQA(aod::Jet const& jet, aod::JetTrackConstituents const& constituents, aod::Tracks const& tracks)
  {//first fill all jet QA hists per jet
    mHistManager.fill(HIST("jetPt"), jet.pt());
    mHistManager.fill(HIST("jetPhi"), jet.phi());
    mHistManager.fill(HIST("jetEta"), jet.eta());
    //fill jet constituent qa
    for(const auto& c : constituents) {
      LOGF(debug, "jet %d: track id %d, track pt %g", jet.index(), c.trackId(), c.track().pt());
      mHistManager.fill(HIST("jetConstPt"), c.track().pt());
      mHistManager.fill(HIST("jetConstPhi"), c.track().phi());
      mHistManager.fill(HIST("jetConstEta"), c.track().eta());
      if(c.track().pt() > leadingJetConstPt){
        leadingJetConstPt = c.track().pt();
        leadingJetConstPhi = c.track().phi();
        leadingJetConstEta = c.track().eta();
      }
    }
    //fill leading constitutent track qa's
    mHistManager.fill(HIST("leadJetConstPt"), leadingJetConstPt);
    mHistManager.fill(HIST("leadJetConstPhi"), leadingJetConstPhi);
    mHistManager.fill(HIST("leadJetConstEta"), leadingJetConstEta);
    //fill track qa (should be identical to the jet constituent qa)
    for(const auto& t : tracks){
      mHistManager.fill(HIST("jetTrackPt"), t.pt());
      mHistManager.fill(HIST("jetTrackPhi"), t.phi());
      mHistManager.fill(HIST("jetTrackEta"), t.eta());
      if(t.pt() > leadingTrackPt){
        leadingTrackPt = t.pt();
        leadingTrackPhi = t.phi();
        leadingTrackEta = t.eta();
        //leadingTrackVtxZ = t.posZ();
      }
    }
    //fill leading track qa
    mHistManager.fill(HIST("leadJetTrackPt"), leadingTrackPt);
    mHistManager.fill(HIST("leadJetTrackPhi"), leadingTrackPhi);
    mHistManager.fill(HIST("leadJetTrackEta"), leadingTrackEta);
  }//end processJetQA
  PROCESS_SWITCH(jetTrackQa, processJetQA, "process jets from jet-finder output", true);

};

//add another process for MC studies
struct jetTrackCollisionQa{

  HistogramRegistry mHistManager{"JetCollisionQAHistograms"};
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};

  void init(InitContext const&)
  {
    mHistManager.add("collisionVtxZ", "control collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -15, 15}});
    //process jet qa
    mHistManager.add("jetPt", "inclusive jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetPhi", "inclusive jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("jetEta", "inclusive jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //process jet constituent qa - constituents as tracks
    mHistManager.add("jetConstTrackPt", "inclusive jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetConstTrackPhi", "inclusive jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetConstTrackEta", "inclusive jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //cross check the cuts from Run2Hybrid selection
    mHistManager.add("selectedTrackPt", "hybrid track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("selectedTrackPhi", "hybrid track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("selectedTrackEta", "hybrid track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});

    //leading jets per collision
    mHistManager.add("leadJetPt", "track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetPhi", "track constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetEta", "track constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //leading constituents per jet in collision
    mHistManager.add("leadJetConstPt", "leading jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetConstPhi", "leading jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetConstEta", "leading jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //leading selected tracks per collision
    mHistManager.add("leadTrackPt", "leading selected track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadTrackPhi", "leading selected track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadTrackEta", "leading selected track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
  }

  void processData(soa::Join<aod::Collisions, aod::EvSels>::iterator const& collision, aod::Jets const& jets, aod::JetTrackConstituents const& constituents, selectedTracks const& tracks)
  {
    if(collision.posZ() > 10){return;}
    mHistManager.fill(HIST("collisionVtxZ"), collision.posZ());

    double leadingJetPt = -1;
    double leadingJetPhi = -1;
    double leadingJetEta = -1;
    //jet QA hists per jet in this collision
    for(const auto& j: jets){
      mHistManager.fill(HIST("jetPt"), j.pt());
      mHistManager.fill(HIST("jetPhi"), j.phi());
      mHistManager.fill(HIST("jetEta"), j.eta());
      if(j.pt() > leadingJetPt){
        leadingJetPt = j.pt();
        leadingJetPhi = j.phi();
        leadingJetEta = j.eta();
      }

      double leadingConstTrackPt = -1;
      double leadingConstTrackPhi = -1;
      double leadingConstTrackEta = -1;
      //access jet constituents as tracks
      for (const auto& jct : constituents) {//or tracks_as<aod::Tracks>() ? this is not yet wroking because required changes are still missing to join JetTrackCOnstituents with Jets
        mHistManager.fill(HIST("jetConstTrackPt"), jct.track().pt());
        mHistManager.fill(HIST("jetConstTrackPhi"), jct.track().phi());
        mHistManager.fill(HIST("jetConstTrackEta"), jct.track().eta());
        if(jct.track().pt() > leadingConstTrackPt){
          leadingConstTrackPt = jct.track().pt();
          leadingConstTrackPhi = jct.track().phi();
          leadingConstTrackEta = jct.track().eta();
        }
      }//end of jet constituent loop

      //fill leading jet constituent qa
      mHistManager.fill(HIST("leadJetConstPt"), leadingConstTrackJetPt);
      mHistManager.fill(HIST("leadJetConstPhi"), leadingConstTrackJetPhi);
      mHistManager.fill(HIST("leadJetConstEta"), leadingConstTrackJetEta);
    }//end of jet loop

    //fill leading jet qa
    mHistManager.fill(HIST("leadJetPt"), leadingJetPt);
    mHistManager.fill(HIST("leadJetPhi"), leadingJetPhi);
    mHistManager.fill(HIST("leadJetEta"), leadingJetEta);

    double leadingTrackPt = -1;
    double leadingTrackPhi = -1;
    double leadingTrackEta = -1;
    //qa histograms for selected tracks in collision
    for(const auto& t : tracks){
      mHistManager.fill(HIST("selectedTrackPt"), t.pt());
      mHistManager.fill(HIST("selectedTrackPhi"), t.phi());
      mHistManager.fill(HIST("selectedTrackEta"), t.eta());
      if(t.pt() > leadingTrackPt){
        leadingTrackPt = t.pt();
        leadingTrackPhi = t.phi();
        leadingTrackEta = t.eta();
      }
    }//end of tracks loop

    //fill leading selected track qa
    mHistManager.fill(HIST("leadTrackPt"), leadingTrackPt);
    mHistManager.fill(HIST("leadTrackPhi"), leadingTrackPhi);
    mHistManager.fill(HIST("leadTrackEta"), leadingTrackEta);
  }//end process
  PROCESS_SWITCH(jetTrackCollisionQa, processData, "process jets from jet-finder output", true);
};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    //adaptAnalysisTask<jetTrackQa>(cfgc)
    adaptAnalysisTask<jetTrackCollisionQa>(cfgc)
    };
}