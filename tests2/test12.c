//Linear search in a linked list (Program to test Dynamic Memory)

struct node
{
    int data;          // Data
    void* next; // Address
};

struct node* head = NULL;

void createList(int n)
{
    struct node *newNode, *temp;
    int data, i;

    head = malloc(sizeof(struct node));

    if (head == NULL)
    {
        prints("Unable to allocate memory. terminating program.");
        exit();
    }

    prints("Enter data of node 1: ");
    data = scani();

    head->data = data; // Link data field with data
    head->next = NULL; // Link address field to 0

    temp = head;

    for (i = 2; i <= n; i++)
    {
        newNode = malloc(sizeof(struct node));

        /* If memory is not allocated for newNode */
        if (newNode == NULL)
        {
            prints("Unable to allocate memory. Terminating program.");
            exit();
        }

        prints("Enter data for node : ");
        data = scani();

        newNode->data = data; // Link data field of newNode
        newNode->next = NULL; // The newNode should point to nothing

        temp->next = newNode; // Link previous node i.e. temp to the newNode
        temp = temp->next;
    }
}

void displayList()
{
    struct node *temp;

    if (head == NULL)
    {
        prints("List is empty");
        return;
    }

    temp = head;
    while (temp != NULL)
    {
        printi(temp->data);
        temp = temp->next;  // Move to next node
    }
}

int search(int key)
{
    int index;
    struct node *curNode;

    index = 0;
    curNode = head;

    // Iterate till last element until key is not found
    while (curNode != NULL && curNode->data != key)
    {
        index++;
        curNode = curNode->next;
    }
    
    return (curNode != NULL) ? index : -1;
}

int main()
{
    int n, keyToSearch, index;

    // Input node count to create
    prints("Enter number of node to create list of: ");
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
