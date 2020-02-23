#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "parser.h"
int TotalRules=0;

//Remember to keep EPS value in mapping table between 32 to 64 as we are directly removing it in case of first and follow set
//Change value of mapping[i-63].str in print functions according to the value of first non terminal in mapping table





//Structure for enum to String mapping

MappingTable mapping;
//Global variable for hard-coded Mapping table

MappingTable mappingString;
//Global variable for hash coded Mapping table




// Structure for individual grammar rule



//Structure for Grammar

GRAMMAR grammarRules;
//Global Variable for Grammar Rules



struct FAndF{
	int First[3];
	int Follow[3];

};

//Structure for First and Follow sets of our Grammar

struct FAndF FAndF_NT[No_Of_NT];
struct FAndF FAndF_Rules[No_Of_Rules];
//Global variable for first and follow sets for Non terminals and Rules correspondingly



void init_mappingtable();
int ComputeFirstSet();
int ComputeFollowSet();
int CreateParseTable();

int parseTable[No_Of_NT][No_Of_T];

struct MT SearchMappingTable(char str[]){
	int c=0;
	int curr=0;
	while(str[curr]!='\0'){
		c+=(int)str[curr];
		curr++;
	}
	c=c%mod;
	//printf("c=%d\n",c);
	while(mappingString[c].flag==1 &&strcmp(str,mappingString[c].str)!=0){
		c++;
		c=c%mod;
	}
	if(mappingString[c].flag!=1){
		printf("%s Token Not found",str);
		return mappingString[c];
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
		/*printf("\n%d::%s\n",c,mapping[i].str);
		*/
		while(mappingString[c].flag==1){
			c++;
			c=c%mod;
			/*if(strcmp(mapping[i].str,"EPS")==0){
				printf("c=%d \n",c);
			}*/
		}
		mappingString[c]=mapping[i];
	}
	return 0;
}
//Function to convert enum hashed Mapping table to String hashed Mapping table


void ParseGrammarFile(char FileName[]){
	FILE *fp;
	fp = fopen(FileName,"r");
	if(fp==NULL){
		printf("Could not find Grammar Source File");
		return ;
	}
	

	int ruleNo = 0; // Vairable for tracking the current rule number
	char grammarString[Max_Rule_Size];
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
		if(tokenMap.flag==0){
			continue;
		}
		grammarRules[ruleNo].sym = tokenMap.s.NT;
		grammarRules[ruleNo].head=NULL;
		while(1){
			i=0;
			while(grammarString[currentPos]!=' ' && grammarString[currentPos]!='\n' && grammarString[currentPos]!='\r'){
				grammarToken[i]=grammarString[currentPos];
				currentPos++;
				i++;
			}
			grammarToken[i]='\0';
			tokenMap=SearchMappingTable(grammarToken);
			if(tokenMap.flag==0){
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
			if(grammarString[currentPos]=='\n' || grammarString[currentPos]=='\r')
				break;
			currentPos++;
		}
		ruleNo++;
	}
	fclose(fp);
	TotalRules=ruleNo;
	return ;
}




int PrintGrammar(){
	printf("Total Rules=%d",TotalRules);
	for(int i=0;i<TotalRules;i++){
		printf("LHS = %d, RHS =",grammarRules[i].sym);
		RHSNODE *token;
		token= grammarRules[i].head;
		while(token !=NULL){
			if(token->tag==0){
				printf("T,%d  ",token->s.T);
			}
			else{
				printf("NT,%d  ",token->s.NT);
			}
			token=token->next;
		}
		printf("\n");
	}
	return 0;
}

void getElementSet(int A[]){
	for(int i=0;i<96;i++){
		if(A[i/32] & (1<<(i%32))){
			printf("%s,", mapping[i].str);
		}
	}

}

int PrintFirstSet_NT(){
	printf("First set\n");
	for(int i=0;mapping[i+63].flag==1;i++){
		printf("\nString:%s, First set=",mapping[i+63].str);
		getElementSet(FAndF_NT[i].First);
	}
	return 0;
}

int PrintFirstSet_Rules(){
	printf("First Set Ruleswise \n");
	for(int i=0;i<TotalRules;i++){
		printf("\n Rule No=%d, First set=",i);
		getElementSet(FAndF_Rules[i].First);
	}
}


int PrintFollowSet_NT(){
	printf("\nFollow set\n");
	for(int i=0;mapping[i+63].flag==1;i++){
		printf("\nString:%s, Follow set=",mapping[i+63].str);
		getElementSet(FAndF_NT[i].Follow);
	}
	return 0;
}

int PrintParseTable(){
	printf("\n Parse Table \n");
	for(int i=0;mapping[i+63].flag==1;i++){
		printf("\nNonTerminal=%s,  ",mapping[i+63].str);
		for(int j=0;j<No_Of_T;j++){
			if(parseTable[i][j]==-1){
				continue;
			}
			printf("T=%s,Rule no=%d,",mapping[j].str,parseTable[i][j]);
		}
	}
}

void initGrammar(char *filename){
	init_mappingtable();
	HashCodeMappingTable(); //Calling function to generate String hashed mapping table
	int noflag=0;
	/*for(int i=0;i<No_Of_Tokens;i++){
		printf("i=%d, T=%d , NT=%d , String= %s, Flag=%d, Tag=%d\n",i,mappingString[i].s.T,mappingString[i].s.NT,mappingString[i].str,mappingString[i].flag, mappingString[i].tag);
		if(mappingString[i].flag==1)
			noflag++;
	}
	*/
	//struct MT tok;
	//tok = SearchMappingTable("program");
	//printf("Value=%s",tok.str);
	ParseGrammarFile("grammar.txt");
	//PrintGrammar();
	ComputeFirstSet();
	//PrintFirstSet_Rules();
	ComputeFollowSet();
	//PrintFollowSet_NT();
	CreateParseTable();
	//PrintParseTable();
}








int ComputeFirstSet(){
	int computeStopFlag=1; //Flag to check whether the computation has to be stopped or not
	int eps_enum;
	struct MT EPSToken;
	EPSToken = SearchMappingTable("EPS");
	eps_enum = EPSToken.s.T;
	//Enum for etsting eps bit
	for(int i=0;i<No_Of_NT;i++){
		FAndF_NT[i].First[0]=0;
		FAndF_NT[i].First[1]=0;
		FAndF_NT[i].First[2]=0;
		FAndF_NT[i].Follow[0]=0;
		FAndF_NT[i].Follow[1]=0;
		FAndF_NT[i].Follow[2]=0;
	}
	for(int i=0;i<No_Of_Rules;i++){
		FAndF_Rules[i].First[0]=0;
		FAndF_Rules[i].First[1]=0;
		FAndF_Rules[i].First[2]=0;
		FAndF_Rules[i].Follow[0]=0;
		FAndF_Rules[i].Follow[1]=0;
		FAndF_Rules[i].Follow[2]=0;
	}
	int eps_remove=0;
	eps_remove=1<<(eps_enum%32);
	eps_remove= ~eps_remove;
	while(computeStopFlag==1){
		computeStopFlag=0;
		for(int i=0;i<TotalRules;i++){
			struct cell currentRule;
			currentRule=grammarRules[i];
			int LHS=currentRule.sym;
			RHSNODE *token;
			token=currentRule.head;
			if(token->tag==0){ // In case of first being a terminal
				int num= token->s.T;
				if(FAndF_Rules[i].First[num/32] != (FAndF_Rules[i].First[num/32] | 1<<(num%32))){
					computeStopFlag=1;
					FAndF_Rules[i].First[num/32] = (FAndF_Rules[i].First[num/32] | 1<<(num%32));
					FAndF_NT[LHS].First[num/32] = FAndF_NT[LHS].First[num/32] | FAndF_Rules[i].First[num/32];
					
				}
			}
			else{ //In case it is non terminal
				int num = token->s.NT;
				if((FAndF_Rules[i].First[0] != (FAndF_Rules[i].First[0] | FAndF_NT[num].First[0])) || (FAndF_Rules[i].First[1] != (FAndF_Rules[i].First[1] | (FAndF_NT[num].First[1] & eps_remove))) ||(FAndF_Rules[i].First[2] != (FAndF_Rules[i].First[2] | FAndF_NT[num].First[2]))){
					computeStopFlag=1;
					FAndF_Rules[i].First[0] = (FAndF_Rules[i].First[0] | FAndF_NT[num].First[0]);
					FAndF_Rules[i].First[1] = (FAndF_Rules[i].First[1] | (FAndF_NT[num].First[1]&eps_remove));
					FAndF_Rules[i].First[2] = (FAndF_Rules[i].First[2] | FAndF_NT[num].First[2]);
					FAndF_NT[LHS].First[0] = (FAndF_Rules[i].First[0] | FAndF_NT[LHS].First[0]);
					FAndF_NT[LHS].First[1] = (FAndF_Rules[i].First[1] | FAndF_NT[LHS].First[1]);
					FAndF_NT[LHS].First[2] = (FAndF_Rules[i].First[2] | FAndF_NT[LHS].First[2]);
				}
				// If there is an eps in the first set of non terminal
				while(FAndF_NT[num].First[eps_enum/32] & (1<<(eps_enum%32))){
					token=token->next;
					if(token==NULL){
						if(FAndF_Rules[i].First[eps_enum/32] != (FAndF_Rules[i].First[eps_enum/32] | 1<<(eps_enum%32))){
							computeStopFlag=1;
							FAndF_Rules[i].First[eps_enum/32] = (FAndF_Rules[i].First[eps_enum/32] | 1<<(eps_enum%32));
							FAndF_NT[LHS].First[eps_enum/32] |= FAndF_Rules[i].First[eps_enum/32]; 
						}
						break;
					}
					//write for Terminal and Non Terminal
					if(token->tag==0){
						num = token->s.T;
						if(FAndF_Rules[i].First[num/32] != (FAndF_Rules[i].First[num/32] | 1<<(num%32))){
							computeStopFlag=1;
							FAndF_Rules[i].First[num/32] = (FAndF_Rules[i].First[num/32] | 1<<(num%32));
							FAndF_NT[LHS].First[num/32] = FAndF_NT[LHS].First[num/32] | FAndF_Rules[i].First[num/32];
					
						}
						break;
					}
					else{
						num=token->s.NT;
						if((FAndF_Rules[i].First[0] != (FAndF_Rules[i].First[0] | FAndF_NT[num].First[0])) || (FAndF_Rules[i].First[1] != (FAndF_Rules[i].First[1] | (FAndF_NT[num].First[1]&eps_remove))) ||(FAndF_Rules[i].First[2] != (FAndF_Rules[i].First[2] | FAndF_NT[num].First[2]))){
							computeStopFlag=1;
							FAndF_Rules[i].First[0] = (FAndF_Rules[i].First[0] | FAndF_NT[num].First[0]);
							FAndF_Rules[i].First[1] = (FAndF_Rules[i].First[1] | (FAndF_NT[num].First[1]&eps_remove));
							FAndF_Rules[i].First[2] = (FAndF_Rules[i].First[2] | FAndF_NT[num].First[2]);
							FAndF_NT[LHS].First[0] = (FAndF_Rules[i].First[0] | FAndF_NT[LHS].First[0]);
							FAndF_NT[LHS].First[1] = (FAndF_Rules[i].First[1] | FAndF_NT[LHS].First[1]);
							FAndF_NT[LHS].First[2] = (FAndF_Rules[i].First[2] | FAndF_NT[LHS].First[2]);
						}
					}
				}
				
			}
		}
	}
}


int ComputeFollowSet(){
	//initial feof in <program>
	//EPS in case of NT and remove eps from first set of succeeding NT
	//Iterative
	int computeStopFlag=1;
	struct MT EPSToken, FEOFToken;
	EPSToken = SearchMappingTable("EPS");
	int eps_enum = EPSToken.s.T;
	//Enum for etsting eps bit
	int eps_remove=1<<(eps_enum%32);
	eps_remove=~eps_remove;
	FEOFToken = SearchMappingTable("FEOF");
	int feof_enum = FEOFToken.s.T;
	FAndF_NT[0].Follow[feof_enum/32]=1<<feof_enum%32;
	while(computeStopFlag==1){
		computeStopFlag=0;
		for(int i=0;i<TotalRules; i++){
			struct cell currentRule;
			currentRule=grammarRules[i];
			int LHS=currentRule.sym;
			RHSNODE *token1=currentRule.head;
			RHSNODE *token2;
			while(token1 != NULL){
				token2 = token1->next;
				if(token1->tag == terminal){
					token1 = token1->next;
					continue;				
				}				
				if(token2 == NULL){
					if((FAndF_NT[token1->s.NT].Follow[0] != (FAndF_NT[LHS].Follow[0]|FAndF_NT[token1->s.NT].Follow[0]))||(FAndF_NT[token1->s.NT].Follow[1] != (FAndF_NT[LHS].Follow[1]|FAndF_NT[token1->s.NT].Follow[1]))||(FAndF_NT[token1->s.NT].Follow[2] != (FAndF_NT[LHS].Follow[2]|FAndF_NT[token1->s.NT].Follow[2]))){
						computeStopFlag = 1;
						FAndF_NT[token1->s.NT].Follow[0] |= FAndF_NT[LHS].Follow[0];
						FAndF_NT[token1->s.NT].Follow[1] |= FAndF_NT[LHS].Follow[1];
						FAndF_NT[token1->s.NT].Follow[2] |= FAndF_NT[LHS].Follow[2];
					}
					break;
				}
				if(token2->tag == terminal){
					int num = token2->s.T;
					if(FAndF_NT[token1->s.NT].Follow[num/32] != (FAndF_NT[token1->s.NT].Follow[num/32]|(1<<(num%32)))){
						computeStopFlag=1;
						FAndF_NT[token1->s.NT].Follow[num/32] =FAndF_NT[token1->s.NT].Follow[num/32]|(1<<(num%32));
					}
					
				}
				else{
					int num=token2->s.NT;
					if((FAndF_NT[token1->s.NT].Follow[0] != (FAndF_NT[num].First[0]|FAndF_NT[token1->s.NT].Follow[0]))||(FAndF_NT[token1->s.NT].Follow[1] != ((FAndF_NT[num].First[1] & eps_remove)|FAndF_NT[token1->s.NT].Follow[1]))||(FAndF_NT[token1->s.NT].Follow[2] != (FAndF_NT[num].First[2]|FAndF_NT[token1->s.NT].Follow[2]))){
						FAndF_NT[token1->s.NT].Follow[0] = FAndF_NT[num].First[0]|FAndF_NT[token1->s.NT].Follow[0];
						FAndF_NT[token1->s.NT].Follow[1] = (FAndF_NT[num].First[1] & eps_remove)|FAndF_NT[token1->s.NT].Follow[1];	
						FAndF_NT[token1->s.NT].Follow[2] = FAndF_NT[num].First[2]|FAndF_NT[token1->s.NT].Follow[2];
						computeStopFlag=1;
						while(FAndF_NT[num].First[eps_enum/32]&(1<<(eps_enum%32))){
							token2=token2->next;
							if(token2==NULL){
								if((FAndF_NT[token1->s.NT].Follow[0] != (FAndF_NT[LHS].Follow[0]|FAndF_NT[token1->s.NT].Follow[0]))||(FAndF_NT[token1->s.NT].Follow[1] != (FAndF_NT[LHS].Follow[1]|FAndF_NT[token1->s.NT].Follow[1]))||(FAndF_NT[token1->s.NT].Follow[2] != (FAndF_NT[LHS].Follow[2]|FAndF_NT[token1->s.NT].Follow[2]))){
									computeStopFlag=1;
									FAndF_NT[token1->s.NT].Follow[0] |= FAndF_NT[LHS].Follow[0];
									FAndF_NT[token1->s.NT].Follow[1] |= FAndF_NT[LHS].Follow[1];
									FAndF_NT[token1->s.NT].Follow[2] |= FAndF_NT[LHS].Follow[2];
								}
								break;	
							}
							if(token2->tag==terminal){
								num=token2->s.T;
								if(FAndF_NT[token1->s.NT].Follow[num/32] !=(FAndF_NT[token1->s.NT].Follow[num/32]|(1<<(num%32)))){
									computeStopFlag=1;
									FAndF_NT[token1->s.NT].Follow[num/32] =FAndF_NT[token1->s.NT].Follow[num/32]|(1<<(num%32));
								}
								break;	
							}
							else{
								num=token2->s.NT;
								if((FAndF_NT[token1->s.NT].Follow[0] != (FAndF_NT[num].First[0]|FAndF_NT[token1->s.NT].Follow[0]))||(FAndF_NT[token1->s.NT].Follow[1] != ((FAndF_NT[num].First[1] & eps_remove)|FAndF_NT[token1->s.NT].Follow[1]))||(FAndF_NT[token1->s.NT].Follow[2] != (FAndF_NT[num].First[2]|FAndF_NT[token1->s.NT].Follow[2]))){
									FAndF_NT[token1->s.NT].Follow[0] = FAndF_NT[num].First[0]|FAndF_NT[token1->s.NT].Follow[0];
									FAndF_NT[token1->s.NT].Follow[1] = (FAndF_NT[num].First[1] & eps_remove)|FAndF_NT[token1->s.NT].Follow[1];	
									FAndF_NT[token1->s.NT].Follow[2] = FAndF_NT[num].First[2]|FAndF_NT[token1->s.NT].Follow[2];
									computeStopFlag=1;
								}
							}
						}
					}
					
				}
											
				token1=token1->next;											
			}
			
			
			
		}
	}
}




int CreateParseTable(){

	struct MT EPSToken;
	EPSToken = SearchMappingTable("EPS");
	int eps_enum = EPSToken.s.T;
	//Enum for etsting eps bit
	for(int i=0;i<No_Of_NT;i++){
		for(int j=0;j<No_Of_T;j++){
			parseTable[i][j]=-1;
		}
	}
	
	for(int i=0;i<TotalRules;i++){
		int nonTerminalIndex;
		nonTerminalIndex=grammarRules[i].sym;
		if(FAndF_Rules[i].First[eps_enum/32]&(1<<(eps_enum%32))){
			for(int j=0;j<96;j++){
				if(FAndF_NT[nonTerminalIndex].Follow[j/32] & (1<<(j%32))){
					parseTable[nonTerminalIndex][j]=i;
				}
			}
			continue;
		}
		for(int j=0;j<96;j++){
			if(FAndF_Rules[i].First[j/32] & (1<<(j%32))){
				parseTable[nonTerminalIndex][j]=i;
			}
		}
	}
	
	//For inputting syn in our table, we are using -2
	for(int i=0;i<No_Of_NT;i++){
		if(FAndF_NT[i].First[eps_enum/32] & (1<<(eps_enum%32))){
			continue;
		}
		for(int j=0;j<96;j++){
			if((FAndF_NT[i].Follow[j/32] & (1<<(j%32))) && parseTable[i][j]==-1){
				parseTable[i][j]=-2;
			}
		}
	}
}








void init_mappingtable(){
	mapping[0].s.T = INTEGER;
	strcpy(mapping[0].str, "INTEGER");
	mapping[0].tag = terminal;
	mapping[0].flag = 1;

	mapping[1].s.T = REAL;
	strcpy(mapping[1].str, "REAL");
	mapping[1].tag = terminal;
	mapping[1].flag = 1;

	mapping[2].s.T = BOOLEAN;
	strcpy(mapping[2].str, "BOOLEAN");
	mapping[2].tag = terminal;
	mapping[2].flag = 1;

	mapping[3].s.T = OF;
	strcpy(mapping[3].str, "OF");
	mapping[3].tag = terminal;
	mapping[3].flag = 1;

	mapping[4].s.T = ARRAY;
	strcpy(mapping[4].str, "ARRAY");
	mapping[4].tag = terminal;
	mapping[4].flag = 1;

	mapping[5].s.T = START;
	strcpy(mapping[5].str, "START");
	mapping[5].tag = terminal;
	mapping[5].flag = 1;

	mapping[6].s.T = END;
	strcpy(mapping[6].str, "END");
	mapping[6].tag = terminal;
	mapping[6].flag = 1;

	mapping[7].s.T = DECLARE;
	strcpy(mapping[7].str, "DECLARE");
	mapping[7].tag = terminal;
	mapping[7].flag = 1;

	mapping[8].s.T = MODULE;
	strcpy(mapping[8].str, "MODULE");
	mapping[8].tag = terminal;
	mapping[8].flag = 1;

	mapping[9].s.T = DRIVER;
	strcpy(mapping[9].str, "DRIVER");
	mapping[9].tag = terminal;
	mapping[9].flag = 1;

	mapping[10].s.T = PROGRAM;
	strcpy(mapping[10].str, "PROGRAM");
	mapping[10].tag = terminal;
	mapping[10].flag = 1;

	mapping[11].s.T = RECORD;
	strcpy(mapping[11].str, "RECORD");
	mapping[11].tag = terminal;
	mapping[11].flag = 1;

	mapping[12].s.T = TAGGED;
	strcpy(mapping[12].str, "TAGGED");
	mapping[12].tag = terminal;
	mapping[12].flag = 1;

	mapping[13].s.T = UNION;
	strcpy(mapping[13].str, "UNION");
	mapping[13].tag = terminal;
	mapping[13].flag = 1;

	mapping[14].s.T = GET_VALUE;
	strcpy(mapping[14].str, "GET_VALUE");
	mapping[14].tag = terminal;
	mapping[14].flag = 1;

	mapping[15].s.T = PRINT;
	strcpy(mapping[15].str, "PRINT");
	mapping[15].tag = terminal;
	mapping[15].flag = 1;

	mapping[16].s.T = USE;
	strcpy(mapping[16].str, "USE");
	mapping[16].tag = terminal;
	mapping[16].flag = 1;

	mapping[17].s.T = WITH;
	strcpy(mapping[17].str, "WITH");
	mapping[17].tag = terminal;
	mapping[17].flag = 1;

	mapping[18].s.T = PARAMETERS;
	strcpy(mapping[18].str, "PARAMETERS");
	mapping[18].tag = terminal;
	mapping[18].flag = 1;

	mapping[19].s.T = TRUE;
	strcpy(mapping[19].str, "TRUE");
	mapping[19].tag = terminal;
	mapping[19].flag = 1;

	mapping[20].s.T = FALSE;
	strcpy(mapping[20].str, "FALSE");
	mapping[20].tag = terminal;
	mapping[20].flag = 1;

	mapping[21].s.T = TAKES;
	strcpy(mapping[21].str, "TAKES");
	mapping[21].tag = terminal;
	mapping[21].flag = 1;

	mapping[22].s.T = INPUT;
	strcpy(mapping[22].str, "INPUT");
	mapping[22].tag = terminal;
	mapping[22].flag = 1;

	mapping[23].s.T = RETURNS;
	strcpy(mapping[23].str, "RETURNS");
	mapping[23].tag = terminal;
	mapping[23].flag = 1;

	mapping[24].s.T = AND;
	strcpy(mapping[24].str, "AND");
	mapping[24].tag = terminal;
	mapping[24].flag = 1;

	mapping[25].s.T = OR;
	strcpy(mapping[25].str, "OR");
	mapping[25].tag = terminal;
	mapping[25].flag = 1;

	mapping[26].s.T = FOR;
	strcpy(mapping[26].str, "FOR");
	mapping[26].tag = terminal;
	mapping[26].flag = 1;

	mapping[27].s.T = IN;
	strcpy(mapping[27].str, "IN");
	mapping[27].tag = terminal;
	mapping[27].flag = 1;

	mapping[28].s.T = SWITCH;
	strcpy(mapping[28].str, "SWITCH");
	mapping[28].tag = terminal;
	mapping[28].flag = 1;

	mapping[29].s.T = CASE;
	strcpy(mapping[29].str, "CASE");
	mapping[29].tag = terminal;
	mapping[29].flag = 1;

	mapping[30].s.T = BREAK;
	strcpy(mapping[30].str, "BREAK");
	mapping[30].tag = terminal;
	mapping[30].flag = 1;

	mapping[31].s.T = DEFAULT;
	strcpy(mapping[31].str, "DEFAULT");
	mapping[31].tag = terminal;
	mapping[31].flag = 1;

	mapping[32].s.T = WHILE;
	strcpy(mapping[32].str, "WHILE");
	mapping[32].tag = terminal;
	mapping[32].flag = 1;

	mapping[33].s.T = PLUS;
	strcpy(mapping[33].str, "PLUS");
	mapping[33].tag = terminal;
	mapping[33].flag = 1;

	mapping[34].s.T = MINUS;
	strcpy(mapping[34].str, "MINUS");
	mapping[34].tag = terminal;
	mapping[34].flag = 1;

	mapping[35].s.T = MUL;
	strcpy(mapping[35].str, "MUL");
	mapping[35].tag = terminal;
	mapping[35].flag = 1;

	mapping[36].s.T = DIV;
	strcpy(mapping[36].str, "DIV");
	mapping[36].tag = terminal;
	mapping[36].flag = 1;

	mapping[37].s.T = LT;
	strcpy(mapping[37].str, "LT");
	mapping[37].tag = terminal;
	mapping[37].flag = 1;

	mapping[38].s.T = LE;
	strcpy(mapping[38].str, "LE");
	mapping[38].tag = terminal;
	mapping[38].flag = 1;

	mapping[39].s.T = GE;
	strcpy(mapping[39].str, "GE");
	mapping[39].tag = terminal;
	mapping[39].flag = 1;

	mapping[40].s.T = GT;
	strcpy(mapping[40].str, "GT");
	mapping[40].tag = terminal;
	mapping[40].flag = 1;

	mapping[41].s.T = EQ;
	strcpy(mapping[41].str, "EQ");
	mapping[41].tag = terminal;
	mapping[41].flag = 1;

	mapping[42].s.T = NE;
	strcpy(mapping[42].str, "NE");
	mapping[42].tag = terminal;
	mapping[42].flag = 1;

	mapping[43].s.T = DEF;
	strcpy(mapping[43].str, "DEF");
	mapping[43].tag = terminal;
	mapping[43].flag = 1;

	mapping[44].s.T = ENDDEF;
	strcpy(mapping[44].str, "ENDDEF");
	mapping[44].tag = terminal;
	mapping[44].flag = 1;

	mapping[45].s.T = DRIVERDEF;
	strcpy(mapping[45].str, "DRIVERDEF");
	mapping[45].tag = terminal;
	mapping[45].flag = 1;

	mapping[46].s.T = DRIVERENDDEF;
	strcpy(mapping[46].str, "DRIVERENDDEF");
	mapping[46].tag = terminal;
	mapping[46].flag = 1;

	mapping[47].s.T = COLON;
	strcpy(mapping[47].str, "COLON");
	mapping[47].tag = terminal;
	mapping[47].flag = 1;

	mapping[48].s.T = RANGEOP;
	strcpy(mapping[48].str, "RANGEOP");
	mapping[48].tag = terminal;
	mapping[48].flag = 1;

	mapping[49].s.T = SEMICOL;
	strcpy(mapping[49].str, "SEMICOL");
	mapping[49].tag = terminal;
	mapping[49].flag = 1;

	mapping[50].s.T = COMMA;
	strcpy(mapping[50].str, "COMMA");
	mapping[50].tag = terminal;
	mapping[50].flag = 1;

	mapping[51].s.T = ASSIGNOP;
	strcpy(mapping[51].str, "ASSIGNOP");
	mapping[51].tag = terminal;
	mapping[51].flag = 1;

	mapping[52].s.T = SQBO;
	strcpy(mapping[52].str, "SQBO");
	mapping[52].tag = terminal;
	mapping[52].flag = 1;

	mapping[53].s.T = SQBC;
	strcpy(mapping[53].str, "SQBC");
	mapping[53].tag = terminal;
	mapping[53].flag = 1;

	mapping[54].s.T = BO;
	strcpy(mapping[54].str, "BO");
	mapping[54].tag = terminal;
	mapping[54].flag = 1;

	mapping[55].s.T = BC;
	strcpy(mapping[55].str, "BC");
	mapping[55].tag = terminal;
	mapping[55].flag = 1;

	mapping[56].s.T = COMMENTMARK;
	strcpy(mapping[56].str, "COMMENTMARK");
	mapping[56].tag = terminal;
	mapping[56].flag = 1;

	mapping[57].s.T = ID;
	strcpy(mapping[57].str, "ID");
	mapping[57].tag = terminal;
	mapping[57].flag = 1;

	mapping[58].s.T = NUM;
	strcpy(mapping[58].str, "NUM");
	mapping[58].tag = terminal;
	mapping[58].flag = 1;

	mapping[59].s.T = RNUM;
	strcpy(mapping[59].str, "RNUM");
	mapping[59].tag = terminal;
	mapping[59].flag = 1;

	mapping[60].s.T = EPS ;
	strcpy(mapping[60].str, "EPS");
	mapping[60].tag = terminal;
	mapping[60].flag = 1;

	mapping[61].s.T = FEOF;
	strcpy(mapping[61].str, "FEOF");
	mapping[61].tag = terminal;
	mapping[61].flag = 1;

	mapping[62].s.T = BEGIN;
	strcpy(mapping[62].str, "BEGIN");
	mapping[62].tag = terminal;
	mapping[62].flag = 1;

	mapping[63].s.NT = program;
	strcpy(mapping[63].str, "program");
	mapping[63].tag = nonTerminal;
	mapping[63].flag = 1;

	mapping[64].s.NT = moduleDeclarations;
	strcpy(mapping[64].str, "moduleDeclarations");
	mapping[64].tag = nonTerminal;
	mapping[64].flag = 1;

	mapping[65].s.NT = moduleDeclaration;
	strcpy(mapping[65].str, "moduleDeclaration");
	mapping[65].tag = nonTerminal;
	mapping[65].flag = 1;

	mapping[66].s.NT = otherModules;
	strcpy(mapping[66].str, "otherModules");
	mapping[66].tag = nonTerminal;
	mapping[66].flag = 1;

	mapping[67].s.NT = driverModule;
	strcpy(mapping[67].str, "driverModule");
	mapping[67].tag = nonTerminal;
	mapping[67].flag = 1;

	mapping[68].s.NT = module;
	strcpy(mapping[68].str, "module");
	mapping[68].tag = nonTerminal;
	mapping[68].flag = 1;

	mapping[69].s.NT = ret;
	strcpy(mapping[69].str, "ret");
	mapping[69].tag = nonTerminal;
	mapping[69].flag = 1;

	mapping[70].s.NT = input_plist;
	strcpy(mapping[70].str, "input_plist");
	mapping[70].tag = nonTerminal;
	mapping[70].flag = 1;

	mapping[71].s.NT = more_input_plist;
	strcpy(mapping[71].str, "more_input_plist");
	mapping[71].tag = nonTerminal;
	mapping[71].flag = 1;

	mapping[72].s.NT = output_plist;
	strcpy(mapping[72].str, "output_plist");
	mapping[72].tag = nonTerminal;
	mapping[72].flag = 1;

	mapping[73].s.NT = more_output_plist;
	strcpy(mapping[73].str, "more_output_plist");
	mapping[73].tag = nonTerminal;
	mapping[73].flag = 1;

	mapping[74].s.NT = dataType;
	strcpy(mapping[74].str, "dataType");
	mapping[74].tag = nonTerminal;
	mapping[74].flag = 1;

	mapping[75].s.NT = range_arrays;
	strcpy(mapping[75].str, "range_arrays");
	mapping[75].tag = nonTerminal;
	mapping[75].flag = 1;

	mapping[76].s.NT = type;
	strcpy(mapping[76].str, "type");
	mapping[76].tag = nonTerminal;
	mapping[76].flag = 1;

	mapping[77].s.NT = moduleDef;
	strcpy(mapping[77].str, "moduleDef");
	mapping[77].tag = nonTerminal;
	mapping[77].flag = 1;

	mapping[78].s.NT = statements;
	strcpy(mapping[78].str, "statements");
	mapping[78].tag = nonTerminal;
	mapping[78].flag = 1;

	mapping[79].s.NT = statement;
	strcpy(mapping[79].str, "statement");
	mapping[79].tag = nonTerminal;
	mapping[79].flag = 1;

	mapping[80].s.NT = ioStmt;
	strcpy(mapping[80].str, "ioStmt");
	mapping[80].tag = nonTerminal;
	mapping[80].flag = 1;

	mapping[81].s.NT = var2;
	strcpy(mapping[81].str, "var2");
	mapping[81].tag = nonTerminal;
	mapping[81].flag = 1;

	mapping[82].s.NT = var;
	strcpy(mapping[82].str, "var");
	mapping[82].tag = nonTerminal;
	mapping[82].flag = 1;

	mapping[83].s.NT = whichId;
	strcpy(mapping[83].str, "whichId");
	mapping[83].tag = nonTerminal;
	mapping[83].flag = 1;

	mapping[84].s.NT = whichIndex;
	strcpy(mapping[84].str, "whichIndex");
	mapping[84].tag = nonTerminal;
	mapping[84].flag = 1;

	mapping[85].s.NT = simpleStmt;
	strcpy(mapping[85].str, "simpleStmt");
	mapping[85].tag = nonTerminal;
	mapping[85].flag = 1;

	mapping[86].s.NT = assignmentStmt;
	strcpy(mapping[86].str, "assignmentStmt");
	mapping[86].tag = nonTerminal;
	mapping[86].flag = 1;

	mapping[87].s.NT = whichStmt;
	strcpy(mapping[87].str, "whichStmt");
	mapping[87].tag = nonTerminal;
	mapping[87].flag = 1;

	mapping[88].s.NT = lvalueIDStmt;
	strcpy(mapping[88].str, "lvalueIDStmt");
	mapping[88].tag = nonTerminal;
	mapping[88].flag = 1;

	mapping[89].s.NT = lvalueARRStmt;
	strcpy(mapping[89].str, "lvalueARRStmt");
	mapping[89].tag = nonTerminal;
	mapping[89].flag = 1;

	mapping[90].s.NT = Index;
	strcpy(mapping[90].str, "Index");
	mapping[90].tag = nonTerminal;
	mapping[90].flag = 1;

	mapping[91].s.NT = moduleReuseStmt;
	strcpy(mapping[91].str, "moduleReuseStmt");
	mapping[91].tag = nonTerminal;
	mapping[91].flag = 1;

	mapping[92].s.NT = optional;
	strcpy(mapping[92].str, "optional");
	mapping[92].tag = nonTerminal;
	mapping[92].flag = 1;

	mapping[93].s.NT = idList;
	strcpy(mapping[93].str, "idList");
	mapping[93].tag = nonTerminal;
	mapping[93].flag = 1;

	mapping[94].s.NT = more_id;
	strcpy(mapping[94].str, "more_id");
	mapping[94].tag = nonTerminal;
	mapping[94].flag = 1;

	mapping[95].s.NT = expression;
	strcpy(mapping[95].str, "expression");
	mapping[95].tag = nonTerminal;
	mapping[95].flag = 1;

	mapping[96].s.NT = posExpr;
	strcpy(mapping[96].str, "posExpr");
	mapping[96].tag = nonTerminal;
	mapping[96].flag = 1;

	mapping[97].s.NT = posExpr2;
	strcpy(mapping[97].str, "posExpr2");
	mapping[97].tag = nonTerminal;
	mapping[97].flag = 1;

	mapping[98].s.NT = relExpr;
	strcpy(mapping[98].str, "relExpr");
	mapping[98].tag = nonTerminal;
	mapping[98].flag = 1;

	mapping[99].s.NT = relExpr2;
	strcpy(mapping[99].str, "relExpr2");
	mapping[99].tag = nonTerminal;
	mapping[99].flag = 1;

	mapping[100].s.NT = addExpr;
	strcpy(mapping[100].str, "addExpr");
	mapping[100].tag = nonTerminal;
	mapping[100].flag = 1;

	mapping[101].s.NT = addExpr2;
	strcpy(mapping[101].str, "addExpr2");
	mapping[101].tag = nonTerminal;
	mapping[101].flag = 1;

	mapping[102].s.NT = multExpr;
	strcpy(mapping[102].str, "multExpr");
	mapping[102].tag = nonTerminal;
	mapping[102].flag = 1;

	mapping[103].s.NT = multExpr2;
	strcpy(mapping[103].str, "multExpr2");
	mapping[103].tag = nonTerminal;
	mapping[103].flag = 1;

	mapping[104].s.NT = basicExpr;
	strcpy(mapping[104].str, "basicExpr");
	mapping[104].tag = nonTerminal;
	mapping[104].flag = 1;

	mapping[105].s.NT = logOp;
	strcpy(mapping[105].str, "logOp");
	mapping[105].tag = nonTerminal;
	mapping[105].flag = 1;

	mapping[106].s.NT = pmOp;
	strcpy(mapping[106].str, "pmOp");
	mapping[106].tag = nonTerminal;
	mapping[106].flag = 1;

	mapping[107].s.NT = mdOp;
	strcpy(mapping[107].str, "mdOp");
	mapping[107].tag = nonTerminal;
	mapping[107].flag = 1;

	mapping[108].s.NT = relOp;
	strcpy(mapping[108].str, "relOp");
	mapping[108].tag = nonTerminal;
	mapping[108].flag = 1;

	mapping[109].s.NT = declareStmt;
	strcpy(mapping[109].str, "declareStmt");
	mapping[109].tag = nonTerminal;
	mapping[109].flag = 1;

	mapping[110].s.NT = conditionalStmt;
	strcpy(mapping[110].str, "conditionalStmt");
	mapping[110].tag = nonTerminal;
	mapping[110].flag = 1;

	mapping[111].s.NT = caseStmt;
	strcpy(mapping[111].str, "caseStmt");
	mapping[111].tag = nonTerminal;
	mapping[111].flag = 1;

	mapping[112].s.NT = caseStmts;
	strcpy(mapping[112].str, "caseStmts");
	mapping[112].tag = nonTerminal;
	mapping[112].flag = 1;

	mapping[113].s.NT = value;
	strcpy(mapping[113].str, "value");
	mapping[113].tag = nonTerminal;
	mapping[113].flag = 1;

	mapping[114].s.NT = Default;
	strcpy(mapping[114].str, "Default");
	mapping[114].tag = nonTerminal;
	mapping[114].flag = 1;

	mapping[115].s.NT = iterativeStmt;
	strcpy(mapping[115].str, "iterativeStmt");
	mapping[115].tag = nonTerminal;
	mapping[115].flag = 1;

	mapping[116].s.NT = range;
	strcpy(mapping[116].str, "range");
	mapping[116].tag = nonTerminal;
	mapping[116].flag = 1;
}

















