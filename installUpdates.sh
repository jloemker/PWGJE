#!/bin/sh

#copy file in tutorial section
cp JetQA.cxx /home/johannalomker/alice/O2Physics/PWGJE/Tasks/JetQA.cxx
cp CMakeLists.txt /home/johannalomker/alice/O2Physics/PWGJE/Tasks/CMakeLists.txt
cp TrackSelectionDefaults.cxx /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.cxx
cp TrackSelectionDefaults.h /home/johannalomker/alice/O2Physics/Common/Core/TrackSelectionDefaults.h
cp trackselection.cxx /home/johannalomker/alice/O2Physics/Common/TableProducer/trackselection.cxx 
cp eventSelection.cxx /home/johannalomker/alice/O2Physics/Common/TableProducer/eventSelection.cxx 
#cp TrackSelection.cxx /home/johannalomker/alice/O2Physics/Common/Core/TrackSelection.cxx
#cp TrackSelection.h /home/johannalomker/alice/O2Physics/Common/Core/TrackSelection.h

#enter installation directory 
cd /home/johannalomker/alice/sw/BUILD/O2Physics-latest/O2Physics/
pwd
#rebuild whole tutorial section via ninja install
eval $(alienv printenv O2Physics/latest ninja/latest)
ninja install Tutorials/all 

