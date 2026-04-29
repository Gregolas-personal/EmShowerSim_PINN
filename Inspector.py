# Quick data sanity check in Python
import h5py, numpy as np, matplotlib.pyplot as plt

with h5py.File("output/shower_10GeV_t0.h5", "r") as f:
    layer = np.array(f["/default_ntuples/hits/layer/pages"])
    edep  = np.array(f["/default_ntuples/hits/edep/pages"])
    x   = np.array(f["/default_ntuples/hits/x/pages"])
    y = np.array(f["/default_ntuples/hits/y/pages"])

# Plot longitudinal profile
edep_per_layer = np.zeros(20)
for l in range(20):
    edep_per_layer[l] = edep[layer == l].sum()

plt.bar(range(20), edep_per_layer)
plt.xlabel("Layer (≈0.56 X₀ each)")
plt.ylabel("Energy deposit [MeV]")
plt.title("Longitudinal EM shower profile")
plt.show()

# Plot transverse profile (x vs y with energy deposit as weight)
plt.figure()
plt.scatter(x, y, c=edep, s=edep/edep.max()*50, alpha=0.6, cmap='viridis')
plt.xlabel("x [mm]")
plt.ylabel("y [mm]")
plt.colorbar(label="Energy deposit [MeV]")
plt.title("Transverse EM shower profile")
plt.show()

