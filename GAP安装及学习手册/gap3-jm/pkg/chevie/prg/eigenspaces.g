#############################################################################
##
#A  eigenspaces.g            CHEVIE library             Jean Michel 
##
#Y  Copyright (C) 1999--2014   University Paris VII.
##
# Fonctions relative to maximal eigenspaces in reflection groups or
# reflection cosets

# RelativeDegrees(W[,zeta]) 
# Degrees of N_W(V)/C_W(V) for maximal zeta-eigenspace of [Spets] W 
# zeta is specified as a fraction in Q/Z. An integer d<>0 means 1/d
RelativeDegrees:=function(arg)local W,d,deg,zeta;
  W:=arg[1];if Length(arg)=1 then d:=0;else d:=arg[2];fi;
  if IsInt(d) and d<>0 then d:=Mod1(1/d);fi;
  zeta:=E(Denominator(d))^Numerator(d);
  deg:=ReflectionDegrees(W);
  if IsSpets(W) then return List(Filtered(deg,p->zeta^p[1]=p[2]),p->p[1]);
  else return Filtered(deg,x->zeta^x=1);
  fi;
end;

RegularEigenvalues:=function(W)local c,d,l,res,p,p1,n;
  d:=ReflectionDegrees(W); c:=ReflectionCoDegrees(W);
  if not IsSpets(W) then return Filtered(Union(List(d,DivisorsInt)),
      x->Number(d,i->i mod x=0)=Number(c,i->i mod x=0));fi;
  l:=Union(List(d,p->DivisorsInt(Denominator(AsRootOfUnity(p[2]))*p[1])));
  res:=[];
  for n in l do
    p:=PrimeResidues(n);
    p1:=Filtered(p,i->Number(d,p->E(n)^(i*p[1])=p[2])
		     =Number(c,p->E(n)^(i*p[1])=p[2]));
    if p1=p then Add(res,n);else Append(res,p1/n);fi;
  od;
  return res;
end;

# d root unity as fraction mod 1
PositionRegularClass:=function(arg)local W,d,drank;
  W:=arg[1];
  if Length(arg)=1 then d:=0;else d:=arg[2];fi;
  if IsInt(d) then if d<>0 then d:=1/d;fi;if d=1 then d:=0;fi;fi;
  drank:=Length(RelativeDegrees(W,d));
  if drank=0 then return false;fi;
  return PositionProperty(ReflectionEigenvalues(W),x->Number(x,y->y=d)=drank);
end;

# w-invariant projector on the d-eigenspace of w elt. of Spets or group WF
# d is given as AsRootOfUnity
EigenspaceProjector:=function(WF,w,d)local c,f,root;
  if IsInt(d) then if d<>0 then d:=1/d;fi;if d=1 then d:=0;fi;fi;
  c:=ReflectionEigenvalues(WF,PositionClass(WF,w));
  root:=x->E(Denominator(x))^Numerator(x);
  c:=List(Difference(c,[d]),root);
  f:=MatXPerm(WF,w);
  if Length(c)=0 then return f^0;
  else return Product(c,x->f-f^0*x)/Product(c,x->root(d)-x);
  fi;
end;
  
# W is a PermRootGroup and L a reflection subgroup. w in W is assumed to
# normalize L; returns the matrix by which w acts on X(Z_L)
GetRelativeAction:=function(W,L,w)local m;
  m:=MatXPerm(Parent(W),w);
  if m=[] then return [];fi;
  m:=m^(L.operations.BaseX(L)^-1);
  return m{[L.semisimpleRank+1..W.rank]}{[L.semisimpleRank+1..W.rank]};
end;

# return 'action' of W.reflections[W.rootRestriction[i]] on X(Z_L)
# a record with root, coroot, eigenvalue, index:=i and parentMap
GetRelativeRoot:=function(W,L,i)local J,F,d,m,rc,r,c,N;
  J:=L.rootInclusion{L.generatingReflections};
  N:=Normalizer(ReflectionSubgroup(W,Concatenation(J,[i])),L);
  F:=N/L;
  if not IsCyclic(F) then Error("in theory N/L expected to be cyclic\n");fi;
  d:=Size(F);
  for rc in Filtered(Elements(F),x->Order(F,x)=d) do
    m:=GetRelativeAction(W,L,rc.element.representative);
    r:=AsReflection(m);
    if r=false then Error("I thought this does not happen");fi;
    if r.eigenvalue=E(d) then 
      r.index:=i;
      rc:=Filtered(List(ConjugacyClasses(N),Representative),
          c->GetRelativeAction(W,L,c)=m);
      c:=Set(List(rc,x->PositionClass(W,x)));
      m:=Maximum(List(ReflectionEigenvalues(W){c},x->Number(x,y->y=0)));
      m:=Filtered(c,x->Number(ReflectionEigenvalues(W,x),y->y=0)=m);
      m:=Filtered(rc,x->PositionClass(W,x)in m);
      if ForAny(m,x->OrderPerm(x)=d) then m:=Filtered(m,x->OrderPerm(x)=d);fi;
 #    if Length(m)>1 then Error("more than one 1-regular");fi;
      r.parentMap:=m[1];
 #    Print("{",Size(ConjugacyClass(N,r.parentMap)),"}\c");
      return r;
    fi;
  od;
  Error("no root found for reflection ",i,"\n");
end;

# RelativeGroup(W,J[,indices])  computes  R=N(L_J)/L_J as  a  reflection
# group acting on V=X(Z_L) (assuming it is one).
# R should be generated by  the images W.reflections{indices} in indices
# in GL(V); if not given indices are  taken to be the complement of J in
# W.rootInclusion{W.generatingReflections}; if this fails all roots are tried.
#
#  R is  considered both  as a  quotient with  quotientmap given  by the
#  function R.MappingFromNormalizer, and  a section from R  to N(L_J) is
#  given  by .parentMap=  for  i in  r.relativeIndices  some element  of
#  N_{W_{J union i}}(W_J) where  .relativeIndices= indices of some roots
#  of W not in J
CHEVIE.Cache.RelativeGroups:=true;
PermRootOps.RelativeGroup:=function(arg)
  local W,J,indices,L,r,N,x,res,i,R,l,f,relname;
# Print("PermRootOps.RelativeGroup ",arg," called \n");
  W:=arg[1]; J:=arg[2]; 
  res:=rec(callarg:=IntListToString(J));
  if Length(arg)>2 then
    Append(res.callarg,",");Append(res.callarg,IntListToString(arg[3]));
  fi;
  res:=CHEVIE.GetCached(W,"RelativeGroups",res,x->x.callarg);
  if Length(RecFields(res))>1 then return res;fi;
  L:=ReflectionSubgroup(W,J);
  if Length(J)=0 then
    W.MappingFromNormalizer:=w->PermMatX(W,GetRelativeAction(W,L,w));
    W.relativeIndices:=W.generatingReflections; return W;
  fi;
  if Length(arg)<3 then
    indices:=Filtered(W.rootInclusion{W.generatingReflections},
      x->not Reflection(Parent(W),x) in L);
  else
    indices:=arg[3];
  fi;
  if Set(L.rootInclusion)=Set(W.rootInclusion) then 
    Inherit(res,PermRootGroup([]));res.relativeIndices:=[];
    res.MappingFromNormalizer:=w->();
    return res;
  fi;
  res.MappingFromNormalizer:=w->PermMatX(res,GetRelativeAction(W,L,w));
  res.roots:=[];
  res.simpleCoroots:=[];
  res.relativeIndices:=[];
  res.parentMap:=[];
  for R in Filtered(List(indices,r->GetRelativeRoot(W,L,r)),x->x<>false) do
    if Position(res.roots,R.root)=false or 
       Position(res.roots,R.root)<>Position(res.simpleCoroots,R.coroot)
    then
      Add(res.roots,R.root);
      Add(res.simpleCoroots,R.coroot);
      Add(res.relativeIndices,R.index);
      Add(res.parentMap,R.parentMap);
    fi;
  od;
  N:=Size(Normalizer(W,L))/Size(L);
#  Print(res.roots,res.simpleCoroots,"\n");
  R:=PermRootGroup(res.roots,res.simpleCoroots);
  relname:=SPrint("W_",ReflectionName(W),"(L_",res.callarg,")");
  if N=Size(R) then 
    Inherit(res,R);
#   res.relativeIndices:=res.relativeIndices{
#     List(R.rootInclusion{R.generatingReflections},x->
#       Position(Reflections(Parent(R)),Reflection(Parent(R),x)))};
    InfoChevie(relname,"=",ReflectionName(res),
       "<",Join(res.relativeIndices),">\n");
    return res;
  fi;
  Print("# warning: ",relname,":size ",N," not generated by ",
    IntListToString(indices,"<>")," =>size ",Size(R),"\n#",
    "           trying all other roots\n");
  indices:=Filtered(W.rootInclusion,r->not Reflection(Parent(W),r)
     in W.generators and not Reflection(Parent(W),r) in L);
  for r in indices do 
    l:=GetRelativeRoot(W,L,r);
    if l<>false and not l.root in res.roots then
      R:=PermRootGroup(Concatenation(res.roots,[l.root]),
                       Concatenation(res.simpleCoroots,[l.coroot]));
      if N=Size(R) then
	Add(res.roots,l.root);
	Add(res.simpleCoroots,l.coroot);
	Add(res.relativeIndices,l.index);
	Add(res.parentMap,l.parentMap);
	Inherit(res,R);
#	Print(relname,"=",ReflectionName(res),"<",
#	  IntListToString(res.relativeIndices),"+",r,">\n");
	return res;
#      else Print("N=",N," Size(R)=",Size(R),"\n");
      fi;
    fi;
  od;
  Error("relgroup not found");
end;

# Finds an element w of W which conjugates reflection subgroup H into
# a standard parabolic W_I if any exists
PermRootOps.StandardParabolic:=function(W,H)local wr,I,i,w,l,hr,w1,a,try;
  wr:=W.rootInclusion{W.generatingReflections};
  hr:=H.rootInclusion{H.generatingReflections};
  if IsSubset(wr,hr) then return ();fi;
  I:=Combinations(wr,Length(hr));
  I:=Filtered(I,l->Size(PermRootOps.ReflectionSubgroupNC(W,l))=Size(H));
  try:=function(a)local H1,w,w1,C;
    H1:=H;w:=();
    for i in [1..Length(a)] do
      C:=Centralizer(W,ReflectionSubgroup(W,a{[1..i-1]}));
      w1:=RepresentativeOperation(C,H1.rootInclusion[i],a[i]);
      if w1=false then w1:=RepresentativeOperation(C,H1.(i),
              Reflection(W,W.rootRestriction[a[i]]));
        if w1=false then return false;fi;
      fi;
      w:=w*w1;H1:=H1^w1;
    od;
    return w;
  end;
  for l in I do # InfoChevie("# trying ",l,"\c");
    for a in Arrangements(l,Length(l)) do
      w:=try(a);
      if w<>false then # InfoChevie("\n"); 
                       return w;fi;
    od;
  od;
  InfoChevie("\n# ****** ",hr," not conjugate to a standard parabolic\n");
  return false;
end;

# SplitLevis(W[,d[,ad]]) returns representatives of the d-split levis L 
# (with d-part of ZoL of dimension ad)
SplitLevis:=function(arg)local W,d,eig,ad,cl,w,V,I,HF,res,refs,H,WF,mats,f,P,Ed;
  WF:=arg[1];
  if IsSpets(WF) then W:=Group(WF);else W:=WF;WF:=Spets(W);fi;
  if Length(arg)=1 then arg[2]:=0; fi;
  d:=arg[2];if IsInt(d) and d<>0 then d:=Mod1(1/d);fi;
  Ed:=E(Denominator(d))^Numerator(d);
  if Length(arg)=2 then 
    return Concatenation(List([0..Length(RelativeDegrees(WF,d))],
      ad->SplitLevis(WF,d,ad)));
  fi;
  ad:=arg[3];
  refs:=Filtered([1..Length(W.roots)],
                  i->Position(W.reflections,Reflection(W,i))=i);
  mats:=List(refs,i->MatXPerm(W,Reflection(W,i)));
  eig:=ReflectionEigenvalues(WF);
  cl:=Filtered([1..Length(eig)],j->Number(eig[j],e->d=e)=ad);
  res:=[];
  while Length(cl)>0 do
    w:=Representative(ConjugacyClasses(WF)[cl[1]]);
    if W.rank=0 then V:=[];
    else V:=NullspaceMat(MatXPerm(WF,w)-Ed*IdentityMat(W.rank));#E(d)-eigenspace
    fi;
    I:=ListBlist(refs,List(mats,m->V=V*m));
    HF:=SubSpets(WF,W.rootInclusion{I},w/WF.phi);
    if HF=false then 
      Error("SubSpets(",WF,",",W.rootInclusion{I},",class#",cl[1],") failed\n");
    else
      f:=Intersection(FusionConjugacyClasses(HF,WF),cl);
      if Length(f)=0 then Error("fusion is wrong");return false;fi;
      cl:=Difference(cl,f); # other classes in f give the same subspets
      H:=Group(HF);
      P:=StandardParabolic(W,H);
      if P=false then P:=();fi;
      P:=[P,OnTuples(H.rootInclusion{H.generatingReflections},P)];
      if P[1]<>() or P[2]<>H.rootInclusion{H.generatingReflections} then
	HF:=SubSpets(WF,P[2],HF.phi^P[1]/WF.phi);
      fi;
      Add(res,HF);
    fi;
  od;
  return res;
end;
