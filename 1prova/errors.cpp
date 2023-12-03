#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"
#include <fstream>
#include "TMath.h"
#include "TLegend.h"

void err()
{
    std::fstream in1;
    std::fstream in2;
    in1.open("diodoSi.dat");
    in2.open("diodoGe.dat");
    std::ofstream txt("./Errors.txt", std::ofstream::out);
    double xs, xg, ys, yg, rs, rg;
    double errSiV, errGeV, errSiI, errGeI;
    while (1)
    {
        in1 >> xs >> rs >> ys;
        errSiV = sqrt(2 * (rs / 10) * (rs / 10) + (xs * 0.03) * (xs * 0.03));
        errSiI = ys * 0.015 + 0.02;
        in2 >> xg >> rg >> yg;
        errGeV = sqrt(2 * (rg / 10) * (rg / 10) + (xg * 0.03) * (xg * 0.03));
        errGeI = yg * 0.015 + 0.02;
        txt << errSiV << " " << errSiI << '\n';
        txt << errGeV << " " << errGeI << '\n'
            << '\n';
        if (!in1.good() || !in2.good())
        {
            break;
        }
    }
}