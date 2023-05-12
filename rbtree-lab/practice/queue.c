#include <stdio.h>
#include <stdlib.h>

// 노드 정의
typedef struct Node{
    int data;
    struct Node* next;
}Node;

// 큐 정의
typedef struct Queue{
    Node* front;
    Node* rear;
    int count; // 큐 안의 노드 개수
}Queue;

// 큐 초기화
void initQueue(Queue* queue){
    queue->front = NULL;
    queue->rear = NULL; 
    queue->count = 0;
}

// 큐가 비어 있는지 확인
int isEmpty(Queue* queue){
    return queue->count == 0; // empty면 true 1 반환
}

// 큐 데이터 삽입
void enqueue(Queue* queue, int data){
    Node* newNode = (Node*)malloc(sizeof(Node)); // malloc으로 새로운 node 메모리공간 할당
    newNode->data = data;
    newNode->next = NULL;

    if (isEmpty(queue)) // 큐가 비어있으면
        queue->front = newNode; // front와 next가 모두 newNode를 가리킴

    else // 비어있지 않으면
        queue->rear->next = newNode; // queue의 맨 뒤에 있던 Node의 next가 새로 매개변수로 전달받은 Node를 가리키도록

    queue->rear = newNode; // queue의 맨 뒤를 매개변수로 입력받은 Node로 바꿔줌
    queue->count++; // 큐 내부의 카운트 1 증가
}

// 큐 데이터 반환
int dequeue(Queue* queue){
    int data;
    Node* ptr;

    // queue가 비어있으면 오류메세지 띄우고, 함수 나가기
    if (isEmpty(queue)){ 
        printf("Queue가 비어있습니다.\n"); 
        return 0;
    }

    ptr = queue->front; // 맨앞의 노드 임시로 ptr로
    data = ptr->data; // 맨앞 데이터 return 
    queue->front = ptr->next; // 맨앞 다음 노드를(next) front로 설정
    free(ptr); // 맨앞 메모리 할당 해제
    queue->count--; // queue 개수 1 빼기

    return data;
}

int main(void){
    Queue queue;
    initQueue(&queue);

    for (int i=1; i<=10; i++)
        enqueue(&queue, i);

    while (!isEmpty(&queue))
        printf("%d\n", dequeue(&queue));

    return 0;
}