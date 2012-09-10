#include"head.h"

bool LiteSQL::DataListLinkInsert(DataListNode *node, int i) {
	//数据表插入
	if(!node || i < 0)return false;

	DataListNode *p = DataList;

	while(p && i--) {
		p = p->next;
	}

	node->next = p->next;
	p->next = node;

	return FileWriteSystem() && FileWriteDataList(node);
}
bool LiteSQL::DataListSelectTable(const char *name, DataListNode **p) {
	if(!name || strlen(name) + 1 > 20)return false;

	*p = DataList->next;

	while(*p && strcmp((*p)->TableName, name)) {
		*p = (*p)->next;
	}

	return !(*p == NULL);
}

bool LiteSQL::DataLinkInsert(DataListNode *p, DataNode *node, int i) {
	if(!p || !node || i < 0)return false;

	DataNode *p2 = p->List;

	while(p2 && i--)p2 = p2->next;

	node->next = p2->next;
	p2->next = node;
	return true;
}

bool LiteSQL::DataListDelete(char *name, DataListNode **p) {
	* p = NULL;

	if(!DataListSelectTable(name, p))return false;

	DataListNode *tp = DataList;

	while(tp->next != *p && tp)tp = tp->next;

	if(tp->next != *p)return false;

	tp->next = (*p)->next;
	return true;
}

LiteSQL::DataNode *LiteSQL::DataLinkDelete(DataListNode *p, DataNode **node) {
	if(!p || !node || !*node)return NULL;

	DataNode   *pt = p->List;

	while(pt->next != *node && pt)pt = pt ->next;

	if(pt->next != *node)return NULL;

	pt->next = (*node)->next;
	return pt;
}