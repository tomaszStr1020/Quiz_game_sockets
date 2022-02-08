#include <string.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fstream>
#include <unistd.h>
#include <pthread.h>
#include <set>
#include <sstream>
#include <cmath>
#include <math.h>
using namespace std;
int BL = 4;


//konstruktor i destruktor
// Lobby::Lobby(structDataThread st_thread, int descriptor)
// {
//     char quiz_name[2] = read(descriptor, quiz_name, 2);
    
// }

// Lobby::~Lobby()
// {
//}

struct Client {
    int descriptor;
    string name;
    int room;
    string nickname;
    int points;
    string answer;
    int waitpoints;
}; 
struct Question {
    string number;
    string question;
    string correctans;
    string ans1;
    string ans2;
    string ans3;
}; 
struct Lobby {
    string name;
    string owner;
    string lobby_id;	
    int room;
    int start;
    string correctans;
    string activeque;
    vector<string> participants;
    vector<Question> questions;
}; 
struct structDataThread {
    Client *client;
    Lobby *lobby;
    pthread_mutex_t rMutex[20];
    pthread_mutex_t* cMutex;
    vector<Client>* clients;
    vector<Lobby>* lobbys;
    bool flag;
}; 

vector<Question> fetchQuiz(std::string fname){
    vector<Question> questions;
    streampos size;
    char * memblock;
    string pyt;
    string cor;
    string ans1;
    string ans2;
    string ans3;
    ifstream file ( fname, ios::in| ios::binary|ios::ate);
    if (file.is_open())
    {   
        cout<<"Otwarty plik!"<<endl;
        size = file.tellg();
        memblock = new char [size];
        file.seekg(0, ios::beg);
        file.read(memblock, size);
        file.close();
    }

    //Szukanie ilosci pytan
    int numberOfQuestions = 0;
    int i=0;
    int j=0;

    for(i; i<5; i++){
        if(memblock[i] == '&'){
            break;
        }
    }
    for(j; j<i; j++)
    {
        numberOfQuestions = numberOfQuestions + ((int)memblock[j]-48)* pow(10, i-j-1) ;
    }

    cout<<"Ilosc pytan: "<<numberOfQuestions<<endl;
    for(int x=0;x<numberOfQuestions;x++){
    
	    i++;	
	    i++;
	    vector <char> data;
	    while(true){
	    	if(memblock[i]==';'){break;}
	    	else{
	    	data.push_back(memblock[i]);
	    	}
	    	i++;
	    }
	    pyt=string(data.begin(),data.end());
	    data.clear();
	    i++;
	    i++;
	    while(true){
	    	if(memblock[i]==';'){break;}
	    	else{
	    	data.push_back(memblock[i]);
	    	}
	    	i++;
	    }
	    cor=string(data.begin(),data.end());
	    data.clear();
	    i++;
	    i++;
	    while(true){
	    	if(memblock[i]==';'){break;}
	    	else{
	    	data.push_back(memblock[i]);
	    	}
	    	i++;
	    }
	    ans1=string(data.begin(),data.end());
	    data.clear();
	    i++;
	    i++;
	    while(true){
	    	if(memblock[i]==';'){break;}
	    	else{
	    	data.push_back(memblock[i]);
	    	}
	    	i++;
	    }
	    ans2=string(data.begin(),data.end());
	    data.clear();
	    i++;
	    i++;
	    while(true){
	    	if(memblock[i]=='&'){break;}
	    	else{
	    	data.push_back(memblock[i]);
	    	}
	    	i++;
	    }
	    ans3=string(data.begin(),data.end());
	    data.clear();
	    Question question;
	    question.number=to_string(x+1);
	    question.question=pyt;
	    question.correctans=cor;
	    question.ans1=ans1;
	    question.ans2=ans2;
	    question.ans3=ans3;
	    questions.push_back(question);
    }    
    return questions;
}

//Deklaracje funkcji wykorzystywanych przed pelna deklaracja


//

int readLength(structDataThread *st_thread){
    int res;
    char length[BL];
    char* readLength = new char[BL];
    string message = "";
    cout<<"1"<<endl;
    memset(readLength, 0, BL);
    for(int i =0; i < BL;) {

        res = read(st_thread -> client->descriptor, readLength, BL - i);
        cout<<"2"<<endl;
        if(res <= 0 ) {
            cout << "length reading failed"<< endl;
            st_thread->flag = true;
            return 0;
        }
        message = message + readLength;
        i = i + res;
        if(i == BL){
            break;
        }
        cout<<"3"<<endl;
        //delete readLength;
        readLength = new char[BL-i];
        memset(readLength, 0, BL-i);
    }
    strcpy(length, message.c_str());
    cout<<"liczba"<<length<<endl;
    return atoi(length);
}

string readMessage(structDataThread *st_thread){
    int len = readLength(st_thread);
    if (len < 1){
        cout <<"Reading message failed lenght unknown"<<endl;
        return "";
    }
    char* mess = new char[len];
    memset(mess, 0, len);
    int res;
    string message = "";
    
    for(int i = 0; i < len;) {
        res = read(st_thread -> client->descriptor, mess, len-i);

        if(res <= 0) {
            cout << "Reading message failed" << endl;
            st_thread->flag = true;
            return 0;
        }
        i = i + res;
        message = message + mess;
        if(i == len){
            break;
        }
        //delete mess;
        mess = new char[len-i];
        memset(mess, 0, len-i);
    }
    //delete mess;
    return message;
}

string getLengthOfMessage(string txt) {
    string length;
    if(txt.size() < 10) {length = "000" + to_string(txt.length());}
    else if(txt.size() < 100) {length = "00" + to_string(txt.length());}
    else if(txt.size() < 1000) {length = "0" + to_string(txt.length());}
    else {length = to_string(txt.length());}
    return length;
}

void CheckNickname(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int flag=0;
    string ans;
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        cout<<"FOR"<<endl; 
        if((*st_thread->clients)[i].nickname == message /*&&(*st_thread->clients)[i].descriptor != without */) {	      cout<<"tutaj"<<endl;
              flag=1; 	
        }
        if(flag==1){
        break; 
    	}
    }
    if(flag==0){
    	ans="ok";
        for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].descriptor == desc){
        		(*st_thread->clients)[i].nickname=message;
        	} 	
        }
    }    
    else{
 	ans="no";   	
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

string Code(){
    int a=rand()%10;
    int b=rand()%10;
    string a1=to_string(a);
    string b1=to_string(b);
    string code=a1+b1;
    return code;
}
 
void CreateRdyQuiz(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    cout<<"Recived message:"<<message<<endl;
    string ans;
    vector<Question> questions;
    int desc = st_thread->client->descriptor;
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }    
    cout<<nickname<<endl;
    string title;
    string code;
    int flag;
    int room=0;
    if(message=="mu"){
    	title="Muzyka";
    	questions=fetchQuiz("muzyka");
    }
    else if(message=="sp"){
    	title="Sport";
    	questions=fetchQuiz("sport");
    }
    else if(message=="hi"){
    	title="Historia";
    	questions=fetchQuiz("historia");
    }
    else if(message=="ma"){
    	title="Matematyka";
    	questions=fetchQuiz("matma");
    }
    cout<<"1"<<endl;
    while(true){
    	code=Code();
    	flag=0;
    	for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == code){
        		flag=1;
        	} 
    	}  
    	if(flag==0){
    		break;
    	}
    }
    while(true){
    	flag=0;
    	room++;
    	for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].room == room){
        		flag=1;
        	} 
    	}  
    	if(flag==0){
    		break;
    	}
    }
    vector<string> participants;
    struct Lobby lobby;
    lobby.room=room;
    lobby.name=title;
    lobby.lobby_id=code;
    lobby.owner=nickname;
    lobby.participants=participants;
    lobby.questions=questions;
    lobby.start=0;
    lobby.activeque="no";
    st_thread->lobbys->push_back(lobby);
    
    //cout<<"Sent response: "<<answer<<endl;
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        cout<<"FOR"<<endl; 
        if((*st_thread->clients)[i].descriptor == desc){
        		(*st_thread->clients)[i].room=room;
        	} 
    }    
    cout<<nickname<<endl;
    ans=title+";"+nickname+";"+code;
    cout<<"tu "<<ans<<endl;
    if(getLengthOfMessage(ans)=="0020"){
    	ans=ans+"&";
    }
    string answer = getLengthOfMessage(ans) + ans; 
    cout<<"tu "<<answer<<endl;
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
    cout<<"WYKONUJE"<<endl;
}

void CreateOwnQuizTitle(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    cout<<"Recived message:"<<message<<endl;
    string ans;
    int desc = st_thread->client->descriptor;
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }    
    string code;
    int flag;
    int room=0;
    while(true){
    	code=Code();
    	flag=0;
    	for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == code){
        		flag=1;
        	} 
    	}  
    	if(flag==0){
    		break;
    	}
    }
    while(true){
    	flag=0;
    	room++;
    	for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].room == room){
        		flag=1;
        	} 
    	}  
    	if(flag==0){
    		break;
    	}
    }
    vector<string> participants;
    vector<Question> questions;
    struct Lobby lobby;
    lobby.room=room;
    lobby.name=message;
    lobby.lobby_id=code;
    lobby.owner=nickname;
    lobby.participants=participants;
    lobby.questions=questions;
    lobby.start=0;
    lobby.activeque="no";
    st_thread->lobbys->push_back(lobby);
    
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        cout<<"FOR"<<endl; 
        if((*st_thread->clients)[i].descriptor == desc){
        		(*st_thread->clients)[i].room=room;
        	} 
    }    
    ans=code;
    if(getLengthOfMessage(ans)=="0020"){
    	ans=ans+"&";
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void CreateOwnQuizQue(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    vector<string> question;
    string rob;
     int j=0;
    for(int i=0; i<message.size();i++){
		if(message[i]==';'){
			rob=message.substr(j,i-j);
			j=i+1;
			question.push_back(rob);	
		}
	}
    question.push_back(message.substr(j,message.size()-j));
    struct Question newque;
    newque.number=question[1];
    newque.question=question[2];
    newque.correctans=question[3];
    newque.ans1=question[4];
    newque.ans2=question[5];
    newque.ans3=question[6];
    cout<<"Recived message:"<<message<<endl;
    string ans;
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == question[0]){
        		(*st_thread->lobbys)[i].questions.push_back(newque);
        	} 
    	}      
    //cout<<"Sent response: "<<answer<<endl;
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    ans="ok";
    if(getLengthOfMessage(ans)=="0020"){
    	ans=ans+"&";
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}
   
void CreateOwnQuizEnd(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    vector<string> question;
    string rob;
    string owner;
    string title;
     int j=0;
    for(int i=0; i<message.size();i++){
		if(message[i]==';'){
			rob=message.substr(j,i-j);
			j=i+1;
			question.push_back(rob);	
		}
	}
    question.push_back(message.substr(j,message.size()-j));
    struct Question newque;
    newque.number=question[1];
    newque.question=question[2];
    newque.correctans=question[3];
    newque.ans1=question[4];
    newque.ans2=question[5];
    newque.ans3=question[6];
    cout<<"Recived message:"<<message<<endl;
    string ans;
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == question[0]){
        		(*st_thread->lobbys)[i].questions.push_back(newque);
        		owner=(*st_thread->lobbys)[i].owner;
        		title=(*st_thread->lobbys)[i].name;
        	} 
    	}      
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    ans=title+";"+owner+";"+question[0];
    if(getLengthOfMessage(ans)=="0020"){
    	ans=ans+"&";
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void JoinLobby(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    string owner;
    string title;
    int flag=0;
    cout<<"Recived message:"<<message<<endl;
    string ans;
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }    
    cout<<nickname<<endl;
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		(*st_thread->lobbys)[i].participants.push_back(nickname);
        		owner=(*st_thread->lobbys)[i].owner;
        		title=(*st_thread->lobbys)[i].name;
        		room=(*st_thread->lobbys)[i].room;
        		flag=1;
        	} 
    	}      
    cout<<(*st_thread->lobbys)[0].questions[0].question<<endl;
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    if(flag==1){
    	ans=title+";"+owner;
    	for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	(*st_thread->clients)[i].room=room;
    	}
    }  
    }
    else{
    	ans="no";
    }
    if(getLengthOfMessage(ans)=="0020"){
    	ans=ans+"&";
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void CheckStatus(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans="no";   
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		if((*st_thread->lobbys)[i].start==0){
        			ans="";
				int a=(*st_thread->lobbys)[i].participants.size();
				for(int j=0;j<a;j++){
					if(j!=a-1){
						ans+=(*st_thread->lobbys)[i].participants[j];
						ans+=";";
						cout<<(*st_thread->lobbys)[i].participants[j]<<endl;
					}
					else{
						ans+=(*st_thread->lobbys)[i].participants[j];
						cout<<(*st_thread->lobbys)[i].participants[j]<<endl;
					}
				}
			}
			else{
				ans="start";
			}
        	} 
    	}      
    cout<<(*st_thread->lobbys)[0].questions[0].question<<endl;
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    if(getLengthOfMessage(ans)=="0020"){
    	ans=ans+"&";
    }
    if(getLengthOfMessage(ans)=="0000"){
    	ans="&";
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void StartQuiz(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int numque;
    string quenum;
    string que;
    string anscor;
    string ans1;
    string ans2;
    string ans3;
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		(*st_thread->lobbys)[i].start=1;
        		(*st_thread->lobbys)[i].activeque="yes";
        		numque=(*st_thread->lobbys)[i].questions.size();
        		quenum=(*st_thread->lobbys)[i].questions[0].number;
        		que=(*st_thread->lobbys)[i].questions[0].question;
        		anscor=(*st_thread->lobbys)[i].questions[0].correctans;
        		ans1=(*st_thread->lobbys)[i].questions[0].ans1;
        		ans2=(*st_thread->lobbys)[i].questions[0].ans2;
        		ans3=(*st_thread->lobbys)[i].questions[0].ans3;
        	} 
    	}      
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl; 
    string abc=to_string(numque);  
    ans=abc+";"+quenum+";"+que+";"+anscor+";"+ans1+";"+ans2+";"+ans3;
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void CorrectAnswer(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }   
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].owner == nickname){
        		cout<<"JESTEM"<<endl;
        		(*st_thread->lobbys)[i].correctans=message;
        	} 
    	}      
    cout<<"room descriptor: "<<room<<endl; 
}

void PlayerAnswer(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int roz=message.size();
    string answer=message.substr(0,1);
    cout<<answer<<endl;
    string points=message.substr(1,roz-1);
    cout<<points<<endl;
    int waitp=stoi(points);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].descriptor == desc){
        		(*st_thread->clients)[i].answer=answer;
        		(*st_thread->clients)[i].waitpoints=waitp;
        	} 
    	}      
    cout<<"room descriptor: "<<room<<endl; 
}

void GetTable(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int best1=-1;
    int best2=-1;
    int best3=-1;
    int best4=-1;
    int best5=-1;
    string answercor;
    int waitpoints;
    string nick1;
    string nick2;
    string nick3;
    string nick4;
    string nick5;
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }   
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		(*st_thread->lobbys)[i].activeque="no";
        		room=(*st_thread->lobbys)[i].room;
        		answercor=(*st_thread->lobbys)[i].correctans;
        		cout<<answercor<<endl;
        	} 
    	}   
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].room == room){
        		waitpoints=(*st_thread->clients)[i].waitpoints;
        		cout<<"wait"+to_string(waitpoints)<<endl;
        		cout<<(*st_thread->clients)[i].answer<<endl;
        		if((*st_thread->clients)[i].answer==answercor){
        			(*st_thread->clients)[i].points+=waitpoints;
        		}
        	} 
    	}	
      for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].room == room && (*st_thread->clients)[i].nickname!=nickname){
        		if((*st_thread->clients)[i].points>best1 && best1<=best2 && best1<=best3 && best1<=best4 && best1<=best5){
        			best1=(*st_thread->clients)[i].points;
        			nick1=(*st_thread->clients)[i].nickname;
        		}
        		else if((*st_thread->clients)[i].points>best2 && best2<=best1 && best2<=best3 && best2<=best4 && best2<=best5){
        			best2=(*st_thread->clients)[i].points;
        			nick2=(*st_thread->clients)[i].nickname;
        		}
        		else if((*st_thread->clients)[i].points>best3 && best3<=best1 && best3<=best2 && best3<=best4 && best3<=best5){
        			best3=(*st_thread->clients)[i].points;
        			nick3=(*st_thread->clients)[i].nickname;
        		}
        		else if((*st_thread->clients)[i].points>best4 && best4<=best1 && best4<=best2 && best4<=best3 && best4<=best5){
        			best4=(*st_thread->clients)[i].points;
        			nick4=(*st_thread->clients)[i].nickname;
        		}
        		else if((*st_thread->clients)[i].points>best5 && best5<=best1 && best5<=best2 && best5<=best3 && best5<=best4){
        			best5=(*st_thread->clients)[i].points;
        			nick5=(*st_thread->clients)[i].nickname;
        		}
        	} 
    	}
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl; 
    ans=to_string(best1)+";"+nick1;
    if(best2>-1){
    	ans+="^"+to_string(best2)+";"+nick2;
    	if(best3>-1){
    		ans+="^"+to_string(best3)+";"+nick3;
    		if(best4>-1){
    			ans+="^"+to_string(best4)+";"+nick4;
    			if(best5>-1){
    				ans+="^"+to_string(best5)+";"+nick5;
    			}
    		}
    	}
    }
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void PlayerScore(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int place=1;
    int score;
    string correct;	
    string mycor;
    string myans;
    string owner;
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		correct=(*st_thread->lobbys)[i].correctans;
        		owner=(*st_thread->lobbys)[i].owner;
        	} 
    	}      
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].descriptor == desc){
        		myans=(*st_thread->clients)[i].answer;
        		score=(*st_thread->clients)[i].points;
        		room=(*st_thread->clients)[i].room;
        	} 
    	}
    if(correct==myans){
    	mycor="1";
    }
    else{
    	mycor="0";
    }
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].room == room && (*st_thread->clients)[i].nickname!=owner){
        		if(score<(*st_thread->clients)[i].points){
        			place+=1;
        		}
        	} 
    	}
    
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    ans=mycor+";"+to_string(place)+";"+to_string(score);
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void GetNextQue(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    string id=message.substr(0,2);
    int x=message.size();
    string numque=message.substr(2,x-2);
    int n=stoi(numque);
    string quenum;
    string que;
    string anscor;
    string ans1;
    string ans2;
    string ans3;
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == id){
        		(*st_thread->lobbys)[i].activeque="yes";
        		quenum=(*st_thread->lobbys)[i].questions[n].number;
        		que=(*st_thread->lobbys)[i].questions[n].question;
        		anscor=(*st_thread->lobbys)[i].questions[n].correctans;
        		ans1=(*st_thread->lobbys)[i].questions[n].ans1;
        		ans2=(*st_thread->lobbys)[i].questions[n].ans2;
        		ans3=(*st_thread->lobbys)[i].questions[n].ans3;
        	} 
    	}      
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    ans=quenum+";"+que+";"+anscor+";"+ans1+";"+ans2+";"+ans3;
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       cout<<"1"<<endl;
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
       cout<<"2"<<endl;
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void QuestionStatus(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;
    string ans;   
    ans="end";
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		ans=(*st_thread->lobbys)[i].activeque;
        	} 
    	}      
    cout<<"room descriptor: "<<room<<endl; 
    pthread_mutex_lock(&st_thread->rMutex[room]);
    cout<<"(int)st_thread->clients->size()"<<(int)st_thread->clients->size()<<endl;   
    string answer = getLengthOfMessage(ans) + ans; 
    char mess[answer.size()];
    strcpy(mess, answer.c_str());
    char* writeMessage = new char[answer.size()];
    strcpy(writeMessage, answer.c_str());
    
    int lengthAnswer = answer.size();
    int res;
    cout<<"Sent message to descriptor:"<<desc<<endl;
    for(int k=0; k < lengthAnswer;) {
    	res = write((*st_thread->client).descriptor, writeMessage, lengthAnswer - k);
        cout<< "Sent bytes:"<<res<<endl;
        if(res <= 0) {
        	cout << "Sent message fail in send message to others clients in the same room" << endl;
                st_thread->flag = true;
                return;
       }
       cout<<"1"<<endl;
       k += res;
       if(k == lengthAnswer){
                break;
       } 
       // delete writeMessage;
       writeMessage = new char[lengthAnswer - k];
       memcpy(writeMessage, mess + k, lengthAnswer - k);
       cout<<"2"<<endl;
    }
   // delete writeMessage;
    writeMessage = new char[lengthAnswer];
    strcpy(writeMessage, answer.c_str());   
    cout << "Client with descriptor " << st_thread -> client->descriptor << " send message to room: " << room << endl;
    pthread_mutex_unlock(&st_thread->rMutex[room]);
}

void EndQuiz(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    int ind;
    cout<<"Recived message:"<<message<<endl;  
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }    
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		room=(*st_thread->lobbys)[i].room;
        	} 
    	}  
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
        	if((*st_thread->clients)[i].room == room){
        		(*st_thread->clients)[i].points=0;
        		(*st_thread->clients)[i].room=0;
        	} 
    	}  
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
    	if((*st_thread->lobbys)[i].owner == nickname){
        	ind=i;
        } 	
    }
    st_thread->lobbys->erase(st_thread->lobbys->begin()+ind);
    cout<<"room descriptor: "<<room<<endl; 
}

void LeaveLobby(structDataThread *st_thread, int without) {
    string message = readMessage(st_thread);
    int desc = st_thread->client->descriptor;
    int room = st_thread->client->room;
    cout<<"Recived message:"<<message<<endl;  
    string nickname;
    for(int i=0; i< (int)st_thread->clients->size(); i++) {
    	if((*st_thread->clients)[i].descriptor == desc){
       	nickname=(*st_thread->clients)[i].nickname;
    	}
    }    
    for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        	if((*st_thread->lobbys)[i].lobby_id == message){
        		for(int j=0; j< (int)(*st_thread->lobbys)[i].participants.size(); j++){
        				if((*st_thread->lobbys)[i].participants[j]==nickname){
        					(*st_thread->lobbys)[i].participants.erase((*st_thread->lobbys)[i].participants.begin()+j);
        				}
        			}
        	} 
    	}  
    cout<<"room descriptor: "<<room<<endl; 
}
   

void *StartNewThread(void *t_data) {
    pthread_detach(pthread_self());
    struct structDataThread *st_thread = (struct structDataThread*) t_data;
    int correctAction;
    struct Client client;
    client.descriptor = st_thread -> client->descriptor;
    client.name = st_thread -> client->name;
    client.nickname = st_thread -> client->nickname;
    client.room = 0;
    client.points=st_thread -> client->points;
    client.answer=st_thread -> client->answer;
    client.waitpoints=st_thread -> client->waitpoints;
    st_thread->clients->push_back(client);

    char action[1];
    cout<<"Waiting on message on descriptor: "<<st_thread -> client->descriptor<<endl;
    while(true) {
        correctAction = read(st_thread -> client->descriptor, action, sizeof(action)); 
        if(correctAction == -1) {
            cout << "Reading action failed" << endl;
            break;
        }
        
        if (correctAction > 0) {
        	string action1(action,1);
            if(action1 == "0") { 
            	int ind;
            	int ind1=-1;
            	int room=-1;
            	int room1;
            	string nickname;
            	for(int i=0; i< (int)st_thread->clients->size(); i++) {
        		if((*st_thread->clients)[i].descriptor == client.descriptor){
        			nickname=(*st_thread->clients)[i].nickname;
        			room1=(*st_thread->clients)[i].room;
        			ind=i;
        		} 	
        	}
        	for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
        		if((*st_thread->lobbys)[i].owner == nickname){
        			room=(*st_thread->lobbys)[i].room;
        		} 
    		}
    		for(int i=0; i< (int)st_thread->clients->size(); i++) {
        		if((*st_thread->clients)[i].room == room){
        			(*st_thread->clients)[i].points=0;
        			(*st_thread->clients)[i].room=0;
        		} 
    		}  	
    		for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
    			if((*st_thread->lobbys)[i].owner == nickname){
        			ind1=i;
        		} 	
    		}
    		if(room1>0){
    			for(int i=0; i< (int)st_thread->lobbys->size(); i++) {
    			if((*st_thread->lobbys)[i].room == room1){
        			for(int j=0; j< (int)(*st_thread->lobbys)[i].participants.size(); j++){
        				if((*st_thread->lobbys)[i].participants[j]==nickname){
        					(*st_thread->lobbys)[i].participants.erase((*st_thread->lobbys)[i].participants.begin()+j);
        				}
        			}
        		} 	
    		}
    		}
    		st_thread->clients->erase(st_thread->clients->begin()+ind);
    		if(ind1!=-1){
    			st_thread->lobbys->erase(st_thread->lobbys->begin()+ind1);  
            	}
            }
            
            if(action1 == "1") { 
                cout<<"action:"<<action<<endl;
                CheckNickname(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "2") { 
                cout<<"action:"<<action<<endl;
                CreateRdyQuiz(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "3") { 
                cout<<"action:"<<action<<endl;
                CreateOwnQuizTitle(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "4") { 
                cout<<"action:"<<action<<endl;
                CreateOwnQuizQue(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "5") { 
                cout<<"action:"<<action<<endl;
                CreateOwnQuizEnd(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;

            }
            
            else if(action1 == "6") { 
                cout<<"action:"<<action<<endl;
                JoinLobby(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "7") {
                cout<<"action:"<<action<<endl;
                CheckStatus(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "8") {
                cout<<"action:"<<action<<endl;
                StartQuiz(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }

            else if(action1 == "9") { 
                cout<<"action:"<<action<<endl;
                CorrectAnswer(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "a") {
                cout<<"action:"<<action<<endl;
                PlayerAnswer(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "b") { 
                cout<<"action:"<<action<<endl;
                GetTable(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "c") { 
                cout<<"action:"<<action<<endl;
                PlayerScore(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "d") { 
                cout<<"action:"<<action<<endl;
                GetNextQue(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "e") {
                cout<<"action:"<<action<<endl;
                QuestionStatus(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "f") { 
                cout<<"action:"<<action<<endl;
                EndQuiz(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }
            
            else if(action1 == "g") { 
                cout<<"action:"<<action<<endl;
                LeaveLobby(st_thread, st_thread->client->descriptor);
                cout<<"Recieved message on descriptor: "<<st_thread->client->descriptor<<endl;
            }

        }
        memset(action, 0, sizeof(action));
    }
    cout << "Close client with descriptor " << st_thread -> client->descriptor << endl;
    close(st_thread -> client->descriptor);
    delete st_thread;
    cout << "After thread"<<endl;
    pthread_exit(NULL);
    
}


void acceptConn(int newConnDescriptor, vector<Client>* clients, vector<Lobby>* lobbys, pthread_mutex_t* cMutex, pthread_mutex_t* rMutex) {
    struct structDataThread *newThreadData = new structDataThread();
    newThreadData->client = new Client();
    newThreadData->client->descriptor =  newConnDescriptor;
    newThreadData->flag = false;
    newThreadData->cMutex = cMutex;
    newThreadData->client->room = 0;
    newThreadData->client->points = 0;
    newThreadData->client->answer = "";
    newThreadData->client->waitpoints = 0;
    newThreadData->client->nickname = "";
    newThreadData->clients = clients;
    newThreadData->lobbys = lobbys;

    pthread_t threadConn;
    for(int i=0; i<20; i++) {
        newThreadData->rMutex[i] = rMutex[i];
    }
    if (pthread_create(&threadConn, NULL, StartNewThread, (void*) newThreadData) != 0) {
        cout << "Creating new thread failed\n";
        exit(-1);
    }
}


int main(int argc, char* argv[]) {
    string line,a,b;
    int backlog = 5;
    int SDserver;
    int SDconnection;
    int option_value = 1;
    struct sockaddr_in sa;
    
    pthread_mutex_t rMutex[20] = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, PTHREAD_MUTEX_INITIALIZER};
    pthread_mutex_t cMutex = PTHREAD_MUTEX_INITIALIZER;
    vector<Client> clients; 
    vector<Lobby> lobbys; 
    if (argc < 2) {
        cout << "Invalid argv, port number not passed\n";
        return 0;
    }
    
    memset(&sa, 0, sizeof(struct sockaddr));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(INADDR_ANY);
    sa.sin_port = htons(atoi(argv[1]));

    SDserver = socket(AF_INET, SOCK_STREAM, 0);

    if (SDserver < 0) {
        cout << "Creating socket server failed\n";
        return 0;
    }
    //aaby port mogl byc szybciej uzyty jeszcze raz
    setsockopt(SDserver, SOL_SOCKET, SO_REUSEADDR, (char*) &option_value, sizeof(option_value));

    if (bind(SDserver, (struct sockaddr*) &sa, sizeof(struct sockaddr)) < 0) {
        cout << "Bind adress with socket server failed\n";
        return 0;
    }

    
    if (listen(SDserver, backlog) < 0) {
        cout << "Set size queue failed\n";
        return 0;
    }
    
    while(1) {
        SDconnection = accept(SDserver, NULL, NULL);

        if (SDconnection < 0) {
            cout << "Creating client-server connection failed\n";
            return 0;
        }
        acceptConn(SDconnection, &clients, &lobbys, &cMutex, rMutex);
    }
    close(SDserver);
    return 0;
}
