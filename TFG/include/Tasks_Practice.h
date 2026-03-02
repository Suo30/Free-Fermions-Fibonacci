void Task2_Diagonalize(){
    long L=10;
    double t=-1;
    Matrix H(L);
    for (long i=1;i<L;i++){
        H(i,i+1)=H(i+1,i)=t;
    }
    Matrix Basis; Vector Eigen;
    H.Diagonalize(Basis,Eigen);
    std::cout << "Hamiltionian" << std::endl;
    H.Write();
    std::cout << "Energies" << std::endl;
    Eigen.Write();
    std::cout << "Eigenvectors" << std::endl;
    Basis.Write();
}

void Task3_Disorder(){
    long L=10;
    double t=-1;
    double W=10;
    Matrix H(L);
    for (long i=1;i<L;i++){
        H(i,i+1)=H(i+1,i)=t;
        H(i,i)=Rand(-W,W);
    }
    Matrix Basis; Vector Eigen;
    H.Diagonalize(Basis,Eigen);
    std::cout << "Hamiltionian" << std::endl;
    H.Write();
    std::cout << "Energies" << std::endl;
    Eigen.Write();
    std::cout << "Eigenvectors" << std::endl;
    Basis.Write();
}

void Task4_Visualize(){
    long L=10;
    double t=-1;
    double W=1;
    Matrix H(L);
    for (long i=1;i<L;i++){
        H(i,i+1)=H(i+1,i)=t;
        H(i,i)=Rand(-W,W);
    }
    H(L,L)=Rand(-W,W);
    Matrix Basis; Vector Eigen;
    H.Diagonalize(Basis,Eigen);
    // std::cout << "Hamiltionian" << std::endl;
    // H.Write();
    // std::cout << "Energies" << std::endl;
    // Eigen.Write();
    // std::cout << "Eigenvectors" << std::endl;
    // Basis.Write();

    double win_w=1000;
    double win_h=750;
	EX_Start(100,100,win_w,win_h);
    EX_Color("red");

    Vector psi=Basis.Col(1);
    double dx=win_w/L;

    for (long i=1; i<=L; i++){
        double density=psi(i)*psi(i);

        double bar_height=density*win_h*0.9;
        double y_pos=win_h-bar_height;

        EX_Fill_Rectangle((i-1)*dx,y_pos,dx,bar_height);

        EX_Color("black");
        EX_Rectangle((i-1)*dx, y_pos, dx, bar_height);
        EX_Color("red");
    }
    psi.Write();
	EX_Read_Key();
}

void Task5_Fermion(){
    long sites=4;    

    for (long i=0; i<=15; i++){
        char* state=Bin_2_String(i,sites-1);
        std::cout << "State " << i << " (" << state <<") Particles: " << Count_Ones(i) << std::endl;
    }
}