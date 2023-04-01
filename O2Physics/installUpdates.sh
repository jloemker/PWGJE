#!/bin/sh

#1) Files for Track O2Physics PR (extend defaults with loose jet validation hybrid cuts; tighten in jetfinder and jet-validation-qa-task):
#cp trackselection.cxx /home/johannalomker/alice/O2Physics/Common/TableProducer/trackselection.cxx 
#cp TrackSelectionDefaults.cxx /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.cxx
#cp TrackSelectionDefaults.h /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.h
#cp TrackSelectionTables.h /home/johannalomker/alice/O2Physics/Common/DataModel/TrackSelectionTables.h 

#2)Files for jet PR:
cp jet-validation-qa.cxx /home/johannalomker/alice/O2Physics/PWGJE/Tasks/jet-validation-qa.cxx
cp CMakeLists.txt /home/johannalomker/alice/O2Physics/PWGJE/Tasks/CMakeLists.txt
#cp jetfinder.cxx /home/johannalomker/alice/O2Physics/PWGJE/TableProducer

#enter installation directory alienv enter O2Physics/latest-master-o2 
cd /home/johannalomker/alice/sw/BUILD/O2Physics-latest-hfTest/O2Physics
pwd
#rebuild whole tutorial section via ninja install
eval $(alienv printenv O2Physics/latest-hfTest-o2 ninja/latest)
ninja install Tutorials/all 

