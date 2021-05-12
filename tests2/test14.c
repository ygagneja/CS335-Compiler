// Program to implement Graph as adjacency list/matrix (Dynamic Memory)
struct head {
  int vertex;
  void* next;
  void* down;
};
struct node {
  int vertex;
  void* next;
};

// Create a node
struct node* createNode(int v) {
  struct node* newNode = malloc(sizeof(struct node));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

// Create a graph
struct head* createAGraph(int vertices) {
  int i;
  struct head* head = malloc(sizeof(struct head));
  struct head* curr = head;
  if (vertices == 0) return NULL;
  head->vertex = 0;
  head->next = head->down = NULL;
  for (i = 1; i < vertices; i++){
    struct head* temp = malloc(sizeof(struct head));
    curr->down = temp;
    temp->vertex = i;
    temp->next = temp->down = NULL;
    curr = temp;
  }
  return head;
}

// Add edge
void addEdge(struct head* graph, int s, int d, int vertices) {
  // Add edge from s to d
  int i;
  struct head* curr = graph;
  struct node* ptr;
  struct node* newNode = createNode(d);
  for (i=0; i<s; i++){
    curr = curr->down;
  }
  if (curr->next == NULL) curr->next = newNode;
  else {
    ptr = curr->next;
    while (ptr->next) ptr = ptr->next;
    ptr->next = newNode;
  }

  curr = graph;

  // Add edge from d to s
  newNode = createNode(s);
  for (i=0; i<d; i++){
    curr = curr->down;
  }
  if (curr->next == NULL) curr->next = newNode;
  else {
    ptr = curr->next;
    while (ptr->next) ptr = ptr->next;
    ptr->next = newNode;
  }
}

// Print the graph
void printGraph(struct head* graph) {
  while (graph){
    struct node* curr = graph->next;
    while (curr){
      printi(curr->vertex);
      curr = curr->next;
    }
    prints("\n");
    graph = graph->down;
  }
}

int main() {
  struct head* graph = createAGraph(4);
  addEdge(graph, 0, 1, 4);
  addEdge(graph, 0, 2, 4);
  addEdge(graph, 0, 3, 4);
  addEdge(graph, 1, 2, 4);

  printGraph(graph);

  return 0;
}