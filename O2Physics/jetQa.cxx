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
//	\since 2022
// The goal would be to set up the jet, track and event QA's for the validation framework 
// Staring with the hybrid tracks 

//  void    in progress:        FillJetHistograms()                                           ;
//  --      not yet started
//  void                        FillTrackHistograms()                                         ;
//  void                        FillCellHistograms()                                          ;
//  void                        FillClusterHistograms()                                       ;
//  void                        FillEventQAHistograms()                                       ;

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

struct jetqa{

  HistogramRegistry mHistManager{"JetQAHistograms"};//from signal selection
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};
  //TrackSelections 3 for validation framework; to check if the Hybrid selection in tracks is sufficient or if we need an extra hybrid selection here! 
  //- I call hybrid tracks in trackselection.cxx in case 0 and hybrid = true; defines in TrackselectionDefaults
  void init(InitContext const&)
  {
    //using o2HistType = o2::framework::HistType;{HistType::kTH1F, {{nBins, -15., 15.}}}
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
    //mHistJetManager.add("jetVtxZ", "inclusive jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistManager.add("jetConstPt", "inclusive jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetConstPhi", "inclusive jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetConstEta", "inclusive jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetConstVtxZ", "inclusive constituent jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistManager.add("leadJetConstPt", "leading jet constituent Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetConstPhi", "leading jet constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetConstEta", "leading jet constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("leadJetConstVtxZ", "leading constituent jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistManager.add("jetTrackPt", "track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetTrackPhi", "track constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetTrackEta", "track constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetTrackVtxZ", "track jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistManager.add("leadJetTrackPt", "leading track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("leadJetTrackPhi", "leading track #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("leadJetTrackEta", "leading track #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("leadJetTrackVtxZ", "leading track jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

  }
  // processEventSelectionQA()

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
  PROCESS_SWITCH(jetqa, processTrackQA, "process selected track qa", true);
  

  //double leadJetPt = -1; for leading jet I would need to loop over the collisions - for now only leading track/constituent per jet
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
    //mHistJetManager.fill(HIST("jetVtxZ"), jet.collisionId().posZ());
    //fill jet constituent qa
    for(const auto& c : constituents) {
      LOGF(debug, "jet %d: track id %d, track pt %g", jet.index(), c.trackId(), c.track().pt());
      mHistManager.fill(HIST("jetConstPt"), c.track().pt());
      mHistManager.fill(HIST("jetConstPhi"), c.track().phi());
      mHistManager.fill(HIST("jetConstEta"), c.track().eta());
      //mHistJetManager.fill(HIST("jetConstVtxZ"), c.track().posZ());

      if(c.track().pt() > leadingJetConstPt){
        leadingJetConstPt = c.track().pt();
        leadingJetConstPhi = c.track().phi();
        leadingJetConstEta = c.track().eta();
        //leadingJetConstVtxZ = c.track().posZ();
      }
    }
    //fill leading constitutent track qa's
    mHistManager.fill(HIST("leadJetConstPt"), leadingJetConstPt);
    mHistManager.fill(HIST("leadJetConstPhi"), leadingJetConstPhi);
    mHistManager.fill(HIST("leadJetConstEta"), leadingJetConstEta);
    //mHistJetManager.fill(HIST("leadJetConstVtxZ"), leadingJetConstVtxZ);
    //fill track qa (should be identical to the jet constituent qa)
    for(const auto& t : tracks){
      mHistManager.fill(HIST("jetTrackPt"), t.pt());
      mHistManager.fill(HIST("jetTrackPhi"), t.phi());
      mHistManager.fill(HIST("jetTrackEta"), t.eta());
      //mHistJetManager.fill(HIST("jetTrackVtxZ"), t.posZ());

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
    //mHistJetManager.fill(HIST("leadJetTrackVtxZ"), leadingJetTrackVtxZ);
  }//end processJetQA
  PROCESS_SWITCH(jetqa, processJetQA, "process jets from jet-finder output", true);

};

//add second struct that handles tracks and jets collision wise with event selection and jet flter:
struct jetCollisionQa{

  HistogramRegistry mHistManager{"JetCollisionQAHistograms"};//from signal selection
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};

  void init(InitContext const&)
  {
    //using o2HistType = o2::framework::HistType;{HistType::kTH1F, {{nBins, -15., 15.}}}
    mHistManager.add("collisionVtxZ", "control collsion VtxZ ; z [cm]", HistType::kTH1F, {{nBins, -15, 15}});

    //process jet qa
    mHistManager.add("jetPt", "inclusive jetPt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetPhi", "inclusive jet #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistManager.add("jetEta", "inclusive jet #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetVtxZ", "inclusive jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistManager.add("jetTrackPt", "track Pt ; p_{T} (GeV/#it{c})", HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistManager.add("jetTrackPhi", "track constituent #phi ; #phi ", HistType::kTH1F, {{nBinsPhi, 0, 6.4}});
    mHistManager.add("jetTrackEta", "track constituent #eta ; #eta ", HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetTrackVtxZ", "track jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

  }

  void process(soa::Join<aod::Collisions, aod::EvSels>::iterator const& collision, aod::Jets const& jets, aod::Tracks const& tracks)
  {
    if(collision.posZ() > 10){return;}//maybe we can set this in the EvSel per .json
    mHistManager.fill(HIST("collisionVtxZ"), collision.posZ());
    //first fill all jet QA hists per jet
    for(const auto& j: jets){
      mHistManager.fill(HIST("jetPt"), j.pt());
      mHistManager.fill(HIST("jetPhi"), j.phi());
      mHistManager.fill(HIST("jetEta"), j.eta());
      //mHistJetManager.fill(HIST("jetVtxZ"), jet.collisionId().posZ());
    }

    for(const auto& t : tracks){
      mHistManager.fill(HIST("jetTrackPt"), t.pt());
      mHistManager.fill(HIST("jetTrackPhi"), t.phi());
      mHistManager.fill(HIST("jetTrackEta"), t.eta());
      //mHistJetManager.fill(HIST("jetTrackVtxZ"), t.posZ());
    }

  }//end process

};

WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<jetqa>(cfgc),
    adaptAnalysisTask<jetCollisionQa>(cfgc)
    };
}