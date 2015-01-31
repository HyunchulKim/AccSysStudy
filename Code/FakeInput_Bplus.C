// Code for step 2 - Get the Ratio

#include <iostream>
#include <TF1.h>
#include <TH1.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TPaveText.h>

const int nBins = 5;
double ptBins[nBins+1] = {10,15,20,25,30,60};

const int nBinsy = 5;
double yBins[nBinsy+1] = {-2.4,-1.465,-0.465,0.535,1.470,2.4};
double yCMBins[nBinsy+1] = {1.935,1.0,0.0,-1.0,-1.935,-2.865};

void FakeInput_Bplus(){

	TFile *foutDataPt = new TFile("../Inputfileforreweighting/SigmaBplusDataPt.root","read");
	TFile *foutDatay	= new TFile("../Inputfileforreweighting/SigmaBplusDatay.root","read");
	TFile *foutMCPt		= new TFile("../Inputfileforreweighting/SigmaBplusMCPt.root","read");
	TFile *foutMCy		= new TFile("../Inputfileforreweighting/SigmaBplusMCy.root","read");

	TH1D*hPtMC	 = (TH1D*)foutMCPt->Get("hPt");
	TH1D*hPtData = (TH1D*)foutDataPt->Get("hPt");
	TH1D*hyMC		 = (TH1D*)foutMCy->Get("hPt");
	TH1D*hyData	 = (TH1D*)foutDatay->Get("hPt");

	hPtMC->Sumw2();
	hPtData->Sumw2();
	hyMC->Sumw2();
	hyData->Sumw2();

	hPtMC->SetName("hPtMC");
	hPtData->SetName("hPtData");
	hyMC->SetName("hyMC");
	hyData->SetName("hyData");

	hPtMC->Scale(1./hPtMC->Integral());
	hPtData->Scale(1./hPtData->Integral());
	hyMC->Scale(1./hyMC->Integral());
	hyData->Scale(1./hyData->Integral());

	cout<<"QUI"<<hPtMC->Integral()<<endl;
	cout<<"QUI"<<hPtData->Integral()<<endl;
	cout<<"QUI"<<hyMC->Integral()<<endl;
	cout<<"QUI"<<hyData->Integral()<<endl;

	TH1D*hReweightDataOverMC_Pt=(TH1D*)hPtData->Clone("hReweightDataOverMC_Pt");
	TH1D*hReweightDataOverMC_y=(TH1D*)hyData->Clone("hReweightDataOverMC_y");

	hReweightDataOverMC_Pt->Divide(hPtData,hPtMC,1,1,"B");
	hReweightDataOverMC_y->Divide(hyData,hyMC,1,1,"B");

	// Prove whether input file is right or not in one canvas, if remove line 65-71, values of stat box in plots is changed because of unknown problem

	TCanvas*c=new TCanvas("c","c",800,500);
	c->Divide(2,2);
	c->cd(1);hPtData->Draw();
	c->cd(2);hPtMC->Draw();
	c->cd(3);hyData->Draw();
	c->cd(4);hyMC->Draw();
	c->SaveAs("prova_Bplus.pdf");

	////////////////////////////////////////////

	TFile*fout=new TFile("InputFilesMCData_Bplus.root","recreate");
	fout->cd();
	hPtData->Write();
	hPtMC->Write();
	hyData->Write();
	hyMC->Write();
	hReweightDataOverMC_Pt->Write();
	hReweightDataOverMC_y->Write();
	fout->Close();

	////////////////////////////////////////////////////////////

	TFile* fin = new TFile("InputFilesMCData_Bplus.root","read");

	hPtData=(TH1D*)fin->Get("hPtData");
	hPtMC=(TH1D*)fin->Get("hPtMC");
	hyData=(TH1D*)fin->Get("hyData");
	hyMC=(TH1D*)fin->Get("hyMC");
	hReweightDataOverMC_Pt=(TH1D*)fin->Get("hReweightDataOverMC_Pt");
	hReweightDataOverMC_y=(TH1D*)fin->Get("hReweightDataOverMC_y");

	TF1 *fRfitft_Bpluspt = new TF1("fRfitft_Bpluspt","[0]+x*[1]",10.0,60.0);//Bplus, pt dependence
	//TF1 *fRfitft_Bplusy = new TF1("fRfitft_Bplusy","[0]*pow((x-0.0),2)+[1]",-2.865,1.935);
	TF1 *fRfitft_Bplusy = new TF1("fRfitft_Bplusy","[0]*pow(x,2)+[1]",-2.865,1.935);//Bplus, y dependence

	TH1D* hParafitft_Bpluspt = new TH1D("hParafitft_Bpluspt","",2,0,2);
	TH1D* hParafitft_Bplusy  = new TH1D("hParafitft_Bplusy","",2,0,2);

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
	pt11->AddText("B^{+}");

	hPtMC->GetYaxis()->SetTitle("Proportion of uncorrected B^{+} dN/dp_{T} (Data/MC)");
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
	canvas11->SaveAs("AccSys_CompNormUncorYield_Bplus.pdf");

	hReweightDataOverMC_Pt->SetMinimum(0.6);
	hReweightDataOverMC_Pt->SetMaximum(1.8);
	hReweightDataOverMC_Pt->Draw("p");
	hReweightDataOverMC_Pt->SetMarkerStyle(20);
	hReweightDataOverMC_Pt->SetMarkerSize(1.5);
	hReweightDataOverMC_Pt->SetLineWidth(1.0);
	hReweightDataOverMC_Pt->SetLineColor(kBlack);
	hReweightDataOverMC_Pt->GetYaxis()->SetTitle("Ratio of uncorrected B^{+} dN/dp_{T} (Data/MC)");

	fRfitft_Bpluspt->SetLineColor(kRed);
	fRfitft_Bpluspt->SetLineWidth(2.0);
	std::cout << "##### Fit with fRfitft_Bpluspt \"[0]+x*[1]\" #############" << std::string(20,'#') << std::endl; 
	hReweightDataOverMC_Pt->Fit("fRfitft_Bpluspt"); 
	std::cout << std::string(76,'#') << std::endl;
	hParafitft_Bpluspt->SetBinContent(1,fRfitft_Bpluspt->GetParameter(0));
	hParafitft_Bpluspt->SetBinContent(2,fRfitft_Bpluspt->GetParameter(1));

	pt11->Clear();
	pt11 = new TPaveText(0.22,0.80,0.27,0.85,"NDC");
	pt11->SetFillColor(0);
	pt11->SetTextSize(0.05);
	pt11->SetBorderSize(0);
	pt11->AddText("B^{+}");
	pt11->Draw();
	canvas11->SaveAs("AccSys_RatioNormUncorYield_Bplus.pdf");

	for (int i=0;i<5;i++) {
		std::cout << i << ": " << hReweightDataOverMC_Pt->GetBinContent(i+1) << " , " << hReweightDataOverMC_Pt->GetBinError(i+1) << std::endl;
	}

	leg11->Clear();
	leg11->AddEntry(hyData,"Data","lep");
	leg11->AddEntry(hyMC,"MC","f");
	leg11->SetFillColor(kWhite);
	leg11->SetBorderSize(0);

	pt11->Clear();
	pt11->AddText("B^{+}");

	hyMC->GetYaxis()->SetTitle("Proportion of uncorrected B^{+} dN/dy_{CM} (Data/MC)");
	hyMC->SetMinimum(0);
	hyMC->SetMaximum(0);
	hyMC->SetFillColor(kPink+6);
	//hyMC->SetFillStyle(3004);
	hyMC->SetLineWidth(1);
	hyMC->SetLineColor(kBlack);
	hyMC->Draw("H");
	hyData->SetMarkerStyle(21);
	hyData->SetMarkerSize(1.0);
	hyData->SetLineWidth(1);
	hyData->SetLineColor(kBlack);
	hyData->Draw("samepe");
	leg11->Draw();
	pt11->Clear();
	pt11 = new TPaveText(0.22,0.62,0.27,0.67,"NDC");
	pt11->SetFillColor(0);
	pt11->SetTextSize(0.05);
	pt11->SetBorderSize(0);
	pt11->AddText("B^{0}");
	pt11->Draw();
	canvas11->SaveAs("AccSys_CompNormUncorYield_Bplusy.pdf");

	hReweightDataOverMC_y->SetMinimum(0.6);
	hReweightDataOverMC_y->SetMaximum(1.8);
	hReweightDataOverMC_y->Draw("p");
	hReweightDataOverMC_y->SetMarkerStyle(20);
	hReweightDataOverMC_y->SetMarkerSize(1.5);
	hReweightDataOverMC_y->SetLineWidth(1.0);
	hReweightDataOverMC_y->SetLineColor(kBlack);
	hReweightDataOverMC_y->GetYaxis()->SetTitle("Ratio of uncorrected B^{+} dN/dy_{CM} (Data/MC)");

	fRfitft_Bplusy->SetLineColor(kRed);
	fRfitft_Bplusy->SetLineWidth(2.0);
	std::cout << "##### Fit with fRfitft_Bplusy  \"[0]*pow(x,2)+[1]\" ######" << std::string(20,'#') << std::endl; 
	hReweightDataOverMC_y->Fit("fRfitft_Bplusy"); 
	std::cout << std::string(76,'#') << std::endl;
	hParafitft_Bplusy->SetBinContent(1,fRfitft_Bplusy->GetParameter(0));
	hParafitft_Bplusy->SetBinContent(2,fRfitft_Bplusy->GetParameter(1));

	pt11->Clear();
	pt11 = new TPaveText(0.22,0.80,0.27,0.85,"NDC");
	pt11->SetFillColor(0);
	pt11->SetTextSize(0.05);
	pt11->SetBorderSize(0);
	pt11->AddText("B^{+}");
	pt11->Draw();
	canvas11->SaveAs("AccSys_RatioNormUncorYield_Bplusy.pdf");

	for (int i=0;i<5;i++) {
		std::cout << i << ": " << hReweightDataOverMC_y->GetBinContent(i+1) << " , " << hReweightDataOverMC_y->GetBinError(i+1) << std::endl;
	}

/*
	// Prove pt dependence results in one canvas

	double p0,p1,p2,p3;
	TF1 *fRexpoPt2 = new TF1("fRexpoPt2","(TMath::Exp([0]-x*[1]))/(TMath::Exp([2]-x*[3]))",10.0,60.0);

	TF1 *fexpoPtData = new TF1("fexpoPtData","TMath::Exp([0]-x*[1])",10.0,60.0);
	TF1 *fexpoPtMC = new TF1("fexpoPtMC","TMath::Exp([0]-x*[1])",10.0,60.0);
	TF1 *fRexpoPt = new TF1("fRexpoPt","(TMath::Exp([0]-x*[1]))/(TMath::Exp([2]-x*[3]))",10.0,60.0);

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
	fRfitft_Bpluspt->SetLineColor(kGreen);
	hReweightDataOverMC_Pt->Fit("fRfitft_Bpluspt"); 
	canvas->SaveAs("FitstoRatioDataMC_Bplus.pdf");

	// Prove y dependence results in one canvas

	TCanvas*canvas2 = new TCanvas("canvas2","canvas2",900,300);
	canvas2->Divide(3,1);

	canvas2->cd(1);

	hyData->SetMinimum(0);
	hyData->SetMaximum(1);
	hyData->Draw("p");
	hyData->SetMarkerStyle(22);
	hyData->SetMarkerSize(0.5);

	canvas2->cd(2);

	hyMC->SetMinimum(0);
	hyMC->SetMaximum(1);
	hyMC->Draw("p");
	hyMC->SetMarkerStyle(22);
	hyMC->SetMarkerSize(0.5);

	canvas2->cd(3);

	hReweightDataOverMC_y->SetMinimum(0.6);
	hReweightDataOverMC_y->SetMaximum(1.8);
	hReweightDataOverMC_y->Draw("p");
	hReweightDataOverMC_y->SetMarkerStyle(22);
	hReweightDataOverMC_y->SetMarkerSize(0.5);

	TF1 *fRy2 = new TF1("fRy2","[0]*pow((x-[2]),2)+[1]",-2.865,1.935);
	fRfitft_Bplusy->SetLineColor(kBlue);
	hReweightDataOverMC_y->Fit("fRfitft_Bplusy");
	fRy2->SetLineColor(kRed);
	hReweightDataOverMC_y->Fit("fRy2");
	//fRfitft_Bplusy->Draw("same");
	fRy2->Draw("same");

	canvas2->SaveAs("FitstoRatioDataMC_Bplusy.pdf");
*/
	////////////////////////////////////////////////////////////////////////////////////////

	TFile* fout2 = new TFile("FunctionsReweighting_Bplus.root","recreate");
	fout2->cd();
	hPtData->Write();
	hPtMC->Write();
	hReweightDataOverMC_Pt->Write();
	hyData->Write();
	hyMC->Write();
	hReweightDataOverMC_y->Write();
	fRfitft_Bplusy->Write();
	fRfitft_Bpluspt->Write();
	hParafitft_Bpluspt->Write();
	hParafitft_Bplusy->Write();
	/*
		 fexpoPtData->Write();
		 fexpoPtMC->Write();
		 fRexpoPt->Write();
	 */
	fout2->Close();
}
