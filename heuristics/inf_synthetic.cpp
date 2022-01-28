#include "other_function.h"
#include "heuristic.h"
#include "syds.h"
#include<algorithm>
#include<iostream>
#include<cmath>
#include<cstdlib>
#include<unordered_map>
#include<string>
#include<vector>
#include<fstream>

using namespace std;

int main(int argc, char *argv[])
{   
    //--------------------------------
    //-    Command line arguments    -
    //--------------------------------
    int heur_type = stoi(argv[1]);
    string network_type = argv[2]; 
    string theta = argv[3]; 
    
    //-----------------------------------------------------------------
    //    Construct the name of the network file and threshold file   -
    //-----------------------------------------------------------------
    string network_file;
    string thresh_file;
    if(network_type == "gnp")
    {
        network_file = "../networks/random/gnp/gnp_d_100_theta_" + theta + ".edges";
        thresh_file = "../networks/random/gnp/gnp_d_100_theta_" + theta + "_threshold.txt";
    }

    else if(network_type == "power_law")
    {
        network_file = "../networks/random/power_law/pl_d_100_theta_" + theta + ".edges";
        thresh_file = "../networks/random/power_law/pl_d_100_theta_" + theta + "_threshold.txt";
    }

    else
    {
        cerr<<"unknown network type"<<endl;
        return -1;
    }

    Syds S;
    S = construct_network(network_file, thresh_file);

    Syds inferred_S;
                
    if(heur_type == 1) // Linear search
    {
        inferred_S = adaptive_linear_search_inference(S.neighbors, S.threshold);
    }
    else if(heur_type == 2) // Binary search
    {
        inferred_S = adaptive_binary_search_inference(S.neighbors, S.threshold);
    }
    else
    {
        cerr<<"unknown herustic type"<<endl;
        return -1;
    }

    int n = S.threshold.size();

    // Check if all thresholds and neighbors are correctly inferred
    bool thresh_correct = 1;
    bool neigh_correct = 1;
    
    int wrong_count = 0;
    for(int u = 0; u < n; ++u)
    {
        if(S.threshold[u] != inferred_S.threshold[u])
        {
            thresh_correct = 0;
            wrong_count++;
            cout<<(S.threshold[u])<<" : "<<(inferred_S.threshold[u])<<endl;
        }

        for(auto v : inferred_S.neighbors[u])
        {
            if(find(S.neighbors[u].begin(), S.neighbors[u].end(), v) == S.neighbors[u].end()) 
            {
                neigh_correct = 0;
            }
            
        }

        for(auto v : S.neighbors[u])
        {
            if(find(inferred_S.neighbors[u].begin(), inferred_S.neighbors[u].end(), v) == inferred_S.neighbors[u].end())
            {
                neigh_correct = 0;
            }
        }
    }

    if(thresh_correct) cout<<"All thresholds are correctly inferred"<<endl;
    else cout<<"Some thresholds are not inferred correctly"<<endl;

    if(neigh_correct) cout<<"All neighbors are correctly inferred"<<endl;
    else cout<<"Some neighbors are not inferred correctly"<<endl;

    return 0;
}
