#include <stdio.h>
#include <stdlib.h>

typedef struct _TreeNode{
    int key;
    TreeNode* left;
    TreeNode* right;
}TreeNode;

TreeNode* search(TreeNode* root, int key){
    if(root = NULL){
        printf("값을 찾을 수 없습니다.\n");
        return NULL;
    }

    // 값이 노드와 같으면 (찾은 경우)
    if(key == root->key){
        return root;
    }
    // 값이 노드보다 작으면 왼쪽 서브트리 탐색
    else if(key < root->key){
        search(root->left, key);
    }
    // 값이 노드보다 크면 오른쪽 서브트리 탐색
    else if(key > root->key){
        search(root->right, key);
    }
}

void insert(TreeNode** root, int key){
    // 초기화
    TreeNode* ptr; // 탐색을 진행할 포인터
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->key = key;
    newNode->left = newNode->right = NULL;


}

