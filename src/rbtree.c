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

  // 새 노드를 삽입 위치 탐색 (BST 방식)
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
  node_t* parent = node->parent;
  node_t* grand_parent = parent->parent;
  node_t* uncle;
  int is_left = (node == parent->left); // 현재 노드가 왼쪽 자식인지 여부
  int is_parent_is_left;

  // 추가된 노드가 root 인 경우 Black으로 
  if (node == t->root){
    node->color = RBTREE_BLACK;
    return;
  }

  // 부모가 Black인 경우 변경x
  if (parent->color == RBTREE_BLACK)
    return;

  is_parent_is_left = (grand_parent->left == parent);
  uncle = (is_parent_is_left) ? grand_parent->right : grand_parent->left;

  // case1, 부모와 부모의 형제가 모두 Red
  if (uncle->color == RBTREE_RED){
    parent->color = RBTREE_BLACK;
    uncle->color = RBTREE_BLACK;
    grand_parent->color = RBTREE_RED;
    rbtree_insert_fixup(t, grand_parent);
    return;
  }

  if (is_parent_is_left){
    if (is_left){
      // case2, 부모의 형제가 Black & 부모가 왼쪽 자식 & 현재 노드가 왼쪽 자식인 경우
      right_rotate(t, parent);
      exchange_color(parent, parent->right);
      return;
    }

    // case3, 부모의 형제가 Black & 부모가 왼쪽 자식 & 현재 노드가 오른쪽 자식인 경우
    left_rotate(t, node);
    right_rotate(t, node);
    exchange_color(node, node->right);
    return;
  }

  if (is_left){
    // case3, 부모의 형제가 Black & 부모가 왼쪽 자식 & 현재 노드가 오른쪽 자식인 경우
    right_rotate(t, node);
    left_rotate(t, node);
    exchange_color(node, node->left);
    return;
  }

  // case2, 부모의 형제가 Black & 부모가 오른쪽 자식 & 현재 노드가 오른쪽 자식인 경우
  left_rotate(t, parent);
  exchange_color(parent, parent->left);
}



// 오른쪽으로 회전하는 함수
void right_rotate(rbtree* t, node_t* node){
  node_t* parent = node->parent;
  node_t* grand_parent = parent->parent;
  node_t* node_right = node->right;

  // 부모가 루트인 경우, 현재 노드를 루트로 지정 
  if (parent == t->root)
    t->root = node;
  else {
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else 
      grand_parent->right = node;
  }
  node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
  parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
  node->right = parent;        // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
  node_right->parent = parent; // 3-1) 노드의 자식의 부모를 parent로 변경
  parent->left = node_right;   // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
}



// 왼쪽으로 회전하는 함수
void left_rotate(rbtree *t, node_t *node)
{
  node_t *parent = node->parent;
  node_t *grand_parent = parent->parent;
  node_t *node_left = node->left;

  // 부모가 루트인 경우: 현재 노드를 루트로 지정 (노드를 삭제한 경우만 해당)
  if (parent == t->root)
    t->root = node;
  else
  { // 1-1) 노드의 부모를 grand_parent로 변경
    if (grand_parent->left == parent)
      grand_parent->left = node;
    else
      grand_parent->right = node;
  }
  node->parent = grand_parent; // 1-2) 노드를 grand_parent의 자식으로 변경 (양방향 연결)
  parent->parent = node;       // 2-1) parent의 부모를 노드로 변경
  node->left = parent;         // 2-2) parent를 노드의 자식으로 변경 (양방향 연결)
  parent->right = node_left;   // 3-1) 노드의 자식의 부모를 parent로 변경
  node_left->parent = parent;  // 3-2) 노드의 자식을 부모의 자식으로 변경 (양방향 연결)
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


// 노드를 삭제 함수
int rbtree_erase(rbtree *t, node_t *delete)
{
  node_t *remove; // 트리에서 없어질 노드
  node_t *remove_parent, *replace_node;
  int is_remove_black, is_remove_left;

  // Step 1) delete 삭제 후 대체할 replace_node 찾기
  // Step 1-1) delete 노드의 자식이 둘인 경우: delete의 키를 후계자 노드의 키값으로 대체, 노드의 색은 delete의 색 유지
  if (delete->left != t->nil && delete->right != t->nil)
  {
    remove = get_next_node(t, delete); // 후계자 노드 (오른쪽 서브트리에서 가장 작은 노드)
    replace_node = remove->right;      // 대체할 노드: 지워질 노드인 후계자는 항상 왼쪽 자식이 없기 때문에, 자식이 있다면 오른쪽 자식 하나뿐임
    delete->key = remove->key;         // delete의 키를 후계자 노드의 키값으로 대체 (색은 변경 X)
  }
  else
  { // Step 1-2) delete 노드의 자식이 없거나 하나인 경우: delete 노드를 자식으로 대체, 노드의 색도 대체되는 노드의 색으로 변경
    remove = delete;
    // 대체할 노드: 자식이 있으면 자식노드로, 없으면 nil 노드로 대체
    replace_node = (remove->right != t->nil) ? remove->right : remove->left;
  }
  remove_parent = remove->parent;

  // Step 2) remove 노드 제거하기
  /* [CASE D1]: remove 노드가 루트인 경우 */
  if (remove == t->root)
  {
    t->root = replace_node;        // 대체할 노드를 트리의 루트로 지정
    t->root->color = RBTREE_BLACK; // 루트 노드는 항상 BLACK
    free(remove);
    return 0; // 불균형 복구 함수 호출 불필요 (제거 전 트리에 노드가 하나 혹은 두개이므로 불균형이 발생하지 않음)
  }

  // Step 2-1) 'remove의 부모'와 'remove의 자식' 이어주기
  is_remove_black = remove->color; // remove 노드 제거 전에 지워진 노드의 색 저장
  is_remove_left = remove_parent->left == remove;

  // Step 2-1-1) 자식 연결
  if (is_remove_left) // remove가 왼쪽 자식이었을 경우: remove 부모의 왼쪽에 이어주기
    remove_parent->left = replace_node;
  else // remove가 오른쪽 자식이었을 경우: remove 부모의 오른쪽에 이어주기
    remove_parent->right = replace_node;

  // Step 2-1-2) 부모도 연결 (양방향 연결)
  replace_node->parent = remove_parent;
  free(remove);

  /* [CASE D2~D6]: remove 노드가 검정 노드인 경우 */
  // Step 3) 불균형 복구 함수 호출
  if (is_remove_black)
    rbtree_erase_fixup(t, remove_parent, is_remove_left);
  return 0;
}

// 노드 삭제 후 불균형을 복구하는 함수
// `parent`: extra_black이 부여된 노드의 부모
// `is_left`: extra_black이 부여된 노드가 왼쪽 자식인지 여부
void rbtree_erase_fixup(rbtree *t, node_t *parent, int is_left)
{
  // 삭제 후 대체한 노드가 RED (Red & Black): BLACK으로 변경
  node_t *extra_black = is_left ? parent->left : parent->right;
  if (extra_black->color == RBTREE_RED)
  {
    extra_black->color = RBTREE_BLACK;
    return;
  }

  node_t *sibling = is_left ? parent->right : parent->left;
  node_t *sibling_left = sibling->left;
  node_t *sibling_right = sibling->right;

  if (sibling->color == RBTREE_RED)
  { // [CASE D3] 형제가 RED
    if (is_left)
      left_rotate(t, sibling);
    else
      right_rotate(t, sibling);
    exchange_color(sibling, parent);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  node_t *near = is_left ? sibling_left : sibling_right;    // 형제의 자식 중 extra_black으로부터 가까운 노드
  node_t *distant = is_left ? sibling_right : sibling_left; // 형제의 자식 중 extra_black으로부터 먼 노드

  if (is_left && near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { // [CASE D4] 형제가 BLACK, 형제의 가까운 자식이 RED, 형제의 더 먼 자식이 BLACK
    right_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  if (is_left && distant->color == RBTREE_RED)
  { // [CASE D5] 형제가 BLACK, 형제의 더 먼 자식이 RED
    left_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  if (near->color == RBTREE_RED && distant->color == RBTREE_BLACK)
  { // [CASE D4] 형제가 BLACK, 형제의 가까운 자식이 RED, 형제의 더 먼 자식이 BLACK
    left_rotate(t, near);
    exchange_color(sibling, near);
    rbtree_erase_fixup(t, parent, is_left);
    return;
  }

  if (distant->color == RBTREE_RED)
  { // [CASE D5] 형제가 BLACK, 형제의 더 먼 자식이 RED
    right_rotate(t, sibling);
    exchange_color(sibling, parent);
    distant->color = RBTREE_BLACK;
    return;
  }

  // [CASE D2] 형제가 BLACK, 형제의 자식이 둘 다 BLACK
  sibling->color = RBTREE_RED;

  if (parent != t->root)
    rbtree_erase_fixup(t, parent->parent, parent->parent->left == parent);
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
node_t *get_next_node(const rbtree* t, node_t* p)
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