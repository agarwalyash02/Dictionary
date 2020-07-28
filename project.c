# include<stdio.h>
# include<stdlib.h>
# include<conio.h>
# include<string.h>
# include<stdbool.h>
# include<windows.h>
# include<time.h>
# define Alphabet_Size 26

void delay(int);
void gotoxy(int ,int);
void ClearConsoleToColors(int,int);
char password[9]="forensic";
struct dictionary
{
	char word[20];
	char meaning[100];
};
struct recents
{
	char searches[20];
};

struct recents arr[5];
int front=0;

void insertins()
{
	front=(front+1)%5;
	return;
}

void stackdisplay()
{
	int i,x=front-1;
	if(x==-1)
		{
			x=4;
		}
	for(i=0;i<5;i++)
	{
	    delay(250);
		printf("\n\t\t\t\t\t%s",arr[x].searches);
		x--;
		if(x==-1)
		{
			x=4;
		}
        x=x%5;
	}
	return;
}
struct trienode
{
	struct trienode *children[Alphabet_Size];
	bool isEnd;
};



struct trienode *getnode(void)
{
	struct trienode *new_node=(struct trienode *)malloc(sizeof(struct trienode));
	if(new_node)
	{
		int i;
		new_node->isEnd=false;
		for(i=0;i<Alphabet_Size;i++)
		{
			new_node->children[i]=NULL;
		}
		return new_node;
	}
}
int char_to_index(char ch)
{
	int i;
	if(ch>='a' && ch<='z')
	{
		i=ch-'a';
		return i;
	}
	else if(ch>='A'&&ch<='Z')
	{
		i=ch-'A';
		return i;
	}
	else
	{
		return -1;
	}
}
void insert(struct trienode *root,char a[])
{
	int level,length,index;
	length=strlen(a);
	struct trienode *n_node=root;


	for(level=0;level<length;level++)
	{
		index=char_to_index(a[level]);
		if(n_node->children[index]==NULL)
		{
			n_node->children[index]=getnode();
		}
		n_node=n_node->children[index];

	}
	n_node->isEnd=true;
}
void Dictionary_read_from_file(struct trienode *root)
{
	FILE *ptr;
	ptr=fopen("dictionary.txt","r");
	char array1[20],array2[100];
	while(!feof(ptr))
	{
		fscanf(ptr,"%s",array1);
		insert(root,array1);
		fgets(array2,100,ptr);
	}
	fclose(ptr);

}
void add()
{
	char user[20];
	system("cls");
	printf("Enter the details:\n\n");
	printf("enter the username :");
	scanf("%s",user);
	printf("enter your password:");
	char pass[20];
	int i=0;
	do
	{
		pass[i]=getch();
		if(pass[i]!='\r')
		{
			printf("*");
		}
		i++;
	}while(pass[i-1]!='\r');
	pass[i-1]='\0';
	if(strcmp(pass,password)!=0)
	{
		printf("\n\nPassword is incorrect\n");

		return;
	}
	struct dictionary *node;
	char a[20],mean[100];
	node=(struct dictionary *)malloc(sizeof(struct dictionary));
	FILE *ptr;
	ptr=fopen("dictionary.txt","a+");
	if(ptr==NULL)
	{
		printf("\nerror in creating file");
		return;
	}
	printf("\nenter the word you want to add:");
	scanf("%s",node->word);
	while(!feof(ptr))
	{
		fscanf(ptr,"%s",a);
		fgets(mean,100,ptr);
		if(strcmp(a,node->word)==0)
		{
			printf("word exist whose meaning is :-%s\n",mean);
			return;
		}
	}
	printf("enter the meaning of the word :");
	fflush(stdin);
	fgets(node->meaning,100,stdin);
	fprintf(ptr,"\n%s %s",node->word,node->meaning);
	fclose(ptr);
	return;
}
void display()
{
	system("CLS");
	FILE *ptr;
	ptr=fopen("dictionary.txt","r");
	if(ptr==NULL)
	{
		printf("error in opening a file ");
		return;
	}
	char a[20],mean[100];
	while(1)
	{
		fscanf(ptr,"%s",a);
		fgets(mean,100,ptr);
        if(feof(ptr))
            break;
	    printf("\n%s : %s",a,mean);

	}
	fclose(ptr);
	return;
}
bool search(struct trienode *root,char array2[])
{
	int level,index;
	struct trienode *n_node=root;
	for(level=0;level<strlen(array2);level++)
	{
		index=char_to_index(array2[level]);
		if(n_node->children[index]==NULL)
		{
			return false;
		}
		n_node=n_node->children[index];

	}
	if(n_node->isEnd==true)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool islastnode(struct trienode *n_node)
{
	int i;
	for(i=0;i<Alphabet_Size;i++)
	{
		if(n_node->children[i]);
		return 0;
	}
	return 1;
}
char *append(char *array2,char word)
{
	char *str=(char *)malloc(sizeof(char)*(strlen(array2)+2));
	int i=0;
	while(array2[i]!='\0')
	{
			str[i]=array2[i];
			i++;
	}

	str[i]=word;
	i++;
	str[i]='\0';
	return str;

}
void suggestrec(struct trienode *n_node,char *array)
{
	if(n_node->isEnd)
	{
		printf("%s\n",array);
	}
	if(islastnode(n_node))
	return;
	int i;
	for(i=0;i<Alphabet_Size;i++)
	{
		if(n_node->children[i]!=NULL)
		{
			suggestrec(n_node->children[i],append(array,i+97));
		}
	}
}
int suggestion(struct trienode *root,char array[])
{
	int level,index,length;
	length=strlen(array);
	struct trienode *n_node=root;
	for(level=0;level<length;level++)
	{
		index=char_to_index(array[level]);
		if(n_node->children[index]==NULL)
		{
			return 0;
		}
		n_node=n_node->children[index];
	}
	bool isword=(n_node->isEnd==true);
	bool islast=islastnode(n_node);
	if(isword&&islast)
    {
        return -1;
    }

	if(!islast)
	{
		suggestrec(n_node,array);
		return 1;
	}
}
void meaning(char *array)
{
    FILE *ptr;
    ptr=fopen("dictionary.txt","r");
    char a[20],mean[100];
    while(!feof(ptr))
	{
		fscanf(ptr,"%s",a);
		fgets(mean,100,ptr);
		if(strcmp(a,array)==0)
		{
			printf("\n\nmeaning of %s is:-  %s",array,mean);
			fclose(ptr);
			return;
		}
	}
	printf("\ngiven word is not present in the dictionary\n");
	fclose(ptr);
}

int main()
{
	int i,ch,val,y;
	char ans,x;
	char array[100];
	ClearConsoleToColors(1,15);
	gotoxy(50,14);
    printf("\a");
	printf("****DICTIONARY****");
    gotoxy(0,27);
	printf("press any key to continue");
	_getch();
    struct trienode *root=getnode();
	Dictionary_read_from_file(root);
	do
	{
		system("cls");
		ClearConsoleToColors(15,1);
		gotoxy(0,5);
		printf("          ----------------------------------------DICTIONARY----------------------------------------\n\n");
		delay(250);
		printf("\n\t\t\t\t\t\t1.Add Word\n");
		delay(250);
		printf("\n\t\t\t\t\t\t2.Show Dictionary\n");
		delay(250);
		printf("\n\t\t\t\t\t\t3.Search\n");
		delay(250);
		printf("\n\t\t\t\t\t\t4.recent search\n");
		delay(250);
		printf("\n\t\t\t\t\t\t0.Exit.");
		delay(250);
		printf("\n\n\t\t\t\t\t\tYour Choice:");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				add();
				Dictionary_read_from_file(root);
				break;
			case 2:
				display();
				break;
			case 3:
				printf("\nEnter the word you want to search:");
				scanf("%s",array);
				val=search(root,array);
				strcpy(arr[front].searches,array);
				insertins();
				if(val)
				{
					meaning(array);
				}
				else
				{
					printf("word not exist suggestion related to given word :\n");
					y=suggestion(root,array);
					if(y==0)
                    {
                        printf("no word found in dictionary of given combination:\n");
                        break;
                    }
                    char arr[20];
					printf("\nenter the word you want to search from the above list:-\t");
					scanf("%s",arr);
					meaning(arr);
				}
                break;
			case 4:
				printf("\t\t\t\trecent searches are:-  ");
				stackdisplay();
				break;
            case 0:
                exit( 0 );
            default:
				printf("wrong choice");
		}
		printf("\n\n\ndo you want to continue :Y or N");
		ans=_getch();
		printf("\n\n\n\n\n");
	}while(ans=='y');
	return 0;
}




void delay(int number_of_miliseconds)
{
    int milli_seconds=number_of_miliseconds;
    clock_t start_time=clock();
    while(clock()<start_time+milli_seconds)
    {
        ;
    }

}

void gotoxy(int x,int y)
{
    COORD c;
    c.X=x;
    c.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),c);
}


void ClearConsoleToColors(int ForgC, int BackC)
 {
 WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
 HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
 COORD coord = {0, 0};

   DWORD count;
 CONSOLE_SCREEN_BUFFER_INFO csbi;
 SetConsoleTextAttribute(hStdOut, wColor);
 if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
 {
      FillConsoleOutputCharacter(hStdOut, (TCHAR) 32, csbi.dwSize.X * csbi.dwSize.Y, coord, &count);

      FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, csbi.dwSize.X * csbi.dwSize.Y, coord, &count );
      SetConsoleCursorPosition(hStdOut, coord);
 }
 return;
}
