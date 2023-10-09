# Djkstra-in-airline-task-solving
About Djkstra ......

The algorithm is designed to read data from a CSV file and store it in a structured variable. It utilizes linked lists to store data and construct a graph-like structure where locations are interconnected.

To address certain storage-related issues (such as potential data overwriting when multiple routes exist between known locations during traversal), I have implemented some small conditional switches.

In the final shortest path algorithm, I have employed the Dijkstra algorithm tailored for adjacency lists. Additionally, I have implemented switches to modify edge weights.