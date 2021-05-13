// pointer manipulation

struct data {
	float m, k;
};
struct point {
	int x;
	struct data* s;
};

int main(){
    struct point arr[10];
    char* str1 = "hello world";
    char* str2 = "hello there";
    char* tmp;
	(*(arr+5)).x = 1;
	(*(arr+5)).s = malloc(sizeof(struct data));
	(arr+5)->s->m = 2.3;
	(arr+5)->s->m++;
	(*(arr[5].s)).k = 3.4;
	(*(arr[5].s)).k++;
	printi(arr[5].x++); printf(arr[5].s->m++); printf(++arr[5].s->k);
    prints("\n");
    prints("Old strings : \n");
    prints(str1);
    prints("\n");
    prints(str2);
    prints("\n");
    tmp = str1;
    str1 = str2;
    str2 = tmp;
    prints("After swapping pointers : \n");
    prints(str1);
    prints("\n");
    prints(str2);
    prints("\n");
    str1[6] = ':'; str1[7] = ')'; str1[8] = 0;
    prints("After some manipulation : ");
    prints(str1);
    prints("\n");
    str1 += 6;
    prints("After some more manipulation : ");
    prints(str1);
    prints("\n");
}