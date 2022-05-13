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
	char follow[10];
	char first[10];
	int first_count;
	int follow_count;
	int count;
	int first_calculer;
	int follow_calculer;
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
		temp->first_count = 0;
		temp->follow_count = 0;
		temp->first_calculer = 0;
		temp->follow_calculer =0;
		temp->first[0] = '\0';
		temp->follow[0] = '\0';
		*p = temp;

	}else{
		rules * x = (*p);
		while(x->next != NULL){
			x =x->next;
		}
		x->next = temp;
		temp->first_count = 0;
		temp->first_calculer = 0;
		temp->follow_calculer =0;
		temp->follow_count = 0;
		temp->first[0] = '\0';
		temp->follow[0] = '\0';
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
		temp->first_calculer =0;
		temp->follow_calculer = 0;
		*p = temp;

	}else{
		rules * x = (*p);
		while(x->next != NULL){
			x =x->next;
		}
		x->next = temp;
		temp->first_calculer =0;
		temp->follow_calculer = 0;
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
int checkForEpsilon(char first[]){
	for (int i = 0; i < strlen(first); ++i)
	{
		if(first[i] == '&'){
			return 1;

		}
	}
	return 0;
}
char * removeEpsilon(char * first){
	int l = 0;
	for (int i = 0; i < strlen(first); ++i)
	{
		if(first[i] == '&'){
			l++;
		}
	}
	char * temp = malloc(strlen(first));
	l=0;
	for (int i = 0; i < strlen(first); ++i)
	{
		if(first[i] != '&'){
			temp[l] = first[i];
			l++;
		}
	}
    temp[l] = '\0';
	return temp;
}
int checkIfExist(char first[],char pr){
	int i =0;
	while(i<strlen(first)){
		if(first[i] == pr){
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
rules * firstForOneRule(rules * p,rules * p2){


	if(p->first_calculer == 0){
		int k =0;
		int max = p->count;
		for (int i = 0; i < max; ++i)
		{
			int l =0;
			if (isupper(p->production[i][l])){
				rules * x = lookForRule(p2,p->production[i][l]);
				rules * temp = firstForOneRule(x,p2);
                strcat(p->first,temp->first);
                while(checkForEpsilon(temp->first) && isupper(p->production[i][l+1])){
                	l++;
					rules * y = lookForRule(p2,p->production[i][l]);
					rules * temp2 = firstForOneRule(y,p2);
					strcat(p->first,temp2->first);	
                }
                  strcpy(p->first,removeEpsilon(p->first));
                  strcpy(p->first,removeDuplicated(p->first));

			}else{

					if(!checkIfExist(p->first,p->production[i][0])){

						p->first[k] = p->production[i][0];
						++k;
						p->first[k] = '\0';
				}
			}

		}
		p->first_count = strlen(p->first);
		p->first_calculer =1;
		return p;
	}
	return p;
}
rules * first(rules * p ){
	rules * iterator = p;
	while(iterator!=NULL){
		iterator = firstForOneRule(iterator,p);
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
void printfirst(rules * p){
	while(p !=NULL){

		printf("first(%s) = {",p->name);
		for (int i = 0; i < p->first_count; ++i)
		{
			if(i == p->first_count - 1){
				printf("%c",p->first[i]);
			}else{
				printf("%c,",p->first[i]);
			}

		}
		printf("}\n");
		p = p->next;
	}
}

rules * followForOneRule(rules * p,rules * p2,rules * h){
	if(p->follow_calculer == 0){
        int k = 0;
		p->follow_calculer = 1;
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
							p->follow[p->follow_count] = p2->production[i][d-1];
							p->follow_count++;
							//p->follow[p->follow_count] = p2->production[i][d-1];
						}else{
							rules * temp  = followForOneRule(p2,h,h);
							if(p->follow[0] == '\0')
                                strcpy(p->follow,temp->follow);
                            else
                                strcat(p->follow,temp->follow);
							p->follow_count = p->follow_count + strlen(temp->follow);
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
							if(checkForEpsilon(temp->first)){
									strcat(p->follow,temp->first);
									strcpy(p->follow , removeEpsilon(p->follow));
									p->follow_count = p->follow_count + temp->first_count - 1;
									if(p2->production[i][d]=='\0' && strcmp(temp->name,p2->name)!=0){
                                            strcat(p->follow,p2->follow);
										p->follow_count = p->follow_count + strlen(p2->follow);
									}
							}else{
									strcat(p->follow,temp->first);
									p->follow_count = p->follow_count + temp->first_count;
									break;
								}

						}else{
							p->follow[p->follow_count] = p2->production[i][d];
							p->follow_count++;
							p->follow[p->follow_count] = '\0';
							break;
						}
					}
				}

			}
			p2 = p2->next;
		}
		strcpy(p->follow,removeDuplicated(p->follow));
		p->follow_count = strlen(p->follow);
		return p;
	}
	return p;
}
rules * follow(rules * p){
	rules * iterator = p;
	while(iterator!=NULL){
		iterator = followForOneRule(iterator,p,p);
		iterator = iterator->next;
	}
	return p;
}
void printfollow(rules * p){
	while(p !=NULL){
		printf("follow(%s) = {",p->name);
		for (int i = 0; i < p->follow_count; ++i)
		{
			if(i == p->follow_count - 1){
				printf("%c",p->follow[i]);
			}else{
				printf("%c,",p->follow[i]);
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
		for (int i = 0; i < p->first_count; ++i)
		{
			if(p->first[i] == '&'){
				for (int j = 0; j < p->follow_count; ++j)
				{
					LL1 * temp = malloc(sizeof(LL1));
					temp->result[0] ='&';
					strcpy(temp->non_terminal,p->name);
					temp->terminal = p->follow[j];
					insertLL1(&table,temp);
				}
			}else{
				int k= find(p,h,p->first[i]);
				LL1 * temp = malloc(sizeof(LL1));
				strcpy(temp->result ,p->production[k]);
				strcpy(temp->non_terminal,p->name);
				temp->terminal = p->first[i];
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
		printf("empty stack");
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
	while(i<strlen(text)){
		char * check = peek(p1);
		if(check[0] == text[i]){
				i++;
				if(check[0] == '$')
					return 1;
				pop(&p1);
		}else{
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
		}
	}
		return 0;

}

void main(){
	char text[50];
	rules * prod = NULL;
	rules * prod_with_no_rec = NULL;
	LL1 * table_LL1 = NULL;
	printf("enter your grammar : \n");
	saisie(&prod);
	printf("\n");
	printRules(prod);
	prod_with_no_rec = removeRecursion(prod);
	printf("\n");
	printRules(prod_with_no_rec);
	prod_with_no_rec->follow[0] ='$';
	prod_with_no_rec->follow[1] = '\0';
	prod_with_no_rec->follow_count = 1;
	prod_with_no_rec = first(prod_with_no_rec);
	printfirst(prod_with_no_rec);
	prod_with_no_rec = follow(prod_with_no_rec);
	printf("\n");
	printfollow(prod_with_no_rec);
	table_LL1 = tableLL1(prod_with_no_rec,prod_with_no_rec);
	printf("\n");
	printf("--LL1 table-- \n");

	printLL1(table_LL1);

	while(1){
		printf("enter a word to verify \n");
		scanf("%s",text);
		if(strcmp(text,".")==0)break;

		int verify = checkIfvalid(table_LL1,text,prod_with_no_rec);
		switch (verify){
			case 1:
				printf("\033[0;32m the word %s is accepted \n",text);
				printf("\033[0;37m");
				break;
			case 0:
				printf("\033[0;31m the %s is rejected  \n",text);
				printf("\033[0;37m");
				break;
		}
		printf("\n");
	}

}

