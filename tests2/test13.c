// Code to test Binary tree traversal (Dynamic Memory)

struct node {
   int data; 
	
   void *leftChild;
   void *rightChild;
};

struct node *root = NULL;

void insert(int data) {
   struct node *tempNode = malloc(sizeof(struct node));
   struct node *current;
   struct node *parent;

   tempNode->data = data;
   tempNode->leftChild = 0;
   tempNode->rightChild = 0;

   //if tree is empty
   if(root == NULL) {
      root = tempNode;
   }else{
      current = root;
      parent = NULL;

      while(1){ 
         parent = current;
         
         //go to left of the tree
         if(data < parent->data) {
            current = current->leftChild;                
            
            //insert to the left
            if(current == NULL) {
               parent->leftChild = tempNode;
               return;
            }
         }  //go to right of the tree
         else {
            current = current->rightChild;

            //insert to the right
            if(current == NULL) {
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
      if(current != NULL)
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
      if(current == NULL) {
         return 0;
      }
   }
   
   return current;
}

void pre_order_traversal(struct node* root) {
   if(root != NULL) {
      printi(root->data);
      pre_order_traversal(root->leftChild);
      pre_order_traversal(root->rightChild);
   }
}

void inorder_traversal(struct node* root) {
   if(root != NULL) {
      inorder_traversal(root->leftChild);
      printi(root->data);          
      inorder_traversal(root->rightChild);
   }
}

void post_order_traversal(struct node* root) {
   if(root != NULL) {
      post_order_traversal(root->leftChild);
      post_order_traversal(root->rightChild);
      printi(root->data);
   }
}

int main() {
   int i;
   int n; // number of elements
   struct node* temp;
   prints("Enter number of elements, n = ");
   n = scani();
   prints("Enter n numbers : \n");
   for(i = 0; i < n; i++)
   {
      int x;
      x = scani();
      insert(x);
   }
   prints("Enter element to search for in the tree: ");
   i = scani();
   temp = search(i);

   if(temp != 0) {
      prints("\ngiven element found in the tree.");
   }else {
      prints("\ngiven element not found.");
   }            

   prints("\nPreorder traversal: ");
   pre_order_traversal(root);

   prints("\nInorder traversal: ");
   inorder_traversal(root);

   prints("\nPost order traversal: ");
   post_order_traversal(root);       

   return 0;
}