F := GF (3);
one := F.one;

gens := [
[[2, 1, 2, 1, 2],
[2, 0, 0, 0, 2],
[0, 2, 0, 0, 0],
[0, 1, 2, 0, 1],
[1, 0, 2, 2, 1] 
],
[[2, 1, 0, 2, 1],
[1, 2, 0, 2, 2],
[1, 1, 2, 1, 1],
[0, 2, 0, 1, 1],
[1, 1, 2, 2, 2] 
]] * one;

G := Group (gens, gens[1]^0);

