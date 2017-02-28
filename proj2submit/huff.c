#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct{
	char value;
	long long freq;
}char_freq;

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

int compare_freq(const void * char1, const void * char2);
void count_freq(char * filename, char_freq * freq);
tree_node * make_tree(char value, long long freq);
tree_node * merge_tree(tree_node *tree1, tree_node *tree2);
tree_node * make_code_tree(char_freq * freq);
int tree_height(tree_node *tree, int height);
int tree_leaf(tree_node *tree);
void mapping(int ** map, tree_node *tree, int *row, int col);
void writeBit(FILE * fptr, unsigned char bit, unsigned char * bit_location, unsigned char * byte_data);
void fill_zero(FILE * fptr, unsigned char * bit_location, unsigned char * byte_data);
void write_header(tree_node *tree, char* outfile);
void write_tree(tree_node *tree, FILE *outfptr, unsigned char * bit_location, unsigned char * byte_data);
void write_data(int ** map, int * map_dir, char * infile, char * outfile);
void free_tree(tree_node *tree);

int compare_freq(const void * char1, const void * char2){
	char_freq * char_comp1 = (char_freq *)char1;
	char_freq * char_comp2 = (char_freq *)char2;
	return (char_comp1 -> freq) - (char_comp2 -> freq);
}

void count_freq(char * filename, char_freq * freq){
	FILE *fptr = fopen(filename,"r");
	int c;
	int i;
	
	if (fptr == NULL){ printf("Cannot open %s\n", filename);}
	else{
		for(i = 0; i < 128; i++){
			freq[i].freq = 0;     //initialize it with 0;
		}
		while((c = fgetc(fptr)) != EOF){   //add the corresponding frequency
			freq[c].value = (char) c;
			freq[c].freq ++;
		}
	}
	qsort(freq, 128, sizeof(char_freq), compare_freq);       //sort the array in ascending order
	return;
}


tree_node * make_tree(char value, long long freq){        //make the value zero and add leaves freq
	tree_node * tree = malloc(sizeof(tree_node));
	tree -> value = value;
	tree -> freq = freq;
	tree -> left = NULL;
	tree -> right = NULL;
	return tree;
}

tree_node * merge_tree(tree_node *tree1, tree_node *tree2){
	tree_node *tree = make_tree(0,(tree1 -> freq + tree2 -> freq));
	tree -> left = tree1;
	tree -> right = tree2;
	return tree;
}


tree_node * make_code_tree(char_freq * freq){
	int i;
	link_tree *head = malloc(sizeof(link_tree));
	head -> tree = make_tree(128,1);        //  make the EOF char a tree node
	link_tree *save_head = head;
	for(i = 0; i < 128; i++){       //make the tree leafs
		if(freq[i].freq != 0){
			tree_node *tree = make_tree(freq[i].value,freq[i].freq);      // make the tree and insert into the link
			link_tree *link = malloc(sizeof(link_tree));
			link -> next = NULL;
			link -> tree = tree;
			head -> next = link;
			head = link;
		}
	}
	head = save_head;     //set the head addr back to first
	while(head -> next != NULL){       // make the tree with only one head
		tree_node * tree = merge_tree(head->tree, head->next->tree);
		save_head = head -> next;
		free(head);
		head = save_head -> next;   //make head two node after
		free(save_head);
		save_head = head;
		if (head == NULL){return tree;}  
		while(head->next != NULL && (head->next->tree->freq) < (tree->freq)){    //find the place need to insert
			head = head->next;
		}
		link_tree *link = malloc(sizeof(link_tree));      //make a new linked node then insert it into the list
		link->tree = tree;
		link->next = NULL;
		if(head->next != NULL){
			link->next = head->next;
			head->next = link;
		}else{

			head->next = link;
		}
		head = save_head;
	}
	
	tree_node *tree = head->tree;
	free(head);
	
	return tree;
}


int tree_height(tree_node *tree, int height){          // height input the depth of tree = 0;
	if (tree->left == NULL && tree->right == NULL){
		return height;
	}
	int left_height = tree_height(tree->left, height+1);
	int right_height = tree_height(tree->right, height+1);
	if(left_height > right_height){
		return left_height;
	}
	else{
		return right_height;
	}
}

int tree_leaf(tree_node *tree){        // count the number of leaves down that node
	if (tree->left == NULL && tree->right == NULL){
		return 1;
	}
	int left_leaf = 0;
	int right_leaf = 0;
	if (tree->left != NULL){
		left_leaf = tree_leaf(tree->left);
	}
	if (tree->right != NULL){
		right_leaf = tree_leaf(tree->right);
	}
	return left_leaf+right_leaf;
}


void mapping(int ** map, tree_node *tree, int *row, int col){     //row init at 0 col init at 1, because value take 0's spot
	if(tree == NULL){return; }
	if(tree -> left == NULL && tree -> right == NULL){     // write the character to the first index of that row
		map[*row][0] = tree -> value;
		*row += 1;
		return;
	}
	if(tree -> left != NULL){
		int num_row = tree_leaf(tree->left);      //count the leaf to know how many 0 need to write in 
		int i;
		for(i = *row; i < (*row)+ num_row; i++){
			map[i][col] = 0;        //write the bits
		}
		mapping(map,tree->left,row,col+1);      //going left
	}
	if(tree -> right != NULL){
		int num_row = tree_leaf(tree->right);
		int i;
		for(i = *row; i < (*row)+ num_row; i++){     //count the leaf to know how many 1 need to write in 
			map[i][col] = 1;     //write the bits
		}
		mapping(map,tree->right,row,col+1);     //going right
	}
}

void writeBit(FILE * fptr, unsigned char bit, unsigned char * bit_location, unsigned char * byte_data){
	if(*bit_location == 0){
		*byte_data = 0;   //reset after one byte have written
	}
	unsigned char temp = bit << (7 - (*bit_location));
	*byte_data |= temp;   //use logic or to store "bit" in to the "byte_data"
	if((*bit_location) == 7){
		fwrite(byte_data, sizeof(unsigned char), 1, fptr);
	}
	*bit_location = ((*bit_location) + 1) % 8;
}

void fill_zero(FILE * fptr, unsigned char * bit_location, unsigned char * byte_data){
	while(*bit_location != 0){
		writeBit(fptr, 0, bit_location, byte_data);        //fill the bit 0 to make the whole byte
	}
}

void write_header(tree_node *tree, char* outfile){
	FILE *outfptr = fopen(outfile,"w");
	if(outfptr == NULL){return;}
	unsigned char bit_location = 0;
	unsigned char byte_data = 0;
	write_tree(tree,outfptr,&bit_location, &byte_data);   //call function to write the tree in to the header.
	writeBit(outfptr, 0, &bit_location, &byte_data);  // one more zero to seperate
	fill_zero(outfptr, &bit_location, &byte_data);   // fill 0 bit to form a byte
	fclose(outfptr);
}

void write_tree(tree_node *tree, FILE *outfptr, unsigned char * bit_location, unsigned char * byte_data){
	if(tree == NULL){return; }
	if(tree->left == NULL && tree->right == NULL){
		writeBit(outfptr, 1, bit_location, byte_data);   // a leaf put 1 in file
		unsigned char value = tree -> value;
		unsigned char mask = 0x80;  // only 8 bits
		while (mask > 0){    //save the char value to file
			writeBit(outfptr, (value&mask) == mask, bit_location, byte_data);    //shift every bits of the word into the binary file
			mask >>= 1;          //move mask, till all bits have shifted in
		}
		return;
	}
	write_tree(tree->left, outfptr, bit_location, byte_data);      //iterating through the tree
	write_tree(tree->right, outfptr, bit_location, byte_data);
	writeBit(outfptr, 0, bit_location, byte_data);   // a node put 0 in file
}

void write_data(int ** map, int * map_dir, char * infile, char * outfile){
	FILE * infptr = fopen(infile,"r");
	if(infptr == NULL){return;}
	FILE * outfptr = fopen(outfile,"a");
	if(outfptr == NULL){return;}
	int c;
	unsigned char bit_location = 0;
	unsigned char byte_data = 0;
	while((c = fgetc(infptr)) != EOF){        //go through the character till the end of file
		int map_ind = map_dir[c];        //find the index of that character in the map
		int col = 1;
		while(map[map_ind][col] != -1){         //write the bit till '-1' end of the code
			writeBit(outfptr, (map[map_ind][col]==1), &bit_location, &byte_data);
			col++;
		}
	}
	int map_ind = map_dir[128];        //find the EOF's index
	int col = 1;
	while(map[map_ind][col] != -1){     //write the EOF
			writeBit(outfptr, (map[map_ind][col]==1), &bit_location, &byte_data);
			col++;
	}
	fill_zero(outfptr, &bit_location, &byte_data);
	fclose(infptr);
	fclose(outfptr);
}

void free_tree(tree_node *tree){         //iteratiing tree and free all node
	if (tree == NULL){return; }
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}

int main(int argc, char ** argv){
	char_freq freq[128];
	tree_node *tree;
	count_freq(argv[1],freq);
	tree = make_code_tree(freq);        //count the frequency of the every char, and sort them
	int num_row = tree_leaf(tree);    //use the data in the freq make a tree
	int num_col = tree_height(tree,0);     // number of tree leaves is the number of the row for the map
	num_col += 2;          // add space for character and -1;
	int ** map = malloc(sizeof(int*)*num_row);
	int row;
	int col;
	for(row = 0; row < num_row; row++){
		map[row] = malloc(sizeof(int)*num_col);
		for(col = 0; col < num_col; col++){
			map[row][col] = -1;        //initiallize the map with -1
		}
	}
	int moving_row = 0;
	mapping(map, tree, &moving_row, 1);      // call function to fill data into the map
	
	int map_dir[129];       //make a search table of content for the map
	int ind_map_dir;
	for(ind_map_dir = 0; ind_map_dir < 129; ind_map_dir++){
		map_dir[ind_map_dir] = -1;         //initiallize with -1
		for(row = 0; row < num_row; row++){
			if(map[row][0] == ind_map_dir){  
				map_dir[ind_map_dir] = row;       // save the character's row in the table of content
			}
		}
	}
	
	char *infile = argv[1];
	char outfile[100];
	strcpy(outfile,infile);
	strcat(outfile,".huff");
	
	write_header(tree, outfile);        //call function to write the head
	write_data(map, map_dir, infile, outfile);     //call function to write the compressed data in
	
	for(row = 0; row < num_row; row++){      // free all memory
		free(map[row]); 
	}
	free(map);
	free_tree(tree);
	
	return 0;
}

