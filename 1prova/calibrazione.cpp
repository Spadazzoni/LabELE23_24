#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"
#include <fstream>

void cal()
{
  gStyle->SetOptFit(1111);
  TCanvas *c = new TCanvas("c", "MyCanvas", 200, 200, 1000, 600);
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
    errx = 0; // sqrt(2 * (r / 10) * (r / 10) + (x * 0.03) * (x * 0.03));
    erry = 0; // y * 0.015;
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
  g->Fit(f, "QS0");
  g->SetTitle("Calibrazione dell'oscilloscopio");
  g->GetHistogram()->GetXaxis()->SetTitle("V_{m} (udm)");
  g->GetHistogram()->GetYaxis()->SetTitle("V_{o} (udm)");
  g->Draw("ape");
  f->Draw("same");
}