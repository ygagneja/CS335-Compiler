// Code to test Binary tree traversal (Dynamic Memory)
void printf(char* str, ...);
void scanf(char* str, ...);

struct node {
   int data; 
	
   void *leftChild;
   void *rightChild;
};

struct node *root = 0;

void insert(int data) {
   struct node *tempNode = (struct node*) malloc(sizeof(struct node));
   struct node *current;
   struct node *parent;

   tempNode->data = data;
   tempNode->leftChild = 0;
   tempNode->rightChild = 0;

   //if tree is empty
   if(root == 0) {
      root = tempNode;
   } else {
      current = root;
      parent = 0;

      while(1) { 
         parent = current;
         
         //go to left of the tree
         if(data < parent->data) {
            current = current->leftChild;                
            
            //insert to the left
            if(current == 0) {
               parent->leftChild = tempNode;
               return;
            }
         }  //go to right of the tree
         else {
            current = current->rightChild;

            //insert to the right
            if(current == 0) {
               parent->rightChild = tempNode;
               return;
            }
         }
      }            
   }
}

struct node* search(int data) {
   struct node *current = root;
   prints("Visiting elements: ");

   while(current->data != data) {
      if(current != 0)
         printi(current->data); 

      //go to left tree
      if(current->data > data) {
         current = current->leftChild;
      }
      //else go to right tree
      else {                
         current = current->rightChild;
      }

      //not found
      if(current == 0) {
         return 0;
      }
   }
   
   return current;
}

void pre_order_traversal(struct node* root) {
   if(root != 0) {
      printi(root->data);
      pre_order_traversal(root->leftChild);
      pre_order_traversal(root->rightChild);
   }
}

void inorder_traversal(struct node* root) {
   if(root != 0) {
      inorder_traversal(root->leftChild);
      printi(root->data);          
      inorder_traversal(root->rightChild);
   }
}

void post_order_traversal(struct node* root) {
   if(root != 0) {
      post_order_traversal(root->leftChild);
      post_order_traversal(root->rightChild);
      printi(root->data);
   }
}

int main() {
   int i;
   int n; // number of elements
   prints("Enter number of elements, n = :");
   n = scani();
   printi("Enter n numbers : ");
   for(i = 0; i < n; i++)
   {
      int x;
      x = scani();
      insert(x);
   }
   prints("Enter element to search for in the tree: ");
   i = scani();
   struct node * temp = search(i);

   if(temp != 0) {
      prints("given element found in the tree.");
   }else {
      prints("[ x ] element not found.");
   }            

   prints("Preorder traversal: ");
   pre_order_traversal(root);

   prints("Inorder traversal: ");
   inorder_traversal(root);

   prints("Post order traversal: ");
   post_order_traversal(root);       

   return 0;
}