#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#define MAX 32
#define MAX_TREE_HT 100

typedef struct MH_Tree{
	char g;
	int len;
	int dec;
	struct MH_Tree *f;
	struct MH_Tree *r;
}MH_Tree;


MH_Tree *tree, *tree_temp, *t;


struct MH_Node{
    	char character;
    	int freq;
    	struct MH_Node *l, *r;
};
     
     
struct M_Heap{
    	int size;
    	int space;
    	struct MH_Node **array;
};
     
    
struct MH_Node* newNode(char character, int freq){
        struct MH_Node* temp = (struct MH_Node*) malloc(sizeof(struct MH_Node));
        temp->l = temp->r = NULL;
        temp->character = character;
        temp->freq = freq;
        return temp;
}
     
     
struct M_Heap* createM_Heap(int space){
        struct M_Heap* M_Heap = (struct M_Heap*) malloc(sizeof(struct M_Heap));
        M_Heap->size = 0;
        M_Heap->space = space;
        M_Heap->array = (struct MH_Node**)malloc(M_Heap->space * sizeof(struct MH_Node*));
        return M_Heap;
}

typedef struct code{
	char k;
	int l;
	int code_arr[16];
	struct code *p;
}code;

code *data, *rear, *front;


int frequency(int fd, int freq[]);
void HuffmanCodes(int fd2, char arr[], int freq[], int size, int f);
struct MH_Node* buildHuffmanTree(char arr[], int freq[], int size);		
struct M_Heap* createAndBuildM_Heap(char arr[], int freq[], int size);
void buildM_Heap(struct M_Heap* M_Heap);
void M_Heapify(struct M_Heap* M_Heap, int idx);
struct MH_Node* extractMin(struct M_Heap* M_Heap);
void insertM_Heap(struct M_Heap* M_Heap, struct MH_Node* MH_Node);
int isSizeOne(struct M_Heap* M_Heap);
void swapMH_Node(struct MH_Node** a, struct MH_Node** b);
void printArr(int arr[], int n);
int isLeaf(struct MH_Node* root);
void printCodes(int fd2, struct MH_Node* root, int t[], int top);
void compressfile(int fd1, int fd2, unsigned char a);
void decimaltobinary(int bin[], int f, int len);
int binarytodecimal(int bin[], int len);
void ExtractCodes(int fd2);
void AgainBuildHuffmanTree(int fd1, int size);
void decompress(int fd1, int fd2, int f);
int isroot(struct MH_Tree* tree_temp);



int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Give Proper Arguements\n");
	}
	if(strcmp(argv[1], "c") == 0){	
		unsigned char a = 0;
		int fd1, fd2, freq[128], i, size = 0, f;
		//for(i = 0; i < MAX; i++){
		//	a[i] = 0;
		//}
		char arr[128];
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("open failed:\n");
			exit(1);
	    	}
	    	fd2 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	    	if(fd2 == -1){
			perror("open failed:\n");
			exit(1);
		}
	
	    	for(i = 0; i < 128; i++){
	    		freq[i] = 0; 
	    		arr[i] = (char)i;	
	    	}
		    	
		f = frequency(fd1, freq);
		//for(i = 0; i < 128; i++){
		//	printf("%c\t%d\t%d\n", arr[i], freq[i], i);
		//}
		for( i = 0; i < 127; i++){
			if(freq[i] != 0){
				size++;
			}
		}
		//printf("\n\n%d\n\n", f);	
		HuffmanCodes(fd2, arr, freq, size, f);
		close(fd1);
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("open failed:\n");
			exit(1);
	    	}
		//printf("Entering File Compression Function\n");
		compressfile(fd1, fd2, a);
		close(fd1);
		close(fd2);	
	}
	else if(strcmp(argv[1], "d") == 0){
		int size, fd1, fd2, f;
		fd1 = open(argv[2], O_RDONLY);
		if(fd1 == -1){
			perror("open failed:\n");
			exit(1);
	    	}
	    	fd2 = open(argv[3], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	    	if(fd2 == -1){
			perror("open failed:\n");
			exit(1);
		}
		read(fd1, &size, sizeof(int));
		read(fd1, &f, sizeof(int));
//		printf("Size = %d\n", size);
		AgainBuildHuffmanTree(fd1, size);
		tree_temp = tree;
		decompress(fd1, fd2, f);
		close(fd1);
		close(fd2);	
	}
		
		
		
		
		
	
	return 0;
		
}


int frequency(int fd, int freq[]){
	char n;
	int k, p = 0;
	while(read(fd, &n, sizeof(char)) != 0){
		p++;
		k = n;
		freq[k]++;
	}
	return p;
}   


void HuffmanCodes(int fd2, char arr[], int freq[], int size, int f){
	int k = 0, i, n = 0;
        for(i = 0; i < 128; i++){
        	if(freq[i] != 0){
        		freq[k] = freq[i];
        		arr[k++] = arr[i];
        		n++;
        	}
        }
        write(fd2, &n, sizeof(int));
        write(fd2, &f, sizeof(int));
//        for(i = 0; i <= k; i++){
  //      	printf("%d\t%c\n", freq[i], arr[i]);
    //    }
      	struct MH_Node* root = buildHuffmanTree(arr, freq, size);
    	int t[MAX_TREE_HT], top = 0;
    	printCodes(fd2, root, t, top);
 
    	/*while(front->p != NULL){
    		printf("%c\t", front->k);
    		for(j = 0; j < (front->l); j++){
    			printf("%d", front->code_arr[j]);
    		}
    		printf("\n");
    		front = front->p;
    	}
    	printf("%c\t", front->k);
    	for(j = 0; j < (front->l); j++){
    		printf("%d", front->code_arr[j]);
   	}
    	printf("\n");*/
}
    
    
struct MH_Node* buildHuffmanTree(char arr[], int freq[], int size){
        struct MH_Node *l, *r, *top;
        struct M_Heap* M_Heap = createAndBuildM_Heap(arr, freq, size);
        while (!isSizeOne(M_Heap))
        {
            l = extractMin(M_Heap);
            
            r = extractMin(M_Heap);
           
            top = newNode('$', l->freq + r->freq);
            top->l = l;
            top->r = r;
            insertM_Heap(M_Heap, top);
        }
        return extractMin(M_Heap);
}   


struct M_Heap* createAndBuildM_Heap(char arr[], int freq[], int size){
    	int i;
        struct M_Heap* M_Heap = createM_Heap(size);
        for (i = 0; i < size; ++i)
            M_Heap->array[i] = newNode(arr[i], freq[i]);
        M_Heap->size = size;
        buildM_Heap(M_Heap);
        return M_Heap;
} 


void buildM_Heap(struct M_Heap* M_Heap){
        int n = M_Heap->size - 1;
        int i;
        for (i = (n - 1) / 2; i >= 0; --i)
    		M_Heapify(M_Heap, i);
}


void M_Heapify(struct M_Heap* M_Heap, int idx){
        int smallest = idx;
        int l = 2 * idx + 1;
        int r = 2 * idx + 2;
     
        if (l < M_Heap->size && M_Heap->array[l]->freq < M_Heap->array[smallest]->freq)
    		smallest = l;
     
    	if (r < M_Heap->size && M_Heap->array[r]->freq < M_Heap->array[smallest]->freq)
    		smallest = r;
     
        if (smallest != idx)
        {
    		swapMH_Node(&M_Heap->array[smallest], &M_Heap->array[idx]);
    	//	printf("For s = %d, l = %d, r = %d\n", smallest, l, r);
        //	for(i = 0; i < (M_Heap->size); i++){
	//		printf("%d\t", M_Heap->array[i]->freq);
	//	}
	//	printf("\n\n\n");
    		M_Heapify(M_Heap, smallest);
        }
        //else{
        //printf("For s = %d, l = %d, r = %d\n", smallest, l, r);
        //for(i = 0; i < (M_Heap->size); i++){
	//	printf("%d\t", M_Heap->array[i]->freq);
	//}
	//printf("\n\n\n");
	//}    
}


struct MH_Node* extractMin(struct M_Heap* M_Heap){
        struct MH_Node* temp = M_Heap->array[0];
        M_Heap->array[0] = M_Heap->array[M_Heap->size - 1];
        --M_Heap->size;
        //printf("extracted  = %d\n", temp->freq);
        M_Heapify(M_Heap, 0);
        return temp;
}


void insertM_Heap(struct M_Heap* M_Heap, struct MH_Node* MH_Node){
     
        int i = M_Heap->size;
        ++M_Heap->size;
        while (i && MH_Node->freq < M_Heap->array[(i - 1)/2]->freq)
        {
    		M_Heap->array[i] = M_Heap->array[(i - 1)/2];
    		i = (i - 1)/2;
        }
        M_Heap->array[i] = MH_Node;
        //for(j = 0; j < (M_Heap->size); j++){
	//	printf("%d\t", M_Heap->array[j]->freq);
	//}
	//printf("\n\n\n");
}


int isSizeOne(struct M_Heap* M_Heap){
        return (M_Heap->size == 1);
}


void swapMH_Node(struct MH_Node** a, struct MH_Node** b){
        struct MH_Node* t = *a;
        *a = *b;
        *b = t;
}


void printArr(int t[], int n){
        int i;
        for (i = 0; i < n; ++i){
    		printf("%d", t[i]);
    	}
    	printf("\n");
}
     
          
int isLeaf(struct MH_Node* root){
        return !(root->l) && !(root->r) ;
}

int k = 0;
void printCodes(int fd2, struct MH_Node* root, int t[], int top){
	int i;
	
	if (root->l){
        	t[top] = 0;
                printCodes(fd2, root->l, t, top + 1);
        }
     
        if (root->r){
                t[top] = 1;
                printCodes(fd2, root->r, t, top + 1);
        }
     
        if (isLeaf(root)){
      //          printf("%c: ", root->character);
        //        printArr(t, top);
                data = (code*)malloc(sizeof(code));
                tree = (MH_Tree*)malloc(sizeof(MH_Tree));
                data->p = NULL;
                data->k = root->character;
                tree->g = root->character;
                write(fd2, &tree->g, sizeof(char));
                for(i = 0; i < top; i++){
                	data->code_arr[i] = t[i];
                }
                tree->len = top;
                write(fd2, &tree->len, sizeof(int));
                tree->dec = binarytodecimal(data->code_arr, top);
                write(fd2, &tree->dec, sizeof(int));
                data->l = top;
                data->p = NULL;
                //printf("%c\t", data->k);
    		//for(j = 0; j < top; j++){
    		//	printf("%d", data->code_arr[j]);
    		//}
    		//printf("\n");
    		if(k == 0){
    			front = rear = data;
    			k++;
    		}
    		else{
    			rear->p = data;
    			rear = rear->p;
    		}
    		//front = front->p;
	}
}


void compressfile(int fd1, int fd2, unsigned char a){
	char n;
	int h = 0, i;
	//printf("Inside File Compression Function\n");
	while(read(fd1, &n, sizeof(char)) != 0){
		//printf("Read Character :- %c\n", n);
		rear = front;
		while(rear->k != n && rear->p != NULL){
			rear = rear->p;
		}
		//printf("rear->k = %c\nrear->l = %d\n", rear->k, rear->l);
		if(rear->k == n){
			for(i = 0; i < rear->l; i++){
				if(h < 7){
					if(rear->code_arr[i] == 1){
		//				printf("Got 1\n");
						a++;
						a = a << 1;
						h++;
					}
					else if(rear->code_arr[i] == 0){
		//				printf("Got 0\n");
						a = a << 1;
						h++;
					}
		//			printf("a = %d\n", a);
				}
				else if(h == 7){
		//			printf("h is 8 now\n");
					if(rear->code_arr[i] == 1){
		//				printf("Got 1\n");
						a++;
						h = 0;
					}
					else{
		//				printf("Got 0\n");
						h = 0;
					}
				//printf("a = %d\n", a);		
				write(fd2, &a, sizeof(char));
				a = 0;
		//		printf("a = %d\n", a);
				}
			}
		}
	}
	for(i = 0; i < 7 - h; i++){
		a = a << 1;
	}
	write(fd2, &a, sizeof(char));
	//printf("Last a = %d\n", a);	
}
				
void decimaltobinary(int bin[], int f, int len){
	//printf("Converting decimal to binary\n");
	int i = 0, t;
	for(i = 0;i < len; i++){
		bin[i] = 0;
	}
	i = 0;
	while(f != 0 && i < len){
		if(f % 2 == 0){
		
			bin[i++] = 0;
			f = f / 2;
		}
		else if(f % 2 == 1){
			bin[i++] = 1;
			f = f / 2;
		}		
	}
	for(i = 0; i < (len)/2; i++){
		t = bin[i];
		bin[i] = bin[(len - 1) - i];
		bin[(len - 1) - i] = t;
	}
}

int binarytodecimal(int bin[], int len){
	int i = 0, sum = 0;
	for(i = 0; i < len; i++){
		sum = sum + bin[i]*pow(2, (len - 1) - i);
	}
	return sum;
}
				

void ExtractCodes(int fd1){
	//printf("Extracting codes\n");	
	
	
	read(fd1, &t->g, sizeof(char));
	read(fd1, &t->len, sizeof(int));
	read(fd1, &t->dec, sizeof(int));
	//printf("\n\n%c\t%d\t%d\n\n", tree->g, tree->len, tree->dec);
}
    
    
void AgainBuildHuffmanTree(int fd1, int size){
	int i = 0, j, k;
	tree = (MH_Tree *)malloc(sizeof(MH_Tree));
	tree_temp = tree;
	tree->f = NULL;
	tree->r = NULL;
	t = (MH_Tree*)malloc(sizeof(MH_Tree));
	t->f = NULL;
	t->r = NULL;
	for(k = 0; k < size; k++){ 
		tree_temp = tree;
		ExtractCodes(fd1);
		int bin[MAX], bin_con[MAX];
		for(i = 0; i < MAX; i++){
			bin[i] = bin_con[i] = 0;
		}
		decimaltobinary(bin, t->dec, t->len);
	//	for(i = 0; i < MAX; i++){
	//		printf("%d ", bin[i]);
	//	
	//	}
	//	printf("\n\n");
	    	for(i = 0; i < t->len; i++){
	    		bin_con[i] = bin[i];
	    	}
	    	//for(i = 0; i < t->len; i++){
		//	printf("%d ", bin_con[i]);
		
		//}
		//printf("\n");
    		for(j = 0; j < t->len; j++){
    			if(bin_con[j] == 0){
    		//		printf("Got 0\n");
    				if(tree_temp->f == NULL){
    					tree_temp->f = (MH_Tree*)malloc(sizeof(MH_Tree));
    				}
    		//		printf("s1\n");
    				tree_temp = tree_temp->f;
    			}
    			else if(bin_con[j] == 1){
    		//		printf("Got 1\n");
    				if(tree_temp->r == NULL){
    					tree_temp->r = (MH_Tree*)malloc(sizeof(MH_Tree));
    				}
    				tree_temp = tree_temp->r;
    			}
    		}
    		//printf("s2\n");
    		tree_temp->g = t->g;
    		//printf("Inserting %c\n", tree_temp->g);
    		tree_temp->len = t->len;
    		//printf("Inserting %d\n", tree_temp->len);
    		tree_temp->dec = t->dec;
   // 		printf("Inserting %d\n", tree_temp->dec);
    		tree_temp->f = NULL;
    		tree_temp->r = NULL;
    		tree_temp = tree;
    	}
    	
}


void decompress(int fd1, int fd2, int f){
	//printf("Into the Decompression function\n");
	int inp[8], i, k = 0;
	unsigned char p;
	read(fd1, &p, sizeof(char));
	//printf("%c\n", p);
	decimaltobinary(inp, p, 8);
//	for(i = 0; i < 8; i++){
//		printf("%d ", inp[i]);
//	}
//	printf("\n");
	tree_temp = tree;
//	printf("***** f = %d *****\n", f);
	for(i = 0; i < 8 && k < f; i++){
		if(!isroot(tree_temp)){
//			printf("NO ROOT\n");
			if(i != 7){
				if(inp[i] == 0){
//					printf("Got 0 --\n");
					tree_temp = tree_temp->f;
				}
				if(inp[i] == 1){
//					printf("Got 1 --\n");
					tree_temp = tree_temp->r;
				}
			}
			else{
				if(inp[i] == 0){
//					printf("Got 0\n");
					tree_temp = tree_temp->f;
				}
				if(inp[i] == 1){
//					printf("Got 1\n");
					tree_temp = tree_temp->r;
				}
				if(read(fd1, &p, sizeof(char)) != 0){
//					printf("%d\n", p);
					decimaltobinary(inp, p, 8);
//					for(j = 0; j < 8; j++){
//						printf("%d ", inp[j]);
//					}
//					printf("\n");
					i = -1;
				}
				else{
					break;
				}
			}
				
		}
		
		else{
//			printf("Root Countered\n");
			k++;
//			printf("%d\n", k);
			write(fd2, &tree_temp->g, sizeof(char));
			tree_temp = tree;
			i--;
		}
	
	}
}
	    
    
    
int isroot(struct MH_Tree* tree_temp){
	return !(tree_temp->f) && !(tree_temp->r);
}			
	












