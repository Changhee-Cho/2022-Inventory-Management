#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

#define SIZE 100

#define COMPANY_NAME "배재상사"
#define PROGRAM_NAME "Fast System"
#define PROGRAM_VER "v1.0"

typedef struct item {
	char number[SIZE]; // 상품번호 
	char name[SIZE]; // 상품명 
	int price; // 금액 
	char place[SIZE]; // 위치 
	int count; // 개수 
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
		fprintf(stderr, "정보가 입력된 파일을 열 수 없습니다.");
		exit(1);
	}

#ifdef COMPANY_NAME
	printf("%s ", COMPANY_NAME);
#endif

#ifdef PROGRAM_NAME
	printf("%s", PROGRAM_NAME);
#else
	printf("재고관리 시스템");
#endif

#ifdef PROGRAM_VER
	printf(" %s\n", PROGRAM_VER);
#else
	printf("\n");
#endif

	while (1) {
		menu();
		printf("정수값을 입력하시오: ");
		scanf("%d", &select);
		printf("====================\n");

		switch (select) {
		case 1: //재고 검색 
			search_record(fp);
			break;
		case 2: // 재고 전체 현황
			load_all_record(fp);
			break;
		case 3: // 재고 수정
			update_record(fp);
			break;
		case 4: // 재고 추가 
			add_record(fp);
			break;
		case 5: // 재고 정보 삭제 
			remove_record(fp);
			break;
		case 6: // 재고 개수 수정
			update_only_count(fp);
			break;
		case 7: // 종료 
			fclose(fp);
			return 0;
		}
		if (!(select > 0 && select < 8))
			printf("올바른 명령어를 입력해 주세요!\n");
	}
}

ITEM get_record()
{
	ITEM data;
	getchar();
	printf("상품번호: ");
	gets_s(data.number, SIZE);
	printf("상품명: ");
	gets_s(data.name, SIZE);
	printf("가격: ");
	scanf("%d", &data.price);
	getchar();
	printf("위치: ");
	gets_s(data.place, SIZE);
	printf("개수: ");
	scanf("%d", &data.count);
	getchar();
	return data;
}

void print_record(ITEM data)
{
	printf("상품번호: %s\n", data.number);
	printf("상품명: %s\n", data.name);
	printf("단가: %d\n", data.price);
	printf("위치: %s\n", data.place);
	printf("개수: %d\n", data.count);
}

void menu()
{
	printf("====================\n");
	printf(" 1. 재고 검색\n 2. 전체 재고 현황 \n 3. 재고 정보 수정\n 4. 재고 정보 추가\n 5. 재고 정보 삭제\n 6. 재고 개수 수정\n 7. 종료\n");
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
		fprintf(stderr, "파일 temp.dat를 열 수 없습니다");
		exit(1);
	}

	while (fread(&data, sizeof(data), 1, fp) == 1) {
		if (strcmp(data.number, getdata.number) == 0)
		{
			printf("이미 같은 상품번호로 등록된 상품이 있어 등록할 수 없습니다.\n");
			status = 1;
		}
		fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 0)
	{
		fwrite(&getdata, sizeof(getdata), 1, fp1);
		printf("저장되었습니다!\n");
	}
	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "삭제 처리 후 파일 items.dat를 열 수 없습니다");
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
		printf("1. 상품번호로 검색\n2. 상품명으로 검색\n");
		printf("====================\n");
		printf("정수값을 입력하시오: ");
		scanf("%d", &user);
		getchar();
		printf("====================\n");
		if (user == 1)
		{
			printf("검색할 상품번호: ");
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
			printf("검색할 상품명: ");
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
			printf("올바른 명령어를 다시 입력해 주세요.\n");

	}
	if (status == 0)
		printf("%s의 데이터를 찾을 수 없습니다.\n", name);
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
		fprintf(stderr, "파일 temp.dat를 열 수 없습니다");
		exit(1);
	}
	getchar();
	printf("수정할 상품번호를 입력해 주세요.: ");
	gets_s(name, SIZE);

	fseek(fp, 0, SEEK_SET);

	while (fread(&data, sizeof(data), 1, fp) == 1) {
		if (strcmp(data.number, name) == 0)
		{
			if (user_status == 0)
			{
				printf("====================\n");
				printf("수정할 상품의 현재 정보는 다음과 같습니다.\n");
				print_record(data);
				printf("====================\n");
				while (user_status == 0)
				{
					printf("수정을 진행하겠습니까?(예:1, 아니오:2): ");
					scanf("%d", &user);
					getchar();
					if (user == 1 || user == 2)
					{
						user_status = 1;
						break;
					}
					else
						printf("올바른 명령어를 다시 입력해 주세요!\n");
				}
			}
			if (user == 1 && status == 0)
			{
				printf("수정하고자 하는 상품번호: ");
				gets_s(data.number, SIZE);
				printf("수정하고자 하는 상품명: ");
				gets_s(data.name, SIZE);
				printf("수정하고자 하는 단가: ");
				scanf("%d", &data.price);
				getchar();
				printf("수정하고자 하는 위치: ");
				gets_s(data.place, SIZE);
				printf("수정하고자 하는 개수: ");
				scanf("%d", &data.count);
				status = 1;
			}
			else if (user == 2)
				status = 2;
		}
		fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 1)
		printf("성공적으로 수정되었습니다.\n");
	else if (status == 2)
		printf("수정이 취소되었습니다.\n");
	else
		printf("수정할 데이터를 찾을 수 없습니다. %s은(는) 저장되어 있지 않습니다.\n", name);

	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "수정 처리 후 파일 items.dat를 열 수 없습니다");
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
			printf("전체 재고 현황입니다.\n");
			printf("┌=======================================================================================┐\n");
			printf("│ 순번 │ 상품번호        │ 상품명                    │ 단가    │ 수량      │ 위치       │\n");
			printf("│------┼-----------------┼---------------------------┼---------┼-----------┼------------│\n");
			task = 1;
		}
		id++;
		printf("│ %-5d│ %-16s│ %-26s│ %-8d│ %-10d│ %-11s│\n", id, data.number, data.name, data.price, data.count, data.place);
		status = 1;
	}

	if (status == 1)
	{
		printf("└=======================================================================================┘\n");
		printf("총 %d 종류의 상품의 데이터 출력 완료\n", id);
	}
	else
		printf("출력할 데이터가 없습니다.\n");
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
		fprintf(stderr, "파일 temp.dat를 열 수 없습니다");
		exit(1);
	}
	fseek(fp, 0, SEEK_SET);
	getchar();
	printf("삭제할 상품번호를 입력해 주세요.: ");
	gets_s(name, SIZE);

	while (fread(&data, sizeof(data), 1, fp) == 1)
	{
		if (strcmp(data.number, name) == 0)
		{
			printf("====================\n");
			printf("삭제할 상품의 현재 정보는 다음과 같습니다.\n");
			print_record(data);
			printf("====================\n");
			while (user_status == 0)
			{
				printf("삭제를 진행하겠습니까?(예:1, 아니오:2): ");
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
					printf("올바른 명령어를 다시 입력해 주세요!\n");
			}
		}
		else
			fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 1)
		printf("성공적으로 삭제되었습니다.\n");
	else if (status == 2)
		printf("삭제가 취소되었습니다.\n");
	else
		printf("삭제할 데이터를 찾을 수 없습니다. %s은(는) 저장되어 있지 않습니다.\n", name);

	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "삭제 처리 후 파일 items.dat를 열 수 없습니다");
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
		fprintf(stderr, "파일 temp.dat를 열 수 없습니다");
		exit(1);
	}
	getchar();
	printf("수정할 상품번호를 입력해 주세요.: ");
	gets_s(name, SIZE);

	fseek(fp, 0, SEEK_SET);

	while (fread(&data, sizeof(data), 1, fp) == 1) {
		if (strcmp(data.number, name) == 0)
		{
			if (user_status == 0)
			{
				printf("====================\n");
				printf("개수를 수정할 상품의 현재 정보는 다음과 같습니다.\n");
				print_record(data);
				printf("====================\n");
				while (user_status == 0)
				{
					printf("수정을 진행하겠습니까?(예:1, 아니오:2): ");
					scanf("%d", &user);
					getchar();
					if (user == 1 || user == 2)
					{
						user_status = 1;
						break;
					}
					else
						printf("올바른 명령어를 다시 입력해 주세요!\n");
				}
			}
			if (user == 1 && status == 0)
			{
				printf("수정하고자 하는 개수: ");
				scanf("%d", &data.count);
				status = 1;
			}
			else if (user == 2)
				status = 2;
		}
		fwrite(&data, sizeof(data), 1, fp1);
	}
	if (status == 1)
		printf("성공적으로 수정되었습니다.\n");
	else if (status == 2)
		printf("수정이 취소되었습니다.\n");
	else
		printf("수정할 데이터를 찾을 수 없습니다. %s은(는) 저장되어 있지 않습니다.\n", name);

	fclose(fp1);
	fclose(fp);

	remove("items.dat");
	rename("temp.dat", "items.dat");

	if ((fp = fopen("items.dat", "a+")) == NULL)
	{
		fprintf(stderr, "수정 처리 후 파일 items.dat를 열 수 없습니다");
		exit(1);
	}
}