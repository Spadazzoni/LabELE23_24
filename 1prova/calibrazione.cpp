#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"

void Cal(Double_t Vmin, Double_t Vmax) {
  gStyle->SetOptFit(1111);
  Int_t const n = 9;
  Double_t y[n] = {104.0, 200.0, 270.0, 360.0, 500.0,
                   560.0 , 660.0, 760.0, 800.0};  // ddp misurata da oscilloscopio
  Double_t x[n] = {100.0, 192.8, 262.1, 348.0, 483.4,
                   542.1, 640.0, 730.0, 772.0};  // ddp misurata da multimetro
  Double_t ex[n] = {0.4, 0.7, 0.9, 1, 1.6, 1.7, 2, 2.2, 2.4};
  Double_t ey[n] = {4, 8, 10, 15, 18, 20, 22, 30, 31};
  TCanvas *c = new TCanvas("c", "MyCanvas", 200, 200, 1000, 600);
  TGraphErrors *g = new TGraphErrors(n, x, y, ex, ey);
  TF1 *f = new TF1("f", "[0]+[1]*x", Vmin, Vmax);
  f->SetParameters(0, 1);
  g->Fit(f, "QS0");
  g->SetTitle("Calibrazione dell'oscilloscopio");
  g->GetHistogram()->GetXaxis()->SetTitle("V_{m} (udm)");
  g->GetHistogram()->GetYaxis()->SetTitle("V_{o} (udm)");
  g->Draw("ape");
  f->Draw("same");
}