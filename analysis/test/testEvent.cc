#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"

void test_getNotExistingBranch()
{
    styr::Event event;
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error, err, event.getBranch<int>("test"));
}

void test_createBranch()
{
    styr::Event event;
    styr::Branch<int>& testBranch = event.createBranch<int>("test");
    (void)testBranch;
}

void test_getExistingBranch()
{
    styr::Event event;
    std::runtime_error err("");
    event.createBranch<int>("test");
    const styr::Branch<int>& testBranch2 = event.getBranch<int>("test");
    (void)testBranch2;
}

void test_getExistingBranchWrongType()
{
    styr::Event event;
    event.createBranch<int>("test");
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error,err,event.getBranch<float>("test"));
}

void test_createExistingBranch()
{
    styr::Event event;
    styr::Branch<int>& testBranch = event.createBranch<int>("test");
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error,err,event.createBranch<int>("test"));
    (void)testBranch;
}

int main()
{
    RUN_TEST(test_getNotExistingBranch);
    RUN_TEST(test_createBranch);
    RUN_TEST(test_createExistingBranch);
    RUN_TEST(test_getExistingBranch);
    RUN_TEST(test_getExistingBranchWrongType);
    return 0;
}
