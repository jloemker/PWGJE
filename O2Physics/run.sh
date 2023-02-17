export OPTIONS="-b --configuration json://config.json"

o2-analysis-je-jet-finder ${OPTIONS} | o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} | o2-analysis-trackselection ${OPTIONS} | o2-analysis-trackextension ${OPTIONS} | o2-analysis-je-jet-track-qa ${OPTIONS} | o2-analysis-collision-converter 
