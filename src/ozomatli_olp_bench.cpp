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
	if (op.compare("OLPAggreg1")==0||op.compare("OLPAggreg2")==0||op.compare("OLPAggreg3")==0){
		return true;
	}
	return false;
}

void printOLP1Aggregation(vector <int> data, vector<int> parts, ofstream * output){
	int count=1;
	int sum=data[0];
	for (unsigned int i=1; i<parts.size()+1; i++){
		if ((i==parts.size())||(parts[i]!=parts[i-1])){
			for (int j=0; j<((count/2)-1+(count%2)); j++){
				*output << "- ";
			}
			*output << sum;
			for (int j=0; j<(count/2); j++){
				*output << " -";
			}
			if (i!=parts.size()){
			*output << " | ";
			}
			count=1;
			sum=data[i];
		}else{
			count++;
			sum+=data[i];
		}
	}
	*output<<endl;
}

void printOLP2Aggregation(vector <vector <int> > data, vector<int> parts, ofstream * output){
	for (unsigned int i=0; i<data.size(); i++){
		printOLP1Aggregation(data[i], parts, output);
	}
}

void printOLP3Aggregation(vector <vector <int> > data, vector<int> parts, vector<int> size, ofstream * output){
	vector <vector <int > > tmp;
	vector <vector <int > > sum;
	for (int j=0; j<size[2]; j++){
		sum.push_back(vector<int>());
		for (int i=0; i<size[0]; i++){
			sum[j].push_back(0);
		}
	}
	for (int k=0; k<size[2]; k++){
		tmp.clear();
		for (int j=0; j<size[1]; j++){
			tmp.push_back(data[k*size[1]+j]);
		}
		printOLP2Aggregation(tmp, parts, output);
	}
	for (int i=0; i<size[0]; i++){
		for (int k=0; k<size[2]; k++){
				for (int j=0; j<size[1]; j++){
				sum[k][i]+=data[j*size[2]+k][i];
			}
		}
	}
	*output<<"#Sum"<<endl;
	printOLP2Aggregation(sum, parts, output);
}




void execOLPOperator(string op, vector<int> size, vector<vector <int> > data, ofstream *output){
	if (op.compare("OLPAggreg1")==0||op.compare("OLPAggreg2")==0||op.compare("OLPAggreg3")==0){
		OLPAggregWrapper manager =OLPAggregWrapper(size.size());
	    *output<<"#Operator: " <<op<<endl;
		if (op.compare("OLPAggreg1")==0){
			*output<<"#Size: "<<size[0]<<endl;
			*output<<endl<<"#Values: "<<endl;
			int i;
			for (i=0; i<size[0]-1; i++){
				manager.push_back(data[0][i]);
				*output<<data[0][i]<<" | ";
			}
			manager.push_back(data[0][i]);
			*output<<data[0][i];
			*output<<endl;
		}else if (op.compare("OLPAggreg2")==0){
			*output<<"#Size: "<<size[0]<<"X"<<size[1]<<endl;
			*output<<endl<<"#Values: "<<endl;
			int i;
			for (int j=0; j<size[1]; j++){
				for (i=0; i<size[0]-1; i++){
				*output<<data[j][i]<<" | ";
				}
				*output<<data[j][i];
				*output<<endl;
			}
			for (i=0; i<size[0]; i++){
				manager.addVector();
				for (int j=0; j<size[1]; j++){
					manager.push_back(data[j][i]);
				}
				}
		}else if (op.compare("OLPAggreg3")==0){
			*output<<"#Size: "<<size[0]<<"X"<<size[1]<<"X"<<size[2]<<endl;
				*output<<endl<<"#Values: "<<endl;
				int i;
				for (int k=0; k<size[2]; k++){
				for (int j=0; j<size[1]; j++){
					for (i=0; i<size[0]-1; i++){
					*output<<data[k*size[1]+j][i]<<" | ";
					}
				*output<<data[k*size[1]+j][i];
				*output<<endl;
				}
			}
				for (i=0; i<size[0]; i++){
					manager.addMatrix();
					for (int j=0; j<size[1]; j++){
						manager.addVector();
						for (int k=0; k<size[2]; k++){
						manager.push_back(data[j*size[2]+k][i]);
						}
					}
				}
		}
		manager.computeQualities(false);
		manager.computeDichotomy(0.0001);
		*output<<endl<<"#Aggregations"<<endl;
		for (int i=0; i<manager.getParameterNumber(); i++){
			*output<<endl<<"#-------------"<<endl;
			float p=manager.getParameter(i);
			*output<<"#Parameter: "<<p<<endl;
			*output<<"#Gain: "<<manager.getGainByIndex(i)<<"; Loss: "<<manager.getLossByIndex(i)<<endl;
			manager.computeParts(p);
			vector<int> res;
			for (int j=0; j<manager.getPartNumber(); j++){
				res.push_back(manager.getPart(j));
			}
			*output<<"#Aggregated Values:"<<endl;
			if (op.compare("OLPAggreg1")==0){
			printOLP1Aggregation(data[0], res, output);
			}
			else if (op.compare("OLPAggreg2")==0){
			printOLP2Aggregation(data, res, output);
			}
			else if (op.compare("OLPAggreg3")==0){
			printOLP3Aggregation(data, res, size, output);
			}
		}
	}
}

int main(int argc, const char* argv[]) {
	if (argc<2){
		cerr<<"Error: no input file provided"<<endl;
		return 1;
	}
	if (argc<3){
		cerr<<"Error: no output file found"<<endl;
		return 1;
	}
    ifstream input(argv[1]);
    ofstream *output=new ofstream(argv[2]);
    bool v_op, v_size;
    v_op = false;
    v_size = false;
    string op;
    vector<int> size;
    vector<vector<int> > data;
    //Iterator for parsing csv file. ',' is used as separator between each field.
    CSVIterator loop(input, ',');
    //operator
    for(; loop != CSVIterator(); ++loop) {
    	//Skip comments
    	if ((*loop)[0][0]=='#'){
    	}
    	else if (!v_op){
    		op=(*loop)[0];
    	    if (!validateOperator(op)){
    			cerr<<"Error: invalid aggregation operator "<<op<<endl;
    			return 2;
    	    }
    		v_op=true;
    	}else if (!v_size){
    		for (unsigned int i=0; i<(*loop).size(); i++){
    			size.push_back(atoi(((*loop)[i]).c_str()));
    		}
    	v_size=true;
    	}else{
    		data.push_back(vector<int>());
    		for (unsigned int i=0; i<(*loop).size(); i++){
    			(data[data.size()-1]).push_back(atoi(((*loop)[i]).c_str()));
    		}
    	}
    }
    *output<<"#Ozomatli: "<<argv[1]<< " output file"<<endl;
    execOLPOperator(op, size, data, output);

    delete output;

    return 0;
}
