#include<iostream>
#include<string>
#include<io.h>
#include<direct.h>

enum LiteSQLListName { //数据表结构类型
	LName,
	LSize,
	LType
};
enum DATATYPE {//数据类型
	CHAR,
	INT,
	LONG,
	DOUBLE,
	FLOAT,
	OTHER
};

//列表中字段名称的长度
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
		//存储数据表
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
	SQLList *s;//创建表属性时的临时数据
	DataListNode *DataList;
	DataListNode *SelectTableStatus; //获取表数据时状态
	NowStatus status;//创建状态
	NowSelect SelectStatus;//选择状态
	char DataFileDir[256];//数据文件路径
	bool ReadingFile;

	int  GetNameSize(SQLList *s, char *name); //获取表中设置字段的长度
	void *GetNameType(SQLList *s, char *name);
	int ListSizeCount(SQLList *s);

	//内存操作
	bool MemoryCopy(void *dst, const void *src, int count);
	void *MemoryAlloc(SQLList *s);
	void *MemoryCount(SQLList *s, DataNode *data, const char *name);//返回name的地址
	bool FreeList(SQLList *p);//释放列表数据
	bool FreeDataList(DataListNode *p);//释放结点数据
	bool FreeData(DataNode *p);//释放Data数据

	//数据列表链表操作
	bool DataListLinkInsert(DataListNode *node, int i); //插入node到第i个数据之后
	bool DataListSelectTable(const char *name, DataListNode **p); //在数据列表中查找表
	bool DataLinkInsert(DataListNode *p, DataNode *node, int i); //插入node到p的第i个数据之后
	bool DataListDelete(char *name, DataListNode **p); //删除name的表并将值返回
	DataNode *DataLinkDelete(DataListNode *p, DataNode **node);  //删除node节点，返回上一个结点

	//数据列表选择条件值操作
	bool SelectWhereClean();//清空
	bool SelectWhereMake(const void *data, char *name); //创建
	int  SelectWhereCompare(DataNode *p);

	//文件操作方法
	bool FileOpenSystem();
	bool FileOpenDataList(const char *name);
	bool FileOpenData(const char *name);
	bool FileWriteSystem();
	bool FileWriteDataList(DataListNode *data);
	bool FileWriteData(DataListNode *data);
	bool FilePathGet(char *path, const char *filename);
	bool FileExists(const char *path);//判断文件或文件夹是否存在
	bool FileMakeDir(const char *path);
	bool FileDeleteTable(const char *tablename);//删除数据文件加下的表文件
	long FileCountSize(const char *pathname);//计算文件大小

public:
	//构造方法
	LiteSQL();

	DATATYPE GetSelectDataType(const char *name);//获取当前操作表的数据类型
	int  CheckType(char *type);

	//创建列表操作方法
	bool CreatList(int number);
	bool CreatListData(const void *data, LiteSQLListName type, int id);
	bool CreatListDataSize(int size, int id);//直接插入长度

	bool CreatListAddList(const char name[20]);//插入表名，并生成表
	void CreatListDebugOut();
	void CreatListDebugOut(const char *tablename);//输出表的字段信息
	bool CreatData(const char *tablename);
	bool CreatDataInput(const void *data, char *name);//创建数据
	bool UpdateDtaInput(const void *data, char *name);//更新当前选择的结点数据

	//获取列表操作方法
	bool SelectTable(char *tablename);
	bool Select();//获取数据
	bool Select(const void *data, char *name); //条件选择数据
	bool SelectNext();
	void *SelectGet(const char *name);
	bool SelectListName(char tablename[20]);
	int SelectListFieldNumber();//返回选择表字段的长度

	//删除表操作
	bool DeleteTable(char *tablename);
	bool DeleteNowData();

	//文件操作
	bool FileSetPath(char *path);
	bool Read();//读取数据
	void FileReadingStatusChange();//改变是否写入的状态
};