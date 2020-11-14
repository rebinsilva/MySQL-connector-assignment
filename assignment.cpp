#include <stdlib.h>
#include <iostream>

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

int main(void)
{
string deptId, courseId, empId, classRoom, rollNo;
bool result;
bool verified = false;
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
    con = driver->connect("tcp://127.0.0.1:3306", "rebin", "#4piggyvector");
    /* Connect to the MySQL test database */
    con->setSchema("academic_insti");


    cout<<"Department id: ";
    cin>>deptId;
    cout<<"Course id: ";
    cin>>courseId;

    verified = false;
    stmt = con->createStatement();
    res = stmt->executeQuery("select courseId from course where deptNo = " + deptId + " and courseId = '" + courseId + "'");

    if (!res->next()) {
      cout<<"The given course is not found in the database"<<endl;
      continue;
    }

    delete res;
    delete stmt;

    verified = false;
    if (response == 1)
    {

	cout<<"Teacher id: ";
	cin>>empId;
	cout<<"Classroom: ";
	cin>>classRoom;

	stmt = con->createStatement();
    	res = stmt->executeQuery("SELECT empId from professor where deptNo = " + deptId + " and empId = '" + empId + "'");

	if (!res->next()) {
	  cout<<"The given teacher is not found in the database"<<endl;
	  continue;
	}

    	delete res;
    	delete stmt;

	stmt = con->createStatement();
    	result = stmt->execute("INSERT INTO teaching VALUES ('" + empId + "', '" 
			+ courseId + "', 'even', '2006', '" + classRoom + "')");

    	delete stmt;
    }
    else if (response == 2)
    {
	bool verified = false;

	cout<<"Roll no of the student to be enrolled: ";
	cin>>rollNo;

	stmt = con->createStatement();
    	res = stmt->executeQuery("SELECT rollNo from student where rollNo = " + rollNo);

    	if (!res->next()) {
	  cout<<"The given student is not found in the database"<<endl;
	  continue;
	}

    	delete res;
    	delete stmt;


	verified = true;
	stmt = con->createStatement();
    	res = stmt->executeQuery("select preReqCourse from prerequisite where courseId = '" + courseId + "'");

    	while (res->next()) {
	  stmt2 = con->createStatement();
    	  res2 = stmt->executeQuery("select * from enrollment where courseId = '" + res->getString("preReqCourse") + "' and rollNo = " + rollNo + " and ((sem = 'even' and year < 2006) or (sem = 'odd' and year <= 2016))");
	  verified = verified && res2->next();
	  if (!verified)
	    break;
    	}

	if (!verified) {
	  cout<<"The given student has not finished the prerequisite course " + res->getString(1) + " for the course " + courseId<<endl;
	  continue;
	}

    	delete res2;
    	delete stmt2;
    	delete res;
    	delete stmt;

	stmt = con->createStatement();
    	result = stmt->execute("INSERT INTO enrollment VALUES ('" + rollNo + "', '" 
			+ courseId + "', 'even', '2006', 'S')");

    	delete stmt;
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
