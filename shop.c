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

typedef struct st{
	link_product object;
	struct st *lower;
	struct st *higher;
	int height;
} store;

/* Function Index */
int avl_height(store* p);
int avl_balanceFactor(store* h);
int encrypt(char *name);
void info_stock(store* h);
store* create_product(int code, char *nome, int price, int n_stock);
store* add_product(store* h,store* p);
store* avl_rotate_left(store* h);
store* avl_rotate_right(store* h);
store* avl_rotate_right_left(store* h);
store* avl_rotate_left_right(store* h);
store* avl_balance(store* h);
store* find_product(store* h, int code);
/* ------------- */


store* create_product(int code, char *name, int price, int n_stock){
	store* h = (store*) malloc(sizeof(store));
	link_product x = (link_product) malloc(sizeof(product));
	x->code = code;
	x->name = strdup(name);
	x->price = price;
	x->n_stock = n_stock;
	h->object = x;
	h->lower = NULL;
	h->higher = NULL;
	h->height = 1;
	return h;
}

store* add_product(store* h,store* p){
	if (h==NULL) return p;
	else if (p->object->code < h->object->code)
		h->lower = add_product(h->lower, p);
	else
		h->higher = add_product(h->higher, p);

	h = avl_balance(h);
	return h;
}

/* AVL FUNCTIONS */

int avl_height(store* h){
	if(h==NULL) return 0;
	return h->height;
}

store* avl_rotate_left(store* h){
	int height_left, height_right;
	store* x = h->higher;
	h->higher = x->lower;
	x->lower = h;
	height_left = avl_height(h->lower);
	height_right = avl_height(h->higher);
	h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	height_left = avl_height(x->lower);
	height_right = avl_height(x->higher);
	x->height = height_left > height_right ? height_left + 1 : height_right + 1;
	return x;
}

store* avl_rotate_right(store* h){
	
	int height_left, height_right;
	store* x = h->lower;
	h->lower = x->higher;
	x->higher = h;
	height_left = avl_height(h->lower);
	height_right = avl_height(h->higher);
	h->height = height_left > height_right ? height_left + 1: height_right + 1;
	height_left = avl_height(x->lower);
	height_right = avl_height(x->higher);
	x->height = height_left > height_right ? height_left + 1: height_right + 1;
	return x;
}

store* avl_rotate_left_right(store* h){
	if(h==NULL) return h;
	h->lower = avl_rotate_left(h->lower);
	return avl_rotate_right(h);
}

store* avl_rotate_right_left(store* h){
	if(h==NULL) return h;
	h->higher = avl_rotate_right(h->higher);
	return avl_rotate_left(h);
}

int avl_balanceFactor(store* h){
	if(h==NULL) return 0;
	return avl_height(h->lower) - avl_height(h->higher);
}

store* avl_balance(store* h){
	int balanceFactor;
	if (h == NULL) return h;
	balanceFactor = avl_balanceFactor(h);
	if(balanceFactor>1){
		if(avl_balanceFactor(h->lower)>0) h = avl_rotate_right(h);
		else h = avl_rotate_left_right(h);
	}
	else if(balanceFactor<-1){
		if(avl_balanceFactor(h->higher)<0) h = avl_rotate_left(h);
		else h = avl_rotate_right_left(h);
	}
	else{
		int height_left = avl_height(h->lower);
		int height_right = avl_height(h->higher);
		h->height = height_left > height_right ? height_left + 1 : height_right + 1;
	}
	return h;
}

void info_stock(store* h){
	if (h == NULL) return;
	info_stock(h->lower);
	printf("Product: %s  Price: %d  Total: %d\n", h->object->name, h->object->price, h->object->n_stock);
	info_stock(h->higher);
}


store* find_product(store* h, int code){
	if(h==NULL) return NULL;

	else if (code==h->object->code)
		return h;

	else if (code < h->object->code)
		return find_product(h->lower,code);

	else
		return find_product(h->higher,code);
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
	store* head = NULL;
	store* temp = NULL;
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
