/* Group Number 2
Yashdeep Gupta 2017A7PS0114P
Saksham Gupta  2017A7PS0218P
Pratik Kakade  2017A7PS0086P
Bhoomi Sawant  2017A7PS0001P
Ayush Singhal  2017A7PS0116P
*/
#ifndef _NODEDEFH
#define _NODEDEFH
#define STRING_MAX_SIZE 25

enum boolean{BOOL_FALSE,BOOL_TRUE};
enum NodeType{PROGRAM_NODE,MODULE_DECLARE_NODE,ID_NODE,MODULE_NODE,PARA_LIST_NODE,NUM_NODE,RNUM_NODE,BOOL_NODE,INPUT_NODE,OUTPUT_NODE,RANGE_NODE,ASSIGN_NODE,MODULE_REUSE_NODE,ID_LIST_NODE,DECLARE_NODE,CONDITION_NODE,CASE_NODE,FOR_NODE,WHILE_NODE,UNARY_NODE,BINARY_NODE};
enum Datatype{DT_INTEGER,DT_REAL,DT_BOOLEAN,DT_ARRAY, DT_ERROR};//DT_ARRAY is not an actual datatype
enum Operator{OP_PLUS, OP_MINUS, OP_MUL, OP_DIV,   OP_AND, OP_OR   , OP_LT, OP_LE, OP_GE, OP_GT, OP_EQ, OP_NE};

//Structure for Main program node
struct ProgramNode{
	struct ASTNode *moduleDeclarations;
	struct ASTNode *otherModules1;
	struct ASTNode *driverModule;
	struct ASTNode *otherModules2;
};

//Structure for Module declaration node
struct ModuleDeclareNode{
	char moduleName[STRING_MAX_SIZE];
	struct ASTNode *next;
};

//Structure for ID node
struct IdNode{
	char varName[STRING_MAX_SIZE];
	struct ASTNode *index;
};

//Structure for Module node
struct ModuleNode{
	char moduleName[STRING_MAX_SIZE];
	struct ASTNode *inputList;
	struct ASTNode *next;
	struct ASTNode *ret;
	struct ASTNode *body;
	int localVariablesSize;
	int startLine;
	int endLine;
};

//Structure for Parameter list node
struct ParaListNode{
	char name[STRING_MAX_SIZE];
	enum Datatype type;
	struct ASTNode *next;
	struct ASTNode *Range;
};

//Structure for Num node
struct NumNode{
	int num;
};

//Structure for Real numbers
struct RNumNode{
	float rnum;
};

//Structure for Boolean value
struct BoolNode{
	enum boolean value;
};

//Structure for Input node
struct InputNode{
	char name[STRING_MAX_SIZE];
	struct ASTNode *next;
};

//Structure for Output node
struct OutputNode{
	struct ASTNode *value;
	struct ASTNode *next;
};

//Structure for Range node
struct RangeNode{
	struct ASTNode *Range1;
	struct ASTNode *Range2;
};

//Structure for Assign node
struct AssignNode{
	char LHS[STRING_MAX_SIZE];
	struct ASTNode *index;
	struct ASTNode *expr;
	struct ASTNode *next;
};

//Structure for Module Reuse node
struct ModuleReuseNode{
	struct ASTNode *optional;
	char id[STRING_MAX_SIZE];
	struct ASTNode *idList;
	struct ASTNode *next;
};

//Structure for ID list node
struct IdListNode{
	char varName[STRING_MAX_SIZE];
	struct ASTNode *next;
};

//Structure for Declare node
struct DeclareNode{
	struct ASTNode *idList;
	enum Datatype dataType;
	struct ASTNode *Range;
	struct ASTNode *next;
};

//Structure for Condition node
struct ConditionNode{
	char id[STRING_MAX_SIZE];
	struct ASTNode *Case;
	struct ASTNode *Default;
	struct ASTNode *next;
	int startLine;
	int endLine;
	int presentDefault; //1 if present, else 0
};

//Structure for Case node
struct CaseNode{
	struct ASTNode *value;
	struct ASTNode *stmt;
	struct ASTNode *next;
};

//Structure for For node
struct ForNode{
	char id[STRING_MAX_SIZE];
	struct ASTNode *range;
	struct ASTNode *stmt;
	struct ASTNode *next;
	int startLine;
	int endLine;
};

//Structure for While node
struct WhileNode{
	struct ASTNode *expr;
	struct ASTNode *stmt;
	struct ASTNode *next;
	int startLine;
	int endLine;
};

//Structure for Unary node
struct UnaryNode{
	enum Operator op;
	struct ASTNode *expr;
	enum Datatype type;
};

//Structure for Binary node
struct BinaryNode{
	enum Operator op;
	struct ASTNode *expr1;
	struct ASTNode *expr2;
	enum Datatype type;
	enum Datatype childType;
};

//Union structure for making Main ast node
union untaggedASTNode{
	struct ProgramNode programNode;
	struct ModuleDeclareNode moduleDeclareNode;
	struct IdNode idNode;
	struct ModuleNode moduleNode;
	struct ParaListNode paraListNode;
	struct NumNode numNode;
	struct RNumNode rNumNode;
	struct BoolNode boolNode;
	struct InputNode inputNode;
	struct OutputNode outputNode;
	struct RangeNode rangeNode;
	struct AssignNode assignNode;
	struct ModuleReuseNode moduleReuseNode;
	struct IdListNode idListNode;
	struct DeclareNode declareNode;
	struct ConditionNode conditionNode;
	struct CaseNode caseNode;
	struct ForNode forNode;
	struct WhileNode whileNode;
	struct UnaryNode unaryNode;
	struct BinaryNode binaryNode;
};

//Structure for Main AST node
struct ASTNode{
	union untaggedASTNode node;
	enum  NodeType tag;
  	int lineNumber;
  	struct VariableEntry *localTableEntry;
};

#endif

