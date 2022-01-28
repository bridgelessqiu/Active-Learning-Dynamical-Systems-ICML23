#include "algo.h"
#include<list>
#include<queue>
#include<unordered_map>
#include<cstdlib>
#include<set>
#include<random>
#include<cmath>
#include<numeric>
#include<algorithm>
#include<vector>
#include<iostream>

using namespace std;

// -------------------------------------------------------- //
//        Binary-searched based inference algorithm         //
// -------------------------------------------------------- //

Syds adaptive_binary_search_inference(const vector<vector<int>>& neighbors, const vector<int>& threshold)
{
    random_device rd;
    mt19937 g(rd());

    int n = neighbors.size();
    int num_of_cp = 0; // The number of critical points

    vector<int> vertex_order(n);
    iota(vertex_order.begin(), vertex_order.end(), 0); //[0, 1, ..., n-1]

    shuffle(vertex_order.begin(), vertex_order.end(), g);

    int total_queries = 0;

    int node_count = 0; // the number of vertices whose critical vertex have been found

    vector<bool> found (n, 0); // found[v] = 1 iff the critical node of v has been found

    vector<vector<int>> critical_vertex(n); // critical_vertex[v] is the list of vertices whose critical vertex is v

    vector<int> q_1 (n, 0); // the first query

    int pointer = 0; // the index of the pointer

    vector<vector<int>> inferred_neighbors(n);

    vector<int> inferred_threshold(n, 1); // assume that there are no constant node, and the baseline value is 1 to account for the critical node

    // Find the critical node of each vertex
    while(node_count < n)
    {
        int current_v = vertex_order[pointer];
        q_1[current_v] = 1;

        bool is_critical = false;

        for(int u = 0; u != n; ++u)
        {
            if(!found[u])
            {
                int input = 0;
                for(auto v : neighbors[u]) input += q_1[v];

                if(input >= threshold[u])
                {
                    is_critical = true;
                    found[u] = 1;
                    critical_vertex[current_v].push_back(u);
                    node_count++;
                    inferred_neighbors[u].push_back(current_v); // THERE IS NO NEED TO DO inferred_neighbors[current_v].push_back(u);
                }
            }
        }
        if(is_critical)
        {
            num_of_cp ++;
        }

        pointer++;
        total_queries++;
    }

    // Now we have critical_vertex[]
    for(int u = 0; u < n; ++u)
    {
        // u is the index of the critial node
        int current_v = vertex_order[u];

        if(!critical_vertex[current_v].empty()) // current_v is the critical vertex of some vertex
        {
            //num_of_cp++; // increase the counter for the number of cirtical vertices

            vector<int> q_2(n, 0);
            for(int i = 0; i <= u; ++i) q_2[vertex_order[i]] = 1; //everything up to current_v set to 1
            vector<int> q_3 = q_2;

            // find neighbors before the critiacl node
            bool nei_exists = true; // There are still undiscovered edges

            int low = 0; //low
            int high = u - 1; //high, we don't inlucde current_v because it is a neighor of all associated vetices
            if(u == 0) nei_exists = false; //the first vertex is the critical node, nothing to search to the left
            while(nei_exists)
            {
                total_queries++;

                if(low == high && high != (u-1)) // 1 2 3 4* 5 6 7 8
                {
                    bool f = false;

                    q_2[vertex_order[low]] = 0;    
                    for(auto v : critical_vertex[current_v])
                    {
                        int input = 0;
                        for(auto w : neighbors[v]) input += q_2[w];
                        
                        if(input < threshold[v])
                        {
                            inferred_neighbors[v].push_back(vertex_order[low]);
                            inferred_threshold[v]++;
                            f = true;
                        }
                    }

                    if(f)
                    {
                        low++;
                        high = u-1;
                    }

                    q_2 = q_3;

                    continue; 
                }

                if(low == high && high == (u-1))
                {   
                    q_2[vertex_order[low]] = 0; 

                    for(auto v : critical_vertex[current_v])
                    {
                        int input = 0;
                        for(auto w : neighbors[v]) input += q_2[w];
                        
                        if(input < threshold[v])
                        {
                            inferred_neighbors[v].push_back(vertex_order[low]);
                            inferred_threshold[v]++;
                        }
                    }

                    break;
                }

                int mid = (low + high) / 2;
                for(int i = low; i <= mid; ++i)
                {
                    q_2[vertex_order[i]] = 0;
                }

                nei_exists = false;
                for(auto v : critical_vertex[current_v])
                {
                    int input = 0;
                    for(auto w : neighbors[v]) input += q_2[w];
                    
                    if(input < threshold[v])
                    {
                        nei_exists = true; // 1 2 3
                        high = mid;
                    }
                }

                q_2 = q_3; // Important

                if(nei_exists == false && low < high)
                {
                    low = mid + 1; 
                    nei_exists = true;
                }
            } //while(nei_exists)

            // ----------------------------------------
            // find neighbors after the critical node
            // ----------------------------------------
            
            q_3[current_v]= 0; // important
            q_2 = q_3;

            nei_exists = true; // There are still undiscovered edges
            low = u + 1; //low
            high = n-1;

            if(u == n-1) nei_exists = false;

            while(nei_exists)
            {
                total_queries++;

                if(low == high && high != (n-1))
                {
                    bool f = false;
                    q_3[vertex_order[low]] = 1;

                    for(auto v : critical_vertex[current_v])
                    {
                        int input = 0;
                        for(auto w : neighbors[v]) input += q_3[w];

                        if(input >= threshold[v])
                        {
                            inferred_neighbors[v].push_back(vertex_order[low]);
                            f = true;
                        }
                    }
                    if(f)
                    {
                        low++;
                        high = n-1;
                    }
                    q_3 = q_2;

                    continue;
                }
                
                if(low == high && high == (n-1))
                {
                    q_3[vertex_order[low]] = 1;
                    for(auto v : critical_vertex[current_v])
                    {
                        int input = 0;
                        for(auto w : neighbors[v]) input += q_3[w];
                        {
                            if(input >= threshold[v])
                            {
                                inferred_neighbors[v].push_back(vertex_order[low]);
                            }
                        }
                    }
                    break;
                }

                int mid = (low + high) / 2;
                
                for(int i = low; i <= mid; ++i)
                {
                    q_3[vertex_order[i]] = 1;
                }

                nei_exists = false;
                for(auto v : critical_vertex[current_v])
                {
                    int input = 0;
                    for(auto w : neighbors[v]) input += q_3[w];

                    if(input >= threshold[v])
                    {
                        nei_exists = true;
                        high = mid;
                    }
                }

                q_3 = q_2; // Important
                if(nei_exists == false && low < high)
                {
                    low = mid + 1; 
                    nei_exists = true;
                }
            }
        }
    }

    int total_batch_queries = n + (n * (n-1)) + 1;

    Syds inferred_S;
    inferred_S.neighbors = inferred_neighbors;
    inferred_S.threshold = inferred_threshold;
    inferred_S.num_of_query = total_queries;
    inferred_S.query_ratio = float(total_queries) / float(total_batch_queries);
    inferred_S.num_of_critical = num_of_cp;

    cout<<"---------------------------------------"<<endl;
    cout<<"- Inference Algorithm - Binary Search: "<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"The total number of adaptive queries: "<<total_queries<<endl;
    cout<<"Fraction of the number of batch queries: "<<float(total_queries) / float(total_batch_queries)<<endl;
    cout<<"Saved "<<(total_batch_queries - total_queries)<<" queries"<<endl;
    cout<<"--------------------------------------"<<endl;
    cout<<"The number of anchor vertices: "<<num_of_cp<<endl;
    cout<<"--------------------------------------"<<endl;
    
    return inferred_S;
} //adaptive_binary_search_inference


// -------------------------------------------------------- //
//        Linear-searched based inference algorithm         //
// -------------------------------------------------------- //

Syds adaptive_linear_search_inference(const vector<vector<int>>& neighbors, const vector<int>& threshold)
{
    random_device rd;
    mt19937 g(rd());

    int n = neighbors.size();
    int num_of_cp = 0; // The number of critical points

    vector<int> vertex_order(n);
    iota(vertex_order.begin(), vertex_order.end(), 0); //[0, 1, ..., n-1]

    shuffle(vertex_order.begin(), vertex_order.end(), g);

    int total_queries = 0;

    int node_count = 0; // the number of vertices whose critical vertex have been found

    vector<bool> found (n, 0); // found[v] = 1 iff the critical node of v has been found

    vector<vector<int>> critical_vertex(n); // critical_vertex[v] is the list of vertices whose critical vertex is v
    
    vector<int> q_1 (n, 0); // the first query

    int pointer = 0; // the index of the pointer

    vector<vector<int>> inferred_neighbors(n);

    vector<int> inferred_threshold(n, 1); // assume that there are no constant node, and the baseline value is 1 to account for the critical node

    // Find the critical node of each vertex
    while(node_count < n)
    {
        int current_v = vertex_order[pointer];
        q_1[current_v] = 1;

        for(int u = 0; u != n; ++u)
        {
            if(!found[u])
            {
                int input = 0;
                for(auto v : neighbors[u]) input += q_1[v];

                if(input >= threshold[u])
                {
                    found[u] = 1;
                    critical_vertex[current_v].push_back(u);
                    node_count++;
                    inferred_neighbors[u].push_back(current_v); // THERE IS NO NEED TO DO inferred_neighbors[pointer].push_back(u);
                }
            }
        }

        pointer++;
        total_queries++;
    }

    // Now we have critical_vertex[]
    for(int u = 0; u < n; ++u)
    {
        int current_v = vertex_order[u];
        if(!critical_vertex[current_v].empty()) // current_v is the critical vertex of some vertex
        {
            num_of_cp++; // increase the counter for the number of cirtical vertices
            vector<int> q_2(n, 0);
            for(int i = 0; i <= u; ++i) q_2[vertex_order[i]] = 1;
            vector<int> q_3 = q_2;

            pointer = 1;
            
            // find neighbors before the pointer
            while(pointer <= u)
            {
                q_2[vertex_order[u-pointer]] = 0;
                
                for(auto v : critical_vertex[current_v])
                {
                    int input = 0;

                    for(auto w : neighbors[v]) input += q_2[w];

                    if(input < threshold[v])
                    {
                        inferred_neighbors[v].push_back(vertex_order[u-pointer]);   
                        inferred_threshold[v]++;
                    }
                    
                }
                
                q_2[vertex_order[u-pointer]] = 1; // very important
                pointer++; 
                total_queries++;
            }

            // find neighbors after the pointer
            q_3[current_v]= 0; // important
            pointer = 1;

            while(u + pointer <= n-1)
            {
                q_3[vertex_order[u+pointer]] = 1; 
                
                for(auto v : critical_vertex[current_v])
                {
                    int input = 0;
                    for(auto w : neighbors[v]) input += q_3[w];
                    if(input >= threshold[v]) 
                    {
                        inferred_neighbors[v].push_back(vertex_order[u+pointer]);
                    }
                }
            
                q_3[vertex_order[u+pointer]] = 0; // IMPORTANT!
                pointer++;
                total_queries++;
            }
                 
        }
    }

    int total_batch_queries = n + (n * (n-1)) + 1;

    Syds inferred_S;
    inferred_S.neighbors = inferred_neighbors;
    inferred_S.threshold = inferred_threshold;
    inferred_S.num_of_query = total_queries;
    inferred_S.query_ratio = float(total_queries) / float(total_batch_queries);
    inferred_S.num_of_critical = num_of_cp;

    cout<<"---------------------------------------"<<endl;
    cout<<"- Inference Algorithm - Linear Search: "<<endl;
    cout<<"---------------------------------------"<<endl;
    cout<<"The total number of adaptive queries: "<<total_queries<<endl;
    cout<<"Fraction of the number of batch queries: "<<float(total_queries) / float(total_batch_queries)<<endl;
    cout<<"Saved "<<(total_batch_queries - total_queries)<<" queries"<<endl;
    cout<<"--------------------------------------"<<endl;
    cout<<"The number of anchor vertices: "<<num_of_cp<<endl;
    cout<<"--------------------------------------"<<endl;
    
    return inferred_S;
}


vector<int> count_k(const vector<vector<int>>& neighbors, const vector<int>& threshold, int num_permu)
{
    vector<int> count_of_ciritial (num_permu, 0);

    for(int z = 0; z < num_permu; ++z)
    {
        cout<<z<<endl;

        int n = neighbors.size();

        int num_of_cp = 0; // The number of critical points

        vector<int> vertex_order(n);
        iota(vertex_order.begin(), vertex_order.end(), 0); //[0, 1, ..., n-1]

        random_shuffle(vertex_order.begin(), vertex_order.end());

        int total_queries = 0;

        int node_count = 0; // the number of vertices whose critical vertex have been found

        vector<bool> found (n, 0); // found[v] = 1 iff the critical node of v has been found

        vector<vector<int>> critical_vertex(n); // critical_vertex[v] is the list of vertices whose critical vertex is v
        
        vector<int> q_1 (n, 0); // the first query

        int pointer = 0; // the index of the pointer

        vector<vector<int>> inferred_neighbors(n);

        vector<int> inferred_threshold(n, 1); // assume that there are no constant node, and the baseline value is 1 to account for the critical node

        // Find the critical node of each vertex
        while(node_count < n)
        {
            int current_v = vertex_order[pointer];

            q_1[current_v] = 1;

            bool is_critical = false;

            for(int u = 0; u != n; ++u)
            {
                if(!found[u])
                {
                    int input = 0;
                    for(auto v : neighbors[u]) input += q_1[v];

                    if(input >= threshold[u])
                    {
                        is_critical = true;
                        found[u] = 1;
                        critical_vertex[current_v].push_back(u);
                        node_count++;
                        inferred_neighbors[u].push_back(current_v); // THERE IS NO NEED TO DO inferred_neighbors[pointer].push_back(u);
                    }
                }
            }
            if(is_critical)
            {
                num_of_cp++;
            }

            pointer++;
            total_queries++;
        }

        count_of_ciritial[z] = num_of_cp;
    }
       
    return count_of_ciritial;
}


