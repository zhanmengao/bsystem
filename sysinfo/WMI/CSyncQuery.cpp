#include"CSyncQuery.h"

#define CHECKWMIHR(hr) assert(hr)

/*
Excute是继承于CWMI基类中的纯虚函数。
在CSynQuery实现的Excute中，将执行一次半同步查询，并枚举返回的结果。
*/
HRESULT CSynQuery::Excute(CComPtr<IWbemServices> pSvc)
{
	HRESULT hr = WBEM_S_FALSE;

	do {
		CComPtr<IEnumWbemClassObject> pEnumerator = NULL;
		hr = pSvc->ExecQuery(
			CComBSTR("WQL"),
			CComBSTR(m_wszWQLQuery.c_str()),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumerator);

		CHECKWMIHR(hr);

		ULONG uReturn = 0;

		while (pEnumerator) 
		{
			CComPtr<IWbemClassObject> pclsObj = NULL;
			HRESULT hr = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

			if (0 == uReturn) 
			{
				break;
			}
			DealWithIWbemClassObject(pclsObj);
		}

	} while (0);

	return hr;
}
/*
首先我们先看一下用于（半）同步查询的函数ExecQuery的声明

  HRESULT ExecQuery(
  [in]   const BSTR strQueryLanguage,
  [in]   const BSTR strQuery,
  [in]   LONG lFlags,
  [in]   IWbemContext *pCtx,
  [out]  IEnumWbemClassObject **ppEnum
  );
strQueryLanguage是用于标记查询的语言种类。
由此参数可以发现，当初微软设计这个接口时是希望未来它支持更多的查询语言。
由于目前只支持WQL语言，所以该参数只能传“WQL”。
第二个参数strQuery是实际用于查询的命令，比如“Select * From XXX"，是不是看着很像SQL？
第三个参数lFlags是一个非常重要的参数，如果仔细看该文标题，可以发现，
我用的是”半同步“而不是“同步”，该参数就控制着该函数到底是“同步”还是“半同步”。
如果该参数包含WBEM_FLAG_RETURN_IMMEDIATELY，则说明该调用是个半同步调用，否则是同步调用。

现在我说一下WMI中同步和半同步两者的区别。
同步这个过程不难理解，如果我们同步调用一个函数，该函数会经过计算后将返回结果准备好，
然后返回到调用处。如果该过程非常消耗时间，且返回的结果非常占用空间，比如返回10240个对象，
是不是觉得这个调用过程非常笨重？是的！那么解决这个问题的很好的方法便诞生了：半同步。
半同步的调用方式非常类似于异步（以后介绍）调用。

当我们调用一个半同步操作后，函数内部会启动线程去执行查询工作，之后会立即返回到我们的调用处。
当半同步内部线程查询到并封装完一个对象后，便会通知我们外面枚举结果的函数，
告诉我们：一个结果准备好了，你可以使用了。这个相当于将合并结果集的过程去掉。
考虑到调用半同步的逻辑处理一个返回对象可能需要一定的时间，
在半同步启动的线程中可以利用这段时间完成下一个对象的查找和封装。
所以总体来说半同步对时间的消耗是比同步好的。
而从占用资源的角度看，半同步不用一次返回那么多个结果，所以占用的资源会比同步方式好很多。
所以大部分情况下，只在同步和半同步中做出选择的情况下，优先考虑使用半同步。

回到ExecQuery这个函数，lFlags还有个非常重要的可选值是WBEM_FLAG_FORWARD_ONLY。该参数让ExecQuery函数返回的枚举是个Forward-Only的。这样做的好处是可以让我们程序更快且占用更少的资源。所以lFlags一般是WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY。
pCtx一般设置为NULL。ppEnum是个返回结果的枚举器。
*/

/*
我们可以如此枚举返回的结果集。关注一下Next第一参数。
在之前对半同步的介绍中，我们说道：半同步函数启动的线程需要查询和封装下一个结果，
其实这个可以看成是个异步操作，于是枚举结果这边需要等待那个实际查询的线程完成一个结果。
我给此参数设置为“一直等待”。一般来说，这个结果是立即返回的。
pclsObj保存的是一个结果集中的一个结果。我们使用DealWithIWbemClassObject函数处理每个结果。
*/
HRESULT CSynQuery::DealWithIWbemClassObject(CComPtr<IWbemClassObject> pclsObj)
{
	HRESULT hr = WBEM_S_NO_ERROR;
	do 
	{
		CComVariant vtClass;

		hr = pclsObj->Get(L"__CLASS", 0, &vtClass, NULL, NULL);
		CHECKWMIHR(hr);
		if (VT_BSTR == vtClass.vt)
		{
			wprintf(L"\n%s\n", vtClass.bstrVal);
		}

		hr = pclsObj->BeginEnumeration(WBEM_FLAG_LOCAL_ONLY);

		do 
		{
			CComBSTR bstrName;
			CComVariant Value;
			CIMTYPE type;
			LONG lFlavor = 0;
			hr = pclsObj->Next(0, &bstrName, &Value, &type, &lFlavor);
			CHECKWMIHR(hr);
			hr = DealWithSingleItem(bstrName, Value, type, lFlavor);
		} while (WBEM_S_NO_ERROR == hr);

		hr = pclsObj->EndEnumeration();
	} while (0);

	return hr;
}

/*
该函数首先会打印出该结果属于哪个WMI类，然后会枚举结果中的每个成员。
其实在实际使用中大可不必如此，我如此设计只是为了该例子可以在不同平台上，针对不同需求都可以正确执行。
需要注意的一点是在枚举前，我们需要制定要枚举什么。
于是我们要调用BeginEnumeration，并传WBEM_FLAG_LOCAL_ONLY，即枚举该类自己的成员。
在枚举完之后，一般要调用EndEnumeration。
其实这个不是必须的，只有在要提前退出枚举的场景下才必须调用EndEnumeration。
针对每个成员，我们又要使用虚函数DealWithSingleItem来处理。
于是处理逻辑又被我封装到一个继承于CSynQuery的类CSynQueryData中。
该类逻辑非常简单，如果成员是非对象，则直接打印出来；
如果结果是一个对象，则再解析这个对象，并将其中非对象打印出来。
*/
