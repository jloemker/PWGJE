#!/bin/bash

if [ -z "$1" ]; then
	DATATYPE="AOD"
else
	DATATYPE="${1}"
fi

if [ -z "$2" ]; then
	PERIOD="LHC18b"
else
	PERIOD="${2}"
fi

if [ -z "$3" ]; then
	FILELIST="files.txt"
else
	FILELIST="${3}"
fi

if [ -z "$4" ]; then
	NEVENTS="5000"
else
	NEVENTS="${4}"
fi
cp runJetQA.C $ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/runJetQA.C
#this for running locally 
#root -b -x <<*EOF
#gROOT->LoadMacro("AliEmcalAodTrackFilterTask_cps.C++g");
#.x /home/christos/Desktop/PhD/EnergyFlowFluctuations/Analysis_task/Small_Rstep005/Embeded_PbPb/runEMCalJetEnergyFlow_embed_2Filter.C("${DATATYPE}","${PERIOD}","${FILELIST}",${NEVENTS})
#*EOF
#this if I add a cp to the installation
root -b -q -x $ALICE_PHYSICS/PWGJE/EMCALJetTasks/macros/runJetQA.C\(\""${DATATYPE}"\",\""${PERIOD}"\",\""${FILELIST}"\","${NEVENTS}"\)
#root -b -x <<*EOF
#gROOT->LoadMacro("runJetQA.C++g");
#.x runJetQA.C("${DATATYPE}","${PERIOD}","${FILELIST}",${NEVENTS})
#*EOF
