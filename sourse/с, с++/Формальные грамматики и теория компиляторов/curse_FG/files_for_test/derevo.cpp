
struct tree { // дерево
	struct tree* p_left;
	struct tree* p_right;
	long double znach;
	int count;
	unsigned char height; // вес вершины (самые нижние 1 и повышается на 1 за каждый уровень вверх, у NULL вершин вес 0)
};


unsigned char height(struct tree* p)//вывод веса поддерева 
{
	if (p != NULL) {
		return p->height;
	}
	else {
		return 0;
	}
}


void fixheight(struct tree* p)
{
	unsigned char hl = height(p->p_left);
	unsigned char hr = height(p->p_right);
	if (hl > hr) {
		p->height = hl + 1;
	}
	else {
		p->height = hr + 1;
	}
}

struct tree* right(struct tree* p) // правый поворот 
{
	struct tree* q = p->p_left;
	p->p_left = q->p_right;
	q->p_right = p;
	fixheight(p);
	fixheight(q);
	return q;
}

struct tree* left(struct tree* q) // левый поворот 
{
	struct tree* p = q->p_right;
	q->p_right = p->p_left;
	p->p_left = q;
	fixheight(q);
	fixheight(p);
	return p;
}

struct tree* balance(struct tree* p) // балансировка 
{
	fixheight(p);
	if (height(p->p_right) - height(p->p_left) == 2)// ситуация родитель->правый->левый
	{
		if (height(p->p_right->p_right) - height(p->p_right->p_left) < 0)
			p->p_right = right(p->p_right);
		return left(p);
	}
	if (height(p->p_right) - height(p->p_left) == -2)// ситуация родитель->левый->правый
	{
		if (height(p->p_left->p_right) - height(p->p_left->p_left)> 0)
			p->p_left = left(p->p_left);
		return right(p);
	}
	return p; 
}

struct tree* addi(struct tree* p, long double znachnew)
{
	if (p == NULL) {
		struct tree* newt;
		newt = (struct tree*)malloc(sizeof(struct tree));
		if (newt == NULL) {
			exit(-1);
		}
		newt->znach = znachnew;
		newt->p_left = NULL;
		newt->p_right = NULL;
		newt->height = 1;
		newt->count = 1;
		p = newt;
		return(p);}
	if (znachnew < p->znach)
		p->p_left = addi(p->p_left, znachnew);
	else if (znachnew > p->znach)
		p->p_right = addi(p->p_right, znachnew);
	else if (znachnew == p->znach)
		p->count += 1;

	return balance(p);
}

void travel(struct tree* tre, int* n) {
	int k=-1;
	if (tre != NULL) {
		if (tre->p_left != NULL) {
			travel(tre->p_left, n);
			//printf("%d \n", tre->znach);
		}
		*n = *n + 1;
		//if (k != tre->znach) {
		printf("%lf %d\n",tre->znach,tre->count);
			//k = tre->znach;
		//}
		//printf("%d \n", tre->znach);
		//k = tre->znach;
		
		//printf("%d \n", tre->znach);
		if (tre->p_right != NULL) {
			travel(tre->p_right, n);
		}
	}
}

void deletetree(struct tree** tre) {
	if ((*tre)->p_left != NULL) {
		deletetree(&((*tre)->p_left));
	}
	if ((*tre)->p_right != NULL) {
		deletetree(&((*tre)->p_right));
	}
	free(*tre);
	*tre = NULL;
}

int number(char a) {
	switch (a) {
	case '0': return 0;
	case '1': return 1;
	case '2': return 2;
	case '3': return 3;
	case '4': return 4;
	case '5': return 5;
	case '6': return 6;
	case '7': return 7;
	case '8': return 8;
	case '9': return 9;
	}
	return -1;
}
int alphas(char a) {
	switch (a) {
	case 'A': return 0;
	case 'B': return 1;
	case 'C': return 2;
	case 'D': return 3;
	case 'E': return 4;
	case 'F': return 5;	
	}
	return -1;
}

char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

void main() {
	FILE* fp;
	FILE* fa;
	FILE* f;
	//"rarcrypt20.cl"
	//fopen("aes_core.c", "r");
	fp = fopen("aaa","r");
	fa = fopen("VEGINER_.txt", "w");
	char k = ' ';
	int t = 0;
	int t1 = 0;
	
	int j = 0;
	
	char k1 = ' ';
	char k3 = ' ';

        printf ("1111");
	//if (fp == NULL) {j = 0; }
	
	while (k != EOF) {
		k3 = k1;
		k1 = k;
		k = fgetc(fp);
		if (k == EOF) {
			break;
		}
				
		if (k == '"' && t1%2==0) {
			t += 1;
		}
		if (k == '\'' && t%2==0) {
			t1 += 1;
		}
		if (k == '\n') {
			//k1 = ungetc(k, fp);
			if (k1!= '\\' && k1!='\n' ) {
				t = 0;
				//t1 = 0;
			}
			if (k1 == '\\' && k3 == '\\') {
				t = 0;
			}
		
			if ( k1 == '\n') {
				j = 0;
				//t1 = 0;
			}
			t1 = 0;
			
		}
		if ((k == '/' && t % 2 == 0 && t1%2==0) || (j==1)) {
			k1 = k;
			k = fgetc(fp);
			if ((k == '/')|| j==1) {
				while (k != '\n' && k!=EOF) {
					k1 = k;
					k = fgetc(fp);					
				}
				
				if (k=='\n') {
					//k1 = ungetc(k, fp);
					if (k1 == '\\') {
						j = 1;
					}
					else {
						j = 0;
						t = 0;
						t1 = 0;
						
					}
					putc(k, fa);
					
				}
			}
			else if (k == '*') {
				while (k != EOF) {
					k = fgetc(fp);
					//fputc(' ', fa);
					if (k == '\n') {
						fputc('\n', fa);
					}
					if (k == '*') {
						k = fgetc(fp);
						if (k == '/') {
							//fputc(' ', fa);
							break;
						}
						else {
							ungetc(k, fp);
							
						}

					}
				}

			}
			else if (k == '"') {
				t += 1;
				putc(k1, fa);
				putc(k, fa);
			}
			else if (k == '\'') {
				t1 += 1;
				putc(k1, fa);
				putc(k, fa);
			}
			else {
				putc(k1, fa);
				putc(k, fa);
			}
		}
		else {
			fputc(k, fa);
		}
	
	}
	fclose(fa);
	fclose(fp);
	
	struct tree* koren = NULL;
	int n = 0, flagkom = 0, kk = 0, flagg = 1;
	
	char znaki[] = "+=-/[]{},()*&<>|^:;%";
	int l = 0;
	long double   totr;
	char s[50] = { "\0" };
	printf("a");
	char cc1=' ';
	int a=0;
	f = fopen("VEGINER_.txt", "r");
	//printf("%d %d", 'A', 'a');
	char cc = ' ';
	char c=' ';
	char ccc = ' ';
	while (!feof(f)) {
		//flagg = 0;
		cc = c;
		c = fgetc(f);		
		if ((number(c) >= 0) && !strchr(letters,cc) ) {
			kk = 0;
			totr = 0;
			l = 0;
			while (number(c) >= 0) {
				s[kk] = c;
				kk++;
				l++;	
				ccc = cc;
				cc = c;
				c = fgetc(f);
			}
			s[kk + 1] = ' ';
			kk--;
			for (int i = 0; i < l; i++) {
				totr += number(s[i]) * pow(10, kk);
				kk--;

			}			
			if (c!='.' && !strchr(letters,c)) {
				if (totr == 6) {
					printf(" - %c %c\n",ccc,c);
					a += 1;
				}
				//printf("%s %lf %d\n",s, totr,l);
				koren = addi(koren, totr);
			}
			
			else {
				//c = fgetc(f);
				//&& c != '\n' && c != EOF
				while (c != ' ' && c != '\n' && c != EOF && !strchr(znaki,c) && c!='"' && c != '\t') {
					c = fgetc(f);
				}
			}	
							
		} 
		else {
			while (c != ' ' && c != '\n' && c != EOF && !strchr(znaki, c)  && c!='\t') {
				c = fgetc(f);
			}
		}
		
	}
	printf("  %d  ", a);
	printf("=======================\n");
	travel(koren, &n);
	printf("%d", n);
	fclose(f);
	deletetree(&koren);
}
