#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	char value;
	long long freq;
}tree_node;

typedef struct link_tree{
	tree_node * tree;
	struct link_tree * next;
}link_tree;

tree_node * make_tree(char value, long long freq);
tree_node * merge_tree(tree_node *tree1, tree_node *tree2);
void readBit(FILE * fptr, unsigned char * bit, unsigned char * bit_location, unsigned char * byte_data);
tree_node *read_header(FILE * fptr);
void Tree_print(tree_node * tn, int level);


tree_node * make_tree(char value, long long freq){
	tree_node * tree = malloc(sizeof(tree_node));
	tree -> value = value;
	tree -> freq = freq;
	tree -> left = NULL;
	tree -> right = NULL;
	return tree;
}

tree_node * merge_tree(tree_node *tree1, tree_node *tree2){
	tree_node *tree = make_tree(0,0);
	tree -> left = tree1;
	tree -> right = tree2;
	return tree;
}


void readBit(FILE * fptr, unsigned char * bit, unsigned char * bit_location, unsigned char * byte_data){
	if((*bit_location) == 0){
		fread(byte_data, sizeof(unsigned char), 1, fptr);
	}
	unsigned char temp = (*byte_data)>>(7 - *bit_location);
	temp &= 0x01;
	*bit_location = ((*bit_location) + 1) % 8;
	*bit = temp;
	//fprintf(stderr,"%d",*bit);
}

tree_node * read_header(FILE * fptr){
	unsigned char bit_location = 0;
	unsigned char byte_data = 0;
	unsigned char onebit = 0;
	int done = 0;
	link_tree *head = NULL;
	while(done == 0){
		readBit(fptr, &onebit, &bit_location, &byte_data);
		if(onebit == 1){
			unsigned char value = 0;
			int char_count;
			for(char_count = 0; char_count < 7; char_count++){     //read char
				readBit(fptr, &onebit, &bit_location, &byte_data);
				value |= onebit << (6 - char_count); 
			}
			tree_node * tree = make_tree(value, 0);
			link_tree *link = malloc(sizeof(link_tree));
			link -> tree = tree;
			link -> next = head;
			head = link;
		}
		else{
			if(head == NULL){ return NULL; }   //error
			if(head -> next == NULL){ //list head->next == NULL
				done = 1;
			}
			else{
				tree_node * tree2 = head->tree;
				link_tree * temp = head;
				head = head->next;
				free(temp);
				tree_node * tree1 = head->tree;
				head -> tree = merge_tree(tree1, tree2);
				//Tree_print(head->tree,0);
			}
		}
	}
	tree_node * final_tree = head->tree;
	free(head);
	return final_tree; 
}

void Tree_print(tree_node * tn, int level)
{
  if (tn == NULL)
    {
      return;
    }
  tree_node * lc = tn -> left;  // left child
  tree_node * rc = tn -> right; // right child
  Tree_print(lc, level + 1);
  Tree_print(rc, level + 1);
  int depth;
  for (depth = 0; depth < level; depth ++)
    {
      printf("    ");
    }
  printf("freq = %llu ", tn -> freq);
  if ((lc == NULL) && (rc == NULL))
    {
      // a leaf node
      printf("value = %d, '%c'", tn -> value, tn -> value);
    }
  printf("\n");
}

void free_tree(tree_node *tree){
	if (tree == NULL){return; }
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}

int main(int argc, char ** argv){
	char *infile = argv[1];
	char outfile[100];
	strcpy(outfile,infile);
	strcat(outfile,".unhuff");
	FILE *infptr = fopen(infile,"r");
	if(infptr == NULL){return 0;}
	FILE *outfptr = fopen(outfile,"w");
	if(outfptr == NULL){return 0;}
	tree_node *tree = read_header(infptr);
	tree_node *save_tree = tree;
	Tree_print(tree,0);
	long long numChar;
	fread(&numChar,sizeof(long long),1,infptr);
	fprintf(stderr,"numChar:%lld\n",numChar);
	unsigned char aline;
	fread(&aline,sizeof(unsigned char),1,infptr);
	fprintf(stderr,"aline:%c",aline);
	long long i;
	unsigned char bit_location = 0;
	unsigned char byte_data = 0;
	unsigned char onebit = 0;
	for(i = 0; i < numChar; i++){
		while(tree->left != NULL || tree->right != NULL){
			readBit(infptr, &onebit, &bit_location, &byte_data);
			if(onebit == 0){
				tree = tree->left;
			}
			else{
				tree = tree->right;
			}
		}
		fputc(tree->value,outfptr);
		tree = save_tree;
	}
	fclose(infptr);
	fclose(outfptr);
	free_tree(tree);
	
	return 0;
}
