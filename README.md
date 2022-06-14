# Inference-ICML

Directory structure:

**algo/**
- the code for the inference algorithm

**networks/**

- the selected real networks: lastfm, gnutella, fb, astroph, deezer
- random networks: gnp network and power-law network of size 1,000 and average degree of around 100. 

## To Compile the code on Linux   

`cd algo/`

`g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_real.cpp -o inf_real`

`g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_synthetic.cpp -o inf_synthetic`


## To Compile the code on Mac  

`cd algo/`

`g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_real.cpp`

`mv a.out inf_real`

`g++ -std=c++11 -O3 algo.cpp other_function.cpp syds.h inf_synthetic.cpp`

`mv a.out inf_synthetic`


## To run inference algorithms on real networks 

`./inf_real heuristic_type network_name theta`
- heuristic_type:
    1. Linear-search based inference 
    2. Binary-search based inference 
    - network_name: lastfm, gnutella, fb, astroph, deezer
    -theta: 0.1, 0.9

Example:
    run the binary-search based algorithm on lastfm network under theta = 0.9:
    `./inf_real 2 lastfm 0.9`

   run the linear-search based algorithm on astroph network under theta = 0.1:
    `./inf_real 1 astroph 0.1`

------------------------------------------------------------
-     To run inference algorithms on synthetic networks    -
------------------------------------------------------------
./inf_synthetic heuristic_type network_name theta
    - heuristic_type:
        1 - Linear-search based inference 
        2 - Binary-search based inference
   - network_name: gnp, power_law
   - theta: 0, 0.9

Example:
    run the binary-search based algorithm on gnp network under theta = 0:
    ./inf_synthetic 2 gnp 0

    run the linear-search based algorithm on power-law network under theta = 0.9:
    ./inf_synthetic 1 power_law 0.9
