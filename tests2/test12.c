//Linear search in a linked list (Program to test Dynamic Memory)
/* Node structure */
struct node
{
    int data;          // Data
    void* next; // Address
};

struct node* head;
/**
 * Create a list of n nodes
 */
void createList(int n)
{
    struct node *newNode, *temp;
    int data, i;

    head = malloc(sizeof(struct node));

    /*
     * Unable to allocate memory, hence exit from app.
     */
    if (head == 0)
    {
        prints("Unable to allocate memory. terminating program.");
        exit(0);
    }
    

    /* Input head node data from user */
    prints("Enter data of node 1: ");
    data = scani();

    head->data = data; // Link data field with data
    head->next = 0; // Link address field to 0

    temp = head;

    /*
     * Create n nodes and add to the list
     */
    for (i = 2; i <= n; i++)
    {
        newNode = malloc(sizeof(struct node));

        /* If memory is not allocated for newNode */
        if (newNode == 0)
        {
            prints("Unable to allocate memory. Terminating program.");
            exit(0);
        }

        prints("Enter data of node : ");
        data = scani();

        newNode->data = data; // Link data field of newNode
        newNode->next = 0; // The newNode should point to nothing

        temp->next = newNode; // Link previous node i.e. temp to the newNode
        temp = temp->next;
    }
    
}


/**
 * Display entire list
 */
void displayList()
{
    struct node *temp;

    /*
     * If the list is empty i.e. head = 0,
     * dont perform any action and return.
     */
    if (head == 0)
    {
        prints("List is empty");
        return;
    }
    
    temp = head;
    while (temp != 0)
    {
        printi(temp->data);
        temp = temp->next;  // Move to next node
    }
}


/**
 * Search an element with given key in linked list.
 * It return a positive integer specifying index of the element 
 * on success, otherwise returns -1.
 */
int search(int key)
{
    int index;
    struct node *curNode;

    index = 0;
    curNode = head;

    // Iterate till last element until key is not found
    while (curNode != 0 && curNode->data != key)
    {
        index++;
        curNode = curNode->next;
    }


    return (curNode != 0) ? index : -1;
}

int main()
{
    int n, keyToSearch, index;

    // Input node count to create
    prints("Enter number of node to create: ");
    n = scani();

    createList(n);

    // Display list
    prints("Data in the list: ");
    displayList();

    // Input element to search from user.
    prints("Enter element to search: ");
    keyToSearch = scani();

    // Call function to search first element by key
    index = search(keyToSearch);
    
    // Element found in the list
    if (index >= 0){
        prints("key found in the list at position :");
        printi(index+1);
    }
    else
        prints("key not found in the list.");

    return 0;
}
