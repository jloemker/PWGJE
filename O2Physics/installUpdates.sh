#!/bin/sh

#copy file in tutorial section
cp jetQa.cxx /home/johannalomker/alice/O2Physics/PWGJE/Tasks/jetQa.cxx
cp CMakeLists.txt /home/johannalomker/alice/O2Physics/PWGJE/Tasks/CMakeLists.txt
cp TrackSelectionDefaults.cxx /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.cxx
cp TrackSelectionDefaults.h /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.h
cp trackselection.cxx /home/johannalomker/alice/O2Physics/Common/TableProducer/trackselection.cxx 

#enter installation directory 
cd /home/johannalomker/alice/sw/BUILD/O2Physics-latest/O2Physics/
pwd
#rebuild whole tutorial section via ninja install
eval $(alienv printenv O2Physics/latest ninja/latest)
ninja install Tutorials/all 

