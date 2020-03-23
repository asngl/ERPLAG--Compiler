
#ifndef _NODEDEFH
#define _NODEDEFH
#define STRING_MAX_SIZE 25

enum boolean{BOOL_FALSE,BOOL_TRUE};
enum NodeType{PROGRAM_NODE,MODULE_DECLARE_NODE,ID_NODE,MODULE_NODE,PARA_LIST_NODE,NUM_NODE,RNUM_NODE,BOOL_NODE,INPUT_NODE,OUTPUT_NODE,RANGE_NODE,ASSIGN_NODE,MODULE_REUSE_NODE,ID_LIST_NODE,DECLARE_NODE,CONDITION_NODE,CASE_NODE,FOR_NODE,WHILE_NODE,UNARY_NODE,BINARY_NODE};

struct ProgramNode{
	struct ASTNode *moduleDeclarations;
	struct ASTNode *otherModules1;
	struct ASTNode *driverModule;
	struct ASTNode *otherModules2;
};
struct ModuleDeclareNode{
	char moduleName[STRING_MAX_SIZE];
	struct ASTNode *next;
};
struct IdNode{
	char varName[STRING_MAX_SIZE];
	struct ASTNode *index;
};
struct ModuleNode{
	char moduleName[STRING_MAX_SIZE];
	struct ASTNode *inputList;
	struct ASTNode *next;
	struct ASTNode *ret;
	struct ASTNode *body;
};
struct ParaListNode{
	char name[STRING_MAX_SIZE];
	struct ASTNode *type;
	struct ASTNode *next;
	struct ASTNode *index;
};
struct NumNode{
	int num;		
};
struct RNumNode{
	float rnum;
};
struct BoolNode{
	enum boolean value;
};
struct InputNode{
	char name[STRING_MAX_SIZE];
	struct ASTNode *next;
};
struct OutputNode{
	struct ASTNode *value;
	struct ASTNode *next;
};

struct RangeNode{
	struct ASTNode *Range1;
	struct ASTNode *Range2;
};

struct AssignNode{
	struct ASTNode *LHS;
	struct ASTNode *index;
	struct ASTNode *expr;
	struct ASTNode *next;
};
struct ModuleReuseNode{
	struct ASTNode *optional;
	char id[STRING_MAX_SIZE];
	struct ASTNode *idList;
	struct ASTNode *next;
};
struct IdListNode{
	char varName[STRING_MAX_SIZE];
	struct ASTNode *next;
};
struct DeclareNode{
	struct ASTNode *idList;
	char dataType[STRING_MAX_SIZE];
	struct ASTNode *Range;
	struct ASTNode *next;
};
struct ConditionNode{
	char id[STRING_MAX_SIZE];
	struct ASTNode *Case;
	struct ASTNode *Default;
	struct ASTNode *next;
};
struct CaseNode{
	struct ASTNode *value;
	struct ASTNode *stmt;
	struct ASTNode *next;
};
struct ForNode{
	char id[STRING_MAX_SIZE];
	struct ASTNode *range;
	struct ASTNode *stmt;
	struct ASTNode *next;
};
struct WhileNode{
	struct ASTNode *expr;
	struct ASTNode *stmt;
	struct ASTNode *next;
};
struct UnaryNode{
	char op[STRING_MAX_SIZE];
	struct ASTNode *expr;
};
struct BinaryNode{
	char op[STRING_MAX_SIZE];
	struct ASTNode *expr1;
	struct ASTNode *expr2;
};


union untaggedASTNode{
	struct ProgramNode programNode;
	struct ModuleDeclareNode moduleDeclareNode;
	struct IdNode idNode;
	struct ModuleNode moduleNode;
	struct ParaListNode paraList;
	struct NumNode numNode;
	struct RNumNode rNumNode;
	struct BoolNode boolNode;
	struct InputNode inputNode;
	struct OutputNode outputNode;
	struct RangeNode rangeNode;
	struct AssignNode assignNode;
	struct IdListNode idListNode;
	struct DeclareNode declareNode;
	struct ConditionNode conditionNode;
	struct CaseNode caseNode;
	struct ForNode forNode;
	struct WhileNode whileNode;
	struct UnaryNode unaryNode;
	struct BinaryNode binaryNode;
};
struct ASTNode{
	union untaggedASTNode node;
	enum  NodeType tag;
};

#endif