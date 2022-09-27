#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define NOME_ARQUIVO "rank.dat"
typedef struct{
	int score;
	char name[100];
}player;

int compara(const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

void criaranking(player* list){
	
	int i;
    FILE* pf = fopen(NOME_ARQUIVO,"wb+");
    
	qsort(list, 11, sizeof(player), compara);
	
	for(i=0; i<10 && list[i].score>0;i++){
		printf("%d %s\n", list[i].score, list[i].name);
		fwrite(&list[i], sizeof(player), 1, pf);
	}
	fclose(pf);
}

void main(){
	FILE* fp = fopen(NOME_ARQUIVO,"rb");
	player* list;
	int i;
	int x;
	int cont = 0;
	int zerado = 0;
    char user[20]; 
	list = (player*)malloc(11*sizeof(player));
	scanf("%d", &x);
	list[10].score = x;
	for(i=0; i<10; i++){
		list[i].score = 0;
	}
	while(fread(&list[i], sizeof(player), 1, fp) > 0){
		if(list[10].score>list[i].score){
			cont++;
		}
		zerado++;
    }
	if(cont || !zerado){
		scanf("%s", user);
		strcpy(list[10].name, user);
		criaranking(list);
	}
	fclose(fp);
}