/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 * empty task which can serve as a starting point for building an analysis
 * as an example, one histogram is filled
 *
 * I try to get the same qa plots as in O2Physics - but I have no idea on how to handle the selections here :D
 *
 */

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAODEvent.h"
#include "AliAODInputHandler.h"
#include "AliAnalysisTaskMyTask.h"

#include <AliEmcalJetTask.h>

class AliAnalysisTaskMyTask;    // your analysis class

using namespace std;            // std namespace: so you can do things like 'cout'

ClassImp(AliAnalysisTaskMyTask) // classimp: necessary for root

AliAnalysisTaskMyTask::AliAnalysisTaskMyTask() : AliAnalysisTaskSE(), 
    fAOD(0), fOutputList(0), fHistPt(0)
{
    // default constructor, don't allocate memory here!
    // this is used by root for IO purposes, it needs to remain empty
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::AliAnalysisTaskMyTask(const char* name) : AliAnalysisTaskSE(name),
    fAOD(0), fOutputList(0), fHistPt(0)
{
    // constructor
    DefineInput(0, TChain::Class());    // define the input of the analysis: in this case we take a 'chain' of events
                                        // this chain is created by the analysis manager, so no need to worry about it, 
                                        // it does its work automatically
    DefineOutput(1, TList::Class());    // define the ouptut of the analysis: in this case it's a list of histograms 
                                        // you can add more output objects by calling DefineOutput(2, classname::Class())
                                        // if you add more output objects, make sure to call PostData for all of them, and to
                                        // make changes to your AddTask macro!
}
//_____________________________________________________________________________
AliAnalysisTaskMyTask::~AliAnalysisTaskMyTask()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;     // at the end of your task, it is deleted from memory by calling this function
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::UserCreateOutputObjects()
{
    // create output objects
    //
    // this function is called ONCE at the start of your analysis (RUNTIME)
    // here you ceate the histograms that you want to use 
    //
    // the histograms are in this case added to a tlist, this list is in the end saved
    // to an output file
    //
    fOutputList = new TList();          // this is a list which will contain all of your histograms
                                        // at the end of the analysis, the contents of this list are written
                                        // to the output file
    fOutputList->SetOwner(kTRUE);       // memory stuff: the list is owner of all objects it contains and will delete them
                                        // if requested (dont worry about this now)
    //nBins** = 200
    int nBins = 200;
    int nBinsPt = 200;
    int nBinsPhi = 200;
    int nBinsEta = 200;

    // example of a histogram
    fHistPt = new TH1F("fHistPt", "fHistPt", nBins, 0, 10);       // create your histogra
    //control track qa
    controlTrackPt = new TH1F("controlTrackPt", "control track Pt ; p_{T} (GeV/#it{c})", nBinsPt, 0, 100); 
    controlTrackPhi = new TH1F("controlTrackPhi", "control track constituent #phi ; #phi ", nBinsPhi, 0, 6.4); 
    controlTrackEta = new TH1F("controlTrackEta", "control track constituent #eta ; #eta ", nBinsEta, -0.9, 0.9); 
    
    controlCollisionVtxZ = new TH1F("controlCollisionVtxZ", "control collsion VtxZ ; z [cm]", nBins, -20, 20); 

    //process jet qa
    jetPt = new TH1F("jetPt", "inclusive jetPt ; p_{T} (GeV/#it{c})", nBinsPt, 0, 100);  
    jetPhi = new TH1F("jetPhi", "inclusive jet #phi ; #phi ", nBinsPhi, 0, 6.4);  
    jetEta = new TH1F("jetEta", "inclusive jet #eta ; #eta ", nBinsEta, -0.9, 0.9);  

    jetConstPt = new TH1F("jetConstPt", "inclusive jet constituent Pt ; p_{T} (GeV/#it{c})", nBinsPt, 0, 100);  
    jetConstPhi = new TH1F("jetConstPhi", "inclusive jet constituent #phi ; #phi ", nBinsPhi, 0, 6.4);  
    jetConstEta = new TH1F("jetConstEta", "inclusive jet constituent #eta ; #eta", nBinsEta, -0.9, 0.9);  

    leadJetConstPt = new TH1F("leadJetConstPt", "leading jet constituent Pt ; p_{T} (GeV/#it{c})", nBinsPt, 0, 100);
    leadJetConstPhi = new TH1F("leadJetConstPhi", "leading jet constituent #phi ; #phi", nBinsPhi, 0, 6.4);
    leadJetConstEta = new TH1F("leadJetConstEta", "leading jet constituent #eta ; #eta", nBinsEta, -0.9, 0.9);

    jetTrackPt = new TH1F("jetTrackPt", "track Pt ; p_{T} (GeV/#it{c})", nBinsPt, 0, 100);
    jetTrackPhi = new TH1F("jetTrackPhi", "track constituent #phi ; #phi", nBinsPhi, 0, 6.4);
    jetTrackEta = new TH1F("jetTrackEta", "track constituent #eta ; #eta", nBinsEta, -0.9, 0.9);

    leadJetTrackPt = new TH1F("leadJetTrackPt", "leading track Pt ; p_{T} (GeV/#it{c})", nBinsPt, 0, 100);
    leadJetTrackPhi = new TH1F("leadJetTrackPhi", "leading track #phi ; #phi", nBinsPhi, 0, 6.4);
    leadJetTrackEta = new TH1F("leadJetTrackEta", "leading track #eta ; #eta", nBinsEta, -0.9, 0.9);

    fOutputList->Add(fHistPt);          // don't forget to add it to the list! the list will be written to file, so if you want
                                        // your histogram in the output file, add it to the list!
    fOutputList->Add(controlTrackPt);
    fOutputList->Add(controlTrackPhi);
    fOutputList->Add(controlTrackEta);

    fOutputList->Add(controlCollisionVtxZ);

    fOutputList->Add(jetPt);
    fOutputList->Add(jetPhi);
    fOutputList->Add(jetEta);

    fOutputList->Add(jetConstPt);
    fOutputList->Add(jetConstPhi);
    fOutputList->Add(jetConstEta);

    fOutputList->Add(leadJetConstPt);
    fOutputList->Add(leadJetConstPhi);
    fOutputList->Add(leadJetConstEta);

    fOutputList->Add(jetTrackPt);
    fOutputList->Add(jetTrackPhi);
    fOutputList->Add(jetTrackEta);

    fOutputList->Add(leadJetTrackPt);
    fOutputList->Add(leadJetTrackPhi);
    fOutputList->Add(leadJetTrackEta);

    PostData(1, fOutputList);           // postdata will notify the analysis manager of changes / updates to the 
                                        // fOutputList object. the manager will in the end take care of writing your output to file
                                        // so it needs to know what's in the output
}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Make(AliAODEvent* fAOD)
{   // http://alidoc.cern.ch/AliRoot/v5-09-24/class_ali_a_o_d_event.html (?)
    // user exec - this is for the eveqnt qa part from the O2 version - we need to add the jet task and then loop over the jets to get the other plots ?

    controlCollisionVtxZ->Fill(fAOD->GetVertex()->GetZ());

    Int_t iTracks(fAOD->GetNumberOfTracks());           // see how many tracks there are in the event
    for(Int_t i(0); i < iTracks; i++) {                 // loop ove rall these tracks
        AliAODTrack* track = static_cast<AliAODTrack*>(fAOD->GetTrack(i));         // get a track (type AliAODTrack) from the event
        if(!track) continue;                            // if we failed, skip this track
        if(!track->TestFilterBit(128)) continue; // ! have to find the correct selection !

        fHistPt->Fill(track->Pt());                     // plot the pt value of the track in a histogram
        
        controlTrackPt->Fill(track->Pt());
        controlTrackPhi->Fill(track->Phi());
        controlTrackEta->Fill(track->Eta());
    }                                                   // continue until all the tracks are processed
    PostData(1, fOutputList);                           // stream the results the analysis of this event to
                                                        // the output manager which will take care of writing
                                                        // it to a file
}

void AliAnalysisTaskMyTask::UserExec(Option_t *) 
{
    // this function is called once for each event
    // the manager will take care of reading the events from file, and with the static function InputEvent() you 
    // have access to the current event. 
    // once you return from the UserExec function, the manager will retrieve the next event from the chain
    fAOD = dynamic_cast<AliAODEvent*>(InputEvent());    // get an event (called fAOD) from the input file
                                                        // there's another event format (ESD) which works in a similar wya
                                                        // but is more cpu/memory unfriendly. for now, we'll stick with aod's
    if(!fAOD) return;                                   // if the pointer to the event is empty (getting it failed) skip this event

	Make(fAOD);//here I have the collision vertex (I think !) and some track qa - needs proper selection

    //I think i should run the jetfinder here - but the documentation made a comment about the user exec and run that is different for the jet framework..
    //AliEmcalJetTask* jetFinderCh = AliEmcalJetTask("usedefault", "",  AliJetContainer::antikt_algorithm, 0.4, AliJetContainer::kChargedJet)
    Int_t iJets(fAOD->GetNJets());
    for(Int_t i(0); i < iJets; i++){
        AliAODJet* jet = static_cast<AliAODJet*>(fAOD->GetJet(i));
        jetPt->Fill(jet->Pt());
        jetPhi->Fill(jet->Phi());
        jetEta->Fill(jet->Eta());

    }

}
//_____________________________________________________________________________
void AliAnalysisTaskMyTask::Terminate(Option_t *)
{
    // terminate
    // called at the END of the analysis (when all events are processed)
}
//_____________________________________________________________________________
