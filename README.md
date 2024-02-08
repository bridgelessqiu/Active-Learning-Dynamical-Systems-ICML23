# Active Learning Dynamical Systems

Paper: *Efficiently Learning the Topology and Behavior of a Networked Dynamical System Via Active Queries*
<br/>
**Full version** (contains complete proofs of theorems): [https://proceedings.mlr.press/v162/rosenkrantz22a/rosenkrantz22a.pdf]


## Directory structure:

1. **algo/**: the code for the inference algorithm

2. **networks/**
- the selected real networks: lastfm, gnutella, fb, astroph, deezer
- random networks: gnp network and power-law network of size 1,000 and average degree of around 100. 

## To Compile the code on Linux   

1. `cd algo/`

2. `g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_real.cpp -o inf_real`

3. `g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_synthetic.cpp -o inf_synthetic`


## To Compile the code on Mac  

1. `cd algo/`

2. `g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_real.cpp`

3. `mv a.out inf_real`

4. `g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_synthetic.cpp`

5. `mv a.out inf_synthetic`


## To run inference algorithms on real networks 

`./inf_real heuristic_type network_name theta`

- heuristic_type: `1` - Linear-search based inference `2` - Binary-search based inference 
- network_name: `lastfm`, `gnutella`, `fb`, `astroph`, `deezer`
- theta: `0.1`, `0.9`

### Example 1: run the binary-search based algorithm on lastfm network under theta = 0.9

`./inf_real 2 lastfm 0.9`

### Example 2: run the linear-search based algorithm on astroph network under theta = 0.1:

`./inf_real 1 astroph 0.1`


## To run inference algorithms on synthetic networks 

`./inf_synthetic heuristic_type network_name theta`

- heuristic_type: `1` - Linear-search based inference `2` - Binary-search based inference 
- network_name: `gnp`, `power_law`
- theta: `0`, `0.9`

### Example 3: run the binary-search based algorithm on gnp network under theta = 0

`./inf_synthetic 2 gnp 0`

### Example 4: run the linear-search based algorithm on power-law network under theta = 0.9

`./inf_synthetic 1 power_law 0.9`
