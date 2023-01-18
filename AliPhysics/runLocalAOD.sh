#!/bin/bash

#the loading requires patience :')
#gROOT->LoadMacro("AliAnalysisTaskMyTask.cxx++g");
#gROOT->LoadMacro("AddMyTask.C");

#this for running locally with a messed up (linked to O2 ?) AliPhysics installation
root -b -x <<*EOF
.L AliAnalysisTaskMyTask.cxx
.L AddMyTask.C

.L runAnalysis.C
.x runAnalysis.C
*EOF
