#include"head.h"
bool LiteSQL::DeleteTable(char *tablename) {
	DataListNode *p = NULL;

	if(!DataListDelete(tablename, &p))return false;

	if(!FreeDataList(p))return false;

	FileDeleteTable(tablename);
	SelectTableStatus = DataList;//÷ÿ÷√—°‘Ò±Ì
	return FileWriteSystem();
}
bool LiteSQL::DeleteNowData() {
	if(!SelectStatus.data || !SelectStatus.Datalist)return false;

	DataNode *p;
	p = DataLinkDelete(SelectStatus.Datalist, &SelectStatus.data);

	if(!p)return false;

	if(!FreeData(SelectStatus.data))return false;

	SelectStatus.data = p;
	return FileWriteData(SelectStatus.Datalist);
}