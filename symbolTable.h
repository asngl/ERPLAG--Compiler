
int computeHashFromString(char *);// Copy from grammarInitDef.h

ParameterList *populateParaList(struct ASTNode *node);
LocalTable *populateLocalTable(struct ASTNode *node);

FunctionTable *insertSymbolTable(SymbolTable *symbolTable,struct ModuleDeclareNode moduleDeclareNode);
FunctionTable *searchSymbolTable(SymbolTable *symbolTable,char *string);

VariableEntry *insertLocalTable(LocalTable *localTable,struct DeclareNode declareNode);
VariableEntry *searchLocalTable(LocalTable *localTable,char *string);

void addChild(LocalTable *localTable, LocalTable *siblingTable);