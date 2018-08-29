#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"

void test_getNotExistingBranch()
{
    TTree tree;
    styr::Event event(&tree);
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error, err, event.getBranch<int>("test"));
}

void test_createBranch()
{
    TTree tree;
    styr::Event event(&tree);
    styr::Branch<int>& testBranch = event.createBranch<int>("test");
    testBranch.get()=10;
    (void)testBranch;
}

void test_getExistingBranch()
{
    for (int j = 0; j < 10; ++j)
    {
        TTree tree;
        styr::Event event(&tree);
        std::runtime_error err("");
        styr::Branch<int>& testBranch1 = event.createBranch<int>("test");
        const styr::Branch<int>& testBranch2 = event.getBranch<int>("test");
        testBranch1.get()=(int)1+j-0.2*j*j;
        ASSERT_EQ(testBranch2.get(),(int)(1+j-0.2*j*j));
    }
}

void test_getExistingBranchWrongType()
{
    TTree tree;
    styr::Event event(&tree);
    event.createBranch<int>("test");
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error,err,event.getBranch<float>("test"));
}

void test_createExistingBranch()
{
    TTree tree;
    styr::Event event(&tree);
    styr::Branch<int>& testBranch = event.createBranch<int>("test");
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error,err,event.createBranch<int>("test"));
    (void)testBranch;
}

void test_getBranchFromTree()
{
    TTree tree;
    float value = 10;
    tree.Branch("value",&value,"value/F");
    float value2 = 10;
    tree.Branch("value2",&value2,"value2/F");
    styr::Event event(&tree);
    const styr::Branch<float>& branch1 = event.getBranch<float>("value");
    const styr::Branch<float>& branch2 = event.getBranch<float>("value");
    const styr::Branch<float>& branch3 = event.getBranch<float>("value2");
    ASSERT(&branch1==&branch2);
    ASSERT(&branch1!=&branch3);
    ASSERT(&branch2!=&branch3);
}

int main()
{
    RUN_TEST(test_getNotExistingBranch);
    RUN_TEST(test_createBranch);
    RUN_TEST(test_createExistingBranch);
    RUN_TEST(test_getExistingBranch);
    RUN_TEST(test_getExistingBranchWrongType);
    RUN_TEST(test_getBranchFromTree);
    return 0;
}
