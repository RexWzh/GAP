gens:=[
[[ 1, 0, 0, 0],
[ 0, 4, 0, 0],
[ 2, 0, 2, 3],
[ 3, 0, 4, 3]],

[[ 4, 0, 0, 1],
[ 2, 4, 4, 0],
[ 1, 0, 1, 2],
[ 0, 0, 0, 1]],

[[ 4, 0, 1, 1],
[ 0, 1, 0, 0],
[ 0, 1, 3, 4],
[ 0, 4, 3, 2]],

[[ 2, 0, 4, 3],
[ 4, 4, 2, 4],
[ 0, 1, 3, 4],
[ 4, 2, 0, 1]],

[[ 1, 1, 3, 4],
[ 0, 0, 3, 4],
[ 2, 0, 0, 4],
[ 3, 1, 3, 4]],

[[ 2, 0, 0, 0],
[ 0, 2, 0, 0],
[ 0, 0, 2, 0],
[ 0, 0, 0, 2]]] * Z(5)^0;
G:=Group(gens,gens[1]^0);
