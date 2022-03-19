// constructing unordered_sets
#include <iostream>
#include <string>
#include <unordered_set>

//用哈希函数组织的set 无序
template<class T>
T cmerge(T a, T b) { T t(a); t.insert(b.begin(), b.end()); return t; }

int mainUS2222()
{
	std::unordered_set<std::string> first;                                // empty
	std::unordered_set<std::string> second({ "red","green","blue" });    // init list
	std::unordered_set<std::string> third({ "orange","pink","yellow" }); // init list
	std::unordered_set<std::string> fourth(second);                    // copy
	std::unordered_set<std::string> fifth(cmerge(third, fourth));       // move
	std::unordered_set<std::string> sixth(fifth.begin(), fifth.end()); // range

	std::cout << "sixth contains:";
	for (const std::string& x : sixth) std::cout << " " << x;
	std::cout << std::endl;

	return 0;
}

int mainUS341312()
{
	std::unordered_set<std::string> myset;

	bool case_insensitive = myset.key_eq()("checking", "CHECKING");

	std::cout << "myset.key_eq() is ";
	std::cout << (case_insensitive ? "case insensitive" : "case sensitive");
	std::cout << std::endl;

	return 0;
}

int mainus3333()
{
	std::unordered_set<std::string> myset = { "hat", "umbrella", "suit" };

	for (auto& x : { "hat","sunglasses","suit","t-shirt" })
	{
		if (myset.count(x)>0)
			std::cout << "myset has " << x << std::endl;
		else
			std::cout << "myset has no " << x << std::endl;
	}

	return 0;
}

typedef std::unordered_set<std::string> stringset;

int main()
{
	stringset myset;

	stringset::hasher fn = myset.hash_function();

	std::cout << "that: " << fn("that") << std::endl;
	std::cout << "than: " << fn("than") << std::endl;

	return 0;
}