#export OPTIONS="-b --aod-file /home/johannalomker/alice/serviceTask/PWGJE/data/AO2D.root"
export OPTIONS="-b --configuration json://configRun2ESD.json"

o2-analysis-je-jet-validation-qa ${OPTIONS} | o2-analysis-zdc-converter ${OPTIONS} | o2-analysis-timestamp ${OPTIONS} | o2-analysis-event-selection ${OPTIONS} | o2-analysis-trackselection ${OPTIONS} | o2-analysis-trackextension ${OPTIONS} | o2-analysis-je-jet-finder ${OPTIONS}  
