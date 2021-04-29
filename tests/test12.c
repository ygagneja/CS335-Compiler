// Program to handle different data-types and type-casting


int main(){
    int a = 2.5;
    bool b = true;
    float c = (float) a;
    float d = (float)a;
    int e = (int)c;
    b = a; //should convert inttobool (no conversion now)
    b = c;
    // int-to-bool conversion possible but reverse not supported
}