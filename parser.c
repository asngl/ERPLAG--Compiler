#define Max_Token_Size 30
#define No_Of_Tokens 150
#define mod 150
//Keep No of tokens and mod same when necessary, Keep them accurate as they are needed for Mapping table
#define No_Of_Rules 100
#define Max_Rule_Size 150
#define No_Of_NT 50
#define No_Of_T 100
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

int TotalRules=0;

union Symb{
	Terminal T;
	NonTerminal NT;
};
typedef union Symb Symbol;
// Union for enumeration of terminals and nonterminals

struct MT{
	Symbol s;
	char str[Max_Token_Size];
	enum type_of_symbol tag;
	int flag; //To determine whether element is present or not, 1 if present
};
typedef struct MT MappingTable[No_Of_Tokens];
//Structure for enum to String mapping

MappingTable mapping;
//Global variable for hard-coded Mapping table

MappingTable mappingString;
//Global variable for hash coded Mapping table

enum type_of_symbol{terminal, nonTerminal};
struct rhsnode{
	Symbol s;
	enum type_of_symbol tag;
	struct rhsnode *next;
};
typedef struct rhsnode RHSNODE;
// Structure for individual grammar rule

struct cell{
	NonTerminal sym;
	RHSNODE *head;
};
typedef struct cell GRAMMAR[No_Of_Rules];
//Structure for Grammar

GRAMMAR grammarRules;
//Global Variable for Grammar Rules




struct MT SearchMappingTable(char str[]){
	int c=0;
	int curr=0;
	while(str[curr]!='\0'){
		c+=(int)str[curr];
	}
	c=c%mod;
	while(mappingString[c].flag!=0 && strcmp(str,mappingString[c].str)!=0){
		c++;
		c=c%mod;
	}
	if(mappingString[c].flag==0){
		printf("%s Token Not found",str);
		return NULL;
	}
	return mappingString[c];

}
//Function for searching returning the mapping of str Token

int HashCodeMappingTable(){
	for(int i=0;i<No_Of_Tokens;i++){
		mappingString[i].flag=0;
	}
	
	for(int i=0;mapping[i].flag==1;i++){
		int c=0;
		int curr=0;
		while(mapping[i].str[curr]!='\0'){
			c+=(int)mapping[i].str[curr];
			curr++;
		}
		c=c%mod;
		while(mappingString[c].flag!=0){
			c++;
			c=c%mod;
		}
		mappingString[c]=mapping[i];
	}
	return 0;
}
//Function to convert enum hashed Mapping table to String hashed Mapping table


int ParseGrammarFile(char FileName[]){
	FILE *fp;
	fp = fopen(FileName,"r");
	if(fp==NULL){
		printf("Could not find Grammar Source File");
		return 0;
	}
	
	HashCodeMappingTable(); //Calling function to generate String hashed mapping table
	int ruleNo = 0; // Vairable for tracking the current rule number
	char grammarString[Max_Rule_size];
	while(fgets(grammarString,Max_Rule_Size,fp)!= NULL){
		char grammarToken[Max_Token_Size];
		int currentPos=0;
		int i=0;
		while(grammarString[currentPos]!=' '){
			grammarToken[i]=grammarString[currentPos];
			currentPos++;
			i++;
		}
		currentPos++;
		grammarToken[i]='\0';
		struct MT tokenMap;
		tokenMap=SearchMappingTable(grammarToken);
		if(tokenMap==NULL){
			continue;
		}
		grammarRules[ruleNo].sym = tokenMap.S.NT;
		grammarRules[ruleNo].head=NULL;
		while(1){
			i=0;
			while(grammarString[currentPos]!=' ' && grammarString[currentPos]!='\n'){
				grammarToken[i]=grammarString[currentPos];
				currentPos++;
				i++;
			}
			grammarToken[i]='\0';
			tokenMap=SearchMappingTable(grammarToken);
			if(tokenMap==NULL){
				continue;
			}
			RHSNODE *ptr,*newElement;
			newElement = (RHSNODE *)malloc(sizeof(RHSNODE));
			newElement->s=tokenMap.s;
			newElement->tag=tokenMap.tag;
			newElement->next=NULL;
			ptr=grammarRules[ruleNo].head;
			if(ptr==NULL){
				grammarRules[ruleNo].head=newElement;
			}
			else{
				while(ptr->next != NULL)
					ptr=ptr->next;
				ptr->next=newElement;
			}
			if(grammarString[currentPos]=='\n')
				break;
			currentPos++;
		}
		ruleNo++;
	}
	fclose(fp);
	TotalRules=ruleNo;
}



struct FAndF{
	int First[3];
	int Follow[3];

};

//Structure for First and Follow sets of our Grammar

struct FAndF FAndF_NT[No_Of_NT];
struct FAndF FAndF_Rules[No_Of_Rules];
//Global variable for first and follow sets for Non terminals and Rules correspondingly

int ComputeFirstAndFollowSets(){
	int computeStopFlag=1; //Flag to check whether the computation has to be stopped or not
	int eps_enum;
	struct MT EPSToken;
	EPSToken = SearchMappingTable("EPS");
	eps_enum = EPSToken.s.T;
	struct FAndF EPS;
	EPS.First[eps_enum/32]= 1<<(eps_enum % 32);
	//BITSET for testing EPS bit
	while(computeStopFlag==1){
		computeStopFlag=0;
		for(int i=0;i<TotalRules;i++){
			struct cell currentRule;
			currentRule=grammarRules[i];
			int LHS=currentRule.sym;
			RHSNODE *token;
			if(token->tag==0){ // In case of first being a terminal
				int num= token->s.T;
				if(FAndF_Rules[i].First[num/32] != (FAndF_Rules[i].First[num/32] | 1<<(num%32))){
					computeStopFlag=1;
					FAndF_Rules[i].First[num/32] = (FAndF_Rules[i].First[num/32] | 1<<(num%32));
					FAndF_NT[LHS].First[num/32] = FAndF_NT[LHS].First[num/32] | FAndF_Rules[i].First[num/32];
					
				}
			}
			else{
				int num = token->s.NT;
				if((FAndF_Rules[i].First[0] != (FAndF_Rules[i].First[0] | FAndF_NT[num].First[0])) || (FAndF_Rules[i].First[1] != (FAndF_Rules[i].First[1] | FAndF_NT[num].First[1])) ||(FAndF_Rules[i].First[2] != (FAndF_Rules[i].First[2] | FAndF_NT[num].First[2]))){
					computeStopFlag=1;
					FAndF_Rules[i].First[0] = (FAndF_Rules[i].First[0] | FAndF_NT[num].First[0]);
					FAndF_Rules[i].First[1] = (FAndF_Rules[i].First[1] | FAndF_NT[num].First[1]);
					FAndF_Rules[i].First[2] = (FAndF_Rules[i].First[2] | FAndF_NT[num].First[2]);
					FAndF_NT[LHS].First[0] = (FAndF_Rules[i].First[0] | FAndF_NT[LHS].First[0]);
					FAndF_NT[LHS].First[1] = (FAndF_Rules[i].First[1] | FAndF_NT[LHS].First[1]);
					FAndF_NT[LHS].First[2] = (FAndF_Rules[i].First[2] | FAndF_NT[LHS].First[2]);
				}
				while()
				//Write functionality for EPS
			}
		}
	}
}


























