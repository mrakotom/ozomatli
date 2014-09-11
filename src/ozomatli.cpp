//============================================================================
// Name        : ozomatli.cpp
// Author      : Damien Dosimont
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <csvreader/CSVIterator.h>
#include <lpaggreg/OLPAggregWrapper.h>
using namespace std;

bool validateOperator(string op){
	if (op.compare("OLPAggreg1")==0){
		return true;
	}
	return false;
}

void printOLP1Aggregation(vector <int> data, vector<int> parts){
	cout<<"#Aggregated Values:"<<endl;
	int count=1;
	int sum=data[1];
	for (unsigned int i=1; i<parts.size(); i++){
		if (parts[i]!=parts[i-1]){
			for (int j=0; j<(count/2-1); j++){
				cout << "- ";
			}
			cout << sum<< " ";
			for (int j=0; j<(count/2); j++){
				cout << "- ";
			}
			count=1;
			sum=parts[i];
		}else{
			count++;
			sum+=parts[i];
		}
	}
}

void execOLPOperator(string op, vector<int> size, vector<vector <int> > data, ofstream *output){
	if (op.compare("OLPAggreg1")==0||op.compare("OLPAggreg2")==0||op.compare("OLPAggreg3")==0){
		OLPAggregWrapper manager = OLPAggregWrapper(size.size());
	    *output<<"#Operator: " <<op<<endl;
		if (op.compare("OLPAggreg1")==0){
			*output<<"#Size: "<<size[0]<<endl;
			*output<<"#Values: "<<size[0]<<endl;
			int i;
			for (i=0; i<size[0]-1; i++){
				manager.push_back(data[0][i]);
				*output<<data[0][i]<<",";
			}
			manager.push_back(data[0][i]);
			*output<<data[0][i];
			*output<<endl;
		}
		manager.computeQualities(false);
		manager.computeDichotomy(0.0001);
		*output<<"#Aggregations"<<endl;
		for (int i=0; i<manager.getParameterNumber(); i++){
			*output<<"-------------"<<endl;
			float p=manager.getParameter(i);
			*output<<"#Parameter: "<<p<<endl;
			*output<<"#Gain: "<<manager.getGainByIndex(i)<<"; Loss: "<<manager.getLossByIndex(i)<<endl;
			manager.computeParts(p);
			vector<int> res;
			for (int j=0; j<manager.getPartNumber(); j++){
				res.push_back(manager.getPart(j));
			}
			printOLP1Aggregation(data[0], res);
		}
	}
}

int main(int argc, const char* argv[]) {
	if (argc<2){
		cerr<<"Error: no trace file provided"<<endl;
		return 1;
	}
	if (argc<3){
		cerr<<"Error: no output file found"<<endl;
		return 1;
	}
    ifstream input(argv[1]);
    ofstream *output = new ofstream(argv[2]);
    string op;
    vector<int> size;
    vector<vector<int> > data;

    //Iterator for parsing csv file. ',' is used as separator between each field.
    CSVIterator loop(input, ',');
    //operator
    for(; loop != CSVIterator(); ++loop) {
    	//Skip comments
    	if ((*loop)[1][0]=='#'){
    	}
    	else{
    		op=(*loop)[1];
    		break;
    	}
    }
    if (!validateOperator(op)){
		cerr<<"Error: invalid aggregation operator"<<endl;
		return 2;
    }
    //size
    for(; loop != CSVIterator(); ++loop) {
    	//Skip comments
    	if ((*loop)[1][0]=='#'){
    	}
    	else{
    		for (unsigned int i=0; i<(*loop).size(); i++){
    			size.push_back(atoi(((*loop)[i]).c_str()));
    		}
    		break;
    	}
    }
    //data
    for(; loop != CSVIterator(); ++loop) {
    	//Skip comments
    	if ((*loop)[1][0]=='#'){
    	}
    	else{
    		data.push_back(vector <int>());
    		for (unsigned int i=0; i<(*loop).size(); i++){
    			data[data.size()].push_back(atoi(((*loop)[i]).c_str()));
    		}
    		break;
    	}
    }
    *output<<"#Ozomatli: "<<argv[1]<< " output file"<<endl;
    execOLPOperator(op, size, data, output);




}