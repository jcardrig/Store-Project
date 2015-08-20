#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct prod{
	char *name;
	int code;
	int price;
	int n_stock;
} product;

typedef product *link_product;

typedef struct store{
	link_product object;
	struct store *menor;
	struct store *maior;
	int height;
}*Store;

Store create_product(int code, char *nome, int price, int n_stock);
Store add_product(Store h,Store p);
int height(Store p);
Store rotL(Store h);
Store rotR(Store h);
Store rotRL(Store h);
Store rotLR(Store h);
int balance(Store h);
Store balance_arv(Store h);

Store create_product(int code, char *name, int price, int n_stock){
	Store h = (Store) malloc(sizeof(struct store));
	link_product x = (link_product) malloc(sizeof(product));
	x->code = code;
	x->name = strdup(name);
	x->price = price;
	x->n_stock = n_stock;
	h->object = x;
	h->menor = NULL;
	h->maior = NULL;
	h->height = 1;
	return h;
}

Store add_product(Store h,Store p){
	if (h==NULL) return p;
	else if (p->object->code < h->object->code)
		h->menor = add_product(h->menor, p);
	else
		h->maior = add_product(h->maior, p);

	h = balance_arv(h);
	return h;
}

/* AVL FUNCTIONS */

int height(Store h){
	if(h==NULL) return 0;
	return h->height;
}

Store rotL(Store h){
	int height_left, height_right;
	Store x = h->maior;
	h->maior = x->menor;
	x->menor = h;
	height_left = height(h->menor);
	height_right = height(h->maior);
	h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	height_left = height(x->menor);
	height_right = height(x->maior);
	x->height = height_left > height_right ? height_left + 1 : height_right + 1;
	return x;
}

Store rotR(Store h){
	
	int height_left, height_right;
	Store x = h->menor;
	h->menor = x->maior;
	x->maior = h;
	height_left = height(h->menor);
	height_right = height(h->maior);
	h->height = height_left > height_right ? height_left + 1: height_right + 1;
	height_left = height(x->menor);
	height_right = height(x->maior);
	x->height = height_left > height_right ? height_left + 1: height_right + 1;
	return x;
}

Store rotLR(Store h){
	if(h==NULL) return h;
	h->menor = rotL(h->menor);
	return rotR(h);
}

Store rotRL(Store h){
	if(h==NULL) return h;
	h->maior = rotR(h->maior);
	return rotL(h);
}

int balance(Store h){
	if(h==NULL) return 0;
	return height(h->menor) - height(h->maior);
}

Store balance_arv(Store h){
	int balanceFactor;
	if (h == NULL) return h;
	balanceFactor = balance(h);
	if(balanceFactor>1){
		if(balance(h->menor)>0) h = rotR(h);
		else h = rotLR(h);
	}
	else if(balanceFactor<-1){
		if(balance(h->maior)<0) h = rotL(h);
		else h = rotRL(h);
	}
	else{
		int height_left = height(h->menor);
		int height_right = height(h->maior);
		h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	}
	return h;
}

void info_stock(Store h){
	if (h == NULL) return;
	info_stock(h->menor);
	printf("Product: %s  Price: %d  Total: %d\n", h->object->name, h->object->price, h->object->n_stock);
	info_stock(h->maior);
}


Store find_product(Store h, int code){
	if(h==NULL) return NULL;

	else if (code==h->object->code)
		return h;

	else if (code < h->object->code)
		return find_product(h->menor,code);

	else
		return find_product(h->maior,code);
}



/* END AVL FUCNTIONS */

int encrypt(char *name){
	int c, sum=0, i, l = strlen(name);
	for(i=0; i < l; i++){
		c = name[i];
		sum += c;
	}
	return sum;
}

int main(){
	Store head = NULL;
	Store temp = NULL;
	int price, n_stock, code, quant;
	char name[35];
	char command[100];
	scanf("%s", command);
	while(strcmp(command,"quit") != 0){

		if(strcmp(command,"add") == 0){
			scanf("%s %d %d" , name, &price, &n_stock);
			code = encrypt(name);
			head = add_product(head, create_product(code, name, price, n_stock));
		}

		else if(strcmp(command, "buy") == 0){
			scanf("%d %s", &quant, name);
			code = encrypt(name);
			temp = find_product(head,code);
			if (temp == NULL)
				printf("ERROR: %s not found in the inventory.\n",name);
			else if (temp->object->n_stock >= quant)
				temp->object->n_stock -= quant; 
			else
				printf("ERROR: Only %d %ss available.\n", temp->object->n_stock, name);
		}

		else if(strcmp(command, "supply") == 0){
			scanf("%d %s",&quant, name);
			code = encrypt(name);
			temp = find_product(head,code);
			if (temp != NULL)
				temp->object->n_stock += quant;
			else
				printf("ERROR: %s not found in the inventory.\n",name);
		}

		else if(strcmp(command, "fstock") == 0){
			info_stock(head);
		}

		scanf("%s", command);
	}
	return 0;
}
