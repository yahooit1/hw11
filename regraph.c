/*
 * Graph Searches
 *
 * Data Structures
 *
 * Department of Computer Science
 * at Chungbuk National University
 *
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 10
#define FALSE 0
#define TRUE 1

//Graph

typedef struct GraphNode{ //GraphNode를 struct로 선언(지정)
    int vertex; //정점 정수형 변수로 선언
    struct GraphNode* link; //GraphNode에 대한 포인터 link 선언
}GraphNode; //구조체별칭

typedef struct GraphType{  //GraphType를 struct로 선언(지정)
    int n; //그래프의 정점의 개수 저장 변수
    GraphNode* adj_list[MAX_VERTICES]; //그래프 정점에 대한 헤드포인터 배열
	int visit[MAX_VERTICES]; //방문 배열
}GraphType; //구조체별칭

//Stack (for DFS)
typedef struct StackNode{ //StackNode를 struct로 선언(지정)
	int data;
	struct StackNode * link;
}StackNode;

StackNode* top; //top을 지정하기 위한 stack 구조체 포인터

//Queue (for BFS)

typedef struct QueueNode{ //QueueNode를 struct로 지정(선언)
	int data;
	struct QueueNode* link;
}QueueNode;

typedef struct QueueType{ //QueueType을 struct로 지정(선언)
	QueueNode* front; //QueueNode의 포인터 front
	QueueNode* rear; //QueueNode의 포인터 rear
}QueueType;

//함수 정의

void initializeGraph(GraphType* g); //그래프 구조체 포인터 초기화
void insertvertex(GraphType *g,int v); //정점 추가 함수
void insertedge(GraphType *g,int u,int v); //간선 추가 함수
void DepthFirstSearch(GraphType *g,int v); //깊이 우선 탐색 함수
int StackEmpty(); //스택 비었는지 확인
void push(int elem); //스택 push 함수
int pop(); //스택 pop 함수
void BreathFirstSearch(GraphType *g,int v); //넓이 우선 탐색 함수
QueueType* createQueue(); //queue 생성 초기화
int QueueEmpty(); //큐 비었는지 확인
void insertqueue(QueueType* lq, int elem); //큐 삽입
int deletequeue(QueueType* lq);//큐 삭제
void PrintGraph(GraphType *g); //그래프 출력
void freeGraph(GraphType *g); //할당된 메모리 해제

int main(){
    char command; //메뉴선택입력받을변수
	int edge; //간선갯수입력받을 변수
	int dfs;//탐색 시작 정점 for dfs
	int bfs;//탐색 시작 정점 for bfs
    GraphType *g=(GraphType*)malloc(sizeof(GraphType));
    do{
		printf("\n");
		printf("[-----[Gahyun] [2020045070] -----\n");
		printf("\n\n");
		printf("----------------------------------------------------------------\n");
		printf("                   Graph Searches                        \n");
		printf("----------------------------------------------------------------\n");
		printf(" Initialize Graph     = z        \n");
		printf(" Insert Vertex        = v      Insert Edge                 = e \n");
		printf(" Depth First Search   = d      Breath First Search         = b \n");
		printf(" Print Graph          = p      Quit                        = q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'z': case 'Z': 
			initializeGraph(g); //그래프 초기화
			break;

		case 'q': case 'Q':
			freeGraph(g); //할당된 메모리 해제
			break;

		case 'v': case 'V':
			for(int v=0;v<MAX_VERTICES;v++){ 
				insertvertex(g,v); //그래프에 정점 삽입
			}
			break;

		case 'e': case 'E':
           
            printf("edge num :");
            scanf("%d",&edge);
            if(edge>MAX_VERTICES*(MAX_VERTICES-1)/2) //입력한 간선의 개수보다 더 많은 간선을 입력하면 break
                break;
                for(int i=0;i<edge;i++){ //간선 개수만큼 반복
                    int a,b; //시작 정점과 끝 정점
			    printf("vertex u & v :");
			    scanf("%d %d",&a,&b);

				//무방향 그래프로 구현하기 위해
			    insertedge(g,a,b); 
			    insertedge(g,b,a);
                }
		
			break;

		case 'd': case 'D':
			
			printf("start vertex for dfs :");
			scanf("%d",&dfs);
			DepthFirstSearch(g,dfs); //깊이우선탐색
			break;

		case 'b': case 'B':
			
			printf("start vertex for bfs :");
			scanf("%d",&bfs);
			BreathFirstSearch(g,bfs); //넓이우선탐색
			break;

		case 'p': case 'P':
			PrintGraph(g); //출력
			break;


		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}


void initializeGraph(GraphType* g){
	
	g->n=0; //그래프 정점 0개
	for (int i=0;i<MAX_VERTICES;i++){
		g->adj_list[i]=NULL; //그래프 정점에 대한 헤드포인터 배열 초기화
		g->visit[i]=FALSE; //그래프 정점에 대한 visit 배열 FALSE로 초기화
		
	}
}

void insertvertex(GraphType *g,int v){
	if((g->n)+1>MAX_VERTICES){
		printf("Vertex max 10\n");
		return;
	}
	g->n++; //그래프 정점수 추가
}

void insertedge(GraphType *g,int u, int v){
	if(u>=g->n||v>=g->n){ //간선을 이을 수 없는 상태(시작과 끝이 현재 정점보다 크거나 같을 때)
		printf("Error\n");	
	}
	
		GraphNode * input=(GraphNode*)malloc(sizeof(GraphNode)); //삽입노드 메모리할당
		input->vertex=v; //삽입노드의 정점에 시작정점 저장
		input->link=NULL; //삽입노드의 링크 초기화

		if(g->adj_list[u]==NULL){ 
			g->adj_list[u]=input; //삽입할 노드를 저장해줌

			return;
		}

		GraphNode* curr=g->adj_list[u];
		GraphNode* prev=g->adj_list[u];

		//삽입할 위치 찾기

		while(curr!=NULL){
			if(curr->vertex>v){//첫 노드 앞에 삽입해야할 경우
				if(curr==g->adj_list[u]){
					input->link=g->adj_list[u];//삽입할노드의 링크에 첫번째노드를 저장
					g->adj_list[u]=input;//삽입 노드를 첫번째 노드로 지정
				}
				//첫 노드가 아닌 경우 (중간이나 마지막인 경우)
				else{
					input->link=curr;//삽입 노드의 링크에 현재 노드 저장
					prev->link=input;//다음 노드의 링크에 삽입 노드 저장
				}
				return;
			}
			prev=curr; //다음 노드에 현재 노드 저장
			curr=curr->link; //curr에는 curr의 link 저장
		}
		//마지막에 삽입해야하는 경우
		prev->link=input; //다음노드의 링크에 삽입 노드 저장
		return;
	
}


int StackEmpty(){ //스택비었는지확인
	if(top==NULL){
		return 1;
	}
	else return 0;
	

}

void push(int elem){ //stack push
	StackNode* temp=(StackNode*)malloc(sizeof(StackNode)); 
	//하나씩 밀기
	temp->data=elem; //temp의 data에 item 저장
	temp->link=top; //temp의 link에 top저장
	top=temp; //top에 temp 저장
}

int pop(){ //stack pop
	int elem;
	StackNode* temp=top;

	if(StackEmpty()){ //stack이 비었다면
		return 0;
	}

	else{
		elem=temp->data;
		top=temp->link; //top에 stackNode의 포인터 temp의 link 저장
		free(temp);//할당된메모리해제

		return elem;
	}
}

void DepthFirstSearch(GraphType *g,int v){ //깊이우선탐색
	GraphNode* curr; //현재노드를 위한 포인터 curr
	top=NULL; //top비우기
	push(v); //정점 push

	g->visit[v]=TRUE; //방문한 정점 TRUE값으로 바꿔주기
	printf("%d",v); //print

	while(!StackEmpty()){//stack이 존재할 동안 반복
		
		curr=g->adj_list[v]; //curr에 g의 인접리스트 저장

		while(curr){
			if(!g->visit[curr->vertex]){ //현재 노드가 FALSE이면
				push(curr->vertex); //현재 정점 push
				g->visit[curr->vertex]=TRUE; //현재 정점의 방문 플래그 TRUE
				printf("%d\t",curr->vertex);//현재 정점 출력
				v=curr->vertex; //v에 현재 정점 저장
				curr=g->adj_list[v]; //curr에 g의 인접리스트 저장
			}
			else
				curr=curr->link; //TRUE이면 curr link 저장			
		}
		v=pop(); //pop
	}
	for(int i=0;i<MAX_VERTICES;i++){
      g->visit[i]=FALSE; //모든 방문 배열 FALSE로 초기화
   }
}

QueueType* createQueue(){ //큐 생성 , 초기화
	QueueType* lq;
	lq=(QueueType*)malloc(sizeof(QueueType));
	lq->front=NULL;
	lq->rear=NULL;

	return lq;
}

int QueueEmpty(QueueType* lq){ //큐 비었는지 확인
	if(lq->front==NULL){
		return 1;
	}
	else{
		return 0;
	}

}

void insertqueue(QueueType* lq,int elem){
	QueueNode* new=(QueueNode*)malloc(sizeof(QueueNode));

	new->data=elem; //new의 data에 elem 저장
	new->link=NULL; //new의 link 초기화

	if(lq->front==NULL){ //lq의 front가 비었을 경우
		lq->front=new; //lq의 front에 new저장
		lq->rear=new; //lq의 rear에 new 저장
	}
	else{
		lq->rear->link=new; //lq의 rear의 link에 new저장
		lq->rear=new; //lq의 rear에 new 저장
	}
}

int deletequeue(QueueType* lq){
	int elem;
	QueueNode* old=lq->front;

	if(QueueEmpty(lq)){ //큐가비었을경우
		return 0;
	}
	else{
		elem=old->data; //elem에 old의 data 저장
		lq->front=lq->front->link; //lq의front에 lq의 front의 link 저장

		if(lq->front==NULL){ //lq의 front가 비었을경우
			lq->rear=NULL;//rear도 비우기
		}

		free(old); //할당된 메모리 해제

		return elem;
	}
}
void BreathFirstSearch(GraphType *g,int v){ //넓이 우선 탐색
	GraphNode* curr;
	QueueType* lq;

	lq=createQueue(); //lq에 큐 생성
	g->visit[v]=TRUE; //방문 플래그 
	printf("%d\t",v); //방문한 플래그 출력 
	insertqueue(lq,v); //큐 삽입

	while(!QueueEmpty(lq)){ //큐가 비어있지 않을 경우
		v=deletequeue(lq); //삭제

		for(curr=g->adj_list[v];curr;curr=curr->link){
			if(!g->visit[curr->vertex]){ //현재 정점의 방문 플래그가 FALSE일 경우
				g->visit[curr->vertex]=TRUE; //방문 정점의 플래그 TRUE로 바꾸기
				printf("%d\t",curr->vertex); //방문한 정점 출력
				insertqueue(lq,curr->vertex); //queue 삽입
			}
		}
	}
	for(int i=0;i<MAX_VERTICES;i++){
      g->visit[i]=FALSE; //방문한 배열 FALSE로 초기화
   }
}

void PrintGraph(GraphType *g){
	for(int i=0;i<g->n;i++){
		GraphNode* print=g->adj_list[i]; //인접리스트를 GraphNode의 포인터를 사용해 출력
		printf("-vertex %d adj_list-",i);
		while(print!=NULL){
			printf("-> %d",print->vertex);//정점 출력

			print=print->link;
		}
		printf("\n");
	}
}

void freeGraph(GraphType *g){ //할당된 메모리 해제
	for(int i=0;i<MAX_VERTICES;i++){
		GraphNode* pointer=g->adj_list[i];
		GraphNode* prev=NULL;
		while(pointer!=NULL){ //pointer가 비어있지 않은 경우 (인접리스트가 비어있지 않은 경우)
			prev=pointer;
			pointer=pointer->link;
			free(prev);
		}
	}
	free(g);
}