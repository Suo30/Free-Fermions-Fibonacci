// Matrix Uniform_H(long N=10, double W=0, string boundary="PBC"){
//     Matrix H(N);
//     //Defining Initial Hamiltionian
//     for(int i=1;i<N;i++){
//         H(i,i+1) = H(i+1,i) = -1;
//         H(i,i) = Rand(-W,W);
//     }
//     H(N,N) = Rand(-W,W);
//     if (boundary == "PBC"){
//         H(N,1) = -1;
//         H(1,N) = -1;
//     }
//     return H;
// }

// Matrix Dimerized_H(long N, double W, string boundary){
//     Matrix H(N);
//     double J1 = 1;
//     double J2 = 0.5;
//     for (int i=1;i<N;i++){
//         if(i%2==0){
//             H(i+1,i) = H(i,i+1) = -J2;
//         }
//         else{
//             H(i+1,1) = H(i,i+1) = -J2;
//         }
//         H(i,i) = Rand(-W,W);
//     }
//     H(N,N) = Rand(-W,W);
//     if (boundary == "PBC"){
//         H(1,N) = H(N,1) -J2;
//     }
//     return H;
// }

// Matrix Rainbow_H(long N, double W, string boundary){
//     Matrix H(N);
//     double h = 1;
//     for (int i=1; i<N; i++){
//         if (i<N/2){
//             H(i,i+1) = H(i+1,i) = -exp(-h*(N/2-i));
//         }
//         else{
//             H(i,i+1) = H(i+1,i) = -exp(h*(N/2-i));
//         }
//         H(i,i) = Rand(-W,W);
//     }
//     H(N,N) = Rand(-W,W);
//     if (boundary == "PBC"){
//         H(1,N) = H(N,1) = -exp(h*(N/2-1));
//     }
//     return H;
// }

long Fibonacci_num (long N){
    long N_0 = 0;
    long N_1 = 1;
    long fibonacci;
    if (N<=0){return N_0;}
    if (N==1){return N_1;}
    for (int i=2;i<=N;i++){
        fibonacci = N_1 + N_0;
        N_0 = N_1;
        N_1 = fibonacci;
        // std::cout << fibonacci << std::endl;
    }
    return fibonacci;
}

Vector Fibonacci_rabbits(long N){
    // Vector sequence (1);
    // Vector sequence1 (1);
    // Vector sequence2 (1);
    // sequence1(1) = 0;
    // sequence2 (1) = 1;
    // // Edge Casees
    // if (N <= 0) return {};
    // if (N == 1) return {0};
    // if (N == 2) return {1};

    // //Iterative addition
    // for (int i=3;i<=N;i++){
    //     sequence = sequence2;
    //     sequence.Append(sequence1);
    //     sequence1 = sequence2;
    //     sequence2 = sequence;
    // }
    Vector sequence (1);
    Vector sequence1 (1);
    Vector sequence2 (1);
    sequence1(1) = 0;
    sequence2 (1) = 1;
    // Edge Casees
    if (N <= 0) return {};
    if (N == 1) return {1};


    //Iterative addition
    for (int i=3;sequence.N<N;i++){
        sequence = sequence2;
        sequence.Append(sequence1);
        sequence1 = sequence2;
        sequence2 = sequence;
    }
    return sequence;
}

long sturmian_num (long N, int k){
    long N_0 = 0;
    long N_1 = 1;
    long sturmian;
    
    // Edge Cases
    if (N <= 0) { return N_0; }
    if (N == 1) { return N_1; }
    
    // Iterative addition
    for (int i = 2; i <= N; i++){
        // The generalized substitution rule: k copies of N_1, plus one N_0
        sturmian = k * N_1 + N_0;
        
        // Shift variables for the next iteration
        N_0 = N_1;
        N_1 = sturmian;
        
        // std::cout << sturmian << std::endl;
    }
    
    return sturmian;
}

Vector Sturmian_Sequence(long N, double theta){
    Vector sequence(N);

    if (N <= 0) return {};

    for (int i=2; i<=N+1; i++){
        sequence(i-1) = floor(i*theta) - floor((i-1)*theta);
    }
    return sequence;
}

Vector Not_Fibonacci(long N, string chain){
    Vector sequence (1);
    Vector sequence1 (1);
    Vector sequence2 (1);

    if (chain == "fib_57"){
        sequence1(1) = 1;
        sequence1.Append(0);
        sequence2 (1) = 1;
        sequence2.Append(0);
        sequence2.Append(1);
        sequence2.Append(0);
        sequence2.Append(1);
    }

    else if (chain == "fib_711"){
        sequence1(1) = 0;
        sequence2 (1) = 1;
        sequence2.Append(0);
        sequence2.Append(1);
    }
    else if (chain == "fib_59"){
        sequence1(1) = 1;
        sequence2 (1) = 1;
        sequence2.Append(0);
        sequence2.Append(1);
        sequence2.Append(0);
    }
    else{
        sequence1(1) = 0;
        sequence2 (1) = 1;
    }

    // Edge Casees
    if (N <= 0) return {};
    if (N == 1) return {1};


    //Iterative addition
    for (int i=3;sequence.N<N;i++){
        sequence = sequence2;
        sequence.Append(sequence1);
        sequence1 = sequence2;
        sequence2 = sequence;
    }
    // sequence.Write();
    return sequence;
}

Matrix Chain_H(long N, double W, string boundary, string chain, double J=1, double sigma=0.1, double h=1, double theta=0.6){
    Matrix H(N);
    if (chain == "uniform"){
        for(int i=1;i<N;i++){
            H(i,i+1) = H(i+1,i) = -1;
            H(i,i) = Rand(-W,W);
        }
        H(N,N) = Rand(-W,W);
        if (boundary == "PBC"){
            H(N,1) = -1;
            H(1,N) = -1;
        }
    }
    else if (chain == "dimerized"){
        double J1 = J*(1+sigma);
        double J2 = J*(1-sigma);
        // double J1 = 1;
        // double J2 = 0;
        for (int i=1;i<N;i++){
            if(i%2==1){
                H(i+1,i) = H(i,i+1) = -J1;
            }
            else{
                H(i+1,i) = H(i,i+1) = -J2;
            }
            H(i,i) = Rand(-W,W);
        }
        H(N,N) = Rand(-W,W);
        if (boundary == "PBC"){
            H(1,N) = H(N,1) = -J2;
        }
    }
    else if (chain == "rainbow"){
        for (int i=1; i<N; i++){
            if (i<N/2){
                H(i,i+1) = H(i+1,i) = -exp(-h*(N/2-i));
            }
            else{
                H(i,i+1) = H(i+1,i) = -exp(h*(N/2-i));
            }
            H(i,i) = Rand(-W,W);
        }
        H(N,N) = Rand(-W,W);
        if (boundary == "PBC"){
            H(1,N) = H(N,1) = -exp(h*(N/2-1));
        }
    }
    else if (chain == "random"){
        for (int i=1; i<N; i++){
            H(i,i+1) = H(i+1,i) = Rand(-1,1);
            H(i,i) = Rand(-W,W);
        }
        H(N,N) = Rand(-W,W);
        if (boundary == "PBC"){
            H(1,N) = H (N,1) = Rand(-1,1);
        }
    }
    else if (chain == "fibonacci"){
        Vector sequence;
        sequence = Fibonacci_rabbits(N-1);
   
        for (int i=1; i<N; i++){
            H(i,i+1) = H(i+1,i) = J*(1+sigma*pow(-1,sequence(i)));
            H(i,i) = Rand(-W,W);
        }
        if (boundary == "PBC"){
            H(1,N) = H(N,1) = J*sequence(1);
        }
    }
    else if (chain == "fib_57" || chain == "fib_59"){
        Vector sequence;
        sequence = Not_Fibonacci(N-1, chain);
   
        for (int i=1; i<N; i++){
            H(i,i+1) = H(i+1,i) = J*(1+sigma*pow(-1,sequence(i)));
            H(i,i) = Rand(-W,W);
        }
        if (boundary == "PBC"){
            H(1,N) = H(N,1) = J*sequence(1);
        }
    }

    else if (chain == "sturmian"){
        Vector sequence;
        sequence = Sturmian_Sequence(N-1, theta);

        for (int i=1; i<N; i++){
            H(i,i+1) = H(i+1,i) = J*(1+sigma*pow(-1,sequence(i)));
            H(i,i) = Rand(-W,W);
        }
        if (boundary == "PBC"){
            H(1,N) = H(N,1) = J*sequence(1);
        }
    }
    
    return H;
}

Matrix Correlation(Matrix H, long N, long P){

    Matrix C(N);
    Matrix Basis; Vector Eigen;
    H.Diagonalize(Basis, Eigen);
    for(int i=1;i<=N;i++){
        for(int j=1;j<=N;j++){
            C(i,j)=0;
            for(int k=1;k<=P;k++){
                C(i,j) = C(i,j) + Basis(i,k)*Basis(j,k);
            }
        }
    }
    // for (int i=1; i<=N; i++){
    //     double sum = 0;
    //     for (int k = 1; k<= N ; k++){
    //         sum = sum + T(Basis).Row(i)(k)*Basis.Col(i)(k);
    //     }
    //     std::cout << "Modulus: " << sum << std::endl;
    // }

    // C.Write();
    // Basis.Write();

    // C.Save("data/correlation.txt");
    return C;
}

double Energy_gap(Vector V, long N, long P){
    double gap;
    gap = V(P+1)-V(P);
    return gap;
}

Vector Gap_vs_N(long minn, long maxn, double W, string boundary, string chain, double J=1, double sigma=0.1, double h=1, double theta=0.5){
    Vector EGap(maxn);
    long j = 0;
    for (int i=minn; j<maxn; i++){
        if (chain == "fibonacci" || chain == "sturmian" || chain == "fib_57" || chain == "fib_59"){
            j = Fibonacci_num(i)+1;
        }
        else{j = i;}
        Matrix H(j);
        H = Chain_H(j,W,boundary,chain,J,sigma,h,theta);
        Vector Eigen; Matrix Basis;
        H.Diagonalize(Basis,Eigen);
        EGap(j) = Energy_gap(Eigen, j, j/2);
        
        // std::cout << i << j <<"-------------------------------------------------------------------" << std::endl;
        // Eigen.Write();
    }
    // EGap.Save("data/energy_gap.txt");
    return EGap;
}

double EE_block(long l,long N, Matrix C, string entropy_order = "forward"){
    Matrix C_A(N);
    // Copy matrix for editing and trimming for the first l entries
    C_A = C;
    double entropy = 0.0;
    // Select Subblock of C depending on the Entropy direction we're calculating
    if (entropy_order == "forward"){
        C_A.Part(1,1,l,l);
    }
    else if (entropy_order == "backward"){
        C_A.Part(N-l+1,N-l+1,N,N);
    }
    else if (entropy_order == "center"){
        C_A.Part(((N-l)/2)+1,((N-l)/2)+1,((N-l+1)/2)+l,((N-l+1)/2)+l);
    }
    // std::cout << ((N-l)/2)+1 << "   " << ((N-l+1)/2)+l << std::endl;
    Vector Eigen; Matrix Basis;
    // Eigenvalues for Von Neumann entropy
    C_A.Diagonalize(Basis, Eigen);
    // Sum every value of k up to l
    for (int k = 1;k<=l;k++){
        double vk = Eigen(k); 

        // Safety Check
        if (vk < 1e-12 || vk > (1-1e-12)){
            continue;
        }
        // Von Neumann Entropy
        entropy += -vk*log(vk) - (1-vk)*log(1-vk);
    }
    // Eigen.Write();
    // std::cout << "-------------------------------------------------------" << entropy << std::endl;
    return entropy;
}

Vector EE_vs_l(long N, Matrix C, string entropy_order = "forward"){
    // Define vector and calculate for every value of l
    Vector Entropy_vector(N+1);
    // First elementn in the vector needs to be 0 (no entropy since no elements)
    Entropy_vector(0) = 0;
    // Add the value increasing the size of the sub matrix
    for (int i=1;i<=N;i++){
        Entropy_vector(i+1) = EE_block(i,N,C,entropy_order);
    }
    // Entropy_vector.Save("data/entropy.txt");
    return Entropy_vector;
}

Vector Two_point_correlation(long N, Matrix C){
    Vector two_point(N/2);
    for (int i=1;i<=N/2;i++){
        two_point(i) = std::abs(C(N/2,i));
    }
    return two_point;
}

double Get_IPR(long N, Matrix Basis){
    // Measure how spread the wavevector is across the system, with 1 being completely localized and 1/N being completely delocalized
    Vector IPR_n(N);
    for (int k=1;k<=N;k++){
        // Calculate the sum of the fourth power of the coefficients of the eigenvector, which gives the IPR
        double sum = 0;
        for (int i=1;i<=N;i++){
            sum += pow(Basis(i,k),4);
        }
        IPR_n(k) = sum;
    }
    double IPR_total_avg = Sum(IPR_n)/N;  // Normalize by the system size to get a value between 1/N and 1
    return IPR_total_avg; 
}

Vector Inverse_participation_ratio(long N, double W, string boundary, string chain, double J=1, double sigma=0.1, double h=1, double theta=0.5, long minf=1, long maxf=16){
    if (chain == "fibonacci" || chain == "sturmian" || chain == "fib_57" || chain == "fib_59"){
        long fib;
        Vector IPR(Fibonacci_num(maxf)+1);
        for (int i=minf;i<=maxf;i++){
            std::cout << "Entry:" << i << std::endl;
            fib = Fibonacci_num(i);
            std::cout << "Fibonacci number:" << fib << std::endl;
            if (fib%2==1){
                Matrix H(fib+1);
                H = Chain_H(fib+1,W,boundary,chain,J,sigma,h,theta);
                Matrix Basis; Vector Eigen;
                H.Diagonalize(Basis, Eigen);
                IPR(fib+1) = Get_IPR(fib+1,Basis);
            }
        }
        return IPR;
    }
    else{
        Vector IPR(N+1);
        for (int i=22;i<=N;i++){
            if (i%2==1){
                Matrix H(i+1);
                H = Chain_H(i+1,W,boundary,chain,J,sigma,h);
                Matrix Basis; Vector Eigen;
                H.Diagonalize(Basis, Eigen);
                IPR(i+1) = Get_IPR(i+1,Basis);
            }
        }
        return IPR;
    }

}

Vector Density_Function(long N, Matrix C){
    Vector density(N);
    for (int i=1;i<=N;i++){
        density(i) = C(i,i);
    }
    return density;
}

Vector entropy_vs_N(long minn, long maxn, double W, string boundary, string chain, double J=1, double sigma=0.1, double h=1, double theta=0.5, string entropy_order = "forward"){
    Vector Entropy_N(maxn);
    long j = 0;
    for (int i=minn; j<maxn; i++){
        // if (chain == "fibonacci" || chain == "sturmian" || chain == "fib_57" || chain == "fib_59"){
        //     j = Fibonacci_num(i)+1;
        // }
        // else{j = i;}
        j = i;
        Matrix H(j);
        if (j%2==0){
            H = Chain_H(j,W,boundary,chain,J,sigma,h,theta);
            Matrix C = Correlation(H,j,j/2);
            Entropy_N(j) = EE_block(j/2,j,C,entropy_order);
        }
        
        // std::cout << i << j <<"-------------------------------------------------------------------" << std::endl;
        // Eigen.Write();
    }
    // EGap.Save("data/energy_gap.txt");
    return Entropy_N;
}