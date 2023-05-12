#include <stdio.h>
#include <stdlib.h>
 
 
typedef struct _TreeNode{
    int key;    // key 값
    TreeNode *left;    // left child
    TreeNode *right;     // right child
}TreeNode;
 
TreeNode* search(TreeNode* root, int key){
    if(root == NULL){    // 값을 찾지 못한 경우  
        printf("Error : 값을 찾을 수 없습니다\n");
        return root;
    }
    
    if(key == root->key){    // 값을 찾음 
        return root;
    }
    else if(key < root->key){    // 왼쪽 서브트리 탐색 
        search(root->left, key);
    }
    else if(key > root->key){    // 오른쪽 서브트리 탐색 
        search(root->right, key);
    }
    
}

 
TreeNode* insert(TreeNode* root, int key){
    TreeNode* ptr;     // 탐색을 진행할 포인터 
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));    // newNode 생성
    newNode->key = key;
    newNode->left = newNode->right = NULL; 
    
    if(root == NULL){    // 트리가 비어 있을 경우 
        root = newNode;
        return root;
    }
    
    ptr = root;    // root 노드부터 탐색 진행  
    
    while(ptr){
        if(key == ptr->key){    // 중복값 
            printf("Error : 중복값은 허용되지 않습니다!\n");
            return root;
        }else if(key < ptr->key){    // 왼쪽 서브트리 
            if(ptr->left == NULL){    // 비어있다면 추가 
                ptr->left = newNode;
                return root;
            }else{    // 비어있지 않다면 다시 탐색 진행 
                ptr= ptr->left;
            }
        }else{    // key > ptr->key 오른쪽 서브트리 
            if(ptr->right == NULL){    // 비어있다면 추가 
                ptr->right = newNode;
                return root;
            }else{    // 비어있지 않다면 다시 탐색 진행 
                ptr = ptr->right;
            }
        }
    }
    
}
 
TreeNode* delete_node(TreeNode* root, int key){
    TreeNode* del = NULL;    // 삭제할 노드     
    TreeNode* parent = NULL;    // 삭제할 노드의 부모 노드 
    TreeNode* successor = NULL;    // 삭제할 노드의 왼쪽 서브트리에서 가장 큰 노드 
    TreeNode* predecessor = NULL;    // successor의 부모노드 
    TreeNode* child = NULL;        // 삭제할 노드의 자식 노드 
    
    del= root;
    while(del != NULL){    // 삭제할 노드 탐색 
        if(key == del->key){
            break;
        }
        parent = del;
        if(key < del->key){
            del = del->left;
        }else{
            del = del->right;
        }
    }
    
    if(del == NULL){
        printf("Error : 존재하지 않는 키\n");
        return root;
    }
    
    if(del->left == NULL && del->right == NULL){    // 삭제할 노드의 자식노드가 없는 경우 
        if(parent != NULL){    // 부모노드가 있는 경우 
            if(parent->left == del){    // 부모노드의 왼쪽노드가 삭제할 노드일 때 
                parent->left = NULL;
            }else{    // 오른쪽 일 때 
                parent->right = NULL;
            }
        }else{    // 부모노드가 없는 경우 = root 노드 
            root = NULL;
        } 
    }else if(del->left != NULL && del->right != NULL){    // 삭제할 노드의 자식 노드가 2개인 경우 
        predecessor = del;
        successor = del->left;
        
        while(successor->right != NULL){    // 왼쪽 서브트리에서 가장 큰 값 찾기 
            predecessor = successor;
            successor = successor->right;
        }
        
        if(predecessor->left == successor){
            predecessor->left = successor->left;
        }else{
            predecessor->right = successor->left;
        }
         
        
        del->key = successor->key;
        del = successor;
 
    }else{    //     삭제할 노드의 자식 노드가 1개인 경우 
        if(del->left != NULL){    // 왼쪽 노드 
            child = del->left;
        }else{    // 오른쪽 노드 
            child = del->right;
        }
        
        if(parent != NULL){    // 부모노드가 있는 경우 
            if(parent->left == del){    // 부모노드의 왼쪽 노드로 삭제할 노드의 자식노드 연결 
                parent->left = child;
            }else{    // 부모노드의 오른쪽 노드로 삭제할 노드의 자식노드 연결  
                parent->right = child;
            }
        }else{
            root = child;
        }
    }
    
    free(del);    // 메모리해제 
    return root; 
}
 
 
void print_tree(TreeNode* root){
    if(root == NULL){
        return;
    }
    printf("%d\n", root->key);
    print_tree(root->left);
    print_tree(root->right);
}
 
int main(){
    TreeNode* root = NULL;
    TreeNode* ptr = NULL;
    root = insert(root, 7);
    root = insert(root, 3);
    root = insert(root, 8);
    root = insert(root, 1);
    root = insert(root, 5);
    root = insert(root, 4);
    root = insert(root, 10);
    
    print_tree(root);
    printf("\n");
    
    ptr = search(root, 7);
    printf("%d\n", ptr->key);
    
    root = delete_node(root,7);
    ptr = search(root, 7);
 
   return 0;
} 