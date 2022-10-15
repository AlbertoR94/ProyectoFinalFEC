#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <vector>

using namespace std;

double sign(double);
int coin(double p);
int random_spin(double p);
void initial_config(vector<int>&, double p, int Nv);
double calculate_Magnetization(vector<int>&, int Nv);
int random_node(int val);
double compute_majority(vector<vector<int>>&, vector<int>&, vector<int>&, int node, double alpha);
void flip_spin(vector<vector<int>>&, vector<int>&, vector<int>&, double& magnetization, double alpha, double q, int Nv);

int main(int argc, char **argv){
	srand(time(NULL));
	
	int Nv = atoi(argv[1]);
	
	double alpha = atof(argv[2]);
	double q = atof(argv[3]);
	double p = atof(argv[4]);
	int step = atoi(argv[5]);
	double m = 1.0*Nv;
	int a,b;
	
	string str;
	string edgesfile = argv[6];
	string filename = "Mag_SF" + to_string(Nv) + "_"+ to_string(step) +".dat";
	
	vector<int> opinion(Nv);
	vector<int> degrees(Nv);
	vector<vector<int>> neighbors(Nv);
	
	double Magnetization;
	int Nsteps = 20000;
	
	ofstream Variables (filename, ios::app);
	ifstream file (edgesfile);
	
	while(getline(file, str)){

		a = stoi(str.substr(0, str.find(" ")));
		b = stoi(str.substr(str.find(" ") + 1));
		neighbors[a].push_back(b);
		neighbors[b].push_back(a);
	}
	
	for(int i=0; i<Nv; i++){
		degrees[i] = neighbors[i].size();
	}
	
	initial_config(opinion, p, Nv);
	//for(int i=0; i<Nv; i++){
	//	cout << opinion[i] << endl; 
	//}
	
	Magnetization = calculate_Magnetization(opinion, Nv);
	cout << Magnetization << endl;
	
	for(int step=0; step<Nsteps; step++){
		for(int i=0; i<Nv; i++){
			flip_spin(neighbors, opinion, degrees, Magnetization, alpha, q, Nv);
			}
		Variables << Magnetization/m << endl;
		}
	Variables.close();
	return 0;
}

double sign(double x) {
  if (x > 0) return 1.;
  if (x < 0) return -1.;
  return x;
}

int coin(double p){
	double val  = 1.0*rand()/RAND_MAX;
	int out; 
	if (val<=p){
	 	out=1;
		}
	else{
		out=0;
		}
	return out; 
	}
	
int random_spin(double p){
	double val  = 1.0*rand()/RAND_MAX;
	int out; 
	if (val<=p){
	 	out=1;
		}
	else{
		out=-1;
		}
	return out; 
	}
	
void initial_config(vector<int>& opinion, double p, int Nv){
	for(int i=0; i<Nv; i++){
			opinion[i] = random_spin(p);
			}
	}
	
double calculate_Magnetization(vector<int>& opinion, int Nv){
	double M = 0;
	for(int i=0; i<Nv; i++)
			M += opinion[i];			
	return M;
}

int random_node(int val){
	return rand()%(val);
}

double compute_majority(vector<vector<int>>& neighbors, vector<int>& opinion, vector<int>& degrees, int node, double alpha){
	int neighbor;
	double S = 0;
	for(int i=0; i<degrees[node]; i++){
		neighbor = neighbors[node][i];
		S += opinion[neighbor]*(1 + (degrees[neighbor]-1)*alpha);
	} 
	return S;
}

void flip_spin(vector<vector<int>>& neighbors, vector<int>& opinion, vector<int>& degrees, double& magnetization, double alpha, double q, int Nv){
	int node;
	int S;
	double deltaM;
	double flip;
	double w;
	
	node = random_node(Nv);
	S = compute_majority(neighbors, opinion, degrees, node, alpha);
	w = 0.5*(1-(1-2*q)*opinion[node]*sign(S));
	
	flip = coin(w);
	
	if(flip){
		deltaM= -2*opinion[node];
		opinion[node]*=-1;
		magnetization += deltaM; 
	}
	
	}

