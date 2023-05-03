#The configuration file configRun3AOD.json is in principle correct, but I always run into the same error, no matter what i pipe and configure.
# [ERROR] SEVERE: Device timestamp-task (218700) had at least one message above severity 5: Got nullptr from CCDB for path CTP/Calib/OrbitReset and timestamp 1668063344876
#Guess it is a problem with the file which was for the LF tutorial (?) Or I am a bit dump right now.
#Could be that I figure it out in the next days - else it will be a task for someone else :P :)

#export OPTIONS="-b --configuration json://configRun3AOD.json"

export OPTIONS="-b --configuration json://config.json"

o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} | o2-analysis-trackselection ${OPTIONS} | o2-analysis-track-propagation ${OPTIONS} | o2-analysis-zdc-converter ${OPTIONS} | o2-analysis-je-jet-finder ${OPTIONS} 
#| o2-analysis-je-emcal-correction-task ${OPTIONS} | o2-analysis-je-jet-validation-qa ${OPTIONS} 
#| o2-analysis-onthefly-tracker ${OPTIONS}
#| o2-analysis-collision-converter ${OPTIONS}
#| o2-analysis-je-emcal-correction-task ${OPTIONS} 

