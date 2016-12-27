//
// Copyright (c) zww0815@foxmail.com.  All rights reserved.
//
//
#pragma once

namespace LeetCodeTest
{
    class TestBase
    {
    public:
        virtual wchar_t* Name() const { return nullptr; };
        virtual wchar_t* Desc() const { return nullptr; };
        virtual BOOL Run() = 0;
    };

    typedef std::shared_ptr<TestBase> SpTestBaseType;
    typedef std::vector<SpTestBaseType> VectorTestsType;

    class Test
    {
    public:
        static Test& GetInstance();

    public:
        template<class T>
        static std::shared_ptr<T> CreateInstance()
        {
            return std::make_shared<T>();
        }

        void SetCategoryName(const std::wstring& categoryName);
        
        void Add(SpTestBaseType test);

        void Run(const std::wstring& testInstName);

        void Run();

    private:
        static std::wstring _curTestInstName;
        static std::map<std::wstring, VectorTestsType> _tests;
    };
}

#define TEST_INIT(testInstName) \
    LeetCodeTest::Test::GetInstance().SetCategoryName(testInstName)

#define TEST_ADD(testClsName) \
    GLOBAL_SCOPE_ENTRY_ACTION(LeetCodeTest::Test::GetInstance().Add(LeetCodeTest::Test::CreateInstance<testClsName>()))

#define TEST_RUN_ALL() \
    LeetCodeTest::Test::GetInstance().Run()

#define TEST_CLASS(name) \
    struct name : public LeetCodeTest::TestBase

#define TEST_NAME(name) \
    wchar_t* Name() const { return name; }

#define TEST_DESC(desc) \
    wchar_t* Desc() const { return desc; }

#define TEST_RUN() \
    BOOL Run() override

#define TEST_BEGIN(name) \
    namespace _namespace##name{ TEST_CLASS(name)

#define TEST_END(name) \
    ;TEST_ADD(name);}
