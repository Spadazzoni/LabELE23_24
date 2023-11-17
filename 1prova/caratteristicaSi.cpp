#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"

void Car(Double_t Vmin, Double_t Vmax)
{
    gStyle->SetOptFit(1111);
    Int_t const n = 15;
    Double_t V[n] = {400, 460, 500, 520, 540, 560, 580, 600, 620, 640, 660, 680, 720, 760, 800};                 // misure di ddp
    Double_t I[n] = {0.01, 0.02, 0.05, 0.08, 0.12, 0.18, 0.27, 0.43, 0.63, 0.98, 1.53, 1.67, 3.64, 7.89, 15.24}; // misure di corrente
    Double_t eV[n] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                              // errore su V
    Double_t eI[n] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};                                              // errore su I
    TCanvas *c = new TCanvas("c", "MyCanvas", 200, 200, 1000, 600);
    TGraphErrors *g = new TGraphErrors(n, V, I, eV, eI);
    TF1 *f = new TF1("f", "[0]*(exp(x/[1])-1)", Vmin, Vmax);
    g->SetMarkerStyle(kOpenSquare);
    f->SetParameters(0.01, 42); // I0 = uA per Ge, nA per Si
    // heta = 1 Ge, 2 Si; Vt = 26 mV a 300 K
    g->Fit(f, "qs0");
    g->SetTitle("Caratteristica I-V del diodo al -Si/Ge-");
    g->GetHistogram()->GetXaxis()->SetTitle("V (udm)");
    g->GetHistogram()->GetYaxis()->SetTitle("I (udm)");
    g->Draw("ape");
    f->Draw("same");
}