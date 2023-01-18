/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyTask_H
#define AliAnalysisTaskMyTask_H

#include "AliAnalysisTaskSE.h"

class AliAODEvent;
class TList;
class TH1F;

class AliAnalysisTaskMyTask : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyTask();
                                AliAnalysisTaskMyTask(const char *name);
        virtual                 ~AliAnalysisTaskMyTask();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

    private:
        AliAODEvent*            fAOD;           //! input event
        TList*                  fOutputList;    //! output list
        TH1F*                   fHistPt;        //! dummy histogram

        TH1F*                   controlTrackPt;
        TH1F*                   controlTrackPhi;
        TH1F*                   controlTrackEta;

        TH1F*                   controlCollisionVtxZ;

        TH1F*                   jetPt;
        TH1F*                   jetPhi;
        TH1F*                   jetEta;

        TH1F*                   jetConstPt;
        TH1F*                   jetConstPhi;
        TH1F*                   jetConstEta;

        TH1F*                   leadJetConstPt;
        TH1F*                   leadJetConstPhi;
        TH1F*                   leadJetConstEta;

        TH1F*                   jetTrackPt;
        TH1F*                   jetTrackPhi;
        TH1F*                   jetTrackEta;

        TH1F*                   leadJetTrackPt;
        TH1F*                   leadJetTrackPhi;
        TH1F*                   leadJetTrackEta;
        
        AliAnalysisTaskMyTask(const AliAnalysisTaskMyTask&); // not implemented
        AliAnalysisTaskMyTask& operator=(const AliAnalysisTaskMyTask&); // not implemented

        ClassDef(AliAnalysisTaskMyTask, 1);
};

#endif
