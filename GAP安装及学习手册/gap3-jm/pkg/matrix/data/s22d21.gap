gens := [[
[0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2],
[0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0],
[2,1,2,1,2,1,1,1,2,2,2,0,0,1,0,1,0,2,0,0,2],
[1,2,0,0,0,0,0,1,0,2,1,2,0,2,1,1,0,0,2,0,2],
[1,2,0,0,0,0,1,1,2,2,1,0,0,2,0,0,0,0,0,2,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0]
]*Z(3),
[
[0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0],
[0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0],
[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0],
[0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0],
[1,1,2,2,1,0,2,1,1,2,0,0,2,0,1,2,1,2,2,2,2],
[0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0],
[1,0,1,2,2,1,2,0,0,0,0,1,2,1,1,1,0,0,2,2,1],
[0,0,2,1,1,1,0,0,2,1,0,2,2,0,2,2,2,0,2,0,2],
[0,2,0,2,2,2,1,1,0,1,0,0,1,2,0,1,2,2,2,0,0]
]*Z(3)];
G := Group (gens, gens[1]^0);
