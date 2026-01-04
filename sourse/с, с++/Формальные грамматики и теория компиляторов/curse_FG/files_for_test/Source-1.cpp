
struct SplayTree
{
	long double chislo;
	int kol;
	struct SplayTree* parent;
	struct SplayTree* left;
	struct SplayTree* right;
};

char numbers[] = "1234567890";
char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";

SplayTree* Search(SplayTree* root, long double number)
{
	if (root->chislo == number)
		return root;
	else
	{
		if (root->chislo < number)
		{
			if (root->right == NULL)
				return root;
			Search(root->right, number);
		}
		else if (root->chislo > number)
		{
			if (root->left == NULL)
				return root;
			Search(root->left, number);
		}
	}
}

void Zig_Left(SplayTree* root)
{
	//printf("Zig Left\n");
	root->parent->left = root->right;
	root->right = root->parent;
	root->parent = root->right->parent;
	root->right->parent = root;
	if (root->right->left != NULL)
		root->right->left->parent = root->right;
	if (root->parent != NULL)
	{
		if (root->parent->left == root->right)
			root->parent->left = root;
		else if (root->parent->right == root->right)
			root->parent->right = root;
	}
}

void Zig_Right(SplayTree* root)
{
	//printf("Zig Right\n");
	root->parent->right = root->left;
	root->left = root->parent;
	root->parent = root->left->parent;
	root->left->parent = root;
	if (root->left->right != NULL)
		root->left->right->parent = root->left;
	if (root->parent != NULL)
	{
		if (root->parent->left == root->left)
			root->parent->left = root;
		else if (root->parent->right == root->left)
			root->parent->right = root;
	}
}

void Zig_Zig_Left(SplayTree* root)
{
	//printf("Zig Zig Left\n");
	Zig_Left(root->parent);
	Zig_Left(root);
}

void Zig_Zig_Right(SplayTree* root)
{
	//printf("Zig Zig Right\n");
	Zig_Right(root->parent);
	Zig_Right(root);
}

void Zig_Zag_Left(SplayTree* root)
{
	//printf("Zig Zag Left\n");
	Zig_Right(root);
	Zig_Left(root);
}

void Zig_Zag_Right(SplayTree* root)
{
	//printf("Zig Zag Right\n");
	Zig_Left(root);
	Zig_Right(root);
}

SplayTree* Add(SplayTree* root, long double number)
{
	SplayTree* place = (SplayTree*)malloc(sizeof(SplayTree));
	if (root == NULL)
	{
		SplayTree* new_root = (SplayTree*)malloc(sizeof(SplayTree));
		new_root->chislo = number;
		new_root->kol = 1;
		new_root->parent = NULL;
		new_root->left = NULL;
		new_root->right = NULL;
		place = new_root;
	}
	else
	{
		place = Search(root, number);
		//printf("search place %d\n", place->chislo);
		if (place->chislo != number)
		{
			SplayTree* new_root = (SplayTree*)malloc(sizeof(SplayTree));
			new_root->chislo = number;
			new_root->kol = 1;
			new_root->parent = place;
			new_root->left = NULL;
			new_root->right = NULL;
			if (place->chislo < number)
				place->right = new_root;
			else
				place->left = new_root;
			place = new_root;
		}
		else
			place->kol++;
	}
	while (place->parent != NULL)
	{
		if (place->parent->parent != NULL)
		{
			//printf("parent %d\nparent parent %d\n",place->parent->chislo, place->parent->parent->chislo);
			if (place == place->parent->right && place->parent == place->parent->parent->left)
				Zig_Zag_Left(place);
			else
				if (place == place->parent->left && place->parent == place->parent->parent->right)
					Zig_Zag_Right(place);
				else
					if (place == place->parent->left && place->parent == place->parent->parent->left)
						Zig_Zig_Left(place);
					else
						Zig_Zig_Right(place);
		}
		else
		{
			if (place == place->parent->left)
				Zig_Left(place);
			else
				Zig_Right(place);
		}
	}
	return place;
}

void Print_Tree(SplayTree* root)
{
	if (root == NULL)
		return;
	printf("number = %f kol = %d\n", root->chislo, root->kol);
	Print_Tree(root->left);
	Print_Tree(root->right);
}

int main()
{
	FILE* file = fopen("213.cl", "rt");
	SplayTree* root = NULL;
	char sim;
	long double number = 0, step = 0.1;
	int chisl_flag = 0, skip_flag = 0, start_flag = 0, end_flag = 0, neg_flag = 0, ruin_flag = 0, check_letter_flag = 1;
	while (!feof(file))
	{
		sim = fgetc(file);
		//printf("%c", sim);
		if (sim == '"')
		{
			//fputc(sim, file_output);
			sim = fgetc(file);
			while (sim != '"')
			{
				if (sim == '\n')
					break;
				if (sim == '\\')
				{
					//fputc(sim, file_output);
					sim = fgetc(file);
					while (sim != '\n' && sim != '"')
					{
						if (sim != ' ')
							break;
						sim = fgetc(file);
					}
					if (sim == '"')
						break;
				}
				//fputc(sim, file_output);
				sim = fgetc(file);
			}
			//fputc(sim, file_output);
			continue;
		}
		if (sim == '\'')
		{
			//fputc(sim, file_output);
			sim = fgetc(file);
			while (sim != '\'' && sim != '\n')
			{
				//fputc(sim, file_output);
				sim = fgetc(file);
			}
			//fputc(sim, file_output);
			continue;
		}
		if (sim == '/')
		{
			int flag1 = 0, flag2 = 0;
			char second_sim = fgetc(file);
			if (second_sim == '*')
			{
				flag1 = 1;
				if ((second_sim = fgetc(file)) == '\n')
					//fputc(second_sim, file_output);
					while (true && second_sim != EOF)
					{
						sim = second_sim;
						second_sim = fgetc(file);
						//printf("%c %c\n", sim, second_sim);
						//if (second_sim == '\n')
							//fputc(second_sim, file_output);
						if (sim == '*' && second_sim == '/')
							break;
					}
				continue;
			}
			else
				if (second_sim == '/')
				{
					flag2 = 1;
					while (true && sim != EOF)
					{
						sim = fgetc(file);
						if (sim == '\n')
						{
							//fputc(sim, file_output);
							break;
						}
						if (sim == '\\')
						{
							sim = fgetc(file);
							while (sim != '\n')
							{
								if (sim != ' ')
									break;
								sim = fgetc(file);
							}
						}
					}
					continue;

				}
			if (flag1 + flag2 == 0)
			{
				//fputc(sim, file_output);
				fseek(file, -1, SEEK_CUR);
				continue;
			}
		}
		//rintf("%c", sim);
		if (!strchr(letters, sim) && !strchr(numbers, sim))
		{
			ruin_flag = 0;
		}
		if (sim == '-')
		{
			neg_flag = 1;
			sim = fgetc(file);
			if (sim == '\'')
			{
				fseek(file, -1, SEEK_CUR);
				continue;
			}
		}
		if (sim == '.')
		{
			skip_flag = 1;
			sim = fgetc(file);
		}
		//printf("%c %d %d\n", sim, ruin_flag, chisl_flag);
		if (strchr(numbers, sim) && ruin_flag == 0)
		{
			if (check_letter_flag == 0)
			{
				check_letter_flag = 1;
				fseek(file, -2, SEEK_CUR);
				char sim2 = fgetc(file);
				//printf("%c\n", sim2);
				if (strchr(letters, sim2))
				{
					//printf("1");
					ruin_flag = 1;
				}
				else
					fseek(file, 1, SEEK_CUR);
			}
			if (chisl_flag == 0)
				chisl_flag = 1;
			if (skip_flag == 0)
			{
				if (start_flag == 0)
					start_flag = 1;
				number *= 10;
				number += sim - '0';
			}
			else
			{
				if (end_flag == 0)
					end_flag = 1;
				number += (sim - '0') * step;
				step /= 10;
			}
		}
		else
		{
			if (ruin_flag == 1)
				chisl_flag = 0;
			if (chisl_flag == 0)
			{
				neg_flag = 0;
				skip_flag = 0;
				check_letter_flag = 0;
			}
			if (chisl_flag == 1 && ruin_flag == 0)
			{
				if (neg_flag == 1)
					number = -number;
				//printf("number %d\n", number);
				//Print_Tree(root);
				//printf("\n");
				root = Add(root, number);
				step = 0.1;
				neg_flag = 0;
				end_flag = 0;
				skip_flag = 0;
				start_flag = 0;
				chisl_flag = 0; 
				check_letter_flag = 0;
			}
			number = 0;
		}
	}
	Print_Tree(root);
	fclose(file);
}