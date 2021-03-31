#include "logger.h"
#include "db.h"

static std::string ip = "127.0.0.1";
static std::string userName = "root";
static std::string password = "111111";
static std::string dbname = "dsf";

MySQL::MySQL() {
//m_conn = mysql_init(nullptr);
}

MySQL::~MySQL() {
#if 0
  if (m_conn != nullptr) {
    mysql_close(m_conn);
  }
#endif
}

bool MySQL::connect() {
#if 0
  MYSQL* p = mysql_real_connect(m_conn, ip.c_str(), userName.c_str(),
  password.c_str(), dbname.c_str(), 3306, nullptr, 0);
  if (p != nullptr) {
    mysql_query(m_conn, "set names gbk");
    LOG_MSG("connect mysql success!\n");
  } else {
    LOG_MSG("connect mysql fail!\n");
  }
  return p;
#endif
  return false;
}

bool MySQL::update(const std::string& sql) {
#if 0
  if (mysql_query(m_conn, sql.c_str())) {
    LOG_FUNC_MSG(sql.c_str(), "update fail");
    return false;
  }
  return true;
#endif
  return false;
}
#if 0
MYSQL_RES* MySQL::query(const std::string& sql) {
  if (mysql_query(m_conn, sql.c_str())) {
    LOG_FUNC_MSG(sql.c_str(), "query fail!\n");
    return nullptr;
  }
  return mysql_use_result(m_conn);
}
MYSQL* MySQL::getConnection() const {
  return m_conn;
}
#endif
