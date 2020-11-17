#include <stdlib.h>
#include <iostream>
#include <cstring>
/*
  Include directly the different
  headers from cppconn/ and mysql_driver.h + mysql_util.h
  (and mysql_connection.h). This will reduce your build time!
*/
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

bool lister(bool verbose,string table ,string row,sql::Connection *con, string sqlQuery){
  sql::Statement *stmt;
  sql::ResultSet *res;
  // cout<<"Executing :"<<sqlQuery<<"\n";
  stmt = con->createStatement();
  res = stmt->executeQuery(sqlQuery);

  if (!res->next()) {
      cout<<"The given "<<table<<" is not found in the database"<<endl;
      return false;
  }
  if(verbose){
    cout<<row<<"s availables are : ";
    while(res->next()){
      cout<<res->getString(1)<<", ";
    }
    cout<<"\n";
  }
  
  // cout<<"--\n";
  delete res;
  delete stmt;
  return true;
} 

int main(int argc, char *argv[])
{
string deptId, courseId, empId, classRoom, rollNo;
bool result;
bool verified = false;
bool verbose = false;

if(argc > 3 && strcmp(argv[3],"verbose")==0){
  verbose =true;
  cout<<"Verbose Mode ON";
}
cout << endl;

while (true) {
  int response;
  cout<<"Thing to be done:\n";
  cout<<"[1] Course Addition for even semester of 2006\n";
  cout<<"[2] Student enrollment\n";
  cout<<"[3] Exit\n";
  cout<<"Enter response[1, 2, 3]: ";
  
  cin>>response;
  if (response != 1 && response != 2)
    return EXIT_SUCCESS;

  try {
    sql::Driver *driver;
    sql::Connection *con;
    sql::Statement *stmt, *stmt2;
    sql::ResultSet *res, *res2;

    /* Create a connection */
    driver = get_driver_instance();
    con = driver->connect("tcp://127.0.0.1:3306", argv[1], argv[2]);
    /* Connect to the MySQL test database */
    con->setSchema("academic_insti");

    lister(verbose,"Database","department", con, 
        "select deptId from department");

    cout<<"Department id: ";
    cin>>deptId;
    if(!lister(verbose,"Department","Course" ,con, 
        "select courseId from course where deptNo = " + deptId + ""))continue;
  
    cout<<"Course id: ";
    cin>>courseId;

    verified = false;
    if(!lister(0,"Course Department Pair","",con,
        "select courseId from course where deptNo = '" + deptId + "' and courseId = '" + courseId + "'"))
        continue;
    verified = false;
    if (response == 1)
    {
      lister(verbose,"Department","Professor", con, 
        "SELECT empId from professor where deptNo = '" + deptId + "'");
	    cout<<"Teacher id: ";
	    cin>>empId;
      if(!lister(0,"Department's Professor","", con, 
        "SELECT empId from professor where deptNo = " + deptId + " and empId = '" + empId + "'"))
        continue;
      if(lister(0,"Course Professor Pair ","", con, 
        "SELECT empId from teaching where  courseId ='" + courseId + "' and empId = '" + empId + "' and sem = 'even' and year='2006'")){
          cout<<"Already Registered for the course\n";
          continue;
      }
      else {
          cout<<"Creating Now....\n";
      };
	    cout<<"Classroom: ";
	    cin>>classRoom;
	    stmt = con->createStatement();
      	result = stmt->execute("INSERT INTO teaching VALUES ('" + empId + "', '" 
			  + courseId + "', 'even', '2006', '" + classRoom + "')");
      cout<<"Finished Adding Teaching Entry\n ";
    }
    else if (response == 2)
    {
      if(!lister(0,"Semester Course","", con, 
        "SELECT empId from teaching where  courseId ='" + courseId + "' and sem = 'even' and year='2006'")){
          // cout<<"Already Registered for the course\n";
          continue;
      }
	    bool verified = false;
      lister(verbose,"Department","Student", con, 
        "SELECT rollNo from student where deptNo = '" + deptId + "' limit 20");
	    cout<<"Roll no of the student to be enrolled: ";
	    cin>>rollNo;
      lister(0,"Student","",con,
        "SELECT rollNo from student where rollNo = '" + rollNo+ "'"
      );


	    verified = true;
	    stmt = con->createStatement();
    	res = stmt->executeQuery("select preReqCourse from prerequisite where courseId = '" + courseId + "'");

    	while (res->next()) {
	      verified = verified &&   lister(0, "Prereq :" + res->getString("preReqCourse"), "",con,
          "select * from enrollment where courseId = '" + res->getString("preReqCourse") + "' and rollNo = " + rollNo + " and ((sem = 'even' and year < 2006) or (sem = 'odd' and year <= 2016))");
	      // if (!verified)
	        // break;
    	}

	    if (!verified) {
	      // cout<<"The given student has not finished the prerequisite course " + res->getString(1) + " for the course " + courseId<<endl;
	      continue;
	    }else{
        cout<<"Student Finished all prerequisties, Enrolling...\n";
      }

      delete res;
      delete stmt;
	    stmt = con->createStatement();
      	result = stmt->execute("INSERT INTO enrollment VALUES ('" + rollNo + "', '" 
			  + courseId + "', 'even', '2006',null)");

      delete stmt;
      cout<<"Finished Enrolling\n";

    }

    delete con;
  
  } catch (sql::SQLException &e) {
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;
  }
  cout << endl;
}

return EXIT_SUCCESS;
}
