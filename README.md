# HGPolyVis

HGPolyVis is a desktop application for visualizing hypergraph data using the polygon visualization metaphor. It contains implementations of the algorithms presented in the following two papers:

> P. Oliver, E. Zhang and Y. Zhang. Scalable Hypergraph Visualization. *IEEE Transactions on Visualization and Computer Graphics*, 30(1):595-605, 2022. doi: 10.1109/TVCG.2023.3326599. arxiv: https://arxiv.org/abs/2308.05043.

> P. Oliver, E. Zhang, and Y. Zhang. Structure-Aware Simplification for Hypergraph Visualization. Accepted. *To appear in IEEE Transactions on Visualization and Computer Graphics*. 2024. arxiv: https://arxiv.org/abs/2407.19621.

The executable `Deploy/HGPolyVis.exe` is built for Windows 11 and is available under the [Apache License, Version 2.0](https://www.apache.org/licenses/LICENSE-2.0). HGPolyVis is not currently available for any other operating system. To run the application, download or clone this repository and run the executable. 

To open a dataset from the `Data/PaperAuthor_DBLP` folder in HGPolyVis, select `file -> Open Entity-Relationship Data`. The ".er" file format stores vertex (entity) positions as well as hyperedge (relationship) colors and can be used to save polygon visualization layouts.

CSV hypergraph data can also be opened by selecting `file -> Open CSV Data`. CSV files should have the following format:

```
hyperedge_1, vertex_1a, vertex_1b, vertex_1c, ...
hyperedge_2, vertex_2a, vertex_2b, vertex_2c, ...
hyperedge_3, vertex_3a, vertex_3b, vertex_3c, ...
...
```