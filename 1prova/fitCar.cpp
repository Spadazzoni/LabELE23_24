#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TStyle.h"
#include <fstream>
#include "TMath.h"
#include "TLegend.h"

void car()
{
    double errSiV[15]; // Array per salvare gli errori sulle misure
    double errSiI[15];
    double errGeV[15];
    double errGeI[15];
    TGraphErrors *gS = new TGraphErrors(); // grafici Si, Ge
    TGraphErrors *gG = new TGraphErrors();
    TCanvas *cS = new TCanvas();
    TCanvas *cG = new TCanvas();
    std::ifstream inS;
    std::ifstream inG;
    inS.open("diodoSi.dat"); // apro il file dati
    double xs, rs, ys, errxs, errys;
    double xg, rg, yg, errxg, erryg;
    int nS = 0;
    int nG = 0;
    // ciclo di riempimento dei grafici
    while (1)
    {
        // grafico Si
        if (!inS.good())
        {
            break;
        }
        inS >> xs >> rs >> ys;
        nS = gS->GetN();
        errxs = sqrt(2 * (rs / 10) * (rs / 10) + (xs * 0.03) * (xs * 0.03));
        errSiV[nS] = errxs;
        errys = ys * 0.015 + 0.02;
        errSiI[nS] = errys;
        gS->SetPoint(nS, xs, ys);
        gS->SetPointError(nS, errxs, errys);
    }
    inS.close();

    // per il germanio
    inG.open("diodoGe.dat");
    while (1)
    {
        // grafico Ge
        if (!inG.good())
        {
            break;
        }
        inG >> xg >> rg >> yg;
        nG = gG->GetN();
        errxg = sqrt(2 * (rg / 10) * (rg / 10) + (xg * 0.03) * (xg * 0.03));
        errGeV[nG] = errxg;
        erryg = yg * 0.015 + 0.02;
        errGeI[nG] = erryg;
        gG->SetPoint(nG, xg, yg);
        gG->SetPointError(nG, errxg, erryg);
    }
    inG.close();

    // metto titolo e assi ai grafici
    gS->SetTitle("Caratteristica I-V del diodo al silicio");
    gS->GetHistogram()->GetXaxis()->SetTitle("V (mV)");
    gS->GetHistogram()->GetYaxis()->SetTitle("I (mA)");

    gG->SetTitle("Caratteristica I-V del diodo al germanio");
    gG->GetHistogram()->GetXaxis()->SetTitle("V (mV)");
    gG->GetHistogram()->GetYaxis()->SetTitle("I (mA)");

    // creo le funzioni da fittare
    TF1 *fS = new TF1("fs", "[0]*(exp(x/[1])-1)", 600, 800);
    TF1 *fSd = new TF1("fSd", "[0]*(exp(x/[1])-1)", 350, 850); // questa la disegno perchè la prima è fittata su un range più piccolo
    TF1 *fG = new TF1("fG", "[0]*(exp(x/[1])-1)", 350, 450);
    TF1 *fGd = new TF1("fG", "[0]*(exp(x/[1])-1)", 40, 450);

    fS->SetParameters(0.01, 42); // parametri di prova iniziali
    fG->SetParameters(0.01, 42);

    gS->Fit(fS, "rqs0"); // r->mette il range di fit (uguale a quello di definizione di f), q->non stampa i risultati,
                         // s->salva i risultati in un vettore TFitResultPtr , 0-> Non disegna direttamente il fit
    fSd->SetParameters(fS->GetParameter(0), fS->GetParameter(1));
    gS->Fit(fSd, "rqs0"); // metto i risultati del fit nei parametri di quella da disegnare
    gG->Fit(fG, "rqs0");
    fGd->SetParameters(fG->GetParameter(0), fG->GetParameter(1));
    gG->Fit(fGd, "rqs0");

    // inserisco le legende
    TLegend *lS = new TLegend(0.11, 0.75, 0.5, 0.89);
    lS->AddEntry(gS, "Punti sperimentali");
    lS->AddEntry(fSd, "Fit");

    TLegend *lG = new TLegend(0.11, 0.75, 0.5, 0.89);
    lG->AddEntry(gG, "Punti sperimentali");
    lG->AddEntry(fGd, "Fit");

    // salvo i dati in un file
    std::ofstream txt("./Result.txt", std::ofstream::out);
    if (txt.is_open())
    {
        txt << "============================" << '\n';
        txt << "||  RISULTATI SPERIMENTALI ||" << '\n';
        txt << "============================" << '\n';
        txt << '\n';
        txt << '\n';
        txt << "I_0 silicio = " << fSd->GetParameter(0) << " +/- " << fSd->GetParError(0) << '\n';
        txt << "#etaV_T silicio = " << fSd->GetParameter(1) << " +/- " << fSd->GetParError(1) << '\n'
            << '\n';
        txt << "I_0 germanio = " << fGd->GetParameter(0) << " +/- " << fGd->GetParError(0) << '\n';
        txt << "#etaV_T germanio = " << fGd->GetParameter(1) << " +/- " << fGd->GetParError(1) << '\n'
            << '\n';
        /* for (int i = 0; i < 15; ++i)
         {
             txt << "Err" << i << " Si v = " << errSiV[i] << '\n';
         }
         txt << '\n';
         for (int i = 0; i < 15; ++i)
         {
             txt << "Err" << i << " Si i = " << errSiI[i] << '\n';
         }
         txt << '\n';
         for (int i = 0; i < 15; ++i)
         {
             txt << "Err" << i << " Ge v = " << errGeV[i] << '\n';
         }
         txt << '\n';
         for (int i = 0; i < 15; ++i)
         {
             txt << "Err" << i << " Ge i = " << errGeI[i] << '\n';
         }*/
        txt.close();
    }

    // disegno i grafici e imposto alcune opzioni grafiche
    cS->cd();
    cS->SetGrid();
    gS->SetMarkerStyle(8);
    gPad->SetLogy(1);
    gS->Draw("ape");
    fSd->Draw("same");
    lS->Draw("same");

    cG->cd();
    cG->SetGrid();
    gG->SetMarkerStyle(8);
    gPad->SetLogy(1);
    gG->Draw("ape");
    fGd->Draw("same");
    lG->Draw("same");

    cS->SaveAs("Silicio.pdf");
    cG->SaveAs("Germanio.pdf");
}