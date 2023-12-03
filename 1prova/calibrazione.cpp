#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"
#include "TLegend.h"
#include <fstream>

void cal()
{
  TCanvas *c = new TCanvas("c", "MyCanvas", 200, 200, 1000, 600);
  c->SetGrid();
  TGraphErrors *g = new TGraphErrors();
  g->SetMarkerStyle(kOpenCircle);
  std::ifstream in1;
  in1.open("calibrazione.dat");
  double x, r, y, errx, erry;
  int N;
  while (1)
  {
    in1 >> x >> r >> y;
    N = g->GetN();
    errx = sqrt(2 * (r / 10) * (r / 10) + (x * 0.03) * (x * 0.03));
    erry = y * 0.003 + 0.1;
    if (!in1.good())
    {
      break;
    }
    g->SetPoint(N, x, y);

    g->SetPointError(N, errx, erry);
  }
  in1.close();
  TF1 *f = new TF1("f", "[0]+[1]*x", 80, 820);
  f->SetParameters(0, 1);
  f->SetParNames("q", "m");
  g->Fit(f, "rS0");
  g->SetMarkerStyle(8);
  // Legenda dei punti e del fit
  TLegend *leg = new TLegend(0.11, 0.75, 0.5, 0.89);
  leg->AddEntry(g, "Punti sperimentali");
  leg->AddEntry(f, "Fit lineare");
  gStyle->SetOptStat();
  g->SetTitle("Calibrazione dell'oscilloscopio");
  g->GetHistogram()->GetXaxis()->SetTitle("V_{m} (mV)"); // i dati della calibrazione sono tutti presi in mV
  g->GetHistogram()->GetYaxis()->SetTitle("V_{o} (mV)");
  g->Draw("ape");
  leg->Draw("same");
  f->Draw("same");
  c->SaveAs("Calibrazione.pdf");
}