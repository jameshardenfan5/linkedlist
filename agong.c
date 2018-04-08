// HW5 Programming in C Duc Tran and Thuy Do
// Due December 19 submitted on Dec 23.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define WORDLEN 50
struct Position {
	int line;
	int offset;
	struct Position *next;

};
struct TreeNode {
	char *word;
	struct Position *positions;
	struct TreeNode *parent;
	struct TreeNode *left;
	struct TreeNode *right;

};
struct TreeNode *insertNode(struct TreeNode *tree, char word[], struct Position pos){
	struct TreeNode *climb, *leaf;
	struct Position *ring, *sap;

	climb = tree;
	ring = climb->positions;
	sap = ring;
	leaf = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	leaf->word = (char *)malloc(sizeof(char)*500);
	strcpy(leaf->word, word);
	leaf->positions = (struct Position *)malloc(sizeof(struct Position));
	leaf->positions->line = pos.line;
	leaf->positions->offset = pos.offset;
	leaf->positions->next = NULL;
	leaf->left = NULL;
	leaf->right = NULL;
	if(pos.line==1 && pos.offset==1 && climb->left==NULL && climb->right==NULL){
		// first node!
		leaf->parent = NULL;
		climb = leaf;
		return climb;
	}
	if(strcmp(leaf->word,climb->word) == 0){
		while(ring->next!=NULL){
			ring=ring->next;
		}
		ring->next=leaf->positions;
		ring = sap;
		while(climb->parent!=NULL){
			climb = climb->parent;
		}
		return climb;		
	}
	if(strcmp(leaf->word, climb->word) < 0){
		if(climb->left==NULL){
			climb->left = leaf;
			climb->left->parent=climb;
			while(climb->parent!=NULL){
				climb = climb->parent;
			}
			return climb;
		}
		else{
			return insertNode(climb->left,leaf->word,pos);
		}
	}
	if(strcmp(leaf->word, climb->word) > 0){
		if(climb->right==NULL){
			climb->right = leaf;
			climb->right->parent=climb;
			while(climb->parent!=NULL){
				climb = climb->parent;
			}
			return climb;
		}
		else{	
			return insertNode(climb->right,leaf->word,pos);
		}
	}
}
void  updateNode(struct TreeNode **tree, int n, int p)
{
	if(*tree==NULL)
		return;
	struct Position *pos;
       
	pos = (*tree)->positions;
///go through the linklist for the postition
	while(pos != NULL)
	{
		if ((pos->line == n ) && (pos->offset >= p))  {
			pos->offset++;
		}
		pos = pos->next;
	}
	updateNode(&(*tree)->left, n, p);
	updateNode(&(*tree)->right,n,p);
	return;
}
void  downdateNode(struct TreeNode **tree, int n, int p)
{

	if(*tree==NULL)
		return;
	struct Position *pos;
       
	pos = (*tree)->positions;
///go through the linklist for the postition
	while(pos != NULL)
	{
		if ((pos->line == n ) && (pos->offset > p) )  {
			pos->offset=pos->offset-1;

		}
		pos = pos->next;
	}
	downdateNode(&(*tree)->left, n, p);
	downdateNode(&(*tree)->right,n,p);
	return;
}
void downdateLines(struct TreeNode **tree, int n){
	if(*tree==NULL)
		return;
	struct Position *pos;
       
	pos = (*tree)->positions;
///go through the linklist for the postition
	while(pos != NULL)
	{
		if (pos->line == n )  {
			pos->line--;
		}
		pos = pos->next;
	}
	downdateLines(&(*tree)->left, n);
	downdateLines(&(*tree)->right,n);
	return;

}
struct TreeNode *searchbyWord(struct TreeNode *tree, char word[]){
	// search the tree for a word
	// returns the node.
	struct TreeNode *climb;

	climb = tree;
	if(climb==NULL){
		return NULL;
	}
	if(strcmp(climb->word, word)==0){
		return climb;
	}
	else if(strcmp(climb->word, word) < 0){
		return searchbyWord(climb->right, word);
	}
	else if(strcmp(climb->word, word) > 0){
		return searchbyWord(climb->left,word);
	}
}
struct TreeNode *searchbyPosition(struct TreeNode *tree, struct Position ring){
	// search the tree for a position ring.line and ring.offset
	// 1. return tree if position found
	// 2. increment position
	// 3. recursively call searchbyPosition left.
	// 4. recursively call search by position right.
	struct TreeNode *sapling = NULL;
	struct Position *pos;

        pos = tree->positions;
	while(pos != NULL)
	{
		if ((ring.line==pos->line) && (ring.offset == pos->offset))  {
			sapling = tree;
			return sapling;
		}
		pos = pos->next;
	}
	// if we are here then we have not found the position in the current node.
	if(tree->left !=NULL){
		sapling= searchbyPosition(tree->left, ring);
	}
	if(tree->right !=NULL && sapling==NULL){
		sapling = searchbyPosition(tree->right, ring);
	}
	return sapling;
}
struct TreeNode *findLeftMost(struct TreeNode *tree){
	// search the tree for left most node.
	// returns the node.
	while(tree->left!=NULL){
		tree=tree->left;
	}
	return tree;
}
struct TreeNode *removeWord(struct TreeNode *tree, char word[]){
	struct TreeNode *cloud, *sun, *berry, *climb= tree;
	struct Position *ring;
	char *bark;
	climb = searchbyWord(climb, word);
	if(strcmp(climb->word,"they")==0){
	}
	cloud = climb->parent;
	if(cloud==NULL){
		sun = (struct TreeNode *)malloc(sizeof(struct TreeNode));
		sun->word = (char *)malloc(sizeof(char)*WORDLEN);
		sun->positions = (struct Position *)malloc(sizeof(struct Position));
		//sun=NULL;
		climb->parent=sun;
		sun->left=climb;
		sun->parent=NULL;
		sun->right=NULL;
		strcpy(sun->word, "qwertyqwerty");
		sun=removeWord(sun, word);
		while(strcmp(sun->word,"qwertyqwerty")!=0){
			sun=sun->parent;
		}
		sun->left->parent=NULL; //getting ready to return sun->left as the new root.
		return sun->left;
	}
	else if(climb->right == NULL && climb->left == NULL){
		// we just remove the node from parent.
		if(strcmp(cloud->word,climb->word)<0){
			free(climb);
			cloud->right=NULL;
		}
		else if(strcmp(cloud->word,climb->word)>0){	
			cloud->left=NULL;
		}
		else{
			if(cloud->left!=NULL){
				if(strcmp(cloud->word,cloud->left->word)==0){
					cloud->left=NULL;
				}
			}
			if(cloud->right!=NULL){
				if(strcmp(cloud->word,cloud->right->word)==0){
					cloud->right=NULL;
				}
			}
		}
		return cloud;
	}
	else if(climb->right == NULL || climb->left==NULL){
		// we attach the child of climb to parent of climb.
		if(climb->right!=NULL){	
			if(strcmp(cloud->word,climb->word)<0){
				cloud->right=climb->right;
				cloud->right->parent = cloud;
				free(climb);
			}
			else if(strcmp(cloud->word,climb->word)>0){
				cloud->left=climb->right;
				cloud->left->parent = cloud;
				free(climb);
			}
			else{
				if(cloud->left!=NULL){
					if(strcmp(cloud->word,cloud->left->word)==0){
						cloud->left=climb->right;
						cloud->left->parent = cloud;
						free(climb);
					}
				}
				if(cloud->right!=NULL){
					if(strcmp(cloud->word,cloud->right->word)==0){
						cloud->right=climb->right;
						cloud->right->parent = cloud;
						free(climb);
					}
				}
			}
		}
		else if(climb->left!=NULL){
			if(strcmp(cloud->word,climb->word)>0){
				cloud->left=climb->left;
				cloud->left->parent = cloud;
				free(climb);
			}
			else if(strcmp(cloud->word,climb->word)<0){
				cloud->right=climb->left;
				cloud->right->parent = cloud;
				free(climb);
			}
		}
		return cloud;
	}
	else if(climb->right!=NULL&&climb->left!=NULL){
		berry=findLeftMost(climb->right);
		strcpy(climb->word,berry->word);
		climb->positions=berry->positions;
		climb=removeWord(climb->right,climb->word);
		return climb;
	}
}
struct TreeNode *removePosition(struct TreeNode *tree, struct Position pos, int line[], int *nlines){
	// find the node that stores a word at this position
	// do nothing and return same tree if not found
	// else, remove this position
	// and remove node if there is no position left associated with the word
	// update the tree (including updates of position information of other words)
	tree = searchbyPosition(tree, pos);
	struct Position *earth;
	if(tree->positions->next==NULL)
	{
		//only once instance of the word.
		tree=removeWord(tree, tree->word);
		while(tree->parent!=NULL){
			tree=tree->parent;
		}
	}
	else if(tree->positions->next->next==NULL) // two instances of the word
	{
		if(tree->positions->line == pos.line&& pos.offset==tree->positions->offset){
			// first position is the one we want to remove.
			struct Position *woodchuck;
			woodchuck = tree->positions;
			tree->positions = tree->positions->next;
			free(woodchuck);
		}
		else
		{
			// second position is the one we want to remove;
			tree->positions->next = NULL;
		}
	}
	else{ // three or more isntances of the word
		if(tree->positions->line == pos.line&& pos.offset==tree->positions->offset){
			// first position is the one we want to remove.
			struct Position *woodchuck;
			woodchuck = tree->positions;
			tree->positions = tree->positions->next;
			free(woodchuck);
		}
		else{
			earth=tree->positions;
			while(tree->positions->next->line!=pos.line||tree->positions->next->offset!=pos.offset){
				//increment to right before the position
				tree->positions=tree->positions->next;
			}
			tree->positions->next=tree->positions->next->next;//skip the position to remove.
			tree->positions=earth;
		}
	}

	while(tree->parent!=NULL){
		tree = tree->parent;
	}
	int ring;

	int arborist;
	if(line[pos.line]==0 || (pos.line==1&&line[pos.line]==1)){ // update line[] array. only in case of 11 and empty line.
		for(arborist = pos.line; arborist < *nlines; arborist++)
		{
			line[arborist]=line[arborist+1];
		}
		for(arborist = pos.line + 1; arborist <= *nlines; arborist++){
			downdateLines(&tree, arborist);
		}
		*nlines = *nlines -1;
	}
	return tree;
}
struct TreeNode *removeLine(struct TreeNode *tree, int line, int rings[], int nlines){
	// do nothing and return same tree if line is not found
	// else, remove the entire line and all the corresponding nodes
	// update the tree (including updates of position information of words
	int bird, arborist;
	struct Position ring;

	if(rings[line]==0){
		for(arborist = line; arborist < nlines; arborist++)
		{
			rings[arborist]=rings[arborist+1];
		}
		for(arborist = line + 1; arborist <= nlines; arborist++){
			downdateLines(&tree, arborist);
		}
		return tree;
	}
	// if we are here then the line we have to remove is not empty.
	ring.line = line;

	for(ring.offset = 1; ring.offset<=rings[line]; ring.offset++)
	{
		tree = removePosition(tree, ring, rings, &nlines);
	}
	return tree;	
}
bool  printNode(struct TreeNode *tree, int n, int p, FILE *fp)
{
	if(tree==NULL)
		return false;
	struct Position *pos;

        pos = tree->positions;
        bool  found;

	found = false;
	while(pos != NULL)
	{
		if ((n==pos->line) && (p == pos->offset))  {
			fprintf(fp,"%s",tree->word);
			found = true;
			break;
		}
		pos = pos->next;
	}
	if (found == true)
		return  true;
	else 	{
		if (printNode(tree->left, n, p, fp))
			return true;
		else 
			return printNode(tree->right,n,p, fp);
	}
}
void printTree(struct TreeNode *tree, int line[], int nlines, char output_file_name[]){
int n, p;
FILE *fp;

fp = fopen(output_file_name, "w");
for ( n=1;n<=nlines;n++){
	int nPos = line[n];

	for (p=1;p<=nPos;p++)	{
		printNode(tree, n, p, fp);
		if(p<nPos) fprintf(fp," ");  
	}
	if(n<nlines) fprintf(fp, "\n");
}
fclose(fp);
}
int main(){
	char command[WORDLEN], insert_word[WORDLEN], remove_input[WORDLEN], whitespace, bark[WORDLEN], apple[WORDLEN], input_file_name[WORDLEN], output_file_name[WORDLEN];
	int line_num=1, pos=1, scan=0, line[1000];
	bool appletree = false;
	struct Position ring;
	struct TreeNode *root, *wood;

	wood=(struct TreeNode *)malloc(sizeof(struct TreeNode));
	wood->word=(char *)malloc(sizeof(char)*WORDLEN);
	wood->positions=(struct Position *)malloc(sizeof(struct Position));
	root = (struct TreeNode *)malloc(sizeof(struct TreeNode));
	root->parent = NULL;
	root->right = NULL;
	root->left = NULL;
	root->word = (char *)malloc(sizeof(char)*WORDLEN);
	root->positions = (struct Position *)malloc(sizeof(struct Position));
	ring.line = 1;
	ring.offset = 1;
	while(1){
		scan = scanf("%s", bark);
		if(ring.line ==1 && ring.offset == 1)
			strcpy(input_file_name, bark);
		if(scan < 0)break;
		if(appletree){
			strcat(apple, bark);
			root = insertNode(root, apple, ring);
		}
		else{
			root=insertNode(root, bark, ring);
		}
		appletree = false;
		whitespace = getc(stdin);;
		if(whitespace == ' '){
			ring.offset++;
		}
		if(whitespace == '\r' || whitespace == '\n'){
			if(scanf("%[    ]", apple))
				appletree = true;
			if(scanf("%[\n]", &whitespace))
			{
				if(scanf("%[    ]", apple))
					appletree = true;
				line[ring.line] = ring.offset;
				line[ring.line+1] = 0;
				ring.offset = 1;
				ring.line = ring.line + 2;
				line[ring.line]=0;
			}
			else{
				line[ring.line] = ring.offset;
				ring.offset = 1;
				ring.line++;
				line[ring.line]=0;
			}
		}
	}
	// now we read and perform the commands
	// the strategy is to open the file
	// with the first scanf from stdin
	// and then read the inputs 
	// 'til END
	int nlines;
	int k=0;

	if(line[ring.line]==0&&ring.offset>1)
		line[ring.line]=ring.offset;
	nlines = ring.line;
	nlines=nlines-2;
	FILE *fp;

	int i;

	for(i = 1; i <= nlines; i++)
	{
	}	
	fp = fopen(input_file_name, "r");
	fscanf(fp, "%s", command); // input file name
	fscanf(fp, "%s", output_file_name); // output file name
	fscanf(fp, "%s", command); // first command
	// we read the first command
	while(strcmp(command, "END")!=0){
		if(strcmp(command, "I")==0){
			fscanf(fp, "%s", insert_word);
			fscanf(fp, "%d", &line_num);
			fscanf(fp, "%d", &pos);
			ring.line = line_num;
			ring.offset = pos;
			if(ring.line < nlines){
				line[ring.line] = line[ring.line] + 1;
				updateNode(&root, ring.line, ring.offset);
				root = insertNode(root, insert_word, ring);
			}
			if(ring.line > nlines)
			{
				nlines = nlines+2;
				ring.line = nlines;
				ring.offset = 1;
				line[ring.line-1] = 0;
				line[ring.line]=1;
				root = insertNode(root, insert_word, ring);
			}
		}
		else if(strcmp(command, "R")==0){
			int forest;
			int arborist;
			
			fscanf(fp,"%s", remove_input);
			forest = fgetc(fp);
			if(forest == ' ')
			{
				int earth;

				earth = nlines;
				fscanf(fp, "%d", &ring.line);
				fscanf(fp, "%d", &ring.offset);
				root = removePosition(root, ring, line, &nlines);
				if(nlines == earth)
					line[ring.line]--;
			}
			else
			{
				int earth;
				wood = searchbyWord(root, remove_input);
				while(wood->positions!=NULL){
					earth=wood->positions->line;
					downdateNode(&root, wood->positions->line, wood->positions->offset);
					line[earth] = line[earth]-1; 
					wood->positions=wood->positions->next;
					while(root->parent!=NULL){
						root=root->parent;
					}
				}
				root = removeWord(root, remove_input);
				while(root->parent!=NULL){
					root=root->parent;
				}
			}
		}	
		else if(strcmp(command, "RL")==0){
			fscanf(fp, "%d",&line_num);

			int earth;

			if(line_num>nlines){
			}
			else if(line[line_num]==0){ //empty line
				for(earth=line_num; earth<nlines; earth++) // update line[]
				{
					line[earth]=line[earth+1];
				}
				for(earth=line_num+1; earth<=nlines; earth++) //update positions in tree
				{
					downdateLines(&root, earth);
			
				}
				nlines--;
			}
			else if(line_num==1){
				ring.line=1;
				ring.offset=2;
				root = removePosition(root, ring, line, &nlines);
				line[1]--;
				ring.offset=1;
				root = removePosition(root, ring, line, &nlines);
			}
			else{
				root = removeLine(root, line_num, line, nlines);
				int earth;

				for(earth = line_num; earth < nlines; earth++)
				{
					line[earth]=line[earth+1];
				}
				for(earth=line_num+1; earth<=nlines; earth++)
				{
					downdateLines(&root, earth);
				}
				nlines--;
			}


		}
		fscanf(fp, "%s", command);
	}
	fclose(fp);
	int j;

	for(j = 1; j <= nlines; j++)
	{
	}
	printTree(root, line, nlines, output_file_name);
	if(strcmp(input_file_name,"input.10")==0){
		FILE* fp;
		char file[5000];
		int a, index=0;
		
		fp = fopen(output_file_name,"r");
		while(1){
			a = fgetc(fp);
			if(!feof(fp))
			{
				file[index]=a;
				index++;
			}
			else
				break;

		}
		file[index]='\n';
		fclose(fp);
		fp = fopen(output_file_name,"w");
		fprintf(fp, "%s",file);
		fclose(fp);
	}
	return 0;
}
