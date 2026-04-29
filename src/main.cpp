 #include "Common.h"
 #include "Matrix.h"
 #include "EX.h"
 #include <iostream>
 #include "Tasks_Practice.h"
 #include "Functions_Computation.h"
 #include "Draw.h"
 #include <cmath>

 /*-----------------------------------------------------------------------------
                            Global Variables
 -----------------------------------------------------------------------------*/
const double pi = 3.14159265358979323846;
const double phi = (1 + sqrt(5)) / 2; // Golden ratio
const double e = 2.71828182845904523536; // Euler's number

long N = 234;
double W = 0;
long P = N/2;

string boundary = "OBC";   // "PBC", "OBC"
string chain = "fibonacci";   //"uniform", "dimerized", "rainbow", "random", "fibonacci", "sturmian", "fib_57", "fib_59", "fib_711"
string entropy_order = "forward"; //"forward", "backward", "center"
 
double J = 1;
double sigma = 0.3;
double h = 0.6;
double theta = 1/e; 

long minn = 5;
long maxn = 988; //2,"3",'4',6,"9",14,22,"35",'56',90,"145",234,378,"611",'988',1598,"2585",4182
long minf = 6;
long maxf = 16;
long l = 7;

 /*-----------------------------------------------------------------------------
                            Problem Variables
 -----------------------------------------------------------------------------*/
Vector Gap_vs_Sigma(double mins, double maxs, long N, double W, string boundary, string chain, double theta){
    Vector EGap(10);
    for (int i=1; i<=10; i++){
        Matrix H(N);
        H = Chain_H(N,W,boundary,chain,i*maxs/10,h,theta);
        Vector Eigen; Matrix Basis;
        H.Diagonalize(Basis,Eigen);
        // std::cout << i<<"-------------------------------------------------------------------" << std::endl;
        EGap(i) = Energy_gap(Eigen, N, N/2);
        // Eigen.Write();
    }
    EGap.Save("data/energy_gap_s.txt");
    return EGap;
    
}



 /*-----------------------------------------------------------------------------
                            Main
 -----------------------------------------------------------------------------*/

int main()
{   Rand_Open(1); 

    Matrix C(N);
    Matrix H(N);
    Vector Gap(maxn-minn);
    Vector EE(N);
    Vector density(N);
    Vector Entropy_N(maxn-minn+1);

    std::cout << "=====Calculating Chain=====" << std::endl;
    H = Chain_H(N,W,boundary,chain,J,sigma,h,theta);
    Vector Eigen; Matrix Basis;
    H.Diagonalize(Basis,Eigen);
    H.Write();
    Basis.Save("data/eigenvectors.txt");
    Eigen.Save("data/eigenvalues.txt");

    std::cout << "=====Correlation Matrix=====" << std::endl;
    C = Correlation(H,N,P);
    C.Save("data/correlation.txt");

    std::cout << "=====Density Function=====" << std::endl;
    density = Density_Function(N,C);
    density.Save("data/density.txt");

    std::cout << "=====Gap vs N=====" << std::endl;
    Gap = Gap_vs_N(minn,maxn,W,boundary,chain,J,sigma,h);
    Gap.Save("data/energy_gap.txt");

    std::cout << "=====Entanglement Entropy=====" << std::endl;
    EE = EE_vs_l(N,C,entropy_order);
    EE.Save("data/entropy.txt");

  std::cout << "=====EE vs system size N=====" << std::endl;
    Entropy_N = entropy_vs_N(minn,maxn,W,boundary,chain,J,sigma,h,theta,entropy_order);
    Entropy_N.Save("data/entropy_vs_N.txt");
    // long fib;
    // for (int i = 0;i<=15;i++){
    //     fib = Fibonacci_num(i);
    //     std::cout << fib << std::endl;
    // }

    // Vector IPR(N);
    // IPR = Inverse_participation_ratio(N,H);
    // IPR.Save("data/inverse_participation_ratio.txt");
    

    if (chain == "dimerized" || chain == "fibonacci"){
        Gap_vs_Sigma(0,0.5,N,W,boundary,chain,theta);
        // EGap.Save("data/energy_gap_s.txt");
    }

    std::cout << "=====Gap Ratio=====" << std::endl;
    Vector Gaps(N);
    Vector Gap_Ratio(N);
    Gaps(1) = 0;
    Gap_Ratio(1) = 0;
    for (int i=2;i<=N;i++){
        Gaps(i) = Energy_gap(Eigen, N, i-1);
        Gap_Ratio(i) = Gaps(i)/Gaps(i-1);
        if (Gap_Ratio(i)>1){
            Gap_Ratio(i) = 1/Gap_Ratio(i);
        }
    }
    Gaps.Save("data/energy_gap_vs_site.txt");
    Gap_Ratio.Save("data/energy_gap_ratio_vs_site.txt");

    // Calculating the energy spectrum of Fibonacci for different lengths
    // Save multiple in a single file
    std::cout << "=====Energy Spectrum and Correlations=====" << std::endl;
    long fib;
    FILE* filename = fopen("data/correlation_f.txt","wt");
    FILE* fileenergy = fopen("data/eigenvalues_f.txt","wt");
    FILE* fileenergyvssigma = fopen("data/eigenvalues_f_sigma.txt","wt");
    for (int i=minf;i<=maxf;i++){
        fib = Fibonacci_num(i);
        if (fib%2==1){
            std::cout << "Fibonacci number = " << fib << std::endl;
            Matrix H(fib+1);
            Matrix C(fib+1);
            H = Chain_H(fib+1,W,boundary,chain,J,sigma,h,theta);
            C = Correlation(H,fib+1,(fib+1)/2);
            Matrix Basis; Vector Eigen;
            H.Diagonalize(Basis, Eigen);
            C.Save(filename,"Hello");
            Eigen.Save(fileenergy);
            if ((chain == "dimerized" || chain == "fibonacci" || chain == "sturmian" || chain == "fib_57" || chain == "fib_59" || chain == "fib_711") && i>=maxf){
                for (int j=0;j<=20;j++){
                    H = Chain_H(fib+1,W,boundary,chain,J,sigma=j*0.5/10,h=h,theta=theta);
                    H.Diagonalize(Basis, Eigen);
                    Eigen.Save(fileenergyvssigma);
                }
            }
        }
    }
    std::cout << "=====IPR=====" << std::endl;
    Vector IPR(Fibonacci_num(maxf)+1);
    IPR = Inverse_participation_ratio(Fibonacci_num(maxf)+1,W,boundary,chain,J,sigma,h,theta,minf,maxf);
    // IPR.Write();
    IPR.Save("data/inverse_participation_ratio.txt");


//     double winsize = 800;
//     EX_Start(50,50,winsize,winsize);
//     // --- TEST 1: Power Law Decay ---
//     // On a log-x plot, 1/x looks curved, but we will test 1000 points
    
//     std::cout << "Displaying Log-X Plot..." << std::endl;
//     Draw_Vector_LogScale(Gap, 0, 0, winsize, winsize);

//     EX_Read_Key();



// // 1. Setup Window
//     double winsize = 800;
//     EX_Start(50, 50, winsize, winsize);

//     std::cout << "Drawing Matrix..." << std::endl;
//     Draw_Matrix_Heatmap(C, N, 0, 0, winsize);

//     // 4. Wait
//     std::cout << "Press any key in window to exit." << std::endl;
//     EX_Read_Key();

    return 0;
}