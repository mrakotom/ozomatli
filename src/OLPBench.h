/*
 * OLPBench.h
 *
 *  Created on: 9 oct. 2014
 *      Author: dosimont
 */

#ifndef OLPBENCH_H_
#define OLPBENCH_H_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <time.h>
#include <benchlpaggreg/OLPAggregWrapper.h>

using namespace std;

class OLPBench {
	private:
		vector<int> dimension1;
		vector<int> dimension2;
		vector<int> dimension3;
		double threshold;
		vector<double> p;
		vector<double> density;
		int iteration;
		OLPAggregWrapper * olp;

	public:
		OLPBench();
		virtual ~OLPBench();
		void addDimension1(vector<string> tab);
		void addDimension2(vector<string> tab);
		void addDimension3(vector<string> tab);
		void addIteration(vector<string> tab);
		void addThreshold(vector<string> tab);
		void addP(vector<string> tab);
		void addDensity(vector<string> tab);
		const vector<double>& getDensity() const;
		const vector<int>& getDimension1() const;
		const vector<int>& getDimension2() const;
		const vector<int>& getDimension3() const;
		const vector<double>& getP() const;
		int getIteration() const;
		void launchBench(ostream *output);
	private:
		void addLineInteger(vector<string> tab, vector<int>);
		void addLineDouble(vector<string> tab, vector<double>);
		void addLineFloat(vector<string> tab, vector<float>);
		void generateMatrix(int i, int j, int k, double density);
		void launchTest(ostream *output, double p);

};

int generateValue(double density);
void printHeader(ostream *output);


#endif /* OLPBENCH_H_ */
