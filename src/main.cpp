
#include <iostream>

//#include "person.dm.h"
#include "storm.h"

void INSERT_QUERY()
{
    auto query =
        ORM::from("test")->create();

    query->set("id", "pjc");
    query->set("nick", "anz");

    (*query)["another"] = "method";

    query->save();
}

void UPDATE_QUERY()
{
    auto query =
        ORM::from("test")
        ->where("id", "pjc0247")
        ->find_one();

    (*query)["nickname"] = "anz";

    query->save();
}
void SELECT_QUERY()
{
    auto query = ORM::from("test");

    auto result = query
        ->where("id", "foo")
        ->where("nick", "bar")
        ->select("level")
        ->find_one();
    std::cout << (*result)["level"];

    auto results = query
        ->where_raw("nick=foo")
        ->limit(5)
        ->find_many();
    for (auto result : results)
        std::cout << (*result)["level"] << std::endl;
}

void DELETE_QUERY()
{
    auto query =
        ORM::from("test")
        ->where("id", "pjc0247")
        ->find_one();
    query->remove();

    auto query2 =
        ORM::from("test")
        ->where("level", "1");
    query2->remove();
}

int main()
{
    ORM::configure("host", "localhost");
    ORM::configure("user", "pjc");
    ORM::configure("password", "1234");
    ORM::configure("db", "dbname");

    if (!ORM::init())
    {
        return -1;
    }

    INSERT_QUERY();
    UPDATE_QUERY();
    SELECT_QUERY();
    DELETE_QUERY();
    ORM::quit();
    return 0;
}
