#pragma once
#include "STD.h"



struct MetaObjectInfo;
namespace
{
	std::map<std::string, MetaObjectInfo*> s_meta_info;
}


struct MetaObjectInfo
{
	using FuncType = std::function<void(void* obj, void* arg, void* ret)>;
	using VoidFuncType = std::function<void(void* obj, void* arg)>;
	using FuncVoidType = std::function<void(void* obj, void* ret)>;
	using VoidFuncVoidType = std::function<void(void* obj)>;
	std::string class_name;
	std::string base_class_name;

	std::map<std::string, FuncType> funcs;
	std::map<std::string, VoidFuncType> void_funcs;
	std::map<std::string, FuncVoidType> func_voids;
	std::map<std::string, VoidFuncVoidType> void_func_voids;

	template<typename ReturnType, typename ArgType>
	ReturnType CallFunc(void* obj, std::string funcName, ArgType arg) {
		Assert(funcs.find(funcName) != funcs.end(), "没有注册函数:" + funcName);
		ReturnType ret;
		funcs[funcName](obj, &arg, &ret);
		return ret;
	}

	template<typename ArgType>
	void CallFunc(void* obj, std::string funcName, ArgType arg) {
		Assert(void_funcs.find(funcName) != void_funcs.end(), "没有注册函数:" + funcName);
		void_funcs[funcName](obj, &arg);
	}

	template<typename ReturnType>
	ReturnType CallFunc(void* obj, std::string funcName) {
		Assert(func_voids.find(funcName) != func_voids.end(), "没有注册函数:" + funcName);
		ReturnType ret;
		func_voids[funcName](obj, &ret);
		return ret;
	}

	void CallFunc(void* obj, std::string funcName) {
		Assert(void_func_voids.find(funcName) != void_func_voids.end(), "没有注册函数:" + funcName);
		void_func_voids[funcName](obj);
	}
};

#define META_OBJECT(DeriveClassType, BaseClassType)	\
public:	\
	using ClassType = DeriveClassType;	\
	static MetaObjectInfo* GetMetaObjectInfo() { static MetaObjectInfo info{ #DeriveClassType, #BaseClassType }; return &info; }	\
	virtual const std::string& GetRealClassName() const { static const std::string real_class_name = #DeriveClassType; return real_class_name;}	\
	DeriveClassType() {	\
		static bool meta_inited = false; \
		if (!meta_inited) {	\
			s_meta_info[GetRealClassName()] = GetMetaObjectInfo();	\
			RegisterAllMethod();	\
			meta_inited = true;	\
		}	\
	}	\
private:	\
	static void RegisterAllMethod()


// 声明注册函数用
#define GEN_REGISTER_FUNC_NAME(FuncName)	__RegisterMetaFunction ## FuncName
#define REGISTER_FUNC(FuncName) GEN_REGISTER_FUNC_NAME(FuncName)()



// 有返回值，有参数的函数
#define META_FUNC_EX(Access, FuncName, ReturnType, ArgType, ArgName)	\
private:	\
static void GEN_REGISTER_FUNC_NAME(FuncName)(){	\
	auto f = [](void* vobj, void* arg, void* ret)	{ *static_cast<ReturnType*>(ret) = static_cast<ClassType*>(vobj)->FuncName(*static_cast<ArgType*>(arg));};	\
	GetMetaObjectInfo()->funcs[#FuncName] = f;	\
}	\
Access:	\
	ReturnType FuncName(ArgType ArgName)

#define META_FUNC(FuncName, ReturnType, ArgType, ArgName)	META_FUNC_EX(public, FuncName, ReturnType, ArgType, ArgName)

#define META_FUNC_P(FuncName, ReturnType, ArgTypeBody) \
struct FuncName ## Args ArgTypeBody;	\
META_FUNC(FuncName, ReturnType, FuncName ## Args, args)


// 无返回值，有参数的函数
#define META_VOID_FUNC_EX(Access, FuncName, ArgType, ArgName)	\
private:	\
static void GEN_REGISTER_FUNC_NAME(FuncName)(){	\
	auto f = [](void* vobj, void* arg)	{ static_cast<ClassType*>(vobj)->FuncName(*static_cast<ArgType*>(arg));};	\
	GetMetaObjectInfo()->void_funcs[#FuncName] = f;	\
}	\
Access:	\
	void FuncName(ArgType ArgName)

#define META_VOID_FUNC(FuncName, ArgType, ArgName) META_VOID_FUNC_EX(public, FuncName, ArgType, ArgName)
#define META_VOID_FUNC_P(FuncName, ArgTypeBody)	\
struct FuncName ## Args ArgTypeBody;	\
META_VOID_FUNC(FuncName, FuncName ## Args, args)

// 有返回值，无参数的函数
#define META_FUNC_VOID_EX(Access, FuncName, ReturnType)	\
private:	\
static void GEN_REGISTER_FUNC_NAME(FuncName)(){	\
	auto f = [](void* vobj, void* ret)	{ *static_cast<ReturnType*>(ret) = static_cast<ClassType*>(vobj)->FuncName();};	\
	GetMetaObjectInfo()->func_voids[#FuncName] = f;	\
}	\
Access:	\
	ReturnType FuncName()

#define META_FUNC_VOID(FuncName, ReturnType)	META_FUNC_VOID_EX(public, FuncName, ReturnType)

// 无返回值，无参数的函数
#define META_VOID_FUNC_VOID_EX(Access, FuncName)	\
private:	\
static void GEN_REGISTER_FUNC_NAME(FuncName)(){	\
	auto f = [](void* vobj)	{ static_cast<ClassType*>(vobj)->FuncName();};	\
	GetMetaObjectInfo()->void_func_voids[#FuncName] = f;	\
}	\
Access:	\
	void FuncName()
	
#define META_VOID_FUNC_VOID(FuncName) META_VOID_FUNC_VOID_EX(public, FuncName)

	

template<typename D, typename B>
bool IsBaseClassOf(const D& d, const B& b)
{
	auto it = s_meta_info.find(d.GetRealClassName());
	auto base_class_name = b.GetRealClassName();
	while (it != s_meta_info.end())
	{
		auto meta_info = it->second;
		if (meta_info->base_class_name == base_class_name)
		{
			return true;
		}
		it = s_meta_info.find(meta_info->base_class_name);
	}
	return false;
}