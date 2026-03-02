import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import os
import re


# try:
#     energy_gap = np.loadtxt("energy_gap.txt")
#     print(energy_gap)
# except Exception as e:
#     print(f"Error reading file: {e}")
#     exit()

# try:
#     entropy = np.loadtxt("entropy.txt")
# except Exception as e:
#     print(f"Error reading file: {e}")
#     exit()


def plot_correlation_matrix(filename = "correlation.txt"):
    try:
        #Load Data
        correlation = np.loadtxt(filename)
        # print(correlation)

        rows,cols = correlation.shape
        print(f"Loaded Matrix: {rows}x{cols}")

        # Setup Color Mapping (Blue-White-Red), 0 perfectly White
        # To ensure White = 0, we must make vmin and vmax symmetric.
        max_val = np.max(np.abs(correlation)) # Find the strongest value (positive or negative)
        if max_val == 0: 
            max_val = 1.0
        #Set Maximum intensity
        limit = max_val
        
        # Create the Plot
        plt.figure(figsize=(8, 6)) # Size in inches
        
        # imshow drawing squares using the seismic map, forcing they range to be symmetric for 0 (white) in the middle
        im = plt.imshow(correlation, cmap='seismic', vmin=-limit, vmax=limit, origin='upper')

        # Styling
        plt.colorbar(im, label="Correlation Intensity")
        plt.title(f"Correlation Matrix ({rows}x{cols})")
        plt.xlabel("Site j")
        plt.ylabel("Site i")

        # Show integers if matrix is small
        if rows <= 20:
            plt.xticks(np.arange(cols), np.arange(1, cols+1))
            plt.yticks(np.arange(rows), np.arange(1, rows+1))

        print("Displaying plot...")
        plt.show()


    # Error protection
    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

def plot_energy_gap(filename = "energy_gap.txt"):
    try:
        # Load data
        energy_gap = np.loadtxt(filename)
        length = len(energy_gap)
        x = np.arange(1,length+1)
        print(f"Loaded Vector of size: {length}")

        # Only represent 4p+2 to avoid degeneration
        x = x[1::4]
        energy_gap = energy_gap[1::4]

        # Filter Data, only keep points where Gap > 0
        mask = energy_gap > 1e-60 
        N_clean = x[mask]
        gaps_clean = energy_gap[mask]

        # # Reference line
        # ref_x = np.linspace(min(N_clean), max(N_clean), 100)
        # # Scale the line to match the first data point roughly
        # ref_y = (ref_x**(-1)) * (gaps_clean[0] * ref_x[0]) 

        # Plot Simulation data
        plt.figure(figsize=(8,6))
        plt.loglog(N_clean,gaps_clean, "bo-", label = "Simulation Data", markersize = 5)
        # Plot Reference
        # plt.loglog(ref_x, ref_y, 'r--', alpha=0.5, label='Reference $1/N$ (Metal)')
        # Style
        plt.title("Scaling of Energy Gap")
        plt.xlabel("System Size ($N$)")
        plt.ylabel("Energy Gap ($\Delta E$)")
        plt.grid(True, which="both", linestyle="--", alpha=0.6)
        plt.legend()

        plt.show()



    # Error protection
    except Exception as e:
        print(f"Error reading file: {e}")
        exit()


def plot_entanglement_entropy (filename = "entropy.txt"):
    try:
        # Load data
        entropy = np.loadtxt(filename)
        l_ent = len(entropy)
        x = np.arange(1,l_ent+1)
        print(f"loaded vector of size: {l_ent}")

        # Plot and stylize
        plt.figure(figsize=(8,6))
        plt.plot(x,entropy,"red",label = "Simulation Entropy", markersize = 5)
        plt.title("Entanglement Entropy")
        plt.xlabel("$l$")
        plt.ylabel("Entanglement Entropy $S(l)$")
        plt.grid(True, which="both", linestyle= "--", alpha=0.6)
        plt.legend()
        plt.show()


    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

def plot_correlations(filename):
    all_matrices = parse_multi_matrix_file(filename)
    
    if all_matrices:
        plot_all_matrices(all_matrices)
    else:
        print("No matrices found.")

def parse_multi_matrix_file(filename):
    """
    Parses a file containing multiple matrices stacked on top of each other.
    Format expected:
    # Rows Cols
    # Optional Comment
    data data data...
    """
    matrices = []
    
    current_dims = None # Tuple (rows, cols)
    current_data = []   # Flat list of numbers
    
    # Regex to find header like "# 14 14"
    header_pattern = re.compile(r'^#\s+(\d+)\s+(\d+)')

    if not os.path.exists(filename):
        print(f"Error: File {filename} not found.")
        return []

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line: continue # Skip empty lines

            # 1. Check if this line is a Header (# Rows Cols)
            match = header_pattern.match(line)
            
            if match:
                # If we were already building a matrix, save it first
                if current_dims and current_data:
                    rows, cols = current_dims
                    # Convert list to numpy array and reshape
                    mat = np.array(current_data, dtype=float)
                    
                    # Safety check for size
                    if mat.size == rows * cols:
                        matrices.append(mat.reshape(rows, cols))
                    else:
                        print(f"Warning: Expected {rows*cols} elements but got {mat.size}. Skipping matrix.")
                
                # Start a new matrix
                rows = int(match.group(1))
                cols = int(match.group(2))
                current_dims = (rows, cols)
                current_data = [] # Reset buffer
                
            # 2. Check if it's a generic comment (starts with # but not a header)
            elif line.startswith('#'):
                continue 
                
            # 3. It's Data
            else:
                # Split line by whitespace and convert to floats
                parts = line.split()
                try:
                    numbers = [float(x) for x in parts]
                    current_data.extend(numbers)
                except ValueError:
                    print(f"Skipping unreadable line: {line}")

        # 4. Save the very last matrix after the loop finishes
        if current_dims and current_data:
            rows, cols = current_dims
            mat = np.array(current_data, dtype=float)
            if mat.size == rows * cols:
                matrices.append(mat.reshape(rows, cols))

    return matrices

def plot_all_matrices(matrices):
    """
    Takes a list of 2D numpy arrays and plots them.
    """
    print(f"Found {len(matrices)} matrices.")

    for i, matrix in enumerate(matrices):
        rows, cols = matrix.shape
        
        # Setup Figure
        fig, ax = plt.subplots(figsize=(6, 5))
        
        # Color Map: 'seismic' or 'bwr' is good for correlations (-1 to 1)
        # vmin/vmax ensures 0 is white
        limit = np.max(np.abs(matrix)) 
        if limit == 0: limit = 1.0
        
        im = ax.imshow(matrix, cmap='seismic', vmin=-limit, vmax=limit, origin='upper')
        
        # Styling
        fig.colorbar(im, ax=ax, label='Correlation')
        ax.set_title(f"System Size: {rows}x{cols}")
        ax.set_xlabel("Site j")
        ax.set_ylabel("Site i")
        
        # Save each plot uniquely
        # savename = f"correlation_plot_{i}_size_{rows}.png"
        # plt.savefig(savename)
        # print(f"Saved {savename}")
        plt.show()
        plt.close(fig) # Close memory

def plot_energy_spectrum(filename):

    all_energies = parse_multi_vector_file(filename)

    if all_energies:
        plot_all_energies(all_energies)
    else:
        print("No energies found")

def parse_multi_vector_file(filename):
    """
    Parses a file containing multiple 1D vectors stacked.
    Format expected:
    # N 
    data
    data
    ...
    """
    matrices = []
    expected_size = None
    current_data = []
    
    # CHANGED: Regex now looks for a '#' followed by just ONE integer
    header_pattern = re.compile(r'^#\s+(\d+)')

    if not os.path.exists(filename):
        print(f"Error: File '{filename}' not found.")
        return []

    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line: continue 

            # 1. Check if the line is a header like "# 14"
            match = header_pattern.match(line)
            
            if match:
                # Save previous data block if it exists
                if current_data:
                    mat = np.array(current_data, dtype=float)
                    
                    # Optional safety check
                    if expected_size and mat.size != expected_size:
                        print(f"Warning: Expected size {expected_size} but read {mat.size} elements.")
                    
                    matrices.append(mat)
                
                # Start new block
                expected_size = int(match.group(1))
                current_data = [] 
                
            # 2. Skip any other generic comments
            elif line.startswith('#'):
                continue 
                
            # 3. Read the float data
            else:
                parts = line.split()
                try:
                    current_data.extend([float(x) for x in parts])
                except ValueError:
                    continue

        # Save the very last block after reaching the end of the file
        if current_data:
            mat = np.array(current_data, dtype=float)
            matrices.append(mat)

    return matrices

def plot_all_energies(matrices):
    if not matrices:
        print("No data to plot.")
        return

    plt.figure(figsize=(10, 7))
    
    print(f"Found {len(matrices)} system sizes. Plotting...")

    # Loop through each loaded eigenvalue vector
    for mat in matrices:
        # The system size N is just the total number of eigenvalues in this block
        N = mat.size 
        energies = mat.flatten() # Ensure it's a flat 1D list
        
        # Create an array of size N filled with the value N. 
        # (e.g., if N=13, x_vals = [13, 13, 13, ..., 13])
        x_vals = np.full(N, N)
        
        # Scatter plot for this specific system size
        # s=4 is the dot size, alpha=0.7 makes dense areas look darker
        plt.scatter(x_vals, energies, color='black', s=4, alpha=0.7, marker='.')

    # Styling
    plt.title("Energy Spectrum")
    plt.xlabel("System Size ($N$)")
    plt.ylabel("Energy Levels ($E_k$)")
    
    # Fibonacci sizes grow exponentially (8, 13, 21, 34...), 
    # so a Log-scale on the X-axis spaces them out perfectly evenly!
    plt.xscale('log') 
    
    # Draw lines at the Fibonacci N values for reference
    plt.grid(True, which='both', axis='x', linestyle='--', alpha=0.4)
    plt.grid(True, which='major', axis='y', linestyle='-', alpha=0.2)

    # Save and show
    plt.savefig("fibonacci_spectrum.png", dpi=300)
    print("Plot saved to fibonacci_spectrum.png")
    plt.show()

def plot_two_point_correlation(filename):
    try:
        # Load data
        correlation = np.loadtxt(filename)
        N = correlation.shape[0] // 2 # Since we only have N/2 values in the file
        two_point = np.abs(correlation[N-1][N:N*2-0:2]) # Extract the N/2-th row (0-indexed), and every 2nd element
        # two_point = np.abs(correlation[N-1][N:N*2-0]) # Extract the N/2-th row (0-indexed)


        # Plot and stylize
        plt.figure(figsize=(8,6))
        plt.loglog(np.arange(1, N + 1,2), two_point, "green", label="Two-Point Correlation", markersize=5) # PLot every 2nd element
        # plt.loglog(np.arange(1, N + 1), two_point, "green", label="Two-Point Correlation", markersize=5)
        plt.title("Two-Point Correlation Function")
        plt.xlabel("Distance $r$")
        plt.ylabel("Correlation $C(r)$")
        plt.grid(True, which="both", linestyle="--", alpha=0.6)
        plt.legend()
        plt.show()

    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

def plot_ipr(filename):
    try:
        # Load data
        ipr = np.loadtxt(filename)
        N = ipr.size  
        print(f"Loaded IPR vector of size: {ipr.size}")
        # print(ipr)
        ipr_values = ipr[::1] 

        # Filter Data, only keep points where Gap > 0
        mask = ipr_values > 1e-60 
        N_clean = np.arange(1, N + 1)[mask]
        ipr_clean = ipr_values[mask]

        # Plot and stylize
        plt.figure(figsize=(8,6))
        plt.loglog(N_clean, ipr_clean, "purple", label="Inverse Participation Ratio", markersize=5) # PLot every 2nd element
        plt.title("Inverse Participation Ratio")
        plt.xlabel("System Size $N$")
        plt.ylabel("$IPR(N)$")
        plt.grid(True, which="both", linestyle="--", alpha=0.6)
        plt.legend()
        plt.show()

    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

def plot_ground_state(filename):
    try:
        # Load data
        eigenvectors = np.loadtxt(filename)
        ground_state = eigenvectors[:][0] # First column is the ground state
        N = ground_state.size  
        print(f"Loaded Ground State vector of size: {N}")

        probability_density = np.abs(ground_state)**2

        # Plot and stylize
        plt.figure(figsize=(8,6))
        plt.plot(np.arange(1, N + 1), probability_density, "orange", label="Ground State Amplitude", markersize=5) 
        plt.title("Ground State Density Matrix")
        plt.xlabel("Site Index")
        plt.ylabel("Probability Density $|\psi_0(i)|^2$")
        plt.grid(True, which="both", linestyle="--", alpha=0.6)
        plt.legend()
        plt.show()

    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

def plot_gap_ratio(filename1,filename2):
    try:
        # Load data
        gap_energy = np.loadtxt(filename1)
        gap_ratio = np.loadtxt(filename2)
        N = gap_ratio.size  
        print(f"Loaded Gap Ratio vector of size: {N}")

        # Plot and stylize
        plt.figure(figsize=(8,6))
        plt.plot(np.arange(1, N + 1), gap_ratio, "cyan", label="Gap Ratio", markersize=5) 
        plt.plot(np.arange(1, N + 1), gap_energy, "magenta", label="Energy Gap)", markersize=5) 

        # Plot theoretical Poisson distribution: P(r) = 2 / (1+r)^2
        # r_theory = np.linspace(0, N, 100*N) # More points for smoother curve
        # p_poisson = 2.0 / (1.0 + r_theory)**2
        # plt.plot(r_theory, p_poisson, 'k--', linewidth=2, label='Poisson Theory (Localized, $\\langle r \\rangle \\approx 0.386$)')
        plt.title("Energy Gap Ratio")
        plt.xlabel("System Size $N$")
        plt.ylabel("Gap Ratio $r(N)$")
        plt.grid(True, which="both", linestyle="--", alpha=0.6)
        plt.legend()
        plt.show()

    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

def plot_histogram_gap_ratio(filename):
    try:
        # Load data
        r_vals = np.loadtxt(filename)
        r_mean = np.mean(r_vals)
        N = r_vals.size

        print(f"Loaded Gap Ratio vector of size: {N}")


        # Plotting
        plt.figure(figsize=(8, 6))
        
        # Plot the histogram of our data
        counts, bins, patches = plt.hist(r_vals, bins=30, density=True, alpha=0.7, color='purple', edgecolor='black', label=f'Data ($\\langle r \\rangle$ = {r_mean:.3f})')
        
        # Plot theoretical Poisson distribution: P(r) = 2 / (1+r)^2
        r_theory = np.linspace(0, 1, 100)
        p_poisson = 2.0 / (1.0 + r_theory)**2
        # plt.plot(r_theory, p_poisson, 'k--', linewidth=2, label='Poisson Theory (Localized, $\\langle r \\rangle \\approx 0.386$)')

        plt.title(f"Level Spacing Statistics (Gap Ratio) for N={N}")
        plt.xlabel("Gap Ratio $r$")
        plt.ylabel("Probability Density $P(r)$")
        plt.legend()
        plt.grid(True, alpha=0.3)
        
        plt.savefig(f"level_statistics_N{N}.png", dpi=300)
        plt.show()

    except Exception as e:
        print(f"Error reading file: {e}")
        exit()

if __name__ == "__main__":

    plot_correlation_matrix("correlation.txt")
    plot_ground_state("eigenvectors.txt")
    plot_gap_ratio("energy_gap_vs_site.txt","energy_gap_ratio_vs_site.txt")
    plot_histogram_gap_ratio("energy_gap_ratio_vs_site.txt")
    plot_energy_gap("energy_gap.txt")
    plot_energy_gap("energy_gap_s.txt")
    plot_entanglement_entropy("entropy.txt")
    plot_ipr("inverse_participation_ratio.txt")
    plot_two_point_correlation("correlation.txt")

    plot_energy_spectrum("eigenvalues.txt")

    plot_correlations("correlation_f.txt")


