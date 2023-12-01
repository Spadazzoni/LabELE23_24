#include <fstream>

#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"
#include "TMultiGraph.h"

void Fit(Double_t const Vmin, Double_t const Vmax) {
  gStyle->SetOptFit(1111);
  std::fstream in1;
  std::fstream in2;
  in1.open("Car200uA.txt");
  in2.open("Car100uA.txt");
  TGraphErrors *g1 = new TGraphErrors();
  TGraphErrors *g2 = new TGraphErrors();
  TCanvas *c = new TCanvas();
  Double_t I;
  Double_t V;
  Double_t fs;  // fondoscala
  Double_t ErrI;
  Double_t ErrV;
  Int_t i = 0;  // indice file 1
  Int_t j = 0;  // indice file 2
  while (1) {
    if (!in1.good()) {
      break;
    }
    in1 >> V >> I >> fs;        // prima colonna V; seconda I; terza fs
    Double_t ErrV1 = fs * 0.1;  // errore sullo 0 e sulla lettura
    Double_t ErrV2 = V * 0.03;  // errore del costruttore
    ErrV = sqrt(2 * ErrV1 * ErrV1 + ErrV2 * ErrV2);
    ErrI = I * 0.03;
    g1->SetPoint(i, V, I);
    g1->SetPointError(i, ErrV, ErrI);
    ++i;
  }
  in1.close();
  while (1) {
    if (!in2.good()) {
      break;
    }
    in2 >> V >> I >> fs;        // prima colonna V; seconda I; terza fs
    Double_t ErrV1 = fs * 0.1;  // errore sullo 0 e sulla lettura
    Double_t ErrV2 = V * 0.03;  // errore del costruttore
    ErrV = sqrt(2 * ErrV1 * ErrV1 + ErrV2 * ErrV2);
    ErrI = I * 0.03;
    g2->SetPoint(j, V, I);
    g2->SetPointError(j, ErrV, ErrI);
    ++j;
  }
  in2.close();
  TF1 *f1 = new TF1("f1", "x/[0]-[1]", Vmin, Vmax);
  TF1 *f2 = new TF1("f2", "x/[0]-[1]", Vmin, Vmax);
  f1->SetLineColor(kBlue);
  f2->SetLineColor(kRed);
  g1->Fit(f1, "qs0");
  g2->Fit(f2, "qs0");
  g1->SetMarkerStyle(kOpenCircle);
  g2->SetMarkerStyle(kOpenCircle);
  auto *mg = new TMultiGraph();
  mg->Add(g1);
  mg->Add(g2);
  mg->SetTitle("Caratteristica I-V del transistor in regione attiva; V (udm); I (udm)");
  mg->Draw("ape");
}