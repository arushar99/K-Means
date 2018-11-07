#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>



void initial_centers(int dim, int ndata, float data[], int K, float **centroids);
void k_means(int dim, int ndata, float data[], 
            int K, float **centroids,int *cluster_size, 
            int *cluster_start, float *cluster_radius);
int search_k_means(int dim, int ndata, float data[], int K, int *cluster_start, int *cluster_size, 
                   float *cluster_radius, float **cluster_centroids, float *query, float *result_pt);
int main(void){
	int dim= 6;
	
	int ndata =20;
	float data[dim*ndata];
    	int K =6;
	srand(1);

	for(int i = 0; i<ndata*dim; i++){
        	data[i] = (rand() % (50 - 0 + 1)) + 0;
	}
	

	float **centroids = (float **) (malloc(sizeof(float *)*K));
	int *cluster_start = (int *) malloc(sizeof(int)*K);
	int *cluster_size = (int *) malloc(sizeof(int)*K);
	float *cluster_radius = (float *) malloc(sizeof(float)*K);
	float *query = (float *) malloc(sizeof(float)*dim);
    for(int i = 0; i<K; i++){
    	centroids[i] = (float *) malloc(sizeof(float)*dim);
    }
	
	initial_centers(dim,ndata,data,K,centroids);
	
	for(int i =0 ;i< K ;i++){
		for(int j=0; j< dim ; j++){
		  printf("%.2f ", centroids[i][j]);
		}
		printf("\n");
	}

	 k_means(dim, ndata, data, K, centroids, cluster_size, cluster_start, cluster_radius);

	for(int i =0 ;i< K ;i++){
		for(int j=0; j< dim ; j++){
		  printf("%.2f ", centroids[i][j]);
		}
		printf("\n");
	}
			

	 printf("\n");
    printf("New cluster start: \n");
    
    for(int i = 0; i<K; i++){        
        printf("%d ", cluster_start[i]);
    }
    printf("\n");
    printf("New cluster size: \n");
    
     for(int i = 0; i<K; i++){
        
        printf("%d ", cluster_size[i]);
        
    }
    printf("\n");

    printf("New cluster centroids: \n");
    
     for(int i = 0; i<K; i++){
        
       for(int j = 0; j<dim; j++){
            printf("%.2f ", centroids[i][j]);
       }

       printf("\n");
        
    }

    printf("\n");
    printf("Cluster radius: \n");
    
     for(int i = 0; i<K; i++){
        
        printf("%.2f ", cluster_radius[i]);
        
    }
    printf("\n");

    //Free memory
	free(centroids);
	free(cluster_size);
	free(cluster_start);
	
	free(cluster_radius);
	//free(result_pt);



    return 0;

}


void initial_centers(int dim, int ndata, float data[], int K, float **centroids){
	float temp_array[ndata]; // to copy the distance between the 2 centers ; used this at the end to compare
	int temp;
	float distance;
	float max;
	int counter;
	int counter2;
	float min;


	printf("hi");

	for (int i=0; i< K;i++){
			
		if (i== 0){
			counter = 0;
			while(counter< dim){
				centroids[i][counter]= data[counter];
				counter++;
			}
			
		}
		else if( i == 1){
			temp = 0; //intialize the max;
			
			for(int j = 0 ; j < ndata * dim; j+=dim){
				distance= 0;
				// calculate the distance;
				counter= j;
				counter2= 0;
				while(counter2 < dim){

					distance+= (data[counter] - centroids[0][counter2]) * (data[counter] - centroids[0][counter2]);
					counter2++;counter++;
				}
				
				
				distance = sqrt(distance);
				//printf("%.2f \n", distance);
				if(temp == 0){
					counter = j;
					max = distance;
					counter2= 0;
					while(counter2< dim){
						centroids[i][counter2]=data[counter];
						counter2++;counter++;
					}
					temp++;
				}
				else if(max < distance){
					
					counter = j;
					counter2 = 0;
					
					max = distance;
					//printf("%.2f \n", max);
					while(counter2< dim){
						centroids[i][counter2]=data[counter];
						//printf("%.2f ",data[counter]);
						counter2++;counter++;
					}
					printf("\n");
					
				}
			
			}	
				
		}

		else if (i >= 2){
			
			
			
			for(int j=0;j< ndata ; j+=dim){
				
				for(int k=0; k < i; k++){
					counter=j;
					distance= 0;
					counter2=0;
					while(counter2 < dim){
						distance += (data[counter] - centroids[k][counter2]) * (data[counter] - centroids[k][counter2]);
						counter++;counter2++;
					}
					distance = sqrt(distance);
					if(k == 0){
						min = distance;
						temp_array[j] = min;
						
					}
					else if(min > distance){
						min=distance;
						temp_array[j] = min;
						
					}
					
					
					
					
				}
	
			}
		// find the max of the mins
			
			for(int j = 0 ; j< ndata; j++){
				
				if (j == 0){
					counter2= 0;
					counter= j;
					max= temp_array[j];
					while(counter2 < dim){
						centroids[i][counter2] = data[counter];
						counter2++;counter++;
					}
				}
				else{

					if ( max < temp_array[j]){
						counter2= 0;
						counter= j;
						max= temp_array[j];
						while(counter2 < dim){
							centroids[i][counter2] = data[counter];
							counter2++;counter++;
						}
					}
				}
			}
		}
	 } 
}
     






void k_means(int dim, int ndata, float data[], 
            int K, float **centroids,int *cluster_size, 
            int *cluster_start, float *cluster_radius){
	
	

	int counter;
	int counter2;
	int counter3 = 0;
	float distance;
	int cluster_assignment[ndata];
	float min_distance;
	float mean[dim];
	float prev_val[K][dim];
	while(true){
		for(int i= 0 ; i < K ; i++){
			for(int j=0; j< dim; j++){
			 	prev_val[i][j]=centroids[i][j];
			}
		}

		for(int i=0; i< ndata; i++){
				
			for(int j = 0; j < K; j++){
				distance = 0;
				counter = i;
				counter2= 0;
				while(counter2 < dim){
					distance +=  (data[counter] - centroids[j][counter2]) * (data[counter] - centroids[j][counter2]);
					counter2++;counter++;
				}
				distance = sqrt(distance);
				if(j == 0){
					min_distance = distance;
					cluster_assignment[counter3] = j;
					
				}
				else if (min_distance > distance){
					min_distance=  distance;
					cluster_assignment[counter3] = j;
					
				}
				
			}
			counter3++;
		}
		
	
		// calculate the new mean 
		
		for(int i =0; i <K ; i++){


			for (int j=0; j< dim;j++){
				mean[j]=0;
			}
			counter= 0;
			for(int j= 0 ; j< ndata ; j++){
				
				if(cluster_assignment[j] == i){
					counter++;
					
					counter2=0;
					counter3= j;
					while( counter2 < dim){
						mean[counter2]+= data[counter3];
						counter2++;counter3++;
					}
				}
				cluster_size[i] = counter;
			}
			if (counter != 0){

				for (int j = 0; j<dim; j++){
					mean[j] = mean[j]/counter;
				}
				for(int j = 0 ;j<dim;j++){
					centroids[i][j]= mean[j];
				}
			}
		}


		counter=0;

		for(int i= 0; i < K;i++){

			for(int j=0; j<dim ; j++){

				if(centroids[i][j] !=prev_val[i][j]){
					counter++;
				}
			}
			
		}
		if(counter>0){
			break;
		}

		
	}
		
		//sort the clusters 

		
		int l;int c;int temp_sort;int temp_sort_point;int temp;
		for(int i = 0; i < (ndata*dim); i+=dim){
            		for(int j = 0; j < (ndata*dim); j+=dim){

                		if(cluster_assignment[(int)(i/dim)] < cluster_assignment[(int)(j/dim)]){
                        
                    			l = i;
                    			c = j;

                    			temp_sort = cluster_assignment[(int)(i/dim)];
                    			cluster_assignment[(int)(i/dim)] = cluster_assignment[(int)(j/dim)];
                    			cluster_assignment [(int)(j/dim)] = temp_sort;

                    			for(int k = 0; k < dim; k++){

                    				temp_sort_point = data[c];
                        			data[c] = data[l];
                        			data[l] = temp_sort_point;

                        			l++;
                        			c++;
                    			}
                		}
            		}
        	}

		printf("\ncluster assign\n");
		
		for(int i= 0 ;i < ndata; i++){
			printf("%d ", cluster_assignment[i]);
		}
		printf("\n");

		//find the cluster_Start

		cluster_start[0] = 0;
		for(int i = 1; i<K; i++){
			if(cluster_size[i]!= 0){
				cluster_start[i] = (cluster_start[i-1]) + (cluster_size[i-1]*dim);
			}
			else{
				cluster_start[i] = -1;
			}        
		}	

		// get the cluster_radius
		
		float max_dis;
		for(int i= 0 ; i< K; i++){
			temp=0;
			max_dis=0;
			if(cluster_size[i] != 0){
				for(int j=cluster_start[i];j<cluster_start[i]+cluster_size[i];j++){
					counter=0;
					distance= 0 ;
					counter2 = j;
					
					while(counter < dim){
						distance+= (data[counter2] - centroids[i][counter]) * (data[counter2] - centroids[i][counter]);
						counter++;counter2++;
					}
					distance = sqrt(distance);
					if(temp == 0){
						max_dis= distance;
						temp++;	
						cluster_radius[i]= max_dis;
					}
					else if (max_dis< distance){
						max_dis = distance;
						cluster_radius[i]= max_dis;
					}
				}
			}
		}
			
		
 
}









   
