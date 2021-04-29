void printf(char* str, ...);
void scanf(char* str, ...);

int main() {
    char operator;
    double n1, n2;

    printf("Enter an operator (+, -, *, /): ");
    scanf("%c", &operator);
    printf("Enter two operands: ");
    scanf("%lf %lf",&n1, &n2);

    switch(operator)
    {
        case 1:
            printf("%.1lf + %.1lf = %.1lf",n1, n2, n1+n2);
            break;

        case 2:
            printf("%.1lf - %.1lf = %.1lf",n1, n2, n1-n2);
            break;

        case 3:
            printf("%.1lf * %.1lf = %.1lf",n1, n2, n1*n2);
            break;

        case 4:
            printf("%.1lf / %.1lf = %.1lf",n1, n2, n1/n2);
            break;

        default:
            printf("Error! operator is not correct");
    }

    return 0;
}