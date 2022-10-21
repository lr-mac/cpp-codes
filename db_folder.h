#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <functional>
#include <boost/format.hpp>

namespace fs = std::filesystem;

typedef struct
{
  int ID;
  std::string PATH;
  int SIZE;
  std::string EXTENSION;
  std::string HASH;
}info_db;

typedef struct
{
  int ID1;
  std::string PATH1;
  int SIZE_LIMIT;
}info_db2;

typedef struct
{
  int ID;
  std::string PATH;
  int SIZE;
  std::string EXTENSION;
  std::string HASH;
  int ID1;
  std::string PATH1;
  int SIZE_LIMIT;

}info_db1;

static int callback(void *NotUsed, int argc, char **argv, char **azColName);

void create_func(char *sql, sqlite3 *db1);

void insert(char *sql, sqlite3 *);

void delete_sql(char *sql, sqlite3 *);

void insert_into_files();

void display_FILES_table(sqlite3 *db);

void display_MY_FOLDER_table(sqlite3 *db);

void display_join_table(sqlite3 *db);