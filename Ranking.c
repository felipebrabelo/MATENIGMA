#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define NOME_ARQUIVO "rank.dat"
typedef struct{
	int score;
	char name[100];
}player;

fopen_e_teste(char* caminho, char* modo){
	FILE* f;
	f = fopen(caminho, modo);
	if(f==NULL){
		perror("Erro ao tentar abrir o arquivo. \n");
		exit(1);
	}
	return f;
}

int compara(const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

void criaranking(player* list){
	
	int i;
    FILE* pf = fopen_e_teste(NOME_ARQUIVO, "wb+");
    
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
	player teste;
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
	//for(i=0; i<10; i++){
	while(fread(&teste, sizeof(player), 1, fp) > 0){
		list[zerado] = teste;
		if(list[10].score>list[zerado].score){
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