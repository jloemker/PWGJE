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

//using selectedTracks = soa::Join<aod::Tracks, aod::TracksExtra, aod::TrackSelection>;

struct jetqa{

  //o2::framework::HistogramRegistry mHistEventManager{"JetEventSelectionQAHistograms"};//from pre selection
  o2::framework::HistogramRegistry mHistJetManager{"JetQAHistograms"};//from signal selection
  //o2::framework::HistogramRegistry mHistJetTrackManager{"JetTrackSelectionQAHistograms"};//from pre selection - control Track
  Configurable<int> nBins{"nBins", 200, "N bins in histos"};
  Configurable<int> nBinsPt{"nBinsPt", 200, "N bins in pT histos"};
  Configurable<int> nBinsEta{"nBinsEta", 200, "N bins in Eta histos"};
  Configurable<int> nBinsPhi{"nBinsPhi", 200, "N bins in Phi histos"};
  //TrackSelections 3 for validation framework !
  //Configurable<int> selectedTracks{"select", 3, "Choice of track selection. 0 = no selection, 1 = globalTracks, 2 = globalTracksSDD, 3 = getJEGlobalTrackSelectionRun2"};

  void init(o2::framework::InitContext&)
  {
    using o2HistType = o2::framework::HistType;
    //control track qa
    mHistJetManager.add("controlTrackPt", "control track Pt ; p_{T} (GeV/#it{c})", o2HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistJetManager.add("controlTrackPhi", "control track constituent #phi ; #phi ", o2HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistJetManager.add("controlTrackEta", "control track constituent #eta ; #eta ", o2HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("controlTrackVtxZ", "control track jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    //process jet qa
    mHistJetManager.add("jetPt", "inclusive jetPt ; p_{T} (GeV/#it{c})", o2HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistJetManager.add("jetPhi", "inclusive jet #phi ; #phi ", o2HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistJetManager.add("jetEta", "inclusive jet #eta ; #eta ", o2HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetVtxZ", "inclusive jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistJetManager.add("jetConstPt", "inclusive jet constituent Pt ; p_{T} (GeV/#it{c})", o2HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistJetManager.add("jetConstPhi", "inclusive jet constituent #phi ; #phi ", o2HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistJetManager.add("jetConstEta", "inclusive jet constituent #eta ; #eta ", o2HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetConstVtxZ", "inclusive constituent jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistJetManager.add("leadJetConstPt", "leading jet constituent Pt ; p_{T} (GeV/#it{c})", o2HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistJetManager.add("leadJetConstPhi", "leading jet constituent #phi ; #phi ", o2HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistJetManager.add("leadJetConstEta", "leading jet constituent #eta ; #eta ", o2HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("leadJetConstVtxZ", "leading constituent jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistJetManager.add("jetTrackPt", "track Pt ; p_{T} (GeV/#it{c})", o2HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistJetManager.add("jetTrackPhi", "track constituent #phi ; #phi ", o2HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistJetManager.add("jetTrackEta", "track constituent #eta ; #eta ", o2HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("jetTrackVtxZ", "track jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

    mHistJetManager.add("LeadJetTrackPt", "leading track Pt ; p_{T} (GeV/#it{c})", o2HistType::kTH1F, {{nBinsPt, 0, 100}});
    mHistJetManager.add("leadJetTrackPhi", "leading track #phi ; #phi ", o2HistType::kTH1F, {{nBinsPhi, -3.2, 6.4}});
    mHistJetManager.add("leadJetTrackEta", "leading track #eta ; #eta ", o2HistType::kTH1F, {{nBinsEta, -0.9, 0.9}});
    //mHistJetManager.add("leadJetTrackVtxZ", "leading track jetVtxZ ; z [cm]", o2HistType::kTH1F, {{nBins, -20, 20}});

  }
  // processEventSelectionQA()

  /* group tracks according to collision - maybe add an event qa here too 
  void processTrackQA(aod::Collision const& collision, selectedTracks const& tracks)
  {//maybe already for collision and event qa ?
    for (auto& track : tracks) {

      // select tracks according to configurable selectedTracks
      if (selectedTracks == 1 && !track.isGlobalTrack()) {
        continue;
      } else if (selectedTracks == 2 && !track.isGlobalTrackSDD()) {
        continue;
      } else if(selectedTracks == 3 && !track.getJEGlobalTrackSelectionRun2()){
        continue;
      }
      mHistJetManager.fill(HIST("controlTrackPt"), track.pt());
      mHistJetManager.fill(HIST("controlTrackPhi"), track.phi());
      mHistJetManager.fill(HIST("controlTrackEta"), track.eta());
      //mHistJetManager.fill(HIST("controlTrackVtxZ"), track.posZ());
      // fill pt histogram
      // pt->Fill(track.pt());
    }
  }
  PROCESS_SWITCH(O2PWGJEQA, processTrackQA, "process selected track qa", true);
  */

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
  void process(aod::Jet const& jet, aod::JetTrackConstituents const& constituents, aod::Tracks const& tracks)
  {//first fill all jet QA hists per jet
    mHistJetManager.fill(HIST("jetPt"), jet.pt());
    mHistJetManager.fill(HIST("jetPhi"), jet.phi());
    mHistJetManager.fill(HIST("jetEta"), jet.eta());
    //mHistJetManager.fill(HIST("jetVtxZ"), jet.posZ());
    //fill jet constituent qa
    for(const auto& c : constituents) {
      LOGF(debug, "jet %d: track id %d, track pt %g", jet.index(), c.trackId(), c.track().pt());
      mHistJetManager.fill(HIST("jetConstPt"), c.track().pt());
      mHistJetManager.fill(HIST("jetConstPhi"), c.track().phi());
      mHistJetManager.fill(HIST("jetConstEta"), c.track().eta());
      //mHistJetManager.fill(HIST("jetConstVtxZ"), c.track().posZ());

      if(c.track().pt() > leadingJetConstPt){
        leadingJetConstPt = c.track().pt();
        leadingJetConstPhi = c.track().phi();
        leadingJetConstEta = c.track().eta();
        //leadingJetConstVtxZ = c.track().posZ();
      }
    }
    //fill leading constitutent track qa's
    mHistJetManager.fill(HIST("leadJetConstPt"), leadingJetConstPt);
    mHistJetManager.fill(HIST("leadJetConstPhi"), leadingJetConstPhi);
    mHistJetManager.fill(HIST("leadJetConstEta"), leadingJetConstEta);
    //mHistJetManager.fill(HIST("leadJetConstVtxZ"), leadingJetConstVtxZ);
    //fill track qa (should be identical to the jet constituent qa)
    for(const auto& t : tracks){
      mHistJetManager.fill(HIST("jetTrackPt"), t.pt());
      mHistJetManager.fill(HIST("jetTrackPhi"), t.phi());
      mHistJetManager.fill(HIST("jetTrackEta"), t.eta());
      //mHistJetManager.fill(HIST("jetTrackVtxZ"), t.posZ());

      if(t.pt() > leadingTrackPt){
        leadingTrackPt = t.pt();
        leadingTrackPhi = t.phi();
        leadingTrackEta = t.eta();
        //leadingTrackVtxZ = t.posZ();
      }
    }
    //fill leading track qa
    mHistJetManager.fill(HIST("leadJetTrackPt"), leadingTrackPt);
    mHistJetManager.fill(HIST("leadJetTrackPhi"), leadingTrackPhi);
    mHistJetManager.fill(HIST("leadJetTrackEta"), leadingTrackEta);
    //mHistJetManager.fill(HIST("leadJetTrackVtxZ"), leadingJetTrackVtxZ);
  }//end processJetQA
  //PROCESS_SWITCH(O2PWGJEQA, processJetQA, "process jets from jet-finder output", true);

};


WorkflowSpec defineDataProcessing(ConfigContext const& cfgc)
{
  return WorkflowSpec{
    adaptAnalysisTask<jetqa>(cfgc)};
}