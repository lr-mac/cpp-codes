#include "db_folder.h"
#include "hash.h"
int main()
{
  sqlite3 *db1;
  char *zErrMsg = 0;
  int rc;
  std::string sql;
  int id = 0;

  rc = sqlite3_open("files.db", &db1);

  if (rc)
  {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db1));
    return (0);
  }
  else
  {
    fprintf(stdout, "Opened database successfully\n");
  }

  sql = "CREATE TABLE     IF NOT EXISTS  FILES("
        "ID             INT     NOT NULL,"
        "PATH           TEXT    NOT NULL,"
        "SIZE           INT     NOT NULL,"
        "EXTENSION      CHAR(15),"
        "HASH           TEXT    NOT NULL);";

  create_func(sql.data(), db1);

  sql = "CREATE TABLE  IF NOT EXISTS MY_FOLDER("
        "F_ID             INT     NOT NULL,"
        "F_PATH           TEXT    NOT NULL,"
        "SIZE_LIMIT       INT     NOT NULL);";

  create_func(sql.data(), db1);

  sql = "DELETE FROM FILES";
  delete_sql(sql.data(), db1);

  Hash obj;

  for (const fs::directory_entry &dir_entry : fs::recursive_directory_iterator("/home/Desktop/myFolder"))
  {
    float size;
    std::string extension = fs::path(dir_entry).extension();
    id++;

    if (dir_entry.is_regular_file())
      size = fs::file_size(dir_entry);
    else
      size = 0;  //can't get size of folder

    std::string message;
    message = get_from_file(dir_entry);

    // std::string encrypted;
    // encrypted = obj.hash_str(message.data());
    //std::cout << encrypted << std::endl;

    if (extension == ".part")
      continue;
    else
    {
      boost::format query =
          boost::format(
              "INSERT INTO FILES (ID, PATH, SIZE, EXTENSION,HASH)"
              " VALUES('%d','%s', '%d', '%s', '%s');") %
          id % dir_entry % size % extension % obj.hash_str(message.data());

      insert(query.str().data(), db1);
    }
  }

  display_FILES_table(db1);

  sql = "DELETE FROM MY_FOLDER";
  delete_sql(sql.data(), db1);

  id = 0; 

  for (const fs::directory_entry &dir_entry : fs::recursive_directory_iterator("/home/Desktop/myFolder"))
  {
    int size_in_table;
    int size_limit_of_file = 2000000;
    id++;


    if (fs::file_size(dir_entry) > size_limit_of_file)
      size_in_table = 0;
    else
      size_in_table = 1;

    boost::format query =
        boost::format(
            "INSERT INTO MY_FOLDER(F_ID, F_PATH, SIZE_LIMIT)"
            " VALUES('%d','%s','%d');") %
        id % dir_entry % size_in_table;

    insert(query.str().data(), db1);
  }
  
  display_MY_FOLDER_table(db1);

  std::cout << std::endl;

  display_join_table(db1);

  sqlite3_close(db1);
  return 0;
}