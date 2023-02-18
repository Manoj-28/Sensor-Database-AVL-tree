#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define RECORDS 30
#define DAYS 365

char sensor_arr[4][20] = {"temperature","sound","humidity","wind"};	//please enter the following sensors type only

typedef struct sensor_tag
{
	int sensor_id;				//sensor id is used as key
	char sensor_type[20];
	char sensor_loc[20];
	int duration;               //Duration in mins	
}sensor_node;

//structure which stores info about various sensors
typedef struct AVL_tree_tag
{
	sensor_node sensor;
	int height;	
	struct AVL_tree_tag* left;
	struct AVL_tree_tag* right;  
}tree_node;

//structure for storing dates and data obtained on that date
typedef struct date_tag
{
	int day;
	int month;
	int year;
	float Time[RECORDS];			 
	float data[RECORDS];			
}Date;

//structure for central repository
typedef struct record_tree_tag
{
	int sensor_ID;
	char sensor_type[20];
	Date d[DAYS];
	int height;
	struct record_tree_tag* left;
	struct record_tree_tag* right;		
}record;

//structure to store AQI index and other entities
typedef struct AQI_record_tag           
{
	int PM10;
	int PM10_time;         //time interval in hours
	int PM25;
	int PM25_time;
	int Ozone_level;
	int OL_time;
	
	int day;
	int month;
	int year;
	
	int final_AQI;	
	
}AQI_info; 

//structure for AQI sensors and other info
 typedef struct Air_Quality_Index
{
	int sensor_id;
	AQI_info A[DAYS];
	char sensor_loc[20];
	struct Air_Quality_Index* left;
	struct Air_Quality_Index* right;
}AQI;


// Max value
int max(int a,int b)
{
	if(a>b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

//Node Height 
int Height(tree_node* H)
{
	if(H==NULL)
	{
		return 0;
	}
	else
	{
		return H->height;
	}	
}
// Record Height
int height(record* H)
{
	if(H==NULL)
	{
		return 0;
	}
	else
	{
		return H->height;
	}	
}

//Balance Factor
int Balance(tree_node* B)
{
	int bal_fac;
	if(B==NULL)
	{
		bal_fac=0;
	}
	else
	{
		bal_fac = Height(B->left)- Height(B->right); 
	}
	return bal_fac;
}
//REcord balance
int balance(record* B)
{
	int bal_fac;
	if(B==NULL)
	{
		bal_fac=0;
	}
	else
	{
		bal_fac = height(B->left)- height(B->right); 
	}
	return bal_fac;
}

//Right Rotation
tree_node* RightRotate(tree_node* N)
{
	tree_node *x;
	tree_node *y;
	
	x= N->left;
	y= x->right;

	x->right = N;
	N->left = y;

	N->height = max(Height(N->left), Height(N->right)) + 1;
	x->height = max(Height(x->left), Height(x->right)) + 1;

  return x;
}

//Left Rotation
tree_node* LeftRotate(tree_node* N)
{
	tree_node *x;
	tree_node *y;
	
	x= N->right;
	y= x->left;

	x->left = N;
	N->right = y;

	N->height = max(Height(N->left), Height(N->right)) + 1;
	x->height = max(Height(x->left), Height(x->right)) + 1;

  return x;
}

// Rotate Record right
record* RotateRight(record* N)
{
	record *x;
	record *y;
	
	x= N->left;
	y= x->right;

	x->right = N;
	N->left = y;

	N->height = max(height(N->left), height(N->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

// Rotate record left
record* RotateLeft(record* N)
{
	record *x;
	record *y;
	
	x= N->right;
	y= x->left;

	x->left = N;
	N->right = y;

	N->height = max(height(N->left), height(N->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;

  return x;
}

// Creating a tree node
tree_node* create_node(int id)
{
	tree_node* new_node;
	new_node = (tree_node*)malloc(sizeof(tree_node));
	
	new_node->sensor.sensor_id =id;
	
	printf("Enter sensor type: ");
	scanf("%s",new_node->sensor.sensor_type);
		
	printf("Enter sensor location: ");
	scanf("%s",new_node->sensor.sensor_loc);
		
	printf("Enter time duration(in mins): ");
	scanf("%d",&new_node->sensor.duration);
	
	new_node->height=1;
	new_node->left= NULL;
	new_node->right = NULL;
	
	return new_node;
}


tree_node* create_sensor_network(tree_node* node, int id)						//replace node name
{
	int bal_fac;
	if (node == NULL)
	{
		node= create_node(id);
    	return node;
    }
	
	if (id < node->sensor.sensor_id)
	{
		node->left = create_sensor_network(node->left,id); 	
	}
 
	else if (id > node->sensor.sensor_id)
	{
		node->right = create_sensor_network(node->right,id);  	
	}
	else
	{
		printf("Sensor id already exists\n");
		return node; 	
	}
	
	tree_node* left_node =node->left;
	tree_node* right_node =node->right;

	node->height = 1 + max(Height(left_node),Height(right_node));

	bal_fac = Balance(node);
	if (bal_fac > 1 && id < left_node->sensor.sensor_id)
    return RightRotate(node);

  if (bal_fac < -1 && id > right_node->sensor.sensor_id)
    return LeftRotate(node);

  if (bal_fac > 1 && id > left_node->sensor.sensor_id) {
    node->left = LeftRotate(left_node);
    return RightRotate(node);
  }

  if (bal_fac < -1 && id < right_node->sensor.sensor_id) {
    node->right = RightRotate(right_node);
    return LeftRotate(node);
  }

  return node;
}

int found=0;				//global variable for finding sensor type

//search function
void search_sensor(tree_node* node,char type[])
{
	if(node ==NULL)
	{
		return;
	}
	else if(strcmp(node->sensor.sensor_type, type)==0)
	{
		found=1;
		return;
	}
	else if(found ==0)
	{
		search_sensor(node->left, type);
		search_sensor(node->right,type);
	}
}

//function to add sensor of specific type if not found
tree_node* install_new_sensor(tree_node* root)
{
	int i,id;
	char type[20];
	for(i=0;i<4;i++)
	{
		strcpy(type,sensor_arr[i]);
		search_sensor(root,type);
		if(found==0)
		{
			printf("No sensor of type %s\n", type);
			printf("Install %s sensor\n",type);
			printf("Enter sensor id\n");
			scanf("%d",&id);
			root= create_sensor_network(root,id);
		}
		else
		{
			found=0;
		}
	}
	return root;
}

// Central Repository
record* central_repo(tree_node* root)
{
	int i,j;
	record* temp_repo= NULL;
	if(root == NULL)
	{
		return temp_repo;
	}
	temp_repo = (record*)malloc(sizeof(record));
	temp_repo->sensor_ID = root->sensor.sensor_id;
	strcpy(temp_repo->sensor_type, root->sensor.sensor_type);
	
	printf("Sensor ID: %d",temp_repo->sensor_ID);
	printf("Sensor Type: %s",temp_repo->sensor_type);

	for(i=0;i<DAYS;i++)
	{
		printf("Enter date(DD MM YY): ");
		scanf("%d %d %d",&temp_repo->d[i].day,&temp_repo->d[i].month,&temp_repo->d[i].year);
		
		printf("Enter time and data\n");
		for(j=0;j<RECORDS;j++)
		{
			scanf("%f",&temp_repo->d[i].Time[j]);			
			scanf("%f",&temp_repo->d[i].data[j]);
		}
	}
	
	
	temp_repo->left=central_repo(root->left);
	temp_repo->right=central_repo(root->right);
	
	return temp_repo;
}

void print_type1_data(record* retrieve,char type[20],int dd,int mm,int yy)
{
	if(retrieve==NULL) return;
	int i=0,j,found=0;
		
	if(strcmp(retrieve->sensor_type,type)==0)
	{
		while(found==0 && i<DAYS)
		{
			for(j=0;j<RECORDS;j++)
			{
				printf("%f ",retrieve->d[i].data[j]);							
			}
			printf("\n");
			if((yy== retrieve->d[i].year) && (mm==retrieve->d[i].month) && (dd==retrieve->d[i].day))
			{
				found=1;
			}
			i++;
		}
	}
	
	print_type1_data(retrieve->left,type,dd,mm,yy);
	print_type1_data(retrieve->right,type,dd,mm,yy);
}

void print_type2_data(record* retrieve,char type[20],int dd1,int mm1,int yy1,int dd2,int mm2,int yy2)
{
	if(retrieve==NULL) return;
	int i=0,j,found=0;
		
	if(strcmp(retrieve->sensor_type,type)==0)
	{
		while((yy1 != retrieve->d[i].year) && (mm1 !=retrieve->d[i].month) && (dd1 !=retrieve->d[i].day))
		{
			i++;
		}
		while(( yy2 <= retrieve->d[i].year) && (mm2 <=retrieve->d[i].month) && (dd2 <=retrieve->d[i].day))
		{
			for(j=0;j<RECORDS && (found==0);j++)
			{
				printf("%f ",retrieve->d[i].data[j]);
				found=1;
			}
			i++;
		}
		
	}
	
	print_type2_data(retrieve->left,type,dd1,mm1,yy1,dd2,mm2,yy2);
	print_type2_data(retrieve->right,type,dd1,mm1,yy1,dd2,mm2,yy2);
}

record* minVal(record* root)
{
	if(root == NULL) return NULL;
	else if(root->left == NULL) return root;
	return minVal(root->left);
}

record* deleteNode(record* root, int key)
{
	if(root == NULL) return NULL;
	else if(root->sensor_ID > key){
		root->left = deleteNode(root->left, key);
	}
	else if(root->sensor_ID < key){
		root->right = deleteNode(root->right, key);
	}
	else{
		if(root->left == NULL || root->right == NULL){
			record* temp = (root->left? root->left:root->right);
			if(temp == NULL){
				temp = root;
				root = NULL;
			}
			else{
				//copying contents 
				*root = *temp;
			}
			free(temp);
		}
		else{
			// delete by balancing : copying contents 
			record* minNode = minVal(root->right);
			*root= *minNode;
			
			root->right = deleteNode(root->right, minNode->sensor_ID);
		}
	}
	if(root == NULL) return NULL;
	root->height = 1 + max(height(root->left),height(root->right));

	int bal_fac = balance(root);
	if (bal_fac > 1 && balance(root->left) >= 0)
		return RotateRight(root);

	if (bal_fac > 1 && balance(root->left) < 0)
	{
		root->left = RotateLeft(root->left);
		return RotateRight(root);
	}

	if (bal_fac < -1 && balance(root->right) <= 0)
		return RotateLeft(root);

	if (bal_fac < -1 && balance(root->right) > 0)
	{
		root->right = RotateRight(root->right);
		return RotateLeft(root);
	}

	return root;	
}

record* idle_sensors(record* root,record* node)
{
	int i,gap,flag=1,key;
	if(node==NULL) return node;
	for(i=0;i<DAYS-1 && (flag==1);i++)
	{
		gap = node->d[i+1].month - node->d[i].month;			//assume d[i+1].month > d[i].month
		
		if(gap> 2)
		{
			printf("Sensor id %d is idle for more than 2 months\n",node->sensor_ID);
			key= node->sensor_ID;
			root = deleteNode(root, key);	
		}
	}
	
	idle_sensors(root,node->left);
	idle_sensors(root,node->right);
	return node;
}




void retrieve_info(record* retrieve)
{
	int i=0,j,info,day2,day3,month2,month3,year2,year3,found=0;
	float time2;
	char type[20];
	printf("Enter Method\n");
	printf("press 1 if depending on sensor type (retrieves till date data)\n");
	printf("press 2 if depending on specified date range (multiple dates) for specific sensor type\n");

	scanf("%d",&info);
	
	switch(info)
	{
		case 1:
			{
				printf("Enter sensor type: ");
				scanf("%s",type);
				printf("Enter date\n");
				scanf("%d %d %d",&day2,&month2,&year2);
				
				print_type1_data(retrieve,type,day2,month2,year2);
				break;
			}
		case 2:
			{
				printf("Enter sensor type\n");
				scanf("%s",type);
				printf("Enter starting date\n");
				scanf("%d %d %d",&day2,&month2,&year2);
				printf("Enter ending date\n");
				scanf("%d %d %d",&day3,&month3,&year3);
				
				print_type2_data(retrieve,type,day2,month2,year2,day3,month3,year3);
				break;
			}
		default:
			{
				printf("Please Enter correct input\n");
				break;
			}
	}
}

void print_sensor_details(tree_node* node)
{
	printf("Sensor Id: %d",node->sensor.sensor_id);
	printf("Sensor type: %s", node->sensor.sensor_type);
	printf("Sensor location: %s",node->sensor.sensor_loc);
	printf("Sensor duration: %d",node->sensor.duration);
}

void sensors_in_between(tree_node* node, int low_value,int high_value)
{
	if(node== NULL) return;
	if((node->sensor.sensor_id >= low_value )&& (node->sensor.sensor_id <= high_value))
	{
		print_sensor_details(node);
	}
	sensors_in_between(node->left, low_value,high_value);
	sensors_in_between(node->right, low_value,high_value);	
}

int max_value=0;			 //global variable to store max AQI
AQI* max_node=NULL;  //global variable to point to node with max AQI
int arr_no=0;


void search_max_AQI(AQI* root)
{
	if(root==NULL) return;
	int i;
	
	for(i=0;i<DAYS;i++)
	{
		if(max_value < root->A[i].final_AQI)
		{
			max_value = root->A[i].final_AQI;
			max_node =root;
			arr_no=i;
		}
	}
	search_max_AQI(root->left);
	search_max_AQI(root->right);
	
	return;
}

void print_max_AQI(AQI* root)
{
	search_max_AQI(root);
	printf("In %d-%d maximum AQI was recorded",root->A[arr_no].month,root->A[arr_no].year);
}

void check_status(AQI* root)
{
	if(root==NULL) return;
	int i;
	for(i=0;i<DAYS;i++)
	{
		if(root->A[i].final_AQI >= 501)
		{
			printf("%d %d %d\n",root->A[i].day,root->A[i].month,root->A[i].year);	
		}	
	}
	check_status(root->left);
	check_status(root->right);
	return;	
}

int main()
{
	int id,low_value,high_value,done=0,info;
	tree_node* root1;
	record* root2;
	AQI* root3;
	

	
	while(done!=1)
	{
		printf("\nEnter 1 to add sensor\n");
		printf("Enter 2 to install new sensor(if required)\n");
		printf("Enter 3 to remove idle sensors\n");
		printf("Enter 4 to retrieve info\n");
		printf("Enter 5 to print sensors in between\n");
		printf("Enter 6 to print max AQI\n");
		printf("Enter 7 to check status\n");
		printf("Enter 0 to quit the program\n");
		scanf("%d",&info);
		switch(info)
		{
			case 0:
				{
					printf("Program ends\n");
					done=1;
					break;
				}
			case 1:
				{
					printf("Enter sensor id no.\n");
					scanf("%d",&id);
					root1 =create_sensor_network(root1,id);
					break;
				}
			case 2:
				{
					root1=install_new_sensor(root1);
					break;
				}
			case 3:
				{
					root2=idle_sensors(root2,root2);
					break;
				}
			case 4:
				{
					retrieve_info(root2);
					break;
				}
			case 5:
				{
					printf("Enter sensor range\n");
					scanf("%d",&low_value);
					printf(" to ");
					scanf("%d",&high_value);
					sensors_in_between(root1,low_value,high_value);
				}
			case 6:
				{
					search_max_AQI(root3);
				}
			case 7:
				{
					check_status(root3);
					break;					
				}
			default:
				{
					printf("Please Enter correct no.\n");
					break;
				}
		}		
	}

}
