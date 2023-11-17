#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"

void Car(Double_t Vmin, Double_t Vmax)
{
    gStyle->SetOptFit(1111);
    Int_t const n = 15;
    Double_t V[n] = {44, 100, 130, 160, 180, 210, 240, 270, 290, 310, 340, 380, 400, 420, 440};                 // misure di ddp
    Double_t I[n] = {0.01, 0.02, 0.05, 0.09, 0.14, 0.24, 0.4, 0.67, 0.92, 1.25, 1.96, 2.82, 3.74, 4.78, 6.03}; // misure di corrente
    Double_t eV[n] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                              // errore su V
    Double_t eI[n] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                              // errore su I
    TCanvas *c = new TCanvas("c", "MyCanvas", 200, 200, 1000, 600);
    TGraphErrors *g = new TGraphErrors(n, V, I, eV, eI);
    TF1 *f = new TF1("f", "[0]*(exp(x/[1])-1)", Vmin, Vmax);
    f->SetParameters(0.01, 42); // I0 = uA per Ge, nA per Si
    // heta = 1 Ge, 2 Si; Vt = 26 mV a 300 K
    f->SetMarkerStyle(kOpenSquare);
    g->Fit(f, "qs0");
    g->SetTitle("Caratteristica I-V del diodo al -Si/Ge-");
    g->GetHistogram()->GetXaxis()->SetTitle("V (udm)");
    g->GetHistogram()->GetYaxis()->SetTitle("I (udm)");
    g->Draw("ape");
    f->Draw("same");
}