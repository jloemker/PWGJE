void jetFinderQaPlots(){

        TFile *GlobalResult = new TFile("/home/johannalomker/alice/serviceTask/PWQJE/GlobalResults.root");
        TFile *HybridResult = new TFile("/home/johannalomker/alice/serviceTask/PWQJE/HybridResults.root");
        
        //Output the Histos from jet-finder
        TH2F *globNJet2D = (TH2F*) GlobalResult->Get("jet-finder-data/h_jet_n");
        TH2F *globEtaJet2D = (TH2F*) GlobalResult->Get("jet-finder-data/h_jet_eta");
        TH2F *globPhiJet2D = (TH2F*) GlobalResult->Get("jet-finder-data/h_jet_phi");
        TH2F *globPtJet2D = (TH2F*) GlobalResult->Get("jet-finder-data/h_jet_pt");

        TH2F *hybNJet2D = (TH2F*) HybridResult->Get("jet-finder-data/h_jet_n");
        TH2F *hybEtaJet2D = (TH2F*) HybridResult->Get("jet-finder-data/h_jet_eta");
        TH2F *hybPhiJet2D = (TH2F*) HybridResult->Get("jet-finder-data/h_jet_phi");
        TH2F *hybPtJet2D = (TH2F*) HybridResult->Get("jet-finder-data/h_jet_pt");

        TCanvas *n2 = new TCanvas("n2D", "number of entries", 800, 400);
        n2->Divide(2,1);
        n2->cd(1);
        hybNJet2D->SetTitle("Hybrid Tracks");
        hybNJet2D->Draw("E");
        n2->cd(2);
        globNJet2D->SetTitle("Gloabl Tracks");
        globNJet2D->Draw("E");
        n2->cd();
        n2->SaveAs("plots/2DNJets.pdf");

        TCanvas *n = new TCanvas("n", "number of entries", 800, 400);
        n->Divide(2,1);
        n->cd(1);
        hybNJet2D->ProjectionX()->SetTitle("Hybrid Tracks");
        hybNJet2D->ProjectionX()->DrawCopy("E");
        n->cd(2);
        globNJet2D->ProjectionX()->SetTitle("Gloabl Tracks");
        globNJet2D->ProjectionX()->DrawCopy("E");
        n->cd();
        n->SaveAs("plots/NJets.pdf");

        TCanvas *e = new TCanvas("eta", "eta", 800, 400);
        e->Divide(2,1);
        e->cd(1);
        hybEtaJet2D->ProjectionX()->SetTitle("Hybrid Tracks");
        hybEtaJet2D->ProjectionX()->Rebin(4);
        hybEtaJet2D->ProjectionX()->DrawCopy("h");
        e->cd(2);
        globEtaJet2D->ProjectionX()->SetTitle("Gloabl Tracks");
        globEtaJet2D->ProjectionX()->Rebin(4);
        globEtaJet2D->ProjectionX()->DrawCopy("h");
        e->cd();
        e->SaveAs("plots/EtaJets.pdf");

        TCanvas *ph = new TCanvas("phi", "phi", 800, 400);
        ph->Divide(2,1);
        ph->cd(1);
        hybPhiJet2D->ProjectionX()->SetTitle("Hybrid Tracks");
        hybPhiJet2D->ProjectionX()->Rebin(4);
        hybPhiJet2D->ProjectionX()->DrawCopy("h");
        ph->cd(2);
        globPhiJet2D->ProjectionX()->SetTitle("Gloabl Tracks");
        globPhiJet2D->ProjectionX()->Rebin(4);
        globPhiJet2D->ProjectionX()->DrawCopy("h");
        ph->cd();
        ph->SaveAs("plots/PhiJets.pdf");

        TCanvas *pt = new TCanvas("pt", "pt", 800, 400);
        pt->Divide(2,1);
        pt->cd(1);
        hybPtJet2D->ProjectionX()->SetTitle("Hybrid Tracks");
        hybPtJet2D->ProjectionX()->Rebin(2);
        hybPtJet2D->ProjectionX()->DrawCopy("h");
        pt->cd(2);
        globPtJet2D->ProjectionX()->SetTitle("Gloabl Tracks");
        globPtJet2D->ProjectionX()->Rebin(2);
        globPtJet2D->ProjectionX()->DrawCopy("h");
        pt->cd();
        pt->SaveAs("plots/PtJets.pdf");
}
