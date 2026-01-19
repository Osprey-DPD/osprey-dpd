### Welcome to the official distribution of the Osprey simulation engine.

**Osprey-DPD - Open Source Polymer Research Engine-Dissipative Particle Dynamics** - is free software distributed under the BSD 3-clause license. It was developed by the author at the Max Planck Institute of Colloids and Interfaces, Germany, 1999-2007. You may use the code in any way you wish, modify and redistribute it or embed it in other software, subject to the conditions of the license. See the license file for details. For further information, please contact the author:  jsnano "at" me "." com

The code is written in standard  C++03 and uses the STL. It has run successfully on Linux, Mac OS X and Windows 10 platforms.

The User Guide describes how to compile and run the code, and explains many of the code features.

On Linux platforms, the code is compiled and linked using the GNU C++ compiler with the following two commands executed in the source code directory:

```shell
$ g++ -c -O3 -std=c++11 *.cpp
$ g++ -o dpd *.o -pthread
```

One can also build it using cmake:
```shell
$ mkdir build
$ cd build

# for Release
$ cmake ..
$ make

# for Debug
$ cmake .. -DCMAKE_BUILD_TYPE=Debug
$ make

# or using Ninja
$ cmake .. -GNinja
$ ninja

```

The main() routine is in the file  "dmpc.cpp".

The code is executed by entering its name at the command line and providing a single argument that is the extension of the required text input file that must be named "dmpci.nnn" where "nnn" is a user-defined alphanumeric string. This command
```shell
$ ./dpd  001
```

will invoke the code and cause it to read its input from the file "dmpci.001" that must be located in the current directory. If the specified file is not found, the code creates a default input file that can then be modified as desired. Some simple input files for various cases can be found in the /examples directory.


### Publications using Osprey-DPD

[Macromolecular crowding is surprisingly unable to deform the structure of a model biomolecular condensate,](https://doi.org/10.3390/biology12020181)
J. C. Shillcock, D. B. Thomas, J. H. Ipsen, A. D. Brown, Biology, 2023, 12, 181.


[Model biomolecular condensates have heterogeneous structure quantitatively dependent on the interaction profile of their constituent macromolecules,](https://doi.org/10.1039/D2SM00387B)
J. C. Shillcock, C. Lagisquet, J. Alexandre, L. Vuillon, J. H. Ipsen, Soft Matter, 2022, 18, 6674-6693.

[Investigating the morphological transitions in an associative surfactant ternary system,](https://pubs.rsc.org/en/content/articlelanding/2022/sm/d1sm01668g)
H. Honaryar, J. A. LaNasa, R. J. Hickey, J. C. Shillcock, Z. Niroobakhsh, Soft Matter, 2022, 18:2611-2633.

[Non-monotonic fibril surface occlusion by GFP tags from coase-grained molecular simulations,](https://www.sciencedirect.com/science/article/pii/S2001037021005262?via%3Dihub)
J. C. Shillcock, J. Hastings, N. Riguet, H. A. Lashuel, Computational and Structural Biotechnology Journal, 2022, 20, 309-321.

[Coupling bulk phase separataion of disordered proteins to membrane domain formation in molecular simulations on a bespoke compute fabric,](https://www.mdpi.com/2077-0375/12/1/17/htm)
J. C. Shillcock, D. B. Thomas, J. R. Beaumont, G. M. Bragg, M. L. Vousden, A. D. Brown, Membranes, 2022, 12, 17

[Phase behaviour and structure of a model biomolecular condensate,](https://pubs.rsc.org/en/content/articlelanding/2020/SM/D0SM00813C#!divAbstract)
J. C. Shillcock, M. Brochut, E. Chenais, J. H. Ipsen, Soft Matter, 2020, 16, 6413.

---

## Helix mode: i↔i+4 contacts + coarse-grained H-bond stabilization
`Last modified: Jan 19, 2026`

Osprey-DPD now includes an optional helix-capable polymer mode designed to capture α-helix–like structure in a coarse-grained setting. This integrates two parts: a helix metric (for analysis/monitoring) and intrachain stabilizing interactions (for dynamics).

### Helix metric (structure detection)

We define a helical contact using the classic i↔i+4 criterion:

* Two helix-forming beads separated by **four bonds** are considered “in helical contact” if their separation is **below `0.85 Rc`**.
* Along the helix-forming segment, we scan forward bead-by-bead and track continuous runs of i↔i+4 contacts.
* With 4 beads per turn, the helix length increases in quarter-turn increments as contacts persist. A helix segment consisting of **m full turns** is reported with length **`m − 1`**.

This provides a direct, interpretable measure of helix formation and persistence over time.

### Simulating H-bonds (intrachain stabilization)

To make helices not only *detectable* but also *physically stable*, we added **intrachain “H-bond-like” interactions** on the helix-forming beads:

* Standard backbone bonding remains unchanged (i↔i+1 harmonic).
* A **secondary harmonic constraint** is included for helix beads at **i↔i+2** (often described as a “1–3” harmonic), which helps reduce excessive torsional freedom and guides the backbone toward helical geometry.
* Two **Morse interactions** are available to model stabilizing contacts:

  * a Morse term on i↔i+2
  * a Morse term on i↔i+4 (often referred to as “1–5” in 1-based indexing)

Each Morse interaction is parameterized by an equilibrium distance, width, well depth, and cutoff (`re`, `α`, `KM`, `rM`), so the strength and range of stabilization can be tuned.

### Scope and assumptions (important)

* These forces apply **only within a single polymer** of type **Helix** and only to the designated helix-forming bead type (e.g., “S beads”). No helix–helix coupling occurs between different polymers.
* End effects are handled naturally: a bead only “projects” interactions forward if the partner bead exists and is also a helix bead. As a result, the first/last helix beads experience fewer helix interactions (no artificial wrap-around).

### Tuning parameters

Helix/Morse parameters are currently hard-coded (by design, for simplicity and reproducibility). If you want to explore different helix strengths, you can modify the constants in the helix force implementation and recompile.

### Implementation note

Most of the helix/H-bond integration is implemented in `src/Polymer.cpp`, primarily in the method `CPolymer::AddHelixForces()`, where the helix-forming bead selection and the additional helix-stabilizing interactions are applied.





