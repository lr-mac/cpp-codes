#include "db_folder.h"

sqlite3 *db1;
char *zErrMsg = 0;
int rc;
char *sql;

std::vector <std::map<std::string, std::string>> vect;  // |
std::map<std::string, std::string> mapa;                // | => to store FILES 
std::vector<info_db> vectorr;                           // |

std::vector <std::map<std::string, std::string>> vect1; // |
std::map<std::string, std::string> mapa1;               // | => to store JOIN 
std::vector<info_db1> vectorr1;                         // |

std::vector <std::map<std::string, std::string>> vect2; // |
std::map<std::string, std::string> mapa2;               // | => to store MY_FOLDER
std::vector<info_db2> vectorr2;                         // |


static int callback(void *NotUsed, int argc, char **argv, char **azColName) 
{
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int callback1(void *data, int argc, char **argv, char **azColName)
{
    std::map<std::string, std::string> mapBuff {};
    std::function<void(std::map<std::string, std::string>)> *fnCallBack;
    fnCallBack =
      static_cast<std::function<void(std::map<std::string, std::string>)> *>(
        data);
    auto callBackValue = *fnCallBack;

    /* pass collumn and record values to map*/
    for (int i = 0; i < argc; i++)
    {
        mapBuff[azColName[i]] = argv[i];
    }
    /* check if callBackVal gets data value */
    if (callBackValue)
    {
        callBackValue(mapBuff);
    }

    return SQLITE_OK;
}

void create_func(char *sql, sqlite3 *db1)
{
   rc = sqlite3_exec(db1, sql, callback, 0, &zErrMsg);

      if( rc != SQLITE_OK ){
         fprintf(stderr, "Create SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      } else {
         //fprintf(stdout, "Table created successfully\n");
      }
}
void insert(char *sql, sqlite3 *db1)
{
   rc = sqlite3_exec(db1, sql, callback, 0, &zErrMsg);

      if( rc != SQLITE_OK ){
         fprintf(stderr, "Insert SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      } else {
         //fprintf(stdout, "Insertion was done successfully\n");
      }
}
void delete_sql(char *sql, sqlite3 *db1)
{
   rc = sqlite3_exec(db1, sql, callback, 0, &zErrMsg);

      if( rc != SQLITE_OK ){
         fprintf(stderr, " Delete SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      } else {
         //fprintf(stdout, "Data was deleted successfully\n");
      }
}

void display_FILES_table(sqlite3 *db)
{
   std::stringstream ss{};
   info_db data; 

   std::string query = ("SELECT * FROM FILES");


   std::function<void(std::map<std::string, std::string> &)> CB =
      [&](std::map<std::string, std::string> &mapa) {
          vect.push_back(mapa);
      };
   rc = sqlite3_exec(db, query.c_str(), callback1, &CB, &zErrMsg);

   if( rc != SQLITE_OK ){
         fprintf(stderr, "Insert in map SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      } else {
         //fprintf(stdout, "insert in map was done successfully\n");
      }

   for(auto &mV : vect)
   {
      data.PATH = mV["PATH"];

      ss << mV["ID"];
      ss >> data.ID;

      ss.clear();

      ss << mV["SIZE"];
      ss >> data.SIZE;

      data.EXTENSION = mV["EXTENSION"];
      data.HASH = mV["HASH"];

      vectorr.push_back(data);



      data = {0};
      ss.clear();
   }
   std::cout << std::endl;
   std::cout << "ID" << "\tPATH" << "\tSIZE" << "\tEXTENSION" << "\tHASH"<< std::endl;
   std::cout << std::endl;
   
   for(auto i : vectorr)
   {
      
      std::cout << i.ID << "\t";
      std::cout << i.PATH << "\t";
      std::cout << i.SIZE << "\t";
      std::cout << i.EXTENSION << "\t";
      std::cout << i.HASH << std::endl;
   }
}

void display_MY_FOLDER_table(sqlite3 *db)
{
   std::stringstream ss{};
   info_db2 data2; 

   std::string query = ("SELECT * FROM MY_FOLDER");

   std::function<void(std::map<std::string, std::string> &)> CB =
      [&](std::map<std::string, std::string> &mapa2) {
          vect2.push_back(mapa2);
      };
   rc = sqlite3_exec(db, query.c_str(), callback1, &CB, &zErrMsg);

   if( rc != SQLITE_OK ){
         fprintf(stderr, "Insert in map SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
      } else {
         //fprintf(stdout, "insert in map was done successfully\n");
      }

   for(auto &mV : vect2)
   {
      data2.PATH1 = mV["F_PATH"];

      ss << mV["F_ID"];
      ss >> data2.ID1;

      ss.clear();

      ss << mV["SIZE_LIMIT"];
      ss >> data2.SIZE_LIMIT;

      vectorr2.push_back(data2);

      data2 = {0};
      ss.clear();
   }

   std::cout << std::endl;
   std::cout << "F_ID" << "\tF_PATH" << "\tSIZE_LIMIT" << std::endl;
   std::cout << std::endl;
   for(auto i : vectorr2)
   {
      std::cout << i.ID1 << "\t";
      std::cout << i.PATH1 << "\t";
      std::cout << i.SIZE_LIMIT << std::endl;
   }
}

 void display_join_table(sqlite3 *db)
{
   std::stringstream ss{};
   info_db1 data1; 

   std::string query = "SELECT * FROM FILES "\
                        "INNER JOIN MY_FOLDER "\
                        "ON FILES.PATH = MY_FOLDER.F_PATH "\
                        "WHERE MY_FOLDER.SIZE_LIMIT = 1;";


   std::function<void(std::map<std::string, std::string> &)> CB =
      [&](std::map<std::string, std::string> &mapa1) {
          vect1.push_back(mapa1);
      };
   rc = sqlite3_exec(db, query.c_str(), callback1, &CB, &zErrMsg);

   if( rc != SQLITE_OK ){
         fprintf(stderr, "Insert in map1 SQL error: %s\n", zErrMsg);
         sqlite3_free(zErrMsg);
   } else {
         //fprintf(stdout, "insert in map was done successfully\n");
   }

   for(auto &mV : vect1)
   {
      ss << mV["ID"];
      ss >> data1.ID;
      ss.clear();

      data1.PATH = mV["PATH"];

      ss << mV["SIZE"];
      ss >> data1.SIZE;
      ss.clear();

      data1.EXTENSION = mV["EXTENSION"];

      data1.HASH = mV["HASH"];

      ss << mV["F_ID"];
      ss >> data1.ID1;

      data1.PATH1 = mV["F_PATH"];

      ss.clear();

      ss << mV["SIZE_LIMIT"];
      ss >> data1.SIZE_LIMIT;

      ss.clear();

      vectorr1.push_back(data1);

      data1 = {0};
      ss.clear();

   }

   std::cout << std::endl;
   std::cout << "ID" << "\tPATH\t" << "\tSIZE" << "\tEXTENSION" << "\tHASH \tF_ID \tF_PATH\tSIZE_LIMIT" << std::endl;
   std::cout << std::endl;

   for(auto i : vectorr1)
   {
      std::cout << i.ID << "\t";
      std::cout << i.PATH << "\t";
      std::cout << i.SIZE << "\t";
      std::cout << i.EXTENSION << "\t";
      std::cout << i.HASH << "\t";
      std::cout << i.ID1 << "\t";
      std::cout << i.PATH1 << "\t";
      std::cout << i.SIZE_LIMIT << std::endl;
   }
}
