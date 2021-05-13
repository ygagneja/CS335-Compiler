// Linear search in a linked list (Program to test Dynamic Memory)

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

    if (n == 0) return;

    head = malloc(sizeof(struct node));

    if (head == NULL)
    {
        prints("Unable to allocate memory, terminating program\n");
        exit();
    }

    prints("Enter data for node 1 :\n");
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
            prints("Unable to allocate memory, terminating program\n");
            exit();
        }

        prints("Enter data for node ");
        printi(i); prints("\n");
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
        prints("List is empty\n");
        return;
    }

    temp = head;
    while (temp != NULL)
    {
        printi(temp->data);
        temp = temp->next;  // Move to next node
    }
    prints("\n");
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

    prints("Enter number of nodes to create list of :\n");
    n = scani();

    createList(n);

    prints("Data in the list :\n");
    displayList();

    // Input element to search from user.
    prints("Enter element to search :\n");
    keyToSearch = scani();

    // Call function to search first element by key
    index = search(keyToSearch);
    
    // Element found in the list
    if (index >= 0){
        prints("Key found in the list at position : ");
        printi(index+1);
        prints("\n");
    }
    else
        prints("Key not found in the list\n");

    return 0;
}
