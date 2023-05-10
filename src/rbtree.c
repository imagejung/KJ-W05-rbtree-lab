#include "rbtree.h"
#include <stdlib.h>
#include <stdio.h>

void traverse_and_delete_node(rbtree *t, node_t *node);
void rbtree_insert_fixup(rbtree *t, node_t *node);
void left_rotate(rbtree *t, node_t *node);
void right_rotate(rbtree *t, node_t *node);
node_t *get_next_node(const rbtree *t, node_t *p);
void rbtree_erase_fixup(rbtree *t, node_t *parent, int is_left);
void exchange_color(node_t *a, node_t *b);



// RB tree 구조체 생성 (rbtree.h에 rbtree정의)
rbtree *new_rbtree(void) {

  // tree 구조체 메모리 동적 할당
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  
  // nil 노드 생성 및 초기화
  node_t *nil = (node_t*)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK; // nil 노드는 Black

  // tree 구조체의 nil / root를 위에서 생성한 nil 노드로 설정 (초기화 이므로)
  t->nil = t->root = nil;

  return t;
}



// tree 순회하면서 각 노드 메모리 할당 해제
void delete_rbtree(rbtree *t) {
  
  node_t* node = t->root;
  
  // 각 노드와 그 자식 노드 메모리 할당해제 하는 함수
  if(node != t->nil)
    traverse_and_delete_node(t, node);

  // nill 노드와 rbtree 구조체 메모리 할당해제
  free(t->nil);
  free(t);
}



// 각 노드와 그 자식 노드 메모리 할당해제 하는 함수
void traverse_and_delete_node(rbtree* t, node_t* node){
  
  // nil 노드 갈 때 까지 재귀로 계속 해제(left / right)
  if(node->left != t->nil)
    traverse_and_delete_node(t, node->left);
  if(node->right != t->nil)
    traverse_and_delete_node(t, node->right);

  // 노드 메모리 해제
  free(node);
}



// 노드 삽입 및 불균형 fix
node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  // 삽입을 위한 새 노드 생성
  node_t* new_node = (node_t*)calloc(1,sizeof(node_t));
  new_node->key = key;
  new_node->color = RBTREE_RED;  // 노드 추가는 Red로
  new_node->left = new_node->right = t->nil;

  // 새 노드를 삽입 위치 탐색 (BST)
  node_t* current = t->root;
  while(current != t->nil){
    // 왼쪽 노드
    if (key<current->key){
      if(current->left == t->nil){
        current->left = new_node; // 새 노드를 왼쪽 자식을 추가
        break;
      }
      current = current->left; // while문 돌면서 계속 탐색
    }
    // 오른쪽 노드
    else{
      if(current->right == t->nil){
        current->right = new_node; // 새 노드를 오른쪽 자식으로 추가
        break;
      }
      current = current->right; // while문 돌면서 계속 탐색
    }
  } 
  // 새 노드의 부모 연결
  new_node->parent = current; 

  // 트리 비어있는 경우, 새 노드를 트리 루트로
  if(current == t->nil)
    t->root = new_node;

  // balancing
  rbtree_insert_fixup(t, new_node);

  return new_node;
}


// RB tree 특성 맞추는 함수
void rbtree_insert_fixup(rbtree* t, node_t* node){

}


// 오른쪽으로 회전하는 함수
void right_rotate(rbtree* t, node_t* node){
  node_t* parent = node->parent;
  node_t* grand_parent = parent->parent;
  node_t* node_right = node->right;

  // 부모가 루트인 경우, 현재 노드를 루트로 지정 
  if (parent == t->root)
    t->root = node;

}





// 노드 찾는 함수
node_t *rbtree_find(const rbtree *t, const key_t key) {
  
  node_t* current = t->root;
  while(current != t-> nil){
    // key 값 찾으면 반환
    if(key == current->key)
      return current;
    else
      if(key < current->key)
        current = current->left;
      else if(key > current->key)
        current = current->right;
  }
  return NULL;
}


// 최소값 노드 찾아서 반환
node_t *rbtree_min(const rbtree *t) {
  node_t* current = t->root;
  while(current->left != t->nil)
    current = current->left;
  return current;
}


// 최대값 노드 찾아서 반환
node_t *rbtree_max(const rbtree *t) {
  node_t* current = t->root;
  while(current->right != t->nil)
    current = current->right;
  return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}


// rbtree를 array로, tree를 inorder(중위순회)로 순회하여 array에 담기 
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  node_t* current = rbtree_min(t);
  arr[0] = current->key; // 우선 최솟값 찾아서 넣기

  for (int i=1; i<n; i++){
    if (current == t->nil)
      break;
    current = get_next_node(t, current);
    if (current == t->nil)
      break;
    arr[i] = current->key;
  }
  return 0;
}


// 키 값을 기준으로 다음 노드를 반환
node_t *age_next_node(const rbtree* t, node_t* p)
{
  node_t *current = p->right;
  if(current == t->nil){ // 오른쪽 자식이 없으면
    current = p;
    while(1){
      if (current->parent->right == current) // current가 오른쪽 자식인 경우
        current = current->parent; // 부모 노드로 이동 후 이어서 탐색
      else
        return current->parent; // current가 왼쪽 자식인 경우 부모 리턴
    }
  }
  while (current->left != t->nil) // 왼쪽 자식이 있으면
    current = current->left; // 왼쪽 끝으로 이동
  return current;
}


// 색 교환
void exchange_color(node_t* a, node_t* b){
  int tmp = a->color;
  a->color = b->color;
  b->color = (tmp == RBTREE_BLACK) ? RBTREE_BLACK : RBTREE_RED;
}