#include "minitest.h"

#include "styr/Config.h"

#include <vector>

void test_createConfig()
{
    styr::Config cfg;
    cfg.set("bal1","blu1").set("bal2","blu2").set("bal3",2);
    ASSERT(cfg.get<std::string>("bal1")=="blu1");
    ASSERT(cfg.get<std::string>("bal2")=="blu2");
    std::runtime_error err("");
    ASSERT_RAISE_A(std::runtime_error, err,cfg.get<std::string>("bl"));
    ASSERT_RAISE_A(std::runtime_error, err,cfg.get<std::string>("bal3"));
    ASSERT(cfg.get<int>("bal3")==2);
}


int main()
{
    RUN_TEST(test_createConfig);
    return 0;
}
