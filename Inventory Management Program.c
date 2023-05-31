#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define SIZE 100

#define COMPANY_NAME "������"
#define PROGRAM_NAME "Fast System"
#define PROGRAM_VER "v1.0"

typedef struct item {
	char number[SIZE]; // ��ǰ��ȣ 
	char name[SIZE]; // ��ǰ�� 
	int price; // �ݾ� 
	char place[SIZE]; // ��ġ 
	int count; // ���� 
} ITEM;

void menu();
ITEM get_record();
void print_record(ITEM data);
void add_record(FILE* fp);
void search_record(FILE* fp);
void update_record(FILE* fp);
void load_all_record(FILE* fp);
void remove_record(FILE* fp);
void update_only_count(FILE* fp);

int main(void)
{
	FILE* fp;
	int select;
	if ((fp = fopen("items.dat", "a+")) == NULL) {
		fprintf(stderr, "������ �Էµ� ������ �� �� �����ϴ�.");
		exit(1);
	}

#ifdef COMPANY_NAME
	printf("%s ", COMPANY_NAME);
#endif

#ifdef PROGRAM_NAME
	printf("%s", PROGRAM_NAME);
#else
	printf("������ �ý���");
#endif

#ifdef PROGRAM_VER
	printf(" %s\n", PROGRAM_VER);
#else
	printf("\n");
#endif

	while (1) {
		menu();
		printf("�������� �Է��Ͻÿ�: ");
		scanf("%d", &select);
		printf("====================\n");

		switch (select) {
		case 1: //��� �˻� 
			search_record(fp);
			break;
		case 2: // ��� ��ü ��Ȳ
			load_all_record(fp);
			break;
		case 3: // ��� ����
			update_record(fp);
			break;
		case 4: // ��� �߰� 
			add_record(fp);
			break;
		case 5: // ��� ���� ���� 
			remove_record(fp);
			break;
		case 6: // ��� ���� ����
			update_only_count(fp);
			break;
		case 7: // ���� 
			fclose(fp);
			return 0;
		}
		if (!(select > 0 && select < 8))
			printf("�ùٸ� ��ɾ �Է��� �ּ���!\n");
	}
}

ITEM get_record()
{
	ITEM data;
	getchar();
	printf("��ǰ��ȣ: ");
	gets_s(data.number, SIZE);
	printf("��ǰ��: ");
	gets_s(data.name, SIZE);
	printf("����: ");
	scanf("%d", &data.price);
	getchar();
	printf("��ġ: ");
	gets_s(data.place, SIZE);
	printf("����: ");
	scanf("%d", &data.count);
	getchar();
	return data;
}

void print_record(ITEM data)
{
	printf("��ǰ��ȣ: %s\n", data.number);
	printf("��ǰ��: %s\n", data.name);
	printf("�ܰ�: %d\n", data.price);
	printf("��ġ: %s\n", data.place);
	printf("����: %d\n", data.count);
}

void menu()
{
	printf("====================\n");
	printf(" 1. ��� �˻�\n 2. ��ü ��� ��Ȳ \n 3. ��� ���� ����\n 4. ��� ���� �߰�\n 5. ��� ���� ����\n 6. ��� ���� ����\n 7. ����\n");
	printf("====================\n");
}

void add_record(FILE* fp)
{
	ITEM getdata;
	ITEM data;
	getdata = get_record();
	FILE* fp1;
	fseek(fp, 0, SEEK_SET);
	int status = 0;

	if ((fp1 = fopen("temp.dat", "w")) == NULL)
	{
		fprintf(stderr, "���� temp.dat�� �� �� �����ϴ�");
		exit(1);
	}

	while (fread(&data, sizeof(data), 1, fp) == 1) {
		if (strcmp(data.number, getdata.number) == 0)
		{
			printf("�̹� ���� ��ǰ��ȣ�� ��ϵ� ��ǰ�� �־� ����� �� �����ϴ�.\n");
			status = 1;
		}
		fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 0)
	{
		fwrite(&getdata, sizeof(getdata), 1, fp1);
		printf("����Ǿ����ϴ�!\n");
	}
	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "���� ó�� �� ���� items.dat�� �� �� �����ϴ�");
		exit(1);
	}
}

void search_record(FILE* fp)
{
	char name[SIZE];
	ITEM data;
	int status = 0;

	fseek(fp, 0, SEEK_SET);
	getchar();
	while (1)
	{
		int user = 0;
		printf("1. ��ǰ��ȣ�� �˻�\n2. ��ǰ������ �˻�\n");
		printf("====================\n");
		printf("�������� �Է��Ͻÿ�: ");
		scanf("%d", &user);
		getchar();
		printf("====================\n");
		if (user == 1)
		{
			printf("�˻��� ��ǰ��ȣ: ");
			gets_s(name, SIZE);
			while (!feof(fp)) {
				fread(&data, sizeof(data), 1, fp);
				if (strcmp(data.number, name) == 0) {
					print_record(data);
					status = 1;
					break;
				}
			}
			break;
		}
		else if (user == 2)
		{
			printf("�˻��� ��ǰ��: ");
			gets_s(name, SIZE);
			while (!feof(fp)) {
				fread(&data, sizeof(data), 1, fp);
				if (strcmp(data.name, name) == 0) {
					print_record(data);
					status = 1;
					break;
				}
			}
			break;
		}
		else
			printf("�ùٸ� ��ɾ �ٽ� �Է��� �ּ���.\n");

	}
	if (status == 0)
		printf("%s�� �����͸� ã�� �� �����ϴ�.\n", name);
}

void update_record(FILE* fp)
{
	char name[SIZE] = "";
	ITEM data;
	FILE* fp1;
	int user = 0;
	int user_status = 0;
	int status = 0;
	if ((fp1 = fopen("temp.dat", "w")) == NULL)
	{
		fprintf(stderr, "���� temp.dat�� �� �� �����ϴ�");
		exit(1);
	}
	getchar();
	printf("������ ��ǰ��ȣ�� �Է��� �ּ���.: ");
	gets_s(name, SIZE);

	fseek(fp, 0, SEEK_SET);

	while (fread(&data, sizeof(data), 1, fp) == 1) {
		if (strcmp(data.number, name) == 0)
		{
			if (user_status == 0)
			{
				printf("====================\n");
				printf("������ ��ǰ�� ���� ������ ������ �����ϴ�.\n");
				print_record(data);
				printf("====================\n");
				while (user_status == 0)
				{
					printf("������ �����ϰڽ��ϱ�?(��:1, �ƴϿ�:2): ");
					scanf("%d", &user);
					getchar();
					if (user == 1 || user == 2)
					{
						user_status = 1;
						break;
					}
					else
						printf("�ùٸ� ��ɾ �ٽ� �Է��� �ּ���!\n");
				}
			}
			if (user == 1 && status == 0)
			{
				printf("�����ϰ��� �ϴ� ��ǰ��ȣ: ");
				gets_s(data.number, SIZE);
				printf("�����ϰ��� �ϴ� ��ǰ��: ");
				gets_s(data.name, SIZE);
				printf("�����ϰ��� �ϴ� �ܰ�: ");
				scanf("%d", &data.price);
				getchar();
				printf("�����ϰ��� �ϴ� ��ġ: ");
				gets_s(data.place, SIZE);
				printf("�����ϰ��� �ϴ� ����: ");
				scanf("%d", &data.count);
				status = 1;
			}
			else if (user == 2)
				status = 2;
		}
		fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 1)
		printf("���������� �����Ǿ����ϴ�.\n");
	else if (status == 2)
		printf("������ ��ҵǾ����ϴ�.\n");
	else
		printf("������ �����͸� ã�� �� �����ϴ�. %s��(��) ����Ǿ� ���� �ʽ��ϴ�.\n", name);

	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "���� ó�� �� ���� items.dat�� �� �� �����ϴ�");
		exit(1);
	}
}

void load_all_record(FILE* fp)
{
	int id = 0;
	int task = 0;
	int status = 0;
	ITEM data;
	fseek(fp, 0, SEEK_SET);

	while (!feof(fp) && fread(&data, sizeof(data), 1, fp) == 1)
	{
		if (task == 0)
		{
			printf("��ü ��� ��Ȳ�Դϴ�.\n");
			printf("��=======================================================================================��\n");
			printf("�� ���� �� ��ǰ��ȣ        �� ��ǰ��                    �� �ܰ�    �� ����      �� ��ġ       ��\n");
			printf("��------��-----------------��---------------------------��---------��-----------��------------��\n");
			task = 1;
		}
		id++;
		printf("�� %-5d�� %-16s�� %-26s�� %-8d�� %-10d�� %-11s��\n", id, data.number, data.name, data.price, data.count, data.place);
		status = 1;
	}

	if (status == 1)
	{
		printf("��=======================================================================================��\n");
		printf("�� %d ������ ��ǰ�� ������ ��� �Ϸ�\n", id);
	}
	else
		printf("����� �����Ͱ� �����ϴ�.\n");
}

void remove_record(FILE* fp)
{
	char name[SIZE] = "";
	ITEM data;
	FILE* fp1;
	int status = 0;
	int user_status = 0;
	int user = 0;
	if ((fp1 = fopen("temp.dat", "w")) == NULL)
	{
		fprintf(stderr, "���� temp.dat�� �� �� �����ϴ�");
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	getchar();
	printf("������ ��ǰ��ȣ�� �Է��� �ּ���.: ");
	gets_s(name, SIZE);

	while (fread(&data, sizeof(data), 1, fp) == 1)
	{
		if (strcmp(data.number, name) == 0)
		{
			printf("====================\n");
			printf("������ ��ǰ�� ���� ������ ������ �����ϴ�.\n");
			print_record(data);
			printf("====================\n");
			while (user_status == 0)
			{
				printf("������ �����ϰڽ��ϱ�?(��:1, �ƴϿ�:2): ");
				scanf("%d", &user);
				getchar();
				if (user == 1)
				{
					status = 1;
					user_status = 1;
					break;
				}
				else if (user == 2)
				{
					status = 2;
					user_status = 1;
					fwrite(&data, sizeof(data), 1, fp1);
					break;
				}
				else
					printf("�ùٸ� ��ɾ �ٽ� �Է��� �ּ���!\n");
			}
		}
		else
			fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 1)
		printf("���������� �����Ǿ����ϴ�.\n");
	else if (status == 2)
		printf("������ ��ҵǾ����ϴ�.\n");
	else
		printf("������ �����͸� ã�� �� �����ϴ�. %s��(��) ����Ǿ� ���� �ʽ��ϴ�.\n", name);

	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "���� ó�� �� ���� items.dat�� �� �� �����ϴ�");
		exit(1);
	}
}

void update_only_count(FILE* fp)
{
	char name[SIZE] = "";
	ITEM data;
	FILE* fp1;
	int user = 0;
	int user_status = 0;
	int status = 0;
	if ((fp1 = fopen("temp.dat", "w")) == NULL)
	{
		fprintf(stderr, "���� temp.dat�� �� �� �����ϴ�");
		exit(1);
	}
	getchar();
	printf("������ ��ǰ��ȣ�� �Է��� �ּ���.: ");
	gets_s(name, SIZE);

	fseek(fp, 0, SEEK_SET);

	while (fread(&data, sizeof(data), 1, fp) == 1) {
		if (strcmp(data.number, name) == 0)
		{
			if (user_status == 0)
			{
				printf("====================\n");
				printf("������ ������ ��ǰ�� ���� ������ ������ �����ϴ�.\n");
				print_record(data);
				printf("====================\n");
				while (user_status == 0)
				{
					printf("������ �����ϰڽ��ϱ�?(��:1, �ƴϿ�:2): ");
					scanf("%d", &user);
					getchar();
					if (user == 1 || user == 2)
					{
						user_status = 1;
						break;
					}
					else
						printf("�ùٸ� ��ɾ �ٽ� �Է��� �ּ���!\n");
				}
			}
			if (user == 1 && status == 0)
			{
				printf("�����ϰ��� �ϴ� ����: ");
				scanf("%d", &data.count);
				status = 1;
			}
			else if (user == 2)
				status = 2;
		}
		fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 1)
		printf("���������� �����Ǿ����ϴ�.\n");
	else if (status == 2)
		printf("������ ��ҵǾ����ϴ�.\n");
	else
		printf("������ �����͸� ã�� �� �����ϴ�. %s��(��) ����Ǿ� ���� �ʽ��ϴ�.\n", name);

	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "���� ó�� �� ���� items.dat�� �� �� �����ϴ�");
		exit(1);
	}
}