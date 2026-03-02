 void Draw_Matrix_Heatmap(const Matrix& M, double sites, double x0, double y0, double width) {
    long rows = sites;
    long cols = sites;
    
    // 1. Calculate the cell size based on total width
    double dx = width / cols;
    double dy = width / rows; // Assuming square cells

    // 2. Find the absolute maximum value to normalize colors
    //    (So the strongest value is always pure Red/Blue)
    double max_val = 0.0;
    for(long i=1; i<=rows; i++) {
        for(long j=1; j<=cols; j++) {
            if (std::abs(M(i,j)) > max_val) max_val = std::abs(M(i,j));
        }
    }
    if (max_val == 0) max_val = 1.0; // Prevent division by zero if matrix is empty

    // 3. Loop through and draw
    for(long i=1; i<=rows; i++) {
        for(long j=1; j<=cols; j++) {
            double val = M(i,j);
            double intensity = std::abs(val) / max_val; // 0.0 to 1.0

            // COLOR LOGIC:
            // White is (1, 1, 1). Red is (1, 0, 0). Blue is (0, 0, 1).
            
            if (val > 0) {
                // POSITIVE: Red Gradient (White -> Red)
                // We keep Red maxed (1.0), and reduce Green/Blue as intensity increases
                EX_Color(1.0, 1.0 - intensity, 1.0 - intensity);
            } 
            else if (val < 0) {
                // NEGATIVE: Blue Gradient (White -> Blue)
                // We keep Blue maxed (1.0), and reduce Red/Green as intensity increases
                EX_Color(1.0 - intensity, 1.0 - intensity, 1.0);
            } 
            else {
                // ZERO: Pure White
                EX_Color(1.0, 1.0, 1.0);
            }

            // Draw the filled square
            // Note: Graphics (0,0) is top-left.
            // x position depends on column (j)
            // y position depends on row (i)
            double px = x0 + (j-1) * dx;
            double py = y0 + (i-1) * dy;

            EX_Fill_Rectangle(px, py, dx, dy);
            
            // Optional: Draw a faint grey border to see the grid for white cells
            // EX_Color(0.9, 0.9, 0.9);
            // EX_Rectangle(px, py, dx, dy);
        }
    }
}


// --- Helper: Maps a value to Log10 X-coordinate ---
// i: current index (must be > 0)
// max_i: total number of entries
// width: screen width
double Get_Log_X(long i, long max_i, double width) {
    if (i <= 0) return 0; // Protect against log(0)
    
    // Math: log10(i) / log10(max) gives a ratio from 0.0 to 1.0
    double ratio = std::log10((double)i) / std::log10((double)max_i);
    return ratio * width;
}

// --- The Plotting Function ---
void Draw_Vector_LogScale(const Vector& V, double x0, double y0, double width, double height) {
    long N = V.N;
    
    // 1. Auto-Scale Y-Axis
    // We need min and max to fit the curve in the box
    double min_val = V(1);
    double max_val = V(1);
    for(long i=1; i<=N; i++) {
        if (V(i) > max_val) max_val = V(i);
        if (V(i) < min_val) min_val = V(i);
    }
    double range_y = max_val - min_val;
    if (range_y == 0) range_y = 1.0; // Avoid division by zero

    // 2. Draw Background Grid (Decades: 1, 10, 100, 1000...)
    EX_Color(0.8, 0.8, 0.8); // Light Grey
    
    // We start at 10^0 (1) and multiply by 10 until we pass N
    for(long decade = 1; decade <= N; decade *= 10) {
        double px = x0 + Get_Log_X(decade, N, width);
        EX_Line(px, y0, px, y0 + height);
        
        // Optional: Draw text marker (requires converting number to string)
        // You can skip this if your library doesn't have easy text tools
    }

    // 3. Draw the Data Curve
    EX_Color(0.0, 0.0, 0.8); // Dark Blue line
    
    double prev_x = x0 + Get_Log_X(1, N, width);
    // Y-Map: Invert so Max is at top (y0), Min is at bottom (y0 + height)
    double prev_y = y0 + height - ((V(1) - min_val) / range_y) * height;

    for(long i=2; i<=N; i++) {
        // Calculate new coordinates
        double curr_x = x0 + Get_Log_X(i, N, width);
        double curr_y = y0 + height - ((V(i) - min_val) / range_y) * height;

        // Draw line connecting previous point to current point
        EX_Line(prev_x, prev_y, curr_x, curr_y);
        
        // Optional: Draw point
        // EX_Fill_Circle(curr_x, curr_y, 2);

        // Update previous for next step
        prev_x = curr_x;
        prev_y = curr_y;
    }
}