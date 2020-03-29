
int computeHashFromString(char *);// Copy from grammarInitDef.h

FunctionTable *insertSymbolTable(SymbolTable *symbolTable,struct ModuleDeclareNode moduleDeclareNode);
FunctionTable *searchSymbolTable(SymbolTable *symbolTable,char *string);

VariableEntry *insertLocalTable(LocalTable *localTable,struct DeclareNode declareNode);
VariableEntry *searchLocalTable(LocalTable *localTable,char *string);

void addChild(LocalTable *localTable, LocalTable *siblingTable);