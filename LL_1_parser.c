#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

struct LL1 {
	char non_terminal[3];
	char terminal;
	char result[5];
	struct LL1 * next;
};
typedef struct LL1 LL1;

struct rules{
	char name[3];
	char production[10][10];
	char suivant[10];
	char premier[10];
	int premier_count;
	int suivant_count;
	int count;
	int premier_calculer;
	int suivant_calculer;
	struct rules * next;
};
typedef struct rules rules;

struct stack{
	char value[3];
	struct stack * next;
};

typedef struct stack stack;

void insert(rules ** p,rules * temp){
	if( *p == NULL){
		temp->next = NULL;
		temp->premier_count = 0;
		temp->suivant_count = 0;
		temp->premier_calculer = 0;
		temp->suivant_calculer =0;
		temp->premier[0] = '\0';
		temp->suivant[0] = '\0';
		*p = temp;

	}else{
		rules * x = (*p);
		while(x->next != NULL){
			x =x->next;
		}
		x->next = temp;
		temp->premier_count = 0;
		temp->premier_calculer = 0;
		temp->suivant_calculer =0;
		temp->suivant_count = 0;
		temp->premier[0] = '\0';
		temp->suivant[0] = '\0';
		temp->next = NULL;

	}
}
char * substring(char * string,int begin,int num){
	char* res;

	int j =0;
	res = malloc((num)*sizeof(char));
	for (int i = begin; i < begin+num; ++i)
	{
		res[j] = string[i];
		j++;
	}
	res[num] = '\0';
	return res;
}
void saisie(rules ** p){
	char *allRules= malloc(40*sizeof(char));
	char * token;
	rules * temp;
	int k = 0;
	while(1){

	scanf("%s",allRules);

	if(strcmp(allRules,".")==0){
		return ;
	}
	temp = malloc(sizeof(rules));
	temp->name[0]=allRules[0];
	temp->name[1] = '\0';
	char *help = substring(allRules,3,strlen(allRules)-3);
	token = strtok(help,"|");
	while(token != NULL){
		strcpy(temp->production[k],token);
		int len = strlen(temp->production[k]);
		temp->production[k][len] = '\0';
		++k;
		token = strtok(NULL,"|");
	}
	temp->count = k;
	k=0;
	if( *p == NULL){
		temp->next = NULL;
		temp->premier_calculer =0;
		temp->suivant_calculer = 0;
		*p = temp;

	}else{
		rules * x = (*p);
		while(x->next != NULL){
			x =x->next;
		}
		x->next = temp;
		temp->premier_calculer =0;
		temp->suivant_calculer = 0;
		temp->next = NULL;

	}

	}
}
void printRules(rules * p){
	while(p!= NULL){
		printf("%s->",p->name);
		for (int i = 0; i < p->count; ++i)
		{
			printf("%s",p->production[i]);
			if(i<p->count-1)printf("|");
		}
		p = p->next;
		printf("\n");
	}
}
int checkForRecursion(rules * p){

		for (int i = 0; i < p->count; ++i)
		{
			if(p->name[0] == p->production[i][0]){
				return 1;
			}
		}
	return 0;
}
rules * removeRecursion (rules * p){
	rules * no_rec = NULL;
	rules * iterator = p;
	while(iterator != NULL){
		if(checkForRecursion(iterator)){
			rules * temp = malloc(sizeof(rules));
			rules * temp2 = malloc(sizeof(rules));
			strcpy(temp->name,iterator->name);
			strcpy(temp2->name,iterator->name);
			strcat(temp2->name,"'");
			int k = 0;
			int l = 0;
			for (int i = 0; i < iterator->count; ++i)
			{
				if(iterator->production[i][0] != iterator->name[0]){
					strcpy(temp->production[k],iterator->production[i]);
					strcat(temp->production[k],temp2->name);
					++k;
				}else{
					char* help = substring(iterator->production[i],1,strlen(iterator->production[i]));
					strcpy(temp2->production[l],help);
					strcat(temp2->production[l],temp2->name);
					++l;
				}
			}
			if(k == 0){
				strcpy(temp->production[0],temp2->name);
				k++;
			}
			temp->count = k;
			strcpy(temp2->production[l],"&");
			temp2->count = l+1;
			 insert(&no_rec,temp);
			 insert(&no_rec,temp2);

		}else{
			 rules * temp = malloc(sizeof(rules));
			 *temp = * iterator;
			 insert(&no_rec,temp);
		}
		iterator = iterator->next;
	}
	return no_rec;
}
rules * lookForRule(rules * p,char name){
	while(p!= NULL){
		if(p->name[0] == name && p->name[1]!= '\''){
			return p;
		}
		p = p->next;
	}
	printf("la regle %c est non existant",name);
	exit(0);
}
int checkForEpsilon(char premier[]){
	for (int i = 0; i < strlen(premier); ++i)
	{
		if(premier[i] == '&'){
			return 1;

		}
	}
	return 0;
}
char * removeEpsilon(char * premier){
	int l = 0;
	for (int i = 0; i < strlen(premier); ++i)
	{
		if(premier[i] == '&'){
			l++;
		}
	}
	char * temp = malloc(strlen(premier));
	l=0;
	for (int i = 0; i < strlen(premier); ++i)
	{
		if(premier[i] != '&'){
			temp[l] = premier[i];
			l++;
		}
	}
    temp[l] = '\0';
	return temp;
}
int checkIfExist(char premier[],char pr){
	int i =0;
	while(i<strlen(premier)){
		if(premier[i] == pr){
			return 1;
		}
		i++;
	}
	return 0;
}
int checkIfNameExist(char production[],char name[]){
	if(strlen(name) == 2 ){
		for (int i = 0; i < strlen(production)-1; ++i)
		{
			if(name[0] == production[i] && name[1] == production[i+1])
				return (i+2);
		}
	}else{
		for (int i = 0; i < strlen(production); ++i)
		{
			if(name[0] == production[i] && production[i+1] != '\'')
				return (i+1);
		}
	}
	return 0;
}
rules * premierForOneRule(rules * p,rules * p2){


	if(p->premier_calculer == 0){
		int k =0;
		int max = p->count;
		for (int i = 0; i < max; ++i)
		{
			int l =1;
			if (isupper(p->production[i][0])){
				rules * x = lookForRule(p2,p->production[i][0]);
				rules * temp = premierForOneRule(x,p2);
                strcat(p->premier,temp->premier);
				while(checkForEpsilon(temp->premier) && isupper(p->production[i][l]) && l<strlen(p->production[i])){
					strcpy(p->premier,removeEpsilon(p->premier));
					rules * y = lookForRule(p2,p->production[i][l]);
					l++;
					rules * temp2 = premierForOneRule(y,p2);
					strcat(p->premier,temp2->premier);
					p->premier_count = strlen(p->premier);
					k = strlen(p->premier);
				}

			}else{

					if(!checkIfExist(p->premier,p->production[i][0])){

						p->premier[k] = p->production[i][0];
						++k;
						p->premier[k] = '\0';
				}
			}

		}
		p->premier_count = strlen(p->premier);
		p->premier_calculer =1;
		return p;
	}
	return p;
}
rules * premier(rules * p ){
	rules * iterator = p;
	while(iterator!=NULL){
		iterator = premierForOneRule(iterator,p);
		iterator = iterator->next;
	}
	return p;
}
rules * nameToRule(rules * p ,char * name){
	while(p !=NULL){
		if(strcmp(name,p->name) == 0){
			return p;
		}
		p = p->next;
	}
}
void printPremier(rules * p){
	while(p !=NULL){

		printf("Premier(%s) = {",p->name);
		for (int i = 0; i < p->premier_count; ++i)
		{
			if(i == p->premier_count - 1){
				printf("%c",p->premier[i]);
			}else{
				printf("%c,",p->premier[i]);
			}

		}
		printf("}\n");
		p = p->next;
	}
}
char *removeDuplicated(char table[]){
	char * temp;
	int k  =0;
	temp = malloc(strlen(table));
	for (int i = 0; i < strlen(table); ++i)
	{
		int exist = 0;
		for (int j = i+1; j < strlen(table); ++j)
		{
				if(table[i] == table[j]){
					exist = 1;
				}
		}
		if(exist == 0){
			temp[k] = table[i];
			++k;
		}
	}
	temp[k] = '\0';
	return temp;
}
rules * suivantForOneRule(rules * p,rules * p2,rules * h){
	if(p->suivant_calculer == 0){
        int k = 0;
		p->suivant_calculer = 1;
		while(p2 != NULL){
            int max = p2->count;
			for (int i = 0; i < max; ++i)
			{
				int d = checkIfNameExist(p2->production[i],p->name);
				if(d == 0){
					continue;
				}
				if(p2->production[i][d] == '\0'){
					if(strcmp(p->name,p2->name)!=0){
						if(!isupper(p2->production[i][d-1]) && p2->production[i][d-1] != '\'' ){
							p->suivant[p->suivant_count] = p2->production[i][d-1];
							p->suivant_count++;
							p->suivant[p->suivant_count] = p2->production[i][d-1];
						}else{
							rules * temp  = suivantForOneRule(p2,h,h);
							if(p->suivant[0] == '\0')
                                strcpy(p->suivant,temp->suivant);
                            else
                                strcat(p->suivant,temp->suivant);
							p->suivant_count = p->suivant_count + strlen(temp->suivant);
						}
					}
				}else if(d != 0){
					while(d<strlen(p2->production[i]))
					{

						if(isupper(p2->production[i][d])){
							rules * temp;
							if(p2->production[i][d+1] == '\''){
								char x[3];
								x[0] = p2->production[i][d];
								x[1] = '\'';
								temp = nameToRule(h,x);
								d =d+2;
							}else{
								temp = lookForRule(h,p2->production[i][d]);
								d++;
							}
							if(checkForEpsilon(temp->premier)){
									strcat(p->suivant,temp->premier);
									strcpy(p->suivant , removeEpsilon(p->suivant));
									p->suivant_count = p->suivant_count + temp->premier_count - 1;
									if(p2->production[i][d]=='\0' && strcmp(temp->name,p2->name)!=0){
                                            strcat(p->suivant,p2->suivant);
										p->suivant_count = p->suivant_count + strlen(p2->suivant);
									}
							}else{
									strcat(p->suivant,temp->premier);
									p->suivant_count = p->suivant_count + temp->premier_count;
									break;
								}

						}else{
							p->suivant[p->suivant_count] = p2->production[i][d];
							p->suivant_count++;
							p->suivant[p->suivant_count] = '\0';
							break;
						}
					}
				}

			}
			p2 = p2->next;
		}
		strcpy(p->suivant,removeDuplicated(p->suivant));
		p->suivant_count = strlen(p->suivant);
		return p;
	}
	return p;
}
rules * suivant(rules * p){
	rules * iterator = p;
	while(iterator!=NULL){
		iterator = suivantForOneRule(iterator,p,p);
		iterator = iterator->next;
	}
	return p;
}
void printSuivant(rules * p){
	while(p !=NULL){
		printf("Suivant(%s) = {",p->name);
		for (int i = 0; i < p->suivant_count; ++i)
		{
			if(i == p->suivant_count - 1){
				printf("%c",p->suivant[i]);
			}else{
				printf("%c,",p->suivant[i]);
			}

		}
		printf("}\n");
		p = p->next;
	}
}
int find( rules * p, rules * h,char c){
	for (int i = 0; i < p->count; ++i)
	{
		int j = 0;
		while (j < strlen(p->production[i]))
		{
			if(p->production[i][j] == c){
				return i;
			}
			if(isupper(p->production[i][j])){
				int k;
				if(p->production[i][j+1] != '\''){
					 k = find(lookForRule(h,p->production[i][j]),h,c);
					++j;
				}else{
					char x[3];
					x[0] = p->production[i][j];
					x[1] = '\'';
					k  = find(nameToRule(h,x),h,c);
					j = j+2;
				}
					if(k == 2){
						continue;
					}else{
						return i;
					}
			}else if(p->production[i][j] == c){
				return 1;
			}else if(p->production[i][j] == '&'){
				return 2;
			}
			break;

		}
	}
}
void insertLL1(LL1 ** table,LL1 * temp){
	temp->next = NULL;
	if( *table == NULL){
		*table = temp;
	}else{
		LL1 * x = (*table);
		while(x->next != NULL){
			x =x->next;
		}
		x->next = temp;
	}
}
LL1 * tableLL1(rules * p, rules * h){
	LL1 * table = NULL;
	while(p!=NULL){
		for (int i = 0; i < p->premier_count; ++i)
		{
			if(p->premier[i] == '&'){
				for (int j = 0; j < p->suivant_count; ++j)
				{
					LL1 * temp = malloc(sizeof(LL1));
					temp->result[0] ='&';
					strcpy(temp->non_terminal,p->name);
					temp->terminal = p->suivant[j];
					insertLL1(&table,temp);
				}
			}else{
				int k= find(p,h,p->premier[i]);
				LL1 * temp = malloc(sizeof(LL1));
				strcpy(temp->result ,p->production[k]);
				strcpy(temp->non_terminal,p->name);
				temp->terminal = p->premier[i];
				insertLL1(&table,temp);
			}
		}
		p = p->next;
	}
	return table;
}
void printLL1(LL1 * l){
	while(l!= NULL){
		printf("( %s,%c ) -> %s \n",l->non_terminal,l->terminal,l->result);
		l = l->next;
	}
}
void push(stack ** p,char * x ){
	stack * temp = malloc(sizeof(stack));
	strcpy(temp->value,x);
	if(*p == NULL){
		temp->next = NULL;
		*p = temp;
	}else{
		temp->next = *p;
		(*p)= temp;
	}
}
void pop(stack **p){
	stack * temp = *p;
	if(p !=NULL){
		(*p) = (*p)->next;
		free(temp);
	}else{
		printf("stack vide");
	}
}
char* peek(stack * p){
	if(p!=NULL){
		return p->value;
	}
}
char * findInLL1(LL1 * table,char * stack_peek , char text_char){
	while(table != NULL){
		if(strcmp(table->non_terminal,stack_peek)==0 && table->terminal == text_char){
			return table->result;
		}
		table = table->next;
	}
	return "0";
}
void printPile(stack * p){
	if(p == NULL)
		return;
	printf("\n%s\n_",p->value);
	printPile(p->next);

}
int checkIfvalid(LL1 * table , char * text,rules * prod){
	stack * p1 =NULL;
	int i = 0;
	strcat(text,"$");
	push(&p1,"$");
	push(&p1,prod->name);
	while(i<strlen(text)-1){
		char * x = findInLL1(table,peek(p1),text[i]);
		if(strcmp(x,"0")==0){
			return 0;
		}
		if(strcmp(x,"&")==0){
			pop(&p1);
		}else if(strlen(x) >1 ){
			int d = strlen(x)-1;
			pop(&p1);
			while(d>=0){
				if(x[d] =='\''){
					char * y = malloc(sizeof(x));
					y[0] = x[d-1];
					y[1] = x[d];
					y[2] = '\0';
					push(&p1,y);
					d=d-2;
				}else{
					char *f = malloc(2);
					f[0] = x[d];
					f[1] = '\0';
					push(&p1,f);
					d--;
				}
			}
		}else{
			pop(&p1);
			push(&p1,x);
		}
		char * check = peek(p1);
		if(strlen(peek(p1))==1){
			if(check[0] == text[i]){
				i++;
				pop(&p1);
			}
		}
	}
	while(strcmp(findInLL1(table,peek(p1),text[i]),"&")==0){
		pop(&p1);
		if(strcmp(peek(p1),"$")==0)
			break;
	}
	if(strcmp(peek(p1),"$")!=0){
		return 0;
	}else{
		return 1;
	}

}

void main(){
	char text[50];
	rules * prod = NULL;
	rules * prod_with_no_rec = NULL;
	LL1 * table_LL1 = NULL;
	saisie(&prod);
	printf("\n");
	printRules(prod);
	prod_with_no_rec = removeRecursion(prod);
	printf("\n");
	printRules(prod_with_no_rec);
	prod_with_no_rec->suivant[0] ='$';
	prod_with_no_rec->suivant[1] = '\0';
	prod_with_no_rec->suivant_count = 1;
	prod_with_no_rec = premier(prod_with_no_rec);
	printPremier(prod_with_no_rec);
	prod_with_no_rec = suivant(prod_with_no_rec);
	printf("\n");
	printSuivant(prod_with_no_rec);
	table_LL1 = tableLL1(prod_with_no_rec,prod_with_no_rec);
	printf("\n");
	printf("--table LL1-- \n");

	printLL1(table_LL1);

	while(1){
		printf("entrer un mot pour verifier \n");
		scanf("%s",text);
		if(strcmp(text,".")==0)break;

		int verify = checkIfvalid(table_LL1,text,prod_with_no_rec);
		switch (verify){
			case 1:
				printf("\033[0;32m le mot %s est accepté \n",text);
				printf("\033[0;37m");
				break;
			case 0:
				printf("\033[0;31m le mot %s n'est pas accepté  \n",text);
				printf("\033[0;37m");
				break;
		}
		printf("\n");
	}

}

