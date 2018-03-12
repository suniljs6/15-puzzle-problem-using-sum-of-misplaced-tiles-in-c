#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#define N 4

typedef struct Node{
	struct Node* parent;
	int matrix[N][N];
	int cost;
	int x,y,level;
	struct Node* next; 
}Node;

Node* closed;
Node* head;

int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

int cal_cost(int initial[N][N],int goal[N][N]){
	int count=0;
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
			if(initial[i][j]!=goal[i][j])
				count++;
	return count;
}

int check_issafe(int x,int y){	
	return (x>=0 && x<N && y>=0 && y<N);
}

void printMatrix(int mat[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
    printf("\n");
} 

void printPath(Node* root){
	if(root->parent!=NULL){
		printPath(root->parent);
	}
	printMatrix(root->matrix);
	
}

void mark_visited(Node* root,Node* parent){
	Node* temp = (Node*)malloc(sizeof(Node));
		temp->parent = parent;
		for(int i=0;i<N;i++){
			for(int j=0;j<N;j++){	
				temp->matrix[i][j]=root->matrix[i][j];
			}
		}
		temp->cost = root->cost;	
		temp->x = root->x;
		temp->y = root->y; 
		temp->level=root->level;
		temp->next = closed;
		closed = temp;
}
  
int check_visited_closed(Node* root){ 
 
 	Node* temp=closed;
 	while(temp!=NULL){
 		int count=0,i,j;
 		for(i=0;i<N;i++){
			for(j=0;j<N;j++){
				if (temp->matrix[i][j]!=root->matrix[i][j]){
					count++;
					break;
				}
			}
			if(count)
			{
				count=0;
				break;
			}
		}
		if(i==N && j==N)	
			return 1;
		temp=temp->next;
 	}
 	return 0; 		
}

Node* MinNode(Node* root){
   
    int min = INT_MAX;
    Node* temp;
    Node* temp1;
    temp1=root;
    if(root->next==NULL){
    	return root;
    }
    while (temp1 != NULL) {
    	if(check_visited_closed(temp1)==0){
		if (min > temp1->cost+temp1->level){
		    	min = temp1->cost+temp1->level;
		    	temp=temp1;
		     }
	    }
	    temp1 = temp1->next;
	   }
   return temp;      
}

Node *InsertNode(int mat[N][N],int x,int y,int newx,int newy,int level,int goal[N][N],Node* parent){

	Node* temp = (Node*)malloc(sizeof(Node));
		temp->parent = parent;
		for(int i=0;i<N;i++)
			for(int j=0;j<N;j++)	
				temp->matrix[i][j]=mat[i][j];
		int swap = temp->matrix[newx][newy];
		temp->matrix[newx][newy] = temp->matrix[x][y];
		temp->matrix[x][y] = swap;
		temp->cost = cal_cost(temp->matrix,goal);	
		temp->level = level;	
		temp->x = newx;
		temp->y = newy; 
		temp->next = NULL;
		return temp;
}

void Puzzle(int goal[N][N]){

	while(head!=NULL){
		Node* temp = MinNode(head);
		if(temp->cost==0){
			printf("\n");
			printPath(temp);
			printf("success\n");
			break;
		}
		int x=temp->x;
		int y=temp->y;
		if(check_visited_closed(temp)==0)
		{
			for(int i=0;i<4;i++){
				if(check_issafe(x+row[i],y+col[i])){
					Node* temp1 = InsertNode(temp->matrix,x,y,x+row[i],y+col[i],temp->level+1,goal,temp);
						if(head==NULL){
							head=temp1;
						}
						else{
							temp1->next=head;
							head=temp1;
						}
				}
			}
		}
		mark_visited(temp,temp->parent);	
	}	
}

int main(){
	int initial[N][N];
	int x,y;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			scanf("%d",&initial[i][j]);
			if(!initial[i][j]){
				x = i;
				y = j;
			}
		}
	}
	int goal[N][N] = {
		{1,2,3,4},
		{5,6,7,8},
		{9,10,11,12},
		{13,14,15,0}
	};
	head = InsertNode(initial,x,y,x,y,0,goal,NULL);
	Puzzle(goal);
}
