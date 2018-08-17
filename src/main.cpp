/**
 * @file test.cpp
 * @brief
 * @author arthur fatih@qq.com
 * @version 1.0.0
 * @date 2017-11-23
 */


#include <iostream>
#include "gdb.hpp"
#include "person.dm.h"

int main()
{
    GDb oGDB("127.0.0.1", 3306, "root", "000000");
    oGDB.init("");

    DBQuery oQuery;

    CDMPB_tb_Person oPerson(oGDB, oQuery);
    oPerson.CreateDB();
    oPerson.CreateTable();

    db::tb_Person data;
    data.set_id(::db::MessageId::MSGID_tb_Person);
    data.set_number("13900112233");
    data.set_email("person@163.com");
    data.set_phonetype(::db::PhoneType::WORK);

    oPerson.Insert(data);

    {
        std::vector<db::tb_Person> datas;
        oPerson.Select(data, datas);
    }


    data.set_number("123456789");

    oPerson.Update(data);

    {
        std::vector<db::tb_Person> datas;
        oPerson.Select(data, datas);
    }

    oPerson.Del(data);

    {
        std::vector<db::tb_Person> datas;
        oPerson.Select(data, datas);
    }
    return 0;
}
