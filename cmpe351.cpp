/* This is code developed by <Jaciara Faif> */
#include <iostream>
#include <cstdlib>
#include <string.h>
#include <iomanip>
#include <fstream>
using namespace std;
struct Process{
    int ProcessID,burstTime,arrivalTime,Priority,waitingTime,Remaing_time;
};
struct ProcessResults{
        int ProcessID,waitingTime;
};
struct node
{
	Process processInfo;
	node* next;
};
struct ResultsNode
{
	ProcessResults  resultsInfo;
	ResultsNode* next;
};
/*Create Node*/
struct node* createNode(Process item){
	struct node *temp;
	temp = new node;
	temp->processInfo = item;
	temp ->next = NULL;
	return temp;
}
/*Create Node for results*/
struct ResultsNode* createResultsNode(int processID,int waitingTime){
	struct ResultsNode* temp = new ResultsNode;
	temp->resultsInfo.ProcessID=processID;
	temp->resultsInfo.waitingTime= waitingTime;
	temp->next=NULL;
	return temp;
}

/*InsertBack Function*/
struct node *insertBack(struct node *header,Process item){
	struct node *temp = createNode(item);
	struct node *headerTemp;
	if(header == NULL){
		header = temp;
		return header;
	}
	headerTemp = header;
	while(headerTemp ->next !=NULL){
		headerTemp = headerTemp-> next;
	}
	headerTemp->next = temp;
	return header;
}
/*Insert Back results*/
void insertResultBack(ResultsNode** header,int processID,int waitingTime){
	struct ResultsNode* temp = createResultsNode(processID,waitingTime);
	struct ResultsNode * headerTemp;
	if(temp ==NULL) return;
	if(*header ==NULL){
		*header= temp;
		return;
	}
	headerTemp=*header;
	while(headerTemp -> next !=NULL){
		headerTemp= headerTemp -> next;
	}
	headerTemp->next=temp;
}
/*Insert back for existing nodes*/
void insertBackRR(struct node** header , struct node* newHeader){
	if(*header == NULL){
		*header = newHeader;
	}
	else{
		struct node* currentNode= *header;
		while(currentNode->next !=NULL){
			currentNode = currentNode->next;
		}
		currentNode->next = newHeader;
	}
}

/*Read from file and insert the values to the nodes*/
struct node* readFile( struct node* header ,string input){
	ifstream File(input);
	if(!File.is_open()){
		cout<<"File could not open "<<endl;
	}
	string row;
	int numberOfRow=0;
	while(getline(File,row)){
		numberOfRow++;
		
		Process item;
		string burstTime;
		string arrivalTime;
		string Priority;
		
		int i =0;
                while(i < row.size() && row[i] != ':'){
			burstTime += row[i];
			i++;
		}
		i=i+1;
                while(i < row.size() && row[i] != ':'){
			arrivalTime= arrivalTime +row[i];
			i++;
		}
		i++;
                while(i < row.size() && row[i] != ':'){
			Priority = Priority +row[i];
			i++;
		}
		item.ProcessID = numberOfRow;
		item.arrivalTime=stoi(arrivalTime);
		item.burstTime = stoi(burstTime);
		item.Priority = stoi(Priority);
		item.Remaing_time =stoi(burstTime);
                item.waitingTime =0;
		header = insertBack(header,item);
	}
	File.close();
	return header;
}

/*count processes Function*/
int count(struct node *header){
	struct node *temp = header;
	int count = 0;
	while (temp !=NULL){
           count++;
           temp= temp -> next;
	}
	return count;
}
/*Count Results Function*/
int countResults(struct ResultsNode* header){
	int count =0;
	struct ResultsNode* temp= header;
	while(temp !=NULL){
		count++;
		temp= temp->next;
	}
	return count;
}
/*Delete List*/
void freeProcess(struct node** header){
	if(header == NULL || *header ==NULL) return;
	struct node* temp= *header;
	while(temp != NULL){
		struct node* headerTemp = temp->next;
		delete temp;
		temp=headerTemp;
	}
	*header = NULL;
}
/*Removehead who call this functions needs to remove the first node*/
struct node* removeHead(struct node** header){
	if(*header==NULL){
		return NULL;
	}
	struct node* removedHead=*header;
	*header = (*header)->next;
	removedHead->next = NULL;
	return removedHead;
}
/*Function Free Results*/
void freeResults(ResultsNode** header){
	if(header ==NULL || *header ==NULL) return;
	struct ResultsNode* temp = *header ;
	while(temp != NULL){
		struct ResultsNode* headerTemp = temp->next;
		delete temp;
		temp=headerTemp;
	}
	*header=NULL;
}
/*A Coppy of the linked list*/
struct node* copyList(node* header){
	if(header == NULL){
		return NULL;
	}
	node * newHeader = new node;
	newHeader ->processInfo = header -> processInfo;
	newHeader ->next = NULL;
	struct node* originalNode= header -> next;
	node* newNode= newHeader;
	while(originalNode!= NULL){
		node* copyValue = new node;
		copyValue -> processInfo = originalNode -> processInfo;
		copyValue-> next = NULL;
		newNode->next = copyValue;
		newNode= copyValue;
		originalNode= originalNode ->next;
	}
	return newHeader;
}

/*Orderd by arrivalTime*/
struct node* arrival(struct node* header){
	struct node* arrivalHeader=copyList(header);
	int numberOfProcess=count(arrivalHeader);
	if(arrivalHeader==NULL){
		return arrivalHeader;
	}else{
	    for(int i =0; i<numberOfProcess ;i++){
		    struct node* temp=arrivalHeader;
			for(int j = 0; j<numberOfProcess -i-1;j++){
				if(temp->processInfo.arrivalTime > temp->next->processInfo.arrivalTime){
					struct Process tempProcess = temp->processInfo;
					temp->processInfo = temp->next->processInfo;
					temp->next->processInfo = tempProcess;
				}
				temp= temp->next;
			}
		}
	}
	return arrivalHeader;
}

/*priority queue*/
void insertPriorityOrdered(struct node** header, struct node* newPriority){
	if(*header == NULL){
		*header = newPriority;
		return;
	}
	if(newPriority->processInfo.Priority < (*header)->processInfo.Priority){
		newPriority->next=*header;
		*header= newPriority;
		return;
	}
	struct node* previousPriority = *header;
	struct node* currentPriority = previousPriority->next;
	while(currentPriority != NULL && currentPriority->processInfo.Priority < newPriority->processInfo.Priority){
		previousPriority=currentPriority;
		currentPriority=currentPriority->next;
	}
	previousPriority->next=newPriority;
	newPriority ->next=currentPriority;
}

/*Orederd by Remaing time */
void insertRemaingOrdered(struct node** header, struct node* newRemaingNode){
	if(*header == NULL){
		*header = newRemaingNode;
		return;
	}
	if(newRemaingNode->processInfo.Remaing_time < (*header)->processInfo.Remaing_time){
		newRemaingNode->next=*header;
		*header= newRemaingNode;
		return;
	}
	struct node* previousRemaingNode = *header;
	struct node* currentRemaingNode = previousRemaingNode->next;
	while(currentRemaingNode != NULL && currentRemaingNode->processInfo.Remaing_time < newRemaingNode->processInfo.Remaing_time){
		previousRemaingNode=currentRemaingNode;
		currentRemaingNode=currentRemaingNode ->next;
	}
	previousRemaingNode->next=newRemaingNode;
	newRemaingNode ->next=currentRemaingNode;
       struct node* temp = *header;
       while (temp != NULL) {
         temp = temp->next;
      }
      cout << endl;
}

/*Orederd by burst time */
void insertBurstOrdered(struct node** header, struct node* newBurstNode){
	if(*header == NULL){
		*header = newBurstNode;
		return;
	}
	if(newBurstNode->processInfo.burstTime < (*header)->processInfo.burstTime){
		newBurstNode->next=*header;
		*header= newBurstNode;
		return;
	}
	struct node* previousBurstNode = *header;
	struct node* currentBurstNode = previousBurstNode->next;
	while(currentBurstNode != NULL && currentBurstNode->processInfo.burstTime < newBurstNode->processInfo.burstTime){
		previousBurstNode=currentBurstNode;
		currentBurstNode =currentBurstNode->next;
	}
	previousBurstNode->next=newBurstNode;
	newBurstNode ->next=currentBurstNode;
}

/*ordering results by id*/
struct ResultsNode* processidOrder(struct ResultsNode* header){
	int numberOfProcess=countResults(header);
	struct ResultsNode* processIDHeader=header;
	if(processIDHeader==NULL){
		return processIDHeader;
	}else{
	    for(int i =0; i<numberOfProcess ;i++){
		    struct ResultsNode* temp=processIDHeader;
			for(int j = 0; j<numberOfProcess -i-1;j++){
				if(temp->resultsInfo.ProcessID > temp->next->resultsInfo.ProcessID){
					struct ProcessResults tempResults = temp->resultsInfo;
					temp->resultsInfo = temp->next->resultsInfo;
					temp->next->resultsInfo = tempResults;
				}
				temp= temp->next;
			}
		}
	}
	return processIDHeader;
}
/*Move to ready queue*/

void moveListFCFS(struct node** notReady, struct node** ready , int time){
	while(*notReady != NULL && (*notReady)->processInfo.arrivalTime <= time){
		struct node* storeNode = * notReady;
                *notReady= (*notReady)->next;
		storeNode -> next =NULL;
		insertBackRR(ready,storeNode);
		
	}
	
}
/*Move Priority preemptive*/
void moveListPriority_Preemptive(struct node** Ready,struct node** notReady,int time){
    while(*notReady != NULL && (*notReady)->processInfo.arrivalTime <= time){
    	struct node* movedNode = *notReady;
    	*notReady = (*notReady)->next;
    	movedNode->next=NULL;
    	insertPriorityOrdered(Ready,movedNode);	
	}	
}
/*Move to ready queue RR*/
void moveListRR(struct node** Ready,struct node ** notReady,int time){
	while(*notReady !=NULL && (*notReady)->processInfo.arrivalTime <=time){
		struct node *movedNode=*notReady;
		*notReady = (*notReady)->next;
		movedNode->next =NULL;
		insertBackRR(Ready,movedNode);
		
	}
}

/*Move Priority*/
void moveListPriority(struct node** Ready,struct node** notReady,int time){
	while(*notReady !=NULL && (*notReady)->processInfo.arrivalTime <= time){
		struct node* movedNode=*notReady;
		*notReady = (*notReady)->next;
		movedNode->next = NULL;
		insertPriorityOrdered(Ready,movedNode);
	}
}

/* Move SJF Preemptive*/
void moveListSJF_Preemptive(struct node** Ready,struct node** notReady,int time){
    while(*notReady != NULL && (*notReady)->processInfo.arrivalTime <= time){
    	struct node* movedNode = *notReady;
    	*notReady = (*notReady)->next;
       	movedNode->next=NULL;
        insertRemaingOrdered(Ready,movedNode);
     }
}
/*Calculate wating time sum*/
int sumResults(ResultsNode* header){
	int Sum = 0;
	struct ResultsNode * temp = header;

	while(temp != NULL){
		Sum += temp ->resultsInfo.waitingTime;
		temp=temp ->next;
	}
	return Sum;
}

/*Print results Function*/
void printResults(int algorithmID , struct ResultsNode* header ,double avg,string output){
	ofstream file;
	file.open(output,ofstream::app);
    if(!file.is_open()){
		cout<<"File could not open outputfile "<<endl;
		exit(1);
	}
	file<<algorithmID;
	cout<<algorithmID;
	struct ResultsNode* temp = header;
	while(temp != NULL){
		file<<":"<<temp->resultsInfo.waitingTime;
		cout<<":"<<temp->resultsInfo.waitingTime;
		temp = temp->next;
	}
	file<<":"<<fixed<<setprecision(2)<<avg<<endl;
	cout<<":"<<fixed<<setprecision(2)<<avg<<endl;
	file.close();
	
}

/*Move SJF*/
void moveListSJF(struct node** Ready,struct node** notReady,int time){
	while(*notReady !=NULL && (*notReady)->processInfo.arrivalTime <= time){
		struct node* movedNode=*notReady;
		*notReady = (*notReady)->next;
		movedNode->next = NULL;
		insertBurstOrdered(Ready,movedNode);
        }
}
/*FCFS*/
void FCFS(struct node* header, string output){
	struct node* notReady = arrival(header);
	struct node * Ready =NULL;
	struct ResultsNode* results = NULL;
	int time= 0;
	int finishedProcesses=0;
	int numberOfProcess = count(header);
	while(finishedProcesses < numberOfProcess){
		moveListFCFS(&notReady,&Ready,time);
		if(Ready != NULL){
			struct node* processExecution = removeHead(&Ready);
			int waitingTime = time- processExecution -> processInfo.arrivalTime;
			insertResultBack(&results,processExecution->processInfo.ProcessID,waitingTime);
			time= time + processExecution ->processInfo.burstTime;
			finishedProcesses ++;
			delete processExecution;
			processExecution = NULL;
		}
		else if(notReady !=NULL){
			time = notReady->processInfo.arrivalTime;
		}	
	}
	results=processidOrder(results);
	int sum = sumResults(results);
	double avg = (double)sum/numberOfProcess;
	printResults(1,results,avg,output);
	freeProcess(&notReady);
	freeProcess(&Ready);
	freeResults(&results);	
}
/*SJF*/
void SJF(struct node* header,string output){
	struct node* notReady = arrival(header);
	struct node* Ready=NULL;
	struct ResultsNode* results =NULL;
	int time=0;
	int finishedProcesses=0;
	int numberOfProcesses = count(header);
	while(finishedProcesses < numberOfProcesses){
		moveListSJF(&Ready,&notReady,time);
		if(Ready != NULL){
			struct node* processExecution = removeHead(&Ready);
			int waitingTime= time - processExecution->processInfo.arrivalTime;
			insertResultBack(&results,processExecution->processInfo.ProcessID,waitingTime);
			time= time +processExecution->processInfo.burstTime;
			finishedProcesses ++;
			delete processExecution;
			processExecution= NULL;
		}else if(notReady !=NULL){
			time=notReady->processInfo.arrivalTime;
		}
	}
	results=processidOrder(results);
	int sum = sumResults(results);
	double avg = (double)sum/numberOfProcesses;
	printResults(2,results,avg,output);
	freeProcess(&notReady);
	freeProcess(&Ready);
	freeResults(&results);	
}


/*SJF preemptive*/
void SJF_Preemptive(struct node* header,string output){
	struct node* notReady = arrival(header);
	struct node* Ready = NULL;
	struct ResultsNode* results =NULL;
	struct node* processExecution =NULL;
	int time =0;
	int finishedProcesses=0;
	int numberOfProcess= count(header);
	while(finishedProcesses < numberOfProcess){
		moveListSJF_Preemptive(&Ready,&notReady,time);
		if(processExecution != NULL && Ready != NULL){
			if(Ready->processInfo.Remaing_time < processExecution->processInfo.Remaing_time){
			    insertRemaingOrdered(&Ready,processExecution);
				processExecution =NULL;	
			}
		}
		if(processExecution == NULL && Ready!=NULL){
			processExecution = removeHead(&Ready);
		}
		if(processExecution !=NULL){
			int executionTime = 1;
			time = time + executionTime;
            processExecution->processInfo.Remaing_time = processExecution->processInfo.Remaing_time - executionTime;
		    if(processExecution->processInfo.Remaing_time==0){
			    int waitingTime = time - processExecution->processInfo.arrivalTime - processExecution->processInfo.burstTime;
			    insertResultBack(&results,processExecution->processInfo.ProcessID,waitingTime);
			    delete processExecution;
			    processExecution = NULL;
			    finishedProcesses ++; 
		    }
		}
		if(processExecution ==NULL && Ready==NULL&&notReady){
			time = notReady->processInfo.arrivalTime;
		}
	}
	results=processidOrder(results);
	int sum = sumResults(results);
	double avg = (double)sum/numberOfProcess;
	printResults(3,results,avg,output);
        if(processExecution != NULL) {
            delete processExecution;
            processExecution = NULL;
        }
	freeProcess(&notReady);
	freeProcess(&Ready);
	freeResults(&results);
	
}
/*Priority Non-preemptive*/
void Priority(struct node* header, string output){
        struct node* notReady = arrival(header);
        struct node* Ready = NULL;
        struct ResultsNode* results = NULL;
        int time = 0;
        int finishedProcesses = 0;
        int numberOfProcesses= count(header);

        while(finishedProcesses < numberOfProcesses){
                moveListPriority(&Ready, &notReady, time);

                if(Ready !=NULL){
                        struct node* processExecution = removeHead(&Ready);
                        int waitingTime =time- processExecution->processInfo.arrivalTime;
                        insertResultBack(&results, processExecution->processInfo.ProcessID, waitingTime);
                        time = time +processExecution->processInfo.burstTime;
                        finishedProcesses++;
                        delete processExecution;
                        processExecution = NULL;
                } else {
                        if(notReady != NULL) {
                                time = notReady->processInfo.arrivalTime;
                        }
                }
        }
        
        results =processidOrder(results);
        int sum =sumResults(results);
        double avg =(double)sum / numberOfProcesses;
        printResults(4,results,avg,output);
        freeProcess(&notReady);
        freeProcess(&Ready);
        freeResults(&results);
}
/*Priority Preemptive*/
void Priority_Preemptive(struct node* header, string output){
        struct node* notReady = arrival(header);
        struct node* Ready = NULL;
        struct ResultsNode* results = NULL;
        struct node* processExecution = NULL;
        int time = 0;
        int finishedProcesses= 0;
        int numberOfProcess=count(header);

        while(finishedProcesses < numberOfProcess){
                moveListPriority_Preemptive(&Ready, &notReady, time);

                if(processExecution != NULL && Ready != NULL){
                        if(Ready->processInfo.Priority < processExecution->processInfo.Priority){
                                insertPriorityOrdered(&Ready, processExecution);
                                processExecution = NULL;
                        }
                }

                if(processExecution== NULL && Ready != NULL){
                        processExecution = removeHead(&Ready);
                }


                if(processExecution!= NULL){
                        time++;
                        processExecution->processInfo.Remaing_time--;

                        if(processExecution->processInfo.Remaing_time == 0){
                                int waitingTime = time - processExecution->processInfo.arrivalTime - processExecution->processInfo.burstTime;
                                insertResultBack(&results, processExecution->processInfo.ProcessID, waitingTime);
                                delete processExecution;
                                processExecution = NULL;
                                finishedProcesses++;
                        }
                }
                else {
                        if(notReady != NULL) {
                                time = notReady->processInfo.arrivalTime;
                        }
                }
        }

        results = processidOrder(results);
        int sum = sumResults(results);
        double avg = (double)sum / numberOfProcess;
        printResults(5,results,avg,output);

        if(processExecution != NULL) delete processExecution;
        freeProcess(&notReady);
        freeProcess(&Ready);
        freeResults(&results);
}


void RR(struct node* header, int quantum, string output){
        struct node* notReady = arrival(header);
        struct node* Ready = NULL;
        struct ResultsNode* results = NULL;
        struct node* processExecution = NULL;
        int time = 0;
        int finishedProcesses = 0;
        int numberOfProcesses = count(header);

        while(finishedProcesses < numberOfProcesses){
                if(processExecution == NULL && Ready == NULL && notReady != NULL && notReady->processInfo.arrivalTime > time){
                        time = notReady->processInfo.arrivalTime;
                }

                moveListRR(&Ready, &notReady, time);

                if(processExecution == NULL && Ready != NULL){
                        processExecution = removeHead(&Ready);
                }

                if(processExecution != NULL){
                        int executionTime = (processExecution->processInfo.Remaing_time < quantum) ? processExecution->processInfo.Remaing_time : quantum;
                        time += executionTime;
                        processExecution->processInfo.Remaing_time -= executionTime;

                        moveListRR(&Ready, &notReady, time);

                        if(processExecution->processInfo.Remaing_time == 0){
                                int waitingTime = time - processExecution->processInfo.arrivalTime - processExecution->processInfo.burstTime;
                                insertResultBack(&results, processExecution->processInfo.ProcessID, waitingTime);
                                delete processExecution;
                                processExecution = NULL;
                                finishedProcesses++;
                        } else {
                                insertBackRR(&Ready, processExecution);
                                processExecution = NULL;
                        }
                }
        }

        results = processidOrder(results);
        int sum = sumResults(results);
        double avg = (double)sum / numberOfProcesses;
        printResults(6, results, avg, output);

        if(processExecution != NULL) delete processExecution;
        freeProcess(&notReady);
        freeProcess(&Ready);
        freeResults(&results);
}
int main(int argc, char *argv[])
{
        string input;
        int quantumValue=0;
        string output;  bool flagT=false;
        bool flagF= false;
        bool flagO = false;
        if(argc < 7 ){
                cout<<"Use : ./cmpe351 -t <value> -f <inputfile> -o <outputfile>"<<endl;
        }
        else{
                for(int i =0 ; i< argc;i++){
                        if(strcmp(argv[i],"-t")==0){
                           quantumValue= atoi(argv[i+1]);
                           if(quantumValue < 1 || quantumValue > 255){
                                 cout<<"Insert a valide quantum value (1-255);"<<endl;
                                 exit(1);
                                 i++;
                           }
                           flagT=true;
                        }
                        else if(strcmp(argv[i],"-f")==0){
                                input = argv[i+1];
                                i++;
                                flagF= true;
                        }
                        else if(strcmp(argv[i],"-o")==0){
                                output = argv[i+1];
                                i++;
                                flagO=true;
                        }
                }
                struct node *header = NULL;
                header = readFile(header,input);
                FCFS(header, output);
                SJF(header,output);
                SJF_Preemptive(header,output);
                Priority(header,output);
                Priority_Preemptive(header,output);
                RR(header,quantumValue,output);
        }
        return 0;

}
