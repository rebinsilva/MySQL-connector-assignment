# MySQL-connector-assignment
A program that interfaces with mysql database. This is an group assignment for CS3700 at IITM (Indian Institute of Technology, Madras). 

## Description
Create an application in any programming language of your choice and access the academic_institute database from it. The application should have the following functionalities.
1. Addition of courses: The application should allow to update the course details of the even semester of 2006 for a particular department. More precisely, it should take the department id, course id, teacher id, and classroom from the user as input, perform validations on course id and teacher id and make necessary changes to the database. 
2. Student enrollment: The application should facilitate enrollment of students into the courses for the same semester and the same department used above. It should take roll No and course Ids from the user as input. Verify if the student has passed the prerequisites of the course that he/she wants to enroll. If the student had passed all the prerequisites, make necessary changes to enroll him/her in the course. It should display appropriate messages. 

## Requirements
  * MySQL server
  * MySQL Connector for C++ devlopment library
  
  On Ubuntu, use the following command to install the requirements:
  ```sh
  sudo apt-get install mysql-server libmysqlcppconn-dev
  ```

## Usage
Use the username and password of the database below

```
./assignment <username> <password>
```
