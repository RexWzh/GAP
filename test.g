# 定义商群
G := FreeGroup(2); # 初始化自由群
gen := GeneratorsOfGroup(G); # 生成元
L:=[[2,3],[3,2]];

K := []; # kernal
for i in [1..2] do  #第i行
    for j in [1..2] do #第j列
    Add(K,(gen[i]*gen[j])^L[i,j]);
od;
od;
#PPrint(K); #调试，检验生成元
GK := G/K;

# 取长度前 k
k := 2;
S := [One(GK)]; # 长为 0
gens := GeneratorsOfGroup(GK); 
S_last := gens; # 长为 1
while k > 0 do 
    k := k - 1;
    for a in gens do
        if a in S then
            continue;
        fi;
        Add(S,a);
    od;
od;


# 编写函数
ListIf := function(list,value,chk)
    ## 模仿 Python 的列表生成器 ##
    local new,i;
    new := [];
    for i in list do
        if chk(i) then 
            Add(new,value(i));
        fi;
    od;
    return new;
end;

GroupTreeDepth:=function(G,k)
    # 群树，获取前 k 层
    local gens,S,S_last,a,b,c,tree;
    if k=0 then
        return [[One(G)]];
    fi;
    # 生成元
    gens := GeneratorsOfGroup(G);
    S := [One(G)];
    S_last := [];
    for a in gens do
        if not a in S then
            Add(S,a);
            Add(S_last,a);
        fi;
    od;
    tree := [[One(G)],S_last];
    if k=1 then
        return tree;
    fi;
    gens := S_last; # 简化的生成元

    # 达到层数，或最后一层为空时，终止
    while k > 0 and Size(S_last) <> 0 do 
        k := k - 1;
        S_last := [];
        for a in tree[Size(tree)] do
            for b in gens do
                c := a * b;
                if not c in S then
                    Add(S,c);
                    Add(S_last,c);
                fi;
            od;
        od;
        Add(tree,S_last); # 可能为空
    od;
    return tree;
end;
        

# 定义商群
G := FreeGroup(3); # 初始化自由群
gen := GeneratorsOfGroup(G); # 生成元
L:=[[3,3,3],[3,3,3],[3,3,3]];

K := List(gen,x->x^3); # kernal
for i in [1..3] do  #第i行
    for j in [1..3] do #第j列
    Add(K,(gen[i]*gen[j])^L[i,j]);
od;
od;
GK := G/K;


