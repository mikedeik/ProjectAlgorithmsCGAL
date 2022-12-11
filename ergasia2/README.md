Mihalis Deiktakis - sdi0800018
Ioannis Psarros - sdi1800216

Gia build prepei na prostethi sto find CMakeLists.txt to path tou installation tis CGAL .
Meta me cmake kanoyme build to makefile. (cmake -S .)
Meta me make ginete build kai to executable name einai ./optimal_polygon

Exoun ylopoiithei oi algorithmoi local_search kai simulated_annealing

Ta arguments einai ta parakatw:

** Arguments should be: **

1. -i <point set input file>
2. -o <output file>
3. -algorithm <local_search or simulated_annealing>
4. -L <L parameter according to algorithm>
5. -max or -min [depending on Polygon Area Target]
6. -threshold <double> OR -annealing <local or global or subdivision> [depending on algorithm]

tha prepei na dwthoun me auti ti seira px (./optimal_polygon -i data/images/euro-night-0001000.instance -o out.txt -algorithm simulated_annealing -L 5000 -min -annealing local)
(./optimal_polygon -i data/images/euro-night-0001000.instance -o out.txt -algorithm local_search -L 5 -min -threshold 0.01)

Genika paratiroume pws o SA algorithmos einai taxyteros apo ton local search to opoio einai logiko afou o local search kanei eksantlitiki anazitisi kai gia polapla paths gia oles tis pithanes ekdoxes me
complexity L\*n^3 oso den exoume ftasei sto katwfli threshold.

Episis paratiroume pws to local step ston SA einai asfalws poly taxytero apo to global logo tou KD-Tree kai tis anazitisis mono sto orthogonio metaksu twn tessarwn simeiwn pou anazitoume.

To subdivision exei ylopoiithei plirws omws yparxei ena bug opou den pairnoume swsta tis akmes prin kai meta apo ta koina simeia q metaksu twn Subsets se merikes periptwseis.

Genika vlepoume poly kalytera apotelesmata ston SA ws pros tin taxythta Me ton global na kanei sxedon dekaplasious xronous apo ton local omws meiwnei simantika to ratio se minimization kai to
auksanei antistoixa se maximization.
Antistoixa gia megalutera simeiosunola kalutera apotelesmata vriskei o subdivision afou spaei tin poluplokotita semikrotera upoprovlimata.
