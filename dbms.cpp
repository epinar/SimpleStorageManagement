// CmpE 321 Introduction to Database Systems
// Spring 2014
// Project 2
// Implementation of the simple storage management system.
// Student Name: Evin Pinar Ornek
// Student ID: 2012400057

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <sstream>

using namespace std;

string syscat = "syscat.dat";

void createFile(){
	ofstream out;
	out.open(syscat, ios::app);
	string fileName, fieldName; int fieldnum, fieldSize, key; 
	cout<<"What is the file name?(Please write it without blank) "; cin>>fileName; out<<fileName<<" ";
	cout<<"What is the number of fields?(Max 10) "; cin>>fieldnum; out<<fieldnum<<" ";
	cout<<"Which one is the primary key? "; cin>>key; out<<key<<" ";
	cout<<"What is the field size? (Max 29 bytes) "; cin>>fieldSize; out<<fieldSize<<" ";
	cout<<"What are the field names? "<<endl;
	for(int i=0; i<fieldnum; i++){
		cout<<(i+1)<<".Field: ";
		cin>>fieldName; 
		out<<fieldName<<" ";
	}
	out<<endl;
	out.close();

	ofstream out2;
	out2.open(fileName);
	out2<<30; //Number of empty records.
	out2.close();
}

void deleteFile(){
	string fileName, line, name, temp;
	temp="temp";
	cout<<"What is the file name you want to delete? "; cin>>fileName; 
	ifstream in(syscat);
	ofstream out(temp);
	while(getline(in, line)){
		istringstream iss(line);
		iss>>name;
		if(name!=fileName){
			out<<line<<"\n";
		}
	}
	in.close();
	out.close();
	remove(syscat.c_str());
	rename(temp.c_str(), syscat.c_str());
	remove(fileName.c_str());
}

void displayFile(){
	string fileName, fieldName, line, name, fieldNum, fieldSize, key; int fieldNumb;
	cout<<"What is the file name? "; cin>>fileName;
	ifstream in(syscat);
	while(getline(in, line)){
		istringstream iss(line);
		iss>>name;
		if(name==fileName){
			iss>>fieldNum;
			fieldNumb=atoi(fieldNum.c_str());
			iss>>fieldSize;
			cout<<"Number of fields: "<<fieldNum<<" -> ";
			for(int i=1; i<fieldNumb; i++){
				iss>>fieldName;
				cout<<fieldName<<" - ";
			}
			iss>>fieldName;
			cout<<fieldName<<endl;
			cout<<"Size of fields: "<<fieldSize<<" bytes"<<endl;
			iss>>key;
			cout<<"The primary key valu : "<<key<<endl;
		}
	}
	in.close();
}

void displayAllFiles(){
	int counter=1;
	string fileName, fieldName, line, name, fieldNum, fieldSize, key; int fieldNumb;
	ifstream in(syscat);
	while(getline(in, line)){
		cout<<endl;
		istringstream iss(line);
		iss>>name;
		cout<<counter<<".File name: "<<name<<endl;
		iss>>fieldNum;
		fieldNumb=atoi(fieldNum.c_str());
		iss>>key;
		cout<<"The primary key value : "<<key<<endl;
		iss>>fieldSize;
		cout<<"Number of fields: "<<fieldNum<<" -> ";
		for(int i=1; i<fieldNumb; i++){
			iss>>fieldName;
			cout<<fieldName<<" - ";
		}
		iss>>fieldName;
		cout<<fieldName<<endl;
		cout<<"Size of fields: "<<fieldSize<<" bytes"<<endl;
		counter++;
	}
	in.close();
}

void insertRecord(){
	string fileName, line, name, fields, record, temp; int emptyRecord=0, fieldNum; bool isEmpty=0; //
	cout<<"The files are listed above, please choose a type to insert your record."<<endl;
	displayAllFiles();
	cout<<"What is the file name to insert a new record?: "; cin>>fileName;
	ifstream in1(syscat); // Check the number of fields from the syscat.
	while(getline(in1, line)){
		istringstream iss(line);
		iss>>name;
		if(name==fileName){
			iss>>fieldNum;
		}
	}
	in1.close();

	record=""; //Take the input.
	for(int i=1; i<=fieldNum; i++){
		cout<<"Please type the "<<i<<". field: ";
		cin>>fields;
		record=record + " " + fields;
	}

	temp="temp";
	ifstream in(fileName);
	ofstream out(temp);

	while(getline(in, line)){
		istringstream iss(line);
		iss>>name;
		isEmpty=1;
		emptyRecord=atoi(name.c_str());
		if(emptyRecord==0){ // The maximum records in a page is 30. 
			out<<emptyRecord<<"\n";
			for(int i=0; i<30; i++){
				getline(in,line);
				out<<line<<"\n";
			}
			isEmpty=0;
		}
		else{ // If the page is empty, we decrease "empty page" from the header and insert a record.
			emptyRecord--;
			out<<emptyRecord<<"\n";
			for(int i=1; i<=30-(emptyRecord+1); i++){
				getline(in, line);
				out<<line<<"\n";
			}	
			isEmpty=1;
		}
	}
	if(isEmpty==0){
		out<<29<<"\n";
		out<<record<<"\n";
	}else{
		out<<record<<"\n";
	}

	in.close();
	out.close();
	remove(fileName.c_str());
	rename(temp.c_str(), fileName.c_str());
}

void retrieveRecord(){
	string fileName, line, name, field, temp, keyValue; 
	int fieldNum, primaryKey, emptyRecord; 
	bool isOk, findIt=0;//
	cout<<"\n All of the files are listed below. \n";
	displayAllFiles();
	cout<<"What is the file name of the record? "; cin>>fileName;
	ifstream in(syscat);  //Find the primary key from the system catalogue.
	while(getline(in, line)){
		istringstream iss(line);
		iss>>name;
		if(name==fileName){
			iss>>fieldNum;
			iss>>primaryKey;
			break;
		}
	}
	in.close();
	cout<<"What is the primary key value of the record? "; 
	cin>>keyValue;
	ifstream in1(fileName); //Find the record with a given primary key value.
	while(getline(in1, line) && findIt==0){
		istringstream iss(line);
		iss>>name;
		emptyRecord=atoi(name.c_str()); 
		for(int i=1; i<=30-emptyRecord; i++){  // Find the field with the given key value.
			getline(in1, line);
			istringstream iss1(line);
			for(int i=1; i<=primaryKey; i++){
				iss1>>field;
			}
			if(field==keyValue){
				istringstream iss2(line);
				for(int i=1; i<=fieldNum; i++){
					iss2>>field;
					cout<<i<<". Field : "<<field<<endl;
				}
				findIt=1;
			}
		}
	}
	in1.close();
}

void retrieveAllRecords(){
	string fileName, line, name, field;
	int fieldNum, curr=0, emptyRecord; 
	bool isOk;
	cout<<"\n All of the files are listed below. \n";
	displayAllFiles();
	cout<<"What is the file name of the record? "; cin>>fileName;
	ifstream in(syscat);  //Find the primary key from the system catalogue.
	while(getline(in, line)){
		istringstream iss(line);
		iss>>name;
		if(name==fileName){
			iss>>fieldNum;
			break;
		}
	}
	in.close();
	ifstream in1(fileName); 
	while(getline(in1, line)){ //Output all the records
		istringstream iss(line);
		iss>>name;
		emptyRecord=atoi(name.c_str()); 
		for(int i=1; i<=30-emptyRecord; i++){  
			getline(in1, line);
			curr++;
			cout<<endl;
			cout<<curr<<". Record's fields: \n";
			istringstream iss1(line);
			for(int j=1; j<=fieldNum; j++){
				iss1>>field;
				cout<<j<<". Field : "<<field<<endl;
			}
			
		}
	}
	in1.close();
}

void deleteRecord(){
	string fileName, line, name, field, temp, keyValue, temp1; 
	int fieldNum, primaryKey, emptyRecord, lineCnt=0, emptyRecLine;//Line counter 
	bool isOk, findIt=0;//
	temp="temp";
	temp1="temp1";
	cout<<"\n All of the files are listed below. \n";
	displayAllFiles();
	cout<<"What is the file name of the record to delete? "; cin>>fileName;
	ifstream in(syscat);  //Find the primary key from the system catalogue.
	ofstream out(temp);
	while(getline(in, line)){
		istringstream iss(line);
		iss>>name;
		if(name==fileName){
			iss>>fieldNum;
			iss>>primaryKey;
			break;
		}
	}
	in.close();
	cout<<"What is the primary key value of the record to delete? "; 
	cin>>keyValue;
	ifstream in1(fileName); //Find the record with a given primary key value.
	while(getline(in1, line)){
		istringstream iss(line);
		iss>>name;
		emptyRecord=atoi(name.c_str()); 
		out<<emptyRecord<<"\n";
		for(int i=1; i<=30-emptyRecord; i++){  // Find the field with the given key value.
			getline(in1, line);
			istringstream iss1(line);
			findIt=0;
			for(int i=1; i<=primaryKey; i++){
				iss1>>field;
			}
			if(field==keyValue){
				istringstream iss2(line);
				for(int i=1; i<=fieldNum; i++){
					iss2>>field;
					cout<<i<<". Field : "<<field<<endl;
				}
				findIt=1;
				emptyRecLine=lineCnt;
			}
			lineCnt++;
			if(!findIt){
				out<<line<<"\n";
			}
		}
	}
	in1.close();
	out.close();

	//Now, change the header, the "empty record" value. 
	lineCnt=0;
	emptyRecLine/=31;
	ifstream in2(temp);
	ofstream out1(temp1);
	while(getline(in2,line)){
		istringstream iss2(line);
		iss2>>name;
		emptyRecord=atoi(name.c_str());
		if(lineCnt==emptyRecLine){
			out1<<(emptyRecord+1)<<"\n";
		}else{
			out1<<emptyRecord<<"\n";
		}
		for(int i=1; i<=30-emptyRecord; i++){
			getline(in2,line);
			out1<<line<<"\n";
		}
		lineCnt++;
	}

	in2.close();
	out1.close();
	remove(fileName.c_str());
	rename(temp1.c_str(), fileName.c_str()); 
	remove(temp.c_str());
}

void deleteAllRecords(){
	string fileName, line, name, temp;
	temp="temp";
	cout<<"\n All of the files are listed below. \n";
	displayAllFiles();
	cout<<"Which records of a file you would like to delete? ";
	cin>>fileName;
	ofstream out(temp);
	out<<30; //Number of empty records. 
	remove(fileName.c_str());
	rename(temp.c_str(), fileName.c_str());
}

void goToOperation(int opt){
	switch(opt){
		case 1 : createFile(); break;
		case 2 : deleteFile(); break;
		case 3 : displayFile(); break;
		case 4 : displayAllFiles(); break;
		case 5 : insertRecord(); break;
		case 6 : retrieveRecord(); break;
		case 7 : retrieveAllRecords(); break;
		case 8 : deleteRecord(); break;
		case 9 : deleteAllRecords(); break;
	}
}

int main(){
	int opt=0;
	bool cont=1;
	cout<<"\n\n\nWelcome to the Simple Storage Management System ! \n\n"<<endl;
	while(cont){
		cout<< "There are some operations for Data Definition Language(DDL) and Data Manipulation Language(DML).\nPlease choose an option.\n" <<endl;
		cout<< "DDL options:\n\t1. Create a Type of File\n\t2. Delete a File\n\t3. Display a File\n\t4. Display all Files" <<endl;
		cout<< "DML options:\n\t5. Insert a Record\n\t6. Retrieve a Record\n\t7. Retrieve all Records\n\t8. Delete a Record\n\t9. Delete all Records" <<endl;
		cout<<"\n\nNumber of your option : ";
		cin>>opt;
		goToOperation(opt);
		cout<<"\nWould you like to continue or exit? (0/1) ";
		cin>>cont;
	}	
	cout<<"\n\nGoodbye !\n\n"<<endl;


	return 0;
} 