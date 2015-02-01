#!/bin/sh

cd Code
root -l -b -q FakeInput_Bplus.C+
root -l -b -q FakeInput_Bzero.C+
root -l -b -q 'RunMCtoy_gaus_allB_pt.C+(0,100)'
root -l -b -q 'RunMCtoy_gaus_allB_pt.C+(1,100)'
