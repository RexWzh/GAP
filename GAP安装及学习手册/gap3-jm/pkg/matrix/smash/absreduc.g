#############################################################################
##
#A  Matrix package                                      Derek Holt
#A                                                      Charles Leedham-Green
#A                                                      Eamonn O'Brien
#A                                                      Sarah Rees 
##
#A  @ (#)$Id: absreduc.g,v 1.1 1997/03/10 13:52:21 gap Exp $
##
#Y  Copyright (C)  1996,  Lehrstuhl D fuer Mathematik,  RWTH Aachen,  Germany
##
#H  $Log: absreduc.g,v $
#H  Revision 1.1  1997/03/10 13:52:21  gap
#H  VERSION 1.0
#H
#H  Revision 1.2  1997/01/05 10:49:16  fceller
#H  added Eamonn's new version to the reprository
#H
#H  Revision 1.1  1996/12/25 09:03:21  fceller
#H  changed long filenames to MS-DOS conform filenames,
#H  the init files are *NOT* yet updated
#H
#H  Revision 1.3  1996/12/22 07:48:03  fceller
#H  new smash version
#H
#H  Revision 1.2  1996/12/12 10:51:21  fceller
#H  new version by Eamonn
#H
#H  Revision 1.1  1996/11/28 13:14:38  fceller
#H  added "smash" and "reducible" to the repository
#H
##
#absirreducible.g
#
############################################################################
##
#F  InfoMeataxe (...)  . . . . . . . . . . . for debugging assistance
##
##
if not IsBound (InfoMeataxe)  then InfoMeataxe := Ignore;  fi;
#############################################################################
##
#F  FrobeniusAction (A, v [, basis]) . . action of matrix A on
##                                  . . Frobenius block of vector v
##
## FrobeniusAction (A, v) computes the Frobenius block of the dxd matrix A
## generated by the length - d vector v, and returns it.
## It is based on code of MinPolCoeffsMat.
## The optional third argument is for returning the basis for this block.
##
FrobeniusAction := function ( arg )

   local   L, d, p, M, one, zero, R, h, v, w, i, j, nd, ans, 
           A, v, basis;

   if Number (arg) = 2  then
      A := arg[1];
      v := arg[2];
      basis := 0;
   elif Number (arg) = 3  then
      A := arg[1];
      v := arg[2];
      basis := arg[3];
   else
      return Error ("Usage: FrobeniusAction ( <A>, <v>, [, <basis>] )");
   fi;
   one := A[1][1]^0;
   zero := 0 * one;
   d := Length ( A );
   M := 0 * A[1];
   Add ( M, M[1] );

   # L[i] (length d) will contain a vector with head entry 1 at position i, 
   # which is in the current block.
   # R[i] (length d + 1 but (d + 1) - entry always 0) is vector expressing
   # L[i] in terms of the basis of the block.
   L := [];
   R := [];

   # <j> - 1 gives the power of <A> we are looking at
   j := 1;

   # spin vector around and construct polynomial
   repeat

      # compute the head of <v>
      h := 1;
      while v[h] = zero  do
         h := h + 1;
      od;

      # start with appropriate polynomial x^(<j> - 1)
      p := ShallowCopy ( M );
      p[j] := one;

      # divide by known left sides
      w := v;
      while h <= d and IsBound ( L[h] ) do
         p := p - w[h] * R[h];
         w := w - w[h] * L[h];
         while h <= d and w[h] = zero do
            h := h + 1;
         od;
      od;

      # if <v> is not the zero vector try next power
      if h <= d  then
         if (basis <> 0) then basis[j] := Copy (v); fi;
         R[h] := p * w[h]^-1;
         L[h] := w * w[h]^-1;
         j := j + 1;
         v := v * A;
      fi;
   until h > d;

   nd := Length (p);
   while 0 < nd  and p[nd] = zero  do
      nd := nd - 1;
   od;
   nd := nd - 1;
   ans := [];
   for i in [1..nd - 1] do
      ans[i] := [];
      for j in [1..nd] do ans[i][j] := zero; od;
      ans[i][i + 1] := one;
   od;
   ans[nd] := [];
   for j in [1..nd] do
      ans[nd][j] :=  - p[j];
   od;

   return ans;
end;

#############################################################################
##
#F CompleteBasis ( matrices, basis ) . . complete a basis under a group action
##
##  CompleteBasis ( matrices, basis ) takes the partial basis 'basis' of the
##  underlying space of the (irreducible) module defined by matrices, and
##  attempts to extend it to a complete basis which is a direct sum of
##  translates of the original subspace under group elements. It returns
##  true or false according to whether it succeeds.
##  It is called by IsAbsolutelyIrreducible ()
## 
CompleteBasis := function ( matrices, basis )

   local  L, d, subd, subd0, zero, h, v, w, i, j, bno, gno, vno, newb, ngens;

   subd := Length (basis);
   subd0 := subd;
   d := Length ( basis[1] );
   if d = subd then
      return true;
   fi;
   # L is list of normalized generators of the subspace spanned by basis.
   L := [];
   zero := 0 * basis[1][1];
   ngens := Length (matrices);

   #First find normalized generators for subspace itself.
   for i in [1..subd] do
      v := basis[i];
      h := 1;
      while v[h] = zero  do
         h := h + 1;
      od;
      w := v;
      while h <= d and IsBound ( L[h] )  do
         w := w - w[h] * L[h];
         while h <= d and w[h] = zero  do
            h := h + 1;
         od;
      od;
      if h <= d then
         L[h] := w * w[h]^-1;
      else 
         return Error ("Initial vectors are not linearly independent.");
      fi;
   od;

   #Now start translating
   bno := 1; gno := 1; vno := 1;
   while subd < d do
      #translate vector vno of block bno by generator gno
      v :=  basis[ (bno - 1) * subd0 + vno] * matrices[gno];
      h := 1;
      while v[h] = zero  do
         h := h + 1;
      od;
      w := v;
      while h <= d and IsBound ( L[h] )  do
         w := w - w[h] * L[h];
         while h <= d and w[h] = zero  do
            h := h + 1;
         od;
      od;
      if (h <= d) then
         #new generator (and block)
         if vno = 1 then
            newb := true;
         elif newb = false then
            return false;
         fi;
         L[h] := w * w[h]^-1;
         subd := subd + 1;
         basis[subd] := v;
      else
         #in existing subspace
         if vno = 1 then
            newb := false;
         elif newb = true then
            return false;
         fi;
      fi;
      vno := vno + 1;
      if vno > subd0 then
         vno := 1;
         gno := gno + 1;
         if gno > ngens then
            gno := 1;
            bno := bno + 1;
         fi;
      fi;
   od;

   return true;
end;

#############################################################################
##
#F IsAbsolutelyIrreducible ( module ) . . 
## decide if irreducible module over a finite field is absolutely irreducible
##
## IsAbsolutelyIrreducible () decides whether module is absolutely irreducible
## and returns true or false accordingly.
## The function calculates the centralizer of the module.
## The centralizer should be isomorphic to the multiplicative 
## group of the field GF (q^e) for some e, or rather to the group of 
## dim/e x dim/e scalar matrices over GF (q^e), or equivalently, 
## dim x dim matrices composed of identical e x e blocks along the diagonal.
##  e = 1 <=> the module is absolutely irreducible.
## The .fieldExtDeg component is set to e during the function call.
## The function shouldn't be called if the module has not already been
## shown to be irreducible, using IsIrreducible. 
## 
Smash.IsAbsolutelyIrreducible := function ( module )

   local dim, ndim, gcd, div, e, ct, F, q, ok, 
         M, v, M0, v0, C, C0, centmat, one, zero, 
         pow, matrices, newmatrices, looking, 
         basisN, basisB, basisBN, P, Pinv, i, j, k, nblocks; 

   if IsGModule (module) = false then
      return Error ("Argument of IsAbsolutelyIrreducible is not a module.");
   fi;

   if AbsolutelyReducibleFlag (module) <> "unknown" then 
      return not AbsolutelyReducibleFlag (module);
   fi;

   #first decide if the module acts irreducibly 
   if IsIrreducible (module) = false then
      InfoMeataxe ("#I GModule is not irreducible.");
      return false;
   fi;

   dim := DimensionFlag (module);
   F := FieldFlag (module);
   q := Size (F);
   matrices := GeneratorsFlag (module);

   # M acts irreducibly on N, which is canonically defined with respect to M
   # as the nullspace of fac (M), where fac is a factor of the char poly of M.
   # ndim is the dimension of N, and v is a vector of N. All these come from
   # the irreducibility test for the module.
   # An element of the centralizer must centralize every element, 
   # and therefore M must preserve N, since N is canonically defined wrt M. 
   # Our plan is therefore first to find an element which centralizes the 
   # restriction of M to N, and then extend it to the whole space. 

   M := AlgElMatFlag (module);
   ndim := AlgElNullspaceDimensionFlag (module);
   v :=  AlgElNullspaceVecFlag (module);
   # e will have to divide both dim and ndim, and hence their gcd.
   gcd := GcdInt (dim, ndim);
   InfoMeataxe ("#I GCD of module and nullspace dimensions = ", gcd, ".\n");
   if gcd = 1 then
      SetDegreeFieldExtFlag (module, 1);
      SetAbsolutelyReducibleFlag (module, false);
      return true;
   fi;
   div := DivisorsInt (gcd);

   # It's easy to find elements in the centralizer of an element in Frobenius 
   # (= rational canonical) form (centralizing elements are defined by their 
   # action on the first basis element).
   # M0 is the Frobenius form for the action of M on N.
   # basisN is set by the function FrobeniusAction to be the 
   # basis v, vM, vM^2, .. for N

   basisN := [];
   InfoMeataxe (
   "Calculate Frobenius action of element from group algebra on nullspace.\n");
   M0 := FrobeniusAction (M, v, basisN);

   zero := Zero (F); one := One (F);
   v0 := zero * M0[1]; v0[1] := one;

   # v0 is just the vector (1, 0, 0....0) of length ndim. It has nothing
   # in particular to do with M0[1], but multiplying a vector that happens 
   # to be around by 0 is a good way to get a zero vector of the right length. 

   # we try all possible divisors of gcd (biggest first) as possibilities for e
   # We're looking for a centralizing element with order dividing q^e - 1, and
   # blocks size e on N. 
   for ct in Reversed ([2..Length (div)]) do
      e := div[ct];
      InfoMeataxe ("#I Trying dimension ", e, " for centralising field.\n");
      # if ndim = e, M0 will do. 
      if ndim > e then
         C := M0;
         # Take smallest power of C guaranteed to have order dividing q^e - 1, 
         # and try that.
         pow := (q^ndim - 1)/ (q^e - 1);
         InfoMeataxe ("#I Looking for a suitable centralising element.\n");
         repeat
            # The first time through the loop C is M0, 
            # otherwise we choose C at random from the centralizer of M0. 
            # Since M0 is in Frobenius form any centralising element is 
            # determined by its top row (which may be anything but
            # the zero vector).

            if C = [] then
               C[1] := [];
               repeat
                  ok := 0;
                  for i in [1..ndim] do 
                     C[1][i] := Random (F); 
                     if C[1][i] <> zero then  ok := 1; fi;
                  od;
               until ok=1;
               for i in [2..ndim] do C[i] := C[i - 1] * M0; od; 
            fi;
            # C0 is the Frobenius form for the action of this power on 
            # one of its blocks, B (all blocks have the same size).
            # basisBN will be set to be a basis for B, in terms of the 
            # elements of basisN. A matrix product gives us the basis 
            # for B in terms of the original basis for the module.
            basisBN := [];
            C0 := FrobeniusAction (C^pow, v0, basisBN);
            C := [];
         until Length (C0) = e;
         InfoMeataxe ("#I Found one.\n");
         basisB := basisBN * basisN;
      else
         C0 := M0;
         basisB := ShallowCopy (basisN);
      fi;
      # Now try to extend basisB to a basis for the whole module, 
      # by translating it by the generating matrices.
      P := basisB;
      InfoMeataxe ("#I Trying to extend basis to whole module.\n");
      if CompleteBasis (matrices, P) then
         # We succeeded in extending the basis (might not have done).
         # So now we have a full basis, which we think of now as a 
         # base change matrix.
         InfoMeataxe ("#I Succeeded. Calculating centralising matrix.\n");
         newmatrices := [];
         Pinv := P^-1;
         for i in [1..Length (matrices)] do
            newmatrices[i] := P * matrices[i] * Pinv;
         od;
         # Make the sum of copies of C0 as centmat
         centmat := NullMat (dim, dim, F);
         nblocks := dim/e;
         for i in [1..nblocks] do
            for j in [1..e] do
               for k in [1..e] do
                  centmat[ (i - 1) * e + j][ (i - 1) * e + k] := C0[j][k];
               od;
            od;
         od;
         InfoMeataxe ("#I Checking that it centralises the generators.\n");
         # Check centralizing.
         looking := true;
         i := 1;
         while looking and i <= Length (newmatrices) do
            if newmatrices[i] * centmat <> centmat * newmatrices[i] then
               looking := false;
            fi;
            i := i + 1;
         od;
         if looking then
            InfoMeataxe ("#I It did!\n");
            SetDegreeFieldExtFlag (module, e);
            SetAbsolutelyReducibleFlag (module, true);
            SetCentMatFlag (module, P^-1 * centmat * P); # get the base right
            # We will also record the minimal polynomial of C0 
            # (and hence of centmat) in case we need it at some future date.
            SetCentMatMinPolyFlag (module, MinimalPolynomial (C0));
            return false;
         fi;
         InfoMeataxe ("#I But it didn't.\n");
      else
         InfoMeataxe ("#I Failed!\n");
      fi;
   od;

   InfoMeataxe ("#I Tried all divisors. Must be absolutely irreducible.\n");
   SetDegreeFieldExtFlag (module, 1);
   SetAbsolutelyReducibleFlag (module, false);
   return true;
end;

#############################################################################
##
#F  UndoAbsolutelyIrreducibleFlags (module) . . . undo the flags set by the 
## function IsAbsolutelyIrreducible. The point of this is to
## allow us to retest for absolute irreducibility from scratch (a context
## in which we might do this is when we have added an extra generator
## to the group acting on a module already shown to be irreducible but
## not absolutely irreducible. To avoid retesting for irreducibility we
## don't remake the whole module, but have to reset the matrices field, and
## everything associated with absolute irreducibility.
## One should be very careful about doing this, because there could
## be other fields that might be affected.
##
UndoAbsolutelyIrreducibleFlags := function (module)

   UndoDegreeFieldExtFlag (module);
   UndoAbsolutelyReducibleFlag (module);
   UndoCentMatFlag (module);

   #This is new, caused by Frank's IsAbsolutelyIrreducible function  
   Unbind (module.isAbsolutelyIrreducible);
end;

#############################################################################
##
#F  FieldGenCentMat ( module ) . . find a centralizing matrix that generates
##                           the centralizing field of an irreducible module
##
## FieldGenCentMat () should only be applied to modules that have already
## been proved irreducible using IsIrreducible. It then tests for absolute
## irreducibility (if not already known) and does nothing if module is
## absolutely irreducible. Otherwise, it replaces the centMat component
## by a matrix that generates (multiplicatively) the centralizing field
## (i.e. its multiplicative order is q^e - 1, where e is the degree of the
## centralizing field. This is not yet used, but maybe in future, if we
## wish to reduce the group to matrices over the larger field.
## It also resets the centMatMinPoly component.
## 
FieldGenCentMat := function ( module )
   local e, F, R, q, qe, minpol, pp, 
         M, v, M0, v0, C, C0, centmat, newcentmat, genpol, looking, 
         i, l, okd; 

   if IsGModule (module) = false then
      return Error ("Argument of IsIrreducible is not a module.");
   elif ReducibleFlag (module) <> false then
      return Error ("GModule is not known to be irreducible.");
   elif AbsolutelyReducibleFlag (module) = "unknown" then 
      IsAbsolutelyIrreducible (module);
   fi;

   if AbsolutelyReducibleFlag (module) = false then
      return;
   fi;

   F := FieldFlag (module);
   R := PolynomialRing (F);
   q := Size (F);
   e := DegreeFieldExtFlag (module);
   qe := q^e - 1;
   minpol := CentMatMinPolyFlag (module);
   # Factorise q^e - 1 
   pp := PrimePowersInt (qe);

   # We seek a generator of the field of order q^e - 1. In other words, a
   # polynomial genpol of degree e, which has multiplicative order 
   # q^e - 1 modulo minpol. We first try the polynomial x, which is the 
   # element we have already. If this does not work, then we try random 
   # non-constant polynomials until we find one with the right order.

   genpol := Indeterminate (F);

   looking := true;
   while looking do
      okd := OrderKnownDividend (R, genpol, minpol, pp); 
      if okd[1] * Order (F, okd[2]) = qe then
         looking := false;
      fi;
      if looking then
         repeat
            genpol := RandomPol (F, e);
         until Degree (genpol) > 0;
         genpol := StandardAssociate (R, genpol);
      fi;
   od;
   # Finally recalculate centmat and its minimal polynomial.
   centmat := CentMatFlag (module);
   newcentmat := Value (genpol, centmat);
   SetCentMatFlag (module, newcentmat);
   SetCentMatMinPolyFlag (module, MinimalPolynomial (newcentmat));
   # Ugh! That was very inefficient - should work out the min poly using
   # polynomials, but will sort that out if its ever needed.
   return;

end;
