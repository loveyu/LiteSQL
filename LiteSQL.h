#include<iostream>
#include<string>
#include<io.h>
#include<direct.h>

enum LiteSQLListName { //���ݱ�ṹ����
	LName,
	LSize,
	LType
};
enum DATATYPE {//��������
	CHAR,
	INT,
	LONG,
	DOUBLE,
	FLOAT,
	OTHER
};

//�б����ֶ����Ƶĳ���
#define ListNameSize 50
#define ListTypeSize 20

class LiteSQL {

	struct SQLList {
		char **name;
		int *size;
		char **type;
		int length;
	};
	struct DataNode {
		char *data;
		DataNode *next;
	};
	struct DataListNode {
		//�洢���ݱ�
		char TableName[20];
		SQLList *DataName;
		DataNode *List;
		DataListNode *next;
	};
	struct NowStatus {
		DataListNode *Datalist;
		DataNode *data;
	};
	struct NowSelect {
		DataListNode *Datalist;
		DataNode *data;
		char *WhereData;
		char *WhereName;
	};

private:
	SQLList *s;//����������ʱ����ʱ����
	DataListNode *DataList;
	DataListNode *SelectTableStatus; //��ȡ������ʱ״̬
	NowStatus status;//����״̬
	NowSelect SelectStatus;//ѡ��״̬
	char DataFileDir[256];//�����ļ�·��
	bool ReadingFile;

	int  GetNameSize(SQLList *s, char *name); //��ȡ���������ֶεĳ���
	void *GetNameType(SQLList *s, char *name);
	int ListSizeCount(SQLList *s);

	//�ڴ����
	bool MemoryCopy(void *dst, const void *src, int count);
	void *MemoryAlloc(SQLList *s);
	void *MemoryCount(SQLList *s, DataNode *data, const char *name);//����name�ĵ�ַ
	bool FreeList(SQLList *p);//�ͷ��б�����
	bool FreeDataList(DataListNode *p);//�ͷŽ������
	bool FreeData(DataNode *p);//�ͷ�Data����

	//�����б��������
	bool DataListLinkInsert(DataListNode *node, int i); //����node����i������֮��
	bool DataListSelectTable(const char *name, DataListNode **p); //�������б��в��ұ�
	bool DataLinkInsert(DataListNode *p, DataNode *node, int i); //����node��p�ĵ�i������֮��
	bool DataListDelete(char *name, DataListNode **p); //ɾ��name�ı���ֵ����
	DataNode *DataLinkDelete(DataListNode *p, DataNode **node);  //ɾ��node�ڵ㣬������һ�����

	//�����б�ѡ������ֵ����
	bool SelectWhereClean();//���
	bool SelectWhereMake(const void *data, char *name); //����
	int  SelectWhereCompare(DataNode *p);

	//�ļ���������
	bool FileOpenSystem();
	bool FileOpenDataList(const char *name);
	bool FileOpenData(const char *name);
	bool FileWriteSystem();
	bool FileWriteDataList(DataListNode *data);
	bool FileWriteData(DataListNode *data);
	bool FilePathGet(char *path, const char *filename);
	bool FileExists(const char *path);//�ж��ļ����ļ����Ƿ����
	bool FileMakeDir(const char *path);
	bool FileDeleteTable(const char *tablename);//ɾ�������ļ����µı��ļ�
	long FileCountSize(const char *pathname);//�����ļ���С

public:
	//���췽��
	LiteSQL();

	DATATYPE GetSelectDataType(const char *name);//��ȡ��ǰ���������������
	int  CheckType(char *type);

	//�����б��������
	bool CreatList(int number);
	bool CreatListData(const void *data, LiteSQLListName type, int id);
	bool CreatListDataSize(int size, int id);//ֱ�Ӳ��볤��

	bool CreatListAddList(const char name[20]);//��������������ɱ�
	void CreatListDebugOut();
	void CreatListDebugOut(const char *tablename);//�������ֶ���Ϣ
	bool CreatData(const char *tablename);
	bool CreatDataInput(const void *data, char *name);//��������
	bool UpdateDtaInput(const void *data, char *name);//���µ�ǰѡ��Ľ������

	//��ȡ�б��������
	bool SelectTable(char *tablename);
	bool Select();//��ȡ����
	bool Select(const void *data, char *name); //����ѡ������
	bool SelectNext();
	void *SelectGet(const char *name);
	bool SelectListName(char tablename[20]);
	int SelectListFieldNumber();//����ѡ����ֶεĳ���

	//ɾ�������
	bool DeleteTable(char *tablename);
	bool DeleteNowData();

	//�ļ�����
	bool FileSetPath(char *path);
	bool Read();//��ȡ����
	void FileReadingStatusChange();//�ı��Ƿ�д���״̬
};