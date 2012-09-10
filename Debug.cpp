#include"head.h"
void Debug(LiteSQL *LS) {
	int chose;
	char tablename[20];
	LS->FileSetPath("D:\\database\\");//�������ݿ��ļ�·��
	printf("��ǰ�����ļ�·��Ϊ: D:\\database\\\n");

	while((chose = DebugMenu())) {
		switch(chose) {
			case 1:
				DebugCreat(LS);
				break;
			case 2:
				DebugRead(LS);
				break;
		}

		printf("\n����ڴ����ݣ��������˵�..\n");
		LS->FileReadingStatusChange();//�ı��ļ���д״̬

		while(LS->SelectListName(tablename))LS->DeleteTable(tablename);

		printf("��������գ����������ļ�");
		LS->FileReadingStatusChange();
	}
}
void DebugCreat(LiteSQL *LS) {
	printf("\n��ʼ��������:\n");
	char tablename[20];//����
	char FieldName[ListNameSize];//�ֶΣ�����
	int FieldSize = 0;
	char data[100];
	int n, i = 0, fn, j;
	printf("���봴���������:");
	scanf("%d", &n);

	if(n < 1)return;

	for(i = 0; i < n; ++i) {
		printf("������ %d,�������:", i + 1);
		scanf("%s", tablename);
		fflush(stdin);
		printf("��������ֶθ���:");
		scanf("%d", &fn);
		LS->CreatList(fn);

		for(j = 0; j < fn; j++) {
			printf("%d->�����ֶ���:", j + 1);
			scanf("%s", FieldName);
			fflush(stdin);
			LS->CreatListData(FieldName, LName, j);
			printf("%d->��������(char,long,double,long)(��ǰ����Ϊcharֻ������char)����!\n", j + 1);
			LS->CreatListData("char", LType, j);
			printf("%d->���볤��(��ǰ����char,��󳤶�100):", j + 1);
			scanf("%d", &FieldSize);
			fflush(stdin);

			if(FieldSize > 100)FieldSize = 100;

			LS->CreatListDataSize(LS->CheckType("char")*FieldSize, j);
		}

		printf("\n�����Ϣ:\n");
		LS->CreatListDebugOut();
		LS->CreatListAddList(tablename);
		printf("->%s �����ɹ�!\n", tablename);
	}

	printf("\n��������:");

	while(1) {
		printf("\n����������ݵı�(Ϊ�ս���):");
		gets(tablename);
		fflush(stdin);

		if(tablename[0] == 0) {
			printf("�� ��������.\n");
			break;
		}

		i = 0;

		while(1) {
			if(!LS->CreatData(tablename)) {
				printf("ѡ������,������\n");
				return;
			}

			while(1) {
				printf("����������ݵ��ֶ���(Ϊ�ս�������):");
				gets(FieldName);
				fflush(stdin);

				if(FieldName[0] == 0)break;

				printf("��������(��ǰ������Ϊchar):");
				gets(data);
				fflush(stdin);

				if(!LS->CreatDataInput(data, FieldName)) {
					printf("�������ݳ���!����!\n");
					return;
				}
			}

			i++;
			printf("��������?[Enter]");
			gets(FieldName);
			fflush(stdin);

			if(FieldName[0] != 0)break;
		}

		printf("��ǰ���� %d ������!\n", i);
	}

	printf("->��ɲ��룡\n");
	printf("\n�������:\n");
	DebugSelect(LS);
}
void DebugRead(LiteSQL *LS) {
	int i;
	char tablename[20];
	printf("\n��ʼ��ȡ����:\n");

	if(!LS->Read()) {
		printf("->��ȡ����ʧ�ܣ��ļ������ڻ����ļ����ݴ���\n");
		return;
	} else printf("->�ļ���ȡ�ɹ�.��������......\n");

	printf("\n���Ի�ȡ���ݱ��б�:\n");
	i = 0;

	while(LS->SelectListName(tablename)) {
		printf("%d -> %s\n", ++i, tablename);
		printf("����Ϣ:\n");
		LS->CreatListDebugOut(tablename);
	}

	if(i == 0) {
		printf("->��ȡ���ݱ�ʧ��\n");
		return;
	} else printf("->�ɹ���ȡ %d �����ݱ�.��������......\n", i);

	printf("\n�������������:\n");
	DebugSelect(LS);
}
void DebugSelect(LiteSQL *LS) {
	char tablename[20];
	char fn[ListNameSize];
	char *Field;//�ֶ���
	int x = 0, i, j;

	while(1) {
		printf("�� �������(Ϊ�ս���):");
		gets(tablename);
		fflush(stdin);

		if(tablename[0] == 0) {
			printf("�� ��������.\n");
			break;
		}

		if(!LS->SelectTable(tablename)) {
			printf("�� ��ѡ��ʧ��,����������:");
			continue;
		}

		printf("�� ������Ҫ���ֵ�ĸ���(���%d):", LS->SelectListFieldNumber());

		while(1) {
			scanf("%d", &x);
			fflush(stdin);

			if(x < 1 || x > LS->SelectListFieldNumber()) {
				printf("�� ��������,��������:");
			} else break;
		}

		Field = (char *)malloc(x * sizeof(char) * ListNameSize);

		for(i = 0; i < x; i++) {
			printf("�� ���� %d �ֶ���:", i + 1);
			scanf("%s", fn);
			fflush(stdin);
			strncpy(Field + i * ListNameSize, fn, ListNameSize);
		}

		printf("�� �������:\n");

		if(LS->Select()) {
			i = 0;

			do {
				printf("%d:-> ", i);

				for(j = 0; j < x; j++) {
					DebugOutData(LS, Field + j * ListNameSize);
				}

				printf("\n");
			} while(LS->SelectNext());

		} else printf("->����Ϊ�գ�\n");

		free(Field);
		printf("�� ��%s�����������\n", tablename);
	}
}
void DebugOutData(LiteSQL *LS, const char *name) { //�������
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
	//���˵�
	int a = -1, i = 0;
	printf("\n������������������������������������\n");
	printf("��     ѡ���������   ��0 �˳�)   ��\n");
	printf("������������������������������������\n");
	printf("��   1  ��������                  ��\n");
	printf("��   2  ��ȡ����                  ��\n");
	printf("������������������������������������\n");
	printf("������ѡ��:");

	do {
		if(a != -1)printf("�� ������������������:");

		scanf("%d", &a);

		if(i++ > 10) {
			printf("��������������,���Խ���!");
			return 0;
		}

		fflush(stdin);
	} while(a < 0 || a > 2);

	return a;
}