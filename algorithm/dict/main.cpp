#include"HashSet.h"
#include"SkipTable.h"
using namespace stb;

void cb(skip_list_node* node)
{
	printf("node key %d  valud = %d \n", node->key, node->value);
}
int main()
{
	HashSet<int> set;
	set.Insert(9);
	set.Insert(2);
	set.Insert(3);
	set.Insert(5);
	set.Insert(888);
	set.print();

	skip_list* sl = skip_list_creat(32);
	skip_list_insert(sl, 9, 5);
	skip_list_insert(sl, 5, 2);
	skip_list_insert(sl, 3, 9);
	//skip_list_loop(sl, cb);
	skip_list_destroy(sl);
	return 0;
}