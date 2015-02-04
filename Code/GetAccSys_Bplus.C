#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TLegend.h>
#include <TPaveStats.h>
#include <TPad.h>
#include <TStyle.h>
#include <TLine.h>
#include <TROOT.h>

void GetMinMaxMean(TH1D* hs);

void GetAccSys_Bplus() {

	gROOT->Macro("rootlogon.C+");
	gStyle->SetCanvasDefW(800);
	gStyle->SetCanvasDefH(800);

	//TFile* fratio_gaus_ptdep=new TFile("test_20141031_gaus_try10k.root");
	//TFile* fratio_gaus_ydep=new TFile("test_20141031_gaus_ydep_try10k.root");
	TFile* fratio_gaus_ptdep=new TFile("../Results/RunMCtoy_gaus_wBpluspt_try100k.root");
	TFile* fratio_gaus_ydep=new TFile("../Results/test_20141031_gaus_ydep_try10k.root");

	TTree* tr11=(TTree*)fratio_gaus_ptdep->Get("ditTree");
	TTree* tr21=(TTree*)fratio_gaus_ydep->Get("ditTree");

	TH1D* htest11=(TH1D*)fratio_gaus_ptdep->Get("htest");
	TH1D* htest21=(TH1D*)fratio_gaus_ydep->Get("htest");

	double a1_gaus,a2_gaus;
	double ay1_gaus,ay2_gaus;

	tr11->SetBranchAddress("a1", &a1_gaus);
	tr11->SetBranchAddress("a2", &a2_gaus);
	tr21->SetBranchAddress("ay1", &ay1_gaus);
	tr21->SetBranchAddress("ay2", &ay2_gaus);

	TFile* facc_gaus_ptdep=new TFile("../Results/AccAna_PtYbinBp_Toy1stpolPtVar_Bu_boosted_BanaAcc_IDfilters_intot100M_20150204.root");
	TFile* facc_gaus_ydep=new TFile("../Results/AccAna_PtYbinBp_Toy2ndpolYvar_Bu_boosted_BanaAcc_IDfilters_intot100M_20150204.root");

	//TFile* fout=new TFile("AccSys_Bplus_20141104.root","RECREATE");
	TFile* fout=new TFile("../Results/AccSys_Bplus.root","RECREATE");

	TH1D* hf11pt[5];
	TH1D* hf21pt[5];
	TH1D* hf11y[5];
	TH1D* hf21y[5];

	TH1D* hpullf11pt[5];
	TH1D* hpullf21pt[5];
	TH1D* hpullf11y[5];
	TH1D* hpullf21y[5];

	TH1D* ha1gaus=new TH1D("ha1gaus",";[0] ([0]p_{T}+[1]);Entries",200,-0.04,0.06);
	TH1D* ha2gaus=new TH1D("ha2gaus",";[1] ([0]p_{T}+[1]);Entries",260,0.1,1.4);
	TH1D* hay1gaus=new TH1D("hay1gaus",";[0] ([0](y+0.465)^{2}+[1]);Entries",200,-0.15,0.25);
	TH1D* hay2gaus=new TH1D("hay2gaus",";[1] ([0](y+0.465)^{2}+[1]);Entries",250,0.6,1.1);

	TFile* fin_AccDef = new TFile("../InputAccFiles/AccAna_PtYbinBp_nowt_Bu_boosted_BanaAcc_IDfilters_intot100M_20150204_default.root");

	TH1F* hAcc1TriPtBin_AccDef = (TH1F*)fin_AccDef->Get("hAcc1TriPtBin_PtYbinBp");
	TH1F* hAcc1TriYBin_AccDef = (TH1F*)fin_AccDef->Get("hAcc1TriYBin_PtYbinBp");
	double avghpt[5];
	double avghy[5];
	for (int i=0;i<5;i++) {
		avghpt[i]=hAcc1TriPtBin_AccDef->GetBinContent(i+1);
		avghy[i]=hAcc1TriYBin_AccDef->GetBinContent(i+1);
	}

	//double avghpt[5]={0.2458,0.4240,0.5134,0.6468,0.7215};
	//double avghy[5]={0.3395,0.3186,0.2486,0.3189,0.3422};


	int ptbin[6]={10,15,20,25,30,60};
	double ybin[6]={-2.4,-1.465,-0.465,0.535,1.470,2.4};
	//double ybinCM[6]={1.935,1.0,0.0,-1.0,-1.935,-2.865};

	for (int i=0;i<5;i++) {
		TH1D* res11pt = (TH1D*)facc_gaus_ptdep->Get(Form("hAccCompBin%i",i+1));
		TH1D* res21pt = (TH1D*)facc_gaus_ydep->Get(Form("hAccCompBin%i",i+1));
		hf11pt[i]=(TH1D*)res11pt->Clone(Form("hf11pt_%i",i+1));
		hf21pt[i]=(TH1D*)res21pt->Clone(Form("hf21pt_%i",i+1));

		double avg11=hf11pt[i]->GetMean();
		double sig11=hf11pt[i]->GetRMS(); 
		double avg21=hf21pt[i]->GetMean();
		double sig21=hf21pt[i]->GetRMS(); 

		double avgh=avghpt[i]; 
		std::cout << avg11 << " " << sig11 << std::endl;
		hpullf11pt[i]=new TH1D(Form("hpullf11pt_%i",i+1),Form("Pull distribution in (%i#leqp_{T}<%i) (GeV/c);pull;Entries",ptbin[i],ptbin[i+1]),1000,((avgh-0.0500-avg11)/sig11),((avgh+0.0500-avg11)/sig11));
		hpullf21pt[i]=new TH1D(Form("hpullf21pt_%i",i+1),Form("Pull distribution in (%i#leqp_{T}<%i) (GeV/c);pull;Entries",ptbin[i],ptbin[i+1]),1000,((avgh-0.0500-avg21)/sig21),((avgh+0.0500-avg21)/sig21));

		for (int j=0;j<1000;j++) {
			hpullf11pt[i]->SetBinContent(j+1,hf11pt[i]->GetBinContent(j+1));
			hpullf21pt[i]->SetBinContent(j+1,hf21pt[i]->GetBinContent(j+1));
		}
	}

	for (int i=0;i<5;i++) {
		TH1D* res11y = (TH1D*)facc_gaus_ptdep->Get(Form("hAccYCompBin%i",i+1));
		TH1D* res21y = (TH1D*)facc_gaus_ydep->Get(Form("hAccYCompBin%i",i+1));
		hf11y[i]=(TH1D*)res11y->Clone(Form("hf11y_%i",i+1));
		hf21y[i]=(TH1D*)res21y->Clone(Form("hf21y_%i",i+1));
		double avg11=hf11y[i]->GetMean();
		double sig11=hf11y[i]->GetRMS(); 
		double avg21=hf21y[i]->GetMean();
		double sig21=hf21y[i]->GetRMS(); 

		double avgh=avghy[i]; 
		std::cout << avg21 << " " << sig21 << std::endl;
		hpullf11y[i]=new TH1D(Form("hpullf11y_%i",i+1),Form("Pull distribution in (%1.3f#leqy_{lab}<%1.3f);pull;Entries",ybin[i],ybin[i+1]),1000,((avgh-0.0500-avg11)/sig11),((avgh+0.0500-avg11)/sig11));
		hpullf21y[i]=new TH1D(Form("hpullf21y_%i",i+1),Form("Pull distribution in (%1.3f#leqy_{lab}<%1.3f);pull;Entries",ybin[i],ybin[i+1]),1000,((avgh-0.0500-avg21)/sig21),((avgh+0.0500-avg21)/sig21));

		for (int j=0;j<1000;j++) {
			hpullf11y[i]->SetBinContent(j+1,hf11y[i]->GetBinContent(j+1));
			hpullf21y[i]->SetBinContent(j+1,hf21y[i]->GetBinContent(j+1));
		}
	}

	gStyle->SetOptStat(1110);

	for (int i=0;i<5;i++){
		double maxuser = std::max(hpullf11pt[i]->GetMaximum(),hpullf21pt[i]->GetMaximum());
		hpullf11pt[i]->GetYaxis()->SetRangeUser(0.0,1.2*maxuser);
		//hpullf11pt[i]->GetXaxis()->SetRangeUser(-10.0,10.0);
		hpullf11pt[i]->SetAxisRange(-10.0,10.0,"X");

		hpullf11pt[i]->SetLineColor(kRed+2);
		//hpullf12pt[i]->SetLineColor(kOrange+7);
		hpullf21pt[i]->SetLineColor(kBlue);
		//hpullf22pt[i]->SetLineColor(kTeal+3);
		hpullf11pt[i]->Draw();
		gPad->Update();
		TPaveStats* p11 = (TPaveStats*)hpullf11pt[i]->FindObject("stats");
		p11->SetX1NDC(0.70);
		p11->SetY1NDC(0.75);
		p11->SetX2NDC(0.90);
		p11->SetY2NDC(0.90);
		p11->SetTextColor(kRed+2);
		/*
		   hpullf12pt[i]->Draw();
		   gPad->Update();
		   TPaveStats* p12 = (TPaveStats*)hpullf12pt[i]->FindObject("stats");
		   p12->SetX1NDC(0.70);
		   p12->SetY1NDC(0.60);
		   p12->SetX2NDC(0.90);
		   p12->SetY2NDC(0.75);
		   p12->SetTextColor(kOrange+7);
		   */
		hpullf21pt[i]->Draw();
		gPad->Update();
		TPaveStats* p21 = (TPaveStats*)hpullf21pt[i]->FindObject("stats");
		p21->SetX1NDC(0.70);
		p21->SetY1NDC(0.45);
		p21->SetX2NDC(0.90);
		p21->SetY2NDC(0.60);
		p21->SetTextColor(kBlue);
		/*
		   hpullf22pt[i]->Draw();
		   gPad->Update();
		   TPaveStats* p22 = (TPaveStats*)hpullf22pt[i]->FindObject("stats");
		   p22->SetX1NDC(0.70);
		   p22->SetY1NDC(0.30);
		   p22->SetX2NDC(0.90);
		   p22->SetY2NDC(0.45);
		   p22->SetTextColor(kTeal+3);
		   */
		//TLegend* leg1 = new TLegend(0.12,0.7,0.4,0.88);
		TLegend* leg1 = new TLegend(0.15,0.80,0.48,0.93);


		leg1->AddEntry(hpullf11pt[i],"p_{T} variation with gaussian","l");
		leg1->AddEntry(hpullf21pt[i],"y variation with gaussian","l");
		leg1->SetFillColor(0);
		leg1->SetBorderSize(0);

		TCanvas* c1=new TCanvas("c1","");
		hpullf11pt[i]->Draw("");
		hpullf21pt[i]->Draw("same");
		p11->Draw("same");
		p21->Draw("same");
		leg1->Draw("same");

		c1->SaveAs(Form("../Results/AccSysGet_Bplus_ptdep_%i.pdf",i));
	}

	for (int i=0;i<5;i++){
		double maxuser = std::max(hpullf11y[i]->GetMaximum(),hpullf21y[i]->GetMaximum());
		hpullf11y[i]->GetYaxis()->SetRangeUser(0.0,1.2*maxuser);
		//hpullf11y[i]->GetYaxis()->SetRangeUser(0.0,5000.0);
		//hpullf11y[i]->GetXaxis()->SetRangeUser(-10.0,10.0);
		hpullf11y[i]->SetAxisRange(-10.0,10.0,"X");

		hpullf11y[i]->SetLineColor(kRed+2);
		hpullf21y[i]->SetLineColor(kBlue);
		hpullf11y[i]->Draw();
		gPad->Update();
		TPaveStats* p11 = (TPaveStats*)hpullf11y[i]->FindObject("stats");
		p11->SetX1NDC(0.70);
		p11->SetY1NDC(0.75);
		p11->SetX2NDC(0.90);
		p11->SetY2NDC(0.90);
		p11->SetTextColor(kRed+2);

		hpullf21y[i]->Draw();
		gPad->Update();
		TPaveStats* p21 = (TPaveStats*)hpullf21y[i]->FindObject("stats");
		p21->SetX1NDC(0.70);
		p21->SetY1NDC(0.45);
		p21->SetX2NDC(0.90);
		p21->SetY2NDC(0.60);
		p21->SetTextColor(kBlue);

		//TLegend* leg1 = new TLegend(0.12,0.7,0.4,0.88);
		TLegend* leg1 = new TLegend(0.15,0.80,0.48,0.93);

		leg1->AddEntry(hpullf11y[i],"p_{T} variation with gaussian","l");
		leg1->AddEntry(hpullf21y[i],"y variation with gaussian","l");
		leg1->SetFillColor(0);
		leg1->SetBorderSize(0);

		TCanvas* c1=new TCanvas("c1","");
		hpullf11y[i]->Draw("");
		hpullf21y[i]->Draw("same");
		p11->Draw("same");
		p21->Draw("same");
		leg1->Draw("same");
		c1->SaveAs(Form("../Results/AccSysGet_Bplus_ydep_%i.pdf",i));
	}

	for (int i=0;i<5;i++){

		GetMinMaxMean(hf11pt[i]);
		GetMinMaxMean(hf21pt[i]);

		double maxuser = std::max(hf11pt[i]->GetMaximum(),hf21pt[i]->GetMaximum());
		hf11pt[i]->GetYaxis()->SetRangeUser(0.0,1.2*maxuser);
		hf11pt[i]->GetXaxis()->SetRangeUser(hf11pt[i]->GetMean()-0.05,hf11pt[i]->GetMean()+0.05);
		hf11pt[i]->SetTitle("Acceptance variation");
		hf11pt[i]->GetXaxis()->SetTitle(Form("Acceptance in %i#leqp_{T}<%i (GeV/c)",ptbin[i],ptbin[i+1]));
		hf11pt[i]->GetYaxis()->SetTitle("Entries");

		hf11pt[i]->SetLineColor(kRed+2);
		hf21pt[i]->SetLineColor(kBlue);
		hf11pt[i]->Draw();
		gPad->Update();
		TPaveStats* p11 = (TPaveStats*)hf11pt[i]->FindObject("stats");
		p11->SetX1NDC(0.70);
		p11->SetY1NDC(0.75);
		p11->SetX2NDC(0.90);
		p11->SetY2NDC(0.90);
		p11->SetTextColor(kRed+2);

		hf21pt[i]->Draw();
		gPad->Update();
		TPaveStats* p21 = (TPaveStats*)hf21pt[i]->FindObject("stats");
		p21->SetX1NDC(0.70);
		p21->SetY1NDC(0.45);
		p21->SetX2NDC(0.90);
		p21->SetY2NDC(0.60);
		p21->SetTextColor(kBlue);

		//TLegend* leg1 = new TLegend(0.12,0.7,0.4,0.88);
		TLegend* leg1 = new TLegend(0.15,0.80,0.48,0.93);

		leg1->AddEntry(hf11pt[i],"p_{T} variation with gaussian","l");
		leg1->AddEntry(hf21pt[i],"y variation with gaussian","l");
		leg1->SetFillColor(0);
		leg1->SetBorderSize(0);

		TLine* ptline = new TLine(avghpt[i],0,avghpt[i],1.2*maxuser);

		TCanvas* c1=new TCanvas("c1","");
		hf11pt[i]->Draw("");
		hf21pt[i]->Draw("same");
		p11->Draw("same");
		p21->Draw("same");
		leg1->Draw("same");
		ptline->Draw("same");

		c1->SaveAs(Form("../Results/AccSysGet_Bplus_accvar_ptdep_%i.pdf",i));
	}

	for (int i=0;i<5;i++){
		double maxuser = std::max(hf11y[i]->GetMaximum(),hf21y[i]->GetMaximum());
		hf11y[i]->GetYaxis()->SetRangeUser(0.0,1.2*maxuser);
		hf11y[i]->GetXaxis()->SetRangeUser(hf11y[i]->GetMean()-0.05,hf11y[i]->GetMean()+0.05);
		hf11y[i]->SetTitle("Acceptance variation");
		hf11y[i]->GetXaxis()->SetTitle(Form("Acceptance in %1.3f#leqy_{lab}<%1.3f (GeV/c)",ybin[i],ybin[i+1]));
		hf11y[i]->GetYaxis()->SetTitle("Entries");

		hf11y[i]->SetLineColor(kRed+2);
		hf21y[i]->SetLineColor(kBlue);
		hf11y[i]->Draw();
		gPad->Update();
		TPaveStats* p11 = (TPaveStats*)hf11y[i]->FindObject("stats");
		p11->SetX1NDC(0.70);
		p11->SetY1NDC(0.75);
		p11->SetX2NDC(0.90);
		p11->SetY2NDC(0.90);
		p11->SetTextColor(kRed+2);

		hf21y[i]->Draw();
		gPad->Update();
		TPaveStats* p21 = (TPaveStats*)hf21y[i]->FindObject("stats");
		p21->SetX1NDC(0.70);
		p21->SetY1NDC(0.45);
		p21->SetX2NDC(0.90);
		p21->SetY2NDC(0.60);
		p21->SetTextColor(kBlue);

		//TLegend* leg1 = new TLegend(0.12,0.7,0.4,0.88);
		TLegend* leg1 = new TLegend(0.15,0.80,0.48,0.93);

		GetMinMaxMean(hf11y[i]);
		GetMinMaxMean(hf21y[i]);

		leg1->AddEntry(hf11y[i],"p_{T} variation with gaussian","l");
		leg1->AddEntry(hf21y[i],"y variation with gaussian","l");
		leg1->SetFillColor(0);
		leg1->SetBorderSize(0);

		TLine* yline = new TLine(avghy[i],0,avghy[i],1.2*maxuser);

		TCanvas* c1=new TCanvas("c1","");
		hf11y[i]->Draw("");
		hf21y[i]->Draw("same");
		p11->Draw("same");
		p21->Draw("same");
		leg1->Draw("same");
		yline->Draw("same");

		c1->SaveAs(Form("../Results/AccSysGet_Bplus_accvar_ydep_%i.pdf",i));
	}

	for(int i=0;i<tr11->GetEntries();i++){
		tr11->GetEntry(i);
		ha1gaus->Fill(a1_gaus);
		ha2gaus->Fill(a2_gaus);
	}
	for(int i=0;i<tr21->GetEntries();i++){
		tr21->GetEntry(i);
		hay1gaus->Fill(ay1_gaus);
		hay2gaus->Fill(ay2_gaus);
	}

	double maxusera1, maxusera2, maxuseray1, maxuseray2;
	maxusera1 = std::max(ha1gaus->GetMaximum(),ha1gaus->GetMaximum());
	ha1gaus->GetYaxis()->SetRangeUser(0.0,1.2*maxusera1);
	maxusera2 = std::max(ha2gaus->GetMaximum(),ha2gaus->GetMaximum());
	ha2gaus->GetYaxis()->SetRangeUser(0.0,1.2*maxusera2);
	maxuseray1 = std::max(hay1gaus->GetMaximum(),hay1gaus->GetMaximum());
	hay1gaus->GetYaxis()->SetRangeUser(0.0,1.2*maxuseray1);
	maxuseray2 = std::max(hay2gaus->GetMaximum(),hay2gaus->GetMaximum());
	hay2gaus->GetYaxis()->SetRangeUser(0.0,1.2*maxuseray2);

	ha1gaus->Draw();
	gPad->Update();
	TPaveStats* pa1g = (TPaveStats*)ha1gaus->FindObject("stats");
	pa1g->SetX1NDC(0.70);
	pa1g->SetY1NDC(0.75);
	pa1g->SetX2NDC(0.90);
	pa1g->SetY2NDC(0.90);
	pa1g->SetTextColor(kRed+2);

	TCanvas* c1=new TCanvas("c1","");

	TLegend* leg1 = new TLegend(0.15,0.80,0.48,0.93);
	leg1->AddEntry(ha1gaus,"p_{T} variation with gaussian","l");
	leg1->SetFillColor(0);
	leg1->SetBorderSize(0);

	ha1gaus->SetLineColor(kRed+2);
	ha1gaus->Draw("");
	leg1->Draw("same");
	c1->SaveAs("../Results/AccSysGet_Bplus_ha1comp.pdf");

	leg1->Clear();
	leg1->AddEntry(ha2gaus,"p_{T} variation with gaussian","l");

	ha2gaus->Draw();
	gPad->Update();
	TPaveStats* pa2g = (TPaveStats*)ha2gaus->FindObject("stats");
	pa2g->SetX1NDC(0.70);
	pa2g->SetY1NDC(0.75);
	pa2g->SetX2NDC(0.90);
	pa2g->SetY2NDC(0.90);
	pa2g->SetTextColor(kRed+2);

	ha2gaus->SetLineColor(kRed+2);
	ha2gaus->Draw("");
	leg1->Draw("same");
	c1->SaveAs("../Results/AccSysGet_Bplus_ha2comp.pdf");

	leg1->Clear();
	leg1->AddEntry(hay1gaus,"y variation with gaussian","l");

	hay1gaus->Draw();

	gPad->Update();
	TPaveStats* pay1g = (TPaveStats*)hay1gaus->FindObject("stats");
	pay1g->SetX1NDC(0.70);
	pay1g->SetY1NDC(0.75);
	pay1g->SetX2NDC(0.90);
	pay1g->SetY2NDC(0.90);
	pay1g->SetTextColor(kBlue);

	hay1gaus->SetLineColor(kBlue);
	hay1gaus->Draw("");
	leg1->Draw("same");
	c1->SaveAs("../Results/AccSysGet_Bplus_hay1comp.pdf");

	leg1->Clear();
	leg1->AddEntry(hay2gaus,"y variation with gaussian","l");

	hay2gaus->Draw();

	gPad->Update();
	TPaveStats* pay2g = (TPaveStats*)hay2gaus->FindObject("stats");
	pay2g->SetX1NDC(0.70);
	pay2g->SetY1NDC(0.75);
	pay2g->SetX2NDC(0.90);
	pay2g->SetY2NDC(0.90);
	pay2g->SetTextColor(kBlue);

	hay2gaus->SetLineColor(kBlue);
	hay2gaus->Draw("");
	leg1->Draw("same");
	c1->SaveAs("../Results/AccSysGet_Bplus_hay2comp.pdf");

	//////////////////////
	leg1->Clear();
	leg1->AddEntry(htest11,"p_{T} variation with gaussian","l");
	maxusera1 = std::max(htest11->GetMaximum(),htest11->GetMaximum());
	htest11->GetYaxis()->SetRangeUser(0.0,1.2*maxusera1);
	maxuseray1 = std::max(htest21->GetMaximum(),htest21->GetMaximum());
	htest21->GetYaxis()->SetRangeUser(0.0,1.2*maxuseray1);
	htest11->Draw();

	gPad->Update();
	TPaveStats* pht11 = (TPaveStats*)htest11->FindObject("stats");
	pht11->SetX1NDC(0.70);
	pht11->SetY1NDC(0.75);
	pht11->SetX2NDC(0.90);
	pht11->SetY2NDC(0.90);
	pht11->SetTextColor(kRed+2);

	htest11->SetLineColor(kRed+2);
	htest11->Draw("");
	leg1->Draw("same");
	c1->SaveAs("../Results/AccSysGet_Bplus_ratioptbin1.pdf");

	leg1->Clear();
	leg1->AddEntry(htest21,"y variation with gaussian","l");

	htest21->Draw();

	gPad->Update();
	TPaveStats* pht21 = (TPaveStats*)htest21->FindObject("stats");
	pht21->SetX1NDC(0.70);
	pht21->SetY1NDC(0.75);
	pht21->SetX2NDC(0.90);
	pht21->SetY2NDC(0.90);
	pht21->SetTextColor(kBlue);

	htest21->SetLineColor(kBlue);
	htest21->Draw("");
	leg1->Draw("same");
	c1->SaveAs("../Results/AccSysGet_Bplus_ratioybin1.pdf");

	fout->Write();
	//fout->Close();

}

void GetMinMaxMean(TH1D* hs){
	int hsbin=hs->GetNbinsX();
	bool isFillMin=false;
	bool isFillMax=false;
	double MinVal,MaxVal;
	for (int k=1;k<hsbin+1;k++){
		if (!isFillMin && (hs->GetBinContent(k))!=0) {isFillMin=true;MinVal=hs->GetBinLowEdge(k);}
		if (k<hsbin && hs->GetBinContent(k)!=0 && (hs->GetBinContent(k+1)==0 || isFillMax)) {isFillMax=true;MaxVal=hs->GetBinLowEdge(k)+hs->GetBinWidth(k);}    
	}
	std::cout << "MinVal : " << MinVal << " , Mean : " << hs->GetMean() << " , MaxVal : " << MaxVal << std::endl;
}

