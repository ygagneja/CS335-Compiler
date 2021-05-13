// Program to implement Graph as adjacency list/matrix (Dynamic Memory)
struct graph {
  int vertex;
  void* next;
  void* down;
};
struct vertex {
  int vertex;
  void* next;
};

// Create a node
struct vertex* createNode(int v) {
  struct vertex* newNode = malloc(sizeof(struct vertex));
  newNode->vertex = v;
  newNode->next = NULL;
  return newNode;
}

// Create a graph
struct graph* createAGraph(int vertices) {
  int i;
  struct graph* graph = malloc(sizeof(struct graph));
  struct graph* curr = graph;
  if (vertices == 0){
    prints("Please provide a non-zero value of vertices\n");
    exit();
  }
  graph->vertex = 0;
  graph->next = graph->down = NULL;
  for (i = 1; i < vertices; i++){
    struct graph* temp = malloc(sizeof(struct graph));
    curr->down = temp;
    temp->vertex = i;
    temp->next = temp->down = NULL;
    curr = temp;
  }
  return graph;
}

bool isExists(struct graph* graph, int s, int d){
  int i;
  struct graph* curr = graph;
  struct vertex* ptr;
  for (i=0; i<s; i++){
    curr = curr->down;
  }
  if (curr->next == NULL) return false;
  else {
    ptr = curr->next;
    while (ptr){
      if (ptr->vertex == d) return true;
      ptr = ptr->next;
    }
  }
  return false;
}

// Add edge
void addEdge(struct graph* graph, int s, int d, int vertices) {
  int i;
  struct graph* curr = graph;
  struct vertex* ptr;
  // Add edge from s to d
  if (s>=vertices || d>=vertices){
    prints("Please provide a valid edge\n");
    return;
  }
  if (isExists(graph, s, d) || isExists(graph, d, s)){
    prints("The edge already exists\n");
    return;
  }
  for (i=0; i<s; i++){
    curr = curr->down;
  }
  if (curr->next == NULL) curr->next = createNode(d);
  else {
    ptr = curr->next;
    while (ptr->next) ptr = ptr->next;
    ptr->next = createNode(d);
  }

  curr = graph;

  // Add edge from d to s
  for (i=0; i<d; i++){
    curr = curr->down;
  }
  if (curr->next == NULL) curr->next = createNode(s);
  else {
    ptr = curr->next;
    while (ptr->next) ptr = ptr->next;
    ptr->next = createNode(s);
  }
}

// Print the graph
void printGraph(struct graph* graph) {
  while (graph){
    struct vertex* curr = graph->next;
    printi(graph->vertex);
    while (curr){
      prints("-> ");
      printi(curr->vertex);
      curr = curr->next;
    }
    prints("\n");
    graph = graph->down;
  }
}

int main() {
  int vertices, edges, i;
  struct graph* graph;
  prints("Enter the number of vertices in graph :\n");
  vertices = scani();
  prints("Enter the number of edges in graph :\n");
  edges = scani();
  graph = createAGraph(vertices);
  for (i=0; i<edges; i++){
    prints("Enter edge number "); printi(i+1); prints("\n");
    addEdge(graph, scani(), scani(), vertices);
  }
  printGraph(graph);

  return 0;
}