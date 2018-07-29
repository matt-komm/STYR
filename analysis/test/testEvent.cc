#include "minitest.h"

#include "styr/Event.h"
#include "styr/Branch.h"



void test_getBranch()
{
    styr::Event event;
    const styr::Branch<int>& testBranch = event.getBranch<int>("test");
    (void)testBranch;
}

void test_createBranch()
{
    styr::Event event;
    styr::Branch<int>& testBranch = event.createBranch<int>("test");
    (void)testBranch;
}

int main()
{
    RUN_TEST(test_getBranch);
    RUN_TEST(test_createBranch);
    return 0;
}
