// void printi(int x);
// void printf(float x);
// void prints(char* str);
// int scani();
// float scanf();
// void scans(char* str, int len);
// void* malloc(int size);
// int strlen(char* str);
// int isqrt(int num);
// float log10(float x);
// float floor(float x);
// int power(int x, int y);
// float sin(float x);
// float cos(float x);
struct soint {
	float m, k;
};
struct point {
	int x;
	struct soint* s;
};
struct list {
	int val;
	void* next;
};
struct list* insert_node(int val){
	struct list* node = malloc(sizeof(struct list));
	node->val = val;
	node->next = NULL;
	return node;
}
int main(){
	/*printi(strlen("hello"));
	printi(isqrt(scani()));
	printf(log10(scanf()));
	printf(floor(scanf()));
	printi(power(scani(), scani()));
	printf(sin(scanf()));
	printf(cos(scanf()));
	printi(4);*/
	struct point arr[10];
	(*(arr+5)).x = 1;
	(*(arr+5)).s = malloc(sizeof(struct soint));
	(arr+5)->s->m = 2.3;
	(arr+5)->s->m++;
	(*(arr[5].s)).k = 3.4;
	(*(arr[5].s)).k++;
	printi(arr[5].x++); printf(arr[5].s->m++); printf(++arr[5].s->k);
	// int i;
	// struct list* curr;
	// struct list* prev;
	// struct list* head = malloc(sizeof(struct list));
	// head->val = 0;
	// head->next = NULL;
	// prev = head;
	// for (i = 1; i<5; ){
	// 	curr = insert_node(i);
	// 	prev->next = curr;
	// 	prev = curr;
	// 	i += 1;
	// }
	// while (head != NULL){
	// 	printi(head->val);
	// 	head = head->next;
	// }
}