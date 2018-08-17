#include "storm.h"

#include <memory>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>

namespace ORM{

/* from storm_config.cpp */
bool initConfig();

/* from storm_pool.cpp */
bool initPool();
void quitPool();

bool init(){
	if( !initConfig() ) return false;
	if( !initPool() ) return false;

	return true;
}
void quit(){
	quitPool();
}

std::string get_last_error(){
	MYSQL *mysql = get_db();

	return mysql_error( mysql );
}
int raw_query(const std::string &query){
	return mysql_query(
		get_db(), query.c_str() );
}
int raw_query(const std::string format, ...){
	va_list va;
	std::unique_ptr<char[]> query;
	int size = (int)(format.size() * 2); /* guessed size */

	while( true ){
		query.reset( new char [size] );

		va_start( va, format );
		int real = vsnprintf(
			&query[0],size,
			format.c_str(), va );
		va_end( va );

		if( real >= size )
			size += abs( real - size + 1);
		else
			break;
	}

	return mysql_query(
		get_db(), query.get() );
}

inline int32_t EscapeMySQLString(const char* from, int32_t from_length, char* to, int32_t to_length)
{
    assert(from && to);

    if (from_length * 2 + 1 < to_length)
    {
        return -1;
    }

    int32_t result_length = 0;

    for (int32_t index = 0; index < from_length; ++index)
    {
        char c = from[index];

        switch (c)
        {
        case '\0':
            to[result_length++] = '\\';
            to[result_length++] = '0';
            break;

        case '\n':
            to[result_length++] = '\\';
            to[result_length++] = 'n';
            break;

        case '\r':
            to[result_length++] = '\\';
            to[result_length++] = 'r';
            break;

        case 0x1a:
            to[result_length++] = '\\';
            to[result_length++] = 'Z';
            break;

        case '\'':
            to[result_length++] = '\\';
            to[result_length++] = '\'';
            break;

        case '"':
            to[result_length++] = '\\';
            to[result_length++] = '\"';
            break;

        case '\\':
            to[result_length++] = '\\';
            to[result_length++] = '\\';
            break;

        default:
            to[result_length++] = c;
        }
    }

    return result_length;
}

std::string escape(const std::string& from)
{
    std::string to;
    to.resize(from.size() * 2 + 1);
    int32_t size = EscapeMySQLString(from.c_str(), from.size(), &*to.begin(), to.size());
    to.resize(size);
    return to;
}

}
