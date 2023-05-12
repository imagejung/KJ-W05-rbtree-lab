#include <stdio.h>
#define MAX_SIZE_STACK 100

int stack[MAX_SIZE_STACK];
int top = -1;

// stack 비어있는지 확인
int IsEmpty(){
    if(top<0)
        return 1;
    else
        return 0;
}

// stack 가득 차있는지 확인
int IsFull(){
    if(top>=MAX_SIZE_STACK)
        return 1;
    else
        return 0;
}

// top 다음 인덱스에 값 push
void push(int value){
    if(IsFull()==1)
        printf("스택이 가득 찼습니다. ");
    else
        stack[++top] = value;
}

// top 값 return하고, top 인덱스 -1
int pop(){
    if(IsEmpty()==1)
        printf("스택이 비었습니다. ");
    else
        return stack[top--];
}

int main(){
    push(3);
    push(5);
    push(12);
    printf("%d\n", pop());
    printf("%d\n", pop());
    printf("%d\n", pop());

    return 0;
}