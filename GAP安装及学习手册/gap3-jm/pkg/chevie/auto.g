CHEVIE.AUTO:=rec();
CHEVIE.AutoLoad:=function(flist,fields,file)local f,t;
  for t in fields do 
    if not IsBound(CHEVIE.AUTO.(t)) then CHEVIE.AUTO.(t):=rec();fi;
    for f in flist do CHEVIE.AUTO.(t).(f):=file;od;
  od;
end;
CHEVIE.AutoLoad(["PrintDiagram"],["AffineA","AffineD","AffineI"],"prg/affine");
CHEVIE.AutoLoad(["PrintDiagram","BraidRelations","CharName","ReflectionName",
  "SemisimpleRank","Size","paramclasses","GeneratingRoots","GeneratingCoRoots",
  "CartanMat","EigenvaluesGeneratingReflections","ReflectionCoDegrees",
  "ReflectionDegrees","PowerMaps","ParabolicRepresentatives",
  "sparseFakeDegrees","FakeDegree","HighestPowerFakeDegrees",
  "LowestPowerFakeDegrees","Generic","SchurModels","paramchars","SchurData",
  "FactorizedSchurElement","SchurElement","DecompositionMatrix","GetParams",
  "Embed","HeckeCharTable","Rep","HeckeRepresentation","Representation",
  "UnipotentCharacters","Invariants","Discriminant"],["G4_22"],"tbl/cmp4_22");
CHEVIE.AutoLoad(["NrConjugacyClasses","ClassInfo","CharInfo","CharTable"],
  ["2G5","G4_22"],"tbl/cmp4_22");
CHEVIE.AutoLoad(["PhiFactors"],["2G5"],"tbl/cmp4_22");
CHEVIE.AutoLoad(["PrintDiagram","GeneratingRoots","GeneratingCoRoots",
  "CartanMat","EigenvaluesGeneratingReflections","BraidRelations","AltPres",
  "ReflectionDegrees","Size","NrConjugacyClasses","ParabolicRepresentatives",
  "WordsClassRepresentatives","ClassNames","PowerMaps","ClassInfo","CharInfo",
  "CycPolSchurElements","sparseFakeDegrees","HeckeCharTable","CharTable",
  "HeckeRepresentation","UnipotentCharacters","Invariants","BasicDerivations",
  "Discriminant"],["G24"],"tbl/cmplxg24");
CHEVIE.AutoLoad(["PrintDiagram","GeneratingRoots",
  "EigenvaluesGeneratingReflections","HyperplaneRepresentatives",
  "BraidRelations","Size","ReflectionDegrees","NrConjugacyClasses",
  "ParabolicRepresentatives","ClassNames","WordsClassRepresentatives",
  "PowerMaps","ClassInfo","CharInfo","HeckeCharTable","CharTable",
  "sparseFakeDegrees","SchurModels","SchurData","HeckeRepresentation",
  "UnipotentCharacters","Invariants","Discriminant"],["G25"],"tbl/cmplxg25");
CHEVIE.AutoLoad(["PrintDiagram","GeneratingRoots","HyperplaneRepresentatives",
  "EigenvaluesGeneratingReflections","BraidRelations","ReflectionName","Size",
  "ReflectionDegrees","NrConjugacyClasses","ParabolicRepresentatives",
  "ClassNames","WordsClassRepresentatives","PowerMaps","ClassInfo","CharInfo",
  "HeckeCharTable","CharTable","sparseFakeDegrees","SchurModels","SchurData",
  "HeckeRepresentation","UnipotentCharacters","Invariants","Discriminant"],
  ["G26"],"tbl/cmplxg26");
CHEVIE.AutoLoad(["AltPres","BraidRelations","PrintDiagram","GeneratingRoots",
  "GeneratingCoRoots","CartanMat","EigenvaluesGeneratingReflections","Size",
  "ReflectionDegrees","NrConjugacyClasses","ParabolicRepresentatives",
  "ClassNames","WordsClassRepresentatives","PowerMaps","ClassInfo","CharInfo",
  "CycPolSchurElements","sparseFakeDegrees","HeckeCharTable","CharTable",
  "HeckeRepresentation","UnipotentCharacters","Invariants","BasicDerivations",
  "Discriminant"],["G27"],"tbl/cmplxg27");
CHEVIE.AutoLoad(["AltPres","PrintDiagram","GeneratingRoots",
  "EigenvaluesGeneratingReflections","BraidRelations","Size",
  "ReflectionDegrees","NrConjugacyClasses","ParabolicRepresentatives",
  "ClassNames","WordsClassRepresentatives","PowerMaps","ClassInfo","CharInfo",
  "CycPolSchurElements","sparseFakeDegrees","HeckeCharTable","CharTable",
  "HeckeRepresentation","UnipotentCharacters","Invariants","BasicDerivations",
  "Discriminant"],["G29"],"tbl/cmplxg29");
CHEVIE.AutoLoad(["PrintDiagram","BraidRelations","GeneratingRoots",
  "EigenvaluesGeneratingReflections","Size","ReflectionCoDegrees",
  "ReflectionDegrees","ParabolicRepresentatives","NrConjugacyClasses",
  "ClassNames","WordsClassRepresentatives","ClassInfo","PowerMaps","CharInfo",
  "cycpolfakedegrees","SchurModels","SchurData","FactorizedSchurElement",
  "SchurElement","HeckeCharTable","CharTable","HeckeRepresentation",
  "Representation","Invariants","BasicDerivations","Discriminant"],["G31"],
  "tbl/cmplxg31");
CHEVIE.AutoLoad(["PrintDiagram","GeneratingRoots",
  "EigenvaluesGeneratingReflections","HyperplaneRepresentatives",
  "BraidRelations","Size","ReflectionDegrees","NrConjugacyClasses",
  "ParabolicRepresentatives","ClassNames","WordsClassRepresentatives",
  "PowerMaps","ClassInfo","CharInfo","HeckeCharTable","CharTable",
  "SchurModels","SchurData","sparseFakeDegrees","HeckeRepresentation",
  "CachedRepresentations","Representation","Invariants","Discriminant",
  "UnipotentCharacters"],["G32"],"tbl/cmplxg32");
CHEVIE.AutoLoad(["PrintDiagram","BraidRelations","AltPres","GeneratingRoots",
  "GeneratingCoRoots","CartanMat","EigenvaluesGeneratingReflections","Size",
  "ReflectionDegrees","NrConjugacyClasses","ParabolicRepresentatives",
  "ClassNames","WordsClassRepresentatives","PowerMaps","ClassInfo","CharInfo",
  "CycPolSchurElements","sparseFakeDegrees","HeckeCharTable","CharTable",
  "HeckeRepresentation","Representation","UnipotentCharacters","Invariants",
  "BasicDerivations","Discriminant"],["G33"],"tbl/cmplxg33");
CHEVIE.AutoLoad(["PrintDiagram","BraidRelations","GeneratingRoots",
  "EigenvaluesGeneratingReflections","Size","ReflectionDegrees",
  "NrConjugacyClasses","ParabolicRepresentatives","ClassNames",
  "WordsClassRepresentatives","PowerMaps","ClassInfo","CharInfo",
  "CycPolSchurElements","HeckeCharTable","CharTable","HeckeRepresentation",
  "Representation","sparseFakeDegrees","UnipotentCharacters","Invariants"],
  ["G34"],"tbl/cmplxg34");
CHEVIE.AutoLoad(["PrintDiagram","SemisimpleRank","BraidRelations","Size",
  "ReflectionName","GeneratingRoots","EigenvaluesGeneratingReflections",
  "CartanMat","ReflectionDegrees","ReflectionCoDegrees",
  "ParabolicRepresentatives","NrConjugacyClasses","ClassInfo","ClassName",
  "PowerMaps","CharInfo","LowestPowerFakeDegrees","HighestPowerFakeDegrees",
  "CharSymbols","FakeDegree","CharName","SchurModel","SchurData",
  "SchurElement","FactorizedSchurElement","HeckeCharTable",
  "HeckeRepresentation","Representation","CharTable","UnipotentCharacters",
  "Invariants"],["imp"],"tbl/cmplximp");
CHEVIE.AutoLoad(["PhiFactors","ReducedInRightCoset","ClassInfo",
  "NrConjugacyClasses","CharInfo","CharName","CharTable",
  "UnipotentCharacters"],["timp"],"tbl/cmpxtimp");
CHEVIE.AutoLoad(["ReflectionName","NrConjugacyClasses",
  "WordsClassRepresentatives","ClassInfo","PhiFactors","ClassParameter",
  "CharName","CharInfo","HeckeCharTable","CharTable","Representation",
  "HeckeRepresentation","UnipotentCharacters"],["2I"],"tbl/cox2i");
CHEVIE.AutoLoad(["ReflectionDegrees","Size","GeneratingRoots",
  "NrConjugacyClasses","cyclestructure","generators","CartanMat","PowerMaps",
  "WordsClassRepresentatives","ParabolicRepresentatives","ClassInfo",
  "CharInfo","vpolheckeirreducibles","CycPolSchurElements","sparseFakeDegrees",
  "HeckeCharTable","Representation","WGraphs","WGraph","HeckeRepresentation",
  "UnipotentCharacters","Invariants","Discriminant",
  "KLeftCellRepresentatives"],["H3"],"tbl/coxh3");
CHEVIE.AutoLoad(["ReflectionDegrees","Size","GeneratingRoots","CartanMat",
  "WordsClassRepresentatives","NrConjugacyClasses","ParabolicRepresentatives",
  "ClassParameter","PowerMaps","ClassInfo","CharInfo","vpolheckeirreducibles",
  "CycPolSchurElements","cycpolfakedegrees","HeckeCharTable","WGraphs",
  "WGraph","HeckeRepresentation","Representation","UnipotentCharacters",
  "Invariants","Discriminant","KLeftCellRepresentatives"],["H4"],"tbl/coxh4");
CHEVIE.AutoLoad(["CartanMat","PrintDiagram","ReflectionName","SemisimpleRank",
  "GeneratingRoots","EigenvaluesGeneratingReflections","Size",
  "ReflectionDegrees","NrConjugacyClasses","ParabolicRepresentatives",
  "CharName","CharInfo","WordsClassRepresentatives","ClassInfo",
  "HeckeCharTable","Representation","HeckeRepresentation","Frobenius",
  "PoincarePolynomial","SchurElement","FakeDegree","CharTable",
  "DecompositionMatrix","FactorizedSchurElement","Invariants",
  "SymbolToParameter","ParameterToSymbol","UnipotentCharacters"],["I"],
  "tbl/coxi");
CHEVIE.AutoLoad(["CharName"],["2E6","2F4","2G5","E6","E7","E8","F4","G2","G24",
  "G25","G26","G27","G29","G31","G32","G33","G34","H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["IrredInfo"],["2E6","3D4","E6","E7","E8","G2","G24","G25",
  "G27","G29","G31","G32","G33","G34","H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["CartanMat"],["G25","G26","G29","G31","G32","G34"],
  "tbl/exceptio");
CHEVIE.AutoLoad(["ReflectionName"],["2A","2D","2E6","2F4","3D4","A","D","E6",
  "E7","E8","G24","G25","G26","G27","G29","G31","G32","G33","G34","H3","H4"],
  "tbl/exceptio");
CHEVIE.AutoLoad(["CharTable"],["2E6","2F4","3D4","E6","E7","E8","F4","G2","H3",
  "H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["PoincarePolynomial","HighestPowerGenericDegrees",
  "LowestPowerGenericDegrees"],["E6","E7","E8","G24","G27","G29","G33","G34",
  "H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["Representation"],["G24","G25","G26","G27","G29"],
  "tbl/exceptio");
CHEVIE.AutoLoad(["SemisimpleRank"],["A","B","D","E6","F4","G2","G24","G25",
  "G26","G27","G29","G31","G32","G33","G34","H3"],"tbl/exceptio");
CHEVIE.AutoLoad(["FakeDegree","HighestPowerFakeDegrees",
  "LowestPowerFakeDegrees"],["E6","E7","E8","F4","G2","G24","G25","G26","G27",
  "G29","G31","G32","G33","G34","H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["PrintDiagram"],["E6","E7","E8","H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["DecompositionMatrix"],["F4","G2","G25","G26"],
  "tbl/exceptio");
CHEVIE.AutoLoad(["SchurElement"],["E6","E7","E8","F4","G24","G25","G26","G27",
  "G29","G32","G33","G34","H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["FactorizedSchurElement"],["E6","E7","E8","F4","G2","G24",
  "G25","G26","G27","G29","G32","G33","G34","H3","H4"],"tbl/exceptio");
CHEVIE.AutoLoad(["WordsClassRepresentatives","ClassInfo","NrConjugacyClasses",
  "ClassParameter","CharParams","CharName","CharInfo","CharTable",
  "HeckeCharTable","PhiFactors","HeckeRepresentation","Representation",
  "FakeDegree","UnipotentCharacters","UnipotentClasses"],["2A"],"tbl/weyl2a");
CHEVIE.AutoLoad(["ClassParams","WordsClassRepresentatives","ClassInfo",
  "NrConjugacyClasses","ClassParameter","IsPreferred","IsGood","testchar",
  "CharParams","CharName","CharInfo","CharTable","HeckeCharTable","FakeDegree",
  "PhiFactors","UnipotentCharacters","UnipotentClasses"],["2D"],"tbl/weyl2d");
CHEVIE.AutoLoad(["NrConjugacyClasses","ClassInfo","CharInfo","cyclestructure",
  "generators","phi","CartanMat","vpolheckeirreducibles","ClassParameter",
  "HeckeCharTable","PhiFactors","UnipotentCharacters","UnipotentClasses"],
  ["2E6"],"tbl/weyl2e6");
CHEVIE.AutoLoad(["NrConjugacyClasses","ClassInfo","CharInfo","cyclestructure",
  "generators","phi","CartanMat","ClassParameter","HeckeCharTable",
  "PhiFactors","Representation","HeckeRepresentation","UnipotentCharacters"],
  ["2F4"],"tbl/weyl2f4");
CHEVIE.AutoLoad(["cyclestructure","generators","phi","CharName","CartanMat",
  "ClassParameter","ClassInfo","NrConjugacyClasses","CharInfo",
  "HeckeCharTable","PhiFactors","Representation","HeckeRepresentation",
  "UnipotentCharacters","UnipotentClasses"],["3D4"],"tbl/weyl3d4");
CHEVIE.AutoLoad(["CartanMat","ReflectionDegrees","PrintDiagram",
  "GeneratingRoots","ParabolicRepresentatives","WordClass","ClassInfo",
  "NrConjugacyClasses","WeightInfo","ClassParameter","CharParams",
  "LowestPowerFakeDegree","HighestPowerFakeDegree","CharName","CharInfo",
  "CharTable","Hk","HeckeCharTable","PoincarePolynomial","SchurElement",
  "FactorizedSchurElement","HeckeRepresentation","Representation","FakeDegree",
  "DecompositionMatrix","UnipotentCharacters","Invariants","UnipotentClasses",
  "KLeftCellRepresentatives"],["A"],"tbl/weyla");
CHEVIE.AutoLoad(["CartanMat","PrintDiagram","ReflectionName","GeneratingRoots",
  "ParabolicRepresentatives","ReflectionDegrees","Size","NrConjugacyClasses",
  "WeightInfo","WordClass","ClassInfo","ClassParameter","CharParams",
  "CharName","LowestPowerFakeDegree","CharInfo","CharTable","Hk",
  "HeckeCharTable","PoincarePolynomial","SchurElement",
  "FactorizedSchurElement","HeckeRepresentation","Representation","FakeDegree",
  "DecompositionMatrix","UnipotentCharacters","UnipotentClasses","Invariants"],
  ["B"],"tbl/weylbc");
CHEVIE.AutoLoad(["CartanMat","Size","PrintDiagram","GeneratingRoots",
  "WeightInfo","ParabolicRepresentatives","WordsClassRepresentatives",
  "ClassInfo","NrConjugacyClasses","CharInfo","CharName","gensMODA",
  "ClassParameter","CharTable","Hk","HeckeCharTable","FactorizedSchurElement",
  "HeckeRepresentation","Representation","PoincarePolynomial",
  "symbolcharparam","Invariants","CycPolGenericDegree","SchurElement",
  "FakeDegree","UnipotentCharacters","ReflectionDegrees","UnipotentClasses"],
  ["D"],"tbl/weyld");
CHEVIE.AutoLoad(["ReflectionDegrees","GeneratingRoots","WeightInfo",
  "NrConjugacyClasses","WordsClassRepresentatives","ClassNames","ClassInfo",
  "ParabolicRepresentatives","CharInfo","cyclestructure","generators",
  "CartanMat","PowerMaps","vpolheckeirreducibles","CycPolSchurElements",
  "sparseFakeDegrees","ClassParameter","HeckeCharTable","WGraphs","WGraph",
  "HeckeRepresentation","Representation","DecompositionMatrix",
  "UnipotentCharacters","Invariants","UnipotentClasses",
  "KLeftCellRepresentatives"],["E6"],"tbl/weyle6");
CHEVIE.AutoLoad(["ReflectionDegrees","Size","GeneratingRoots","WeightInfo",
  "NrConjugacyClasses","WordsClassRepresentatives","ClassNames","ClassInfo",
  "ParabolicRepresentatives","CharInfo","cyclestructure","generators",
  "CartanMat","PowerMaps","vpolheckeirreducibles","longestperm",
  "CycPolSchurElements","cycpolfakedegrees","ClassParameter","HeckeCharTable",
  "WGraphs","WGraph","HeckeRepresentation","Representation",
  "DecompositionMatrix","UnipotentCharacters","Invariants","UnipotentClasses",
  "KLeftCellRepresentatives"],["E7"],"tbl/weyle7");
CHEVIE.AutoLoad(["ReflectionDegrees","Size","GeneratingRoots",
  "NrConjugacyClasses","WordsClassRepresentatives","ClassNames","ClassInfo",
  "ParabolicRepresentatives","CharInfo","cyclestructure","generators",
  "CartanMat","PowerMaps","vpolheckeirreducibles","CycPolSchurElements",
  "cycpolfakedegrees","ClassParameter","HeckeCharTable","WGraphs","WGraph",
  "HeckeRepresentation","Representation","DecompositionMatrix",
  "UnipotentCharacters","Invariants","UnipotentClasses",
  "KLeftCellRepresentatives"],["E8"],"tbl/weyle8");
CHEVIE.AutoLoad(["CartanMat","PrintDiagram","ReflectionName","GeneratingRoots",
  "HyperplaneRepresentatives","ParabolicRepresentatives","Size",
  "ReflectionDegrees","NrConjugacyClasses","ClassInfo","CharInfo",
  "cyclestructure","generators","orbits","PowerMaps","sparseFakeDegrees",
  "ClassParameter","HeckeCharTable","PoincarePolynomial","SchurModels",
  "SchurData","Representation","WGraphs","WGraph","HeckeRepresentation",
  "UnipotentCharacters","Invariants","Discriminant","UnipotentClasses",
  "KLeftCellRepresentatives"],["F4"],"tbl/weylf4");
CHEVIE.AutoLoad(["CartanMat","PrintDiagram","ReflectionName",
  "ParabolicRepresentatives","GeneratingRoots","HyperplaneRepresentatives",
  "Size","ReflectionDegrees","NrConjugacyClasses","CharInfo",
  "WordsClassRepresentatives","ClassNames","ClassInfo","PowerMaps",
  "sparseFakeDegrees","ClassParameter","squv","HeckeCharTable",
  "HeckeRepresentation","Representation","PoincarePolynomial","SchurModels",
  "SchurData","SchurElement","UnipotentCharacters","Invariants","Discriminant",
  "UnipotentClasses","KLeftCellRepresentatives"],["G2"],"tbl/weylg2");
