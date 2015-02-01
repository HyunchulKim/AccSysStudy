#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>

  const int nBins = 3;
  double ptBins[nBins+1] = {10,15,20,60};
  
  const int nBinsy = 5;
  double yBins[nBinsy+1] = {-2.4,-1.465,-0.465,0.535,1.470,2.4};
  double yCMBins[nBinsy+1] = {1.935,1.0,0.0,-1.0,-1.935,-2.865};

void FakeInput_Bzero(){

  TFile *foutDataPt = new TFile("../Inputfileforreweighting/SigmaBzero.root","read");
  TFile *foutMCPt		= new TFile("../Inputfileforreweighting/SigmaBzeroMC.root","read");
/*
  TFile *foutDataPt = new TFile("/Users/hyunchulKIM/BanaStudy/20141031/ResultsBzero/SigmaBzero.root","read");
  TFile *foutMCPt		= new TFile("/Users/hyunchulKIM/BanaStudy/20141031/ResultsBzero/SigmaBzeroMC.root","read");
*/
  TH1D*hPtMC=(TH1D*)foutMCPt->Get("hPt");
  TH1D*hPtData=(TH1D*)foutDataPt->Get("hPt");

  hPtMC->Sumw2();
  hPtData->Sumw2();

  hPtMC->SetName("hPtMC");
  hPtData->SetName("hPtData");

  hPtMC->Scale(1./hPtMC->Integral());
  hPtData->Scale(1./hPtData->Integral());
  
  cout<<"QUI"<<hPtMC->Integral()<<endl;
  cout<<"QUI"<<hPtData->Integral()<<endl;
  
  TH1D*hReweightDataOverMC_Pt=(TH1D*)hPtData->Clone("hReweightDataOverMC_Pt");
  
  hReweightDataOverMC_Pt->Divide(hPtData,hPtMC,1,1,"B");

	// Prove whether input file is right or not in one canvas, if remove line 65-71, values of stat box in plots is changed because of unknown problem

  TCanvas*c=new TCanvas("c","c",800,500);
  c->Divide(2,2);
  c->cd(1);hPtData->Draw();
  c->cd(2);hPtMC->Draw();
  c->SaveAs("prova_Bzero.pdf");

	///////////////////////////////////////////////////////
  
  TFile*fout=new TFile("../Results/InputFilesMCData_Bzero.root","recreate");
  fout->cd();
  hPtData->Write();
  hPtMC->Write();
  hReweightDataOverMC_Pt->Write();
  fout->Close();

/////////////////////////////////////////////////////////////

  TFile* fin = new TFile("../Results/InputFilesMCData_Bzero.root","read");

  hPtData=(TH1D*)fin->Get("hPtData");
  hPtMC=(TH1D*)fin->Get("hPtMC");
  hReweightDataOverMC_Pt=(TH1D*)fin->Get("hReweightDataOverMC_Pt");

  TF1 *fRfitft_Bzeropt = new TF1("fRfitft_Bzeropt","[0]+x*[1]",10.0,60.0);

	TH1D* hParafitft_Bzeropt = new TH1D("hParafitft_Bzeropt","",2,0,2);

  TCanvas* canvas11 = new TCanvas("canvas11","",600,600);
  canvas11->cd();

  TLegend* leg11 = new TLegend(0.20,0.70,0.45,0.85);
  leg11->AddEntry(hPtData,"Data","lep");
  leg11->AddEntry(hPtMC,"MC","f");
  leg11->SetFillColor(kWhite);
  leg11->SetBorderSize(0);

  TPaveText* pt11 = new TPaveText(0.22,0.62,0.27,0.67,"NDC");
  pt11->SetFillColor(0);
  pt11->SetTextSize(0.05);
  pt11->SetBorderSize(0);
  pt11->AddText("B^{0}");

  hPtMC->GetYaxis()->SetTitle("Proportion of uncorrected B^{0} dN/dp_{T} (Data/MC)");
  hPtMC->SetMinimum(0);
  hPtMC->SetMaximum(0);
  hPtMC->SetFillColor(kPink+6);
  //hPtMC->SetFillStyle(3004);
  hPtMC->SetLineWidth(1);
  hPtMC->SetLineColor(kBlack);
  hPtMC->Draw("H");
  hPtData->SetMarkerStyle(21);
  hPtData->SetMarkerSize(1.0);
  hPtData->SetLineWidth(1);
  hPtData->SetLineColor(kBlack);
  hPtData->Draw("samepe");
  leg11->Draw();
  pt11->Draw();
  canvas11->SaveAs("../Results/AccSys_CompNormUncorYield_Bzero.pdf");

  hReweightDataOverMC_Pt->SetMinimum(0.6);
  hReweightDataOverMC_Pt->SetMaximum(1.8);
  hReweightDataOverMC_Pt->Draw("p");
  hReweightDataOverMC_Pt->SetMarkerStyle(20);
  hReweightDataOverMC_Pt->SetMarkerSize(1.5);
  hReweightDataOverMC_Pt->SetLineWidth(1.0);
  hReweightDataOverMC_Pt->SetLineColor(kBlack);
  hReweightDataOverMC_Pt->GetYaxis()->SetTitle("Ratio of uncorrected B^{0} dN/dp_{T} (Data/MC)");

  fRfitft_Bzeropt->SetLineColor(kRed);
  fRfitft_Bzeropt->SetLineWidth(2.0);
	std::cout << "##### Fit with fRfitft_Bzeropt \"[0]+x*[1]\" #############" << std::string(20,'#') << std::endl;
  hReweightDataOverMC_Pt->Fit("fRfitft_Bzeropt");
	std::cout << std::string(76,'#') << std::endl;
  hParafitft_Bzeropt->SetBinContent(1,fRfitft_Bzeropt->GetParameter(0));
	hParafitft_Bzeropt->SetBinContent(2,fRfitft_Bzeropt->GetParameter(1));

  pt11->Clear();
  pt11 = new TPaveText(0.22,0.80,0.27,0.85,"NDC");
  pt11->SetFillColor(0);
  pt11->SetTextSize(0.05);
  pt11->SetBorderSize(0);
  pt11->AddText("B^{0}");
  pt11->Draw();
  canvas11->SaveAs("../Results/AccSys_RatioNormUncorYield_Bzero.pdf");

/*
  TF1 *fexpoPtData = new TF1("fexpoPtData","TMath::Exp([0]-x*[1])",10.0,60.0);
  TF1 *fexpoPtMC = new TF1("fexpoPtMC","TMath::Exp([0]-x*[1])",10.0,60.0);
  TF1 *fRexpoPt = new TF1("fRexpoPt","(TMath::Exp([0]-x*[1]))/(TMath::Exp([2]-x*[3]))",10.0,60.0);
  TF1 *fRexpoPt2 = new TF1("fRexpoPt2","(TMath::Exp([0]-x*[1]))/(TMath::Exp([2]-x*[3]))",10.0,60.0);
  double p0,p1,p2,p3;

 //  TCanvas* canvas = new TCanvas("canvas","canvas",1800,1200);
//  canvas->Divide(3,2);
  TCanvas* canvas = new TCanvas("canvas","canvas",900,300);
  canvas->Divide(3,1);

  canvas->cd(1);
  
  hPtData->SetMinimum(0);
  hPtData->SetMaximum(1);
  hPtData->Draw("p");
  hPtData->SetMarkerStyle(22);
  hPtData->SetMarkerSize(0.5);
  //fexpoPtData->SetParameter(1.19402,1.44347e-01);
  hPtData->Fit("fexpoPtData");
  p0=fexpoPtData->GetParameter(0);
  p1=fexpoPtData->GetParameter(1);

  canvas->cd(2);

  hPtMC->SetMinimum(0);
  hPtMC->SetMaximum(1);
  hPtMC->Draw("p");
  hPtMC->SetMarkerStyle(22);
  hPtMC->SetMarkerSize(0.5);
  hPtMC->Fit("fexpoPtMC");
  p2=fexpoPtMC->GetParameter(0);
  p3=fexpoPtMC->GetParameter(1);

  canvas->cd(3);

  hReweightDataOverMC_Pt->SetMinimum(0.6);
  hReweightDataOverMC_Pt->SetMaximum(1.8);
  hReweightDataOverMC_Pt->Draw("p");
  hReweightDataOverMC_Pt->SetMarkerStyle(22);
  hReweightDataOverMC_Pt->SetMarkerSize(0.5);
  fRexpoPt->SetParameters(p0,p1,p2,p3);
  fRexpoPt->SetLineColor(kBlue);
  fRexpoPt->Draw("same");
  fRexpoPt2->SetParameters(p0,p1,p2,p3);
  hReweightDataOverMC_Pt->Fit("fRexpoPt2");
  fRexpoPt2->SetLineColor(kRed);
  fRexpoPt2->Draw("same");
  fRfitft_Bzeropt->SetLineColor(kGreen);
  hReweightDataOverMC_Pt->Fit("fRfitft_Bzeropt"); 
  canvas->SaveAs("../Results/FitstoRatioDataMC_Bzero.pdf");
  for (int i=0;i<5;i++) {
    std::cout << i << ": " << hReweightDataOverMC_Pt->GetBinContent(i+1) << " , " << hReweightDataOverMC_Pt->GetBinError(i+1) << std::endl;
  }
  //canvas->Close();
*/
/*
  TCanvas*canvas2 = new TCanvas("canvas2","canvas2",900,300);
  canvas2->Divide(3,1);

  canvas2->cd(1);
  
  hyData->SetMinimum(0);
  hyData->SetMaximum(1);
  hyData->Draw("p");
  hyData->SetMarkerStyle(22);
  hyData->SetMarkerSize(0.5);
  //fexpoyData->SetParameter(1.19402e+00,1.44347e-01);
  //hyData->Fit("fexpoyData");
  //p0=fexpoyData->GetParameter(0);
  //p1=fexpoyData->GetParameter(1);

  canvas2->cd(2);

  hyMC->SetMinimum(0);
  hyMC->SetMaximum(1);
  hyMC->Draw("p");
  hyMC->SetMarkerStyle(22);
  hyMC->SetMarkerSize(0.5);
  //hyMC->Fit("fexpoyMC");
  //p2=fexpoyMC->GetParameter(0);
  //p3=fexpoyMC->GetParameter(1);

  canvas2->cd(3);

  hReweightDataOverMC_y->SetMinimum(0.6);
  hReweightDataOverMC_y->SetMaximum(1.8);
  hReweightDataOverMC_y->Draw("p");
  hReweightDataOverMC_y->SetMarkerStyle(22);
  hReweightDataOverMC_y->SetMarkerSize(0.5);
  fRy->SetLineColor(kBlue);
  hReweightDataOverMC_y->Fit("fRy");
  fRy2->SetLineColor(kRed);
  hReweightDataOverMC_y->Fit("fRy2");
  fRy->Draw("same");
  fRy2->Draw("same");

  //fRexp->SetLineColor(kRed);
  //fRexpoy2->Draw("same");
  //fRexpoy3->SetLineColor(kGreen);
  //hReweightDataOverMC_y->Fit("fRexpoy3"); 
  canvas2->SaveAs("../Results/FitstoRatioDataMC_3_20141031_v2.pdf");
  for (int i=0;i<5;i++) {
    std::cout << i << ": " << hReweightDataOverMC_y->GetBinContent(i+1) << " , " << hReweightDataOverMC_y->GetBinError(i+1) << std::endl;
  }
*/

  TFile* fout2 = new TFile("../Results/FunctionsReweighting_Bzero.root","recreate");
  fout2->cd();
  hPtData->Write();
  hPtMC->Write();
  hReweightDataOverMC_Pt->Write();
  fRfitft_Bzeropt->Write();
	hParafitft_Bzeropt->Write(); 
/*
  fexpoPtData->Write();
  fexpoPtMC->Write();
  fRexpoPt->Write();
*/
  fout2->Close();
}
