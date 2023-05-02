#!/bin/sh

#2)Files for jet PR:
cp jetvalidationqa.cxx /home/johannalomker/alice/O2Physics/PWGJE/Tasks/jetvalidationqa.cxx
#cp jetfinder.cxx /home/johannalomker/alice/O2Physics/PWGJE/TableProducer
#cp jetfinder.h /home/johannalomker/alice/O2Physics/PWGJE/TableProducer
#enter installation directory alienv enter O2Physics/latest-master-o2 
cd /home/johannalomker/alice/sw/BUILD/O2Physics-latest-jetVal/O2Physics
#/home/johannalomker/alice/sw/BUILD/O2Physics-latest-hfTest/O2Physics
pwd
#rebuild manipulated O2Physics section via ninja install
eval $(alienv printenv O2Physics/latest-jetVal-o2 ninja/latest)
ninja install Tutorials/all


