export OPTIONS="-b --configuration json://configMC3Jet.json"

o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} | o2-analysis-trackselection ${OPTIONS} | o2-analysis-track-propagation ${OPTIONS} | o2-analysis-collision-converter ${OPTIONS} | o2-analysis-je-jet-finder ${OPTIONS} | o2-analysis-je-jet-validation-qa ${OPTIONS} | o2-analysis-zdc-converter ${OPTIONS} | o2-analysis-je-emcal-correction-task ${OPTIONS} 

