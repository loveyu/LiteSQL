#include"head.h"
void Debug(LiteSQL *LS) {
	int chose;
	char tablename[20];
	LS->FileSetPath("D:\\database\\");//设置数据库文件路径
	printf("当前数据文件路径为: D:\\database\\\n");

	while((chose = DebugMenu())) {
		switch(chose) {
			case 1:
				DebugCreat(LS);
				break;
			case 2:
				DebugRead(LS);
				break;
		}

		printf("\n清空内存数据，返回主菜单..\n");
		LS->FileReadingStatusChange();//改变文件读写状态

		while(LS->SelectListName(tablename))LS->DeleteTable(tablename);

		printf("数据已清空，保留数据文件");
		LS->FileReadingStatusChange();
	}
}
void DebugCreat(LiteSQL *LS) {
	printf("\n开始创建测试:\n");
	char tablename[20];//表名
	char FieldName[ListNameSize];//字段，名字
	int FieldSize = 0;
	char data[100];
	int n, i = 0, fn, j;
	printf("输入创建表的数量:");
	scanf("%d", &n);

	if(n < 1)return;

	for(i = 0; i < n; ++i) {
		printf("创建表 %d,输入表名:", i + 1);
		scanf("%s", tablename);
		fflush(stdin);
		printf("输入表中字段个数:");
		scanf("%d", &fn);
		LS->CreatList(fn);

		for(j = 0; j < fn; j++) {
			printf("%d->输入字段名:", j + 1);
			scanf("%s", FieldName);
			fflush(stdin);
			LS->CreatListData(FieldName, LName, j);
			printf("%d->输入类型(char,long,double,long)(当前定义为char只能输入char)跳过!\n", j + 1);
			LS->CreatListData("char", LType, j);
			printf("%d->输入长度(当前定义char,最大长度100):", j + 1);
			scanf("%d", &FieldSize);
			fflush(stdin);

			if(FieldSize > 100)FieldSize = 100;

			LS->CreatListDataSize(LS->CheckType("char")*FieldSize, j);
		}

		printf("\n输出信息:\n");
		LS->CreatListDebugOut();
		LS->CreatListAddList(tablename);
		printf("->%s 创建成功!\n", tablename);
	}

	printf("\n创建数据:");

	while(1) {
		printf("\n输入插入数据的表(为空结束):");
		gets(tablename);
		fflush(stdin);

		if(tablename[0] == 0) {
			printf("└ 结束操作.\n");
			break;
		}

		i = 0;

		while(1) {
			if(!LS->CreatData(tablename)) {
				printf("选择表出错,结束！\n");
				return;
			}

			while(1) {
				printf("输入插入数据的字段名(为空结束输入):");
				gets(FieldName);
				fflush(stdin);

				if(FieldName[0] == 0)break;

				printf("输入数据(当前程序定义为char):");
				gets(data);
				fflush(stdin);

				if(!LS->CreatDataInput(data, FieldName)) {
					printf("插入数据出错!结束!\n");
					return;
				}
			}

			i++;
			printf("继续输入?[Enter]");
			gets(FieldName);
			fflush(stdin);

			if(FieldName[0] != 0)break;
		}

		printf("当前插入 %d 条数据!\n", i);
	}

	printf("->完成插入！\n");
	printf("\n数据输出:\n");
	DebugSelect(LS);
}
void DebugRead(LiteSQL *LS) {
	int i;
	char tablename[20];
	printf("\n开始读取测试:\n");

	if(!LS->Read()) {
		printf("->读取数据失败，文件不存在或者文件数据错误！\n");
		return;
	} else printf("->文件读取成功.继续操作......\n");

	printf("\n尝试获取数据表列表:\n");
	i = 0;

	while(LS->SelectListName(tablename)) {
		printf("%d -> %s\n", ++i, tablename);
		printf("表信息:\n");
		LS->CreatListDebugOut(tablename);
	}

	if(i == 0) {
		printf("->读取数据表失败\n");
		return;
	} else printf("->成功读取 %d 个数据表.继续操作......\n", i);

	printf("\n表数据输出测试:\n");
	DebugSelect(LS);
}
void DebugSelect(LiteSQL *LS) {
	char tablename[20];
	char fn[ListNameSize];
	char *Field;//字段名
	int x = 0, i, j;

	while(1) {
		printf("┌ 输入表名(为空结束):");
		gets(tablename);
		fflush(stdin);

		if(tablename[0] == 0) {
			printf("└ 结束操作.\n");
			break;
		}

		if(!LS->SelectTable(tablename)) {
			printf("├ 表选择失败,请重新输入:");
			continue;
		}

		printf("├ 输入你要输出值的个数(最多%d):", LS->SelectListFieldNumber());

		while(1) {
			scanf("%d", &x);
			fflush(stdin);

			if(x < 1 || x > LS->SelectListFieldNumber()) {
				printf("├ 输入有误,请重输入:");
			} else break;
		}

		Field = (char *)malloc(x * sizeof(char) * ListNameSize);

		for(i = 0; i < x; i++) {
			printf("├ 输入 %d 字段名:", i + 1);
			scanf("%s", fn);
			fflush(stdin);
			strncpy(Field + i * ListNameSize, fn, ListNameSize);
		}

		printf("├ 输出数据:\n");

		if(LS->Select()) {
			i = 0;

			do {
				printf("%d:-> ", i);

				for(j = 0; j < x; j++) {
					DebugOutData(LS, Field + j * ListNameSize);
				}

				printf("\n");
			} while(LS->SelectNext());

		} else printf("->数据为空！\n");

		free(Field);
		printf("└ 表%s数据输出结束\n", tablename);
	}
}
void DebugOutData(LiteSQL *LS, const char *name) { //输出数据
	switch(LS->GetSelectDataType(name)) {
		case INT:
			printf("%s:%d ", name, *(int *)LS->SelectGet(name));
			break;
		case DOUBLE:
			printf("%s:%lf ", name, *(double *)LS->SelectGet(name));
			break;
		case LONG:
			printf("%s:%d ", name, *(long *)LS->SelectGet(name));
			break;
		case FLOAT:
			printf("%s:%f ", name, *(float *)LS->SelectGet(name));
			break;
		case CHAR:
			printf("%s:%s ", name, (char *)LS->SelectGet(name));
			break;
		default:
			printf("ERROR ");
	}
}
int DebugMenu() {
	//主菜单
	int a = -1, i = 0;
	printf("\n┌────────────────┐\n");
	printf("│     选择调试类型   （0 退出)   │\n");
	printf("├────────────────┤\n");
	printf("│   1  创建调试                  │\n");
	printf("│   2  读取调试                  │\n");
	printf("├────────────────┘\n");
	printf("├输入选择:");

	do {
		if(a != -1)printf("├ 输入有误，请重新输入:");

		scanf("%d", &a);

		if(i++ > 10) {
			printf("输出错误次数过多,测试结束!");
			return 0;
		}

		fflush(stdin);
	} while(a < 0 || a > 2);

	return a;
}