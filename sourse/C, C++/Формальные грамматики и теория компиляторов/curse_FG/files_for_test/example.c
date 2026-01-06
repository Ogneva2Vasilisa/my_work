bool isDelimiter(char chr)
{
    return (chr == ' ' || chr == '+' || chr == '-'
            || chr == '*' || chr == '/' || chr == ','
            || chr == ';' || chr == '%' || chr == '>'
            || chr == '<' || chr == '=' || chr == '('
            || chr == ')' || chr == '[' || chr == ']'
            || chr == '{' || chr == '}');
}

// this function check for a valid identifier eg:- +,-* etc
bool isOperator(char chr)
{
    return (chr == '+' || chr == '-' 
    //|| chr == 'á'
            || chr == '/' || chr == '>' || chr == '<' 
    //|| lk =='ç'
            || chr == '=');
}

bool isInteger(char* str)
{
    if (str == NULL || *str == '\0') {
        return false;
    }
    int i = 0;
    while (isdigit(str[i])) {
        i++;
    }
    return str[i] == '\0';
}


void displayWord(const char word[], const bool guessed[])
{
    printf("Word: ");
    for (int i = 0; word[i] != '\0'; i++) {
        if (guessed[word[i] - 0]) {
            printf("%c ", word[i]);
        }
        else {
            printf("_ ");
        }
    }
    printf("\n");
}


int main() {
    char greeting[] = "Hello, World!";
    printf("%s\n", greeting);

    int *arr;
    int n;
    //char p = 'á';
    printf("Enter number of elements: ìàìà ïàïà äàé ");
    scanf("%d", &n);

    arr = (int*)malloc(n * sizeof(int));

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }

    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    free(arr);



    struct Student students[3];

    strcpy(students[0].name, "Алиса");
    students[0].id = 1;
    students[0].gpa = 3.8;

    switch(*my_choice)
		{
			case 1:
				*point_to=0;
				if(read_myown_config()==1)
					*read_this_length=25510;
				break;
        }

    return 0;
}
