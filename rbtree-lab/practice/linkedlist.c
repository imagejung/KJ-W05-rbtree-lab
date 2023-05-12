#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node* next;
}Node;

int main(void){
    // 헤드 노드
    Node* head = (Node*)malloc(sizeof(Node)); 
    head->next = NULL;

    // 노드1
    Node* node1 = (Node*)malloc(sizeof(Node));
    node1->next = head->next; // 머리노드가 가리키던 NULL을 node1이 가리킴
    node1->data = 10;
    head->next = node1; // 머리노드가 node1을 가리킴

    // 노드2
    Node* node2 = (Node*)malloc(sizeof(Node));
    node2->next = node1->next; // node1이 가리키던 NULL을 node2가 가리킴
    node2->data = 20;
    node1->next = node2; // node1이 node2를 가리킴
    
    // 순회용 노드 생성
    Node* curr = head->next;
    while(curr != NULL){
        printf("%d\n", curr->data);
        curr = curr->next;
    }

    free(head);
    free(node1);
    free(node2);
    return 0;
}