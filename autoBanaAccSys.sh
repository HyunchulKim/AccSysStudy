#!/bin/sh

cd Code
root -l -b -q FakeInput_Bplus.C+
root -l -b -q FakeInput_Bzero.C+
root -l -b -q 'RunMCtoy_gaus_allB_pt.C+(0,100)'
root -l -b -q 'RunMCtoy_gaus_allB_pt.C+(1,100)'
#root -l -b -q 'OniaAccAnalyzer_20150204.C+(int optionBinning = 1, int optionA = 1, int optionB = 1, double varmag = 0.0, std::string stringA = "PtYbinBp", bool isBoosted = true, int OniaCode = 521, int Kpdau0Code = -999, int Kpdau1Code = -999, int KpCode = 321, std::string remarkstring = "BanaAcc_IDfilters_intot100M_20150204_default", double dimuptbinmin=0.0, double dimuptbinmax=100.0, double rapbinmin=-2.4, double rapbinmax=2.4, double Bptbinmin=10.0, double Bptbinmax=60.0, double Brapbinmin=-2.4, double Brapbinmax=2.4, int BCode=521, double trkptcut=0.9)'

root -l -b -q 'OniaAccAnalyzer_20150204.C+(1, 1, 1, 0.0, "PtYbinBp", true, 521, -999, -999, 321, "BanaAcc_IDfilters_intot100M_20150204_default", 0.0, 100.0, -2.4, 2.4, 10.0, 60.0, -2.4, 2.4, 521, 0.9)'

root -l -b -q 'OniaAccAnalyzer_20150204.C+(1, 1, 100, 0.0, "PtYbinBp", true, 521, -999, -999, 321, "BanaAcc_IDfilters_intot100M_20150204", 0.0, 100.0, -2.4, 2.4, 10.0, 60.0, -2.4, 2.4, 521, 0.9)'

root -l -b -q 'OniaAccAnalyzer_20150204.C+(1, 1, 200, 0.0, "PtYbinBp", true, 521, -999, -999, 321, "BanaAcc_IDfilters_intot100M_20150204", 0.0, 100.0, -2.4, 2.4, 10.0, 60.0, -2.4, 2.4, 521, 0.9)'

