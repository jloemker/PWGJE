#!/bin/sh

#copy file to jet tasks
cp jet-validation-qa.cxx /home/johannalomker/alice/O2Physics/PWGJE/Tasks/jet-validation-qa.cxx
cp CMakeLists.txt /home/johannalomker/alice/O2Physics/PWGJE/Tasks/CMakeLists.txt
cp TrackSelectionDefaults.cxx /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.cxx
cp TrackSelectionDefaults.h /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.h
#cp trackselection.cxx /home/johannalomker/alice/O2Physics/Common/TableProducer/trackselection.cxx 
cp jetfinder.cxx /home/johannalomker/alice/O2Physics/PWGJE/TableProducer

#enter installation directory alienv enter O2Physics/latest-master-o2 
cd /home/johannalomker/alice/sw/BUILD/O2Physics-latest-hfTest/O2Physics
pwd
#rebuild whole tutorial section via ninja install
eval $(alienv printenv O2Physics/latest-hfTest-o2 ninja/latest)
ninja install Tutorials/all 

