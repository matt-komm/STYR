#include "minitest.h"

#include "styr/Branch.h"

#include "TLeaf.h"
#include "TTree.h"
#include "TTreeReader.h"


#include <vector>

void test_readBranchFromTree()
{
    for (int i = 0; i < 10; ++i)
    {
        TTree tree("testTree","testTree");
        float value = 0;
        tree.Branch("value",&value,"value/F");
        
        std::vector<float> values;
        for (int j = 0; j < 10; ++j)
        {
            value = 1-0.1*i*i+10.*j;
            tree.Fill();
            values.push_back(value);
        }
        ASSERT_EQ((size_t)values.size(),(size_t)tree.GetEntries());
        TTreeReader reader(&tree);
        styr::InputBranch<float> branch("value",reader);
        for (size_t entry = 0; entry< values.size(); ++entry)
        {
            reader.SetEntry(entry);
            ASSERT_EQ(branch.get(),values[entry]);
        }
    }
}

void test_readArrayFromTree()
{
    TTree tree("testTree","testTree");
    int size = 10;
    float value[20];
    tree.Branch("size",&size,"size/I");
    tree.Branch("value",&value,"value[size]/F");
    
    for (int i = 0; i < 10; ++i)
    {
        size = i+1;
        for (int j = 0; j < size; ++j)
        {
            value[j] = 1+0.5*i-0.1*j*j;
        }
        
        tree.Fill();
    }
    TTreeReader reader(&tree);
    styr::InputBranch<std::vector<float>> branch("value",reader);
    for (int i = 0; i < 10; ++i)
    {
        reader.SetEntry(i);
        ASSERT_EQ((int)branch.size(),i+1);
        
        for (int j = 0; j < size; ++j)
        {
            float v = 1+0.5*i-0.1*j*j;
            ASSERT_EQ(branch.get()[j],v);
        }
        
    }
}

int main()
{
    RUN_TEST(test_readBranchFromTree);
    RUN_TEST(test_readArrayFromTree);
    return 0;
}
