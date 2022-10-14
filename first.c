#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct variable{
	char name[20];
	int val;
	struct variable* next;
}variable;

variable* var;
variable* outvar;
variable* varend;

typedef struct circuit{
	char op[20];
	int insize;
	variable* ins;
	variable* sels;
	variable* outs;
	struct circuit* next;
}circuit;

circuit* cstart;
circuit* cend;

void addcir(char comm[20], int is, variable* in, variable* sel, variable* out){
	circuit* newcir = (circuit*)malloc(sizeof(circuit));
	strcpy(newcir->op, comm);
	newcir->insize = is;
	newcir->ins = in;
	newcir->sels = sel;
	newcir->outs = out;
	newcir->next = NULL;
	if(cstart == NULL){
		cstart = newcir;
		cend = newcir;
	}
	else{
		cend->next = newcir;
		cend = cend->next;
	}
}

int findval(char nom[20]){
	if(strcmp(nom, "1") == 0) return 1;
	else if(strcmp(nom, "0") == 0) return 0;
	variable* insea = var;
	while(insea != NULL){
		if(strcmp(insea->name, nom) == 0) break;
		insea = insea->next;
	}
	return insea->val;
}

void putval(char nom[20], int value){
	variable* outsea = outvar;
	while(outsea != NULL){
		if(strcmp(outsea->name, nom) == 0){
			outsea->val = value;
			return;
		}
		outsea = outsea->next;
	}
	outsea = var;
	while(outsea != NULL){
		if(strcmp(outsea->name, nom) == 0){
			outsea->val = value;
			return;
		}
		varend = outsea;
		outsea = outsea->next;
	}
	variable* newt = (variable*)malloc(sizeof(variable));
	strcpy(newt->name, nom);
	newt->val = value;
	newt->next = NULL;
	varend->next = newt;
	varend = newt;
}

int main(int argc, char* argv[argc + 1]){
	FILE* infile = fopen(argv[1], "r");
	int slines;
	int count;
	int inum;
	char comm[20];
	var = NULL;
	outvar = NULL;
	cstart = NULL;
	cend = NULL;
	varend = NULL;
	
	
	//Make variable LL and circuit LL
	fscanf(infile, "%d\n", &slines);
	for(int i = 0; i < slines; i++){
		fscanf(infile, "%s ", comm);
		if(strcmp(comm, "INPUTVAR") == 0){
			fscanf(infile, "%d ", &inum);
			for(int i = 0; i < inum; i++){
				variable* new = (variable*)malloc(sizeof(variable));
				fscanf(infile, "%s ", new->name);
				new->val = 0;
				new->next = NULL;
				if(var == NULL){
					var = new;
					varend = new;
				}
				else{
					varend->next = new;
					varend = varend->next;
				}
			}
		}
		else if(strcmp(comm, "OUTPUTVAR") == 0){
			fscanf(infile, "%d", &count);
			variable* outvarend = NULL;
			for(int i = 0; i < count; i++){
				variable* new = (variable*)malloc(sizeof(variable));
				fscanf(infile, "%s", new->name);
				new->val = 0;
				new->next = NULL;
				if(outvar == NULL){
					outvar = new;
					outvarend = new;
				}
				else{
					outvarend->next = new;
					outvarend = outvarend->next;
				}
			}
		}
		else if(strcmp(comm, "NOT") == 0){
			variable* nin = (variable*)malloc(sizeof(variable));
			nin->val = 0;
			nin->next = NULL;
			variable* nout = (variable*)malloc(sizeof(variable));
			nout->val = 0;
			nout->next = NULL;
			fscanf(infile, "%s %s\n", nin->name, nout->name);
			addcir(comm, 0, nin, NULL, nout);
		}
		else if(strcmp(comm, "DECODER") == 0){
			int size;
			variable* in = NULL;
			variable* out = NULL;
			fscanf(infile, "%d ", &size);
			for(int i = 0; i < size; i++){
				variable* new = (variable*)malloc(sizeof(variable));
				fscanf(infile, "%s ", new->name);
				new->val = 0;
				new->next = NULL;
				if(in == NULL){
					in = new;
					varend = new;
				}
				else{
					varend->next = new;
					varend = varend->next;
				}
			}
			for(int i = 0; i < pow(2, size); i++){
				variable* new = (variable*)malloc(sizeof(variable));
				fscanf(infile, "%s ", new->name);
				new->val = 0;
				new->next = NULL;
				if(out == NULL){
					out = new;
					varend = new;
				}
				else{
					varend->next = new;
					varend = varend->next;
				}
			}
			addcir(comm, size, in, NULL, out);
		}
		else if(strcmp(comm, "MULTIPLEXER") == 0){
			int msize;
			variable* min = NULL;
			variable* msel = NULL;
			fscanf(infile, "%d ", &msize);
			for(int i = 0; i < msize; i++){
				variable* new = (variable*)malloc(sizeof(variable));
				fscanf(infile, "%s ", new->name);
				new->val = 0;
				new->next = NULL;
				if(min == NULL){
					min = new;
					varend = new;
				}
				else{
					varend->next = new;
					varend = varend->next;
				}
			}
			for(int i = 0; i < log2(msize); i++){
				variable* new = (variable*)malloc(sizeof(variable));
				fscanf(infile, "%s ", new->name);
				new->val = 0;
				new->next = NULL;
				if(msel == NULL){
					msel = new;
					varend = new;
				}
				else{
					varend->next = new;
					varend = varend->next;
				}
			}
			variable* mout = (variable*)malloc(sizeof(variable));
			mout->val = 0;
			mout->next = NULL;
			fscanf(infile, "%s\n", mout->name);
			addcir(comm, msize, min, msel, mout);
		}
		else{ //AND, OR, XOR
			variable* sin2 = (variable*)malloc(sizeof(variable));
			sin2->val = 0;
			sin2->next = NULL;
			variable* sin1 = (variable*)malloc(sizeof(variable));
			sin1->val = 0;
			sin1->next = sin2;
			variable* sout = (variable*)malloc(sizeof(variable));
			sout->val = 0;
			sout->next = NULL;
			fscanf(infile, "%s %s %s\n", sin1->name, sin2->name, sout->name);
			addcir(comm, 0, sin1, NULL, sout);;
		}
	}
	
	//Compute Output Values
	int numin;
	circuit* begin;
	while(fscanf(infile, "%d\n", &numin) != EOF){
		variable* iptr = var;
		for(int i = inum-1; i >= 0; i--){
			iptr->val = (numin>>i)&1;
			iptr = iptr->next;
		}
		
		begin = cstart;
		variable* follow;
		while(begin != NULL){
			int valo = 0;
			int valt = 0;
			if(strcmp(begin->op, "NOT")==0){
				valo = findval(begin->ins->name);
				putval(begin->outs->name, valo^1);
			}
			else if(strcmp(begin->op, "AND") == 0){
				follow = begin->ins;
				valo = findval(follow->name);
				follow = follow->next;
				valt = findval(follow->name);
				putval(begin->outs->name, valo&valt);
				
			}
			else if(strcmp(begin->op, "OR") == 0){
				follow = begin->ins;
				valo = findval(follow->name);
				follow = follow->next;
				valt = findval(follow->name);
				putval(begin->outs->name, valo|valt);
			}
			else if(strcmp(begin->op, "XOR") == 0){
				follow = begin->ins;
				valo = findval(follow->name);
				follow = follow->next;
				valt = findval(follow->name);
				putval(begin->outs->name, valo^valt);
			}
			else if(strcmp(begin->op, "DECODER")==0){
				int size = begin->insize;
				follow = begin->ins;
				for(int i = size - 1; i >= 0; i--){
					valo += pow(2, i) * findval(follow->name);
					follow = follow->next;
				}
				follow = begin->outs;
				for(int i = 0; i < pow(2, size); i++){
					if((i ^ (i>>1)) == valo) putval(follow->name, 1);
					else putval(follow->name, 0);
					follow = follow->next;
				}
			}
			else{ //Multiplexer
				int size = begin->insize;
				follow = begin->sels;
				for(int i = log2(size) - 1; i >= 0; i--){
					valo += pow(2, i) * findval(follow->name);
					follow = follow->next;
				}
				follow = begin->ins;
				for(int i = 0; i < size; i++){
					if((i ^ (i>>1)) == valo) break;
					follow = follow->next;
				}
				valt = findval(follow->name);
				putval(begin->outs->name, valt);
			}
			begin = begin->next;
		}
		
		variable* ptime = outvar;
		while(ptime != NULL){
			printf("%d\t", ptime->val);
			ptime = ptime->next;
		}
		printf("\n");
	}
	//Free var
	variable* ptr = var;
	variable* prev = NULL;
	while(ptr != NULL){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
	ptr = outvar;
	prev = NULL;
	//Free outvar
	while(ptr != NULL){
		prev = ptr;
		ptr = ptr->next;
		free(prev);
	}
	//Free circuit
	while(cstart != NULL){
		ptr = cstart->ins;
		while(ptr != NULL){
			prev = ptr;
			ptr = ptr->next;
			free(prev);
		}
		ptr = cstart->sels;
		while(ptr != NULL){
			prev = ptr;
			ptr = ptr->next;
			free(prev);
		}
		ptr = cstart->outs;
		while(ptr != NULL){
			prev = ptr;
			ptr = ptr->next;
			free(prev);
		}
		cend = cstart;
		cstart = cstart->next;
		free(cend);
	}
	return EXIT_SUCCESS;
}
