/*
 * OLPBench.cpp
 *
 *  Created on: 9 oct. 2014
 *      Author: dosimont
 */

#include "OLPBench.h"

OLPBench::OLPBench():threshold(0.1),iteration(1),olp(0) {
	srand (time(NULL));
}

const vector<double>& OLPBench::getDensity() const {
	return density;
}

const vector<int>& OLPBench::getDimension1() const {
	return dimension1;
}

const vector<int>& OLPBench::getDimension2() const {
	return dimension2;
}

const vector<int>& OLPBench::getDimension3() const {
	return dimension3;
}

const vector<double>& OLPBench::getP() const {
	return p;
}

void OLPBench::addDimension1(vector<string> tab) {
	addLineInteger(tab, dimension1);
}

void OLPBench::addDimension2(vector<string> tab) {
	addLineInteger(tab, dimension2);
}

void OLPBench::addDimension3(vector<string> tab) {
	addLineInteger(tab, dimension3);
}

void OLPBench::addThreshold(vector<string> tab) {
	if (tab.size()>0){
		threshold=atof(tab[0].c_str());
	}
}

void OLPBench::addP(vector<string> tab) {
	addLineDouble(tab, p);
}

void OLPBench::addDensity(vector<string> tab) {
	addLineDouble(tab, density);
}

OLPBench::~OLPBench() {
}

void OLPBench::addLineInteger(vector<string> tab, vector<int> allocator) {
	if (tab.size()==3&&(atoi((tab[1]).c_str())>atoi((tab[2]).c_str()))){
		for (int i=(atoi((tab[0]).c_str())); i<(atoi((tab[1]).c_str())); i+=(atoi((tab[2]).c_str())))
		allocator.push_back(i);
	}else{
		for (unsigned int i=0; i<tab.size(); i++){
			allocator.push_back(atoi((tab[i]).c_str()));
		}
	}
}

int OLPBench::getIteration() const {
	return iteration;
}

void OLPBench::addIteration(vector<string> tab) {
	if (tab.size()>0){
		iteration=atoi((tab[0]).c_str());
	}
}

void OLPBench::addLineDouble(vector<string> tab, vector<double> allocator) {
	if (tab.size()==3&&(atof((tab[1]).c_str())>atof((tab[2]).c_str()))){
		for (unsigned int i=(atof((tab[0]).c_str())); i<(atof((tab[1]).c_str())); i+=(atof((tab[2]).c_str())))
		allocator.push_back(i);
	}else{
		for (unsigned int i=0; i<tab.size(); i++){
			if (tab[i].compare("rand")){
				allocator.push_back(-1);
			}else{
				allocator.push_back(atof((tab[i]).c_str()));
			}
		}
	}
}


void OLPBench::addLineFloat(vector<string> tab, vector<float> allocator) {
	if (tab.size()==3&&(atof((tab[1]).c_str())>atof((tab[2]).c_str()))){
		for (unsigned int i=(atof((tab[0]).c_str())); i<(atof((tab[1]).c_str())); i+=(atof((tab[2]).c_str())))
		allocator.push_back(i);
	}else{
		for (unsigned int i=0; i<tab.size(); i++){
			if (tab[i].compare("rand")){
				allocator.push_back(-1);
			}else{
				allocator.push_back(atof((tab[i]).c_str()));
			}
		}
	}
}


void OLPBench::launchBench(ostream *output) {
	int num=0;
	int cdimension1=1;
	int cdimension2=1;
	int cdimension3=1;
	double cdensity=1;
	double cp=1;
	printHeader(output);
	for (unsigned int i=0; i<dimension1.size();i++){
		cdimension1=dimension1[i];
		for (unsigned int j=0; j<dimension2.size();j++){
			cdimension2=dimension2[j];
			for (unsigned int k=0; j<dimension3.size();k++){
				cdimension3=dimension3[k];
				for (unsigned int l=0; l<density.size(); l++){
					cdensity=density[l];
					for (unsigned int m=0; m<p.size(); m++){
						cp=p[m];
						for (int n=0; n<iteration; n++){
							*output<< (++num) <<", "<< cdimension1 <<", "<< cdimension2 <<", "+ cdimension3 <<", "<< threshold;
							if (cp==-1){
								*output<<", "<< "rand";
							}else{
								*output<<", " << cp;
							}
							*output<<", " << cdensity;
							generateMatrix(i,j,k,l);
							launchTest(output,cp);
					}
				}
				}
			}
		}

	}
}

int generateValue(double density){
	if ((rand()/RAND_MAX)<density){
		return rand();
	}
	else{
		return 0;
	}
}

void OLPBench::generateMatrix(int i, int j, int k, double density) {
	olp = new OLPAggregWrapper(3);
	for (int it=0; it<i; it++){
		olp->addMatrix();
		for (int jt=0; jt<j; jt++){
			olp->addVector();
			for (int kt=0; kt<k; kt++){
				olp->push_back(generateValue(density));
			}
		}
	}
}

void OLPBench::launchTest(ostream *output, double p) {
	olp->computeQualities(false);
	olp->computeDichotomy(threshold);
	olp->computeParts(p);
	*output << ", " << 0 << ", " << 0 << ", " << 0 << ", " << 0 << ", " << 0 << ", " << 0 << ", " << 0 << ", " << 0 << ", " << olp->getParameterNumber()<<endl;
}

void printHeader(ostream *output){
	*output << "N, DIM1, DIM2, DIM3, DICHO, P, DENSITY, COUNTER Q, COUNTER DICHO, COUNTER BC, COUNTER BP, TIME Q, TIME DICHO, TIME BC, TIME BP, P RETRIEVED" <<endl;
}
