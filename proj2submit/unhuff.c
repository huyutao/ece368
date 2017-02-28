#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct tree_node{
	struct tree_node * left;
	struct tree_node * right;
	unsigned char value;
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


tree_node * make_tree(char value, long long freq){      // function to allocate memory for a tree and initialize it.
	tree_node * tree = malloc(sizeof(tree_node));
	tree -> value = value;
	tree -> freq = freq;
	tree -> left = NULL;
	tree -> right = NULL;
	return tree;
}

tree_node * merge_tree(tree_node *tree1, tree_node *tree2){      //merge two tree into one 
	tree_node *tree = make_tree(0,0);
	tree -> left = tree1;
	tree -> right = tree2;
	return tree;
}


void readBit(FILE * fptr, unsigned char * bit, unsigned char * bit_location, unsigned char * byte_data){
	if((*bit_location) == 0){
		fread(byte_data, sizeof(unsigned char), 1, fptr);
	}
	unsigned char temp = (*byte_data)>>(7 - *bit_location);       //shift bit out and use mask to read that bit
	temp &= 0x01;
	*bit_location = ((*bit_location) + 1) % 8;
	*bit = temp;
}

tree_node * read_header(FILE * fptr){
	unsigned char bit_location = 0;
	unsigned char byte_data = 0;
	unsigned char onebit = 0;
	int done = 0;
	link_tree *head = NULL;
	while(done == 0){
		readBit(fptr, &onebit, &bit_location, &byte_data);
		if(onebit == 1){        // one is make a tree and push it into stack
			unsigned char value = 0;
			int char_count;
			for(char_count = 0; char_count < 8; char_count++){     //read char
				readBit(fptr, &onebit, &bit_location, &byte_data);
				value |= onebit << (7 - char_count); 
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
			else{        //pop two tree out and merge then into one tree and push the tree back to the stack
				tree_node * tree2 = head->tree;
				link_tree * temp = head;
				head = head->next;
				free(temp);
				tree_node * tree1 = head->tree;
				head -> tree = merge_tree(tree1, tree2);
			}
		}
	}
	tree_node * final_tree = head->tree;      //finished, get the address of the tree
	free(head);      // free linked list memory
	return final_tree; 
}


void free_tree(tree_node *tree){      //function to free the tree
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
	tree_node *tree = read_header(infptr);      //call a function to read the header, make the tree. 
	tree_node *save_tree = tree;
	unsigned char bit_location = 0;
	unsigned char byte_data = 0;
	unsigned char onebit = 0;
	while(1){
		while(tree->left != NULL || tree->right != NULL){       //iterating go into the tree by the bit input
			readBit(infptr, &onebit, &bit_location, &byte_data);
			if(onebit == 0){
				tree = tree->left;
			}
			else{
				tree = tree->right;
			}
		}
		if(tree -> value == 128){break;}    //if read EOF, break
		fputc(tree->value,outfptr);      //save the value of the leaf to the output file
		tree = save_tree;      
	}
	fclose(infptr);   //close all file.
	fclose(outfptr);
	free_tree(tree);
	
	return 0;
}
