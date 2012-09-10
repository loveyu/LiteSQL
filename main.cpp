#include"head.h"

int main() {
	//最后主文件
	LiteSQL *LS = new LiteSQL();
	Debug(LS);
	return 0;
}

int main1() {
	//文件操作调试主文件
	LiteSQL *LS = new LiteSQL();
	LS->FileSetPath("D:\\database\\");

	if(!LS->Read())printf("DataBase File Read Error.\n");

	LS->SelectTable("table1");
	printf("-------Result table1:\nno if:\n");

	if(LS->Select())
		do {
			printf("select-> id:%d, name:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"));
		} while(LS->SelectNext());

	LS->SelectTable("table2");
	printf("-------Result table2:\nno if:\n");

	if(LS->Select())
		do {
			printf("select-> id:%d, name:%s, value:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"), (char *)LS->SelectGet("value"));
		} while(LS->SelectNext());

	system("pause");
	return 0;
}
int main2() {
	//程序创建初期调试主文件
	LiteSQL *LS = new LiteSQL();
	LS->FileSetPath("D:\\database\\");
	int i = 0;

	printf("\n---------Creat table1 Debug:\n");
	//table1
	LS->CreatList(2);
	LS->CreatListData("id", LName, 0);
	LS->CreatListData("int", LType, 0);
	LS->CreatListDataSize(sizeof(int), 0);

	LS->CreatListData("name", LName, 1);
	LS->CreatListData("char", LType, 1);
	LS->CreatListDataSize(sizeof(char) * 20, 1);

	LS->CreatListDebugOut();
	LS->CreatListAddList("table1");

	LS->CreatData("table1");
	LS->CreatDataInput("zhangsan", "name");
	LS->CreatDataInput(&(++i), "id");

	LS->CreatData("table1");
	LS->CreatDataInput("lisi", "name");
	LS->CreatDataInput(&(++i), "id");
	LS->CreatData("table1");
	LS->CreatDataInput("wangwu", "name");
	LS->CreatDataInput(&(++i), "id");

	LS->SelectTable("table1");
	printf("-------Result table1:\nno if:\n");

	if(LS->Select())
		do {
			printf("select-> id:%d, name:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"));
		} while(LS->SelectNext());

	LS->SelectTable("table1");
	printf("\nif name = zhangsan:\n");

	if(LS->Select("zhangsan", "name"))
		do {
			printf("select-> id:%d, name:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"));
		} while(LS->SelectNext());

	printf("\n-------Creat table2 Debug:\n");
	//table2
	LS->CreatList(3);
	LS->CreatListData("id", LName, 0);
	LS->CreatListData("int", LType, 0);
	LS->CreatListDataSize(sizeof(int), 0);

	LS->CreatListData("name", LName, 1);
	LS->CreatListData("char", LType, 1);
	LS->CreatListDataSize(sizeof(char) * 20, 1);

	LS->CreatListData("value", LName, 2);
	LS->CreatListData("char", LType, 2);
	LS->CreatListDataSize(sizeof(char) * 40, 2);

	LS->CreatListDebugOut();
	LS->CreatListAddList("table2");

	LS->CreatData("table2");
	LS->CreatDataInput("xiaoli", "name");
	LS->CreatDataInput(&(++i), "id");
	LS->CreatDataInput("other1", "value");
	LS->CreatData("table2");
	LS->CreatDataInput("laowang", "name");
	LS->CreatDataInput(&(++i), "id");
	LS->CreatDataInput("other2", "value");
	LS->CreatData("table2");
	LS->CreatDataInput("pangzi", "name");
	LS->CreatDataInput(&(++i), "id");
	LS->CreatDataInput("other3", "value");


	LS->SelectTable("table2");
	printf("------Result table2:\nno if:\n");

	if(LS->Select())
		do {
			printf("select-> id:%d, name:%s, value:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"), (char *)LS->SelectGet("value"));
		} while(LS->SelectNext());

	LS->SelectTable("table2");
	printf("\nif name = pangzi:\n");

	if(LS->Select("pangzi", "name"))
		do {
			printf("select-> id:%d, name:%s, value:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"), (char *)LS->SelectGet("value"));

			if(LS->DeleteNowData())printf("delete one completed!\n");
		} while(LS->SelectNext());

	printf("\nDelete if, Result no if:\n");
	LS->SelectTable("table2");

	if(LS->Select())
		do {
			printf("select-> id:%d, name:%s, value:%s\n", *(int *)LS->SelectGet("id"), (char *)LS->SelectGet("name"), (char *)LS->SelectGet("value"));
		} while(LS->SelectNext());

	//delete table1
	/*
	printf("\n------Delete table1\n");
	LS->DeleteTable("table1");
	printf("Select table1: ");
	if(!LS->SelectTable("table1"))printf("table1 no find\n");

	if(LS->Select())
		do{
			printf("select-> id:%d, name:%s\n",*(int *)LS->SelectGet("id"),(char *)LS->SelectGet("name"));
		}while(LS->SelectNext());
	*/

	printf("Debug end!\n");
	system("pause");
	return 0;
}