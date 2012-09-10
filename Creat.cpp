#include"head.h"
bool LiteSQL::CreatList(int number) {
	if(number < 1) return false;

	//初始化新的列表数据
	s = (SQLList *)malloc(sizeof(SQLList));

	if(!s)return false;

	s->length = number;
	s->size = (int *)malloc(sizeof(int) * number);

	if(!s->size) return false;

	s->name = (char **)malloc(sizeof(char *) * number);

	if(!s->name) return false;

	s->type = (char **)calloc(number, sizeof(char *));

	if(!s->type) return false;

	for(int i = 0; i < number; i++) {
		*(s->name + i) = (char *)malloc(sizeof(char) * ListNameSize);

		if(!*(s->name + i)) return false;

		*(s->type + i) = (char *)malloc(sizeof(char) * ListTypeSize);

		if(!*(s->type + i)) return false;
	}

	return true;
}

bool LiteSQL::CreatListData(const void *data, LiteSQLListName type, int id) {
	if(data == NULL || id < 0 || id >= s->length)return false;

	switch(type) {
		case LName:
			return MemoryCopy(*(s->name + id), data, (strlen((char *)data) + 1) * sizeof(char));
			break;
		case LSize:
			*(s->size + id) = *(int *)data;
			break;
		case LType:
			return MemoryCopy(*(s->type + id), data, (strlen((char *)data) + 1) * sizeof(char));
			break;
	}

	return true;
}
bool LiteSQL::CreatListDataSize(int size, int id) {
	if(id >= s->length || size < sizeof(char))return false;

	*(s->size + id) = size;
	return true;
}

void LiteSQL::CreatListDebugOut() {
	using namespace std;

	for(int i = 0; i < s->length; i++) {
		if(*(s->size + i) > 0)cout << "field:" << *(s->name + i) << "  size:" << *(s->size + i) << "  type:" << *(s->type + i) << "\n";
	}

	cout << endl;
}
void LiteSQL::CreatListDebugOut(const char *tablename) {
	using namespace std;
	SQLList *sl;
	DataListNode *p;
	DataListSelectTable(tablename, &p);
	sl = p->DataName;

	for(int i = 0; i < sl->length; i++) {
		if(*(sl->size + i) > 0)cout << "field:" << *(sl->name + i) << "  size:" << *(sl->size + i) << "  type:" << *(sl->type + i) << "\n";
	}

	cout << endl;
}
bool LiteSQL::CreatListAddList(const char name[20]) {
	DataListNode *NewNode;

	if(strlen(name) + 1 > 20)return false;

	//生成新的数据表结点
	NewNode = (DataListNode *)malloc(sizeof(DataListNode));

	if(!NewNode)return false;

	strncpy(NewNode->TableName, name, 20); //复制名称
	//插入名称表及数据表
	NewNode->DataName = s;
	NewNode->next = NULL;

	NewNode->List = (DataNode *)malloc(sizeof(DataNode));

	if(!NewNode->List)return false;

	NewNode->List->data = NULL;
	NewNode->List->next = NULL;
	return DataListLinkInsert(NewNode, 0);
}
bool LiteSQL::CreatData(const char *tablename) {
	DataListNode *p = NULL;

	if(!DataListSelectTable(tablename, &p))return false;

	DataNode *node;
	node = (DataNode *)malloc(sizeof(DataNode));

	if(!node)return false;

	char *data;
	data = (char *)MemoryAlloc(p->DataName);

	if(!data)return false;

	node->data = data;
	node->next = NULL;
	//设置状态
	status.Datalist = p;
	status.data = node;

	return DataLinkInsert(p, node, 0);
}
bool LiteSQL::CreatDataInput(const void *data, char *name) {
	if(!data || !name)return false;

	char *p;
	p = (char *)MemoryCount(status.Datalist->DataName, status.data, name);

	return MemoryCopy(p, data, GetNameSize(status.Datalist->DataName, name)) && FileWriteData(status.Datalist);
}
bool LiteSQL::UpdateDtaInput(const void *data, char *name) {
	//更新当前获取列表时选中的数据
	if(!data || !name)return false;

	char *p;
	p = (char *)MemoryCount(SelectStatus.Datalist->DataName, SelectStatus.data, name);

	return MemoryCopy(p, data, GetNameSize(SelectStatus.Datalist->DataName, name)) && FileWriteData(SelectStatus.Datalist);
}