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
    auto testBranch = event.createBranch<int>("test");
    testBranch->get()=10;
    (void)testBranch;
}

void test_getExistingBranch()
{
    for (int j = 0; j < 10; ++j)
    {
        TTree tree;
        styr::Event event(&tree);
        std::runtime_error err("");
        auto testBranch1 = event.createBranch<int>("test");
        auto testBranch2 = event.getBranch<int>("test");
        testBranch1->get()=(int)1+j-0.2*j*j;
        ASSERT_EQ(testBranch2->get(),(int)(1+j-0.2*j*j));
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
    auto testBranch = event.createBranch<int>("test");
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error,err,event.createBranch<int>("test"));
    (void)testBranch;
}

void test_getBranchFromTree()
{
    for (int i = 0; i < 10; ++i)
    {
        TTree tree;
        float value = 1+i;
        tree.Branch("value",&value,"value/F");
        float value2 = value+1;
        tree.Branch("value2",&value2,"value2/F");
        tree.Fill();
        styr::Event event(&tree);
        auto branch1 = event.getBranch<float>("value");
        auto branch2 = event.getBranch<float>("value");
        auto branch3 = event.getBranch<float>("value2");
        
        ASSERT_EQ(branch1->size(),(size_t)1);
        ASSERT_EQ(branch2->size(),(size_t)1);
        ASSERT_EQ(branch3->size(),(size_t)1);
        
        ASSERT(branch1.get()==branch2.get());
        ASSERT(branch1.get()!=branch3.get());
        ASSERT(branch2.get()!=branch3.get());
        
        ASSERT_EQ(branch1->get(),branch2->get());
        ASSERT(branch1->get()!=branch3->get());
        ASSERT(branch2->get()!=branch3->get());
    }
}

void test_createAndGetValueBranch()
{
    TTree tree;
    styr::Event event(&tree);
    std::vector<float> values;
    for (int i = 0; i < 10; ++i)
    {
        auto branch1 = event.createBranch<float>("value"+std::to_string(i));
        float value = 1.+i*0.5-0.2*i*i;
        branch1->get()=value;
        values.push_back(value);
    }
    for (int i = 0; i < 10; ++i)
    {
        auto branch2 = event.getBranch<float>("value"+std::to_string(i));
        ASSERT_EQ(branch2->get(),values[i]);
    }
}

void test_createAndGetArrayBranch()
{
    TTree tree;
    styr::Event event(&tree);
    std::vector<std::vector<float>> values;
    for (int i = 0; i < 10; ++i)
    {
        std::vector<float> valueArray;
        auto branch1 = event.createBranch<std::vector<float>>("valueList"+std::to_string(i));
        for (int j = 0; j < i*3; ++j)
        {
            valueArray.push_back(1.+j*0.5-0.2*i*i);
        }
        branch1->get()=valueArray;
        values.push_back(valueArray);
    }
    for (int i = 0; i < 10; ++i)
    {
        auto branch2 = event.getBranch<std::vector<float>>("valueList"+std::to_string(i));
        ASSERT_EQ(branch2->size(),values[i].size());
        for (size_t j = 0; j < branch2->get().size(); ++j)
        {
            ASSERT_EQ(branch2->get()[j],values[i][j]);
        }
    }
}

int main()
{
    RUN_TEST(test_getNotExistingBranch);
    RUN_TEST(test_createBranch);
    RUN_TEST(test_createExistingBranch);
    RUN_TEST(test_getExistingBranch);
    RUN_TEST(test_getExistingBranchWrongType);
    RUN_TEST(test_getBranchFromTree);
    RUN_TEST(test_createAndGetValueBranch);
    RUN_TEST(test_createAndGetArrayBranch);
    return 0;
}
