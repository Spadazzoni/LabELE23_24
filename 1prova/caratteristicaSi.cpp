#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"
#include <fstream>
#include "TMath.h"

void carSi()
{
    gStyle->SetOptFit(1111);
    TGraphErrors *g = new TGraphErrors();
    std::ifstream in1;
    in1.open("diodoSi.dat");
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
    TF1 *f = new TF1("f", "[0]*(exp(x/[1])-1)", 350, 850);
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