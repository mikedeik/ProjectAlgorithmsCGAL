Mihalis Deiktakis - sdi0800018
Ioannis Psarros - sdi1800216

Gia build prepei na prostethi sto find CMakeLists.txt to path tou installation tis CGAL .
Meta me cmake kanoyme build to makefile. (cmake -S .)
Meta me make ginete build kai to executable name einai ./optimal_polygon

Ta arguments einai ta parakatw:

** Arguments should be: **

1. -i <point set path>
2. -o <output file>
3. -preprocess <optional>

tha prepei na dwthoun me auti ti seira px (./evaluate -i data/images -o out.txt )

Paratiroume pws o SA algorithmos einai taxyteros apo ton local search to opoio einai logiko afou o local search kanei eksantlitiki anazitisi kai gia polapla paths gia oles tis pithanes ekdoxes me complexity L\*n^3 oso den exoume ftasei sto katwfli threshold.

Paratiroume pws to local step ston SA einai asfalws poly taxytero apo to global logo tou KD-Tree kai tis anazitisis mono sto orthogonio metaksu twn tessarwn simeiwn pou anazitoume.

Paratiroume pws o Local Search mas dinei kalitero accuracy , diladi to ratio emvadou se sxesi me to emvado tou KP dioti xrisimopoioume poli mikro threshold kai einai eksantlitikos algorithmos.

Paratiroume pws o Auksitikos Algorithmos einai o grigoroteros apo olous omos dinei mikrotero accuracy apo to Local Search kai ton Simulated Annealing. Stin periptosi mas einai grigorotero apo ton Algorithmo me Vasi To KP dioti eixei ilopoioithei mono me tixaies akmes .

Telos paratiroume pws o Algorithmos me vasi to KP einai o pio argos kai dinei to xeirotero accuracy apo olous tous algorithmous. 


Genika vlepoume poly kalytera apotelesmata ston SA ws pros tin taxythta Me ton global na kanei sxedon dekaplasious xronous apo ton local omws meiwnei simantika to ratio se minimization kai to auksanei antistoixa se maximization.
Antistoixa gia megalutera simeiosunola kalutera apotelesmata vriskei o subdivision afou spaei tin poluplokotita semikrotera upoprovlimata.
